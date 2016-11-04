<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_DBXREFPROP class.
 *
 */
class CHADO_DBXREFPROP  {

 /**
  *  Data members for CHADO_DBXREFPROP.
  */
  protected $member_arr    = NULL;
  protected $dbxrefprop_id = NULL;
  protected $dbxref_id     = NULL;
  protected $type_id       = NULL;
  protected $value         = NULL;
  protected $rank          = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'dbxrefprop_id' => 'serial',
    'dbxref_id'     => 'integer',
    'type_id'       => 'integer',
    'value'         => 'text',
    'rank'          => 'integer',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->dbxrefprop_id = array_key_exists('dbxrefprop_id', $details) ? $details['dbxrefprop_id'] : '';
    $this->dbxref_id     = array_key_exists('dbxref_id', $details)     ? $details['dbxref_id']     : '';
    $this->type_id       = array_key_exists('type_id', $details)       ? $details['type_id']       : '';
    $this->value         = array_key_exists('value', $details)         ? $details['value']         : '';
    $this->rank          = array_key_exists('rank', $details)          ? $details['rank']          : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_DBXREFPROP by key(s).
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
    $sql = "SELECT * FROM chado.dbxrefprop WHERE " . implode(" AND " , $where_arr);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    if ($details) {
      return new self($details);
    }
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
        $fields['dbxref_id'] = $this->dbxref_id;
        $fields['type_id']   = $this->type_id;
        $fields['value']     = $this->value;
        $fields['rank']      = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->dbxrefprop_id = db_insert('chado.dbxrefprop')
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
      $fields['dbxref_id'] = $this->dbxref_id;
      $fields['type_id']   = $this->type_id;
      $fields['value']     = $this->value;
      $fields['rank']      = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.dbxrefprop')
        ->fields($fields)
        ->condition('dbxrefprop_id', $this->dbxrefprop_id, '=')
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
   * Delete the record.
   */
  public function delete() {

    $transaction = db_transaction();
    try {

      db_delete('chado.dbxrefprop')
        ->condition('dbxrefprop_id', $this->dbxrefprop_id, '=')
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
   * Retrieves the dbxrefprop_id.
   *
   * @retrun serial
   */
  public function getDbxrefpropID() {
    return $this->dbxrefprop_id;
  }

  /**
   * Updates the dbxrefprop_id.
   *
   * @param serial $dbxrefprop_id
   */
  public function setDbxrefpropID($dbxrefprop_id) {
    $this->dbxrefprop_id = $dbxrefprop_id;
  }

  /**
   * Retrieves the dbxref_id.
   *
   * @retrun integer
   */
  public function getDbxrefID() {
    return $this->dbxref_id;
  }

  /**
   * Updates the dbxref_id.
   *
   * @param integer $dbxref_id
   */
  public function setDbxrefID($dbxref_id) {
    $this->dbxref_id = $dbxref_id;
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