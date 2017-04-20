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
  protected $std_dir      = NULL;
  protected $file_dir     = NULL;
  protected $pid          = NULL;

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
    $std_dir = $this->getPropByKey('std_dir');
    if ($std_dir) {
      $this->std_dir = $std_dir;
    }
    $file_dir = $this->getPropByKey('file_dir');
    if ($file_dir) {
      $this->file_dir = $file_dir;
    }
    $pid = $this->getPropByKey('pid');
    if ($pid) {
      $this->pid = $pid;
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

      // Get MCL user directory.
      $working_dir = $this->getPropByKey('working_dir');
      if (!$working_dir) {
        $working_dir = mcl_get_config_setting('mcl_working_dir') . '/user';
      }

      // Gets username.
      $drupal_user = MCL_USER::getDrupalUser($this->user_id);
      $username = $drupal_user->name;

      // Updates MCL directories.
      $this->user_dir = "$working_dir/$username";
      $this->job_dir  = $this->user_dir . '/' . $this->job_id;
      $this->log_dir  = $this->job_dir . '/logs';
      $this->std_dir  = $this->job_dir . '/std';
      $this->file_dir = $this->job_dir . '/files';
      $this->setPropByKey('working_dir', $this->user_dir);
      $this->setPropByKey('user_dir', $this->user_dir);
      $this->setPropByKey('job_dir', $this->job_dir);
      $this->setPropByKey('log_dir', $this->log_dir);
      $this->setPropByKey('std_dir', $this->std_dir);
      $this->setPropByKey('file_dir', $this->file_dir);
      $this->setPropByKey('pid', $this->pid);
      $this->update();

      // Creates directories.
      mcl_create_dir($this->job_dir);
      mcl_create_dir($this->log_dir);
      mcl_create_dir($this->std_dir);
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
   * @parma string $type
   *
   * @return array of MCL_UPLOAD_JOB
   */

  public static function getJobsByUser($user_id, $type = NULL) {
    $sql = "
      SELECT J.job_id, J.class_name
      FROM {mcl_job} J
      WHERE J.user_id = :user_id
    ";
    $args =array (':user_id' => $user_id);

    // Adds conditions.
    if ($type) {
      $sql .= ' AND J.type = :type';
      $args[':type'] = $type;
    }

    $sql .= ' ORDER BY J.submit_date DESC';
    $results = db_query($sql, $args);
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
   * Returns the name of the std file.
   *
   * @param string $ext
   *
   * @return string
   */
  public function getStdFile($ext) {
    return $this->getStdDir() . '/mcl-' . date("Y-m-d-G-i-s") . '.' . $ext;
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
   * Changes the working directory. Needs to update job_dir, log_dir, std_dir
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
    $this->setStdDir("$job_dir/std");
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

  /**
   * Resets the progress file.
   */
  public function resetProgress() {
    $progress_file = $this->getJobDir() . '/progress';
    if (file_exists($progress_file)) {
      unlink($progress_file);
    }

    // Creates a progress file.
    file_put_contents($progress_file, 'RESET PROGRESS');
  }

  /**
   * Retrieves the progress of the job.
   *
   * @retrun string
   */
  public function getProgress() {

    // Gets the process file.
    $progress_file = $this->getJobDir() . '/progress';
    if (file_exists($progress_file)) {
      return file_get_contents($progress_file);
    }
    else {
      return 'No progress report.';
    }
  }

  /**
   * Updates the progress of the job.
   *
   * @param string $progress
   * @param boolean $append
   */
  public function setProgress($progress, $append = FALSE) {

    // Gets the process file.
    $progress_file = $this->getJobDir() . '/progress';

    // Appends the previous message.
    $old_progress = '';
    if ($append) {
      if (file_exists($progress_file)) {
        $old_progress = file_get_contents($progress_file) . "\n";
      }
    }
    file_put_contents($progress_file, $old_progress . $progress);
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
   * Retrieves the directory of the std files.
   *
   * @retrun string
   */
  public function getStdDir() {
    return $this->std_dir;
  }

  /**
   * Updates the directory of the std files.
   *
   * @param string $std_dir
   */
  public function setStdDir($std_dir) {
    $this->std_dir = $std_dir;
    $this->setPropByKey('std_dir', $std_dir);
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
   * Retrieves the process ID.
   *
   * @retrun integer
   */
  public function getPID() {
    return $this->pid;
  }

  /**
   * Updates the process ID.
   *
   * @param integer $pid
   */
  public function setPID($pid) {
    $this->pid = $pid;
    $this->setPropByKey('pid', $pid);
    $this->update();
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