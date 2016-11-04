<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREPOS class.
 *
 */
class CHADO_FEATUREPOS  {

 /**
  *  Data members for CHADO_FEATUREPOS.
  */
  protected $member_arr     = NULL;
  protected $featurepos_id  = NULL;
  protected $featuremap_id  = NULL;
  protected $feature_id     = NULL;
  protected $map_feature_id = NULL;
  protected $mappos         = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'featurepos_id'  => 'serial',
    'featuremap_id'  => 'serial',
    'feature_id'     => 'integer',
    'map_feature_id' => 'integer',
    'mappos'         => 'double',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->featurepos_id  = array_key_exists('featurepos_id', $details)  ? $details['featurepos_id']  : '';
    $this->featuremap_id  = array_key_exists('featuremap_id', $details)  ? $details['featuremap_id']  : '';
    $this->feature_id     = array_key_exists('feature_id', $details)     ? $details['feature_id']     : '';
    $this->map_feature_id = array_key_exists('map_feature_id', $details) ? $details['map_feature_id'] : '';
    $this->mappos         = array_key_exists('mappos', $details)         ? $details['mappos']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREPOS by key(s).
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
    $sql = "SELECT * FROM chado.featurepos WHERE " . implode(" AND " , $where_arr);
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
        $fields['feature_id']     = $this->feature_id;
        $fields['map_feature_id'] = $this->map_feature_id;
        $fields['mappos']         = $this->mappos;


        // Inserts the record.
        $this->featurepos_id = db_insert('chado.featurepos')
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
      $fields['feature_id']     = $this->feature_id;
      $fields['map_feature_id'] = $this->map_feature_id;
      $fields['mappos']         = $this->mappos;


      // Updates the record.
      db_update('chado.featurepos')
        ->fields($fields)
        ->condition('featurepos_id', $this->featurepos_id, '=')
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

      db_delete('chado.featurepos')
        ->condition('featurepos_id', $this->featurepos_id, '=')
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
   * Retrieves the featurepos_id.
   *
   * @retrun serial
   */
  public function getFeatureposID() {
    return $this->featurepos_id;
  }

  /**
   * Updates the featurepos_id.
   *
   * @param serial $featurepos_id
   */
  public function setFeatureposID($featurepos_id) {
    $this->featurepos_id = $featurepos_id;
  }

  /**
   * Retrieves the featuremap_id.
   *
   * @retrun serial
   */
  public function getFeaturemapID() {
    return $this->featuremap_id;
  }

  /**
   * Updates the featuremap_id.
   *
   * @param serial $featuremap_id
   */
  public function setFeaturemapID($featuremap_id) {
    $this->featuremap_id = $featuremap_id;
  }

  /**
   * Retrieves the feature_id.
   *
   * @retrun integer
   */
  public function getFeatureID() {
    return $this->feature_id;
  }

  /**
   * Updates the feature_id.
   *
   * @param integer $feature_id
   */
  public function setFeatureID($feature_id) {
    $this->feature_id = $feature_id;
  }

  /**
   * Retrieves the map_feature_id.
   *
   * @retrun integer
   */
  public function getMapFeatureID() {
    return $this->map_feature_id;
  }

  /**
   * Updates the map_feature_id.
   *
   * @param integer $map_feature_id
   */
  public function setMapFeatureID($map_feature_id) {
    $this->map_feature_id = $map_feature_id;
  }

  /**
   * Retrieves the mappos.
   *
   * @retrun double
   */
  public function getMappos() {
    return $this->mappos;
  }

  /**
   * Updates the mappos.
   *
   * @param double $mappos
   */
  public function setMappos($mappos) {
    $this->mappos = $mappos;
  }
}