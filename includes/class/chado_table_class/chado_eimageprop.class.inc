<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_EIMAGEPROP class.
 *
 */
class CHADO_EIMAGEPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_EIMAGEPROP.
   */
  protected $member_arr    = NULL;
  protected $eimageprop_id = NULL;
  protected $eimage_id     = NULL;
  protected $type_id       = NULL;
  protected $value         = NULL;
  protected $rank          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'eimageprop_id' => 'serial',
    'eimage_id'     => 'integer',
    'type_id'       => 'integer',
    'value'         => 'text',
    'rank'          => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr    = $details;
    $this->eimageprop_id = array_key_exists('eimageprop_id', $details) ? $details['eimageprop_id'] : '';
    $this->eimage_id     = array_key_exists('eimage_id', $details)     ? $details['eimage_id']     : '';
    $this->type_id       = array_key_exists('type_id', $details)       ? $details['type_id']       : '';
    $this->value         = array_key_exists('value', $details)         ? $details['value']         : '';
    $this->rank          = array_key_exists('rank', $details)          ? $details['rank']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_EIMAGEPROP by key(s).
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
    $sql = "SELECT * FROM chado.eimageprop WHERE " . implode(" AND " , $where_arr);
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
      array('eimage_id' => array('fk_table' => 'eimage', 'fk_attr' => 'eimage_id')),
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
        $fields['eimage_id'] = $this->eimage_id;
        $fields['type_id']   = $this->type_id;
        $fields['value']     = $this->value;
        if (is_numeric($this->rank)) { $fields['rank'] = $this->rank; }

        // Inserts the record.
        $this->eimageprop_id = db_insert('chado.eimageprop')
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
      $fields['eimage_id'] = $this->eimage_id;
      $fields['type_id']   = $this->type_id;
      $fields['value']     = $this->value;
      if (is_numeric($this->rank)) { $fields['rank'] = $this->rank; }

      // Updates the record.
      db_update('chado.eimageprop')
        ->fields($fields)
        ->condition('eimageprop_id', $this->eimageprop_id, '=')
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

      db_delete('chado.eimageprop')
        ->condition('eimageprop_id', $this->eimageprop_id, '=')
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
   * Retrieves the eimageprop_id.
   *
   * @retrun serial
   */
  public function getEimagepropID() {
    return $this->eimageprop_id;
  }

  /**
   * Updates the eimageprop_id.
   *
   * @param serial $eimageprop_id
   */
  public function setEimagepropID($eimageprop_id) {
    $this->eimageprop_id = $eimageprop_id;
  }

  /**
   * Retrieves the eimage_id.
   *
   * @retrun integer
   */
  public function getEimageID() {
    return $this->eimage_id;
  }

  /**
   * Updates the eimage_id.
   *
   * @param integer $eimage_id
   */
  public function setEimageID($eimage_id) {
    $this->eimage_id = $eimage_id;
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