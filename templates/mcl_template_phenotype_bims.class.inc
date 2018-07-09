<?php
/**
 * The declaration of MCL_TEMPLATE_PHENOTYPE_BIMS class.
 *
 */
class MCL_TEMPLATE_PHENOTYPE_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'PHENOTYPE';
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
    $desc = 'The description for phenotype sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'      => array('req' => TRUE,  'width' => 15, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'accession'         => array('req' => TRUE,  'width' => 15, 'var' => TRUE, 'desc' => "ID of the accession that has been phenotyped. It should match an 'accession' column entry in the 'Accession' sheet or 'progeny_name' column entry 'Progeny' sheet."),
      'genus'             => array('req' => TRUE,  'width' => 10, 'desc' => "Genus to which the accession belongs to."),
      'species'           => array('req' => TRUE,  'width' => 10, 'desc' => "Species name. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'unique_id'         => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "Unique ID of sample"),
      'primary_order'     => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The primary order of a sample"),
      'secondary_order'   => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The secondary order of a sample"),
      'clone_id'          => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a spefic clone if available (eg. individual tree)."),
      'evaluator'         => array('req' => FALSE, 'width' => 15, 'desc' => "Person who did the phenotyping. Multiple person can be entered with ';' in between. It should match 'contact_name' of the Contact sheet."),
      'site_name'         => array('req' => FALSE, 'width' => 10, 'desc' => "Site information where the accession for the phenotyping is planted. It should match 'site_name' in the 'Site' sheet."),
      'rootstock'         => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the rootstock if the plant is grafted to a rootstock. It should match an 'accession' column of the 'Accession' sheet."),
      'plant_date'        => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the planting."),
      'data_year'         => array('req' => FALSE, 'width' => 10, 'desc' => "Phenotyping date if only year is known."),
      'evaluation_date'   => array('req' => FALSE, 'width' => 10, 'desc' => "Date of phenotype evaluation."),
      'pick_date'         => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the sample collection if the collection is done on a different date than the phenotype evaluation."),
      'previous_entry'    => array('req' => FALSE, 'width' => 10, 'desc' => "Accession of the previous entry."),
      'barcode'           => array('req' => FALSE, 'width' => 10, 'desc' => "Barcode"),
      'fiber_pkg'         => array('req' => FALSE, 'width' => 10, 'desc' => "Group of samples for phenotyping, can contain samples from multiple germplasm."),
      'storage_time'      => array('req' => FALSE, 'width' => 10, 'desc' => "Time between collection and phenotyping."),
      'storage_regime'    => array('req' => FALSE, 'width' => 10, 'desc' => "The condition of sample storage between the collection and phenotyping."),
      'comments'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the phenotyping."),
      '#descriptor_name'  => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by descriptor name."),
      '##property_name'   => array('req' => FALSE, 'width' => 10, 'desc' => "Followed by property (cvterm) name. The data will be stored in property table."),
    );

    // Adds labels.
    $headers['clone_id']['label'] = 'clone_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['rootstock_id']             = -1;
    $cvterms['SITE_CV']['nd_geolocation_id']        = -1;
    $cvterms['SITE_CV']['sample']                   = -1;
    $cvterms['SITE_CV']['sample_of']                = -1;
    $cvterms['SITE_CV']['clone']                    = -1;
    $cvterms['SITE_CV']['clone_of']                 = -1;
    $cvterms['SITE_CV']['data_year']                = -1;
    $cvterms['SITE_CV']['plant_date']               = -1;
    $cvterms['SITE_CV']['pick_date']                = -1;
    $cvterms['SITE_CV']['evaluation_date']          = -1;
    $cvterms['SITE_CV']['storage_time']             = -1;
    $cvterms['SITE_CV']['storage_regime']           = -1;
    $cvterms['SITE_CV']['previous_entry']           = -1;
    $cvterms['SITE_CV']['barcode']                  = -1;
    $cvterms['SITE_CV']['fiber_pkg']                = -1;
    $cvterms['SITE_CV']['comments']                 = -1;
    $cvterms['SITE_CV']['source']                   = -1;
    $cvterms['SITE_CV']['previous_entry']           = -1;
    $cvterms['BIMS_FIELD_BOOK']['unique_id']        = -1;
    $cvterms['BIMS_FIELD_BOOK']['primary_order']    = -1;
    $cvterms['BIMS_FIELD_BOOK']['secondary_order']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::checkHeaders()
   */
  public function checkHeaders($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $fb_req_cols  = $bims_program->getBIMSCols();
    $no_error = TRUE;
    foreach ($fb_req_cols as $col => $label) {
      if (!array_key_exists(strtolower($label), $line)) {
        $this->updateMsg('E', "The required column ($label) is missing");
        $no_error = FALSE;
      }
    }
    return $no_error;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_accession  = new BIMS_CHADO_ACCESSION($program_id);
    $bc_project    = new BIMS_CHADO_PROJECT($program_id);
    $bc_site       = new BIMS_CHADO_SITE($program_id);

    // Gets FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Checks accession.
    $accession = $line[strtolower($bims_cols['accession'])];
    $bc_accession->checkAccession($this, $prefix . $accession, $line['genus'], $line['species']);

    // Checks dataset.
    $bc_project->checkProject($this, $prefix . $line['dataset_name']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks rootstock.
    if ($line['rootstock']) {
      $bc_accession->checkAccession($this, $prefix . $line['rootstock']);
    }

    // Checks evaluator for contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['evaluator']);

    // Checks site.
    if ($line['site_name']) {
      $bc_site->checkSite($this, $prefix . $line['site_name']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Checks the data column. Skip this line if no data presents.
    if ($this->isEmptyRow($line, '^#')) {
      return TRUE;
    }

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

    // Gets FB required columns.
    $bims_program     = BIMS_PROGRAM::byID($program_id);
    $bims_cols        = $bims_program->getBIMSCols();
    $accession        = $line[strtolower($bims_cols['accession'])];
    $unique_id        = $line[strtolower($bims_cols['unique_id'])];
    $primary_order    = $line[strtolower($bims_cols['primary_order'])];
    $secondary_order  = $line[strtolower($bims_cols['secondary_order'])];

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Gets the dataset.
    $dataset = $bc_project->byTKey('project', array('name' => $prefix . $line['dataset_name']));

    // Gets nd_geolocation_id.
    $site_name = ($line['site_name']) ? $prefix . $line['site_name'] : '';
    $nd_geolocation_id = $bc_site->getLocationIDBySite($site_name);

    // Gets the accession.
    $args = array(
      'uniquename'  => $prefix . $accession,
      'organism_id' => $organism_id,
    );
    $accession = $bc_accession->byTKey('accession', $args);
    if ($accession) {

      // Gets the contact ID.
      $contact_name = $line['evaluator'] ? $line['evaluator'] : 'bims.anonymous';
      $contact      = MCL_CHADO_CONTACT::byName($contact_name);
      $contact_id   = $contact->getContactID();

      // Adds sample.
      $sample_id = sprintf("%d.%d.%d.%d.%s.%s.%s", $dataset->project_id, $nd_geolocation_id, $contact_id, $accession->stock_id, $unique_id, $primary_order, $secondary_order);
      $details = array(
        'uniquename'  => $prefix . $sample_id,
        'name'        => $sample_id,
        'organism_id' => $organism_id,
        'type_id'     => $this->cvterms['SITE_CV']['sample'],
      );
      $stock_id_sample = $bc_accession->addAccession($this, $details);
      if (!$stock_id_sample) {
        return FALSE;
      }
      $bc_sample->setStockID($stock_id_sample);

      // Adds clone.
      $bc_clone = NULL;
      if ($line['clone_id']) {
        $details = array(
          'uniquename'  => $prefix . $line['clone_id'],
          'name'        => $line['clone_id'],
          'organism_id' => $organism_id,
          'type_id'     => $this->cvterms['SITE_CV']['clone'],
        );
        $stock_id_clone = $bc_accession->addAccession($this, $details);
        if ($stock_id_clone) {
          $bc_clone = new BIMS_CHADO_ACCESSION($program_id);
          $bc_clone->setStockID($stock_id_clone);
        }
      }

      // Adds relationships between accession, sample and clone.
      if ($bc_clone) {
        $bc_clone->addRelatedAccession($this, $accession, $this->cvterms['SITE_CV']['clone_of']);
        $bc_sample->addRelatedAccession($this, $bc_clone->getObject('accession'), $this->cvterms['SITE_CV']['sample_of']);
      }
      $bc_sample->addRelatedAccession($this, $accession, $this->cvterms['SITE_CV']['sample_of']);

      // Gets rootstock_id.
      if ($line['rootstock']) {
        $keys = array(
          'uniquename'  => $prefix . $line['rootstock'],
          'organism_id' => $organism_id,
        );
        $rootstock = $bc_accession->byTKey('accession', $keys);
        if ($rootstock) {

          // Adds rootstock_id to smaple and clone.
          $bc_sample->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock->stock_id);
          if ($stock_id_clone) {
            $bc_clone->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock->stock_id);
          }
        }
      }

      // Adds clone properties.
      if ($stock_id_clone) {
        $bc_clone->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
        $bc_clone->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      }

      // Adds sample properties.
      $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'unique_id', $unique_id);
      $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'primary_order', $primary_order);
      $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'secondary_order', $secondary_order);
      $bc_sample->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
      $bc_sample->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      $bc_sample->addProp($this, 'SITE_CV', 'pick_date', $line['pick_date']);
      $bc_sample->addProp($this, 'SITE_CV', 'storage_time', $line['storage_time']);
      $bc_sample->addProp($this, 'SITE_CV', 'barcode', $line['barcode']);
      $bc_sample->addProp($this, 'SITE_CV', 'fiber_pkg', $line['fiber_pkg']);
      $bc_sample->addProp($this, 'SITE_CV', 'storage_regime', $line['storage_regime']);
      $bc_sample->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
      $bc_sample->addProp($this, 'SITE_CV', 'evaluation_date', $line['evaluation_date']);
      $bc_sample->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $bc_sample, $line);

      // Adds the previous-entry.
      $bc_sample->addPreviousEntry($this, $line['previous-entry']);

      // Adds the phenotyping data for each descriptor.
      $descriptors = $this->getJob()->getParamByKey('#');
      foreach ($descriptors as $cvterm_id => $descriptor) {
        $name = strtolower($descriptor['name']);

        // Gets the value.
        $value = $line['#' . $name];

        // Sets the default value if provided.
        if (!$value && $descriptor['defaultValue']) {
          $value = $descriptor['defaultValue'];
        }

        // Adds the value.
        if ($value || $value == '0') {

          // Updates the date if the value is the data type is 'date' 5 digits.
          if ($descriptor['format'] == 'date') {
            $value = MCL_CHADO_CVTERM::convertFBDate($value);
          }

          // Updates the boolean to be uppercase 'TRUE' or 'FALSE'.
          if ($descriptor['format'] == 'boolean') {
            $value = strtoupper($value);
          }

          // Adds photos.
          if ($descriptor['format'] == 'photo') {
            $bc_image->addSampleImage($this, $bc_sample->getStockID(), $cvterm_id, $value, "[;]");
          }

          // Adds a data point.
          $args = array(
            'project_id'        => $dataset->project_id,
            'nd_geolocation_id' => $nd_geolocation_id,
            'stock_id'          => $bc_sample->getStockID(),
            'cvterm_id'         => $cvterm_id,
            'value'             => $value,
            'contact_id'        => $contact_id,
            'time'              => date("Y-m-d G:i:s"),
          );
          $phenotype_call_id = $bc_pc->addData($this, $args);
          if (!$phenotype_call_id) {
            $this->updateMsg('E', "Error : Failed to add phenotype_call.");
          }
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Gets the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv_arr['sample']['cv_id'], $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the user-specific property columns [##]");
    }

    // Saves the descriptors.
    if (!$this->saveSpecialColumns('#', $cv_arr['descriptor']['cv_id'], $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the descriptor columns [#]");
    }

    // Checks the evaluation data for each descriptor.
    $descriptors = $this->getJob()->getParamByKey('#');
    foreach ($data_lines as $data_line) {
      foreach ($descriptors as $cvterm_id => $descriptor) {

        // Gets and checks the value.
        $name = strtolower($descriptor['name']);
        $value = $data_line['#' . $name];

        // Updates the value if the value is empty and the descriptor has the default value.
        $defaultvalue = $descriptor['defaultvalue'];
        if ($defaultvalue && $value == '') {
          $value = $defaultvalue;
        }

        // Checks the value.
        if ($value || $value == '0') {
          MCL_CHADO_CVTERM::checkFBDescriptorValue($this, $descriptor, $value, $data_line['line_no'] - 1);
        }
      }
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

  /**
   * @see MCL_TEMPLATE::validColumns()
   */
  public function validColumns() {

    // Gets BIMS_PROGRAM.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $bims_program = BIMS_PROGRAM::byID($program_id);

    // Returns FB required columns.
    return array_values($bims_program->getBIMSCols());
  }
}