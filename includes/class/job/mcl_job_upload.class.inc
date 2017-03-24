<?php
/**
 * The declaration of MCL_JOB_UPLOAD class.
 *
 */
class MCL_JOB_UPLOAD extends MCL_JOB {

 /**
  *  Class data members.
  */
  protected $upload_file      = NULL;
  protected $csv_dir          = NULL;
  private   $excludes         = array();
  private   $force            = FALSE;
  private   $cur_template     = '';

  /**
   * @see MCL_JOB::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);

    // Adds status for a uploading job.
    $this->status_label['20']         = 'copying';
    $this->status_label['30']         = 'syntax error';
    $this->status_label['40']         = 'data error';
    $this->status_label['60']         = 'uploading';
    $this->status_int['copying']      = 20;
    $this->status_int['syntax error'] = 30;
    $this->status_int['data error']   = 40;
    $this->status_int['uploading']    = 60;

    // Creates a MCL_FILE object for the uploaded file.
    $file_id = $this->getParamByKey('file_id');
    if ($file_id) {
      $this->upload_file = MCL_FILE::byKey(array('file_id' => $file_id));
    }

    // Sets job properties.
    $this->force = $this->getPropByKey('force');
    $this->csv_dir = $this->getPropByKey('csv_dir');

    // Creates folders for CSV files.
    if ($this->csv_dir && !file_exists($this->csv_dir)) {
      mcl_create_dir($this->csv_dir);
    }

    // Resets 'columns' in job properties.
    $this->setPropByKey('columns', array());

    // Resets new log files.
    if ($this->job_id) {
      $this->resetNewLogFiles();
    }
  }

  /**
   * @see MCL_JOB::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see MCL_JOB::insert()
   */
  public function insert() {
    if (parent::insert()) {
      // Creates folders for CSV files.
      $csv_dir = $this->job_dir . '/csv';
      if (!file_exists($csv_dir)) {
        mcl_create_dir($csv_dir);
      }
      $this->setCSVDir($csv_dir);
      return $this->update();
    }
    return FALSE;
  }

  /**
   * @see MCL_JOB::resetLogs()
   */
  public function resetLogs($template) {
    $this->cur_template = $template;
    $this->msgs = array();
  }

  /**
   * @see MCL_JOB::createNewLogs()
   */
  public function createNewLogs() {
    $dir_new_data = $this->getNewDataLogDir();
    $dir_prevous  = $this->getPreviousLogDir();
    $dir_recent   = $this->getRecentLogDir();

    // Creates 'new_data' and 'previous' folders if not exists.
    if (!mcl_create_dir($dir_prevous)) {
      return FALSE;
    }
    if (!mcl_create_dir($dir_new_data)) {
      return FALSE;
    }

    // Creates or re-create up recent folder.
    if (file_exists($dir_recent)) {
      $cmd = "rm -rf $dir_recent";
      exec($cmd, $output, $return_var);
      if ($return_var) {
        return FALSE;
      }
    }
    if (!mcl_create_dir($dir_recent)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see MCL_JOB::KeepLogs()
   */
  public function KeepLogs() {
    $dir_previous = $this->getPreviousLogDir();
    $dir_recent   = $this->getRecentLogDir();

    // Copies 'recent' folder to previous'.
    $target_dir = "$dir_previous/" . date("Y-m-d-G-i-s");
    $cmd = "cp -r $dir_recent $target_dir";
    exec($cmd, $output, $return_var);
    if ($return_var) {
        return FALSE;
    }
    return TRUE;
  }

  /**
   * @see MCL_JOB::noError()
   */
  public function noError() {
    if (array_key_exists('E', $this->msgs)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * @see MCL_JOB::updateMsg()
   */
  public function updateMsg($key, $msg, $line_no = 0) {
    if (!array_key_exists($key, $this->msgs)) {
      $this->msgs[$key] = array();
    }
    $line_info = ($line_no > 0) ? " at line $line_no\n" : "\n";
    $this->msgs[$key][$line_no] .= "$msg\n";
  }

  /**
   * @see MCL_JOB::writeLog()
   */
  public function writeLog() {
    $template     = $this->cur_template;
    $status       = $this->status;
    $dir_recent   = $this->getRecentLogDir();
    $dir_new_data = $this->getNewDataLogDir();
    $file_type    = $this->upload_file->getType();

    // Iterates through log messages and adds a line no.
    $messages_tmp = array();
    foreach ($this->msgs as $type => $lines) {
      if (empty($lines)) {
        continue;
      }

      // Sorts by line numbers.
      ksort($lines);
      foreach ($lines as $line_no => $msg) {

        // Adds the line number to the messages.
        if ($line_no == 1) {
          $messages_tmp[$type] .= ">Required Headers\n$msg\n";
        }
        else if ($line_no == 0) {
          $messages_tmp[$type] .= ">Template\n$msg\n";
        }
        else if ($line_no == -1) {
          $messages_tmp[$type] .= ">Required cvterms\n$msg\n";
        }
        else {
          $ln = ($file_type == 'excel') ? 'row ' . ($line_no - 1) : "ln. $line_no";
          $messages_tmp[$type] .= ">$ln --------------\n$msg\n";
        }
      }
    }

    // Creates log files for each type.
    try {

      foreach ($messages_tmp as $type => $msgs) {

        // Creates a folder.
        $log_folder = "$dir_recent/$status-$type";
        mcl_create_dir($log_folder);

        // Sets filename and filepath.
        $filename = "$status-$type-$template.log";
        $filepath = "$log_folder/$filename";

        // Opens the log file for writing.
        $fdw = fopen($filepath, 'w');
        if (!$fdw) {
          throw new Exception("Fail to open file for writing [$filepath]");
        }
        fputs($fdw, $msgs);
        mcl_print("-- Log file ($status-$type) for $template created", 2);
        fclose($fdw);

        // Keeps the all logs for new data for later use.
        if ($type == 'N') {
          $this->addNewLogFile($template, $filepath);
        }
      }
    }
    catch (Exception $e) {
      watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }

    // Shows the error message if exists.
    $error_flag = !$this->noError();
    if ($error_flag) {
      $label = $this->getStatusLabel();
      mcl_print("[$template] Error found at job status = $status ($label)", 1);
    }
    return !$error_flag;
  }

  /**
   * Adds a filepath of a log file for new data.
   *
   * @param string $template
   * @param string $filepath
   *
   * @return boolean
   */
  public function addNewLogFile($template, $filepath) {

    // Gets the new data logs.
    $new_data_logs = $this->getPropByKey('new_data_logs');
    if ($new_data_logs) {
      $new_data_logs[$template]= $filepath;
    }
    else {
      $new_data_logs = array($template => $filepath);
    }
    $this->setPropByKey('new_data_logs', $new_data_logs);
    return $this->update();
  }

  /**
   * Returns an array of the filepaths of logs file for new data.
   *
   * @return array
   */
  public function getNewLogFiles() {

    // Gets the new data logs.
    $new_data_logs = $this->getPropByKey('new_data_logs');
    if (!$new_data_logs) {
      $new_data_logs = array();
    }
    return $new_data_logs;
  }

  /**
   * Resets log files for new data.
   */
  public function resetNewLogFiles() {
    $this->setPropByKey('new_data_logs', '');
  }

  /**
   * @see MCL_JOB::getJobInfoTable()
   */
  public function getJobInfoTable() {

    // Gets MCL_FILE.
    $file = $this->getUploadFile();

    // Gets and set job information.
    $file_link = "No file found";
    if (file_exists($file->getFilepath())) {
      $file_link = l($file->getFilename(), file_create_url($file->getUri()));
    }
    $rows = array(
      array('<b>Name</b>', $this->getName()),
      array('<b>Type</b>', $this->getType()),
      array('<b>Status</b>', $this->getStatusLabel()),
      array('<b>Uploaded File</b>', $file_link),
      array('<b>Filesize</b>', $file->getFilesize()),
      array('<b>Submit Date</b>', $this->getSubmitDate()),
    );

    // Table variables.
    $table_vars = array(
      'header'      => NULL,
      'rows'        => $rows,
      'attributes'  => array('style' => 'max-width:500px;'),
    );
    return theme('table', $table_vars);
  }

  /**
   * @see MCL_JOB::getJobInfo()
   */
  public function getJobInfo() {

    // Gets directories.
    $dirs = "\n  Job     = " . $this->getJobDir() . "\n  Working = " . $this->getWorkingDir() .
            "\n  File    = " . $this->getFileDir() . "\n  CSV     = " . $this->getCSVDir() .
            "\n  Log     = " . $this->getLogDir() . "\n";

    // Adds base information.
    $info = array(
      'Job ID'      => $this->job_id,
      'Job Name'    => $this->getName(),
      'Status'      => $this->getStatusLabel(),
      'Directories' => $dirs,
      'Submit Date' => $this->getSubmitDate(),
    );

    // Adds re-run command.
    $rerun_cmd = $this->getPropByKey('rerun_cmd');
    if ($rerun_cmd) {
      $info['rerun_cmd'] = $rerun_cmd;
    }
    return $info;
  }

  /**
   * @see MCL_JOB::run()
   */
  public function run() {

    // Sets the transaction.
    $transaction = NULL;
    $trans = $this->getTransaction();
    if ($trans) {
      $transaction = db_transaction();
    }

    // Runs uploading job.
    try {
      $this->resetProgress();
      $job_id = $this->getJobID();

      // Initializes the job.
      if ($this->status == 10) {
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Job [" . $this->getJobID() . "] has been started", 1);
        mcl_print("----------------------------------", 1, 2, FALSE);
        $this->setProgress("The job has been started");
        $this->setStatus(20);
        $this->update();
      }

      // Extracts CVS files and copy them.
      if ($this->status == 20) {

        // Copies all CSV files to the CSV file directory.
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Phase I : Copying CSV files", 1);
        mcl_print("----------------------------------", 1, 1, FALSE);
        $this->setProgress("Copying CSV files ...");
        if ($this->_copyCSV()) {
          mcl_print("CSV file created", 1, 2);
          $this->setProgress("Done copying CSV files");
          $this->setStatus(30);
          $this->update();
        }
        else {
          throw new Exception("Error : Failed to copy CSV files.");
        }
      }

      // Runs syntax error check.
      if ($this->status == 30) {

        // Runs syntax error check each CSV files.
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Phase II : Running syntax error check", 1);
        mcl_print("----------------------------------", 1, 1, FALSE);
        $this->setProgress("Started running syntax error check");
        if ($this->_runErrorCheckSyntax()) {
          mcl_print("Syntax error check completed without error", 1, 2);
          $this->setProgress("Done syntax error check");
          $this->setStatus(40);
          $this->update();
        }
        else {
          throw new Exception("\n\tSyntax Error exists : please check error log file.\n\n\n");
        }
      }

      // Uploads the data file. The data error check is conducted before
      // uploading data.
      if ($this->status == 40) {

        // Uploads CSV files into database.
        mcl_print("==================================", 1, 1, FALSE);
        mcl_print("Phase III : Uploading data files", 1);
        mcl_print("----------------------------------", 1, 1, FALSE);
        $this->setProgress("Started uploading data template files");
        if ($this->_upload_csv()) {
          mcl_print("Data uploading completed without error", 1, 2);
          $this->setStatus(60);
          $this->update();
        }
        else {
          throw new Exception("\n\tUploading data failed : please check error log file.\n\n\n");
        }
      }

      // Sends the completion notifiation email.
      if ($this->status == 60) {

        // Sets the status to '100' and sends completion notifiation email.
        $this->setStatus(100);
        $this->update();
        $this->sendCompleteEmail();
        $this->setProgress('COMPLETED');
      }
    }
    catch (Exception $e) {

      // Sends an error notifiation email.
      $this->sendError();

      // If error occurs, rollbacks transactions.
      if ($transaction) {
        $transaction->rollback();
      }

      // Sets the status as 'FAILED'
      $this->setStatus(-200);
      $this->update();
      $this->setProgress('FAILED');

      // Keeps the error message.
      watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }

    // No error has occured. So copy the logs file for new data.
    $new_log_files = $this->getNewLogFiles();
    if ($new_log_files) {
      $date_str = date("Y-m-d-G-i-s");
      $dir_new_data = $this->getNewDataLogDir();
      foreach ($new_log_files as $template => $log_file) {
        $new_data_log = "$dir_new_data/$template" . '-' . $date_str . '.log';
        $cmd = "cp $log_file $new_data_log";
        exec($cmd);
      }
    }
    return TRUE;
  }

  /**
   * Sends an error notification email for the following errors.
   *   - syntax error
   *   - data integrity error
   *   - data uploading error
   *
   */
  private function sendError() {
    $err_msg = '';

    // Sends a syntax error email.
    if ($this->status == 30) {}

    // Sends a data uploading error email.
    else if ($this->status == 40) {}

    // Sends a data uploading error email.
    else if ($this->status == 50) {}

    // Sends an error message.
    if ($err_msg) {
      $this->sendErrorEmail($err_msg);
    }
  }

  /**
   * Copy all CSV files to the CSV file directory. If an uploaded
   * file is Excel, convert it to CSV file(s).
   *
   * @return boolean
   */
  private function _copyCSV() {

    // Resets variables.
    $this->resetLogs('COPY_CSV');

    // Gets the upload file.
    $filepath   = $this->upload_file->getFilepath();
    $file_type  = $this->upload_file->getType();

    // If the uploaded file is excel, convert all the sheets in the excel
    // file into csv files.
    if ($file_type == 'excel') {

      // Converts Excel file to CSV files.
      if (!$this->_convertExcel2CSV($filepath)) {
        $this->updateMsg('E', "Failed to convert to CVS files.");
      }
    }

    // Uncompresses (zip) file and copy them to to CSV folder.
    else if ($file_type == 'zip') {

      // Unzips and copy them to the CSV folder.
      if ($this->_unzipCSV($filepath)) {
        $this->updateMsg('E', "Failed to unzip the input file.");
      }

      // Registers all file in the CSV folder.
      foreach (glob($this->csv_dir . '/*.csv') as $filename) {
        $this->_registerCSVFile($this->csv_dir . "/$filename");
      }
    }

    // Copies a single CSV file to CSV folder.
    else if ($file_type == 'csv') {
      $filename     = basename($filepath);
      $destination  = "$csv_dir/$filename";

      if (copy($filepath, $destination)) {
        $this->_registerCSVFile($destination);
      }
    }

    // Error : Invalid file type.
    else {
      $this->updateMsg('E', "Invalid file type ($filepath).");
    }

    // Writes the log files.
    return $this->writeLog();
  }

  /**
   * Extract the template name from the provided file. The moudule name is
   * specified at the first line of a CSV file.
   *
   * @param string $filepath.
   *
   * @return string
   */
  private function _getTemplateFromFile($filepath) {
    $template = '';

    // Gets the first line of the file.
    if (file_exists($filepath)) {
      $line = fgets(fopen($filepath, 'r'));
      if (preg_match("/^#([a-zA-Z_]+)$/", $line, $matches)) {
        $template = $matches[1];
      }
    }
     return $template;
  }

  /**
   * Register a CSV file.
   *
   * @param string $filepath.
   *
   * @return boolean
   */
  private function _registerCSVFile($filepath) {

    // Gets the template name from the file.
    $template = $this->_getTemplateFromFile($filepath);
    if (!$template) {
      $this->updateMsg('E', "No template line at the begining of the file.\n$destination.");
      return FALSE;
    }

    // Creates MCL_FILE and adds it to this job if not exists.
    if (!$this->existCSV($template)) {
      $csv_file = $this->_createMCL_FILE_CSV($destination, $template);
      if ($csv_file || $csv_file->insert()) {

        // Adds a new CSV file.
        if (!$this->_addCSVFile($csv_file)) {
          $this->updateMsg('E', "Failed to add CSV file.");
        }
      }
      else {
        $this->updateMsg('E', "Failed to create CSV file.");
      }
    }
    return TRUE;
  }

  /**
   * Unzips the compress file and copy all CSV files to CSV folder.
   *
   * @param string $filepath.
   *
   * @return boolean
   */
  private function _unzipCSV($filepath) {

    // Unzips files to CSV folder
    $cmd = "unzip -j $filepath -d " . $this->csv_dir;
    exec($cmd, $output, $return_val);
    return !$return_val;
  }

  /**
   * Converts Exel file to CSV files.
   *
   * @param string $filepath.
   *
   * @return boolean
   */
  private function _convertExcel2CSV($filepath) {

    // Includes PHPExcel libararies.
    $lib_path = 'sites/all/libraries';
    require_once("$lib_path/PHPExcel/PHPExcel.php");
    require_once("$lib_path/PHPExcel/PHPExcel/IOFactory.php");

    // Sets the line ending.
    $line_ending = '&&&&&';

    // Gets Excel file type [Excel5 | ].
    $file_type = PHPExcel_IOFactory::identify($filepath);

    // Gets the defualt precision.
    $def_precision = ini_get('precision');

    // Read input Excel file.
    $reader = PHPExcel_IOFactory::createReader($file_type);
    $reader->setReadDataOnly(true);
    $excel = $reader->load($filepath);
    $num_sheets = $excel->getSheetCount();
    print "\tExcel has $num_sheets sheets\n\n";

    // Re-set the precision. It seems that PHPExcel change the precision.
    ini_set('precision', $def_precision);

    // Empties CSV folder.
    mcl_empty_dir($this->csv_dir);

    // Gets the 'template-rename' arary if it exists in job parameter.
    $param = $this->getParamByKey('template_rename');
    $template_map = array();
    if ($param) {
      $template_map = $param;
    }

    // Save each sheet as CSV file.
    $writer = PHPExcel_IOFactory::createWriter($excel, 'CSV');
    $dup_templates = array();
    for ($i = 0; $i < $num_sheets; $i++) {
      $excel->setActiveSheetIndex($i);
      $template = strtolower($excel->getActiveSheet()->getTitle());
      print "\tcopying $template\n\n";

      // Renames the template if necessary.
      if (array_key_exists($template, $template_map)) {
        $template = $template_map[$template];
      }

      // Checks for dupliacted sheet name.
      if (array_key_exists($template, $dup_templates)) {
        $this->updateMsg('E', "There are duplicated sheet names $template in your file.");
        return FALSE;
      }
      else {
        $dup_templates[$template] = 1;
      }

      // Validates sheet name.
      if (MCL_TEMPLATE::validateTemplate($template)) {

        // Sets CSV filepath.
        $csv_filepath = $this->csv_dir . "/$template" . '.csv';

        // Write to a CSV file.
        $writer->setSheetIndex($i);
        $writer->setLineEnding($line_ending);
        $writer->save($csv_filepath);

        // Replace newline characters in cell value with white spaces.
        $file_contents = file_get_contents($csv_filepath);
        $file_contents = str_replace("\n", ' ', $file_contents);
        $file_contents = str_replace($line_ending, "\n", $file_contents);
        file_put_contents($csv_filepath,  $file_contents);

        // Opens the cvs file.
        if (!($fdr = fopen($csv_filepath, 'r'))) {
          return FALSE;
        }

        // Adds the template name at begining of the file.
        $file_contents = "#$template\n";

        // Cleanup the contents of the file.
        while (!feof($fdr)) {
          $line = trim(fgets($fdr));

          // Exits if no data in the line.
          $check = preg_replace("/[\",\s]/", '', $line);
          if ($check == '') {
            break;
          }
          // Adds the data line.
          $file_contents .= "$line\n";
        }
        fclose($fdr);

        // Updates the contents of the file.
        file_put_contents($csv_filepath,  $file_contents);

        // Creates MCL_FILE if not exists.
        if (!$this->existCSV($template)) {
          $csv_file = $this->_createMCL_FILE_CSV($csv_filepath, $template);
          if ($csv_file && $csv_file->insert()) {

            // Adds a new CSV file.
            if (!$this->_addCSVFile($csv_file)) {
              $this->updateMsg('E', "Failed to add CSV file.");
              return FALSE;
            }
          }
          else {
            $this->updateMsg('E', "Failed to create CSV file.");
            return FALSE;
          }
        }
      }
      else {
        $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
        $msg = "\nThe sheet name '$template' is not a valid template name.\nIt could be the following reason.\n\n";
        $msg .= "  1. wrong spell.\n";
        $msg .= "  2. $class_name is not defined.\n\n";
        $msg .= "'$template' is skipped.\n\n";
        $this->updateMsg('W', $msg);
      }
    }

    // Updates csv_files in job property.
    if (!$this->updateCSV($dup_templates)) {
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Creates a MCL_FILE_CSV object.
   *
   * @param string $filepath
   * @param string $template
   *
   * @return MCL_FILE_CSV
   */
  private function _createMCL_FILE_CSV($filepath, $template) {

    // CSV file properites.
    $prop = array(
      'template' => $template,
    );

    // Create a MCL_FILE_CSV object.
    $details = array(
      'filepath'    => $filepath,
      'filename'    => basename($filepath),
      'filesize'    => filesize($filepath),
      'uri'         => file_build_uri(mcl_get_rel_filepath($filepath)),
      'submit_date' => date("Y-m-d G:i:s"),
      'user_id'     => $this->getUserID(),
      'job_id'      => $this->getJobID(),
      'prop'        => json_encode($prop),
    );
    return new MCL_FILE_CSV($details);
  }

  /**
   * Adds a new CSV file and updates 'csv_files' in mcl_job.prop.
   *
   * @param MCL_FILE $csv_file
   *
   * @return boolean
   */
  private function _addCSVFile(MCL_FILE $csv_file) {

    // Adds the CSV file to csv_files array in job properties.
    $csv_files = $this->getPropByKey('csv_files');
    if (!is_array($csv_files)) {
      $csv_files = array();
    }

    // Removes if exists.
    foreach ($csv_files as $file_id => $csv_filename) {
      if ($csv_filename == $csv_file->getFilename()) {
        unset($csv_files[$file_id]);
      }
    }

    // Updates $this->csv_files.
    $csv_files[$csv_file->getFileID()] = $csv_file->getFilename();
    $this->setPropByKey('csv_files', $csv_files);
    return $this->update();
  }

  /**
   * Performs syntax error check on CSV files.
   *
   * @return boolean
   *   Return TRUE if no error.
   */
  private function _runErrorCheckSyntax() {
    $no_error_flag = TRUE;

    // Gets all CSV files.
    $csv_files = $this->getCSVFiles();

    // Performs the error check.
    foreach ($csv_files as $mcl_file_id => $filename) {
      mcl_print("[$mcl_file_id] $filename", 1);
      mcl_print("- Running the syntax error checking : [$mcl_file_id] $filename", 2);

      // Gets the MCL_FILE_CSV object.
      $csv_file = MCL_FILE_CSV::byKey(array('file_id' => $mcl_file_id));

      // Skips if the error checking has been completed.
      if ($csv_file->hasPassed($this->status_int['syntax error']) && !$this->getForce()) {
        mcl_print("  > Skipped - Syntax error has already been checked", 2, 2);
        continue;
      }

      // Resets the logs.
      $this->resetLogs($csv_file->getTemplate());

      // Runs the syntax error checking.
      $tmpl_obj = MCL_TEMPLATE::getTemplateClass($this, $csv_file);
      if (!$tmpl_obj) {
        $no_error_flag = FALSE;
        mcl_print("Cannot found template class", 2, 2);
        continue;
      }
      $this->setProgress("Running syntax error check for " . $tmpl_obj->getTemplate());
      if ($tmpl_obj->runErrorCheckSyntax()) {
        $csv_file->setPassed($this->status_int['syntax error'], TRUE);
      }
      $this->setProgress("Done syntax error check for " . $tmpl_obj->getTemplate());

      // Writes the log files.
      $this->writeLog();

      // Checks for errors.
      if (!$this->noError()) {
        $no_error_flag = FALSE;
      }
    }
    return $no_error_flag;
  }

  /**
   * Uploads CSV files.
   *
   * 1. Run data error check.
   * 2. Upload the CSV file.
   *
   * @return boolean
   */
  private function _upload_csv() {
    $job_id = $this->getJobID();

    // Gets all CSV files.
    $csv_files = $this->getCSVFiles();

    // Upload the data.
    foreach ($csv_files as $mcl_file_id => $filename) {

      // Skips the excluded templates.
      if (in_array($filename , $this->excludes)) {
        continue;
      }

      // Creates a MCL_FILE_CSV object.
      $csv_file = MCL_FILE_CSV::byKey(array('file_id' => $mcl_file_id));
      $template = $csv_file->getTemplate();

      // Resets the logs.
      $this->resetLogs($template);

      // Gets MCL_TEMPLATE object.
      $tmpl_obj = MCL_TEMPLATE::getTemplateClass($this, $csv_file);

      // Runs the data error checking. Skips if the error checking has been
      // completed.
      mcl_print("[$mcl_file_id] $filename", 1);
      mcl_print("- Running the data error checking", 2);
      if ($csv_file->hasPassed($this->status_int['data error']) && !$this->getForce()) {
        mcl_print("  > Skipped - Data error has already been checked", 2, 2);
      }
      else {

        // Runs the data error checking.
        $this->setProgress("Running data error check for " . $tmpl_obj->getTemplate());
        if ($tmpl_obj->runErrorCheckData()) {
          $csv_file->setPassed($this->status_int['data error'], TRUE);
          mcl_print("Data error check passed", 2, 2);
        }
        $this->setProgress("Done data error check for " . $tmpl_obj->getTemplate());

        // Check if an error occured. If error found, write a log file
        // and return FALSE.
        if (!$this->noError()) {
          $this->writeLog();
          return FALSE;
        }
      }

      // Uploads CSV data file.
      mcl_print("- Uploading data file", 2);

      // Skips if the data has been uploaded.
      if ($csv_file->hasPassed($this->status_int['uploading']) && !$this->getForce()) {
        mcl_print("  > Skipped - Data has already been uploaded", 2, 2);
        continue;
      }

      // Upload the data file.
      $this->setProgress("Uploading data for " . $tmpl_obj->getTemplate());
      if ($tmpl_obj->uploadData()) {
        $csv_file->setPassed($this->status_int['uploading'], TRUE);
        mcl_print("Data have been uploaded to $template", 2, 2);
        $this->setProgress("Done uploading data to " . $tmpl_obj->getTemplate());
      }
      else {
        $this->setProgress("Failed to upload data for " . $tmpl_obj->getTemplate());

        // Detetes the log for new data.
        $this->deleteMsg('N');
      }

      // Write the messages to the logs.
      $this->writeLog();

      // Check if an error occured. If error found, returns FALSE.
      if (!$this->noError($template)) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Return all CSV files in order.
   *
   * @return array
   */
  public function getCSVFiles() {

    // Gets all CSV files.
    $csv_files = $this->getPropByKey('csv_files');

    // Ranks CSV files.
    $csv_files_unsorted = array();
    foreach ($csv_files as $file_id => $csv_filename) {

      // Gets the rank of csv file.
      $mcl_file_csv = MCL_FILE_CSV::byKey(array('file_id' => $file_id));
      $rank = MCL_TEMPLATE::getRank($mcl_file_csv->getTemplate());
      $info = array(
        'file_id'       => $file_id,
        'csv_filename'  => $csv_filename,
        'rank' => $rank
      );
      $csv_files_unsorted[] = $info;
    }

    // Sorts CSV files.
    usort($csv_files_unsorted,
      function($a, $b) {
        if ($a['rank'] == $b['rank']) {
          return $a['csv_filename'] < $b['csv_filename'];
        }
        return $a['rank'] > $b['rank'];
      }
    );
    $csv_files_sorted = array();
    foreach ($csv_files_unsorted as  $info) {
      $csv_files_sorted[$info['file_id']] = $info['csv_filename'];
    }
    return $csv_files_sorted;
  }

  /**
   * Checks if CSV file exists.
   *
   * @param string $template
   *
   * @return boolean
   */
  public function existCSV($template) {

    // Gets all CSV files.
    $csv_files = $this->getPropByKey('csv_files');
    if (is_array($csv_files)) {
      foreach ($csv_files as $file_id => $csv_filename) {
        $csv_file = MCL_FILE_CSV::byKey(array('file_id' => $file_id));
        if ($csv_file) {
          $csv_template = $csv_file->getTemplate();
          if ($template == $csv_file->getTemplate()) {
            return TRUE;
          }
        }
      }
    }
    return FALSE;
  }

  /**
   * Updates csv_files in the job properties.
   *
   * @param array $templates
   *
   * @return boolean
   */
  public function updateCSV($templates) {

    // Gets all CSV files.
    $csv_files = $this->getPropByKey('csv_files');
    if (is_array($csv_files)) {
      $new_csv_files = array();
      foreach ($csv_files as $file_id => $csv_filename) {
        $key = preg_replace('/\.csv$/', '', $csv_filename);
        if (array_key_exists($key, $templates)) {
          $new_csv_files[$file_id] = $csv_filename;
        }
      }
      $this->setPropByKey('csv_files', $new_csv_files);
      return $this->update();
    }
    return TRUE;
  }


  /**
   * Returns all logs in an array.
   *
   * @return array
   */
  public function getLogs() {
    $all_logs = array();

    // Gets the path to the public folder.
    $public_path  = drupal_realpath('public://');
    $public_url   = file_create_url('public://');

    // Adds log files  (Warning / Errror / Dup-Data) in recent directory.
    $recent_dir = $this->getRecentLogDir('recent');
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
    $new_data_dir = $this->getNewDataLogDir('new_data');
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
      // usort($log_files, $this->_cmpDate);
      $all_logs['N']['40'] = $log_files;
    }
    return $all_logs;
  }

  /**
   * Translates status int to label.
   *
   * @retrun boolean
   */
  public function _cmpDate() {
    return strcmp($b['date'], $a['date']);
  }

/**
   * Translates status int to label.
   *
   * @retrun boolean
   */
  public function trStatusIntToLabel($int) {
    return $this->status_label["$int"];
  }

  /**
   * Returns the log directory of the provided type.
   *
   * @retrun string
   */
  public function getLogDirByType($type) {
    return $this->getLogDir() . "/$type";
  }

  /**
   * Returns the recent log directory.
   *
   * @retrun string
   */
  public function getRecentLogDir() {
    return $this->getLogDir() . '/recent';
  }

  /**
   * Returns the previous log directory.
   *
   * @retrun string
   */
  public function getPreviousLogDir() {
    return $this->getLogDir() . '/previous';
  }

  /**
   * Returns the new data log directory.
   *
   * @retrun string
   */
  public function getNewDataLogDir() {
    return $this->getLogDir() .'/new_data';
  }

  /**
   * Returns the file ID of the uploaded file.
   *
   * @retrun integer
   */
  public function getFileID() {
    $file_id = $this->getParamByKey('file_id');
    if ($file_id) {
      return intval($file_id);
    }
    return NULL;
  }

  /**
   * Updates the file ID of the uploaded file.
   *
   * @retrun integer
   */
  public function setFileID($file_id) {

    // Creates a MCL_FILE object for the uploaded file.
    $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
    if ($mcl_file) {
      $this->upload_file = $mcl_file;
      $this->setParamByKey('file_id', $file_id);
    }
  }

  /**
   * Returns the file ID of the uploaded file.
   *
   * @retrun MCL_FILE
   */
  public function getMCLFile() {
    $file_id = $this->getFileID();
    if (is_int($file_id)) {
      return MCL_FILE::byKey(array('file_id' => $file_id));
    }
    return NULL;
  }

  /**
   * Retrieves the force flag.
   *
   * @retrun boolean
   */
  public function getTransaction() {
    return $this->getPropByKey('transaction');
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the force flag.
   *
   * @retrun boolean
   */
  public function getForce() {
    return $this->force;
  }

  /**
   * Updates the force flag.
   *
   * @param boolean $force
   */
  public function setForce($force) {
    $this->force = $force;
    $this->setPropByKey('force', $force);
  }

  /**
   * Retrieves the directory of the CSV files.
   *
   * @retrun string
   */
  public function getCSVDir() {
    return $this->csv_dir;
  }

  /**
   * Updates the directory of the CSV files.
   *
   * @param string $csv_dir
   */
  public function setCSVDir($csv_dir) {
    $this->csv_dir = $csv_dir;
    $this->setPropByKey('csv_dir', $csv_dir);
  }

  /**
   * Retrieves the uploaded file.
   *
   * @retrun boolean
   */
  public function getUploadFile() {
    return $this->upload_file;
  }

  /**
   * Updates the uploaded file.
   *
   * @param MCL_FILE $upload_file
   */
  public function setUploadFile(MCL_FILE $upload_file) {
    $this->upload_file = $upload_file;
  }
}