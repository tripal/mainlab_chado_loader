<?php
/**
 * The declaration of MCL_TEMPLATE_FB_FIELD_DATABASE class.
 *
 */
class MCL_TEMPLATE_FB_FIELD_DATABASE extends MCL_TEMPLATE {

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
      'person'    => array('req' => FALSE,  'width' => 15, 'desc' => "Name of the person who enter the data."),
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
   * @see MCL_TEMPLATE::checkHeaders()
   */
  public function checkHeaders($line) {

    // Lists the job paremters.
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

    // Lists the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $cv_arr     = $this->getJob()->getParamByKey('cv_arr');
    $trial_name = $this->getJob()->getParamByKey('trial_name');
    $site_name  = $this->getJob()->getParamByKey('site_name');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_accession  = new BIMS_CHADO_ACCESSION($program_id);
    $bc_project    = new BIMS_CHADO_PROJECT($program_id);
    $bc_site       = new BIMS_CHADO_SITE($program_id);

    // Gets the FB required columns.
    $bims_program = BIMS_PROGRAM::byID($program_id);
    $bims_cols    = $bims_program->getBIMSCols();

    // Gets the accession.
    $accession = $line[strtolower($bims_cols['accession'])];

    // Checks trial.
    $bc_project->checkProject($this, $prefix . $trial_name);

    // Checks site.
    if ($line['location']) {
      $bc_site->checkSite($this, $prefix . $line['location']);
    }
    else if ($site_name) {
      $bc_site->checkSite($this, $prefix . $site_name);
    }

    // Checks person.
    MCL_CHADO_CONTACT::checkContact($this, $line['person']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks accession.
    $bc_accession->checkAccession($this, $prefix . $accession, $line['genus'], $line['species']);

    // Checks trait (descriptor).
    MCL_CHADO_TRAIT::checkTrait($this, $line['trait'], $cv_arr['descriptor']['name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id     = $this->getJob()->getParamByKey('program_id');
    $trial_name     = $this->getJob()->getParamByKey('trial_name');
    $site_name      = $this->getJob()->getParamByKey('site_name');
    $sample_props   = $this->getJob()->getParamByKey('sample_props');
    $cv_arr         = $this->getJob()->getParamByKey('cv_arr');
    $default_values = $this->getJob()->getParamByKey('default_values');

    // Gets FB required columns.
    $bims_program     = BIMS_PROGRAM::byID($program_id);
    $bims_cols        = $bims_program->getBIMSCols();
    $accession        = $line[strtolower($bims_cols['accession'])];
    $unique_id        = $line[strtolower($bims_cols['unique_id'])];
    $primary_order    = $line[strtolower($bims_cols['primary_order'])];
    $secondary_order  = $line[strtolower($bims_cols['secondary_order'])];

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_pc        = new BIMS_CHADO_PHENOTYPE_CALL($program_id);
    $bc_accession = new BIMS_CHADO_ACCESSION($program_id);
    $bc_sample    = new BIMS_CHADO_ACCESSION($program_id);
    $bc_project   = new BIMS_CHADO_PROJECT($program_id);
    $bc_site      = new BIMS_CHADO_SITE($program_id);
    $bc_image     = new BIMS_CHADO_IMAGE($program_id);

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Gets nd_geolocation_id.
    $nd_geolocation_id = MCL_VAR::getValueByName('ND_GEOLOCATION_ID');
    if ($line['location']) {
      $nd_geolocation_id = $bc_site->getLocationIDBySite($prefix . $line['location']);
    }
    else  if ($site_name) {
      $nd_geolocation_id = $bc_site->getLocationIDBySite($prefix . $site_name);
    }

    // Gets the project.
    $dataset = $bc_project->byTKey('project', array('name' => $prefix . $trial_name));

    // Gets the stock.
    $accession = $bc_accession->byTKey('accession', array('uniquename' => $prefix . $accession));
    if ($accession) {

      // Gets the contact ID.
      $contact_name = $line['person'] ? $line['person'] : 'bims.anonymous';
      $contact      = MCL_CHADO_CONTACT::byName($contact_name);
      $contact_id   = $contact->getContactID();

      // Creates sample ID.
      $sample_id = sprintf("%d.%d.%d.%d.%s.%s.%s", $dataset->project_id, $nd_geolocation_id, $contact_id, $accession->stock_id, $unique_id, $primary_order, $secondary_order);

      // Checks if it is a new sample.
      $new_sample_flag = TRUE;
      $sample = $bc_accession->byTKey('accession', array('uniquename' => $prefix . $sample_id));
      if ($sample) {
        $new_sample_flag = FALSE;
      }
      $details = array(
        'uniquename'  => $prefix . $sample_id,
        'name'        => $sample_id,
        'organism_id' => $accession->organism_id,
        'type_id'     => $this->cvterms['SITE_CV']['sample'],
      );
      $stock_id_sample = $bc_accession->addAccession($this, $details);
      if (!$stock_id_sample) {
        return FALSE;
      }
      $bc_sample->setStockID($stock_id_sample);

      // Add relationships and properties if it is a new sample.
      if (!$new_sample_flag) {

        // Adds relationships between stock and sample.
        $bc_sample->addRelatedAccession($this, $accession, $this->cvterms['SITE_CV']['sample_of']);

        // Adds sample properties.
        $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'unique_id', $unique_id);
        $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'primary_order', $primary_order);
        $bc_sample->addProp($this, 'BIMS_FIELD_BOOK', 'secondary_order', $secondary_order);
        $bc_sample->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
        foreach ($sample_props as $prop) {

          // Adds a property.
          $cvterm = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv_arr['sample']['name'], $prop);
          if ($cvterm) {
            $bc_sample->addProp($this, $cv_arr['sample']['name'], $prop, $line[$prop]);
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
        $cvterm_id  = MCL_CHADO_CVTERM::getCvterm($cv_arr['descriptor']['name'], $cvterm_name)->getCvtermID();
        $descriptor = MCL_CHADO_TRAIT::byID($cvterm_id);

        // Adds photos.
        if ($descriptor->getFormat() == 'photo') {
          $bc_image->addSampleImage($this, $bc_sample->getStockID(), $cvterm_id, $value);
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
          $this->updateMsg('E', "Error : failed to add phenotype_call.");
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Lists the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Checks if the custom table exists.
    $phenotype_call = new BIMS_CHADO_PHENOTYPE_CALL($program_id);
    if (!$phenotype_call->exists()) {
      $this->updateMsg('E', "The custom table does not exist.");
    }

    // Gets the site CV name.
    $site_cv_name = MCL_VAR::getValueByName('SITE_CV');

    // Checks db.
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));

    // Skip data checking
    if (FALSE) {
      // Keeps the default value of descriptors.
      $default_values = array();

      // Saves descriptors.
      $descriptors = array();

      // Checks the evaluation data for each descriptor.
      foreach ($data_lines as $data_line) {

        // Gets the properties of the cvterm.
        $cvterm_name = $data_line['trait'];

        // Gets the Field Book descriptor if not have one.
        if (!array_key_exists($cvterm_name, $descriptors)) {
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
    }

    // Saves the default value of descriptors.
    $this->getJob()->setParamByKey('default_values', $default_values);
    $this->getJob()->update();
  }

  /**
   * @see MCL_TEMPLATE::validColumns()
   */
  public function validColumns() {

    // Lists the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets BIMS_PROGRAM.
    $bims_program = BIMS_PROGRAM::byID($program_id);

    // Gets FB required columns.
    $fb_req_cols = $bims_program->getBIMSCols();

    // Gets sample property columns.
    $sample_props = $this->getJob()->getParamByKey('sample_props');
    $columns = array_map('strtolower', array_merge(array_values($fb_req_cols), $sample_props));
    return array_values($columns);
  }
}