<?php
/**
 * @file
 * Contains function relating to drush-integration of this module.
 */
/**
 * Describes each drush command implemented by the module.
 *
 * @return
 *   The first line of description when executing the help for a given command
 */
function mcl_drush_help($command) {
  switch ($command) {}
}

/**
 * Registers a drush command and constructs the full help for that command.
 *
 * @return
 *   And array of command descriptions
 */
function mcl_drush_command() {
  $items = array();
  $items['mcl-dump-schema'] = array(
    'description' => t('Dumps a database schema.'),
    'arguments' => array(
      'schema'  => t('The schema name.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-dump-schema [schema]',
    ),
    'options' => array(
      'working_dir' => t('Specifies the working directory.'),
    ),
    'aliases' => array('mcl-dump'),
  );
  $items['mcl-run'] = array(
    'description' => t('Runs job in mcl_job table.'),
    'examples' => array(
      'Standard example' => 'drush mcl-run',
    ),
  );
  $items['mcl-upload-data'] = array(
    'description' => t('Uploads data file.'),
    'arguments'   => array(
      'username'  => t('Drupal user name.'),
      'filepath'  => t('The filename of the uploading file with path.'),
    ),
    'options' => array(
      'working_dir'     => t('Specifies the working directory.'),
      'no_transaction'  => t('No transaction.'),
      'job_name'        => t('Specifies the name of a job.'),
      'move_input'      => t('Moves the input file to the file folder in the job directory.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-upload-data [username] [filepath] --working_dir=/yourpath',
    ),
  );
  $items['mcl-rerun-job'] = array(
    'description' => t('Re-Run the uploading job.'),
    'arguments' => array(
      'job_id'    => t('The job ID.'),
      'filepath'  => t('The filename of the uploading file with path.'),
    ),
    'options' => array(
      'force'           => t('Forces to re-run the passed steps.'),
      'move_input'      => t('Moves the input file to the file folder in the job directory.'),
      'no_transaction'  => t('No transaction.'),
      'status'          => t('The status to start with.'),
      'working_dir'     => t('Specifies the working directory.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-rerun-job [job_id] [filepath] --status=0 --force --working_dir=/yourpath',
    ),
  );
  $items['mcl-test-template'] = array(
    'description' => t('Test templates.'),
    'arguments' => array(),
    'options' => array(
      'filepath' => t('List of templates to be tested.'),
    ),
    'examples' => array(
        'Standard example' => 'drush mcl-test-template --filepath=/filepath',
    ),
  );
  $items['mcl-get-template'] = array(
    'description' => t('Generates a template in Excel format.'),
    'arguments' => array(
      'template'  => t('The template name.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-get-template [template]',
    ),
  );
  $items['mcl-submit-uploading-job'] = array(
    'description' => t('Submits an uploading job.'),
    'arguments' => array(
      'username'  => t('Drupal user name.'),
      'filepath'  => t('The filename of the uploading file with path.'),
    ),
    'options' => array(
      'working_dir' => t('Specify the working directory.'),
      'move_input'  => t('Moves the input file to the file folder in the job directory.'),
      'param'       => t('Adds the job parameter (JSON format).'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-submit-uploading-job [username] [filepath] --working_dir=/your_path',
    ),
  );
  $items['mcl-list-jobs'] = array(
    'description' => t('Views a list of your jobs.'),
    'arguments' => array(
      'username' => t('Drupal user name.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-list-jobs [username]',
    ),
  );
  $items['mcl-job-info'] = array(
    'description' => t('Shows the information about a job.'),
    'arguments' => array(
      'job_id'    => t('The Job ID.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-job-info [job_id]',
    ),
  );
  $items['mcl-delete-job'] = array(
    'description' => t('Deletes a job.'),
    'arguments' => array(
      'job_id'    => t('The Job ID of the job to be deleted.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-delete-job [job_id]',
    ),
  );
  $items['mcl-delete-jobs'] = array(
    'description' => t('Deletes jobs.'),
    'arguments' => array(
      'job_ids' => t('Specify Job ID by the range.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-delete-jobs 4-20',
    ),
  );
  $items['mcl-generate-class'] = array(
    'description' => t('Generate class files.'),
    'arguments' => array(
      'module'      => t('The target module.'),
      'schema'      => t('The schema name.'),
      'dump_file'   => t('The filename of the Postgresql dump file.'),
      'table_file'  => t('The filename of the list of tables.'),
    ),
    'options' => array(
      'namespace'   => t('Specifies the namespace.'),
      'extends'     => t('Specifies the extends.'),
      'overwrite'   => t('Overwrite all class files in the class directory.'),
      'working_dir' => t('Specifies the working directory.'),
      'class_dir'   => t('Specifies the class directory.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-generate-class [module] [schema] [dump_file] [table_file] --working_dir=/yourpath',
    ),
    'aliases' => array('mcl-class'),
  );
  $items['mcl-fix-data'] = array(
    'description' => t('Fix data.'),
    'arguments' => array(
      'inc_file'   => t('The include filename.'),
      'input_file' => t('The input filename.'),
    ),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-fix-data [input_file]',
    ),
  );
  return $items;
}

/**
 * Callback for dump-schema command.
 *
 * @param string $schema
 */
function drush_mcl_dump_schema($schema) {

  // Checks arguments
  if (!$schema) {
    mcl_print("ERROR: please provide a name of schema", 1);
    exit;
  }

  // Sets the target file.
  $target_file = drupal_get_path('module', 'mcl') . "/schema/drupal7.$schema.sql";

  // Sets the command line.
  $cmd = "pg_dump -U postgres -h localhost --no-owner --schema=$schema --schema-only drupal7 > $target_file";
  mcl_print("Execute the following command:", 1);
  mcl_print(">$cmd", 2);
  exec($cmd);
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-run command.
 */
function drush_mcl_run() {

  // Shows job type if specifed.
  mcl_print("Running uploading job in queue.", 1, 2);

  // Launches jobs.
  $manager = new MCL_JOB_MANAGER();
  $manager->processJobs('upload');
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-rerun-job command.
 *
 * @param string $job_id
 * @param string $filepath
 */
function drush_mcl_rerun_job($job_id, $filepath) {

  // Checks arguments.
  $args = array(
    'job_id'    => $job_id,
    'filepath'  => $filepath,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Gets MCL_JOB_UPLOAD.
  $mcl_job = MCL_JOB_UPLOAD::byKey(array('job_id' => $job_id));

  // Gets and sets process ID.
  $pid = getmypid();
  $mcl_job->setPID($pid);

  // Gets ans sets the last run date.
  $mcl_job->setPropByKey('last_run', date("Y-m-d G:i:s"));

  // Checks the status option.
  $status = drush_get_option('status');
  if ($status) {
    if (is_numeric($status)) {

      // Sets the status.
      $mcl_job->setStatus($status);
    }
    else {
      mcl_print("Error : The status ($status) is not numeric", 1, 3);
      exit;
    }
  }

  // Checks the force option.
  $force = drush_get_option('force');
  $force = ($force) ? TRUE : FALSE;
  $mcl_job->setForce($force);

  // Checks the move_input option.
  $move_input = drush_get_option('move_input');
  $move_input = ($move_input) ? TRUE : FALSE;

  // Checks the transaction option.
  $no_transaction = drush_get_option('no_transaction');
  $transaction = ($no_transaction) ? FALSE : TRUE;
  $mcl_job->setPropByKey('transaction', $transaction);

  // Checks the working_dir option.
  $working_dir = drush_get_option('working_dir');
  if ($working_dir) {
    if (!mcl_check_working_dir($working_dir)) {
      exit;
    }
    $mcl_job->changeWorkingDir($working_dir);
  }

  // Overwrites the input file.
  if ($filepath) {

    // Gets the new filepath.
    $file_dir     = $mcl_job->getJobDir() . '/files';
    $new_filepath = $file_dir . '/' . basename($filepath);

    // Set the new filepath.
    $file_id = $mcl_job->getParamByKey('file_id');
    $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
    $mcl_file->setFilepath($new_filepath);
    $mcl_file->update();
    $mcl_job->setUploadFile($mcl_file);

    // Copies it over.
    $cmd = ($move_input) ? "mv $filepath $new_filepath" : "cp $filepath $new_filepath";
    exec($cmd, $return_var);
    if (!empty($return_var)) {
      mcl_print("Fail to overwrite the file", 1, 2);
      mcl_print($cmd, 1, 3);
      exit;
    }
  }

  // Sets the status to be 0 if
  // - status is negative.
  // - filepath is not empty.
  if ($mcl_job->getStatus() < 0 || $filepath) {
    $mcl_job->setStatus(0);
  }

  // Gets process ID.
  $pid = getmypid();
  $mcl_job->setPID($pid);
  print "\tProcess ID = $pid\n\n";

  // Updates all changes of the job properties.
  $mcl_job->update();

  // Launches the job.
  mcl_print("Re-running the job ($job_id)", 1, 2);
  $manager = new MCL_JOB_MANAGER();
  $status = $manager->processJob($mcl_job);
  if ($status) {
    mcl_print("DONE : The uploading job was completed without an error.", 1, 4);

    // Sets the complete date if it has no date. Keep the first one.
    $complete_date = $mcl_job->getCompleteDate();
    if (!$complete_date) {
      $mcl_job->setCompleteDate(date("Y-m-d G:i:s"));
      $mcl_job->update();
    }
  }
  else {
    $job_id = $mcl_job->getJobID();
    $opt_working_dir = '--working_dir=' . $mcl_job->getWorkingDir();
    mcl_print("DONE : The job (Job ID = $job_id) was completed with some errors", 1, 2);
    mcl_print("Please fix the errors and re-run this job", 2, 2);
    mcl_print(">drush mcl-rerun-job $job_id $filepath $opt_working_dir", 2, 4, FALSE);
  }
}

/**
 * Callback for list jobs.
 *
 * @param string $username
 */
function drush_mcl_list_jobs($username) {

  // Checks arguments.
  $args = array(
    'username' => $username,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('name' => $username));

  // Gets all jobs that belong to this user.
  $manager = new MCL_JOB_MANAGER();
  $jobs = $manager->getJobs(array('user_id' => $mcl_user->getUserID()));
  $job_list = '';
  foreach ($jobs as $job) {
    $job_id = $job->getJobID();
    $name   = $job->getName();
    $status = $job->getStatus();
    $job_list .= "\t[$job_id] $name at $status\n";
  }

  // Print out the job list.
  if ($job_list) {
    mcl_print("List jobs for $username", 1);
    mcl_print("==================================", 1);
    mcl_print($job_list, 1);
    mcl_print("----------------------------------", 1, 3);
  }
  else {
    mcl_print("No job found for $username", 1, 3);
  }
}

/**
 * Callback for job info.
 *
 * @param string $job_id
 */
function drush_mcl_job_info($job_id) {

  // Checks arguments.
  $args = array(
    'job_id' => $job_id,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Gets MCL_JOB.
  $mcl_job = MCL_JOB_UPLOAD::byKey(array('job_id' => $job_id));

  // Gets and shows the job information.
  $info = $mcl_job->getJobInfo();
  mcl_print("Job Information", 1, 2);
  foreach ($info as $key => $value) {
    printf("%s : %s\n", $key, $value);
  }
  print "\n\n\n";
}

/**
 * Callback for delete a job.
 */
function drush_mcl_delete_job($job_id) {

  // Checks arguments.
  $args = array(
    'job_id' => $job_id,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Gets MCL_JOB.
  $mcl_job = MCL_JOB::byKey(array('job_id' => $job_id));
  if ($mcl_job->delete()) {
    $msg =  "Job has been deleted";
  }
  else {
    $msg = "Fail to delete the job";
  }
  mcl_print($msg, 1, 3);
}


/**
 * Callback for delete jobs.
 */
function drush_mcl_delete_jobs($job_ids) {

  // Checks arguments.
  if (preg_match("/(\d+)-(\d+)/", $job_ids, $matches)) {
    $start  = $matches[1];
    $stop   = $matches[2];

    // Deletes jobs.
    for ($job_id = $start; $job_id <= $stop; $job_id++) {
      $mcl_job = MCL_JOB::byKey(array('job_id' => $job_id));
      if ($mcl_job) {
        if ($mcl_job->delete()) {
          $msg =  "Job ID ($job_id) has been deleted";
        }
        else {
          $msg = "Fail to delete the job";
        }
        mcl_print($msg, 1, 3);
      }
      else {
        mcl_print("Job ID ($job_id) not exist", 1, 3);
      }
    }
  }
  else {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }
}

/**
 * Callback for testing templates.
 *
 * @param string $template
 *
 * @return boolean
 */
function drush_mcl_test_template() {

  // Gets all templates to be tested. If the filepath option is provided,
  // the listed templates in the file are tested.
  $templates = MCL_TEMPLATE::getTemplates();
  $filepath = drush_get_option('filepath');
  if ($filepath) {
    $templates = array();
    if (!file_exists($filepath)) {
      mcl_print("Error : The file ($filepath) dose not exist", 1, 3);
      exit;
    }
    if (!($fdr = fopen($filepath, 'r'))) {
      mcl_print("Error : Cannot open file ($filepath)", 1, 3);
      exit;
    }
    while (!feof($fdr)) {
      $template = trim(fgets($fdr));
      if ($template) {
        $templates []= MCL_TEMPLATE::getByTemplate($template);
      }
    }
    fclose($fdr);
  }

  // Creates a temporary directory.
  $tmp_dir = file_directory_temp() . '/mcl-test-temp-' . date(Y-m-d-G-i-s);
  if (!mcl_create_dir($tmp_dir)) {
    mcl_print("Error : Failed to create temporary directory for MCL test templates", 1, 3);
    exit;
  }

  // Tests templates.
  //MCL_TEMPLATE::testTemplates($templates, $tmp_dir);

  // Cleanup the emporary directory.
  if (!mcl_remove_dir($tmp_dir)) {
    mcl_print("Error : Failed to remove temporary directory for MCL test templates", 1, 3);
    exit;
  }
  mcl_print("Done testing templates", 1, 3);
}

/**
 * Callback for uploading cvterms from an Excel file.
 *
 * @param string $template
 *
 * @return boolean
 */
function drush_mcl_get_template($template) {

  // Checks arguments.
  if ($template) {
    $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
    if (!$mcl_template) {
      mcl_print("Error : template ($template) have not been defined", 1, 3);
      exit;
    }
  }
  else {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Sets the file path.
  $filepath = getcwd();

  // Gets MCL_TEMPLATE.
  $args = array(
    'template_id' => $mcl_template->getTemplateID(),
    'type'        => 'cmd',
    'filepath'    => $filepath,
  );
  if (mcl_download_template_excel($args)) {
    mcl_print("\n\nTemplate file has been created in ($filepath)", 1, 3);
  }
  else {
    mcl_print("Error : Failed to generate template file", 1, 3);
  }
}

/**
 * Callback for submitting an uploading job.
 *
 * @param string $username
 * @param string $filepath
 */
function drush_mcl_submit_uploading_job($username, $filepath) {

  // Checks arguments.
  $args = array(
    'username'  => $username,
    'filepath'  => $filepath,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Checks the working_dir_dir option.
  $working_dir = drush_get_option('working_dir');
  if ($working_dir) {
    if (!file_exists($working_dir)) {
      mcl_print("Error : The working directory ($working_dir) dose not exist", 1, 3);
      exit;
    }
  }

  // Gets the job_name option.
  $job_name = drush_get_option('job_name');

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('name' => $username));

  // Prepares an uploading job.
  $mcl_job = mcl_prepare_uploading_job($mcl_user, $filepath, $working_dir, $job_name, FALSE);
  if ($mcl_job) {
    mcl_print("Error : Failed to prepare an uploading job", 1, 3);
    exit;
  }

  // Shows the message.
  $msg = "\n\n\t=========================================" .
      "\n\tUploading job has been submitted.\n\tThe notification email " .
      "would be sent to\n\tthe following address shortly.\n\n" .
      "\temail  : " . $mcl_user->getMail() .
      "\nJob ID : " . $mcl_job->getJobID() .
      "\n\t-----------------------------------------\n\n\n\n\n";
  print $msg;
}

/**
 * Callback for uploading data file.
 *
 * @param string $username
 * @param string $filepath
 */
function drush_mcl_upload_data($username, $filepath) {

  // Checks arguments.
  $args = array(
    'username'  => $username,
    'filepath'  => $filepath,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Checks the working_dir option.
  $working_dir = drush_get_option('working_dir');
  if (!mcl_check_working_dir($working_dir)) {
    exit;
  }

  // Gets the job_name option.
  $job_name = drush_get_option('job_name');

  // Gets the transaction option.
  $no_transaction = drush_get_option('no_transaction');
  $trans = ($no_transaction) ? FALSE : TRUE;

  // Check the move_input option.
  $move_input = drush_get_option('move_input');
  $move_input = ($move_input) ? TRUE : FALSE;

  // Handles the param option.
  $param_file = drush_get_option('param');
  $param = array();
  if ($param_file) {
    if (!file_exists($param)) {
      mcl_print("Error : The parameter file doese not exist", 1, 3);
      exit;
    }
    $contents = file_get_contents($param);
    $param = json_decode($contents, TRUE);
    if (!is_array($param)) {
      mcl_print("Error : The parameter file doese not hava valid JSON string", 1, 3);
      exit;
    }
  }

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('name' => $username));

  // Prepares an uploading job.
  $mcl_job = mcl_prepare_uploading_job($mcl_user, $filepath, $move_input, $trans, $working_dir, $job_name, $param);
  if (!$mcl_job) {
    mcl_print("Error : Failed to prepare an uploading job", 1, 3);
    exit;
  }

  // Gets process ID.
  $pid = getmypid();
  $mcl_job->setPID($pid);
  $mcl_job->update();
  print "\tProcess ID = $pid\n\n";

  // Uploads the file.
  $job_id   = $mcl_job->getJobID();
  $job_name = $mcl_job->getName();
  mcl_print("Running $job_name (Job ID = $job_id)", 1, 2);
  $manager = new MCL_JOB_MANAGER();
  $status = $manager->processJob($mcl_job);
  if ($status) {
    mcl_print("\n\nDONE : The uploading job was completed without an error.", 1, 4);

    // Sets the complete date if it has no date. Keep the first one.
    $complete_date = $mcl_job->getCompleteDate();
    if (!$complete_date) {
      $mcl_job->setCompleteDate(date("Y-m-d G:i:s"));
      $mcl_job->update();
    }
  }
  else {
    $job_id = $mcl_job->getJobID();
    $opt_working_dir  = ($working_dir) ? "--working_dir=$working_dir" : '';
    $opt_move_input   = ($move_input) ? '--move_input' : '';
    $opt_transaction  = ($trans) ? '' : '--no_transaction';

    mcl_print("DONE : The job (Job ID = $job_id) was completed with some errors", 1);
    mcl_print("Please fix the errors and re-run this job ", 2, 2);

    // Shows re-run command on screen.
    $rerun_cmd = "drush mcl-rerun-job $job_id $filepath $opt_working_dir $opt_move_input $opt_trans";
    mcl_print(">$rerun_cmd", 2, 4, FALSE);

    // Saves the re-run command as job property.
    $mcl_job->setPropByKey('rerun_cmd', $rerun_cmd);
    $mcl_job->update();
  }
}

/**
 * Prepares for an uploading job.
 *
 * 1. Checks arguments.
 * 2. Copies an input file to job folder.
 *
 * @param MCL_USER $mcl_user
 * @param string $filepath *
 * @param boolean $move_input
 * @param string $working_dir
 * @param string $job_name
 *
 * @return MCL_JOB_UPLOAD
 */
function mcl_prepare_uploading_job(MCL_USER $mcl_user, $filepath, $move_input, $trans, $working_dir = '', $job_name = '', $param = array()) {
  $mcl_job  = NULL;

  // Sets the job directory as working directory.
  $prop = array();
  if ($working_dir) {
    $prop['working_dir'] = $working_dir;
  }

  // Sets the transaction.
  $prop['transaction'] = $trans;

  // Sets the job name.
  $name = 'Uploading-' . basename($filepath);
  if ($job_name) {
    $name = $job_name;
  }

  // Prepares for an uploading job.
  $transaction = db_transaction();
  try {

    // Creates a job for uploading data.
    $details = array(
      'name'        => $name,
      'type'        => 'upload',
      'class_name'  => 'MCL_JOB_UPLOAD',
      'status'      => 0,
      'user_id'     => $mcl_user->getUserID(),
      'submit_date' => date("Y-m-d G:i:s"),
      'prop'        => json_encode($prop),
      'param'       => json_encode($param),
    );
    $mcl_job = new MCL_JOB_UPLOAD($details);
    if ($mcl_job->insert()) {

      // Moves the input file to 'file' folder in the job folder.
      $filename     = basename($filepath);
      $new_filepath = $mcl_job->getFileDir() . "/$filename";
      $cmd          = ($move_input) ? "mv $filepath $new_filepath" : "cp $filepath $new_filepath";
      mcl_print("Copying the input files", 1);
      mcl_print("$cmd", 1, 2);
      exec($cmd, $return_var);
      if (!empty($return_var)) {
        throw new Exception("Fail to copy file.\n\n$cmd\n\n");
      }

      // Adds the uploaded file to mcl_file.
      $details = array(
        'filename'    => $filename,
        'filepath'    => $new_filepath,
        'uri'         => '',
        'filesize'    => filesize($new_filepath),
        'user_id'     => $mcl_user->getUserID(),
        'job_id'      => $mcl_job->getJobID(),
        'submit_date' => date("Y-m-d G:i:s"),
      );
      $upload_file = new MCL_FILE($details);
      if ($upload_file->insert()) {

        // Updates the job properties.
        $mcl_job->setFileID($upload_file->getFileID());
        if (!$mcl_job->update()) {
          throw new Exception("Fail to update Job property.");
        }
      }
      else {
        throw new Exception("Fail to insert mcl_FILE.");
      }
    }
    else {
      throw new Exception("Fail to insert MCL_JOB.");
    }
  } catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
    return array('flag' => FALSE);
  }
  return $mcl_job;
}

/**
 * Callback for generating class files.
 *
 * @param string $module
 * @param string $schema
 * @param string $dump_file
 * @param string $table_file
 * @param string $namespace
 */
function drush_mcl_generate_class($module, $schema, $dump_file, $table_file) {

  // Checks the working directory option. This option is required.
  $working_dir = drush_get_option('working_dir');
  if ($working_dir) {
    if (!file_exists($working_dir)) {
      mcl_print("Error : The working directory does not exist", 1, 4);
      exit;
    }

    // Updates the path for schema and table list files.
    $dump_file  = "$working_dir/$dump_file";
    $table_file = "$working_dir/$table_file";
  }
  else {
    mcl_print("Error : Please specify the working directory", 1, 4);
    exit;
  }

  // Sets the class path directory if provided. Otherwise, sets the
  // default class path.
  $class_dir = drush_get_option('class_dir');
  if (!$class_dir) {

    // Sets the default class directory.
    $module_path = drupal_get_path('module', $module);
    $class_dir = "$module_path/includes/class/$schema" . '_table_class';
  }
  if (!file_exists($class_dir)) {
    mcl_print("The class directory does not exist", 1, 4);
    exit;
  }

  // Checks the overwrite option.
  $flag_overwrite = drush_get_option('overwrite');

  // Checks arguments.
  $args = array(
    'module'     => $module,
    'dump_file'  => $dump_file,
    'table_file' => $table_file,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Checks the namespace and extends options.
  $namespace  = drush_get_option('namespace');
  $extends    = drush_get_option('extends');

  // Creates a tempolary directory and move the current class files.
  $tmp_dir = "$working_dir/tmp";
  mcl_create_dir($tmp_dir);

  // Read the information of the tables in the schema.
  $mcl_table_info = new MCL_TABLE_INFO($dump_file, $table_file);

  // Generats claa files.
  $writer = new MCL_TABLE_WRITER($mcl_table_info, $tmp_dir, $namespace, $extends);
  if ($writer->generateClassFiles()) {

    // Overwrites all the class files with newly generated ones.
    if ($flag_overwrite) {
      $cmd = "mv $tmp_dir/*.inc $class_dir; rmdir $tmp_dir";
      exec($cmd);
      mcl_print("The class files are generated to :", 1 , 1, FALSE);
      mcl_print($class_dir, 1, 4);
    }
    else {
      mcl_print("The class files are generated in to :", 1, 1, FALSE);
      mcl_print($tmp_dir, 1, 1);
      mcl_print("Please copy them over to the class directory", 1, 4);
    }
  }
  else {
    mcl_print("Failed to generate class files", 1, 4);
  }
}

/**
 * Callback for fixing data.
 *
 * @param string $input_file.
 */
function drush_mcl_fix_data($inc_file, $input_file = NULL) {

  // Checks arguments.
  $args = array(
    'inc_file' => $inc_file,
  );
  if (!mcl_check_arguments($args)) {
    mcl_print("Error : Please check arguments", 1, 3);
    exit;
  }

  // Checks the include file.
  $fix_data_dir = drupal_get_path('module', 'mcl') . '/fix_data/';
  $inc_filepath = "$fix_data_dir/$inc_file";
  if (file_exists($inc_filepath)) {
    require_once($inc_filepath);

    // Gets the working directory.
    $working_dir = dirname($inc_filepath);

    // Checks the input file.
    $input_filepath = NULL;
    if ($input_file) {
      $input_filepath = "$working_dir/$input_file";
      if (!file_exists($input_filepath)) {
        print "The input file cannot found : $input_filepath\n\n";
        exit;
      }
    }
    $inc_filename = basename($inc_file);
    $func_name = str_replace('.inc', '', $inc_filename);
    call_user_func($func_name, $working_dir, $input_filepath);
  }
  else {
    print "The include file cannot found : $inc_filepath\n\n";
  }
}

/**
 * Checks working_dir directory.
 *
 * @param string $working_dir
 *
 * @return boolean
 */
function mcl_check_working_dir($working_dir) {

  // Checks the working directory.
  if ($working_dir) {

    // Checks the existence.
    if (!file_exists($working_dir)) {
      mcl_print("Error : The working directory ($working_dir) dose not exist", 1, 3);
      return FALSE;
    }

    // Checks for write permission.
    if (!is_writable($working_dir)) {
      mcl_print("Error : Cannot write on the working directory ($working_dir)", 1);
      mcl_print("Please change the write permission of the directory", 1, 3);
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Checks arguments.
 *
 * @param array $args
 *
 * @return boolean
 */
function mcl_check_arguments($args) {

  // Checks the number of arugments.
  $num_args = sizeof($args);
  $count_args = 0;
  foreach ($args as $key => $value) {
    if (trim($value)) {
      $count_args++;
    }
  }
  if ($count_args != $num_args) {
    mcl_print("Error : The number of arguments does not match", 1, 3);
    return FALSE;
  }

  // Checks each type of argument.
  foreach ($args as $key => $value) {

    // Checks for empty value.
    if (empty($value)) {
      mcl_print("Error : $key is empty", 1, 3);
      return FALSE;
    }

    // Checks for each type of argument.
    if ($key == 'job_id') {

      // Checks for an integer.
      if (!preg_match("/^(\d+)$/", $value)) {
        mcl_print("Error : $key must be an integer", 1, 3);
        return FALSE;
      }

      // Checks for MCL_JOB.
      $mcl_job = MCL_JOB::byKey(array('job_id' => $value));
      if (!$mcl_job) {
        mcl_print("Error : Job ID ($value) does not exist", 1, 3);
        return FALSE;
      }
    }
    else if ($key == 'username') {

      // Checks for MCL_USER.
      $mcl_user = MCL_USER::byKey(array('name' => $value));
      if (!$mcl_user) {
        mcl_print("Error : $value does not exist", 1, 3);
        return FALSE;
      }
    }
    else if (in_array($key, array('filepath', 'dump_file' ,'table_file'))) {

      // Checks for filepath.
      if (!file_exists($value)) {
        mcl_print("Cannot find file", 1);
        mcl_print("$value", 1, 2);
        mcl_print("Please check file name or add full path to the filename", 1, 3);
        return FALSE;
      }
    }
    else if ($key == 'status') {

      // Checks for an integer.
      if (!preg_match("/^(\d+)$/", $value)) {
        mcl_print("Error : $key must be an integer", 1, 3);
        return FALSE;
      }
    }
    else if ($key == 'module') {

      // Checks if module exists.
      if (!module_exists($value)) {
        mcl_print("Error : The module ($value) does not exist", 1, 3);
        return FALSE;
      }
    }
  }
  return TRUE;
}
