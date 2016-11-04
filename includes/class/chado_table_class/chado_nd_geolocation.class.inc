<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ND_GEOLOCATION class.
 *
 */
class CHADO_ND_GEOLOCATION  {

 /**
  *  Data members for CHADO_ND_GEOLOCATION.
  */
  protected $member_arr        = NULL;
  protected $nd_geolocation_id = NULL;
  protected $description       = NULL;
  protected $latitude          = NULL;
  protected $longitude         = NULL;
  protected $geodetic_datum    = NULL;
  protected $altitude          = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'nd_geolocation_id' => 'serial',
    'description'       => 'character_varying(255)',
    'latitude'          => 'real',
    'longitude'         => 'real',
    'geodetic_datum'    => 'character_varying(32)',
    'altitude'          => 'real',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->nd_geolocation_id = array_key_exists('nd_geolocation_id', $details) ? $details['nd_geolocation_id'] : '';
    $this->description       = array_key_exists('description', $details)       ? $details['description']       : '';
    $this->latitude          = array_key_exists('latitude', $details)          ? $details['latitude']          : '';
    $this->longitude         = array_key_exists('longitude', $details)         ? $details['longitude']         : '';
    $this->geodetic_datum    = array_key_exists('geodetic_datum', $details)    ? $details['geodetic_datum']    : '';
    $this->altitude          = array_key_exists('altitude', $details)          ? $details['altitude']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ND_GEOLOCATION by key(s).
   */
  public static function byKey($keys) {

    // Returns NULL if non-integer value are given to the data member whose
    // data type is 'integer'.
    $where_arr = array();
    $args      = array();
    foreach ($keys as $key => $value) {

      // Returns NULL if $value is empty string.
      if ($value === '') {
        return NULL;
      }

      // Gets data type.
      $data_type = self::$data_types[$key];

      // Checks the value if data type is 'integer'.
      if (in_array($data_type, array('serial', 'integer'))) {
        if (!is_int((int)$value)) {
          return NULL;
        }
      }

      // Adds LOWER() function to the characters.
      if (preg_match("/^(char|text)/", $data_type)) {
        $where_arr []= " LOWER($key) = LOWER(:$key) ";
      }
      else {
        $where_arr []= " $key = :$key ";
      }
      $args[":$key"] = $keys[$key];
    }

    // Gets the table properties.
    $sql = "SELECT * FROM chado.nd_geolocation WHERE " . implode(" AND " , $where_arr);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    if ($details) {
      return new self($details);
    }
    return NULL;
  }

  /**
   * Adds a new record.
   */
  public function insert() {

    $transaction = db_transaction();
    try {

      // TODO:Checks for duplication.
      $dup = FALSE;

      // Inserts the record if not duplicated.
      if (!$dup) {

        // Populates all fields. The attribute with 'NOT NULL' would be skipped
        // if the value is empty.
        $fields = array();
        $fields['description']    = $this->description;
        $fields['geodetic_datum'] = $this->geodetic_datum;
        if (is_numeric($this->latitude))  { $fields['latitude']  = $this->latitude; }
        if (is_numeric($this->longitude)) { $fields['longitude'] = $this->longitude; }
        if (is_numeric($this->altitude))  { $fields['altitude']  = $this->altitude; }

        // Inserts the record.
        $this->nd_geolocation_id = db_insert('chado.nd_geolocation')
          ->fields($fields)
          ->execute();
      }
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Updates the record.
   */
  public function update() {

    $transaction = db_transaction();
    try {

      // Populates all fields. The attribute with 'NOT NULL' would be skipped
      // if the value is empty.
      $fields = array();
      $fields['description']    = $this->description;
      $fields['geodetic_datum'] = $this->geodetic_datum;
      if (is_numeric($this->latitude))  { $fields['latitude']  = $this->latitude; }
      if (is_numeric($this->longitude)) { $fields['longitude'] = $this->longitude; }
      if (is_numeric($this->altitude))  { $fields['altitude']  = $this->altitude; }

      // Updates the record.
      db_update('chado.nd_geolocation')
        ->fields($fields)
        ->condition('nd_geolocation_id', $this->nd_geolocation_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Delete the record.
   */
  public function delete() {

    $transaction = db_transaction();
    try {

      db_delete('chado.nd_geolocation')
        ->condition('nd_geolocation_id', $this->nd_geolocation_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the data type of the member.
   *
   * @retrun various
   */
  public function getDataType($member) {
    return $this->data_types[$member];
  }

  /**
   * Retrieves the data member array.
   *
   * @retrun array
   */
  public function getMemberArr() {
    return $this->member_arr;
  }

  /**
   * Updates the data member array.
   *
   * @param array $member_arr
   */
  public function setMemberArr($member_arr) {
    $this->member_arr = $member_arr;
  }

  /**
   * Retrieves the nd_geolocation_id.
   *
   * @retrun serial
   */
  public function getNdGeolocationID() {
    return $this->nd_geolocation_id;
  }

  /**
   * Updates the nd_geolocation_id.
   *
   * @param serial $nd_geolocation_id
   */
  public function setNdGeolocationID($nd_geolocation_id) {
    $this->nd_geolocation_id = $nd_geolocation_id;
  }

  /**
   * Retrieves the description.
   *
   * @retrun character_varying(255)
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Updates the description.
   *
   * @param character_varying(255) $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }

  /**
   * Retrieves the latitude.
   *
   * @retrun real
   */
  public function getLatitude() {
    return $this->latitude;
  }

  /**
   * Updates the latitude.
   *
   * @param real $latitude
   */
  public function setLatitude($latitude) {
    $this->latitude = $latitude;
  }

  /**
   * Retrieves the longitude.
   *
   * @retrun real
   */
  public function getLongitude() {
    return $this->longitude;
  }

  /**
   * Updates the longitude.
   *
   * @param real $longitude
   */
  public function setLongitude($longitude) {
    $this->longitude = $longitude;
  }

  /**
   * Retrieves the geodetic_datum.
   *
   * @retrun character_varying(32)
   */
  public function getGeodeticDatum() {
    return $this->geodetic_datum;
  }

  /**
   * Updates the geodetic_datum.
   *
   * @param character_varying(32) $geodetic_datum
   */
  public function setGeodeticDatum($geodetic_datum) {
    $this->geodetic_datum = $geodetic_datum;
  }

  /**
   * Retrieves the altitude.
   *
   * @retrun real
   */
  public function getAltitude() {
    return $this->altitude;
  }

  /**
   * Updates the altitude.
   *
   * @param real $altitude
   */
  public function setAltitude($altitude) {
    $this->altitude = $altitude;
  }
}