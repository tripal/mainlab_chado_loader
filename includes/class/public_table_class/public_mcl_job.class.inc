<?php
/**
 * Adds namespace.
 */
/**
 * The declaration of PUBLIC_MCL_JOB class.
 *
 */
class PUBLIC_MCL_JOB  {

  /**
   *  Data members for PUBLIC_MCL_JOB.
   */
  protected $member_arr    = NULL;
  protected $job_id        = NULL;
  protected $name          = NULL;
  protected $type          = NULL;
  protected $class_name    = NULL;
  protected $status        = NULL;
  protected $param         = NULL;
  protected $prop          = NULL;
  protected $user_id       = NULL;
  protected $submit_date   = NULL;
  protected $complete_date = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'job_id'        => 'serial',
    'name'          => 'character_varying(255)',
    'type'          => 'character_varying(255)',
    'class_name'    => 'character_varying(255)',
    'status'        => 'integer',
    'param'         => 'text',
    'prop'          => 'text',
    'user_id'       => 'integer',
    'submit_date'   => 'timestamp_without_time_zone',
    'complete_date' => 'timestamp_without_time_zone',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->job_id        = array_key_exists('job_id', $details)        ? $details['job_id']        : '';
    $this->name          = array_key_exists('name', $details)          ? $details['name']          : '';
    $this->type          = array_key_exists('type', $details)          ? $details['type']          : '';
    $this->class_name    = array_key_exists('class_name', $details)    ? $details['class_name']    : '';
    $this->status        = array_key_exists('status', $details)        ? $details['status']        : '';
    $this->param         = array_key_exists('param', $details)         ? $details['param']         : '';
    $this->prop          = array_key_exists('prop', $details)          ? $details['prop']          : '';
    $this->user_id       = array_key_exists('user_id', $details)       ? $details['user_id']       : '';
    $this->submit_date   = array_key_exists('submit_date', $details)   ? $details['submit_date']   : NULL;
    $this->complete_date = array_key_exists('complete_date', $details) ? $details['complete_date'] : NULL;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_JOB by key(s).
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
    $sql = "SELECT * FROM mcl_job WHERE " . implode(" AND " , $where_arr);
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
        $fields['name']        = $this->name;
        $fields['type']        = $this->type;
        $fields['class_name']  = $this->class_name;
        $fields['param']       = $this->param;
        $fields['prop']        = $this->prop;
        $fields['user_id']     = $this->user_id;
        $fields['submit_date'] = $this->submit_date;
        if (is_numeric($this->status))    { $fields['status']        = $this->status; }
        if (!empty($this->complete_date)) { $fields['complete_date'] = $this->complete_date; }

        // Inserts the record.
        $this->job_id = db_insert('mcl_job')
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
      $fields['type']        = $this->type;
      $fields['class_name']  = $this->class_name;
      $fields['param']       = $this->param;
      $fields['prop']        = $this->prop;
      $fields['user_id']     = $this->user_id;
      $fields['submit_date'] = $this->submit_date;
      if (is_numeric($this->status))    { $fields['status']        = $this->status; }
      if (!empty($this->complete_date)) { $fields['complete_date'] = $this->complete_date; }

      // Updates the record.
      db_update('mcl_job')
        ->fields($fields)
        ->condition('job_id', $this->job_id, '=')
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

      db_delete('mcl_job')
        ->condition('job_id', $this->job_id, '=')
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
   * Retrieves the job_id.
   *
   * @retrun serial
   */
  public function getJobID() {
    return $this->job_id;
  }

  /**
   * Updates the job_id.
   *
   * @param serial $job_id
   */
  public function setJobID($job_id) {
    $this->job_id = $job_id;
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
   * Retrieves the class_name.
   *
   * @retrun character_varying(255)
   */
  public function getClassName() {
    return $this->class_name;
  }

  /**
   * Updates the class_name.
   *
   * @param character_varying(255) $class_name
   */
  public function setClassName($class_name) {
    $this->class_name = $class_name;
  }

  /**
   * Retrieves the status.
   *
   * @retrun integer
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * Updates the status.
   *
   * @param integer $status
   */
  public function setStatus($status) {
    $this->status = $status;
  }

  /**
   * Retrieves the param.
   *
   * @retrun text
   */
  public function getParam() {
    return $this->param;
  }

  /**
   * Updates the param.
   *
   * @param text $param
   */
  public function setParam($param) {
    $this->param = $param;
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
   * Retrieves the complete_date.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getCompleteDate() {
    return $this->complete_date;
  }

  /**
   * Updates the complete_date.
   *
   * @param timestamp_without_time_zone $complete_date
   */
  public function setCompleteDate($complete_date) {
    $this->complete_date = $complete_date;
  }
}