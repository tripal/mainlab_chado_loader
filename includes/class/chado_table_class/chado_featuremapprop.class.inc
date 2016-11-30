<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREMAPPROP class.
 *
 */
class CHADO_FEATUREMAPPROP  {

  /**
   *  Data members for CHADO_FEATUREMAPPROP.
   */
  protected $member_arr        = NULL;
  protected $featuremapprop_id = NULL;
  protected $featuremap_id     = NULL;
  protected $type_id           = NULL;
  protected $value             = NULL;
  protected $rank              = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featuremapprop_id' => 'serial',
    'featuremap_id'     => 'integer',
    'type_id'           => 'integer',
    'value'             => 'text',
    'rank'              => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->featuremapprop_id = array_key_exists('featuremapprop_id', $details) ? $details['featuremapprop_id'] : '';
    $this->featuremap_id     = array_key_exists('featuremap_id', $details)     ? $details['featuremap_id']     : '';
    $this->type_id           = array_key_exists('type_id', $details)           ? $details['type_id']           : '';
    $this->value             = array_key_exists('value', $details)             ? $details['value']             : '';
    $this->rank              = array_key_exists('rank', $details)              ? $details['rank']              : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREMAPPROP by key(s).
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
    $sql = "SELECT * FROM chado.featuremapprop WHERE " . implode(" AND " , $where_arr);
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
        $fields['type_id']       = $this->type_id;
        $fields['value']         = $this->value;
        $fields['rank']          = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->featuremapprop_id = db_insert('chado.featuremapprop')
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
      $fields['type_id']       = $this->type_id;
      $fields['value']         = $this->value;
      $fields['rank']          = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.featuremapprop')
        ->fields($fields)
        ->condition('featuremapprop_id', $this->featuremapprop_id, '=')
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

      db_delete('chado.featuremapprop')
        ->condition('featuremapprop_id', $this->featuremapprop_id, '=')
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
   * Retrieves the featuremapprop_id.
   *
   * @retrun serial
   */
  public function getFeaturemappropID() {
    return $this->featuremapprop_id;
  }

  /**
   * Updates the featuremapprop_id.
   *
   * @param serial $featuremapprop_id
   */
  public function setFeaturemappropID($featuremapprop_id) {
    $this->featuremapprop_id = $featuremapprop_id;
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
   * Retrieves the type_id.
   *
   * @retrun integer
   */
  public function getTypeID() {
    return $this->type_id;
  }

  /**
   * Updates the type_id.
   *
   * @param integer $type_id
   */
  public function setTypeID($type_id) {
    $this->type_id = $type_id;
  }

  /**
   * Retrieves the value.
   *
   * @retrun text
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Updates the value.
   *
   * @param text $value
   */
  public function setValue($value) {
    $this->value = $value;
  }

  /**
   * Retrieves the rank.
   *
   * @retrun integer
   */
  public function getRank() {
    return $this->rank;
  }

  /**
   * Updates the rank.
   *
   * @param integer $rank
   */
  public function setRank($rank) {
    $this->rank = $rank;
  }
}