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
   * Checks the site code. Write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string site_code
   */
  public static function checkSiteCode(MCL_TEMPLATE $mcl_tmpl, $site_code) {
    if ($site_code) {

      // TODO:
      $description = $site_code;

      // Gets nd_geolocation.
      $args = array(
        'description' => $description,
      );
      $bim_location = MCL_CHADO_LOCATION::byKey($args);
      if (!$bim_location) {
        $mcl_tmpl->addMsg('E', 'nd_geolocation', $args);
      }
    }
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
    $nd_geolocation_id = NULL;

    if ($site_code) {

      // TODO:
      $description = $site_code;

      // Gets nd_geolocation.
      $args = array(
        'description' => $description,
      );
      $mcl_location = MCL_CHADO_LOCATION::byKey($args);
      if ($mcl_location) {
        $nd_geolocation_id = $mcl_location->getNdGeolocationID();
      }
      else {
        $mcl_tmpl->addMsg('E', 'nd_geolocation', $args);
      }
    }
    else {

      // Returns a default nd_geolocation_id.
      $nd_geolocation_id = MCL_VAR::getValueByName('ND_GEOLOCATION_ID');
    }
    return $nd_geolocation_id;
  }

  /**
   * Checks the site name. Write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $site_name
   */
  public static function checkSite(MCL_TEMPLATE $mcl_tmpl, $site_name) {
    if ($site_name) {

      // Gets nd_geolocation.
      $args = array(
        'description' => $site_name,
      );
      $bim_location = MCL_CHADO_LOCATION::byKey($args);
      if (!$bim_location) {
        $mcl_tmpl->updateMsg('E', "(description) = ($site_name) not found in nd_geolocation");
      }
    }
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
      $mcl_location = MCL_CHADO_LOCATION::byKey($args);
      if ($mcl_location) {
        $nd_geolocation_id = $mcl_location->getNdGeolocationID();
      }
      else {
        $mcl_tmpl->addMsg('E', 'nd_geolocation', $args);
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

    // Checks if it has already exists.
    $mcl_location = MCL_CHADO_LOCATION::byKey(array('description' => 'Not Available'));
    if (!$mcl_location) {
      $details = array('description' => 'Not Available');
      $mcl_location = new MCL_CHADO_LOCATION($details);
      if (!$mcl_location->insert()) {
        return NULL;
      }
    }
    return $mcl_location;
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
  public static function addLocation(MCL_TEMPLATE $mcl_tmpl, $name, $loc_info = NULL) {

    // Sets the arguments.
    $args = array('description' => $name);

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks $loc_info.
    if (!empty($loc_info)) {
      $valid_keys = array('latitude', 'longitude', 'altitude', 'geodetic_datum');
      foreach ($loc_info as $key => $value) {
        if (!in_array($key, $valid_keys)) {
          $mcl_tmpl->updateMsg('E', "$key is an invalid attribute of nd_geolocation table");
          return NULL;
        }
      }
    }

    // Checks for duplication.
    $mcl_location = MCL_CHADO_LOCATION::byKey($args);
    if ($mcl_location) {
      $mcl_tmpl->addMsg('D', 'nd_geolocation', $args);
    }
    else {

      // Adds location information if provided.
      if ($loc_info) {
        $loc_info['description'] = $name;
        $args = $loc_info;
      }

      // Adds a new location.
      $mcl_location = new MCL_CHADO_LOCATION($args);
      if ($mcl_location->insert()) {
        $mcl_tmpl->addMsg('N', 'nd_geolocation', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'nd_geolocation', $args);
      }
    }
    return $mcl_location;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $value, $separator = '') {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    $mcl_tmpl->addProperty('nd_geolocationprop', 'nd_geolocation_id', $this->nd_geolocation_id, $type_id, $value, $separator);
  }
}