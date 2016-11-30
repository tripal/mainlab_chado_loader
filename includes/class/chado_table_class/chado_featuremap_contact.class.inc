<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREMAP_CONTACT class.
 *
 */
class CHADO_FEATUREMAP_CONTACT  {

  /**
   *  Data members for CHADO_FEATUREMAP_CONTACT.
   */
  protected $member_arr            = NULL;
  protected $featuremap_contact_id = NULL;
  protected $featuremap_id         = NULL;
  protected $contact_id            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featuremap_contact_id' => 'serial',
    'featuremap_id'         => 'integer',
    'contact_id'            => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr            = $details;
    $this->featuremap_contact_id = array_key_exists('featuremap_contact_id', $details) ? $details['featuremap_contact_id'] : '';
    $this->featuremap_id         = array_key_exists('featuremap_id', $details)         ? $details['featuremap_id']         : '';
    $this->contact_id            = array_key_exists('contact_id', $details)            ? $details['contact_id']            : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREMAP_CONTACT by key(s).
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
    $sql = "SELECT * FROM chado.featuremap_contact WHERE " . implode(" AND " , $where_arr);
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
        $fields['featuremap_id'] = $this->featuremap_id;
        $fields['contact_id']    = $this->contact_id;


        // Inserts the record.
        $this->featuremap_contact_id = db_insert('chado.featuremap_contact')
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
      $fields['featuremap_id'] = $this->featuremap_id;
      $fields['contact_id']    = $this->contact_id;


      // Updates the record.
      db_update('chado.featuremap_contact')
        ->fields($fields)
        ->condition('featuremap_contact_id', $this->featuremap_contact_id, '=')
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

      db_delete('chado.featuremap_contact')
        ->condition('featuremap_contact_id', $this->featuremap_contact_id, '=')
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
   * Retrieves the featuremap_contact_id.
   *
   * @retrun serial
   */
  public function getFeaturemapContactID() {
    return $this->featuremap_contact_id;
  }

  /**
   * Updates the featuremap_contact_id.
   *
   * @param serial $featuremap_contact_id
   */
  public function setFeaturemapContactID($featuremap_contact_id) {
    $this->featuremap_contact_id = $featuremap_contact_id;
  }

  /**
   * Retrieves the featuremap_id.
   *
   * @retrun integer
   */
  public function getFeaturemapID() {
    return $this->featuremap_id;
  }

  /**
   * Updates the featuremap_id.
   *
   * @param integer $featuremap_id
   */
  public function setFeaturemapID($featuremap_id) {
    $this->featuremap_id = $featuremap_id;
  }

  /**
   * Retrieves the contact_id.
   *
   * @retrun integer
   */
  public function getContactID() {
    return $this->contact_id;
  }

  /**
   * Updates the contact_id.
   *
   * @param integer $contact_id
   */
  public function setContactID($contact_id) {
    $this->contact_id = $contact_id;
  }
}