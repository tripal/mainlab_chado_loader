<?php
/**
 * The declaration of MCL_JOB_MANAGER class.
 *
 */
class MCL_JOB_MANAGER  {

 /**
  *  Data members.
  */
 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct() {}

  /**
   * Implements the class destructor.
   */
  public function __destruct() {}

  /**
   * Returns the list of jobs.
   *
   * @param array $param
   *  'user_id'
   *  'status'
   *    - array  : contains a list status in integer.
   *    - intger : status
   *    - string : status label
   *  'job_type'
   *  'nagate'
   *
   * @return array of MCL_JOB objects.
   */
  public function getJobs($param) {

    // Gets variables.
    $user_id  = (array_key_exists('user_id', $param))   ? $param['user_id']   : '';
    $status   = (array_key_exists('status', $param))    ? $param['status']    : 'ALL';
    $job_type = (array_key_exists('job_type', $param))  ? $param['job_type']  : '';
    $nagate   = (array_key_exists('nagate', $param))    ? $param['nagate']    : FALSE;

    // Creates SQL statement.
    $sql = " SELECT job_id FROM {mcl_job} WHERE 1=1 ";
    $args = array();

    // Adds user_id.
    if ($user_id) {
      $sql .= " AND user_id = :user_id ";
      $args[':user_id'] = $user_id;
    }

    // Adds job type.
    if ($job_type) {
      $sql .= " AND LOWER(type) = LOWER(:type) ";
      $args[':type'] = $job_type;
    }

    // Adds status.
    if ($status != 'ALL') {

      // Waiting job : status = 0.
      if ($status == '0') {
        $sql .= ' AND status = :status ';
        $args[':status'] = 0;
      }
      // Running job : 0 <= status < 100.
      else if ($status == 'RUN') {
        $sql .= " AND (status >= :status_bot AND status < :status_top) ";
        $args[':status_bot'] = 0;
        $args[':status_top'] = 100;
      }
      else {

        // $status is array.
        if (is_array($status)) {
          $neg = ($nagate) ? ' NOT ' : '';
          $sql .= " AND status $neg IN (" . implode(',', $status) . ') ';
        }
        else {
          // Converts status to integer if string was given.
          $int_status = (preg_match("/^\d+$/", $status)) ? $status : intval($status);
          $int_status = ($int_status) ? $int_status : '0';
          $op = ($nagate) ? ' != ' : ' = ';
          $sql .= " AND status $op :status ";
          $args[':status'] = $int_status;
        }
      }
    }

    // Returns all jobs that satisfy status.
    $jobs = array();
    $result = db_query($sql . " ORDER BY submit_date DESC", $args);
    while ($job_id = $result->fetchField()) {
      $jobs[] = MCL_JOB::byKey(array('job_id' => $job_id));
    }
    return $jobs;
  }

  /**
   * Process jobs in mcl_job table.
   *
   * @param string $job_type
   *
   * @return boolean
   */
  public function processJobs($job_type = NULL) {
    $no_error_flag = TRUE;

    // Gets all jobs for running.
    $jobs = $this->getJobs(array('status' => 'RUN', 'job_type' => $job_type));
    foreach ($jobs as $job) {

      // Process a job.
      $this->processJob($job);
    }
    return $no_error_flag;
  }

  /**
   * Process a job.
   *
   * @param MCL_JOB $job
   *
   * @return boolean
   */
  public function processJob(MCL_JOB $mcl_job) {
    $no_error_flag  = TRUE;

    // Updates status as 'started'.
    if ($mcl_job->getStatus() == 0) {
      $mcl_job->setStatus(10);
      $mcl_job->update();
    }

    // Runs the job if 'run.lock' file does not exist in job directory.
    $lock_file = $mcl_job->getJobDir() . '/run.lock';
    $fp = fopen($lock_file, 'w+');
    if (flock($fp, LOCK_EX|LOCK_NB)) {
      mcl_print("Processing job : " . $mcl_job->getName() . " [" . $mcl_job->getJobID() . "]", 1, 2);

      // Clears previous log files.
      $mcl_job->createNewLogs();
      if (!$mcl_job->run()) {
        $no_error_flag = FALSE;
      }
      $mcl_job->keepLogs();
      fclose($fp);
      unlink($lock_file);
    }
    else {
      mcl_print("The job (" . $mcl_job->getJobID() . " = " . $mcl_job->getType() . ") is running...", 1, 2);
    }
    return $no_error_flag;
  }
}