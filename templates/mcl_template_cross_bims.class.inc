<?php
/**
 * The declaration of MCL_TEMPLATE_CROSS_BIMS class.
 *
 */
class MCL_TEMPLATE_CROSS_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'CROSS';
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
    $desc = 'The description for cross sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'site_name'       => array('req' => FALSE, 'width' => 10, 'desc' => "Site information where the stock for the phenotyping is planted. It should match 'site_name' in the 'Site' sheet."),
      'cross_number'    => array('req' => TRUE,  'width' => 17, 'desc' => "ID number or name for this specific cross."),
      'mother'          => array('req' => TRUE,  'width' => 13, 'desc' => "ID name or number for the seed parent for the cross.  This ID must also be an entry in the Stock sheet."),
      'mother_location' => array('req' => FALSE, 'width' => 17, 'desc' => "Location details for the mother of this cross (eg. Plant location, individual stock name, etc)."),
      'father'          => array('req' => TRUE,  'width' => 13, 'desc' => "ID name or number for the pollen parent for the cross.  This ID must also be an entry in the Stock sheet."),
      'father_location' => array('req' => FALSE, 'width' => 17, 'desc' => "Location details for the father of this cross (eg. Plant location, individual stock name, etc)."),
      'cross_date'      => array('req' => FALSE, 'width' => 15, 'desc' => "Date for the cross."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['cross_experiment'] = -1;
    $cvterms['SITE_CV']['cross_date']       = -1;
    $cvterms['SITE_CV']['cross_number']     = -1;
    $cvterms['SITE_CV']['mother_location']  = -1;
    $cvterms['SITE_CV']['father_location']  = -1;
    $cvterms['SITE_CV']['maternal_parent']  = -1;
    $cvterms['SITE_CV']['paternal_parent']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');
    if (!$program_id) {
      $this->updateMsg('E', "program_id not found in job.param");
    }

    // Checks cross dataset.
    $project_id_cross = $this->getJob()->getParamByKey('project_id_cross');
    if ($project_id_cross) {
      MCL_CHADO_DATASET::checkDatasetByID($this, $project_id_cross);
    }
    else {
      $this->updateMsg('E', "project_id_cross not found in job.param");
    }

    // Checks site.
    if ($line['site_name']) {
      MCL_CHADO_LOCATION::checkSite($this, $program_id . '.' . $line['site_name']);
    }

    // Checks maternal parent.
    if ($line['mother'] != 'N/A') {
      MCL_CHADO_STOCK::checkStock($this, $program_id . '.' . $line['mother']);
    }

    // Checks paternal parent.
    if ($line['father'] != 'N/A') {
      MCL_CHADO_STOCK::checkStock($this, $program_id . '.' . $line['father']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets cross dataset.
    $project_id_cross = $this->getJob()->getParamByKey('project_id_cross');
    $cross_dataset = MCL_CHADO_DATASET::byKey(array('project_id' => $project_id_cross));

    // Gets nd_geolocation_id.
    $site_name = $line['site_name'];
    if ($site_name) {
      $site_name = $program_id . '.' . $line['site_name'];
    }
    $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite($site_name);

    // Adds a nd_experiment.
    $dup_key = $project_id_cross . '_' . $line['cross_number'];
    $cross = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['cross_experiment'], $nd_geolocation_id, $this->cvterms['SITE_CV']['cross_experiment'], $dup_key);
    if ($cross) {

      // Adds properties.
      $cross->addProp($this, 'SITE_CV', 'cross_number', $line['cross_number']);
      $cross->addProp($this, 'SITE_CV', 'cross_date', $line['cross_date']);
      $cross->addProp($this, 'SITE_CV', 'mother_location', $line['mother_location']);
      $cross->addProp($this, 'SITE_CV', 'father_location', $line['father_location']);

      // Adds a dataset.
      $cross->addDataset($this, $cross_dataset->getName());

      // Adds a maternal parent.
      if ($line['mother'] != 'N/A') {
        $mother = $program_id . '.' . $line['mother'];
        $maternal_parent = MCL_CHADO_STOCK::getStock($mother);
        $cross->addStock($this, $maternal_parent, $this->cvterms['SITE_CV']['maternal_parent']);
      }

      // Adds a paternal parent.
      if ($line['father'] != 'N/A') {
        $father = $program_id . '.' . $line['father'];
        $paternal_parent = MCL_CHADO_STOCK::getStock($father);
        $cross->addStock($this, $paternal_parent, $this->cvterms['SITE_CV']['paternal_parent']);
      }
    }
  }
}