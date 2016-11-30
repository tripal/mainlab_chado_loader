<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURE_SYNONYM class.
 *
 */
class CHADO_FEATURE_SYNONYM  {

  /**
   *  Data members for CHADO_FEATURE_SYNONYM.
   */
  protected $member_arr         = NULL;
  protected $feature_synonym_id = NULL;
  protected $synonym_id         = NULL;
  protected $feature_id         = NULL;
  protected $pub_id             = NULL;
  protected $is_current         = NULL;
  protected $is_internal        = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'feature_synonym_id' => 'serial',
    'synonym_id'         => 'integer',
    'feature_id'         => 'integer',
    'pub_id'             => 'integer',
    'is_current'         => 'boolean',
    'is_internal'        => 'boolean',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->feature_synonym_id = array_key_exists('feature_synonym_id', $details) ? $details['feature_synonym_id'] : '';
    $this->synonym_id         = array_key_exists('synonym_id', $details)         ? $details['synonym_id']         : '';
    $this->feature_id         = array_key_exists('feature_id', $details)         ? $details['feature_id']         : '';
    $this->pub_id             = array_key_exists('pub_id', $details)             ? $details['pub_id']             : '';
    $this->is_current         = array_key_exists('is_current', $details)         ? $details['is_current']         : '';
    $this->is_internal        = array_key_exists('is_internal', $details)        ? $details['is_internal']        : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURE_SYNONYM by key(s).
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
    $sql = "SELECT * FROM chado.feature_synonym WHERE " . implode(" AND " , $where_arr);
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
        $fields['synonym_id']  = $this->synonym_id;
        $fields['feature_id']  = $this->feature_id;
        $fields['pub_id']      = $this->pub_id;
        $fields['is_current']  = (preg_match("/^(true|false)/i", $this->is_current))  ? "'" . $this->is_current . "'"  : 'false';
        $fields['is_internal'] = (preg_match("/^(true|false)/i", $this->is_internal)) ? "'" . $this->is_internal . "'" : 'false';


        // Inserts the record.
        $this->feature_synonym_id = db_insert('chado.feature_synonym')
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
      $fields['synonym_id']  = $this->synonym_id;
      $fields['feature_id']  = $this->feature_id;
      $fields['pub_id']      = $this->pub_id;
      $fields['is_current']  = (preg_match("/^(true|false)/i", $this->is_current))  ? "'" . $this->is_current . "'"  : 'false';
      $fields['is_internal'] = (preg_match("/^(true|false)/i", $this->is_internal)) ? "'" . $this->is_internal . "'" : 'false';


      // Updates the record.
      db_update('chado.feature_synonym')
        ->fields($fields)
        ->condition('feature_synonym_id', $this->feature_synonym_id, '=')
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

      db_delete('chado.feature_synonym')
        ->condition('feature_synonym_id', $this->feature_synonym_id, '=')
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
   * Retrieves the feature_synonym_id.
   *
   * @retrun serial
   */
  public function getFeatureSynonymID() {
    return $this->feature_synonym_id;
  }

  /**
   * Updates the feature_synonym_id.
   *
   * @param serial $feature_synonym_id
   */
  public function setFeatureSynonymID($feature_synonym_id) {
    $this->feature_synonym_id = $feature_synonym_id;
  }

  /**
   * Retrieves the synonym_id.
   *
   * @retrun integer
   */
  public function getSynonymID() {
    return $this->synonym_id;
  }

  /**
   * Updates the synonym_id.
   *
   * @param integer $synonym_id
   */
  public function setSynonymID($synonym_id) {
    $this->synonym_id = $synonym_id;
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
   * Retrieves the pub_id.
   *
   * @retrun integer
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Updates the pub_id.
   *
   * @param integer $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
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

  /**
   * Retrieves the is_internal.
   *
   * @retrun boolean
   */
  public function getIsInternal() {
    return $this->is_internal;
  }

  /**
   * Updates the is_internal.
   *
   * @param boolean $is_internal
   */
  public function setIsInternal($is_internal) {
    $this->is_internal = $is_internal;
  }
}