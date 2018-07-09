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
      'mother'          => array('req' => FALSE, 'width' => 13, 'desc' => "ID name or number for the seed parent for the cross.  This ID must also be an entry in the Stock sheet."),
      'mother_location' => array('req' => FALSE, 'width' => 17, 'desc' => "Location details for the mother of this cross (eg. Plant location, individual stock name, etc)."),
      'father'          => array('req' => FALSE, 'width' => 13, 'desc' => "ID name or number for the pollen parent for the cross.  This ID must also be an entry in the Stock sheet."),
      'father_location' => array('req' => FALSE, 'width' => 17, 'desc' => "Location details for the father of this cross (eg. Plant location, individual stock name, etc)."),
      'cross_date'      => array('req' => FALSE, 'width' => 15, 'desc' => "Date for the cross."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments on the cross."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Followed by property (cvterm) name. The data will be stored in property table."),
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

    // Gets the job paremters.
    $program_id       = $this->getJob()->getParamByKey('program_id');
    $project_id_cross = $this->getJob()->getParamByKey('project_id_cross');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_accession  = new BIMS_CHADO_ACCESSION($program_id);
    $bc_site       = new BIMS_CHADO_SITE($program_id);

    // Checks site.
    if ($line['site_name']) {
      $bc_site->checkSite($this, $prefix . $line['site_name']);
    }

    // Checks the parents only if 'progeny' sheet exists.
    $flag_progeny = $this->hasTemplate('progeny');

    // Checks maternal parent.
    if ($line['mother'] && $line['mother'] != 'N/A' && !$flag_progeny) {
      $bc_accession->checkAccession($this, $prefix . $line['mother']);
    }

    // Checks paternal parent.
    if ($line['father'] && $line['father'] != 'N/A' && !$flag_progeny) {
      $bc_accession->checkAccession($this, $prefix . $line['father']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id       = $this->getJob()->getParamByKey('program_id');
    $project_id_cross = $this->getJob()->getParamByKey('project_id_cross');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_accession  = new BIMS_CHADO_ACCESSION($program_id);
    $bc_site       = new BIMS_CHADO_SITE($program_id);
    $bc_cross      = new BIMS_CHADO_CROSS($program_id);

    // Gets nd_geolocation_id.
    $site_name = ($line['site_name']) ? $prefix . $line['site_name'] : '';

    // Adds a cross.
    $details = array(
      'ne_type_id'                => $this->cvterms['SITE_CV']['cross_experiment'],
      'nd_geolocation_id'         => $bc_site->getLocationIDBySite($site_name),
      'dup_key'                   => $prefix . $project_id_cross . '.' . $line['cross_number'],
      'nep_type_id_dup_key'       => $this->cvterms['SITE_CV']['cross_experiment'],
      'cross_number'              => $line['cross_number'],
      'nep_type_id_cross_number'  => $this->cvterms['SITE_CV']['cross_number'],
      'project_id_cross'          => $project_id_cross,
    );
    $nd_experiment_id = $bc_cross->addCross($this, $details);
    if ($nd_experiment_id) {
      $bc_cross->setNdExperimentID($nd_experiment_id);

      // Adds properties.
      $bc_cross->addProp($this, 'SITE_CV', 'cross_date', $line['cross_date']);
      $bc_cross->addProp($this, 'SITE_CV', 'mother_location', $line['mother_location']);
      $bc_cross->addProp($this, 'SITE_CV', 'father_location', $line['father_location']);
      $bc_cross->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $bc_cross, $line);

      // Checks the parental data. If any of parents does not exist in the stock table,
      // it could be a progeney in later years. So keeps it in $GLOBALS.

      // Adds a maternal parent.
      if ($line['mother'] && $line['mother'] != 'N/A') {
        $mother = $prefix . $line['mother'];
        $maternal_parent = $bc_accession->byUniquename($mother);
        if ($maternal_parent) {
          $bc_cross->addParent($this, 'm', $maternal_parent, $this->cvterms['SITE_CV']['maternal_parent']);
        }
        else {
           $GLOBALS['cross_bims'][$nd_experiment_id]['mother'] = $mother;
        }
      }

      // Adds a paternal parent.
      if ($line['father'] && $line['father'] != 'N/A') {
        $father = $prefix . $line['father'];
        $paternal_parent = $bc_accession->byUniquename($father);
        if ($paternal_parent) {
          $bc_cross->addParent($this, 'p', $paternal_parent, $this->cvterms['SITE_CV']['paternal_parent']);
        }
        else {
          $GLOBALS['cross_bims'][$nd_experiment_id]['father'] = $father;
        }
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
    if (!$this->saveSpecialColumns('##', $cv_arr['cross']['cv_id'], $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the special columns");
    }
  }
}