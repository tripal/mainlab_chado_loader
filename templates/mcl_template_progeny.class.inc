<?php
/**
 * The declaration of MCL_TEMPLATE_PROGENY class.
 *
 */
class MCL_TEMPLATE_PROGENY extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'PROGENY';
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
      'progeny_name'    => array('req' => TRUE,  'width' => 14, 'desc' => ""),
      'alias'           => array('req' => FALSE, 'width' => 14, 'desc' => "Record all the aliases for the cross here.  The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for an alias, just write the aliases without the specific type. (eg. Breeder:LG98-942; Institution:4321;2-F04-01)."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => ""),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "When only the genus is known, write 'sp.' in the species."),
      'dataset_name'    => array('req' => TRUE,  'width' => 14, 'desc' => ""),
      'cross_number'    => array('req' => TRUE,  'width' => 10, 'desc' => "Specific ID of a cross that generated the progeny; Must also be found in the Cross table."),
      'description'     => array('req' => FALSE, 'width' => 14, 'desc' => "Any other descriptive data about the selection or seedling."),
      'advanced_to_2nd' => array('req' => FALSE, 'width' => 10, 'desc' => "Yes if it is advanced to 2nd phase, No otherwise. Leave it empty if the selection decision is yet to be made."),
      'advanced_to_3rd' => array('req' => FALSE, 'width' => 10, 'desc' => "Yes if it is advanced to 3rd phase, No otherwise."),
      'advanced_to_4th' => array('req' => FALSE, 'width' => 10, 'desc' => "Yes if it is advanced to 4th phase, No otherwise."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['progeny']                    = -1;
    $cvterms['SITE_CV']['breeding_research_material'] = -1;
    $cvterms['SITE_CV']['cross_number']               = -1;
    $cvterms['SITE_CV']['cross_experiment']           = -1;
    $cvterms['SITE_CV']['maternal_parent']            = -1;
    $cvterms['SITE_CV']['paternal_parent']            = -1;
    $cvterms['SITE_CV']['is_a_maternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_paternal_parent_of']    = -1;
    $cvterms['SITE_CV']['description']                = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks cross_number.
    $dataset = MCL_CHADO_DATASET::byName($line['dataset_name']);
    MCL_CHADO_ND_EXPERIMENT::checkCrossNumber($this, $dataset->getProjectID(), $line['cross_number'], $this->cvterms['SITE_CV']['cross_experiment'], $this->cvterms['SITE_CV']['cross_experiment']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Gets the dataset.
    $dataset = MCL_CHADO_DATASET::byName($line['dataset_name']);

    // Adds a progeny.
    $progeny = MCL_CHADO_STOCK::addStock($this, $line['progeny_name'], '', $organism_id, $this->cvterms['SITE_CV']['breeding_research_material']);
    if ($progeny) {

      // Adds the properties.
      $progeny->addProp($this, 'SITE_CV', 'description', $line['description']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $progeny, $line);

      // Gets the cross.
      $cross = MCL_CHADO_ND_EXPERIMENT::byCrossNumber($dataset->getProjectID(), $line['cross_number'], $this->cvterms['SITE_CV']['cross_experiment'], $this->cvterms['SITE_CV']['cross_experiment']);

      // Adds parents and links this progeny and cross.
      if ($cross) {

        // Adds the cross.
        $progeny->addCross($this, $cross, $this->cvterms['SITE_CV']['progeny']);

        // Adds the maternal parent.
        $mother = $cross->getRelatedStock($this->cvterms['SITE_CV']['maternal_parent']);
        if ($mother) {
          $mother->addRelatedStock($this, $progeny, $this->cvterms['SITE_CV']['is_a_maternal_parent_of']);
        }

        // Adds the paternal parent.
        $father = $cross->getRelatedStock($this->cvterms['SITE_CV']['paternal_parent']);
        if ($father) {
          $father->addRelatedStock($this, $progeny, $this->cvterms['SITE_CV']['is_a_paternal_parent_of']);
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($this->getJob()->getParamByKey('cv'), 'SITE_CV');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $data_lines[0])) {
      return;
    }
  }
}