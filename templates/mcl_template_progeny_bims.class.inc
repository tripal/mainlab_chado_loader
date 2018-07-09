<?php
/**
 * The declaration of MCL_TEMPLATE_PROGENY_BIMS class.
 *
 */
class MCL_TEMPLATE_PROGENY_BIMS extends MCL_TEMPLATE {

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
      'progeny_name'    => array('req' => TRUE,  'width' => 14, 'desc' => "Progeny name."),
      'alias'           => array('req' => FALSE, 'width' => 14, 'desc' => "Record all the aliases for the cross here.  The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for an alias, just write the aliases without the specific type. (eg. Breeder:LG98-942; Institution:4321;2-F04-01)."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the stock"),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the stock. When only the genus is known, write 'sp.' in the species."),
      'cross_number'    => array('req' => TRUE,  'width' => 10, 'desc' => "Specific ID of a cross that generated the progeny; Must also be found in the Cross table."),
      'description'     => array('req' => FALSE, 'width' => 14, 'desc' => "Any other descriptive data about the selection or seedling."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Comments on the progeny."),
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
    $cvterms['SITE_CV']['description']                = -1;
    $cvterms['SITE_CV']['maternal_parent']            = -1;
    $cvterms['SITE_CV']['paternal_parent']            = -1;
    $cvterms['SITE_CV']['is_a_maternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_paternal_parent_of']    = -1;
    $cvterms['SITE_CV']['comments']                   = -1;
//    $cvterms['SITE_CV']['advanced_to_2nd']            = -1;
//    $cvterms['SITE_CV']['advanced_to_3rd']            = -1;
//    $cvterms['SITE_CV']['advanced_to_4th']            = -1;
    $cvterms['SITE_CV']['description']                = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id       = $this->getJob()->getParamByKey('program_id');
    $project_id_cross = $this->getJob()->getParamByKey('project_id_cross');

    // Gets BIMS_CHADO tables.
    $bc_accession  = new BIMS_CHADO_ACCESSION($program_id);
    $bc_cross      = new BIMS_CHADO_CROSS($program_id);

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Adds an accession.
    $details = array(
      'uniquename'  => $prefix . $line['progeny_name'],
      'name'        => $line['progeny_name'],
      'organism_id' => $organism_id,
      'type_id'     => $this->cvterms['SITE_CV']['breeding_research_material'],
    );
    $stock_id = $bc_accession->addAccession($this, $details);
    if ($stock_id) {
      $bc_accession->setStockID($stock_id);

      // Keesp all the progeny.
      $GLOBALS['progeny_bims'][$line['progeny_name']] = $stock_id;

      // Adds the properties.
//      $progeny->addProp($this, 'SITE_CV', 'advanced_to_2nd', $line['advanced_to_2nd']);
//      $progeny->addProp($this, 'SITE_CV', 'advanced_to_3rd', $line['advanced_to_3rd']);
//      $progeny->addProp($this, 'SITE_CV', 'advanced_to_4th', $line['advanced_to_4th']);
      $bc_accession->addProp($this, 'SITE_CV', 'description', $line['description']);
      $bc_accession->addProp($this, 'SITE_CV', 'comments', $line['comments']);
      $bc_accession->addProp($this, 'SITE_CV', 'alias', $line['alias'], "[,;]");

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $bc_accession, $line);

      // Gets the cross and adds the parents and the links this progeny and cross.
      $cross_number = $line['cross_number'];
      $cross = $bc_cross->byCrossNumber($cross_number, $project_id_cross);
      if ($cross) {

        // Adds the cross.
        $bc_accession->addCross($this, $cross, $this->cvterms['SITE_CV']);
      }

      // If cross could not find in the database, it could be in $GLOBALS since
      // their parents are progenis. So add them to $GLOBALS for now. Then add
      // them in the postProcess().
      else if (array_key_exists($cross_number, $GLOBALS['cross_bims'])) {
        $GLOBALS['cross_bims'][$cross_number]['progenies'][] = $stock_id;
      }

      // The cross does not exists in database and cross sheet.
      else {
        $this->updateMsg('E', " The cross number '$cross_number' cannot be found in the database nor Cross sheet.");
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Lists the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv_arr['progeny']['cv_id'], $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the special columns");
    }
  }

  /**
   * @see MCL_TEMPLATE::postProcess()
   */
  public function postProcess() {


    // Gets the job paramters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets BIMS_CHADO tables.
    $bc_accession  = new BIMS_CHADO_ACCESSION($program_id);
    $bc_cross      = new BIMS_CHADO_CROSS($program_id);

    // Adds all the cross in $GLOBALS.
    foreach ((array)$GLOBALS['cross_bims'] as $nd_experiment_id => $data) {
      $mother           = $data['mother'];
      $father           = $data['father'];
      $progenies        = $data['progenies'];
      $maternal_parent  = NULL;
      $paternal_parent  = NULL;

      // Sets the cross.
      $bc_cross->setNdExperimentID($nd_experiment_id);

      // Adds maternal parent.
      if ($mother) {
        $maternal_parent = $bc_accession->byUniquename($mother);
        if (!$maternal_parent && array_key_exists($mother, $GLOBALS['progeny_bims'])) {
          $maternal_parent = $GLOBALS['progeny_bims'][$mother];
        }
        if ($maternal_parent) {
          $bc_cross->addParent($this, 'm', $maternal_parent, $this->cvterms['SITE_CV']['maternal_parent']);
        }
        else {
          $this->updateMsg('E', " $mother found in Cross sheet does not exist in Accession nor Progeny sheet.");
        }
      }

      // Adds paternal parent.
      if ($father) {
        $paternal_parent = $bc_accession->byUniquename($father);
        if (!$paternal_parent && array_key_exists($father, $GLOBALS['progeny_bims'])) {
          $paternal_parent = $GLOBALS['progeny_bims'][$father];
        }
        if ($paternal_parent) {
          $bc_cross->addParent($this, 'p', $paternal_parent, $this->cvterms['SITE_CV']['paternal_parent']);
        }
        else {
          $this->updateMsg('E', " $father found in Cross sheet does not exist in Accession nor Progeny sheet.");
        }
      }

      // Adds the cross to progenies.
      foreach ((array)$progenies as $progeny) {
        $progeny->addCross($this, $cross, $this->cvterms['SITE_CV']);
      }
    }
  }
}