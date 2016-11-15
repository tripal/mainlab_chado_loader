<?php

/**
 * Checks configuration variables in Mainlab Chado Loader.
 *
 *
 */
function mcl_check_settings() {
  $issues = '';

  $dir = 'sites/default/files/mcl';
  $dir = 'sites/default/files/mcl/user';
  $dir = 'sites/default/files/mcl/job';

  // Checks working directory.
//  $working_dir  = mcl_get_config_setting('working_dir');
//  if (!is_writable($working_dir)) {
//    $issues .= '<li>' . t("The working directory, $working_dir, does not exist or is not writeable by the web server.") . '</li>';
//  }
  if ($issues) {
    drupal_set_message($issues);
  }
}

/**
 * Prints the message on screen.
 *
 * @param string $msg
 * @param integer $indent
 */
function mcl_print($msg, $indent = 0, $line_feed = 1, $period_flag = TRUE) {

  // Adds indents.
  $space = str_repeat(' ', $indent  * 4);

  // Adds new lines.
  $new_lines = str_repeat("\n", $line_feed);

  // Adds a period.
  $period = ($period_flag) ? '.' : '';
  print $space . $msg . $period . $new_lines;
}

/**
 * Empties a dirctory
 *
 * @param string $dir
 *
 * @return boolean
 */
function mcl_empty_dir($dir) {
  if (mcl_remove_dir($dir)) {
    return mcl_create_dir($dir);
  }
  return FALSE;
}

/**
 * Creates a dirctory
 *
 * @param string $dir
 *
 * @return boolean
 */
function mcl_create_dir($dir) {

  // Creates a dirctory.
  if(!file_prepare_directory($dir, FILE_CREATE_DIRECTORY)) {
    drupal_set_message("Cannot create directory : $dir");
    watchdog('mcl', "Fail to create directory: %dir.", array('%dir' => $dir), WATCHDOG_ERROR);
    return FALSE;
  }
  return TRUE;
}

/**
 * Removes all files and sub-diretories.
 *
 * @param $target_dir
 *
 * @return boolean TRUE|FALSE
 */
function mcl_remove_dir($target_dir) {

  if (is_dir($target_dir)) {

    // Gets the files in the target directory.
    $objects = scandir($target_dir);
    foreach ($objects as $object) {

      // Ignores the . and .. objects
      if ($object == "." or $object == "..") {
        continue;
      }

      // If the object is another directory then recurse.
      if (filetype($target_dir . "/" . $object) == "dir") {
        mcl_remove_dir($target_dir . "/" . $object);
      }
      // Otherwise delete the file.
      else {
        unlink($target_dir . "/" . $object);
      }
    }
    reset($objects);

    // Finaly, removes the directory.
    return rmdir($target_dir);
  }
  return false;
}

/**
 * Returns MCL public folder URL.
 *
 * @return string
 */
function mcl_get_public_URL() {
  return file_create_url('public://') . '/mcl';
}

/**
 * Returns MCL public folder URL.
 *
 * @param string $filepath
 *
 * @return string
 */
function mcl_get_rel_filepath($filepath) {

  // Gets the relative path to the file.
  $mcl_file_dir = mcl_get_config_setting('mcl_file_dir');
  return str_replace($mcl_file_dir, '', $filepath);
}

/**
 * Returns the all files under the provided directory.
 *
 * @param string $target_dir
 * @param string $pattern
 *
 * @return array of string
 */
function mcl_retrive_files($target_dir, $pattern) {
  $dir = new RecursiveDirectoryIterator($target_dir);
  $ite = new RecursiveIteratorIterator($dir);
  $files = new RegexIterator($ite, $pattern, RegexIterator::GET_MATCH);
  $file_list = array();
  foreach($files as $file) {
    $file_list = array_merge($file_list, $file);
  }
  return $file_list;
}

/**
 * Download all log files.
 *
 * @param integer $job_id
 */
function mcl_download_log($job_id) {

  // Gets MCL_UPLOAD_JOB.
  $mcl_job = MCL_JOB_UPLOAD::byKey(array('job_id' => $job_id));
  $log_root_dir = dirname($mcl_job->getLogDir());

  // Sets zip path and name.
  $filename = "mcl-$job_id-logs.zip";
  $filepath = "$log_root_dir/$filename";

  // Create a zip file.
  $zip = new ZipArchive;
  $zip->open($filepath, ZipArchive::OVERWRITE);

  $files = mcl_retrive_files($mcl_job->getLogDir(), '/.*\.log$/');
  foreach ($files as $file) {
    $file_name = basename($file);
    $rel_path = str_replace("$log_root_dir/", '', dirname($file));
    $zip->addFile($file, "$rel_path/$file_name");
  }
  $zip->close();

  // Attaches the zip file.
  header("Content-Type: application/zip");
  header("Content-Disposition: attachment; filename=$filename");
  readfile($filepath);
  exit();
}

/**
 * Download a file.
 *
 * @param integer $file_id
 */
function mcl_download_file($file_id) {

  // Gets MCL_FILE and the filepath
  $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
  $filepath = $mcl_file->getFilepath();
  $filename = basename($filepath);
  if (!file_exists($filepath)) {
    drupal_set_message("Error : $filepath not found");
    drupal_goto();
  }
  else {

    // Attaches the file.
    header("Content-Type: text/plain");
    header("Content-Disposition: attachment; filename=$filename");
    header('Content-Length: ' . filesize($filepath));
    readfile($filepath);
    exit();
  }
}

/**
 * Views log file.
 *
 * @param integer $job_id
 * @param string $filepath_str
 */
function mcl_view_log($job_id, $filepath_str) {

  // Gets MCL_UPLOAD_JOB.
  $mcl_job = MCL_JOB_UPLOAD::byKey(array('job_id' => $job_id));

  $filepath = $mcl_job->getLogDir() . '/' . str_replace(':', '/', $filepath_str);

   // Open the log file.
  $contents = "Log file not found [$filepath].";
  if (file_exists($filepath)) {
    $contents = file_get_contents($filepath);
  }
  //drupal_json_output($contents);
  header("Content-Type: text/plain");
  echo $contents;
  exit();
}
