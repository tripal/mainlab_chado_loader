<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CVTERMPROP class.
 *
 */
class CHADO_CVTERMPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_CVTERMPROP.
   */
  protected $member_arr    = NULL;
  protected $cvtermprop_id = NULL;
  protected $cvterm_id     = NULL;
  protected $type_id       = NULL;
  protected $value         = NULL;
  protected $rank          = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cvtermprop_id' => 'serial',
    'cvterm_id'     => 'integer',
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
    $this->cvtermprop_id = array_key_exists('cvtermprop_id', $details) ? $details['cvtermprop_id'] : '';
    $this->cvterm_id     = array_key_exists('cvterm_id', $details)     ? $details['cvterm_id']     : '';
    $this->type_id       = array_key_exists('type_id', $details)       ? $details['type_id']       : '';
    $this->value         = array_key_exists('value', $details)         ? $details['value']         : '';
    $this->rank          = array_key_exists('rank', $details)          ? $details['rank']          : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CVTERMPROP by key(s).
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
    $sql = "SELECT * FROM chado.cvtermprop WHERE " . implode(" AND " , $where_arr);
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
      array('cvterm_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id')),
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
        $fields['cvterm_id'] = $this->cvterm_id;
        $fields['type_id']   = $this->type_id;
        $fields['value']     = $this->value;
        $fields['rank']      = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->cvtermprop_id = db_insert('chado.cvtermprop')
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
      $fields['cvterm_id'] = $this->cvterm_id;
      $fields['type_id']   = $this->type_id;
      $fields['value']     = $this->value;
      $fields['rank']      = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.cvtermprop')
        ->fields($fields)
        ->condition('cvtermprop_id', $this->cvtermprop_id, '=')
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

      db_delete('chado.cvtermprop')
        ->condition('cvtermprop_id', $this->cvtermprop_id, '=')
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
   * Retrieves the cvtermprop_id.
   *
   * @retrun serial
   */
  public function getCvtermpropID() {
    return $this->cvtermprop_id;
  }

  /**
   * Updates the cvtermprop_id.
   *
   * @param serial $cvtermprop_id
   */
  public function setCvtermpropID($cvtermprop_id) {
    $this->cvtermprop_id = $cvtermprop_id;
  }

  /**
   * Retrieves the cvterm_id.
   *
   * @retrun integer
   */
  public function getCvtermID() {
    return $this->cvterm_id;
  }

  /**
   * Updates the cvterm_id.
   *
   * @param integer $cvterm_id
   */
  public function setCvtermID($cvterm_id) {
    $this->cvterm_id = $cvterm_id;
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