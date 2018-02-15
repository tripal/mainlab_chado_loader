<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_STOCK_IMAGE class.
 *
 */
class CHADO_STOCK_IMAGE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_STOCK_IMAGE.
   */
  protected $member_arr     = NULL;
  protected $stock_image_id = NULL;
  protected $stock_id       = NULL;
  protected $eimage_id      = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'stock_image_id' => 'serial',
    'stock_id'       => 'integer',
    'eimage_id'      => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->stock_image_id = array_key_exists('stock_image_id', $details) ? $details['stock_image_id'] : '';
    $this->stock_id       = array_key_exists('stock_id', $details)       ? $details['stock_id']       : '';
    $this->eimage_id      = array_key_exists('eimage_id', $details)      ? $details['eimage_id']      : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_STOCK_IMAGE by key(s).
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
    $sql = "SELECT * FROM chado.stock_image WHERE " . implode(" AND " , $where_arr);
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
      array('stock_id' => array('fk_table' => 'stock', 'fk_attr' => 'stock_id')),
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
        $fields['stock_id']  = $this->stock_id;
        $fields['eimage_id'] = $this->eimage_id;


        // Inserts the record.
        $this->stock_image_id = db_insert('chado.stock_image')
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
      $fields['stock_id']  = $this->stock_id;
      $fields['eimage_id'] = $this->eimage_id;


      // Updates the record.
      db_update('chado.stock_image')
        ->fields($fields)
        ->condition('stock_image_id', $this->stock_image_id, '=')
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

      db_delete('chado.stock_image')
        ->condition('stock_image_id', $this->stock_image_id, '=')
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
   * Retrieves the stock_image_id.
   *
   * @retrun serial
   */
  public function getStockImageID() {
    return $this->stock_image_id;
  }

  /**
   * Updates the stock_image_id.
   *
   * @param serial $stock_image_id
   */
  public function setStockImageID($stock_image_id) {
    $this->stock_image_id = $stock_image_id;
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
}