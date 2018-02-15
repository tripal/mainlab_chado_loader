<?php
/**
 * The declaration of MCL_TEMPLATE_PHENOTYPE_ND_BIMS class.
 *
 */
class MCL_TEMPLATE_PHENOTYPE_ND_BIMS extends MCL_TEMPLATE {

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
    $cvterms['SITE_CV']['phenotyping']              = -1;
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
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

     // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $bims_cols  = $this->getJob()->getParamByKey('bims_cols');

    // Gets the accession.
    $accession = $line[strtolower($bims_cols['accession'])];

    // Adds prefix.
    $dataset_name = $program_id . '.' . $line['dataset_name'];
    $accession    = $program_id . '.' . $accession;
    $clone_id     = $program_id . '.' . $line['clone_id'];

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $dataset_name);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks accession.
    MCL_CHADO_STOCK::checkStock($this, $accession, $line['genus'], $line['species']);

    // Checks rootstock.
    if ($line['rootstock']) {
      MCL_CHADO_STOCK::checkStock($this, $program_id . '.' . $line['rootstock'], $line['genus'], $line['species']);
    }

    // Checks evaluator for contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['evaluator'], '[;]');

    // Checks site.
    if ($line['site_name']) {
      MCL_CHADO_LOCATION::checkSite($this, $program_id . '.' . $line['site_name']);
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

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Gets the program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the dataset.
    $dataset_name = $program_id . '.' . $line['dataset_name'];
    $dataset = MCL_CHADO_DATASET::byName($dataset_name);

    // Gets nd_geolocation_id.
    $site_name = $line['site_name'];
    if ($site_name) {
      $site_name = $program_id . '.' . $site_name;
    }
    $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite($site_name);

    // Gets BIMS columns.
    $bims_cols        = $this->getJob()->getParamByKey('bims_cols');
    $accession        = $line[strtolower($bims_cols['accession'])];
    $unique_id        = $line[strtolower($bims_cols['unique_id'])];
    $primary_order    = $line[strtolower($bims_cols['primary_order'])];
    $secondary_order  = $line[strtolower($bims_cols['secondary_order'])];

    // Gets the accession.
    $args = array(
      'uniquename'  => $program_id . '.' . $accession,
      'organism_id' => $organism->getOrganismID(),
    );
    $accession = MCL_CHADO_STOCK::byKey($args);
    if ($accession) {

      // Adds sample.
      $sample_id = sprintf("%d.%d.%d.%d.%s.%s.%s", $program_id, $dataset->getProjectID(), $nd_geolocation_id, $accession->getStockID(), $unique_id, $primary_order, $secondary_order);
      $sample = MCL_CHADO_STOCK::addStock($this, $sample_id, $sample_id, $organism->getOrganismID(), $this->cvterms['SITE_CV']['sample']);
      if (!$sample) {
        return FALSE;
      }

      // Adds clone.
      $clone_id = $line['clone_id'];
      $clone = NULL;
      if ($clone_id) {
        $clone_id = $program_id . '.' . $clone_id;
        $clone = MCL_CHADO_STOCK::addStock($this, $clone_id, '', $organism->getOrganismID(), $this->cvterms['SITE_CV']['clone']);
      }

      // Adds relationships between accession, sample and clone.
      if ($clone) {
        $clone->addRelatedStock($this, $accession, $this->cvterms['SITE_CV']['clone_of']);
        $sample->addRelatedStock($this, $clone, $this->cvterms['SITE_CV']['sample_of']);
      }
      $sample->addRelatedStock($this, $accession, $this->cvterms['SITE_CV']['sample_of']);

      // Gets rootstock_id.
      $rootstock_id = '';
      if ($line['rootstock']) {
        $rootstock_name = $program_id . '.' . $line['rootstock'];
        $rootstock = MCL_CHADO_STOCK::getStock($rootstock_name, $line['genus'], $line['species']);
        if ($rootstock) {
          $rootstock_id = $rootstock->getStockID();
        }

        // Adds rootstock.
        $sample->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock_id);
        if ($clone) {
          $clone->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock_id);
        }
      }

      // Adds clone properties.
      if ($clone) {
        $clone->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
        $clone->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      }

      // Adds sample properties.
      $sample->addProp($this, 'BIMS_FIELD_BOOK', 'unique_id', $unique_id);
      $sample->addProp($this, 'BIMS_FIELD_BOOK', 'primary_order', $primary_order);
      $sample->addProp($this, 'BIMS_FIELD_BOOK', 'secondary_order', $secondary_order);
      $sample->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
      $sample->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      $sample->addProp($this, 'SITE_CV', 'pick_date', $line['pick_date']);
      $sample->addProp($this, 'SITE_CV', 'storage_time', $line['storage_time']);
      $sample->addProp($this, 'SITE_CV', 'barcode', $line['barcode']);
      $sample->addProp($this, 'SITE_CV', 'fiber_pkg', $line['fiber_pkg']);
      $sample->addProp($this, 'SITE_CV', 'storage_regime', $line['storage_regime']);
      $sample->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds user-specific sample properties.
      $this->addSpecialColumns('##', $sample, $line);

      // Adds previous-entry.
      $sample->addPreviousEntry($this, $line['previous-entry']);

      // Adds phenotyping data for each descriptor.
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

          // Adds a phenotype.
          $uniquename = $program_id . '.' . $cvterm_id . '_' . $value;
          $phenotype = MCL_CHADO_PHENOTYPE::addPhenotype($this, $uniquename, $cvterm_id, $value);

          // Adds a nd_experiment.
          $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['phenotyping'], $nd_geolocation_id, $cvterm_id, $sample->getStockID());
          if ($nd_experiment) {

            // Adds properties.
            $nd_experiment->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
            $nd_experiment->addProp($this, 'SITE_CV', 'evaluation_date', $line['evaluation_date']);

            // Adds the evaluators.
            $nd_experiment->addContact($this, $line['evaluator'], "[;]");

            // Adds the sample.
            $nd_experiment->addStock($this, $sample, $sample->getTypeID());

            // Adds the dataset.
            $nd_experiment->addDataset($this, $dataset_name);

            // Adds the phenotype.
            $nd_experiment->addPhenotype($this, $phenotype);
          }
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Checks the BIMS columns.
    $this->_checkBIMSCols($data_lines[0]);

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($this->getJob()->getParamByKey('cv'));
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $data_lines[0])) {
      return;
    }

    // Saves the descriptors.
    if (! $this->saveSpecialColumns('#', $cv->getCvID(), $data_lines[0])) {
      return;
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
   * Checks the BIMS columns exist.
   *
   * @param array $line
   */
  private function _checkBIMSCols($line) {
    $bims_cols = $this->getJob()->getParamByKey('bims_cols');
    foreach ($bims_cols as $bims_col => $label) {
      if (!array_key_exists(strtolower($label), $line)) {
        $this->updateMsg('E', "The required BIMS column ($label) is missing");
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
    $this->_updateHeaders($bims_program);

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