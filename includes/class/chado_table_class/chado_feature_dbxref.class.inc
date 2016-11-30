<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURE_DBXREF class.
 *
 */
class CHADO_FEATURE_DBXREF  {

  /**
   *  Data members for CHADO_FEATURE_DBXREF.
   */
  protected $member_arr        = NULL;
  protected $feature_dbxref_id = NULL;
  protected $feature_id        = NULL;
  protected $dbxref_id         = NULL;
  protected $is_current        = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'feature_dbxref_id' => 'serial',
    'feature_id'        => 'integer',
    'dbxref_id'         => 'integer',
    'is_current'        => 'boolean',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->feature_dbxref_id = array_key_exists('feature_dbxref_id', $details) ? $details['feature_dbxref_id'] : '';
    $this->feature_id        = array_key_exists('feature_id', $details)        ? $details['feature_id']        : '';
    $this->dbxref_id         = array_key_exists('dbxref_id', $details)         ? $details['dbxref_id']         : '';
    $this->is_current        = array_key_exists('is_current', $details)        ? $details['is_current']        : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURE_DBXREF by key(s).
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
    $sql = "SELECT * FROM chado.feature_dbxref WHERE " . implode(" AND " , $where_arr);
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
        $fields['feature_id'] = $this->feature_id;
        $fields['dbxref_id']  = $this->dbxref_id;
        $fields['is_current'] = (preg_match("/^(true|false)/i", $this->is_current)) ? "'" . $this->is_current . "'" : 'true';


        // Inserts the record.
        $this->feature_dbxref_id = db_insert('chado.feature_dbxref')
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
      $fields['feature_id'] = $this->feature_id;
      $fields['dbxref_id']  = $this->dbxref_id;
      $fields['is_current'] = (preg_match("/^(true|false)/i", $this->is_current)) ? "'" . $this->is_current . "'" : 'true';


      // Updates the record.
      db_update('chado.feature_dbxref')
        ->fields($fields)
        ->condition('feature_dbxref_id', $this->feature_dbxref_id, '=')
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

      db_delete('chado.feature_dbxref')
        ->condition('feature_dbxref_id', $this->feature_dbxref_id, '=')
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
   * Retrieves the feature_dbxref_id.
   *
   * @retrun serial
   */
  public function getFeatureDbxrefID() {
    return $this->feature_dbxref_id;
  }

  /**
   * Updates the feature_dbxref_id.
   *
   * @param serial $feature_dbxref_id
   */
  public function setFeatureDbxrefID($feature_dbxref_id) {
    $this->feature_dbxref_id = $feature_dbxref_id;
  }

  /**
   * Retrieves the feature_id.
   *
   * @retrun integer
   */
  public function getFeatureID() {
    return $this->feature_id;
  }

  /**
   * Updates the feature_id.
   *
   * @param integer $feature_id
   */
  public function setFeatureID($feature_id) {
    $this->feature_id = $feature_id;
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
   * Retrieves the is_current.
   *
   * @retrun boolean
   */
  public function getIsCurrent() {
    return $this->is_current;
  }

  /**
   * Updates the is_current.
   *
   * @param boolean $is_current
   */
  public function setIsCurrent($is_current) {
    $this->is_current = $is_current;
  }
}