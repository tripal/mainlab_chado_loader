<?php
/**
 * @file
*/
/**
 * MCL Uploading Data form.
*
* @param array $form
* @param array $form_state
* @param string $op
* @param integer $job_id
*/
function mcl_upload_data_form($form, &$form_state) {
  global $user;

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Upload Data.
  $form['upload_data'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Uploading Data',
  );

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('user_id' => $user->uid));
  if (!$mcl_user) {
    $form['upload_data']['#collapsed'] = FALSE;
    $form['upload_data']['err'] = array(
      '#markup' => "You don't have the permission to upload data. Please ask site manager to get the permission.",
    );
  }
  else {

    // Upload Data.
    $desc = "MCL data uploading phases.<br /><br />
    <p>Phase 1 : checks syntax errors.<br />
       It checks syntax errors such as missing columns, missing data on required columns, miss-spelled
       column name and so on.</p>
    <p>Phase 2 : checks data errors.<br />
       The data errors are the errors on data in database. For instance, genus and species are
       listed in stock data template. MCL checks if organism_id for these genus and species
       exists in organism table. If not found, it throws an error and it adds an appropriate error
       message to the error log file.</p>
    <p>Phase 3 : uploads data templates.<br />
       After checking errors, it finally starts uploading data template into chado schema.</p>
    <p>During the uploading phases, MCL creates several different types of log files and stores in the
    job directory. MCL seizes the process if it finds an error and ask a user to fix the data. The
    user goes though the all log files to find the errors and fix them. After fixing the error,
    the user come back to the \"Upload Job\" page to re-upload the fixed data file. Then, perform
    re-run job.</p>
    ";
    $form['upload_data']['desc'] = array(
      '#markup' => $desc,
    );

    // Adds the job list table.
    $form['job_list'] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => FALSE,
      '#collapsible'  => TRUE,
      '#title'        => 'Uploading Jobs',
    );
    $form['job_list']['job_table'] = array(
      '#markup' => _mcl_get_uploading_job_table(),
    );

    // Adds the add form.
    _mcl_get_add_job_form($form, $mcl_user);
  }

  $form['home_link'] = array(
    '#markup' => "<div style=''><em>Returns to <a href='/mcl'>MCL home</a></em></div>",
  );

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-upload-data-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns a list of all uploading jobs in a table.
 *
 * @return string
 */
function _mcl_get_uploading_job_table() {
  global $user;

  // Sets the headers.
  $headers = array(
    array('data' => 'Job ID', 'style' => 'width:70px'),
    array('data' => 'Status', 'style' => 'width:60px'),
    array('data' => 'Name', 'style' => ''),
    array('data' => 'Filesize', 'style' => 'width:90px'),
    array('data' => 'Last Run', 'style' => 'width:100px'),
    array('data' => 'Submit Date', 'style' => 'width:100px'),
    array('data' => 'Details', 'style' => 'width:40px'),
    array('data' => 'Action', 'style' => 'width:40px'),
  );

  // Gets all jobs.
  $mcl_jobs = MCL_JOB::getJobsByUser($user->uid);
  $rows = array();
  foreach ($mcl_jobs as $mcl_job) {
    $job_id = $mcl_job->getJobID();

    // Creates 'Delete' link.
    $confirm_attr = array(
      'attributes' => array(
        'id' => 'delete_job_' . $job_id,
        'class' => array('use-ajax', 'mcl-confirm'),
      )
    );
    $delete_link = l('delete', "mcl/delete_job/$job_id", $confirm_attr);

    // Sets status.
    $status = $mcl_job->getStatusLabel();
    $run_lock = $mcl_job->getJobDir() . '/run.lock';
    if (file_exists($run_lock)) {
      $status = "Running";
    }

    // Sets the last run date.
    $last_run = $mcl_job->getPropByKey('last_run');
    $last_run_date = '<em>N/A</em>';
    if ($last_run) {
      $tmp = explode(" ", $last_run);
      $d = $tmp[0];
      $t = $tmp[1];
      $last_run_date = "<span title='$t'>$d</span>";
    }

    // Sets the submit date.
    $submit = $mcl_job->getSubmitDate();
    $submit_date = '<em>N/A</em>';
    if ($submit) {
      $tmp = explode(" ", $submit);
      $d = $tmp[0];
      $t = $tmp[1];
      $submit_date = "<span title='$t'>$d</span>";
    }

    // Gets filesize.
    $file_id = $mcl_job->getFileID();
    $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
    $filesize = $mcl_file->getFilesize();

    // Adds the row.
    $row = array(
      $job_id,
      $status,
      $mcl_job->getName(),
      mcl_format_bytes($filesize),
      $last_run_date,
      $submit_date,
      l('view', "/mcl/job_view/$job_id"),
      $delete_link,
    );
    $rows []= $row;
  }

  // Gets the upload_job information.
  $table_vars = array(
    'header'      => $headers,
    'rows'        => $rows,
    'empty'       => t("You don't have any uploading job."),
    'attributes'  => array(),
  );
  return theme('table', $table_vars);
}

/**
 * Adds form elements for adding a job.
 *
 * @param array $form
 * @param MCL_USER $mcl_user
 */
function _mcl_get_add_job_form(&$form, MCL_USER $mcl_user) {
  global $user;

  // Adds a add_upload_job form.
  $form['add_upload_job'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Add Upload Job',
  );
  $form['add_upload_job']['name'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Job Name',
    '#description'  => "Please specify the name of a job.",
    '#required'     => TRUE,
  );
  $form['add_upload_job']['upload_file'] = array(
    '#type'               => 'managed_file',
    '#title'              => 'Data template file',
    '#description'        => "Please provide a data template file.",
    '#upload_location'    => $mcl_user->getUserDirURL(),
    '#required'           => TRUE,
    '#progress_indicator' => 'bar',
    '#upload_validators'  => array(
      'file_validate_extensions' => array('xls xlsx zip csv'),
    ),
  );
  $form['add_upload_job']['advanced'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Advanced options',
    '#attributes'   => array('style' => 'width:100%;'),
  );
  $markup = "Please provide job paramters in a pair of key and value in each row in the textarea below.<br />(e.g)<br /><div style='margin-left:15px;'>cv::MAIN<br />callbacks::[mcl-run 3]</div><br />";
  $form['add_upload_job']['advanced']['job_params_desc'] = array(
    '#markup' => $markup,
  );
  $form['add_upload_job']['advanced']['job_params'] = array(
    '#type'         => 'textarea',
    '#rows'         => 4,
    '#attributes'   => array('style' => 'width:100%;'),
    '#description'  => "Please provide job parameters.",
  );
  $form['add_upload_job']['advanced']['no_transaction_cb'] = array(
    '#type'           => 'checkbox',
    '#title'          => 'No transaction',
    '#description'    => "Check if the size of your uploading file is large.",
    '#default_value'  => FALSE,
  );
  $form['add_upload_job']['email_cb'] = array(
    '#type'           => 'checkbox',
    '#title'          => 'Email notification',
    '#description'    => "Check if you'd like to recieve email notification when the job is completed.",
    '#default_value'  => FALSE,
  );
  $form['add_upload_job']['add_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'add_btn',
    '#value'  => 'Add a uploading job',
  );
}

/**
 * Parses the job parameters.
 *
 * @param string $job_params
 *
 * @return array
 */
function _mcl_get_add_job_form_get_job_param($job_params_str) {
  $param = array();
  if (trim($job_params_str)) {
    $job_params = preg_split("/((\r?\n)|(\r\n?))/", $job_params_str);
    $tmp = array();
    foreach ($job_params as $job_param_str) {
      $job_param = preg_split("/::/", $job_param_str);
      if (sizeof($job_param) == 2) {
        $key = trim($job_param[0]);
        $val = trim($job_param[1]);
        if ($key && $val) {
          $param[$key] = $val;
        }
      }
    }
  }
  return $param;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_upload_data_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_upload_data_form_validate($form, &$form_state) {
  global $user;

  // Checks job name for duplication.
  $name = trim($form_state['values']['add_upload_job']['name']);
  $mcl_job = MCL_JOB::byKey(array('name' => $name, 'user_id' => $user->uid));
  if ($mcl_job) {
    form_set_error('add_upload_job][name', t("The job name '$name' exists. Please change the name of the job."));
    return;
  }
}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_upload_data_form_submit($form, &$form_state) {
  global $user;

  // Gets MCL_USER.
  $mcl_user   = MCL_USER::byKey(array('user_id' => $user->uid));
  $user_name  = $mcl_user->getName();
  $user_dir   = $mcl_user->getUserDir();

  // Gets the values from the form_state.
  $job_params = trim($form_state['values']['add_upload_job']['advanced']['job_params']);
  $job_name   = trim($form_state['values']['add_upload_job']['name']);
  $email_cb   = $form_state['values']['add_upload_job']['email_cb'];

  // Parses the job parameters.
  $param = _mcl_get_add_job_form_get_job_param($job_params);

  // Adds 'email' option.
  $param['send_email'] = $email_cb;

  // Gets the Drupal file object.
  $drupal_file  = file_load($form_state['values']['add_upload_job']['upload_file']);

  // Create the user temp directory.
  $user_tmp_dir = "$user_dir/tmp";
  mcl_create_dir($user_tmp_dir);

  // Moves the file to the user temp directory.
  $dest_filepath    = "$user_tmp_dir/" . $drupal_file->filename;
  $target_filepath  = drupal_realpath($drupal_file->uri);
  $cmd              = "mv $target_filepath $dest_filepath";
  exec($cmd);

  // Removes from file_managed.
  db_delete('file_managed')
    ->condition('fid', $drupal_file->fid, '=')
    ->execute();

  // Gets Drupal user directory.
  $drupal_user_directory = dirname($mcl_user->getUserDir());

  // Creates a MCL_JOB_UPLOAD and gets job ID.
  $details = array(
    'name'        => $job_name,
    'type'        => 'upload',
    'class_name'  => 'MCL_JOB_UPLOAD',
    'status'      => 0,
    'user_id'     => $user->uid,
    'submit_date' => date("Y-m-d G:i:s"),
    'prop'        => json_encode(array('working_dir' => $drupal_user_directory)),
    'param'       => json_encode($param),
  );
  $mcl_job = new MCL_JOB_UPLOAD($details);
  if ($mcl_job->insert()) {
    $job_id = $mcl_job->getJobID();

    // Creates a MCL_File.
    $details = array(
      'filename'    => basename($dest_filepath),
      'filepath'    => $dest_filepath,
      'uri'         => '',
      'filesize'    => filesize($dest_filepath),
      'user_id'     => $user->uid,
      'job_id'      => $job_id,
      'submit_date' => date("Y-m-d G:i:s"),
    );
    $upload_file = new MCL_FILE($details);
    if ($upload_file->insert()) {

      // Updates the job properties.
      $mcl_job->setFileID($upload_file->getFileID());
      if ($mcl_job->update()) {

        // Gets 'no_transaction' option.
        $opt_no_transaction = '';
        $no_transaction_cb = trim($form_state['values']['add_upload_job']['advanced']['no_transaction_cb']);
        if ($no_transaction_cb) {
          $opt_no_transaction = '--no_transaction';
        }

        // Runs the job.
        $drush = mcl_get_config_setting('mcl_drush_binary');
        $cmd = "$drush mcl-rerun-job $job_id $dest_filepath --move_input $opt_no_transaction > /dev/null 2>/dev/null  & ";
        $pid = exec($cmd, $output, $return_var);
        if ($return_var) {
          drupal_set_message("Failed o add an uploading job");
        }
      }
      else {
        drupal_set_message("Failed o add an uploading job : Error on updating job property");
      }
    }
    else {
      drupal_set_message("Failed o add an uploading job : Error on adding a file");
    }
  }
  else {
    drupal_set_message("Failed o add an uploading job : Error on adding a job");
  }

  // Re-direct to "Data Uploading" page.
  $form_state['redirect'] = 'mcl/upload_data';
}
