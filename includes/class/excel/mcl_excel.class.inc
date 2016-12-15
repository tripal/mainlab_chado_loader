<?php
/**
 * The declaration of MCL_EXCEL class.
 *
 */
$lib_path = mcl_get_config_setting('mcl_library_dir');
require_once("$lib_path/PHPZip/zip.lib.php");
require_once("$lib_path/PHPExcel/PHPExcel.php");
require_once("$lib_path/PHPExcel/PHPExcel/Writer/Excel2007.php");
require_once("$lib_path/PHPExcel/PHPExcel/IOFactory.php");

class MCL_EXCEL {

  /**
   *  Data members for MCL_EXCEL.
   */
  private $excel          = NULL;
  private $creator        = NULL;
  private $filename       = NULL;
  private $filepath       = NULL;
  private $title          = NULL;
  private $subject        = NULL;
  private $keywords       = NULL;
  private $category       = NULL;
  private $description    = NULL;

  /*
   * Version
   *
   * This version of class supports ('Excel2007' and 'Excel5')
   */
  private $version = NULL;

  /*
   *  Sheets array
   *
   *  $sheet['name']      : name
   *  $sheet['headers']   : headers array (optional)
   *
   *
   *  > Header array
   *
   *  $header['A'] = array('heading' => 'dataset_name');
   *  $header['B'] = array('heading' => 'ID');
   *  $header['C'] = array('heading' => 'smaple_ID');
   *
   */
  private $sheets = NULL;

  /**
   * Implements the class constructor.
   *
   * @param $details
  */
  public function __construct($details = array()) {

    // Initializes data members.
    $this->creator      = array_key_exists('creator', $details)     ? $details['creator']     : 'UNKNOWN';
    $this->filepath     = array_key_exists('filepath', $details)    ? $details['filepath']    : '';
    $this->filename     = array_key_exists('filename', $details)    ? $details['filename']    : '';
    $this->title        = array_key_exists('title', $details)       ? $details['title']       : 'NEW TITLE';
    $this->subject      = array_key_exists('subject ', $details)    ? $details['subject ']    : '';
    $this->keywords     = array_key_exists('keywords ', $details)   ? $details['keywords ']   : '';
    $this->category     = array_key_exists('category', $details)    ? $details['category']    : '';
    $this->version      = array_key_exists('version', $details)     ? $details['version']     : '';
    $this->sheets       = array_key_exists('sheets', $details)      ? $details['sheets']      : array();
    $this->header_flag  = array_key_exists('header_flag', $details) ? $details['header_flag'] : TRUE;

    // Updates filename.
    if ($this->filepath && !$this->filename) {
      $this->filename = basename($this->filepath);
    }

    // Sets the default version.
    if (!$this->version) {
      $this->version = 'Excel2007';
    }

    // Gets the defualt precision.
    $def_precision = ini_get('precision');

    // Creates new PHPExcel object.
    $this->excel = new PHPExcel();

    // Re-set the precision. It seems that PHPExcel change the precision.
    ini_set('precision', $def_precision);

    // Sets the properties of an Excel file.
    $this->_setExcelProperties();
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {}

  /**
   * Creates an excel file.
   *
   * @param array $data
   */
  public function create($type = 'online') {

    // Returns if $this->sheets is empty.
    if (empty($this->sheets)) {
      return;
    }

    // Populates excel sheets.
    $sheet_no = 0;
    foreach ($this->sheets as $sheet) {

      // Sets header flag.
      $header_flag  = empty($sheet['headers']) ? FALSE : TRUE;
      $data_flag    = empty($sheet['data']) ? FALSE : TRUE;

      // Adds a new worksheet.
      if ($sheet_no) {
        $this->excel->createSheet();
      }

      // Creates an excel sheet.
      $this->excel->setActiveSheetIndex($sheet_no++);

      // Sets sheet name.
      $this->excel->getActiveSheet()->setTitle($sheet['name']);

      // Adds headers.
      if ($header_flag) {

        // Sets the styles of an Excel sheet.
        $this->_setExcelStyle($sheet['headers']);

        // Writes headers.
        foreach ($sheet['headers'] as $col_chr => $arr) {
          $this->excel->getActiveSheet()->setCellValue($col_chr . '1', $arr['heading']);
        }
      }

      // Adds data
      if ($data_flag) {

        // Adds data rows.
        foreach ($sheet['data'] as $idx => $row) {

          // Process a row.
          foreach ($row as $cell) {

            // Updates the cell location.
            $row_no = ($header_flag) ? $idx + 2 : $idx + 1;
            $loc = $cell['col'] . $row_no;

            // Adds a cell.
            $this->_addCell($loc, $cell);
          }
        }
      }
    }

    // Downloads the excel file.
    if ($type == 'online') {

      // Redirects output to a client web browser.
      header('Content-Type: application/vnd.openxmlformats-officedocument.spreadsheetml.sheet');
      header("Content-Disposition: attachment;filename=" . $this->filename);
      header('Cache-Control: max-age=0');
      $this->excel->setActiveSheetIndex(0);
      $objWriter = PHPExcel_IOFactory::createWriter($this->excel, $this->version);
      ob_end_clean();
      $objWriter->save('php://output');
      exit;
    }
    else {

      // Saves the excel file
      $this->excel->setActiveSheetIndex(0);
      $objWriter = PHPExcel_IOFactory::createWriter($this->excel, $this->version);
      $objWriter->save($this->filepath);
      return TRUE;
    }
  }

  /**
   * Adds a cell to the sheet.
   *
   * @param string $loc
   * @param array $cell
   */
  private function _addCell($loc, $cell) {

    // Type : text
    if ($cell['type'] == 'text') {
      $this->excel->getActiveSheet()->setCellValue($loc, $cell['value']);
    }

    // Type : hyperlink
    else if ($cell['type'] == 'link') {
      $this->excel->getActiveSheet()->setCellValue($loc, $cell['value']);
      if (array_key_exists('url', $cell)) {

        // Adds an underline and sets the color for the link.
        $this->excel->getActiveSheet()->getStyle($col_chr.$row_no)->getFont()->setColor(new PHPExcel_Style_Color( PHPExcel_Style_Color::COLOR_DARKBLUE ));
        $this->excel->getStyle($loc)->getFont()->setUnderline(PHPExcel_Style_Font::UNDERLINE_SINGLE);

        // Sets URL.
        $this->excel->getCell($loc)->getHyperlink()->setUrl($cell['url']);
      }
    }
  }

  /**
   * Creates an Excel file from CSV files.
   *
   * @param array $csv_files
   */
  public function createFromCSV($csv_files) {

    // Creates reader for reading CSV file.
    $reader = PHPExcel_IOFactory::createReader('CSV');

    // Creates Excel sheets.
    $idx = 0;
    foreach ($csv_files as $csv_filename => $csv_filepath) {

      // Gets the file.
      if (file_exists($csv_filepath)) {

        // Gets the template name at the top of the line.
        if (!($fdr = fopen($csv_filepath, 'r'))) {
          return FALSE;
        }
        $tmp = dirname($csv_filepath) . '/tmp';
        if (!($fdw = fopen($tmp, 'w'))) {
          return FALSE;
        }

        // Copies the file except the first line.
        $line_no = 1;
        $template = '';
        while (!feof($fdr)) {
          $line = fgets($fdr);

          if ($line_no == 1) {
            $template = substr(trim($line), 1);
          }
          else {
            fputs($fdw, $line);
          }
          $line_no++;
        }
        fclose($fdr);
        fclose($fdw);

        // Loads CSV file.
        if ($idx == 0) {
          $this->excel = $reader->load($tmp);
        }
        else {
          $reader->setSheetIndex($idx);
          $reader->LoadIntoExisting($tmp, $this->excel);
        }

        // Sets the sheet name.
        $this->excel->getActiveSheet()->setTitle($template);
        $idx++;
      }
      else {
        return FALSE;
      }
    }

    // Saves them to the Excel file.
    $writer = PHPExcel_IOFactory::createWriter($this->excel, 'Excel2007');
    $writer->save($this->filepath);
    return TRUE;
  }

  /**
   * Sets the properties of the Excel file.
   *
   */
  private function _setExcelProperties() {

    // Sets Excel File Properties.
    if ($this->creator) {
      $this->excel->getProperties()->setCreator($this->creator);
      $this->excel->getProperties()->setLastModifiedBy($this->creator);
    }
    if ($this->title) {
      $this->excel->getProperties()->setTitle($this->title);
    }
    if ($this->subject) {
      $this->excel->getProperties()->setSubject($this->subject);
    }
    if ($this->keywords) {
      $this->excel->getProperties()->setKeywords($this->keywords);
    }
    if ($this->category) {
      $this->excel->getProperties()->setCategory($this->category);
    }
    if ($this->description) {
      $this->excel->getProperties()->setDescription($this->description);
    }
  }

  /**
   * Sets the style of the Excel sheet.
   *
   * @param array $headers
   */
  private function _setExcelStyle($headers) {

    // Sets the default styles.
    $this->excel->getDefaultStyle()->getFont()->setName('Calibri');
    $this->excel->getDefaultStyle()->getFont()->setSize(11);

    // Sets the style for the headers.
    foreach ($headers as $col_chr => $arr) {
      if (array_key_exists('width', $arr)) {
        $this->excel->getActiveSheet()->getColumnDimension($col_chr)->setWidth($arr['width']);
      }
      $this->excel->getActiveSheet()->getStyle($col_chr.'1')->getFont()->setBold(true);
      $this->excel->getActiveSheet()->getStyle($col_chr.'1')->getFill()->setFillType(PHPExcel_Style_Fill::FILL_SOLID);
      $this->excel->getActiveSheet()->getStyle($col_chr.'1')->getFill()->getStartColor()->setARGB('FFCECECE');
    }
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the sheets.
   *
   * @retrun array
   */
  public function getSheets() {
    return $this->sheets;
  }

  /**
   * Updates the sheets.
   *
   * @param array $sheets
   */
  public function setSheets($sheets) {
    $this->sheets = $sheets;
  }

  /**
   * Retrieves the creator.
   *
   * @retrun string
   */
  public function getCreator() {
    return $this->creator;
  }

  /**
   * Updates the creator.
   *
   * @param string $creator
   */
  public function setCreator($creator) {
    $this->creator = $creator;
  }

  /**
   * Retrieves the filename.
   *
   * @retrun string
   */
  public function getFilename() {
    return $this->filename;
  }

  /**
   * Updates the filename.
   *
   * @param string $filename
   */
  public function setFilename($filename) {
    $this->filename = $filename;
  }

  /**
   * Retrieves the filepath.
   *
   * @retrun string
   */
  public function getFilepath() {
    return $this->filepath;
  }

  /**
   * Updates the filepath.
   *
   * @param string $filepath
   */
  public function setFilepath($filepath) {
    $this->filepath = $filepath;
  }

  /**
   * Retrieves the version.
   *
   * @retrun string
   */
  public function getVersion() {
    return $this->version;
  }

  /**
   * Updates the version.
   *
   * @param string $version
   */
  public function setVersion($version) {
    $this->version = $version;
  }

  /**
   * Retrieves the title.
   *
   * @retrun string
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Updates the title.
   *
   * @param string $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Retrieves the subject.
   *
   * @retrun string
   */
  public function getSubject() {
    return $this->subject;
  }

  /**
   * Updates the subject.
   *
   * @param string $subject
   */
  public function setSubject($subject) {
    $this->subject = $subject;
  }

  /**
   * Retrieves the keywords.
   *
   * @retrun string
   */
  public function getKeywords() {
    return $this->keywords;
  }

  /**
   * Updates the keywords.
   *
   * @param string $keywords
   */
  public function setKeywords($keywords) {
    $this->keywords = $keywords;
  }

  /**
   * Retrieves the category.
   *
   * @retrun string
   */
  public function getCategory() {
    return $this->category;
  }

  /**
   * Updates the category.
   *
   * @param string $category
   */
  public function setCategory($category) {
    $this->category = $category;
  }
}
