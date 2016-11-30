<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_FILE class.
 *
 */
class PUBLIC_MCL_FILE  {

  /**
   *  Data members for PUBLIC_MCL_FILE.
   */
  protected $member_arr  = NULL;
  protected $file_id     = NULL;
  protected $type        = NULL;
  protected $filename    = NULL;
  protected $filepath    = NULL;
  protected $filesize    = NULL;
  protected $uri         = NULL;
  protected $user_id     = NULL;
  protected $job_id      = NULL;
  protected $submit_date = NULL;
  protected $prop        = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'file_id'     => 'serial',
    'type'        => 'character_varying(255)',
    'filename'    => 'character_varying(255)',
    'filepath'    => 'character_varying(512)',
    'filesize'    => 'integer',
    'uri'         => 'character_varying(255)',
    'user_id'     => 'integer',
    'job_id'      => 'integer',
    'submit_date' => 'timestamp_without_time_zone',
    'prop'        => 'text',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->file_id     = array_key_exists('file_id', $details)     ? $details['file_id']     : '';
    $this->type        = array_key_exists('type', $details)        ? $details['type']        : '';
    $this->filename    = array_key_exists('filename', $details)    ? $details['filename']    : '';
    $this->filepath    = array_key_exists('filepath', $details)    ? $details['filepath']    : '';
    $this->filesize    = array_key_exists('filesize', $details)    ? $details['filesize']    : 0;
    $this->uri         = array_key_exists('uri', $details)         ? $details['uri']         : '';
    $this->user_id     = array_key_exists('user_id', $details)     ? $details['user_id']     : '';
    $this->job_id      = array_key_exists('job_id', $details)      ? $details['job_id']      : '';
    $this->submit_date = array_key_exists('submit_date', $details) ? $details['submit_date'] : NULL;
    $this->prop        = array_key_exists('prop', $details)        ? $details['prop']        : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_FILE by key(s).
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
    $sql = "SELECT * FROM mcl_file WHERE " . implode(" AND " , $where_arr);
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
        $fields['type']        = $this->type;
        $fields['filename']    = $this->filename;
        $fields['filepath']    = $this->filepath;
        $fields['filesize']    = (is_numeric($this->filesize)) ? $this->filesize : 0;
        $fields['uri']         = $this->uri;
        $fields['user_id']     = $this->user_id;
        $fields['submit_date'] = $this->submit_date;
        $fields['prop']        = $this->prop;
        if (is_numeric($this->job_id)) { $fields['job_id'] = $this->job_id; }

        // Inserts the record.
        $this->file_id = db_insert('mcl_file')
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
      $fields['type']        = $this->type;
      $fields['filename']    = $this->filename;
      $fields['filepath']    = $this->filepath;
      $fields['filesize']    = (is_numeric($this->filesize)) ? $this->filesize : 0;
      $fields['uri']         = $this->uri;
      $fields['user_id']     = $this->user_id;
      $fields['submit_date'] = $this->submit_date;
      $fields['prop']        = $this->prop;
      if (is_numeric($this->job_id)) { $fields['job_id'] = $this->job_id; }

      // Updates the record.
      db_update('mcl_file')
        ->fields($fields)
        ->condition('file_id', $this->file_id, '=')
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

      db_delete('mcl_file')
        ->condition('file_id', $this->file_id, '=')
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
   * Retrieves the file_id.
   *
   * @retrun serial
   */
  public function getFileID() {
    return $this->file_id;
  }

  /**
   * Updates the file_id.
   *
   * @param serial $file_id
   */
  public function setFileID($file_id) {
    $this->file_id = $file_id;
  }

  /**
   * Retrieves the type.
   *
   * @retrun character_varying(255)
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Updates the type.
   *
   * @param character_varying(255) $type
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * Retrieves the filename.
   *
   * @retrun character_varying(255)
   */
  public function getFilename() {
    return $this->filename;
  }

  /**
   * Updates the filename.
   *
   * @param character_varying(255) $filename
   */
  public function setFilename($filename) {
    $this->filename = $filename;
  }

  /**
   * Retrieves the filepath.
   *
   * @retrun character_varying(512)
   */
  public function getFilepath() {
    return $this->filepath;
  }

  /**
   * Updates the filepath.
   *
   * @param character_varying(512) $filepath
   */
  public function setFilepath($filepath) {
    $this->filepath = $filepath;
  }

  /**
   * Retrieves the filesize.
   *
   * @retrun integer
   */
  public function getFilesize() {
    return $this->filesize;
  }

  /**
   * Updates the filesize.
   *
   * @param integer $filesize
   */
  public function setFilesize($filesize) {
    $this->filesize = $filesize;
  }

  /**
   * Retrieves the uri.
   *
   * @retrun character_varying(255)
   */
  public function getUri() {
    return $this->uri;
  }

  /**
   * Updates the uri.
   *
   * @param character_varying(255) $uri
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Retrieves the user_id.
   *
   * @retrun integer
   */
  public function getUserID() {
    return $this->user_id;
  }

  /**
   * Updates the user_id.
   *
   * @param integer $user_id
   */
  public function setUserID($user_id) {
    $this->user_id = $user_id;
  }

  /**
   * Retrieves the job_id.
   *
   * @retrun integer
   */
  public function getJobID() {
    return $this->job_id;
  }

  /**
   * Updates the job_id.
   *
   * @param integer $job_id
   */
  public function setJobID($job_id) {
    $this->job_id = $job_id;
  }

  /**
   * Retrieves the submit_date.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getSubmitDate() {
    return $this->submit_date;
  }

  /**
   * Updates the submit_date.
   *
   * @param timestamp_without_time_zone $submit_date
   */
  public function setSubmitDate($submit_date) {
    $this->submit_date = $submit_date;
  }

  /**
   * Retrieves the prop.
   *
   * @retrun text
   */
  public function getProp() {
    return $this->prop;
  }

  /**
   * Updates the prop.
   *
   * @param text $prop
   */
  public function setProp($prop) {
    $this->prop = $prop;
  }
}