<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_DATA_VALID class.
 *
 */
class PUBLIC_MCL_DATA_VALID  {

  /**
   *  Data members for PUBLIC_MCL_DATA_VALID.
   */
  protected $member_arr         = NULL;
  protected $data_valid_id      = NULL;
  protected $data_valid_type_id = NULL;
  protected $name               = NULL;
  protected $cvterm_id          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'data_valid_id'      => 'serial',
    'data_valid_type_id' => 'integer',
    'name'               => 'character_varying(255)',
    'cvterm_id'          => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->data_valid_id      = array_key_exists('data_valid_id', $details)      ? $details['data_valid_id']      : '';
    $this->data_valid_type_id = array_key_exists('data_valid_type_id', $details) ? $details['data_valid_type_id'] : '';
    $this->name               = array_key_exists('name', $details)               ? $details['name']               : '';
    $this->cvterm_id          = array_key_exists('cvterm_id', $details)          ? $details['cvterm_id']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_DATA_VALID by key(s).
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
    $sql = "SELECT * FROM mcl_data_valid WHERE " . implode(" AND " , $where_arr);
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
    return NULL;
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
        $fields['data_valid_type_id'] = $this->data_valid_type_id;
        $fields['name']               = $this->name;
        if (is_numeric($this->cvterm_id)) { $fields['cvterm_id'] = $this->cvterm_id; }

        // Inserts the record.
        $this->data_valid_id = db_insert('mcl_data_valid')
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
      $fields['data_valid_type_id'] = $this->data_valid_type_id;
      $fields['name']               = $this->name;
      if (is_numeric($this->cvterm_id)) { $fields['cvterm_id'] = $this->cvterm_id; }

      // Updates the record.
      db_update('mcl_data_valid')
        ->fields($fields)
        ->condition('data_valid_id', $this->data_valid_id, '=')
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

      db_delete('mcl_data_valid')
        ->condition('data_valid_id', $this->data_valid_id, '=')
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
   * Retrieves the data_valid_id.
   *
   * @retrun serial
   */
  public function getDataValidID() {
    return $this->data_valid_id;
  }

  /**
   * Updates the data_valid_id.
   *
   * @param serial $data_valid_id
   */
  public function setDataValidID($data_valid_id) {
    $this->data_valid_id = $data_valid_id;
  }

  /**
   * Retrieves the data_valid_type_id.
   *
   * @retrun integer
   */
  public function getDataValidTypeID() {
    return $this->data_valid_type_id;
  }

  /**
   * Updates the data_valid_type_id.
   *
   * @param integer $data_valid_type_id
   */
  public function setDataValidTypeID($data_valid_type_id) {
    $this->data_valid_type_id = $data_valid_type_id;
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
   * Retrieves the cvterm_id.
   *
   * @retrun integer
   */
  public function getCvtermID() {
    return $this->cvterm_id;
  }

  /**
   * Updates the cvterm_id.
   *
   * @param integer $cvterm_id
   */
  public function setCvtermID($cvterm_id) {
    $this->cvterm_id = $cvterm_id;
  }
}