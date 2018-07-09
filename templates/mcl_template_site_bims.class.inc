<?php
/**
 * The declaration of MCL_TEMPLATE_SITE_BIMS class.
 *
 */
class MCL_TEMPLATE_SITE_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'SITE';
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
    $desc = 'The description for site sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'site_name'       => array('req' => TRUE,  'width' => 10, 'desc' => "Curator-assigned name of the site."),
      'site_long_name'  => array('req' => FALSE, 'width' => 10, 'desc' => "long name of the site."),
      'type'            => array('req' => TRUE,  'width' => 10, 'desc' => "'orchard', 'nursery' or any other types of location."),
      'latitude'        => array('req' => FALSE, 'width' => 10, 'desc' => "The decimal latitude coordinate of the georeference, using positive and negative sign to indicate N and S, respectively."),
      'longitude'       => array('req' => FALSE, 'width' => 10, 'desc' => "The decimal longitude coordinate of the georeference, using positive and negative sign to indicate E and W, respectively."),
      'altitude'        => array('req' => FALSE, 'width' => 10, 'desc' => "The altitude (elevation) of the location in meters. If the altitude is only known as a range, this is the average, and altitude_dev will hold half of the width of the range."),
      'geodetic_datum'  => array('req' => FALSE, 'width' => 10, 'desc' => "The geodetic system on which the geo-reference coordinates are based. For geo-references measured between 1984 and 2010, this will typically be WGS84."),
      'country'         => array('req' => FALSE, 'width' => 10, 'desc' => "Country where the site belongs to."),
      'state'           => array('req' => FALSE, 'width' => 10, 'desc' => "State where the site belongs to."),
      'region'          => array('req' => FALSE, 'width' => 10, 'desc' => "Region where the site belongs to."),
      'address'         => array('req' => FALSE, 'width' => 10, 'desc' => "The entire address except the country."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any further comments on the site."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['type']           = -1;
    $cvterms['SITE_CV']['site_long_name'] = -1;
    $cvterms['SITE_CV']['country']        = -1;
    $cvterms['SITE_CV']['state']          = -1;
    $cvterms['SITE_CV']['region']         = -1;
    $cvterms['SITE_CV']['address']        = -1;
    $cvterms['SITE_CV']['comments']       = -1;
    $cvterms['SITE_CV']['site_code']      = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks for the contact type.
    if (!MCL_DATA_VALID_TYPE::validate('location_type', $line['type'])) {
      $this->updateMsg('E', 'Invalid location type : ' . $line['type']);
    }
  }

    /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets the prefix.
    $prefix = BIMS_PROGRAM::getPrefixByID($program_id);

    // Gets BIMS_CHADO tables.
    $bc_site = new BIMS_CHADO_SITE($program_id);

    // Adds a site.
    $details = array(
      'description'     => $prefix . $line['site_name'],
      'altitude'        => $line['altitude'],
      'latitude'        => $line['latitude'],
      'longitude'       => $line['longitude'],
      'geodetic_datum'  => $line['geodetic_datum'],
    );
    $nd_geolocation_id = $bc_site->addSite($this, $details);
    if ($nd_geolocation_id) {
      $bc_site->setNdGeolocationID($nd_geolocation_id);

      // Adds the properties.
      $bc_site->addProp($this, 'SITE_CV', 'type', $line['type']);
      $bc_site->addProp($this, 'SITE_CV', 'site_long_name', $line['site_long_name']);
      $bc_site->addProp($this, 'SITE_CV', 'country', $line['country']);
      $bc_site->addProp($this, 'SITE_CV', 'state', $line['state']);
      $bc_site->addProp($this, 'SITE_CV', 'region', $line['region']);
      $bc_site->addProp($this, 'SITE_CV', 'address', $line['address']);
      $bc_site->addProp($this, 'SITE_CV', 'country', $line['country']);
      $bc_site->addProp($this, 'SITE_CV', 'comments', $line['comments']);
      $bc_site->addProp($this, 'SITE_CV', 'description', $line['description']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $bc_site, $line);
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Lists the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv_arr['site']['cv_id'], $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the special columns");
    }
  }
}