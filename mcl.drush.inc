<?php
/**
 * @file
 * Contains function relating to drush-integration of this module.
 */
/**
 * Global variables.
 */
$GLOBALS['verbose'] = TRUE;

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
  $items['mcl-debug'] = array(
    'description' => t('Debug.'),
    'examples' => array(
      'Standard example' => 'drush mcl-debug',
    ),
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
  $items['mcl-upload-test-data'] = array(
    'description' => t('Uploading the test data.'),
    'arguments' => array(
      'username'  => t('The user name.'),
    ),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-upload-test-data <username>',
    ),
  );
  $items['mcl-delete-test-data'] = array(
    'description' => t('Deleting the test data.'),
    'arguments' => array(),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-delete-test-data',
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
      'job_id' => t('The Job ID of the job to be deleted.'),
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
      'working_dir' => t('Specifies the working directory.'),
    ),
    'examples' => array(
      'Standard example' => 'drush mcl-generate-class [module] [schema] [dump_file] [table_file] --working_dir=/yourpath',
    ),
    'aliases' => array('mcl-class'),
  );
  $items['mcl-add-cv'] = array(
    'description' => t('Add a cv.'),
    'arguments' => array(
      'cv_name' => t('The cv name.'),
    ),
    'options' => array(
      'def' => t('The definition of the cv'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-cv [cv_name] --def=<definition>',
    ),
  );
  $items['mcl-add-cvterm'] = array(
    'description' => t('Add a cvterm.'),
    'arguments' => array(
      'cvterm_name' => t('The cvterm name.'),
      'cv_name' => t('The cv name.'),
    ),
    'options' => array(
      'def' => t('The definition of the cvterm'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-add-cvterm [cvterm_name] --def=<definition>',
    ),
  );
  $items['mcl-delete-cvterm'] = array(
    'description' => t('Delete a cvterm.'),
    'arguments' => array(
      'cvterm_name' => t('The cvterm name.'),
      'cv_name' => t('The cv name.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-cvterm [cvterm_name] [cv_name]',
    ),
  );
  $items['mcl-delete-cvterm-by-id'] = array(
    'description' => t('Delete a cvterm by ID.'),
    'arguments' => array(
      'cvterm_id' => t('The cvterm ID.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-cvterm-by-id [cvterm_id]',
    ),
  );
  $items['mcl-delete-phenotype-data-by-projects'] = array(
    'description' => t('Delete phenotyping data by projects.'),
    'arguments' => array(
      'project_ids' => t('The project IDs separated by colons.'),
    ),
    'options' => array(
      'delete' => t('Deletes data'),
      'project' => t('Deletes projects after deleting phenotyping data.'),
      'nd_geolocation' => t('Deletes nd_geolocation.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-phenotype-data-by-projects [project_ids] --project --nd_geolocation',
    ),
  );
  $items['mcl-delete-phenotype-data-by-cv'] = array(
    'description' => t('Delete phenotyping data by cv.'),
    'arguments' => array(
      'cv_name' => t('The cv name.'),
    ),
    'options' => array(
      'delete' => t('Deletes data'),
      'nd_geolocation' => t('Deletes nd_geolocation.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-delete-phenotype-data-by-projects [project_ids] --project --nd_geolocation',
    ),
  );
  $items['mcl-list-cvterm'] = array(
    'description' => t('List all cvterms of the give cv name.'),
    'arguments' => array(
      'cv_name' => t('The cv name.'),
    ),
    'examples' => array(
      'Standard example' => 'mcl-list-cvterm [cv_name]',
    ),
  );
  $items['mcl-fix-date'] = array(
    'description' => t('Fix date (integer to date).'),
    'arguments' => array(
      'cv_name'     => t('The cv name.'),
      'cvterm_name' => t('The cvterm name.'),
      'table_name'  => t('The table name.'),
    ),
    'options' => array(),
    'examples' => array(
      'Standard example' => 'drush mcl-fix-data [cv_name] [cvterm_name] [table_name]',
    ),
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
  $items['mcl-call-func'] = array(
    'description' => t('Call the provided function name.'),
    'arguments' => array(
      'function_name' => t('The function name.'),
    ),
    'options' => array(
      'param' => t('The parameters of calling function in JSON format.'),
    ),
    'examples' => array(
       'Standard example' => 'drush mcl-call-func [function_name] --param=<parameters in JSON>',
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

  // Checks schema.
  mcl_check_schema($schema);

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
function drush_mcl_rerun_job($job_id, $filepath = '') {

  // Checks the job ID.
  $mcl_job = mcl_check_job_id($job_id);

  // Sets the filepath if not provided.
  if ($filepath == '') {
    $mcl_file = mcl_check_file_id($mcl_job->getFileID());
    $filepath = $mcl_file->getFilepath();
  }

  // Checks the filepath.
  mcl_check_filepath($filepath);

  // Updates the file path in case of file path and name changed.
  $mcl_file_id = $mcl_job->getParamByKey('file_id');
  $mcl_file = MCL_FILE::byKey(array('file_id' => $mcl_file_id));
  if (!$mcl_file) {
    mcl_print("Error : Failed to get MCL_FILE ($mcl_file_id)", 1, 3);
  }
  $mcl_file->setFilepath($filepath);
  $mcl_file->setFilename(basename($filepath));
  $mcl_file->setFilesize(filesize($filepath));
  if (!$mcl_file->update()) {
    mcl_print("Error : Failed to update filepath", 1, 3);
  }

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
    $new_filepath = $mcl_job->getJobDir() . '/files/' . basename($filepath);

    // Set the new filepath.
    $file_id = $mcl_job->getParamByKey('file_id');
    $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
    $mcl_file->setFilepath($new_filepath);
    $mcl_file->update();
    $mcl_job->setUploadFile($mcl_file);

    // Overwrite the file.
    if ($filepath != $new_filepath) {
      $cmd = ($move_input) ? "mv $filepath $new_filepath" : "cp $filepath $new_filepath";
      exec($cmd, $return_var);
      if (!empty($return_var)) {
        mcl_print("Error : Fail to overwrite the file", 1, 2);
        mcl_print($cmd, 1, 3);
        exit;
      }
    }
    else {
      mcl_print("The target and destination files are the same so use the current one", 1, 2);
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

  // Checks the username.
  $mcl_user = mcl_check_username($username);

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

  // Checks the job ID.
  $mcl_job = mcl_check_job_id($job_id);

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

  // Checks the job ID.
  $mcl_job = mcl_check_job_id($job_id);

  // Deletes the job.
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
 * Callback for mcl-add-cv.
 */
function drush_mcl_add_cv($cv_name) {

  // Gets the cv.
  $mcl_cv = MCL_CHADO_CV::getCV($cv_name);
  if ($mcl_cv) {
    $cv_id = $mcl_cv->getCvID();
    mcl_print("Error : The cv '$cv_name' ($cv_id) has already existed", 1, 3);
    exit;
  }

  // Gets the definition if exists.
  $definition = drush_get_option('def');

  // Add a new cv.
  $transaction = db_transaction();
  try {
    $mcl_cv = MCL_CHADO_CV::addCV(NULL, $cv_name, $definition);
    if (!$mcl_cv) {
      throw new Exception("Fail to add the cv ($cv_name)");
    }
    $cv_id = $mcl_cv->getCvID();
    mcl_print("The cv '$cv_name' ($cv_id) has been added", 1, 3);
  }
  catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    exit;
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-add-cvterm.
 */
function drush_mcl_add_cvterm($cvterm_name, $cv_name = 'SITE_CV') {

  // Check the cvterm.
  $mcl_cvterm = MCL_CHADO_CVTERM::checkCvterm(NULL, $cv_name, $cvterm_name);
  if ($mcl_cvterm) {
    $cvterm_id = $mcl_cvterm->getCvtermID();
    mcl_print("The cvterm '$cvterm_name' ($cvterm_id) exists", 1, 3);
  }
  else {

    // Gets the definition if exists.
    $definition = drush_get_option('def');

    // Add a new cvterm.
    $transaction = db_transaction();
    try {
      $cvterm = MCL_CHADO_CVTERM::addCvterm(NULL, 'SITE_DB', $cv_name, $cvterm_name, $definition);
      if (!$cvterm) {
        throw new Exception("Fail to add the cvterm ($cvterm_name)");
      }
      $cvterm_id = $cvterm->getCvtermID();
      mcl_print("The cvterm '$cvterm_name' ($cvterm_id) has been added", 1, 3);
    }
    catch (Exception $e) {
      $transaction->rollback();
      mcl_print("Error : " . $e->getMessage(), 1, 3);
      exit;
    }
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-add-cvterm.
 */
function drush_mcl_delete_cvterm($cvterm_name, $cv_name = 'SITE_CV') {

  // Gets MCL_CHADO_CVTERM.
  $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
  if (!$mcl_cvterm) {
    mcl_print("Error : The cvterm ('$cvterm_name', '$cv_name') does not exist", 1, 3);
    exit;
  }

  // Deletes the cvterm.
  $cvterm_name = $mcl_cvterm->getName();
  $cvterm_id   = $mcl_cvterm->getCvtermID();
  if ($mcl_cvterm->delete()) {
    mcl_print("The cvterm '$cvterm_name' ($cvterm_id) has been deleted", 1, 3);
  }
  else {
    mcl_print("Error : Failed to delete '$cvterm_name' ($cvterm_id)", 1, 3);
  }
}

/**
 * Callback for mcl-delete-cvterm.
 */
function drush_mcl_delete_cvterm_by_id($cvterm_id) {

  // Gets MCL_CHADO_CVTERM.
  $mcl_cvterm = MCL_CHADO_CVTERM::byID($cvterm_id);
  if (!$mcl_cvterm) {
    mcl_print("Error : The cvterm ID ($cvterm_id) does not exist", 1, 3);
    exit;
  }

  // Deletes the cvterm.
  $cvterm_name = $mcl_cvterm->getName();
  if ($mcl_cvterm->delete()) {
    mcl_print("The cvterm '$cvterm_name' ($cvterm_id) has been deleted", 1, 3);
  }
  else {
    mcl_print("Error : Failed to delete '$cvterm_name' ($cvterm_id)", 1, 3);
  }

}

/**
 * Callback for mcl-delete-phenotype-data-by-cv.
 *
 * Deletes the phenotyping data by cv name
 *
 * @param string $cv_name
 *  The name of cv.
 */
function drush_mcl_delete_phenotype_data_by_cv($cv_name) {

  // Check the cv name.
  if (!$cv_name) {
    mcl_print("Please provide cv name", 1, 3);
    exit;
  }
  $cv = MCL_CHADO_CV::byName($cv_name);
  if (!$cv) {
    mcl_print("The cv name ($cv_name) does not exist", 1, 3);
    exit;
  }

  // Gets the delete option.
  $delete = drush_get_option('delete')  ? TRUE : FALSE;

  // Gets the nd_geolocation option.
  $nd_geolocation = drush_get_option('nd_geolocation') ? TRUE : FALSE;

  // Gets all the projects that belongs to the cv.
  $project_ids = array();
  $sql = "
    SELECT DISTINCT PRJ.project_id
    FROM {chado.project} PRJ
      INNER JOIN {chado.nd_experiment_project} NEPRJ on NEPRJ.project_id = PRJ.project_id
      INNER JOIN {chado.nd_experiment_phenotype} NEP on NEP.nd_experiment_id = NEPRJ.nd_experiment_id
      INNER JOIN {chado.phenotype} P on P.phenotype_id = NEP.phenotype_id
      INNER JOIN {chado.cvterm} C on C.cvterm_id = P.attr_id
    WHERE C.cv_id = :cv_id
  ";
  $results = db_query($sql, array(':cv_id' => $cv->getCvID()));
  if ($delete) {
    mcl_print("Delete phenotyping data by cv ($cv_name)", 1, 1.3);
  }
  else {
    mcl_print("Checking phenotyping data by cv ($cv_name)", 1, 1.3);
  }
  while ($project_id = $results->fetchField()) {
    $project_ids []= $project_id;
  }
  if (empty($project_ids)) {
    mcl_print("No pheotyping data found for the cv ($cv_name)", 1, 2);
  }
  else {
    if ($delete) {
      mcl_print("Deletes the phenotyping data", 2, 2);
    }
    else {
      mcl_print("Checking the phenotyping data", 2, 2);
    }
    _drush_mcl_delete_phenotype_data($project_ids, $delete, TRUE, $nd_geolocation);
  }
}

/**
 * Callback for mcl-delete-phenotype-data-by-projects.
 *
 * Deletes the phenotyping data by projects. This won't delete any data
 * in phenotype table. It deletes the followings.
 *
 *   1. samples in stock table.
 *   2. locations in nd_geolocation table.
 *   3. phenotype experiment data in nd_experiment table.
 *   4. projects in project table.
 *
 * @param string project_ids
 *  A list of project ID separed by colon.
 */
function drush_mcl_delete_phenotype_data_by_projects($project_ids) {

  // Parses out project IDs.
  $tmp = preg_split('/:/', $project_ids, -1, PREG_SPLIT_NO_EMPTY);
  if (sizeof($tmp) == 0) {
    mcl_print("Please provide project ID", 1, 3);
    exit;
  }

  // Gets the options.
  $delete = drush_get_option('delete')  ? TRUE : FALSE;
  $project = drush_get_option('project') ? TRUE : FALSE;
  $nd_geolocation = drush_get_option('nd_geolocation') ? TRUE : FALSE;

  // Deletes phenotyping data.
  _drush_mcl_delete_phenotype_data($tmp, $delete, $project, $nd_geolocation);
}

/**
 * Deletes phenotyping data.
 *
 * @param boolean $project_ids
 * @param boolean $flag_project
 * @param boolean $flag_nd_geolocation
 */
function _drush_mcl_delete_phenotype_data($project_ids, $delete = FALSE, $flag_project = FALSE, $flag_nd_geolocation = FALSE) {

  $transaction = db_transaction();
  try {

    // Deletes phenotyping data for each project.
    $op = ($delete) ? 'Deleting' : 'Checking';
    $num_project = sizeof($project_ids);
    mcl_print("$op phenotyping data for $num_project projects", 2, 2);
    foreach ((array)$project_ids as $project_id) {
      $project = MCL_CHADO_DATASET::byId($project_id);
      if (!$project) {
        mcl_print("Project ID ($project_id) not found in project table. Skipped", 2, 2);
        continue;
      }

      // Deleting / Checking phenotyping data for a project.
      mcl_print("$op phenotyping data for " . $project->getName() . " [$project_id]", 2, 1.2);

      // Sets the arguments.
      $args = array(
        ':cv_name'      => 'main',
        ':cvterm_name'  => 'sample',
        ':project_id'   => $project_id,
      );

      // 1. Deleting samples.
      if ($delete) {
        mcl_print("Deleting samples in stock table", 2, 1);
        $sql = "
          DELETE FROM chado.stock WHERE stock_id IN (
            SELECT DISTINCT S.stock_id
            FROM chado.stock S
              INNER JOIN chado.cvterm C on C.cvterm_id = S.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
              INNER JOIN chado.nd_experiment_stock NES on NES.stock_id = S.stock_id
              INNER JOIN chado.nd_experiment_project NEP on NEP.nd_experiment_id = NES.nd_experiment_id
            WHERE LOWER(CV.name) = :cv_name AND LOWER(C.name) = :cvterm_name AND NEP.project_id = :project_id
          )
        ";
        db_query($sql, $args);
      }
      else {
        $sql = "
          SELECT COUNT(DISTINCT S.stock_id)
            FROM chado.stock S
              INNER JOIN chado.cvterm C on C.cvterm_id = S.type_id
              INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
              INNER JOIN chado.nd_experiment_stock NES on NES.stock_id = S.stock_id
               INNER JOIN chado.nd_experiment_project NEP on NEP.nd_experiment_id = NES.nd_experiment_id
          WHERE LOWER(CV.name) = :cv_name AND LOWER(C.name) = :cvterm_name AND NEP.project_id = :project_id
        ";
        $num = db_query($sql, $args)->fetchField();
        mcl_print("The number of samples : $num", 2, 1);
      }

      // 2. Deleting from nd_experiment.
      $args = array(':project_id' => $project_id);
      if ($delete) {
        mcl_print("Deleting nd_experiment in nd_experiment table", 2, 1);
        $sql = "
          DELETE FROM chado.nd_experiment WHERE nd_experiment_id IN (
            SELECT NEP.nd_experiment_id
            FROM chado.nd_experiment_project NEP
            WHERE NEP.project_id = :project_id
          )
        ";
        db_query($sql, $args);
      }
      else {
        $sql = "
          SELECT COUNT(NEP.nd_experiment_id)
          FROM chado.nd_experiment_project NEP
          WHERE NEP.project_id = :project_id
        ";
        $num = db_query($sql, $args)->fetchField();
        mcl_print("The number of nd_experiment : $num", 2, 1);
      }

      // 3. Deleting from nd_geolocation.
      if ($delete) {
        if ($flag_nd_geolocation) {
          mcl_print("Deleting locations in nd_geolocation table", 2, 1);
          $sql = "
            DELETE FROM chado.nd_geolocation WHERE nd_geolocation_id IN (
              SELECT DISTINCT NE.nd_geolocation_id
              FROM chado.nd_experiment NE
                INNER JOIN chado.nd_experiment_project NEP on NEP.nd_experiment_id = NE.nd_experiment_id
              WHERE NEP.project_id = :project_id
            )
          ";
          db_query($sql, $args);
        }
      }
      else {
        $sql = "
          SELECT COUNT(DISTINCT NE.nd_geolocation_id)
          FROM chado.nd_experiment NE
            INNER JOIN chado.nd_experiment_project NEP on NEP.nd_experiment_id = NE.nd_experiment_id
          WHERE NEP.project_id = :project_id
        ";
        $num = db_query($sql, $args)->fetchField();
        mcl_print("The number of nd_geolocation : $num", 2, 1);
      }

      // 4. Deleting from project.
      if ($delete && $flag_project) {
        mcl_print("Deleting projects in project table", 2, 1);
        $sql = "DELETE FROM chado.project WHERE project_id = :project_id";
        db_query($sql, $args);
      }
    }
  } catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
  }
  mcl_print("DONE", 2, 1.3);
}

/**
 * Callback for mcl-list-cvterm.
 */
function drush_mcl_list_cvterm($cv_name) {

  // Gets the cv.
  $cv = MCL_CHADO_CV::getCV($cv_name);
  if (!$cv) {
    mcl_print("Error : The cv ($cv_name) does not exist", 1, 3);
    exit;
  }

  $sql = "SELECT C.* FROM {chado.cvterm} C WHERE C.cv_id = :cv_id ORDER BY C.name";
  $results = db_query($sql, array(":cv_id" => $cv->getCvID()));
  $table = '';
  $num_cvterms = 0;
  while ($obj = $results->fetchObject()) {
    $table .= sprintf("%d %s\t%d\t%s\n", $cv->getCvID(), $cv_name, $obj->cvterm_id, $obj->name);
    $num_cvterms++;
  }
  if ($table) {
    $headers = sprintf("%05s %s\t%s\t%s\n", 'cv_id', 'v_name', 'cvterm_id', 'cvterm_name');
    mcl_print($headers . $table, 0, 2.2, FALSE);
    mcl_print("$num_cvterms of cvterms belong to $cv_name", 1, 0.3);
  }
  else {
    mcl_print("No cvterms belong to $cv_name", 1, 2.3, TRUE);
  }
}

/**
 * Callback for uploading test data.
 */
function drush_mcl_upload_test_data($username) {

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Test data file is stored in DATA_FILES.
  $module_path = drupal_get_path('module', 'mcl');
  $filename = 'TEST_MCL_DATA.xlsx';
  $test_data =  "$module_path/DATA_FILES/$filename";
  if (!file_exists($test_data)) {
    print "\n\nTest data file not found in /DATA_FILES/$filename";
    exit;
  }

  // Check pub ID (1) and add 'mcl_pub' if pub ID (1) dose not exist.
  $pub = CHADO_PUB::byKey(array('pub_id' => 1));
  if (!$pub) {
    $pub = CHADO_PUB::byKey(array('uniquename' => 'mcl_pub'));
    if (!$pub) {
      $fields = array(
        'title'       => 'mcl_pub',
        'uniquename'  => 'mcl_pub',
        'pub_id'      => 1,
        'type_id'     => 1,
      );
      db_insert('chado.pub')
        ->fields($fields)
        ->execute();
    }
  }

  // Upload the test file.
  $param_json = '{"cv":"mcl_test_cv_bp"}';
  drush_mcl_upload_data($username, $test_data, $param_json);
}

/**
 * Callback for deleting test data.
 */
function drush_mcl_delete_test_data() {

  $sqls = array(
    "SET search_path TO chado",
    "DELETE FROM chado.phenotype WHERE attr_id IN (SELECT cvterm_id FROM chado.cvterm WHERE cv_id IN (SELECT cv_id FROM chado.cv WHERE name LIKE 'mcl_test_%'))",
    "DELETE FROM chado.db WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.cv WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.cvterm WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.dbxref WHERE accession LIKE 'mcl_test_%'",
    "DELETE FROM chado.synonym WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.contact WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.feature WHERE uniquename LIKE 'mcl_test_%'",
    "DELETE FROM chado.project WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.featuremap WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.stock WHERE uniquename LIKE 'mcl_test_%'",
    "DELETE FROM chado.nd_geolocation WHERE description LIKE 'mcl_test_%'",
    "DELETE FROM chado.library WHERE name LIKE 'mcl_test_%'",
    "DELETE FROM chado.eimage WHERE eimage_data LIKE 'mcl_test_%'",
    "DELETE FROM chado.genotype WHERE uniquename LIKE 'mcl_test_%'",
    "DELETE FROM chado.organism WHERE genus LIKE 'mcl_test_%' AND species LIKE 'mcl_test_%'",
    "DELETE FROM chado.nd_experiment WHERE nd_experiment_id IN (SELECT nd_experiment_id FROM chado.nd_experimentprop WHERE value like 'mcl_test_%')",
    "DELETE FROM chado.pub WHERE title = 'mcl_test_pub'",
  );
  foreach ($sqls as $sql) {
    db_query($sql);
  }
  mcl_print("Done deleteing test data", 1, 3);
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
 * Callback for submitting a uploading job.
 *
 * @param string $username
 * @param string $filepath
 */
function drush_mcl_submit_uploading_job($username, $filepath) {

  // Checks the filepath.
  mcl_check_filepath($filepath);

  // Checks the username.
  $mcl_user = mcl_check_username($username);

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

  // Prepares a uploading job.
  $mcl_job = mcl_prepare_uploading_job($mcl_user, $filepath, $working_dir, $job_name, FALSE);
  if ($mcl_job) {
    mcl_print("Error : Failed to prepare a uploading job", 1, 3);
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
function drush_mcl_upload_data($username, $filepath, $param_json = '') {

  // Checks the filepath.
  mcl_check_filepath($filepath);

  // Checks the username.
  $mcl_user = mcl_check_username($username);

  // Checks the working_dir option.
  $working_dir = drush_get_option('working_dir');
  if ($working_dir) {
    if (!mcl_check_working_dir($working_dir)) {
      exit;
    }
  }

  // Gets the job_name option.
  $job_name = drush_get_option('job_name');

  // Gets the transaction option.
  $no_transaction = drush_get_option('no_transaction');
  $trans = ($no_transaction) ? FALSE : TRUE;

  // Check the move_input option.
  $move_input = drush_get_option('move_input');
  $move_input = ($move_input) ? TRUE : FALSE;

  // Adds the parameters from the 3rd argument.
  $param = array();
  if ($param_json) {
    $param = json_decode($param_json, TRUE);
  }

  // Adds parameters from the file.
  $param_file = drush_get_option('param');
  if ($param_file) {
    if (!file_exists($param)) {
      mcl_print("Error : The parameter file doese not exist", 1, 3);
      exit;
    }
    $contents = file_get_contents($param_file);
    $tmp = json_decode($contents, TRUE);
    if (!is_array($param)) {
      mcl_print("Error : The parameter file doese not hava valid JSON string", 1, 3);
      exit;
    }
    foreach ($tmp as $key => $value) {
      $param[$key] = $value;
    }
  }

  // Prepares a uploading job.
  $mcl_job = mcl_prepare_uploading_job($mcl_user, $filepath, $move_input, $trans, $working_dir, $job_name, $param);
  if (!$mcl_job) {
    mcl_print("Error : Failed to prepare a uploading job", 1, 3);
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
  }
  else {
    $job_id = $mcl_job->getJobID();
    $opt_working_dir  = ($working_dir) ? " --working_dir=$working_dir " : '';
    $opt_move_input   = ($move_input) ? ' --move_input ' : '';
    $opt_transaction  = ($trans) ? '' : ' --no_transaction ';

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
 * Prepares for a uploading job.
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

  // Prepares for a uploading job.
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

  // Checks arguments.
  mcl_check_module($module);
  mcl_check_schema($schema);
  mcl_check_empty_arg('dump_file', $dump_file);
  mcl_check_empty_arg('table_file', $table_file);

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

  // Checks the overwrite option.
  $flag_overwrite = drush_get_option('overwrite');

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
    mcl_print("The class files are generated in to :", 1, 1, FALSE);
    mcl_print($tmp_dir, 1, 1);
    mcl_print("Please copy them over to the class directory", 1, 4);
  }
  else {
    mcl_print("Failed to generate class files", 1, 4);
  }
}

/**
 * Callback for fixing date on prop table.
 *
 * @param string $cvterm_name.
 * @param string $cv_name.
 * @param string $table_name.
 */
function drush_mcl_fix_date($cv_name, $cvterm_name, $table_name) {

  // Checks the cv.
  if (!MCL_CHADO_CV::checkCV(NULL, $cv_name)) {
    mcl_print("Error : The cv ($cv_name) does not exist", 1, 1);
    exit;
  }

  // Checks the cvterm.
  if (!MCL_CHADO_CVTERM::checkCvterm(NULL, $cv_name, $cvterm_name)) {
    mcl_print("Error : The cvterm ($cvterm_name) does not exist", 1, 1);
    exit;
  }

  // Checks the table.
  if (!db_table_exists('chado.' . $table_name)) {
    mcl_print("Error : The table ($table_name) does not exist", 1, 1);
    exit;
  }

  // Gets MCL_CHADO_CVTERM.
  $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);

  // Fixes the date.
  if ($table_name == 'phenotype') {
    _mcl_fix_date_phenotype($cvterm, $table_name);
  }
  else if (preg_match("/prop$/", $table_name)) {
    _mcl_fix_date_prop($cvterm, $table_name);
  }
  mcl_print("DONE", 1, 3);
}

/**
 * Fixes the date on phenotype table.
 *
 * @param MCL_CHADO_CVTERM $cvterm
 * @param string $table_name
 */
function _mcl_fix_date_phenotype(MCL_CHADO_CVTERM $cvterm, $table_name) {

  // Gets all phenotype.
  $sql = "
    SELECT DISTINCT P.*
    FROM {chado.phenotype} P
    WHERE P.attr_id = :cvterm_id
  ";
  $results = db_query($sql, array(':cvterm_id' => $cvterm->getCvtermID()));
  $updates = '';
  while ($obj = $results->fetchObject()) {
    $phenotype_id = $obj->phenotype_id;
    $uniquename   = $obj->uniquename;
    $value        = $obj->value;
    if (preg_match("/^(\d\d\d\d\d)$/", $value, $matches)) {

      // Gets the date in number and converts the date (number to string
      $date_num = $matches[1];
      $date_str = date("n/j/Y", mktime(0, 0 , 0 , 1, $date_num - 1, 1900));

      // Gets the uniquename.
      preg_match("/^(.*)_\d+$/", $uniquename, $matches);
      $prefix = $matches[1];
      $uniquename = $prefix . "_$date_str";
      $updates .= "-- phenotype_id = $phenotype_id : value = $value\n";
      $updates .= "UPDATE chado.phenotype SET uniquename = '$uniquename', value = '$date_str' WHERE phenotype_id = $phenotype_id;\n";
    }
  }
  print "-- UPDATE SQL\n$updates\n";
}

/**
 * Fixes the date on phenotype table.
 *
 * @param MCL_CHADO_CVTERM $cvterm
 * @param string $table_names
 */
function _mcl_fix_date_prop(MCL_CHADO_CVTERM $cvterm, $table_name) {

  // Fixes the date. Converting 5 digits to date format (xxxx/xx/xx).
  $primary_key = $table_name . '_id';
  $cvterm_id = $cvterm->getCvtermID();
  $sql = "
    SELECT $primary_key, value
    FROM chado.$table_name P
    WHERE P.type_id = $cvterm_id
  ";
  print "$sql\n";
  $results = db_query($sql);
  $updates = '';
  while ($row = $results->fetchAssoc()) {
    $value = $row['value'];
    $id    = $row[$primary_key];
    if (preg_match("/^(\d\d\d\d\d)$/", $value, $matches)) {
      $date_num = $matches[1];
      $date_str = date("Y/m/d", mktime(0, 0 , 0 , 1, $date_num - 1, 1900));
      $updates .= "-- $primary_key = $id : value = $value\n";
      $updates .= "UPDATE chado.$table_name SET value = '$date_str' WHERE $primary_key = $id;\n";
    }
  }
  print "-- UPDATE SQL\n$updates\n";
}

/**
 * Callback for fixing data.
 *
 * @param string $inc_file.
 * @param string $input_file.
 */
function drush_mcl_fix_data($inc_file, $input_file = NULL) {

  // Checks arguments.
  mcl_check_empty_arg('inc_file', $inc_file);

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
 * Callback for mcl-call-func.
 *
 * @param string $func_name.
 */
function drush_mcl_call_func($func_name) {

  // Checks arguments.
  mcl_check_function($func_name);

  // Checks the parameter option.
  $param = drush_get_option('param');
  $args = array();
  if ($param) {
    $args = json_decode($param);
  }

  // Call the function.
  call_user_func_array($func_name, $args);
  mcl_print("DONE", 1, 3);
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
  mcl_check_filepath($working_dir);

  // Checks for write permission.
  if (!is_writable($working_dir)) {
    mcl_print("Error : Cannot write on the working directory ($working_dir)", 1);
    mcl_print("Please change the write permission of the directory", 1, 3);
    return FALSE;
  }
  return TRUE;
}

/**
 * Checks the username.
 *
 * @param string $username
 *
 */
function mcl_check_username($username) {

  // Checks for MCL_USER.
  $mcl_user = MCL_USER::byKey(array('name' => $username));
  if (!$mcl_user) {
    mcl_print("Error : The username ($username) does not exist", 1, 3);
    exit;
  }
  return $mcl_user;
}

/**
 * Checks the job ID.
 *
 * @param integer $job_id
 *
 */
function mcl_check_job_id($job_id) {

  // Checks for an integer.
  if (!mcl_is_int($job_id)) {
    mcl_print("Error : Job ID ($job_id) must be an integer", 1, 3);
    exit;
  }

  // Checks for MCL_JOB.
  $mcl_job = MCL_JOB_UPLOAD::byID($job_id);
  if (!$mcl_job) {
    mcl_print("Error : Job ID ($job_id) does not exist", 1, 3);
    exit;
  }
  return $mcl_job;
}

/**
 * Checks the file ID.
 *
 * @param integer $file_id
 *
 */
function mcl_check_file_id($file_id) {

  // Checks for an integer.
  if (!mcl_is_int($file_id)) {
    mcl_print("Error : File ID ($file_id) must be an integer", 1, 3);
    exit;
  }

  // Checks for MCL_FILE.
  $mcl_file = MCL_FILE::byID($file_id);
  if (!$mcl_file) {
    mcl_print("Error : file ID ($file_id) does not exist", 1, 3);
    exit;
  }
  return $mcl_file;
}

/**
 * Checks the filepath.
 *
 * @param string $filepath
 */
function mcl_check_filepath($filepath) {

  // Checks the file path.
  if (!file_exists($filepath)) {
    mcl_print("Cannot find file\n\n\t$filepath", 1, 2);
    mcl_print("Please check file name or add full path to the filename", 1, 3);
    exit;
  }
  return TRUE;
}

/**
 * Checks the schema.
 *
 * @param string $schema
 *
 */
function mcl_check_schema($schema) {

  // Checks the schema for empty.
  if (!$schema) {
    mcl_print("ERROR: please provide a name of schema", 1);
    exit;
  }

  // Checks if the schema exists.
  $sql = "
    SELECT schema_name
    FROM information_schema.schemata
    WHERE LOWER(schema_name) = LOWER(:schema)
  ";
  // TODO:
  $schema_name = $schema;
  // $schema_name = db_query($sql, array(':schema' => $schema))->fetchField();
  return ($schema_name) ? TRUE : FALSE;
}

/**
 * Checks the function.
 *
 * @param string $function
 *
 */
function mcl_check_function($function) {

  // Checks the function name for empty.
  if (!$function) {
    mcl_print("ERROR: please provide a name of function", 1);
    exit;
  }

  // Checks if the function exists.
  if (!function_exists($function)) {
    mcl_print("ERROR: The function does not exist", 1);
    exit;
  }
}

/**
 * Checks the module.
 *
 * @param string $module
 *
 */
function mcl_check_module($module) {

  // Checks if module exists.
  if (!module_exists($module)) {
    mcl_print("Error : The module ($module) does not exist", 1, 3);
    exit;
  }
  return TRUE;
}

/**
 * Checks the argument for empty.
 *
 * @param string $key
 * @param string $value
 *
 * @return boolean
 */
function mcl_check_empty_arg($key, $value) {

  // Checks for empty value.
  if (empty($value)) {
    mcl_print("Error : $key is empty", 1, 3);
    exit;
  }
  return TRUE;
}

/**
 * Updatess residues in feature table.
 */
function _mcl_update_feature_residues() {
  $GLOBALS['verbose'] = FALSE;
  mcl_print("Updating feature.residues", 1, 1.2);
  _mcl_update_feature_residues_bracket();
  _mcl_update_feature_residues_with_code();
  mcl_print("DONE", 1, 3);
}

/**
 * Updatess residues in feature table.
 */
function _mcl_update_feature_residues_bracket() {
  mcl_print("Updating feature.residues with bracket", 1, 1.2);

  // Gets cvterms.
  $cvterm_id_genetic_marker = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker')->getCvtermID();
  $cvterm_id_five_prime     = MCL_CHADO_CVTERM::getCvterm('sequence', 'five_prime_flanking_region')->getCvtermID();
  $cvterm_id_three_prime    = MCL_CHADO_CVTERM::getCvterm('sequence', 'three_prime_flanking_region')->getCvtermID();
  $cvterm_id_allele         = MCL_CHADO_CVTERM::getCvterm('sequence', 'allele')->getCvtermID();
  mcl_print("genetic_marker : $cvterm_id_genetic_marker", 1);
  mcl_print("five_prime     : $cvterm_id_five_prime", 1);
  mcl_print("three_prime    : $cvterm_id_three_prime", 1);
  mcl_print("Allele         : $cvterm_id_allele", 1, 2);

  $sql = "SELECT feature_id FROM chado.feature WHERE residues ~ '\[' AND type_id = $cvterm_id_genetic_marker";
  $results = db_query($sql);
  $count = 0;
  while ($feature_id = $results->fetchField()) {
    $count++;
    $marker = MCL_CHADO_FEATURE::byKey(array('feature_id' => $feature_id));
    $residues = $marker->getResidues();
    $flag = MCL_CHADO_FEATURE::checkResidues(NULL, $residues);
    if (!$flag) {
      print "-- $feature_id The allele has invalid format or no IUPAC ($residues)\n";
    }
    else {
      $seq_info     = MCL_CHADO_FEATURE::parseResidues($residues);
      $five_prime   = $seq_info['five_prime'];
      $allele       = $seq_info['allele'];
      $three_prime  = $seq_info['three_prime'];
      $residues     = $seq_info['residues'];

      // Updates feature.residues.
      print "UPDATE chado.feature SET residues = '$residues' WHERE feature_id = $feature_id;\n";

      // Adds prop if not exist.
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_five_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_five_prime, '$five_prime');\n";
      }
      else {
        print "-- $feature_id has five prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_three_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_three_prime, '$three_prime');\n";
      }
      else {
        print "-- $feature_id has three prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_allele));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_allele, '$allele');\n";
      }
      else {
        print "-- $feature_id has allele\n";
      }
    }
  }
  mcl_print("$count of features were needed to be fixed", 1, 1.2);
}

/**
 * Updatess residues in feature table.
 */
function _mcl_update_feature_residues_with_code() {
  mcl_print("Updating feature.residues with IUPAC code", 1, 1.2);

  // Gets cvterms.
  $cvterm_id_genetic_marker = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker')->getCvtermID();
  $cvterm_id_five_prime     = MCL_CHADO_CVTERM::getCvterm('sequence', 'five_prime_flanking_region')->getCvtermID();
  $cvterm_id_three_prime    = MCL_CHADO_CVTERM::getCvterm('sequence', 'three_prime_flanking_region')->getCvtermID();
  $cvterm_id_allele         = MCL_CHADO_CVTERM::getCvterm('sequence', 'allele')->getCvtermID();
  mcl_print("genetic_marker : $cvterm_id_genetic_marker", 1);
  mcl_print("five_prime     : $cvterm_id_five_prime", 1);
  mcl_print("three_prime    : $cvterm_id_three_prime", 1);
  mcl_print("Allele         : $cvterm_id_allele", 1, 2);

  $sql = "
    SELECT F.feature_id, F.uniquename, F.residues
    FROM chado.feature F
    WHERE F.residues ~ '[R|Y|S|W|K|M|B|D|H|V]' AND F.type_id = $cvterm_id_genetic_marker
  ";
  $results = db_query($sql);
  $count = 0;
  while ($feature_obj = $results->fetchObject()) {
    $feature_id   = $feature_obj->feature_id;
    $residues     = $feature_obj->residues;
    $num_matches  = preg_match_all("/[R|Y|S|W|K|M|B|D|H|V]/", $residues, $matches);
    if ($num_matches == 1) {
      $count++;
      preg_match("/(.*)([R|Y|S|W|K|M|B|D|H|V])(.*)$/", $residues, $matches);
      $five_prime   = $matches[1];
      $iupac_code   = $matches[2];
      $three_prime  = $matches[3];
      $bases        = MCL_CHADO_FEATURE::getBases($iupac_code);
      print "\n-- Residues ($iupac_code): $residues\n";

      // Adds prop if not exist.
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_five_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_five_prime, '$five_prime');\n";
      }
      else {
        print "-- $feature_id has five prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_three_prime));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_three_prime, '$three_prime');\n";
      }
      else {
        print "-- $feature_id has three prime\n";
      }
      $prop = CHADO_FEATUREPROP::byKey(array('feature_id' => $feature_id, 'type_id' => $cvterm_id_allele));
      if (!$prop) {
        print "INSERT INTO chado.featureprop (feature_id, type_id, value) VALUES ($feature_id, $cvterm_id_allele, '$bases');\n";
      }
      else {
        print "-- $feature_id has allele\n";
      }
    }
    else {
      print "-- Error : The residues of $feature_id has $num_matches IUPAC codes.\n";
    }
  }
  mcl_print("$count of features were needed to be fixed", 1, 1.2);
}

/**
 * Updatess residues in feature table.
 */
function _mcl_fix_phenotype_value() {
  mcl_print("Fixing phenotype data for precision", 1, 1.2);

  // Gets all numeric numbers.
  $sql = "
    SELECT *
    FROM {chado.phenotype}
    WHERE value ~ '\.\d+{3}$'
  ";
  $results = db_query($sql);
  $phenotypes = array();
  while ($obj = $results->fetchObject()) {
    $new_value = round($obj->value, 3, PHP_ROUND_HALF_UP);

    $phenotypes[$obj->phenotype_id] = array(
      'uniquename'      => $obj->uniquename,
      'value'           => $obj->value,
      'new_value'       => $new_value,
      'new_uniquename'  => $obj->attr_id . '.' . $new_value,
    );
  }
  print_r($phenotypes);


  mcl_print("DONE", 1, 3);
}

/**
 * Callback for mcl-debug command.
 */
function drush_mcl_debug() {
  mcl_print("DEBUG", 1, 3);
  $project_id = 590;

  // Test data file is stored in DATA_FILES.
  $module_path = drupal_get_path('module', 'mcl');
  $filename = 'sc_genotype_2016.txt';
  $filepath =  "$module_path/DATA_FILES/$filename";
  if (!file_exists($filepath)) {
    print "\n\nTest data file not found in /DATA_FILES/$filename";
    exit;
  }
  if (!($fdr = fopen($filepath, 'r'))) {
    mcl_print("Error : Cannot open file ($filepath)", 1, 3);
    exit;
  }
  $header_flag  = TRUE;
  $error_flag   = FALSE;
  $markers      = array();
  $stocks       = array();
  $genotypes    = array();
  $lines        = array();
  while ($cur_line = fgets($fdr)) {
    $cur_line = trim($cur_line);
    if (!$cur_line) {
      continue;
    }
    $tmp = explode("\t", $cur_line);
    if ($header_flag) {
      $header_flag = FALSE;
      for ($i = 1; $i < sizeof($tmp); $i++) {
        $markers[$i] = $tmp[$i];
      }
    }
    else {
      $lines []= $cur_line;
      $uniquename = $tmp[0];
      $stock = MCL_CHADO_STOCK::byKey(array('uniquename' => $uniquename));
      if ($stock) {
        $stocks[$uniquename] = $stock->getStockID();
      }
      else {
        print "$uniquename does not exist in stock\n";
        $error_flag = TRUE;
      }

      for ($i = 1; $i < sizeof($tmp); $i++) {
        if (trim($tmp[$i])) {
          $genotype_str = str_replace('|', '', $tmp[$i]);
          if (!array_key_exists($genotype_str, $genotypes)) {
            $genotype = MCL_CHADO_GENOTYPE::byKey(array('uniquename' => $genotype_str));
            if (!$genotype) {
              $genotype = MCL_CHADO_GENOTYPE::addGenotype(NULL, $genotype_str, 990, $genotype_str);
            }
            $genotypes[$genotype_str] = $genotype->getGenotypeID();
          }
        }
      }
    }
  }
  fclose($fdr);
  if (!$error_flag) {
    foreach ($lines as $line) {
      $tmp = explode("\t", $line);
      $uniquename = trim($tmp[0]);
      $stock_id = $stocks[$uniquename];
      if (!$stock_id) {
        print "Error: " . $uniquename . " cannot find stock_id\n";
      }

      for ($i = 1; $i < sizeof($tmp); $i++) {
        $feature_id = $markers[$i];
        if ($tmp[$i]) {
          $genotype_str = str_replace('|', '', $tmp[$i]);
          $genotype_id = $genotypes[$genotype_str];
          print "INSERT INTO chado.genotype_call (project_id, stock_id, feature_id, genotype_id) VALUES ($project_id, $stock_id, $feature_id, $genotype_id);\n";
        }
      }
    }
  }
  else {
    print "\nError\n\n";
  }
  mcl_print("DONE", 1, 3);
}