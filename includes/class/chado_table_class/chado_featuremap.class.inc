<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREMAP class.
 *
 */
class CHADO_FEATUREMAP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATUREMAP.
   */
  protected $member_arr    = NULL;
  protected $featuremap_id = NULL;
  protected $name          = NULL;
  protected $description   = NULL;
  protected $unittype_id   = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featuremap_id' => 'serial',
    'name'          => 'character_varying(255)',
    'description'   => 'text',
    'unittype_id'   => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->featuremap_id = array_key_exists('featuremap_id', $details) ? $details['featuremap_id'] : '';
    $this->name          = array_key_exists('name', $details)          ? $details['name']          : '';
    $this->description   = array_key_exists('description', $details)   ? $details['description']   : '';
    $this->unittype_id   = array_key_exists('unittype_id', $details)   ? $details['unittype_id']   : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREMAP by key(s).
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
    $sql = "SELECT * FROM chado.featuremap WHERE " . implode(" AND " , $where_arr);
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
      array('unittype_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id')),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'featuremap_id' => array(
        array('table' => 'featuremap_contact', 'attr' => 'featuremap_id'),
        array('table' => 'featuremap_organism', 'attr' => 'featuremap_id'),
        array('table' => 'featuremap_pub', 'attr' => 'featuremap_id'),
        array('table' => 'featuremap_stock', 'attr' => 'featuremap_id'),
        array('table' => 'featuremapprop', 'attr' => 'featuremap_id'),
        array('table' => 'featurepos', 'attr' => 'featuremap_id'),
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
        $fields['description'] = $this->description;
        if (is_numeric($this->unittype_id)) { $fields['unittype_id'] = $this->unittype_id; }

        // Inserts the record.
        $this->featuremap_id = db_insert('chado.featuremap')
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
      $fields['description'] = $this->description;
      if (is_numeric($this->unittype_id)) { $fields['unittype_id'] = $this->unittype_id; }

      // Updates the record.
      db_update('chado.featuremap')
        ->fields($fields)
        ->condition('featuremap_id', $this->featuremap_id, '=')
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

      db_delete('chado.featuremap')
        ->condition('featuremap_id', $this->featuremap_id, '=')
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
   * Retrieves the featuremap_id.
   *
   * @retrun serial
   */
  public function getFeaturemapID() {
    return $this->featuremap_id;
  }

  /**
   * Updates the featuremap_id.
   *
   * @param serial $featuremap_id
   */
  public function setFeaturemapID($featuremap_id) {
    $this->featuremap_id = $featuremap_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(255)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Updates the name.
   *
   * @param character_varying(255) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the description.
   *
   * @retrun text
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Updates the description.
   *
   * @param text $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * Retrieves the unittype_id.
   *
   * @retrun integer
   */
  public function getUnittypeID() {
    return $this->unittype_id;
  }

  /**
   * Updates the unittype_id.
   *
   * @param integer $unittype_id
   */
  public function setUnittypeID($unittype_id) {
    $this->unittype_id = $unittype_id;
  }
}