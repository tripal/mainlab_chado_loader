<?php
/**
 * The declaration of MCL_TEMPLATE_FB_FIELD_DATABASE_ND class.
 *
 */
class MCL_TEMPLATE_FB_FIELD_DATABASE_ND extends MCL_TEMPLATE {

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
    $desc = 'The description for FB_FIELD_DATABASE sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'accession' => array('req' => TRUE,   'width' => 15, 'desc' => "ID of the accession that has been phenotyped."),
      'trait'     => array('req' => TRUE,   'width' => 12, 'desc' => "Trait of the value."),
      'value'     => array('req' => FALSE,  'width' => 12, 'desc' => "Value of the data."),
      'timestamp' => array('req' => TRUE,   'width' => 10, 'desc' => "timestamp of the data."),
      'trial'     => array('req' => FALSE,  'width' => 15, 'desc' => "Name of the trial."),
      'location'  => array('req' => FALSE,  'width' => 10, 'desc' => "Name of the location."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['phenotyping']              = -1;
    $cvterms['SITE_CV']['sample']                   = -1;
    $cvterms['SITE_CV']['sample_of']                = -1;
    $cvterms['BIMS_FIELD_BOOK']['unique_id']        = -1;
    $cvterms['BIMS_FIELD_BOOK']['primary_order']    = -1;
    $cvterms['BIMS_FIELD_BOOK']['secondary_order']  = -1;
    $cvterms['BIMS_FIELD_BOOK']['timestamp']        = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the job paremters.
    $program_id   = $this->getJob()->getParamByKey('program_id');
    $cv_name      = $this->getJob()->getParamByKey('cv');
    $trial_name   = $this->getJob()->getParamByKey('trial_name');
    $site_name    = $this->getJob()->getParamByKey('site_name');
    $fb_req_cols  = $this->getJob()->getParamByKey('fb_req_cols');

    // Gets the accession.
    $accession = $line[strtolower($fb_req_cols['accession'])];

    // Adds prefix.
    $accession  = $program_id . '.' . $accession;
    $trial_name = $program_id . '.' . $trial_name;

    // Checks trial.
    MCL_CHADO_DATASET::checkDataset($this, $trial_name);

    // Checks location.
    if ($site_name) {
      MCL_CHADO_LOCATION::checkSite($this, $program_id . '.' . $site_name);
    }

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $accession);

    // Checks trait (descriptor).
    MCL_CHADO_TRAIT::checkTrait($this, $line['trait'], $cv_name);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id   = $this->getJob()->getParamByKey('program_id');
    $trial_name   = $this->getJob()->getParamByKey('trial_name');
    $site_name    = $this->getJob()->getParamByKey('site_name');
    $sample_props = $this->getJob()->getParamByKey('sample_props');
    $fb_req_cols  = $this->getJob()->getParamByKey('fb_req_cols');
    $cv_name      = $this->getJob()->getParamByKey('cv');

    // Gets the accession.
    $accession = $line[strtolower($fb_req_cols['accession'])];

    // Adds prefix.
    $trial_name = $program_id . '.' . $trial_name;
    $accession  = $program_id . '.' . $accession;

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Gets the default values of the descriptors.
    $default_values = $this->getJob()->getParamByKey('default_values');

    // Gets nd_geolocation_id.
    $nd_geolocation_id = MCL_VAR::getValueByName('ND_GEOLOCATION_ID');
    if ($site_name) {
      $site_name  = $program_id . '.' . $site_name;
      $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite($site_name);
    }

    // Gets the project ID.
    $dataset = MCL_CHADO_DATASET::byName($trial_name);

    // Gets the stock.
    $accession = MCL_CHADO_STOCK::byKey(array('uniquename' => $accession));
    if ($accession) {

      // Gets FB required columns.
      $unique_id        = $line[$fb_req_cols['unique_id']];
      $primary_order    = $line[$fb_req_cols['primary_order']];
      $secondary_order  = $line[$fb_req_cols['secondary_order']];

      // Creates sample ID.
      $sample_id = sprintf("%d.%d.%d.%d.%s.%s.%s", $program_id, $dataset->getProjectID(), $nd_geolocation_id, $accession->getStockID(), $unique_id, $primary_order, $secondary_order);

      // Checks if it is a new sample.
      $new_sample_flag = TRUE;
      $sample = MCL_CHADO_STOCK::getStock($sample_id);
      if ($sample) {
        $new_sample_flag = FALSE;
      }

      // Adds sample.
      $sample = MCL_CHADO_STOCK::addStock($this, $sample_id, $sample_id, $accession->getOrganismID(), $this->cvterms['SITE_CV']['sample']);
      if (!$sample) {
        return FALSE;
      }

      // Add relationships and properties if it is a new sample.
      if (!$new_sample_flag) {

        // Adds relationships between stock and sample.
        $sample->addRelatedStock($this, $accession, $this->cvterms['SITE_CV']['sample_of']);

        // Adds sample properties.
        $sample->addProp($this, 'BIMS_FIELD_BOOK', 'unique_id', $unique_id);
        $sample->addProp($this, 'BIMS_FIELD_BOOK', 'primary_order', $primary_order);
        $sample->addProp($this, 'BIMS_FIELD_BOOK', 'secondary_order', $secondary_order);
        $sample->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
        foreach ($sample_props as $prop) {

          // Adds a cvterm.
          $cvterm = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv_name, $prop);
          if ($cvterm) {
            $sample->addProp($this, $cv_name, $prop, $line[$prop]);
          }
        }
      }

      // Gets the triat and the value.
      $cvterm_name  = $line['trait'];
      $value        = $line['value'];

      // Sets the default value if provided.
      if (!$value && $default_values[$cvterm_name]) {
        $value = $default_values[$cvterm_name];
      }

      // Adds the phenotyping data.
      if ($value || $value == '0') {

        // Gets the descriptor.
        $descriptor = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
        $cvterm_id  = $descriptor->getCvtermID();

        // Adds a phenotype.
        $uniquename = $program_id . '.' . $cvterm_id . '_' . $value;
        $phenotype = MCL_CHADO_PHENOTYPE::addPhenotype($this, $uniquename, $cvterm_id, $value);

        // Adds a nd_experiment.
        $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['phenotyping'], $nd_geolocation_id, $cvterm_id, $sample->getStockID());
        if ($nd_experiment) {

          // Adds a stock.
          $nd_experiment->addStock($this, $sample, $sample->getTypeID());

          // Adds a project.
          $nd_experiment->addDataset($this, $trial_name);

          // Adds a phenotype.
          $nd_experiment->addPhenotype($this, $phenotype);
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Checks Field Book required columns.
    $this->_checkFBReqCols($data_lines[0]);

    // Gets the site CV name.
    $site_cv_name = MCL_VAR::getValueByName('SITE_CV');

    // Checks db.
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));

    // Checks cv_name.
    $cv_name = $this->getJob()->getParamByKey('cv');
    MCL_CHADO_CV::checkCV($this, $cv_name, TRUE);

    // Keeps the default value of descriptors.
    $default_values = array();

    // Saves descriptors.
    $descriptors = array();

    // Checks the evaluation data for each descriptor.
    foreach ($data_lines as $data_line) {

      // Gets the properties of the cvterm.
      $cvterm_name = $data_line['trait'];

      // Gets the Field Book descriptor if not have one.
      if (array_key_exists($cvterm_name, $descriptors)) {
        $descriptor = MCL_CHADO_CVTERM::getFBDescriptor($site_cv_name, $cv_id, $cvterm_name);
        $descriptors[$cvterm_name]    = $descriptor;
        $default_values[$cvterm_name] = $descriptor['defaultvalue'];
      }

      // Gets the value.
      $value = $data_line['value'];

      // Updates the value if the value is empty and the descriptor has the default value.
      $defaultvalue = $descriptor['defaultvalue'];
      if ($defaultvalue && $value == '') {
        $value = $defaultvalue;
      }

      // Checks the value.
      if ($value || $value == '0') {
        MCL_CHADO_CVTERM::checkFBDescriptorValue($this, $descriptors[$cvterm_name], $value, $data_line['line_no'] - 1);
      }
    }

    // Saves the default value of descriptors.
    $this->getJob()->setParamByKey('default_values', $default_values);
    $this->getJob()->update();
  }

  /**
   * Checks if Field Book required columns exist.
   *
   * @param array $line
   */
  private function _checkFBReqCols($line) {

    // Gets Field Book required columns.
    $fb_req_cols = $this->getJob()->getParamByKey('fb_req_cols');
    foreach ($fb_req_cols as $req_col => $label) {
      if (!array_key_exists(strtolower($label), $line)) {
        $this->updateMsg('E', "The required column ($label) is missing");
      }
    }
  }
}