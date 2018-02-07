<?php
/**
 * The declaration of MCL_CHADO_LOCATION class.
 *
 */
class MCL_CHADO_LOCATION extends CHADO_ND_GEOLOCATION {

 /**
  *  Class data members.
  */
  private $type     = NULL;
  private $country  = NULL;
  private $state    = NULL;
  private $region   = NULL;
  private $address  = NULL;

  /**
   * @see CHADO_ND_GEOLOCATION::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_ND_GEOLOCATION::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_LOCATION by ID.
   *
   * @param integer $nd_geolocation_id
   *
   * @return MCL_CHADO_LOCATION
   */
  public static function byID($nd_geolocation_id) {
    return MCL_CHADO_LOCATION::bykey(array('nd_geolocation_id' => $nd_geolocation_id));
  }

  /**
   * Checks the site code. Write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string site_code
   *
   * @return boolean
   */
  public static function checkSiteCode(MCL_TEMPLATE $mcl_tmpl = NULL, $site_code) {
    if ($site_code) {

      // TODO:
      $description = $site_code;

      // Gets nd_geolocation.
      $args = array(
        'description' => $description,
      );
      $bim_location = MCL_CHADO_LOCATION::byKey($args);
      if (!$bim_location) {
        self::addMsg($mcl_tmpl, 'E', 'nd_geolocation', $args);
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns nd_geolocation_id of the provided site code. If site code
   * is empty, return the default nd_geolocation_id.
   *
   * @param string $site_code
   *
   * @return integer nd_geolocation_id
   */
  public static function getLocationIDBySiteCode($site_code) {

    // Sets the default nd_geolocation_id.
    $nd_geolocation_id = MCL_VAR::getValueByName('ND_GEOLOCATION_ID');

    if ($site_code) {

      // TODO:
      $description = $site_code;

      // Gets nd_geolocation.
      $args = array(
        'description' => $description,
      );
      $location = MCL_CHADO_LOCATION::byKey($args);
      if ($location) {
        $nd_geolocation_id = $location->getNdGeolocationID();
      }
      else {
        return NULL;
      }
    }
    return $nd_geolocation_id;
  }

  /**
   * Checks the site name. Write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $site_name
   *
   * @return boolean
   */
  public static function checkSite(MCL_TEMPLATE $mcl_tmpl = NULL, $site_name) {
    if ($site_name) {

      // Gets nd_geolocation.
      $args = array(
        'description' => $site_name,
      );
      $bim_location = MCL_CHADO_LOCATION::byKey($args);
      if (!$bim_location) {
        self::updateMsg($mcl_tmpl, 'E', "(description) = ($site_name) not found in nd_geolocation");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns nd_geolocation_id of the provided site name. If site name
   * is empty, return the default nd_geolocation_id.
   *
   * @param string $site_name
   *
   * @return integer nd_geolocation_id
   */
  public static function getLocationIDBySite($site_name) {

    // Sets the default nd_geolocation_id.
    $nd_geolocation_id = MCL_VAR::getValueByName('ND_GEOLOCATION_ID');

    // Gets the nd_geolocation_id.
    if ($site_name) {

      // Gets nd_geolocation.
      $args = array(
        'description' => $site_name,
      );
      $location = MCL_CHADO_LOCATION::byKey($args);
      if ($location) {
        $nd_geolocation_id = $location->getNdGeolocationID();
      }
      else {
        return NULL;
      }
    }
    return $nd_geolocation_id;
  }

  /**
   * Adds 'Not Available' location.
   *
   * @return MCL_CHADO_LOCATION
   */
  public static function addNA() {

    // Checks if it already exists.
    $location = MCL_CHADO_LOCATION::byKey(array('description' => 'Not Available'));
    if (!$location) {
      $details = array('description' => 'Not Available');
      $location = new MCL_CHADO_LOCATION($details);
      if (!$location->insert()) {
        return NULL;
      }
    }
    return $location;
  }

  /**
   * Adds a location.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   * @param array $loc_info
   *   Valid keys for $loc_info array
   *   - latitude
   *   - longitude
   *   - altitude
   *   - geodetic_datum
   *
   * @return MCL_CHADO_LOCATION
   */
  public static function addLocation(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $loc_info = NULL) {

    // Sets the arguments.
    $args = array('description' => $name);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks $loc_info.
    if (!empty($loc_info)) {
      $valid_keys = array('latitude', 'longitude', 'altitude', 'geodetic_datum');
      foreach ($loc_info as $key => $value) {
        if (!in_array($key, $valid_keys)) {
          self::updateMsg($mcl_tmpl, 'E', "$key is an invalid attribute of nd_geolocation table");
          return NULL;
        }
      }
    }

    // Checks for duplication.
    $location = MCL_CHADO_LOCATION::byKey($args);
    if ($location) {
      self::addMsg($mcl_tmpl, 'D', 'nd_geolocation', $args);
    }
    else {

      // Adds location information if provided.
      if ($loc_info) {
        $loc_info['description'] = $name;
        $args = $loc_info;
      }

      // Adds a new location.
      $location = new MCL_CHADO_LOCATION($args);
      if ($location->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'nd_geolocation', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'nd_geolocation', $args);
        return NULL;
      }
    }
    return $location;
  }

  /**
   * Gets property.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getProp($cv_name, $cvterm_name) {
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    return $this->getProperty('nd_geolocationprop', 'nd_geolocation_id', $this->nd_geolocation_id, $type_id);
  }

  /**
   * Adds a property by type ID (cvterm ID).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addPropByID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $value, $separator = '') {
    if ($value != '') {
      return $this->addProperty($mcl_tmpl, 'nd_geolocationprop', 'nd_geolocation_id', $this->nd_geolocation_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value, $separator = '') {
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'nd_geolocationprop', 'nd_geolocation_id', $this->nd_geolocation_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Updates the property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   *
   * @return boolean
   */
  public function updateProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value) {
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->updateProperty($mcl_tmpl, 'nd_geolocationprop', 'nd_geolocation_id', $this->nd_geolocation_id, $type_id, $value);
    }
    return TRUE;
  }
}