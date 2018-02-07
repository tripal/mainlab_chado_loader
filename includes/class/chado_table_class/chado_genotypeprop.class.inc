<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_GENOTYPEPROP class.
 *
 */
class CHADO_GENOTYPEPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_GENOTYPEPROP.
   */
  protected $member_arr      = NULL;
  protected $genotypeprop_id = NULL;
  protected $genotype_id     = NULL;
  protected $type_id         = NULL;
  protected $value           = NULL;
  protected $rank            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'genotypeprop_id' => 'serial',
    'genotype_id'     => 'integer',
    'type_id'         => 'integer',
    'value'           => 'text',
    'rank'            => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr      = $details;
    $this->genotypeprop_id = array_key_exists('genotypeprop_id', $details) ? $details['genotypeprop_id'] : '';
    $this->genotype_id     = array_key_exists('genotype_id', $details)     ? $details['genotype_id']     : '';
    $this->type_id         = array_key_exists('type_id', $details)         ? $details['type_id']         : '';
    $this->value           = array_key_exists('value', $details)           ? $details['value']           : '';
    $this->rank            = array_key_exists('rank', $details)            ? $details['rank']            : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_GENOTYPEPROP by key(s).
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
    $sql = "SELECT * FROM chado.genotypeprop WHERE " . implode(" AND " , $where_arr);
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
      array('genotype_id' => array('fk_table' => 'genotype', 'fk_attr' => 'genotype_id')),
      array('type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id')),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return NULL;
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
        $fields['genotype_id'] = $this->genotype_id;
        $fields['type_id']     = $this->type_id;
        $fields['value']       = $this->value;
        $fields['rank']        = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->genotypeprop_id = db_insert('chado.genotypeprop')
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
      $fields['genotype_id'] = $this->genotype_id;
      $fields['type_id']     = $this->type_id;
      $fields['value']       = $this->value;
      $fields['rank']        = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.genotypeprop')
        ->fields($fields)
        ->condition('genotypeprop_id', $this->genotypeprop_id, '=')
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

      db_delete('chado.genotypeprop')
        ->condition('genotypeprop_id', $this->genotypeprop_id, '=')
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
   * Retrieves the genotypeprop_id.
   *
   * @retrun serial
   */
  public function getGenotypepropID() {
    return $this->genotypeprop_id;
  }

  /**
   * Updates the genotypeprop_id.
   *
   * @param serial $genotypeprop_id
   */
  public function setGenotypepropID($genotypeprop_id) {
    $this->genotypeprop_id = $genotypeprop_id;
  }

  /**
   * Retrieves the genotype_id.
   *
   * @retrun integer
   */
  public function getGenotypeID() {
    return $this->genotype_id;
  }

  /**
   * Updates the genotype_id.
   *
   * @param integer $genotype_id
   */
  public function setGenotypeID($genotype_id) {
    $this->genotype_id = $genotype_id;
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

  /**
   * Retrieves the value.
   *
   * @retrun text
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Updates the value.
   *
   * @param text $value
   */
  public function setValue($value) {
    $this->value = $value;
  }

  /**
   * Retrieves the rank.
   *
   * @retrun integer
   */
  public function getRank() {
    return $this->rank;
  }

  /**
   * Updates the rank.
   *
   * @param integer $rank
   */
  public function setRank($rank) {
    $this->rank = $rank;
  }
}