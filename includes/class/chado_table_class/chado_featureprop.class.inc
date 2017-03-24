<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATUREPROP class.
 *
 */
class CHADO_FEATUREPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATUREPROP.
   */
  protected $member_arr     = NULL;
  protected $featureprop_id = NULL;
  protected $feature_id     = NULL;
  protected $type_id        = NULL;
  protected $value          = NULL;
  protected $rank           = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featureprop_id' => 'serial',
    'feature_id'     => 'integer',
    'type_id'        => 'integer',
    'value'          => 'text',
    'rank'           => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->featureprop_id = array_key_exists('featureprop_id', $details) ? $details['featureprop_id'] : '';
    $this->feature_id     = array_key_exists('feature_id', $details)     ? $details['feature_id']     : '';
    $this->type_id        = array_key_exists('type_id', $details)        ? $details['type_id']        : '';
    $this->value          = array_key_exists('value', $details)          ? $details['value']          : '';
    $this->rank           = array_key_exists('rank', $details)           ? $details['rank']           : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATUREPROP by key(s).
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
    $sql = "SELECT * FROM chado.featureprop WHERE " . implode(" AND " , $where_arr);
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
    return array(
      array('feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id')),
      array('type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id')),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
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
        $fields['feature_id'] = $this->feature_id;
        $fields['type_id']    = $this->type_id;
        $fields['value']      = $this->value;
        $fields['rank']       = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->featureprop_id = db_insert('chado.featureprop')
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
      $fields['feature_id'] = $this->feature_id;
      $fields['type_id']    = $this->type_id;
      $fields['value']      = $this->value;
      $fields['rank']       = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.featureprop')
        ->fields($fields)
        ->condition('featureprop_id', $this->featureprop_id, '=')
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

      db_delete('chado.featureprop')
        ->condition('featureprop_id', $this->featureprop_id, '=')
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
   * Retrieves the featureprop_id.
   *
   * @retrun serial
   */
  public function getFeaturepropID() {
    return $this->featureprop_id;
  }

  /**
   * Updates the featureprop_id.
   *
   * @param serial $featureprop_id
   */
  public function setFeaturepropID($featureprop_id) {
    $this->featureprop_id = $featureprop_id;
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