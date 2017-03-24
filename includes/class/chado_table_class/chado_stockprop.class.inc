<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STOCKPROP class.
 *
 */
class CHADO_STOCKPROP  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STOCKPROP.
   */
  protected $member_arr   = NULL;
  protected $stockprop_id = NULL;
  protected $stock_id     = NULL;
  protected $type_id      = NULL;
  protected $value        = NULL;
  protected $rank         = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'stockprop_id' => 'serial',
    'stock_id'     => 'integer',
    'type_id'      => 'integer',
    'value'        => 'text',
    'rank'         => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->stockprop_id = array_key_exists('stockprop_id', $details) ? $details['stockprop_id'] : '';
    $this->stock_id     = array_key_exists('stock_id', $details)     ? $details['stock_id']     : '';
    $this->type_id      = array_key_exists('type_id', $details)      ? $details['type_id']      : '';
    $this->value        = array_key_exists('value', $details)        ? $details['value']        : '';
    $this->rank         = array_key_exists('rank', $details)         ? $details['rank']         : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STOCKPROP by key(s).
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
    $sql = "SELECT * FROM chado.stockprop WHERE " . implode(" AND " , $where_arr);
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
      array('stock_id' => array('fk_table' => 'stock', 'fk_attr' => 'stock_id')),
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
        $fields['stock_id'] = $this->stock_id;
        $fields['type_id']  = $this->type_id;
        $fields['value']    = $this->value;
        $fields['rank']     = (is_numeric($this->rank)) ? $this->rank : 0;


        // Inserts the record.
        $this->stockprop_id = db_insert('chado.stockprop')
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
      $fields['stock_id'] = $this->stock_id;
      $fields['type_id']  = $this->type_id;
      $fields['value']    = $this->value;
      $fields['rank']     = (is_numeric($this->rank)) ? $this->rank : 0;


      // Updates the record.
      db_update('chado.stockprop')
        ->fields($fields)
        ->condition('stockprop_id', $this->stockprop_id, '=')
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

      db_delete('chado.stockprop')
        ->condition('stockprop_id', $this->stockprop_id, '=')
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
   * Retrieves the stockprop_id.
   *
   * @retrun serial
   */
  public function getStockpropID() {
    return $this->stockprop_id;
  }

  /**
   * Updates the stockprop_id.
   *
   * @param serial $stockprop_id
   */
  public function setStockpropID($stockprop_id) {
    $this->stockprop_id = $stockprop_id;
  }

  /**
   * Retrieves the stock_id.
   *
   * @retrun integer
   */
  public function getStockID() {
    return $this->stock_id;
  }

  /**
   * Updates the stock_id.
   *
   * @param integer $stock_id
   */
  public function setStockID($stock_id) {
    $this->stock_id = $stock_id;
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