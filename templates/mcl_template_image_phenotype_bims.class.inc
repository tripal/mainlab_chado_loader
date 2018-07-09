<?php
/**
 * The declaration of MCL_TEMPLATE_IMAGE_PHENOTYPE_BIMS class.
 *
 */
class MCL_TEMPLATE_IMAGE_PHENOTYPE_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'IMAGE_PHENOTYPE';
    parent::__construct($details);
  }

  /**
   * @see MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for field book sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 15, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'unique_id'       => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "Unique ID of sample"),
      'primary_order'   => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The primary order of a sample"),
      'secondary_order' => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The secondary order of a sample"),
      'filename'        => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the image file."),
      'descriptor_name' => array('req' => TRUE,  'width' => 10, 'desc' => "The name of the desriptor of the photo."),
      'caption'         => array('req' => FALSE, 'width' => 10, 'desc' => "The caption of the photo."),
      'description'     => array('req' => FALSE, 'width' => 10, 'desc' => "The description of the photo image."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['photo_descriptor'] = -1;
    $cvterms['SITE_CV']['caption']          = -1;
    $cvterms['SITE_CV']['description']      = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $prefix . $line['dataset_name']);

    // Checks the image type.
    MCL_CHADO_CVTERM::checkCvterm($this, 'SITE_CV', $line['descriptor_name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_pc        = new BIMS_CHADO_PHENOTYPE_CALL($program_id);
    $bc_accession = new BIMS_CHADO_ACCESSION($program_id);
    $bc_sample    = new BIMS_CHADO_ACCESSION($program_id);
    $bc_project   = new BIMS_CHADO_PROJECT($program_id);
    $bc_site      = new BIMS_CHADO_SITE($program_id);
    $bc_image     = new BIMS_CHADO_IMAGE($program_id);

    // Gets the dataset.
    $dataset = $bc_project->byTKey('project', array('name' => $prefix . $line['dataset_name']));

    // Gets FB required columns.
    $bims_program     = BIMS_PROGRAM::byID($program_id);
    $bims_cols        = $bims_program->getBIMSCols();
    $accession        = $line[strtolower($bims_cols['accession'])];
    $unique_id        = $line[strtolower($bims_cols['unique_id'])];
    $primary_order    = $line[strtolower($bims_cols['primary_order'])];
    $secondary_order  = $line[strtolower($bims_cols['secondary_order'])];

    // Gets the sample.
    $keys = array(
      'project_id'      => $dataset->project_id,
      'unique_id'       => $unique_id,
      'primary_order'   => $primary_order ,
      'secondary_order' => $secondary_order,
    );
    $sample = $bc_pc->getSample($keys);
    if ($sample) {
/*
      // Sets the default image type if the type is not provided.
      $cvterm_id = $this->cvterms['SITE_CV']['photo'];
      if ($line['descriptor_name']) {
        $cvterm_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['descriptor_name'])->getCvtermID();
      }

      // Adds eimage.
      $eimage = MCL_CHADO_IMAGE::addImage($this, $line['filename'], 'filename', $line['filename'], $cvterm_id);
      if ($eimage) {

        // Adds the properties.
        $eimage->addProp($this, 'SITE_CV', 'caption', $line['caption']);
        $eimage->addProp($this, 'SITE_CV', 'description', $line['description']);

        // Adds the cvterm ID of the descriptor.
        $descriptor = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['descriptor_name']);
        $eimage->addProp($this, 'SITE_CV', 'photo_descriptor', $descriptor->getCvtermID());

        // Adds the user-specific properties.
        $this->addSpecialColumns('##', $eimage, $line);

        // Adds the images.
       // $bc_image->addSampleImage($this, $sample->getStockID(), $cvterm_id, $value);
       // $sample->addImage($this, 'filename', $value, $cvterm_id, "[;]");
      }
*/
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Lists the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv_arr['image']['cv_id'], $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the special columns");
    }
  }

  /**
   * @see MCL_TEMPLATE::getTemplateInfo()
   */
  public function getTemplateInfo() {

    // Gets BIMS_USER and BIMS_PROGRAM.
    $bims_user    = getBIMS_USER();
    $bims_program = $bims_user->getProgram();

    // Updates the headers.
    $bims_cols = $bims_program->getBIMSCols();
    $this->updateHeaders($bims_cols);

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
    foreach ($this->headers as $heading => $prop) {
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
    $regex = $this->getRegExSpCol();
    foreach ($this->headers as $heading => $prop) {
      $label  = $heading;
      $header = array();

      if (!preg_match("/^~/", $heading)) {
        if (preg_match($regex, $heading)) {
          continue;
        }
      }

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
}