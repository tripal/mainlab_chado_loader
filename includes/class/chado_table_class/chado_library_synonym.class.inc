<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_LIBRARY_SYNONYM class.
 *
 */
class CHADO_LIBRARY_SYNONYM  {

 /**
  *  Data members for CHADO_LIBRARY_SYNONYM.
  */
  protected $member_arr         = NULL;
  protected $library_synonym_id = NULL;
  protected $synonym_id         = NULL;
  protected $library_id         = NULL;
  protected $pub_id             = NULL;
  protected $is_current         = NULL;
  protected $is_internal        = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'library_synonym_id' => 'serial',
    'synonym_id'         => 'integer',
    'library_id'         => 'integer',
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
    $this->library_synonym_id = array_key_exists('library_synonym_id', $details) ? $details['library_synonym_id'] : '';
    $this->synonym_id         = array_key_exists('synonym_id', $details)         ? $details['synonym_id']         : '';
    $this->library_id         = array_key_exists('library_id', $details)         ? $details['library_id']         : '';
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
   * Generate CHADO_LIBRARY_SYNONYM by key(s).
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
    $sql = "SELECT * FROM chado.library_synonym WHERE " . implode(" AND " , $where_arr);
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
        $fields['library_id']  = $this->library_id;
        $fields['pub_id']      = $this->pub_id;
        $fields['is_current']  = (preg_match("/^(true|false)/i", $this->is_current))  ? "'" . $this->is_current . "'"  : 'true';
        $fields['is_internal'] = (preg_match("/^(true|false)/i", $this->is_internal)) ? "'" . $this->is_internal . "'" : 'false';


        // Inserts the record.
        $this->library_synonym_id = db_insert('chado.library_synonym')
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
      $fields['library_id']  = $this->library_id;
      $fields['pub_id']      = $this->pub_id;
      $fields['is_current']  = (preg_match("/^(true|false)/i", $this->is_current))  ? "'" . $this->is_current . "'"  : 'true';
      $fields['is_internal'] = (preg_match("/^(true|false)/i", $this->is_internal)) ? "'" . $this->is_internal . "'" : 'false';


      // Updates the record.
      db_update('chado.library_synonym')
        ->fields($fields)
        ->condition('library_synonym_id', $this->library_synonym_id, '=')
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

      db_delete('chado.library_synonym')
        ->condition('library_synonym_id', $this->library_synonym_id, '=')
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
   * Retrieves the library_synonym_id.
   *
   * @retrun serial
   */
  public function getLibrarySynonymID() {
    return $this->library_synonym_id;
  }

  /**
   * Updates the library_synonym_id.
   *
   * @param serial $library_synonym_id
   */
  public function setLibrarySynonymID($library_synonym_id) {
    $this->library_synonym_id = $library_synonym_id;
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
   * Retrieves the library_id.
   *
   * @retrun integer
   */
  public function getLibraryID() {
    return $this->library_id;
  }

  /**
   * Updates the library_id.
   *
   * @param integer $library_id
   */
  public function setLibraryID($library_id) {
    $this->library_id = $library_id;
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