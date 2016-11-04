<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_LIBRARY class.
 *
 */
class CHADO_LIBRARY  {

 /**
  *  Data members for CHADO_LIBRARY.
  */
  protected $member_arr       = NULL;
  protected $library_id       = NULL;
  protected $organism_id      = NULL;
  protected $name             = NULL;
  protected $uniquename       = NULL;
  protected $type_id          = NULL;
  protected $is_obsolete      = NULL;
  protected $timeaccessioned  = NULL;
  protected $timelastmodified = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'library_id'       => 'serial',
    'organism_id'      => 'integer',
    'name'             => 'character_varying(255)',
    'uniquename'       => 'text',
    'type_id'          => 'integer',
    'is_obsolete'      => 'integer',
    'timeaccessioned'  => 'timestamp_without_time_zone',
    'timelastmodified' => 'timestamp_without_time_zone',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->library_id       = array_key_exists('library_id', $details)       ? $details['library_id']       : '';
    $this->organism_id      = array_key_exists('organism_id', $details)      ? $details['organism_id']      : '';
    $this->name             = array_key_exists('name', $details)             ? $details['name']             : '';
    $this->uniquename       = array_key_exists('uniquename', $details)       ? $details['uniquename']       : '';
    $this->type_id          = array_key_exists('type_id', $details)          ? $details['type_id']          : '';
    $this->is_obsolete      = array_key_exists('is_obsolete', $details)      ? $details['is_obsolete']      : 0;
    $this->timeaccessioned  = array_key_exists('timeaccessioned', $details)  ? $details['timeaccessioned']  : '';
    $this->timelastmodified = array_key_exists('timelastmodified', $details) ? $details['timelastmodified'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_LIBRARY by key(s).
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
    $sql = "SELECT * FROM chado.library WHERE " . implode(" AND " , $where_arr);
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
        $fields['organism_id']      = $this->organism_id;
        $fields['name']             = $this->name;
        $fields['uniquename']       = $this->uniquename;
        $fields['type_id']          = $this->type_id;
        $fields['is_obsolete']      = (is_numeric($this->is_obsolete))  ? $this->is_obsolete      : 0;
        $fields['timeaccessioned']  = (!empty($this->timeaccessioned))  ? $this->timeaccessioned  : 'now()';
        $fields['timelastmodified'] = (!empty($this->timelastmodified)) ? $this->timelastmodified : 'now()';


        // Inserts the record.
        $this->library_id = db_insert('chado.library')
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
      $fields['organism_id']      = $this->organism_id;
      $fields['name']             = $this->name;
      $fields['uniquename']       = $this->uniquename;
      $fields['type_id']          = $this->type_id;
      $fields['is_obsolete']      = (is_numeric($this->is_obsolete))  ? $this->is_obsolete      : 0;
      $fields['timeaccessioned']  = (!empty($this->timeaccessioned))  ? $this->timeaccessioned  : 'now()';
      $fields['timelastmodified'] = (!empty($this->timelastmodified)) ? $this->timelastmodified : 'now()';


      // Updates the record.
      db_update('chado.library')
        ->fields($fields)
        ->condition('library_id', $this->library_id, '=')
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

      db_delete('chado.library')
        ->condition('library_id', $this->library_id, '=')
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
   * Retrieves the library_id.
   *
   * @retrun serial
   */
  public function getLibraryID() {
    return $this->library_id;
  }

  /**
   * Updates the library_id.
   *
   * @param serial $library_id
   */
  public function setLibraryID($library_id) {
    $this->library_id = $library_id;
  }

  /**
   * Retrieves the organism_id.
   *
   * @retrun integer
   */
  public function getOrganismID() {
    return $this->organism_id;
  }

  /**
   * Updates the organism_id.
   *
   * @param integer $organism_id
   */
  public function setOrganismID($organism_id) {
    $this->organism_id = $organism_id;
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
   * Retrieves the is_obsolete.
   *
   * @retrun integer
   */
  public function getIsObsolete() {
    return $this->is_obsolete;
  }

  /**
   * Updates the is_obsolete.
   *
   * @param integer $is_obsolete
   */
  public function setIsObsolete($is_obsolete) {
    $this->is_obsolete = $is_obsolete;
  }

  /**
   * Retrieves the timeaccessioned.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTimeaccessioned() {
    return $this->timeaccessioned;
  }

  /**
   * Updates the timeaccessioned.
   *
   * @param timestamp_without_time_zone $timeaccessioned
   */
  public function setTimeaccessioned($timeaccessioned) {
    $this->timeaccessioned = $timeaccessioned;
  }

  /**
   * Retrieves the timelastmodified.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTimelastmodified() {
    return $this->timelastmodified;
  }

  /**
   * Updates the timelastmodified.
   *
   * @param timestamp_without_time_zone $timelastmodified
   */
  public function setTimelastmodified($timelastmodified) {
    $this->timelastmodified = $timelastmodified;
  }
}