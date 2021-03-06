<?php
/**
 * @file
*/
/**
 * MCL Job View form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_job_view_form($form, &$form_state, $job_id = NULL) {
  global $user;

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('user_id' => $user->uid));

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Adds job details.
  $form['job_view'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Job Details',
  );

  if ($job_id) {

    // Saves job ID.
    $form['job_id'] = array(
      '#type'   => 'value',
      '#value'  => $job_id,
    );

    // Gets MCL_JOB_UPLOAD.
    $mcl_job = MCL_JOB_UPLOAD::byKey(array('job_id' => $job_id));
    if ($mcl_job) {

      // Adds the table for job details.
      $form['job_view']['details'] = array(
        '#markup' => _mcl_get_detail_table($mcl_job),
      );

      // Adds re-run.
      $form['job_rerun'] = array(
        '#type'         => 'fieldset',
        '#collapsed'    => FALSE,
        '#collapsible'  => FALSE,
        '#title'        => 'Re-Run Job',
      );
      $form['job_rerun']['upload_file'] = array(
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
      $form['job_rerun']['rerun_btn'] = array(
        '#type'   => 'submit',
        '#name'   => 'rerun_btn',
        '#value'  => 'Re-Run the Job',
      );

      // Displays all log files of this job.
      $all_logs = _mcl_get_all_logs($mcl_job);

      // Adds the log fieldsets.
      $log_types = array('E', 'N', 'W', 'D');
      foreach ($log_types as $log_type) {
        if (array_key_exists($log_type, $all_logs)) {
          _mcl_add_logs($form, $mcl_job, $log_type, $all_logs[$log_type]);
        }
      }
    }
    else {
      $form['job_view']['error'] = array(
        '#markup' => "Error : Could not find job for $job_id",
      );
    }
  }
  else {
    $form['job_view']['error'] = array(
      '#markup' => "Error : Please specify job ID.",
    );
  }

  // Adds the link to 'data upload' page.
  $form['link_upload_page'] = array(
    '#markup' => "<div style=''><em>Returns to <a href='/mcl/upload_data'>the uploading data page</a></em></div>",
  );

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-job-view-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns all logs in an array.
 *
 * @param MCL_JOB_UPLOAD $mcl_job
 *
 * @return array
 */
function _mcl_get_all_logs(MCL_JOB_UPLOAD $mcl_job) {
  $all_logs = array();

  // Gets the path to the public folder.
  $public_path  = drupal_realpath('public://');
  $public_url   = file_create_url('public://');

  // Adds log files  (Warning / Errror / Dup-Data) in recent directory.
  $recent_dir = $mcl_job->getRecentLogDir('recent');
  $objects = scandir($recent_dir);
  foreach ($objects as $object) {
    if ($object === '.' or $object === '..') {
      continue;
    }
    if (is_dir("$recent_dir/$object")) {
      preg_match("/(\d+)-([A-Z])/", $object, $matches);
      $status_int = $matches[1];
      $type       = $matches[2];

      // Skips new data log.
      if ($type == 'N') {
        continue;
      }

      // Initialzes the array.
      if (!array_key_exists($type, $all_logs)) {
        $all_logs[$type] = array();
      }

      $files = file_scan_directory("$recent_dir/$object", "/\.log$/");
      $log_files = array();
      foreach ($files as $filepath => $file) {
        preg_match("/-([A-Za-z\_]+)\.log$/", $filepath, $matches2);
        $key = $matches2[1];
        $log_files []= array(
          'key'      => $key,
          'filepath' => $filepath,
          'log_file' => "recent:$object:" . basename($filepath),
        );
      }

      // Adds log files.
      if (!empty($log_files)) {
        $all_logs[$type][$status_int] = $log_files;
      }
    }

    // Adds log files.
    if (!empty($log_files)) {
      $all_logs[$type][$status_int] = $log_files;
    }
  }

  // Adds log file for new data.
  // Gets log (new data) files in 'new_data' directory.
  $new_data_dir = $mcl_job->getNewDataLogDir('new_data');
  $files        = file_scan_directory($new_data_dir, "/\.log$/");
  $log_files    = array();
  krsort($files);
  foreach ($files as $filepath => $file) {
    preg_match("/([A-Za-z\-\_]+)-(\d+-\d+-\d+)-(\d+-\d+-\d+)\.log$/", $filepath, $matches);
    $key  = $matches[1];
    $date = $matches[2];
    $time = $matches[3];
    $log_files []= array(
      'key'       => $key,
      'date'      => "$date $time",
      'filepath'  => $filepath,
      'log_file'  => 'new_data:' . basename($filepath),
    );
  }

  // Adds log files.
  if (!empty($log_files)) {

    // Sort by date-time.
    usort($log_files, "_mcl_cmp_log");
    $all_logs['N']['40'] = $log_files;
  }
  return $all_logs;
}

/**
 * Comparison function for sorting array.
 *
 * @param associ array $a
 * @param associ array $b
 *
 * @return boolean
 */
function _mcl_cmp_log($a, $b) {
  return strcmp($b['date'], $a['date']);
}

/**
 * Adds the error logs to the form.
 *
 * @param array $form
 * @param MCL_JOB $mcl_job
 * @param string $type
 * @param array $logs
 */
function _mcl_add_logs(&$form, MCL_JOB $mcl_job, $log_type, $logs) {
  $job_id     = $mcl_job->getJobID();
  $title      = $mcl_job->getMessageType($log_type) . ' Logs';
  $id_form    = 'log_' . $log_type;
  $id_viewer  = 'id_log_viewer_' . $log_type;
  $id_table   = 'id_table_' . $log_type;
  $id_anchor  = 'anchor_' . $log_type;

  // Adds the anchor.
  $form[$id_anchor] = array(
    '#markup' => "<a name='$id_anchor' style='display:block;position:relative;top:-40px;'></a>",
  );

  // Adds the log form.
  $form[$id_form] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => ($log_type != 'E'),
    '#collapsible'  => TRUE,
    '#title'        => $title,
  );

  // Creates a table for each status.
  $table = '';
  $total_row = 0;
  foreach ($logs as $status_int => $log_files) {
    $total_row += 2;

    // Populates the rows.
    $rows = array();
    foreach ($log_files as $log_file) {
      $total_row++;
      $url = "/mcl/ajax_view_log/$job_id/" . $log_file['log_file'];

      // Adds the link to view if the size of the log file is less than
      // 15 MB (15000000 B).
      $filepath = $log_file['filepath'];
      $link_view = '<em>N/A</em>';
      if (file_exists($filepath) && filesize($filepath) < 15000000) {
        $tmp_name = $log_file['key'];
        $link_view = "<a href='#$id_anchor' class='mcl_log' viewer-id='$id_viewer' tmp-name='[$tmp_name]' log-file='$url'>view</a>";
      }
      $row = array(
        $log_file['key'],
        $link_view,
      );
      if ($log_type == 'N') {
        $row []= $log_file['date'];
      }
      $rows []= $row;
    }

    // Table variables.
    $column_name = 'Template Name';
    if ($status_int == 20) {
      $column_name = 'Type';
    }

    // Adds headers.
    $headers = array(
      $column_name,
      array('data' => 'View', 'style' => 'width:28px;'),
    );
    if ($log_type == 'N') {
      $headers []= array('data' => 'Uploaded Date', 'style' => 'width:175px;');
    }

    // Sets table properties.
    $table_width = 'width:330px;';
    if ($log_type == 'N') {
      $table_width = 'width:470px;';
    }

    // Creates a talbe.
    $table_vars = array(
      'header'      => $headers,
      'rows'        => $rows,
      'empty'       => 'No logs',
      'attributes'  => array('style' => $table_width),
    );

    // Adds the caption.
    $caption = ucwords($mcl_job->trStatusIntToLabel($status_int));
    if (!preg_match("/[ND]/", $log_type)) {
      $table_vars['caption'] = "<b>$caption</b>";
    }
    $table .= theme('table', $table_vars);
  }

  // Creates two horizontal div and show log tale on the left and log
  // viewer on the right.
  $div = "
    <div id='$id_viewer' style='overflow-y:scroll;height:150px;width:98%;padding:0px;border:1px solid lightgray;'>
      <div style='margin:0px 0px 0px 5px;'><em>Please click <b>view</b> on the table</em>.<br /></div>
    </div>
    <div id='$id_table' style='margin-top:15px;width:98%;'>$table</div>
    <br clear='all'>";

  // Adds to the error logs.
  $form[$id_form]['table'] = array(
    '#markup' => $div,
  );
}

/**
 * Adds the job info. table.
 *
 * @parma MCL_JOB_UPLOAD $mcl_job
 *
 */
function _mcl_get_detail_table(MCL_JOB_UPLOAD $mcl_job) {
  global $base_url;
  global $base_root;

  // Adds the link to the uploaded file.
  $mcl_file = $mcl_job->getMCLFile();
  $link_template =  l('download',  'mcl/download_file/' . $mcl_file->getFileID());

  // Adds the link to download all log files in zip.
  $link_log = l('download all', 'mcl/download_log/' . $mcl_job->getJobID());

  // Gets the compete date.
  $complete_date = $mcl_job->getCompleteDate();
  if (!$complete_date) {
    $complete_date = '<em>N/A</em>';
  }

  // Sets status.
  $status = $mcl_job->getStatusLabel();
  $run_lock = $mcl_job->getJobDir() . '/run.lock';
  if (file_exists($run_lock)) {
    $status = "Running";
  }

  // Sets the last run date.
  $last_run = $mcl_job->getPropByKey('last_run');
  $last_run_date = ($last_run) ? $last_run : '<em>N/A</em>';

  // Adds job information.
  $rows = array();
  $rows []= array(array('data' => '<b>Job ID</b>', 'style' => 'width:160px;'), $mcl_job->getJobID());
  $rows []= array('<b>Name</b>', $mcl_job->getName());
  $rows []= array('<b>Data Template File</b>', $link_template);
  $rows []= array('<b>Status</b>', $status);

  // Adds progress row if not empty.
  $progress = $mcl_job->getProgress();
  if ($progress) {
    $rows []= array('<b>Progress</b>', $progress);
  }

  $rows []= array('<b>Last Run</b>', $last_run_date);
  $rows []= array('<b>Submit Date</b>', $mcl_job->getSubmitDate());
  $rows []= array('<b>Complete Date</b>', $complete_date);
  $rows []= array('<b>Log Files</b>', $link_log);

  // Table variables.
  $table_vars = array(
    'header'      => NULL,
    'rows'        => $rows,
    'attributes'  => array('style' => 'max-width:550px;'),
  );
  return theme('table', $table_vars);
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_job_view_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_job_view_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_job_view_form_submit($form, &$form_state) {
  global $user;

  // Gets MCL_USER.
  $mcl_user   = MCL_USER::byKey(array('user_id' => $user->uid));
  $user_name  = $mcl_user->getName();
  $user_dir   = $mcl_user->getUserDir();

  // Gets job_id.
  $job_id = $form['job_id']['#value'];

  // Gets the re-uploading file.
  $drupal_file = file_load($form_state['values']['job_rerun']['upload_file']);

  // Moves the file to the user temp space.
  $dest_filepath    = "$user_dir/tmp/" . $drupal_file->filename;
  $target_filepath  = drupal_realpath($drupal_file->uri);
  $cmd              = "mv $target_filepath $dest_filepath";
  exec($cmd);

  // Removes file form manage_file table.
  db_delete('file_managed')
  ->condition('fid', $drupal_file->fid, '=')
  ->execute();

  // Submits the re-uploading job.
  $cmd = "drush mcl-rerun-job $job_id $dest_filepath --move_input > /dev/null 2>/dev/null & ";
  $pid = exec($cmd, $output, $return_var);
  if ($return_var) {
    drupal_set_message("Failed to re-run the job");
  }

  // Re-direct to "Data Uploading" page.
  $form_state['redirect'] = "mcl/job_view/$job_id";
}
