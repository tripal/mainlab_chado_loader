<?php
/**
 * The declaration of MCL_TEMPLATE_PHENOTYPE class.
 *
 */
class MCL_TEMPLATE_PHENOTYPE extends MCL_TEMPLATE {

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
      'dataset_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'stock_name'        => array('req' => TRUE,  'width' => 10, 'desc' => "ID of the stock that has been phenotyped. It should match a 'stock_name' column entry in the 'Stock' sheet."),
      'genus'             => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the stock"),
      'species'           => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the stock"),
      'sample_id'         => array('req' => TRUE,  'width' => 10, 'desc' => "The ID of the smallest sample or sub-sample that was tested (Each row should have unique sample_ID). If there is no unique sample ID, users can concatenate a set of columns that gives a unique phenotypic value and those columns become required fields. For example, if a combination of dataset_name, ID, phenotyping_date is unique for the specific phenotype, those three can be concatenated with '_' in between."),
      'clone_id'          => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a spefic clone if available (eg. individual tree)."),
      'evaluator'         => array('req' => FALSE, 'width' => 10, 'desc' => "Person who did the phenotyping. Multiple person can be entered with ';' in between. It should match 'contact_name' of the Contact sheet."),
      'site_name'         => array('req' => FALSE, 'width' => 10, 'desc' => "Site information where the stock for the phenotyping is planted. It should match 'site_name' in the 'Site' sheet."),
      'rep'               => array('req' => FALSE, 'width' => 10, 'desc' => "Any repetition number."),
      'rootstock'         => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the rootstock if the plant is grafted to a rootstock. It should match a 'stock_name' column of the 'Stock' sheet."),
      'plot'              => array('req' => FALSE, 'width' => 10, 'desc' => "Plot name."),
      'row'               => array('req' => FALSE, 'width' => 10, 'desc' => "Row."),
      'position'          => array('req' => FALSE, 'width' => 10, 'desc' => "Position of the plant."),
      'plant_date'        => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the planting."),
      'data_year'         => array('req' => FALSE, 'width' => 10, 'desc' => "Phenotyping date if only year is known."),
      'evaluation_date'   => array('req' => FALSE, 'width' => 10, 'desc' => "Date of phenotype evaluation."),
      'pick_date'         => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the sample collection if the collection is done on a different date than the phenotype evaluation."),
      'previous_entry'    => array('req' => FALSE, 'width' => 10, 'desc' => "stock ID of the previous entry."),
      'barcode'           => array('req' => FALSE, 'width' => 10, 'desc' => "Barcode"),
      'fiber_pkg'         => array('req' => FALSE, 'width' => 10, 'desc' => "Group of samples for phenotyping, can contain samples from multiple germplasm."),
      'storage_time'      => array('req' => FALSE, 'width' => 10, 'desc' => "Time between collection and phenotyping."),
      'storage_regime'    => array('req' => FALSE, 'width' => 10, 'desc' => "The condition of sample storage between the collection and phenotyping."),
      'comments'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the phenotyping."),
      '#descriptor_name'  => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by descriptor name."),
    );

    // Adds labels.
    $headers['sample_id']['label']  = 'sample_ID';
    $headers['clone_id']['label']   = 'clone_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['phenotyping']        = -1;
    $cvterms['SITE_CV']['nd_geolocation_id']  = -1;
    $cvterms['SITE_CV']['sample']             = -1;
    $cvterms['SITE_CV']['sample_of']          = -1;
    $cvterms['SITE_CV']['clone']              = -1;
    $cvterms['SITE_CV']['clone_of']           = -1;
    $cvterms['SITE_CV']['plot']               = -1;
    $cvterms['SITE_CV']['row']                = -1;
    $cvterms['SITE_CV']['position']           = -1;
    $cvterms['SITE_CV']['plant_date']         = -1;
    $cvterms['SITE_CV']['pick_date']          = -1;
    $cvterms['SITE_CV']['evaluation_date']    = -1;
    $cvterms['SITE_CV']['storage_time']       = -1;
    $cvterms['SITE_CV']['storage_regime']     = -1;
    $cvterms['SITE_CV']['previous_entry']     = -1;
    $cvterms['SITE_CV']['barcode']            = -1;
    $cvterms['SITE_CV']['fiber_pkg']          = -1;
    $cvterms['SITE_CV']['comments']           = -1;
    $cvterms['SITE_CV']['source']             = -1;
    $cvterms['SITE_CV']['previous_entry']     = -1;

    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organisms.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);

    // Checks rootstock.
    MCL_CHADO_STOCK::checkStock($this, $line['rootstock'], $line['genus'], $line['species']);

    // Checks evaluator for contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['evaluator'], '[;]');

    // Checks site.
    MCL_CHADO_LOCATION::checkSite($this, $line['site_name']);
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

    // Gets the stock.
    $args = array(
      'uniquename' => $line['stock_name'],
      'organism_id' => $organism->getOrganismID(),
    );
    $stock = MCL_CHADO_STOCK::byKey($args);
    if ($stock) {

      // Adds sample.
      $sample = MCL_CHADO_STOCK::addStock($this, $line['sample_id'], '', $organism->getOrganismID(), $this->cvterms['SITE_CV']['sample']);
      if (!$sample) {
        return FALSE;
      }

      // Adds clone.
      $clone = MCL_CHADO_STOCK::addStock($this, $line['clone_id'], '', $organism->getOrganismID(), $this->cvterms['SITE_CV']['clone']);

      // Adds relationships between stock, sample and clone.
      if ($clone) {
        $clone->addRelatedStock($this, $stock, $this->cvterms['SITE_CV']['clone_of']);
        $sample->addRelatedStock($this, $clone, $this->cvterms['SITE_CV']['sample_of']);
      }
      $sample->addRelatedStock($this, $stock, $this->cvterms['SITE_CV']['sample_of']);

      // Gets rootstock_id.
      $rootstock = MCL_CHADO_STOCK::getStock($line['rootstock'], $line['genus'], $line['species']);
      $rootstock_id = '';
      if ($rootstock) {
        $rootstock_id = $rootstock->getStockID();
      }

      // Gets nd_geolocation_id.
      $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite($line['site_name']);

      // Adds clone properties.
      if ($clone) {
        $clone->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock_id);
        $clone->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
        $clone->addProp($this, 'SITE_CV', 'rep', $line['rep']);
        $clone->addProp($this, 'SITE_CV', 'plot', $line['plot']);
        $clone->addProp($this, 'SITE_CV', 'row', $line['row']);
        $clone->addProp($this, 'SITE_CV', 'position', $line['position']);
        $clone->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      }

      // Adds sample properties.
      $sample->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock_id);
      $sample->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
      $sample->addProp($this, 'SITE_CV', 'rep', $line['rep']);
      $sample->addProp($this, 'SITE_CV', 'plot', $line['plot']);
      $sample->addProp($this, 'SITE_CV', 'row', $line['row']);
      $sample->addProp($this, 'SITE_CV', 'position', $line['position']);
      $sample->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      $sample->addProp($this, 'SITE_CV', 'pick_date', $line['pick_date']);
      $sample->addProp($this, 'SITE_CV', 'storage_time', $line['storage_time']);
      $sample->addProp($this, 'SITE_CV', 'barcode', $line['barcode']);
      $sample->addProp($this, 'SITE_CV', 'fiber_pkg', $line['fiber_pkg']);
      $sample->addProp($this, 'SITE_CV', 'storage_regime', $line['storage_regime']);
      $sample->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds previous-entry.
      $sample->addPreviousEntry($this, $line['previous-entry']);

      // Adds phenotyping data for each descriptor.
      $descriptors = $this->getJob()->getParamByKey('descriptors');
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
          $uniquename = $cvterm_id . '_' . $value;
          $phenotype = MCL_CHADO_PHENOTYPE::addPhenotype($this, $uniquename, $cvterm_id, $value);

          // Adds a nd_experiment.
          $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['phenotyping'], $nd_geolocation_id, $cvterm_id, $sample->getStockID());
          if ($nd_experiment) {

            // Adds properties.
            $nd_experiment->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
            $nd_experiment->addProp($this, 'SITE_CV', 'evaluation_date', $line['evaluation_date']);

            // Adds evaluators.
            $nd_experiment->addContact($this, $line['evaluator'], "[;]");

            // Adds a stock.
            $nd_experiment->addStock($this, $sample, $sample->getTypeID());

            // Adds a dataset.
            $nd_experiment->addDataset($this, $line['dataset_name']);

            // Adds a phenotype.
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

    // Saves the descriptor.
    $descriptors = $this->_saveDescriptors($data_lines[0]);

    // Checks the evaluation data for each descriptor.
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
   * Saves trait descriptors for cvterm_id. Save them in job.param.
   *
   * @param array $line
   *
   * @return array
   */
  private function _saveDescriptors($line) {

    // Gets the site CV name.
    $site_cv_name = MCL_VAR::getValueByName('SITE_CV');

    // Gets the cv for the descriptors.
    $cv_id = '';
    $cv_name = $this->getJob()->getParamByKey('cv');
    if ($cv_name) {
      $cv = MCL_CHADO_CV::getCV($cv_name);
      if (!$cv) {
        $this->updateMsg('E', "cv does not exist for prefix '#'.");
        return;
      }
      $cv_id = $cv->getCvID();
    }
    if (!$cv_id) {
      $cv = $this->getCvByPrefix('#');
      if (!$cv) {
        $this->updateMsg('E', "cv does not exist for prefix '#'.");
        return;
      }
      $cv_id = $cv->getCvID();
    }

    // Goes through headers stored in the line array.
    $descriptors = array();
    foreach ($line as $column => $value) {
      if (preg_match("/^#[A-Za-z0-9\_]/", $column)) {
        $cvterm_name = str_replace('#', '', $column);

        // Gets the Field Book descriptor.
        $descriptor = MCL_CHADO_CVTERM::getFBDescriptor($site_cv_name, $cv_id, $cvterm_name);
        if ($descriptor) {
          $descriptors[$descriptor['cvterm_id']] = $descriptor;
        }
        else {
          $this->updateMsg('E', "$cvterm_name not found in cvterm");
        }
      }
    }
    $this->getJob()->setParamByKey('descriptors', $descriptors);
    $this->getJob()->update();
    return $descriptors;
  }
}