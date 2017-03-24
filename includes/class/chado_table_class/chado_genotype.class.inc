<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_GENOTYPE class.
 *
 */
class CHADO_GENOTYPE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_GENOTYPE.
   */
  protected $member_arr  = NULL;
  protected $genotype_id = NULL;
  protected $name        = NULL;
  protected $uniquename  = NULL;
  protected $description = NULL;
  protected $type_id     = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'genotype_id' => 'serial',
    'name'        => 'text',
    'uniquename'  => 'text',
    'description' => 'character_varying(255)',
    'type_id'     => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->genotype_id = array_key_exists('genotype_id', $details) ? $details['genotype_id'] : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->uniquename  = array_key_exists('uniquename', $details)  ? $details['uniquename']  : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
    $this->type_id     = array_key_exists('type_id', $details)     ? $details['type_id']     : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_GENOTYPE by key(s).
   */
  public static function byKey($keys) {

    // Returns NULL if non-integer value are given to the data member whose
    // data type is 'integer'.
    $where_arr = array();
    $args      = array();
    foreach ($keys as $key => $value) {

      // Returns NULL if $value is empty string.
      if ($value === '') {
        return NULL;
      }

      // Gets data type.
      $data_type = self::$data_types[$key];

      // Checks the value if data type is 'integer'.
      if (in_array($data_type, array('serial', 'integer'))) {
        if (!is_int((int)$value)) {
          return NULL;
        }
      }

      // Adds LOWER() function to the characters.
      if (preg_match("/^(char|text)/", $data_type)) {
        $where_arr []= " LOWER($key) = LOWER(:$key) ";
      }
      else {
        $where_arr []= " $key = :$key ";
      }
      $args[":$key"] = $keys[$key];
    }

    // Gets the table properties.
    $sql = "SELECT * FROM chado.genotype WHERE " . implode(" AND " , $where_arr);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    if ($details) {
      return new self($details);
    }
    return NULL;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return array(
      array('type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id')),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'genotype_id' => array(
        array('table' => 'feature_genotype', 'attr' => 'genotype_id'),
        array('table' => 'genotype_call', 'attr' => 'genotype_id'),
        array('table' => 'genotypeprop', 'attr' => 'genotype_id'),
        array('table' => 'nd_experiment_genotype', 'attr' => 'genotype_id'),
      ),
    );
  }

  /**
   * Adds a new record.
   */
  public function insert() {

    $transaction = db_transaction();
    try {

      // TODO:Checks for duplication.
      $dup = FALSE;

      // Inserts the record if not duplicated.
      if (!$dup) {

        // Populates all fields. The attribute with 'NOT NULL' would be skipped
        // if the value is empty.
        $fields = array();
        $fields['name']        = $this->name;
        $fields['uniquename']  = $this->uniquename;
        $fields['description'] = $this->description;
        $fields['type_id']     = $this->type_id;


        // Inserts the record.
        $this->genotype_id = db_insert('chado.genotype')
          ->fields($fields)
          ->execute();
      }
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Updates the record.
   */
  public function update() {

    $transaction = db_transaction();
    try {

      // Populates all fields. The attribute with 'NOT NULL' would be skipped
      // if the value is empty.
      $fields = array();
      $fields['name']        = $this->name;
      $fields['uniquename']  = $this->uniquename;
      $fields['description'] = $this->description;
      $fields['type_id']     = $this->type_id;


      // Updates the record.
      db_update('chado.genotype')
        ->fields($fields)
        ->condition('genotype_id', $this->genotype_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Deletes the record.
   */
  public function delete() {

    $transaction = db_transaction();
    try {

      db_delete('chado.genotype')
        ->condition('genotype_id', $this->genotype_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the data type of the member.
   *
   * @retrun various
   */
  public function getDataType($member) {
    return $this->data_types[$member];
  }

  /**
   * Retrieves the data member array.
   *
   * @retrun array
   */
  public function getMemberArr() {
    return $this->member_arr;
  }

  /**
   * Updates the data member array.
   *
   * @param array $member_arr
   */
  public function setMemberArr($member_arr) {
    $this->member_arr = $member_arr;
  }

  /**
   * Retrieves the genotype_id.
   *
   * @retrun serial
   */
  public function getGenotypeID() {
    return $this->genotype_id;
  }

  /**
   * Updates the genotype_id.
   *
   * @param serial $genotype_id
   */
  public function setGenotypeID($genotype_id) {
    $this->genotype_id = $genotype_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun text
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Updates the name.
   *
   * @param text $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the uniquename.
   *
   * @retrun text
   */
  public function getUniquename() {
    return $this->uniquename;
  }

  /**
   * Updates the uniquename.
   *
   * @param text $uniquename
   */
  public function setUniquename($uniquename) {
    $this->uniquename = $uniquename;
  }

  /**
   * Retrieves the description.
   *
   * @retrun character_varying(255)
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Updates the description.
   *
   * @param character_varying(255) $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * Retrieves the type_id.
   *
   * @retrun integer
   */
  public function getTypeID() {
    return $this->type_id;
  }

  /**
   * Updates the type_id.
   *
   * @param integer $type_id
   */
  public function setTypeID($type_id) {
    $this->type_id = $type_id;
  }
}