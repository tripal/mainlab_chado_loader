<?php
/**
 * The declaration of MCL_TEMPLATE_CROSS class.
 *
 */
class MCL_TEMPLATE_CROSS extends MCL_TEMPLATE {

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
      'dataset_name'    => array('req' => TRUE,  'width' => 17, 'desc' => "Name of the dataset (super-dataset or sub-category) to which the cross belongs. The details of the dataset should be given in the 'Dataset' sheet."),
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

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Gets genus and species from cross_number.
    $genus = '';
    $species = '';

    // Checks maternal parent.
    MCL_CHADO_STOCK::checkStock($this, $line['mother'], $genus, $species);

    // Checks paternal parent.
    MCL_CHADO_STOCK::checkStock($this, $line['father'], $genus, $species);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets genus and species from cross_number.




    // Adds a nd_eperiment.
    $dup_key = $line['dataset_name'] . '_' . $line['cross_number'];
    $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['cross_experiment'], $nd_geolocation_id, $this->cvterms['SITE_CV']['cross_number'], $dup_key);
    if ($nd_experiment) {

      // Adds properties.
      $nd_experiment->addProp($this, 'SITE_CV', 'cross_number', $line['cross_number']);
      $nd_experiment->addProp($this, 'SITE_CV', 'cross_date', $line['cross_date']);
      $nd_experiment->addProp($this, 'SITE_CV', 'mother_location', $line['mother_location']);
      $nd_experiment->addProp($this, 'SITE_CV', 'father_location', $line['father_location']);

      // Adds a dataset.
      $nd_experiment->addDataset($this, $line['dataset_name']);

      // Adds a maternal parent.
      $maternal_parent = MCL_CHADO_STOCK::getStock($line['mother'], $line['mother_genus'], $line['mother_species']);
      $nd_experiment->addStock($this, $maternal_parent, $this->cvterms['SITE_CV']['maternal_parent']);

      // Adds a paternal parent.
      $paternal_parent = MCL_CHADO_STOCK::getStock($line['father'], $line['father_genus'], $line['father_species']);
      $nd_experiment->addStock($this, $maternal_parent, $this->cvterms['SITE_CV']['paternal_parent']);;
    }
  }
}