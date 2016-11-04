<?php
/**
 * The declaration of MCL_JOB class.
 *
 */
class MCL_JOB extends PUBLIC_MCL_JOB {

 /**
  *  Class data members.
  */
  protected $prop_arr     = NULL;
  protected $param_arr    = NULL;
  protected $working_dir  = NULL;
  protected $user_dir     = NULL;
  protected $job_dir      = NULL;
  protected $log_dir      = NULL;
  protected $file_dir     = NULL;

  // Message types.
  protected $msg_types = array(
    'E' => 'Error',
    'W' => 'Warning',
    'N' => 'New Data',
    'D' => 'Duplicate',
  );

  // Temporary variables.
  protected $msgs = array(
    'E' => '',
    'W' => '',
    'N' => '',
    'D' => '',
  );

  // Job status table.
  protected $status_label = array(
    '0'     => 'waiting',
    '10'    => 'started',
    '100'   => 'completed',
    '-100'  => 'cancelled',
    '-200'  => 'failed',
  );
  protected $status_int = array(
    'waiting'   =>    0,
    'started'   =>   10,
    'completed' =>  100,
    'cancelled' => -100,
    'failed'    => -200,
  );

  /**
   * @see PUBLIC_MCL_JOB::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);

    // Updates property array ($this->prop_arr).
    if ($this->prop == '') {
      $this->prop_arr = array();
    }
    else {
      $this->prop_arr = json_decode($this->prop, TRUE);
    }

    // Updates property array ($this->param_arr).
    if ($this->param == '') {
      $this->param_arr = array();
    }
    else {
      $this->param_arr = json_decode($this->param, TRUE);
    }

    // Updates directories.
    $working_dir = $this->getPropByKey('working_dir');
    if ($working_dir) {
      $this->working_dir = $working_dir;
    }
    $user_dir = $this->getPropByKey('user_dir');
    if ($user_dir) {
      $this->user_dir = $user_dir;
    }
    $job_dir = $this->getPropByKey('job_dir');
    if ($job_dir) {
      $this->job_dir = $job_dir;
    }
    $log_dir = $this->getPropByKey('log_dir');
    if ($log_dir) {
      $this->log_dir = $log_dir;
    }
    $file_dir = $this->getPropByKey('file_dir');
    if ($file_dir) {
      $this->file_dir = $file_dir;
    }
  }

  /**
   * @see PUBLIC_MCL_JOB::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see PUBLIC_MCL_JOB::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see PUBLIC_MCL_JOB::insert()
   */
  public function insert() {

    // Updates the parent:$param and parent:$prop fields.
    $this->param = json_encode($this->param_arr);
    $this->prop = json_encode($this->prop_arr);

    // Insert a new job.
    if (parent::insert()) {

      // If $this->working_dir is empty, assigns the default working directory.
      if (empty($this->working_dir)) {
        $this->working_dir = mcl_get_config_setting('mcl_working_dir');
      }

      // Creates the user directory if not exists.
      if (empty($this->user_dir)) {
        $drupal_user = MCL_USER::getDrupalUser($this->user_id);
        $this->user_dir  = $this->working_dir . '/' . $drupal_user->name;
      }

      $this->job_dir  = $this->user_dir . '/' . $this->job_id;
      $this->log_dir  = $this->job_dir . '/logs';
      $this->file_dir = $this->job_dir . '/files';
      $this->setPropByKey('working_dir', $this->working_dir);
      $this->setPropByKey('user_dir', $this->user_dir);
      $this->setPropByKey('job_dir', $this->job_dir);
      $this->setPropByKey('log_dir', $this->log_dir);
      $this->setPropByKey('file_dir', $this->file_dir);
      $this->update();

      // Creates directories.
      mcl_create_dir($this->job_dir);
      mcl_create_dir($this->log_dir);
      mcl_create_dir($this->file_dir);
    }
    else {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see PUBLIC_MCL_JOB::update()
   */
  public function update() {

    // Updates the parent:$param and parent:$prop fields.
    $this->param  = json_encode($this->param_arr);
    $this->prop   = json_encode($this->prop_arr);

    // Updates the user properties.
    return parent::update();
  }

  /**
   * @see PUBLIC_MCL_JOB::delete()
   */
  public function delete() {
    $job_dir = $this->getJobDir();

    // Deletes files associated with this job.
    MCL_FILE::deleteByJob($this);

    // Deletes the job directory.
    if (file_exists($job_dir)) {
      mcl_remove_dir($job_dir);
    }

    // Delete this job.
    return parent::delete();
  }

  /**
   * Returns jobs that belong to the user.
   *
   * @parma integer $user_id
   *
   * @return array of MCL_UPLOAD_JOB
   */

  public static function getJobsByUser($user_id) {
    $sql = "
      SELECT J.job_id, J.class_name
      FROM {mcl_job} J
      WHERE J.user_id = :user_id
      ORDER BY J.submit_date DESC
    ";
    $results = db_query($sql, array(':user_id' => $user_id));
    $mcl_jobs = array();
    while ($obj = $results->fetchObject()) {
      $class_name = $obj->class_name;
      $mcl_jobs []= $class_name::byKey(array('job_id' => $obj->job_id));
    }
    return $mcl_jobs;
  }

  /**
   * Returns job inforamtion table.
   *
   * @return string
   */
  public function getJobInfoTable() {
    // To be overridden by Child class.
  }

  /**
   * Checks if an error has been occurred.
   * Return TRUE if no error message found.
   *
   * @return boolean
   */
  public function noError() {
    // To be overridden by Child class.
  }

  /**
   * Updates the message.
   *
   * @param string $key
   * @param string $msg
   * @param integer $line_no
   */
  public function updateMsg($key, $msg, $line_no = 0) {
    // To be overridden by Child class.
  }

  /**
   * Deletes the messages of the provided type.
   *
   * @param string $type
   */
  public function deleteMsg($type) {
    if (array_key_exists($type, $this->msgs)) {
      $this->msgs[$type] = '';
    }
  }

  /**
   * Resets the logs.
   *
   * @param string $template
   */
  public function resetLogs($template) {
    // To be overridden by Child class.
  }

  /**
   * Creates folders for various types of logs.
   *
   * @return boolean
   */
  public function createNewLogs() {
    // To be overridden by Child class.
  }

  /**
   * Keeps the previous log files to other locaiton.
   */
  public function KeepLogs() {
    // To be overridden by Child class.
  }

  /**
   * Writes all messages in the message array ($this->msgs) to log files.
   * Creates 2 copies of a log file. One would be saved in 'recent' and the other
   * one in 'backups' folder.
   */
  public function writeLog() {
    // To be overridden by Child class.
  }

  /**
   * Called before run().
   */
  public function preRun() {
    // To be overridden by Child class.
  }

  /**
   * Runs a job.
   *
   * @return boolean
   */
  public function run() {
    // To be overridden by Child class.
    return TRUE;
  }

  /**
   * Returns the job inforamtion in array.
   *
   * @return array
   */
  public function getJobInfo() {
    // To be overridden by Child class.
  }

  /**
   * Called after  run().
   *
   * @param array $param
   */
  public function postRun($param = NULL) {
    // To be overridden by Child class.
  }

  /**
   * Sends a completion email.
   *
   */
  public function sendCompleteEmail() {

    // Gets the owner.
    $mcl_user = MCL_USER::byKey(array('user_id' => $this->getUserID()));


  }

  /**
   * Sends an error email.
   *
   */
  public function sendErrorEmail() {

    // Gets the owner.
    $mcl_user = MCL_USER::byKey(array('user_id' => $this->getUserID()));


  }

  /**
   * Changes the working directory. Needs to update job_dir, log_dir
   * and file_dir.
   *
   * @param string $working_dir
   *
   * @return boolean
   */
  public function changeWorkingDir($working_dir) {
    $job_dir = "$working_dir/" . $this->job_id;
    $this->setWorkingDir($working_dir);
    $this->setJobDir($job_dir);
    $this->setLogDir("$job_dir/logs");
    $this->setFileDir("$job_dir/files");
    return $this->update();
  }

  /**
   * Returns the description of message type.
   *
   * @param string $type
   *
   * @return string
   */
  public function getMessageType($type) {
    return $this->msg_types[$type];
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Returns the array of status_label.
   *
   * @return string
   */
  public function getStatusLabelArr() {
    return $this->status_label;
  }

  /**
   * Returns the array of status_int.
   *
   * @return array
   */
  public function getStatusIntArr() {
    return $this->status_int;
  }

  /**
   * Returns the job status label.
   *
   * @return string
   */
  public function getStatusLabel() {
    return $this->status_label[$this->status];
  }

  /**
   * Retrieves the working directory.
   *
   * @retrun string
   */
  public function getWorkingDir() {
    return $this->working_dir;
  }

  /**
   * Updates the working directory.
   *
   * @param string $working_dir
   */
  public function setWorkingDir($working_dir) {
    $this->working_dir = $working_dir;
    $this->setPropByKey('working_dir', $working_dir);
  }

  /**
   * Retrieves the user directory.
   *
   * @retrun string
   */
  public function getUserDir() {
    return $this->user_dir;
  }

  /**
   * Updates the user directory.
   *
   * @param string $user_dir
   */
  public function setUserDir($user_dir) {
    $this->user_dir = $user_dir;
    $this->setPropByKey('user_dir', $user_dir);
  }

  /**
   * Retrieves the directory of the job.
   *
   * @retrun string
   */
  public function getJobDir() {
    return $this->job_dir;
  }

  /**
   * Updates the directory of the job.
   *
   * @param string $job_dir
   */
  public function setJobDir($job_dir) {
    $this->job_dir = $job_dir;
    $this->setPropByKey('job_dir', $job_dir);
  }

  /**
   * Retrieves the directory of the log files.
   *
   * @retrun string
   */
  public function getLogDir() {
    return $this->log_dir;
  }

  /**
   * Updates the directory of the log files.
   *
   * @param string $log_dir
   */
  public function setLogDir($log_dir) {
    $this->log_dir = $log_dir;
    $this->setPropByKey('log_dir', $log_dir);
  }

  /**
   * Retrieves the directory of the files.
   *
   * @retrun string
   */
  public function getFileDir() {
    return $this->file_dir;
  }

  /**
   * Updates the directory of the files.
   *
   * @param string $file_dir
   */
  public function setFileDir($file_dir) {
    $this->file_dir = $file_dir;
    $this->setPropByKey('file_dir', $file_dir);
  }

  /**
   * Updates the param.
   *
   * @param array
   */
  public function setParam($param) {
    parent::setParam($param);
    $this->param_arr = json_decode($param, TRUE);
  }

  /**
   * Returns the value of the given key in param.
   */
  public function getParamByKey($key) {
    if (array_key_exists($key, $this->param_arr)) {
      return $this->param_arr[$key];
    }
    return NULL;
  }

  /**
   * Updates the value of the given key in param.
   */
  public function setParamByKey($key, $value) {
    $this->param_arr[$key] = $value;
  }

  /**
   * Returns the value of the given key in prop.
   */
  public function getPropByKey($key) {
    if (array_key_exists($key, $this->prop_arr)) {
      return $this->prop_arr[$key];
    }
    return NULL;
  }

  /**
   * Updates the value of the given key in prop.
   */
  public function setPropByKey($key, $value) {
    $this->prop_arr[$key] = $value;
  }
}