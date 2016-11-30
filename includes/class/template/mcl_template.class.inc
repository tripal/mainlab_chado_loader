<?php
/**
 * The declaration of MCL_TEMPLATE class.
 *
 */
class MCL_TEMPLATE extends PUBLIC_MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  protected $template_type  = NULL;
  protected $headers        = NULL;
  protected $headers_sp     = NULL;
  protected $cvterms        = NULL;
  protected $csv            = NULL;
  protected $job            = NULL;
  protected $line_no        = NULL;
  protected $stats          = NULL;

  /**
   * @see PUBLIC_MCL_TEMPLATE::__construct()
   */
  public function __construct($details) {
    parent::__construct($details);
    $this->csv  = array_key_exists('csv', $details) ? $details['csv'] : NULL;
    $this->job  = array_key_exists('job', $details) ? $details['job'] : NULL;

    // Updates the template type.
    if (array_key_exists('template_type', $details)) {
      $this->template_type =  MCL_TEMPLATE_TYPE::byKey(array('type' => $details['template_type']));
    }
    if (!$this->template_type && $this->template_type_id) {
      $this->template_type = MCL_TEMPLATE_TYPE::byKey(array('template_type_id' => $this->template_type_id));
    }

    // Initialize variables.
    $this->line_no = -1;

    // Sets the headers.
    $this->headers = $this->defineHeaders();

    // Sets the cvterms.
    $this->cvterms = $this->defineCvterms();
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Return the description of of template properties for both Excel and CSV
   * files.
   *
   * @return string
   *  The description of a template.
   */
  public static function getProperties() {

    // Sets the headers.
    $headers = array(
      'Prefix',
      array('data' => 'Column Type', 'style' => 'width:80px;'),
      'Description'
    );

    // Special column table.
    $desc_REQ   = "The star symbol indicates a required column. All the values in this column must be provided";
    $desc_PROP  = "A column name followed by double pound signs must be a cvterm name. The cvterm is used as a type of a property table.";
    $desc_DESC  = "A column name followed by single pound sign must be a cvterm name. The cvterm is used as a descriptor of evaluation data in the column." .
                  "<br />(<em>e.g.</em>) #brix #pH #fruit_weight";
    $desc_SEQ   = "A column name followed by a tilde symbol indicates that the column holds sequnce information.";

    $rows = array(
      array('*', 'required', $desc_REQ),
      array('##', 'property', $desc_PROP),
      array('#', 'descriptor', $desc_DESC),
      array('~', 'descriptor', $desc_SEQ),
    );
    $table_vars = array(
      'header'      => $headers,
      'rows'        => $rows,
      'attributes'  => array(),
    );
    $table = theme('table', $table_vars);

    // Excel format properties.
    $items = array();
    $items[] = "Column names are case-insensitive.";
    $items[] = "Data sheet name must match a template name.";
    $items[] = "special column - special charcter at the begining of column name.\n$table";
    $excel_prop = theme('item_list', array('items' => $items));

    // CSV format properties.
    $items = array();
    $items[] = "The first line of CSV file must start with '#' followed by a template name.";
    $items[] = "The second line of a CSV file must with '#' followed by headers.";
    $items[] = "Data section starts from the third line. Each column should be double quoted and separated by commas.";
    $csv_prop = theme('item_list', array('items' => $items));

    $desc = <<< DESC
    <div><b>Data in Excel format</b></div>
    <div>$excel_prop</div>
    <div><b>Data in CSV format</b></div>
    <div>$csv_prop</div>
    <div style="margin-left:30px;">(<em>e.g.</em>)<br />
    #contact<br />
    #"*name","first_name","last_name","alias","*type","lab","institution"<br />
    "N","F","L","", "person","Main Lab", "WSU"<br />
    </div>
DESC;
    return $desc;
  }

  /**
   * Gets the description of this template.
   *
   * @return string
   *   The description of this template.
   */
  public function defineDescription() {
    // To be overridden by Child class.
    return '';
  }

  /**
   * Gets headers used in this template.
   *
   * @return array of header columns.
   */
  public function defineHeaders() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Gets cvterms used in this template.
   *
   * @return array of cvterms.
   */
  public function defineCvterms() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Adds a template.
   *
   * @param string $template_type
   * @param string $template
   *
   * @return boolean
   */
  public static function addTemplate($template_type, $template) {

    // Gets the template type.
    $mcl_template_type = MCL_TEMPLATE_TYPE::byKey(array('type' => $template_type));

    if (!$mcl_template_type) {
      return FALSE;
    }

    // Check for duplication.
    $mcl_template = PUBLIC_MCL_TEMPLATE::byKey(array('template' => $template));
    if (!$mcl_template) {

      // Adds a new template.
      $details = array(
        'template'          => $template,
        'template_type_id'  => $mcl_template_type->getTemplateTypeID(),
      );
      $mcl_template = new MCL_TEMPLATE($details);
      if (!$mcl_template->insert()) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Updates templates.
   *
   * @param boolean $new_template_only
   *
   */
  public static function updateTemplates($new_template_only = FALSE) {

    // If $new_template_only is FALSE, removes all templates from the mcl_template table.
    if (!$new_template_only) {
      //$result = db_truncate('mcl_template')->execute();
    }

    // Gets the directory of the templates.
    $module_path = drupal_get_path('module', 'mcl');
    $files = file_scan_directory("$module_path/includes/class/template/module", "/\.inc$/");
    foreach ($files as $filepath => $file) {
      if (preg_match("/^mcl_template_(.*)\.class$/", $file->name, $matches)) {
        $template   = $matches[1];
        $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
        if (class_exists($class_name)) {

          // Adds a template.
          $mcl_template = new $class_name();
          if ($mcl_template->getTemplateType(TRUE)) {
            if (MCL_TEMPLATE::addTemplate($mcl_template->getTemplateType(), $template)) {
              drupal_set_message("New template of $class_name has been added.");
            }
            else {
              drupal_set_message("Failed to add the template of $class_name.");
            }
          }
          else {
            drupal_set_message("Template type of $class_name not found. Check the template type.");
          }
        }
        else {
          drupal_set_message("Skipped : $class is not defined.");
        }
      }
    }
  }

  /**
   * Returns template information in array. Adds the information for
   * creating two excel sheets for header definition and data template.
   *
   * @return array
   *  It holds information about this template.
   */
  public function getTemplateInfo() {
    $tmp = $this->defineHeaders();

    // Adds info. for header definition.
    $def_sheet = array();
    $def_sheet['name']  = 'Headers';

    // Sets headers.
    $headers = array();
    $headers['A'] = array('heading' => 'Name',        'width' => 25);
    $headers['B'] = array('heading' => 'Required',    'width' => 10);
    $headers['C'] = array('heading' => 'Definition',  'width' => 100);
    $def_sheet['headers'] = $headers;

    // Sets data.
    $data = array();
    foreach ($tmp as $heading => $prop) {
      $name = array(
        'col'   => 'A',
        'type'  => 'text',
        'value' => $heading,
      );
      $req = array(
        'col'   => 'B',
        'type'  => 'text',
        'value' => ($prop['req']) ? 'REQ' : ''
      );
      $def = array(
        'col'   => 'C',
        'type'  => 'text',
        'value' => $prop['desc'],
      );

      $data[] = array($name, $req, $def);
    }
    $def_sheet['data'] = $data;

    // Adds info. for data.
    $data_sheet = array();
    $data_sheet['name'] = $this->template;

    // Sets headers.
    $headers = array();
    $col_char = 'A';
    foreach ($tmp as $heading => $prop) {
      $label  = $heading;
      $header = array();
      $cell   = array();

      // Adds a label if exists.
      if (array_key_exists('label', $prop)) {
        $label = $prop['label'];
      }

      // Adds a prefix to heading.
      $prefix = ($prop['req']) ? '*' : '';
      $header['heading'] = $prefix . $label;

      // Adds width if defined.
      if (array_key_exists('width', $prop)) {
        $header['width'] = $prop['width'];
      }
      $headers[$col_char++] = $header;
    }
    $data_sheet['headers'] = $headers;

    // Adds both header definition and data sheets.
    return array($def_sheet, $data_sheet);
  }

  /**
   * Checks if a template for this template exists. Also checks if
   * a class file for this template exists.
   *
   * @param string $template
   *
   * @return boolean
   */
  public static function validateTemplate($template) {

    // Checks if a template for this template exists.
    $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
    if ($mcl_template) {

      // Checks if class file for this template is defined.
      $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
      return class_exists($class_name);
    }
    return FALSE;
  }

  /**
   * Gets the rank of the template. The rank is based on the type of template.
   *
   * @param string $template
   *
   * @return integer
   *  The rank of the template.
   *
   */
  public static function getRank($template) {

    // Gets the rank of the template.
    $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
    return $mcl_template->template_type->getRank();
  }

  /**
   * Returns MCL_TEMPLATE object.
   *
   * @param MCL_JOB_UPLOAD $job
   * @param MCL_FILE_CSV $csv
   *
   * @return MCL_TEMPLATE
   */
  public static function getTemplateClass(MCL_JOB_UPLOAD $job, MCL_FILE_CSV $csv) {

    // Gets the child class object of MCL_TEMPLATE.
    $class_name = 'MCL_TEMPLATE_' . strtoupper($csv->getTemplate());
    $mcl_template = $class_name::byKey(array('template' => $csv->getTemplate()));
    $mcl_template->setJob($job);
    $mcl_template->setCSV($csv);
    return $mcl_template;
  }

  /**
   * Returns MCL_TEMPLATE object.
   *
   * @param integer $template_id
   *
   * @return Descendance of MCL_TEMPLATE
   */
  public static function getTemplateByID($template_id) {

    // Gets the template.
    $mcl_template = MCL_TEMPLATE::byKey(array('template_id' => $template_id));
    $template = $mcl_template->getTemplate();

    // Gets the child class object of MCL_TEMPLATE.
    $class_name = 'MCL_TEMPLATE_' . strtoupper($template);
    if (!class_exists($class_name)) {
      return NULL;
    }
    return $class_name::byKey(array('template_id' => $template_id));
  }

  /**
   * Returns the all templates.
   *
   * @return array of MCL_TEMPLATE.
   */
  public static function getTemplates() {
    $sql = "
      SELECT T.template_id
      FROM {mcl_template} T
        INNER JOIN {mcl_template_type} TT on TT.template_type_id = T.template_type_id
      ORDER BY TT.rank, T.template
    ";
    $mcl_templates = array();
    $result = db_query($sql);
    while ($template_id = $result->fetchField()) {
      $mcl_templates []= MCL_TEMPLATE::byKey(array('template_id' => $template_id));
    }
    return $mcl_templates;
  }

  /**
   * Updates the headers. All headers to be lowercase.
   *
   * @return boolean
   */
  public function updateHeaders() {
    $headers = $this->defineHeaders();
    $updated_headers = array();
    foreach ($headers as $header => $prop) {
      $prop['col'] = -1;
      $updated_headers[strtolower($header)] = $prop;
    }
    return $updated_headers;
  }

  /**
   * Updates cvterms array.
   *
   * @return boolean
   */
  public function updateCvterms() {
    $error = FALSE;

    // Gets cvterm IDs.
    foreach ($this->cvterms as $cv_name => $cvterms) {
      $tmp_cv_name = $cv_name;

      // Updates the name if name is 'SITE_CV' or 'SITE_TRAIT_ONTOLOGY'.
      if ($cv_name == 'SITE_CV') {
        $tmp_cv_name = MCL_VAR::getValueByName('SITE_CV');
      }
      else if ($cv_name == 'SITE_TRAIT_ONTOLOGY') {
        $tmp_cv_name = MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
      }

      // Gets CV.
      $cv = CHADO_CV::byKey(array('name' => $tmp_cv_name));
      if (!$cv) {
        $this->updateMsg('E', "Error : Could not find cv_id for  $cv_name");
        $error = TRUE;
        continue;
      }

      // Gets cvterms.
      $cv_id = $cv->getCvID();
      foreach ($cvterms as $cvterm_name => $dummy) {
        $cvterm = CHADO_CVTERM::byKey(array('name' => $cvterm_name, 'cv_id' => $cv_id));
        if ($cvterm) {
          $this->cvterms[$cv_name][$cvterm_name] = $cvterm->getCvtermID();
        }
        else {
          $this->updateMsg('E', "Error : Could not find cvterm : $cvterm_name [cv_id = $cv_id].");
          $error = TRUE;
        }
      }
    }
    return !$error;
  }

  /**
   * Returns the information of the headers in table.
   *
   * @return string
   */
  public function getHeaderTable() {

    // Sets red star.
    $star = '<b><span style="color:red;">*</span></b>';

    // Populates the rows with the headers.
    $rows = array();
    foreach ($this->headers as $name => $info) {

      // Shows a label if exists.
      if (array_key_exists('label', $info)) {
        $name = $info['label'];
      }
      $name = ($info['req']) ? "$star $name" : $name;
      $row = array($name, $info['desc']);
      $rows []= $row;
    }
    $table_vars = array(
      'header'      => array('Column', 'Description'),
      'rows'        => $rows,
      'empty'       => 'No header found',
      'attributes'  => array(),
    );
    $table = theme('table', $table_vars);

    // Adds the footer.
    $footer = "<div style='left-margin:5px;'>$star&nbsp;Required field</div>";
    return $table.$footer;
  }

  /**
   * Returns the information of the cvterms in table.
   *
   * @return string
   */
  public function getCvtermTable() {

    // Populates the rows with the cvterms.
    $rows = array();
    foreach ($this->cvterms as $cv_name => $cvterms) {
      if ($cv_name == 'SITE_CV') {
        $cv_name = MCL_VAR::getValueByName('SITE_CV');
      }
      else if ($cv_name == 'SITE_TRAIT_ONTOLOGY') {
        $cv_name = MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
      }
      ksort($cvterms);
      foreach ($cvterms as $cvterm_name => $cvterm_id) {
        $row = array($cv_name, $cvterm_name);
        $rows []= $row;
      }
    }
    $table_vars = array(
      'header'      => array('cv.name', 'cvterm.name'),
      'rows'        => $rows,
      'empty'       => 'No cvterm found',
      'attributes'  => array(),
    );
    return theme('table', $table_vars);
  }

  /**
   * Checks if this job stage has been passed.
   *
   * @return boolean
   */
  public function isPassed() {
    return $this->job->noError($this->template);
  }

  /**
   * Updates the message.
   *
   * @param string $key
   * @param string $msg
   */
  public function updateMsg($key, $msg) {
    $this->job->updateMsg($key, $msg, $this->line_no);
  }

  /**
   * Adds the message.
   *
   * @param string $key
   * @param string $table
   * @param array $args
   */
  public function addMsg($key, $table, $args) {
    $msg = '';
    if ($key == 'N') {
      $msg = "Added to $table : " . $this->arrStr($args);
    }
    else if ($key == 'E') {
      $msg = "Failed to upload to $table : " . $this->arrStr($args);
    }
    else if ($key == 'D') {
      $msg = "Duplicated found in $table : " . $this->arrStr($args);
    }
    $this->job->updateMsg($key, $msg, $this->line_no);
  }

  /**
   * Runs syntax error check.
   *
   * The input file should have
   *   1st line : pound sign (#) and template name.
   *   2nd line : headers with asteric for required fields.
   *   3rd line : data section starts.
   *
   * Syntax error checking includes the following errors.
   *   1. Validation on header names.
   *   2. duplicated headers.
   *   3. The number of columns in data section.
   *   4. Missing required fields.
   *   5. Missing value on the requried fields.
   *
   *   @return boolean
   */
  public function runErrorCheckSyntax() {
    $header_flag      = TRUE;
    $template_flag    = TRUE;
    $num_cols_headers = -1;
    $line_no          = 0;
    $header_dup_arr   = array();

    // Opens the input file.
    $infile = $this->csv->getFilepath();
    if (!($fh = fopen($infile, 'r'))) {
      $this->updateMsg('E', "Error : Fail to open the input file : $infile.");
      return;
    }
    while(!feof($fh)) {

      $this->setLineNo($line_no++);
      $line = trim(fgets($fh));
      if (!$line) {
        continue;
      }

      // Skips the template line.
      if ($template_flag) {
        $template_flag = FALSE;
      }

      // Handles the headers line.
      else if ($header_flag) {
        $header_flag = FALSE;

        // Tokenizes the line.
        $columns  = $this->_tokenizeLine($line, TRUE);
        $num_cols = sizeof($columns);

        // Sets the number of columns of the headers.
        $num_cols_headers = $num_cols;

        // Checks each column.
        for ($i =0; $i < sizeof($columns); $i++) {
          $column_name_orig = $columns[$i];

          // Skips if column is emtpy.
          if (!$column_name_orig) {
            continue;
          }

          // Checks if column has valid name and saves the position of the column.
        // Ignore the column name starting with '#', '@' or '$'.
          $column_name = strtolower($column_name_orig);
          if (!array_key_exists($column_name, $this->headers) && !preg_match("/^[#@$~]/", $column_name)) {
            $this->updateMsg('W', "Invalid header : '$column_name_orig' is not valid header name, so this column would be ignored.");
          }
          else {

            // Checks header name for duplication.
            if (array_key_exists($column_name, $header_dup_arr)) {
              $dup_column = $header_dup_arr[$column_name];
              $this->updateMsg('E', "Duplicated header : Found duplicated header '$column_name_orig' at column $dup_column and $i. Please removed either one.");
            }
            else {
              $this->headers[$column_name]['col'] = $i;
              $header_dup_arr[$column_name] = $i;
            }
          }
        }

        // Checks if required columns exists.
        foreach ($this->headers as $column => $prop) {
          if ($prop['req'] && (!array_key_exists('col', $prop) || $prop['col'] < 0)) {
            $this->updateMsg('E', "Error : Required column [$column] does not exists.");
          }
        }
      }

      // Handles data lines.
      else {

        // Tokenizes the line. Skips the rest of data section if no data is in the line.
        $columns = $this->_tokenizeLine($line, FALSE);
        if (empty($columns)) {
          break;
        }

        // The number of columns in data line.
        if ($num_cols != $num_cols_headers) {
          $this->updateMsg('E', "Error : Invalid number of columns ($num_cols/$num_cols_headers)");
        }

        // Checks if the value of required column not empty.
        foreach ($this->headers as $column => $prop) {
          if ($prop['req']) {
            $col = $this->headers[$column]['col'];
            $value = $columns[$col];
            if (!$value && $value === '') {
              $this->updateMsg('E', "Error : Missing data on column [$column]");
            }
          }
        }
      }
    }
    fclose($fh);
    return $this->isPassed();
  }

  /**
   * Cleanup the input line and return it in an array.
   * Removes double quote. If $headers is TRUE, remove '*'. It returns
   * an empty array if no data present in a data line.
   *
   * @param string $line
   * @param boolean $headers
   *
   * @return array
   */
  private function _tokenizeLine($line, $headers = FALSE) {

    // Tokenizes the line.
    $columns = preg_split('/","/', $line);
    $data_str = '';
    foreach ($columns as &$column) {
      $column = trim($column, '"');
      if ($headers) {
        $column = trim(strtolower(str_replace('*', '', $column)));
      }
      $data_str .= $column;
    }
    if (trim($data_str)) {
      return $columns;
    }
    return array();
  }

  /**
   * Run data error check.
   *
   * Data error checking includes the following errors.
   * - existence of data
   * - consistency of data
   *
   * @return boolean
   */
  public function runErrorCheckData() {
    $header_flag    = TRUE;
    $template_flag  = TRUE;
    $line_no        = 0;

    // Updates $cvterms for the template.
    if (!$this->updateCvterms()) {
      return FALSE;
    }

    // Opens the input file.
    $infile = $this->csv->getFilePath();
    if (!($fh = fopen($infile, 'r'))) {
      $this->updateMsg('E', "Error : Fail to open the input file : $infile.");
      return FALSE;
    }
    while(!feof($fh)) {
      $line_no++;
      $line = trim(fgets($fh));
      if (!$line) {
        continue;
      }

      // Skips the template line.
      if ($template_flag) {
        $template_flag = FALSE;
      }

      // Handles the headers line.
      else if ($header_flag) {
        $header_flag = FALSE;

        // Tokenizes the line.
        $columns = $this->_tokenizeLine($line, TRUE);

        // Saves the position of the column.
        for ($i =0; $i < sizeof($columns); $i++) {
          $column_name = $columns[$i];

          // Skips if column is emtpy.
          if (!$column_name) {
            continue;
          }

          // Sets column #.
          if (array_key_exists($column_name, $this->headers) || preg_match("/^#/", $column_name)) {
            $this->headers[$column_name]['col'] = $i;
          }
        }
      }

      // Handles data lines.
      else {

        // Tokenizes the line. Skips the rest of data section if no data is in the line.
        $columns = $this->_tokenizeLine($line, FALSE);
        if (empty($columns)) {
          break;
        }

        // Populates the data line array.
        $line_arr = array();
        foreach ($this->headers as $column => $prop) {
          if ($prop['col'] > -1) {
            $line_arr[$column] = trim($columns[$prop['col']]);
          }
        }
        $this->setLineNo($line_no);
        $this->runErrorCheckDataLine($line_arr);
      }
    }
    fclose($fh);
    return $this->isPassed();
  }

  /**
   * Checks a data line for this template.
   *
   * @param associative array $line
   */
  public function runErrorCheckDataLine($line) {
    // Defined in child class.
  }

  /**
   * Uploads a CSV data file.
   *
   * @return boolean
   */
  public function uploadData() {
    $header_flag    = TRUE;
    $template_flag  = TRUE;
    $line_no        = 0;

    // Updates the cvterms for the template.
    if (!$this->updateCvterms()) {
      $this->updateMsg('E', "Error : Updating cvterms.");
      return FALSE;
    }

    // Opens the input file.
    $infile = $this->csv->getFilePath();
    if (!($fh = fopen($infile, 'r'))) {
      $this->updateMsg('E', "Error : Fail to open the input file : $infile.");
      return FALSE;
    }

    // Gets the number of lines in the file.
    $total_lines = count(file($infile)) - 1;

    // Upload data by line.
    while(!feof($fh)) {
      $line_no++;
      $line = trim(fgets($fh));
      if (!$line) {
        continue;
      }

      // Skips the template line.
      if ($template_flag) {
        $template_flag = FALSE;
      }

      // Handles the headers line.
      else if ($header_flag) {
        $header_flag = FALSE;

        // Tokenizes the line.
        $columns = $this->_tokenizeLine($line, TRUE);
        if (empty($columns)) {
          break;
        }

        // Saves the position of the column.
        for ($i =0; $i < sizeof($columns); $i++) {
          $column_name = $columns[$i];
          if (array_key_exists($column_name, $this->headers) || preg_match("/^[#@~]/", $column_name)) {
            $this->headers[$column_name]['col'] = $i;
          }
        }
      }

      // Handles data lines.
      else {

        // Tokenizes the line.  Skips the rest of data section if no data is in the line.
        $columns = $this->_tokenizeLine($line, FALSE);
        if (empty($columns)) {
          break;
        }

        // Populates the data line array.
        $line_arr = array();
        foreach ($this->headers as $column => $prop) {
          if ($prop['col'] > -1) {
            $line_arr[$column] = trim($columns[$prop['col']]);
          }
        }
        $this->setLineNo($line_no);
        $this->uploadDataLine($line_arr);
      }

      // Updates the progress of the job.
      if ($line_no % 3 == 0) {
        $progress = sprintf("%s : %d / %d = %.03f %%", $this->template, $line_no, $total_lines, $line_no / $total_lines * 100.0);
        $this->job->setProgress($progress);
      }
    }
    fclose($fh);
    return $this->isPassed();
  }

  /**
   * Upload a data line for this template.
   *
   * @param associative array $line
   */
  public function uploadDataLine($line) {
    // Defined in child class.
  }

  /**
   * Adds one or more properties
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   */
  public function addProperty($table_name, $target_field, $target_id, $type_id, $value, $separator = '') {
    if ($value) {

      // Checks duplication before adding a new property.
      $args = array(
        $target_field => $target_id,
        'type_id'     => $type_id,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Poluplates the array with values.
        $value_arr = preg_split($this->getSepRegex($separator), $value, NULL, PREG_SPLIT_NO_EMPTY);

        // Adds each property.
        $rank = 0;
        foreach ($value_arr as $val) {
          $args['value'] = $val;
          $args['rank'] = $rank++;
          $obj = new $class_name($args);
          if ($obj->insert()) {
            $this->addMsg('N', $table_name, $args);
         }
          else {
            $this->addMsg('E', $table_name, $args);
          }
        }
      }
    }
  }

  /**
   * Adds one property with the specified rank.
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param integer $rank
   */
  public function addPropertyRanked($table_name, $target_field, $target_id, $type_id, $value, $rank) {
    if ($value) {

      // Checks duplication before adding a new property.
      $args = array(
        $target_field => $target_id,
        'type_id'     => $type_id,
        'rank'        => $rank,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Adds a property.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg('N', $table_name, $args);
        }
        else {
          $this->addMsg('E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Gets the property.
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   */
  public function getProperty($table_name, $target_field, $target_id, $type_id) {

    // Gets the value of the property table.
    $args = array(
      $target_field => $target_id,
      'type_id'     => $type_id,
    );
    $class_name = strtoupper('CHADO_' . $table_name);
    $obj = $class_name::byKey($args);
    if ($obj) {
      return $obj->getValue();
    }
    else {
      return '';
    }
  }

  /**
   * Adds a relationship.
   *
   * @param string $table_name
   * @param string $subject_field
   * @param integer $subject_id
   * @param string $object_field
   * @param string $object_id
   * @param integer $type_id
   */
  public function addRelationship($table_name, $subject_field, $subject_id, $object_field, $object_id, $type_id) {
    if ($subject_id && $object_id && $type_id) {

      // Checks duplication before adding a new relationship.
      $args = array(
        $subject_field  => $subject_id,
        $object_field   => $object_id,
        'type_id'       => $type_id,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Adds a new relationship.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg('N', $table_name, $args);
        }
        else {
          $this->addMsg('E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Adds a link (Adding to a linking table).
   *
   * @param string $table_name
   * @param string $field_1
   * @param integer $id_1
   * @param string $field_2
   * @param string $id_2
   */
  public function addLink($table_name, $field_1, $id_1, $field_2, $id_2, $type_id = NULL) {
    if ($id_1 && $id_2) {

      // Checks for a duplication before adding a new relationship.
      $args = array(
        $field_1  => $id_1,
        $field_2  => $id_2,
      );
      if ($type_id) {
        $args['type_id'] = $type_id;
      }
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg('D', $table_name, $args);
      }
      else {

        // Adds a new link.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg('N', $table_name, $args);
        }
        else {
          $this->addMsg('E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Returns the template summary.
   */
  public function templateSummary() {

    $summary = "\n\n\t================================\n" .
    "\tTemplate : " . $this->template . "\n" .
    "\tType     : " . $this->template_type . "\n" .
    "\t--------------------------------\n";





    return $summary;
  }

  /**
   * Creates a string from array.
   *
   * @param assoc array $args
   * @param string $field_1
   * @param integer $id_1
   * @param string $field_2
   * @param string $id_2
   */
  public function arrStr($args) {
    if (empty($args)) {
      return '';
    }
    $keys = '';
    $values = '';
    $flag = TRUE;
    foreach ($args as $key => $value) {
      $prefix = ', ';
      if ($flag) {
        $prefix = '';
        $flag = FALSE;
      }
      $keys .= $prefix . $key;
      $values .= $prefix . $value;
    }
    return "($keys) = ($values)";
  }

  /**
   * Returns the regular expression of the separator.
   *
   * @param string
   */
  public function getSepRegex($separator) {
    return ($separator) ? "/$separator/" : '/@@@@@/';
  }

  /**
   * Retrieves the cvterm_id.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return integer
   */
  public function getCvtermID($cv_name, $cvterm_name) {
    return $this->cvterms[$cv_name][$cvterm_name];
  }

  /**
   * Checks argments for empty.
   *
   * @param array $args
   *
   * @return boolean
   */
  public function checkReqArgs($args) {
    foreach ($args as $key => $value) {
      if ($value == '') {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks if there are data in a row.
   *
   * @param array $line
   * @param string $regex
   *
   * @return boolean
   * Returns TRUE if data row are empty.
   */
  public function isEmptyRow($line, $regex = '') {
    $data_str = '';
    foreach ($line as $column => $value) {
      if (preg_match("/$regex/", $column)) {
        $data_str .= trim($line[$column]);
      }
    }
    return empty($data_str);
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the template type.
   *
   * @param boolean $object
   *
   * @return string
   */
  public function getTemplateType($object = FALSE) {
    if ($object) {
      return $this->template_type;
    }
    return $this->template_type->getType();
  }

  /**
   * Updates the template type.
   *
   * @param string $template_type
   */
  public function setTemplateType($template_type) {
    $this->template_type = MCL_TEMPLATE_TYPE::byKey(array('type' => $template_type));
  }

  /**
   * Retrieves the headers.
   *
   * @return string
   */
  public function getHeaders() {
    return $this->headers;
  }

  /**
   * Updates the headers.
   *
   * @param string $headers
   */
  public function setHeaders($headers) {
    $this->headers = $headers;
  }

  /**
   * Retrieves the job.
   *
   * @return MCL_JOB
   */
  public function getJob() {
    return $this->job;
  }

  /**
   * Updates the job.
   *
   * @param MCL_JOB $job
   */
  public function setJob($job) {
    $this->job = $job;
  }

  /**
   * Retrieves the csv.
   *
   * @return MCL_CSV_FILE
   */
  public function getCSV() {
    return $this->csv;
  }

  /**
   * Updates the csv.
   *
   * @param MCL_CSV_FILE $csv
   */
  public function setCSV($csv) {
    $this->csv = $csv;
  }

  /**
   * Retrieves the line no.
   *
   * @return integer
   */
  public function getLineNo() {
    return $this->line_no;
  }

  /**
   * Updates the line no.
   *
   * @param integer $line_no
   */
  public function setLineNo($line_no) {
    $this->line_no = $line_no;
  }
}