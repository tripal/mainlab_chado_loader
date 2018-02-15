<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_EIMAGE class.
 *
 */
class CHADO_EIMAGE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_EIMAGE.
   */
  protected $member_arr  = NULL;
  protected $eimage_id   = NULL;
  protected $eimage_data = NULL;
  protected $eimage_type = NULL;
  protected $image_uri   = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'eimage_id'   => 'serial',
    'eimage_data' => 'text',
    'eimage_type' => 'character_varying(255)',
    'image_uri'   => 'character_varying(255)',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->eimage_id   = array_key_exists('eimage_id', $details)   ? $details['eimage_id']   : '';
    $this->eimage_data = array_key_exists('eimage_data', $details) ? $details['eimage_data'] : '';
    $this->eimage_type = array_key_exists('eimage_type', $details) ? $details['eimage_type'] : '';
    $this->image_uri   = array_key_exists('image_uri', $details)   ? $details['image_uri']   : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_EIMAGE by key(s).
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
    $sql = "SELECT * FROM chado.eimage WHERE " . implode(" AND " , $where_arr);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    if ($details) {
      return new self($details);
    }
    return NULL;
  }

  /**
   * Returns the foreign keys.
   */
  public static function getFK() {
    return NULL;
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'eimage_id' => array(
        array('table' => 'eimageprop', 'attr' => 'eimage_id'),
        array('table' => 'feature_image', 'attr' => 'eimage_id'),
        array('table' => 'stock_image', 'attr' => 'eimage_id'),
      ),
    );
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
        $fields['eimage_data'] = $this->eimage_data;
        $fields['eimage_type'] = $this->eimage_type;
        $fields['image_uri']   = $this->image_uri;


        // Inserts the record.
        $this->eimage_id = db_insert('chado.eimage')
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
      $fields['eimage_data'] = $this->eimage_data;
      $fields['eimage_type'] = $this->eimage_type;
      $fields['image_uri']   = $this->image_uri;


      // Updates the record.
      db_update('chado.eimage')
        ->fields($fields)
        ->condition('eimage_id', $this->eimage_id, '=')
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
   * Deletes the record.
   */
  public function delete() {

    $transaction = db_transaction();
    try {

      db_delete('chado.eimage')
        ->condition('eimage_id', $this->eimage_id, '=')
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
   * Retrieves the eimage_id.
   *
   * @retrun serial
   */
  public function getEimageID() {
    return $this->eimage_id;
  }

  /**
   * Updates the eimage_id.
   *
   * @param serial $eimage_id
   */
  public function setEimageID($eimage_id) {
    $this->eimage_id = $eimage_id;
  }

  /**
   * Retrieves the eimage_data.
   *
   * @retrun text
   */
  public function getEimageData() {
    return $this->eimage_data;
  }

  /**
   * Updates the eimage_data.
   *
   * @param text $eimage_data
   */
  public function setEimageData($eimage_data) {
    $this->eimage_data = $eimage_data;
  }

  /**
   * Retrieves the eimage_type.
   *
   * @retrun character_varying(255)
   */
  public function getEimageType() {
    return $this->eimage_type;
  }

  /**
   * Updates the eimage_type.
   *
   * @param character_varying(255) $eimage_type
   */
  public function setEimageType($eimage_type) {
    $this->eimage_type = $eimage_type;
  }

  /**
   * Retrieves the image_uri.
   *
   * @retrun character_varying(255)
   */
  public function getImageUri() {
    return $this->image_uri;
  }

  /**
   * Updates the image_uri.
   *
   * @param character_varying(255) $image_uri
   */
  public function setImageUri($image_uri) {
    $this->image_uri = $image_uri;
  }
}