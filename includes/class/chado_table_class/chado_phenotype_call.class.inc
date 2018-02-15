<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PHENOTYPE_CALL class.
 *
 */
class CHADO_PHENOTYPE_CALL  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PHENOTYPE_CALL.
   */
  protected $member_arr        = NULL;
  protected $phenotype_call_id = NULL;
  protected $stock_id          = NULL;
  protected $cvterm_id         = NULL;
  protected $project_id        = NULL;
  protected $phenotype_id      = NULL;
  protected $contact_id        = NULL;
  protected $time              = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'phenotype_call_id' => 'serial',
    'stock_id'          => 'integer',
    'cvterm_id'         => 'integer',
    'project_id'        => 'integer',
    'phenotype_id'      => 'integer',
    'contact_id'        => 'integer',
    'time'              => 'timestamp_without_time_zone',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->phenotype_call_id = array_key_exists('phenotype_call_id', $details) ? $details['phenotype_call_id'] : '';
    $this->stock_id          = array_key_exists('stock_id', $details)          ? $details['stock_id']          : '';
    $this->cvterm_id         = array_key_exists('cvterm_id', $details)         ? $details['cvterm_id']         : '';
    $this->project_id        = array_key_exists('project_id', $details)        ? $details['project_id']        : '';
    $this->phenotype_id      = array_key_exists('phenotype_id', $details)      ? $details['phenotype_id']      : '';
    $this->contact_id        = array_key_exists('contact_id', $details)        ? $details['contact_id']        : '';
    $this->time              = array_key_exists('time', $details)              ? $details['time']              : NULL;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PHENOTYPE_CALL by key(s).
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
    $sql = "SELECT * FROM chado.phenotype_call WHERE " . implode(" AND " , $where_arr);
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
      array('contact_id' => array('fk_table' => 'contact', 'fk_attr' => 'contact_id')),
      array('cvterm_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id')),
      array('phenotype_id' => array('fk_table' => 'phenotype', 'fk_attr' => 'phenotype_id')),
      array('project_id' => array('fk_table' => 'project', 'fk_attr' => 'project_id')),
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
        $fields['stock_id']     = $this->stock_id;
        $fields['cvterm_id']    = $this->cvterm_id;
        $fields['project_id']   = $this->project_id;
        $fields['phenotype_id'] = $this->phenotype_id;
        $fields['time']         = $this->time;
        if (is_numeric($this->contact_id)) { $fields['contact_id'] = $this->contact_id; }

        // Inserts the record.
        $this->phenotype_call_id = db_insert('chado.phenotype_call')
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
      $fields['stock_id']     = $this->stock_id;
      $fields['cvterm_id']    = $this->cvterm_id;
      $fields['project_id']   = $this->project_id;
      $fields['phenotype_id'] = $this->phenotype_id;
      $fields['time']         = $this->time;
      if (is_numeric($this->contact_id)) { $fields['contact_id'] = $this->contact_id; }

      // Updates the record.
      db_update('chado.phenotype_call')
        ->fields($fields)
        ->condition('phenotype_call_id', $this->phenotype_call_id, '=')
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

      db_delete('chado.phenotype_call')
        ->condition('phenotype_call_id', $this->phenotype_call_id, '=')
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
   * Retrieves the phenotype_call_id.
   *
   * @retrun serial
   */
  public function getPhenotypeCallID() {
    return $this->phenotype_call_id;
  }

  /**
   * Updates the phenotype_call_id.
   *
   * @param serial $phenotype_call_id
   */
  public function setPhenotypeCallID($phenotype_call_id) {
    $this->phenotype_call_id = $phenotype_call_id;
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
   * Retrieves the project_id.
   *
   * @retrun integer
   */
  public function getProjectID() {
    return $this->project_id;
  }

  /**
   * Updates the project_id.
   *
   * @param integer $project_id
   */
  public function setProjectID($project_id) {
    $this->project_id = $project_id;
  }

  /**
   * Retrieves the phenotype_id.
   *
   * @retrun integer
   */
  public function getPhenotypeID() {
    return $this->phenotype_id;
  }

  /**
   * Updates the phenotype_id.
   *
   * @param integer $phenotype_id
   */
  public function setPhenotypeID($phenotype_id) {
    $this->phenotype_id = $phenotype_id;
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

  /**
   * Retrieves the time.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTime() {
    return $this->time;
  }

  /**
   * Updates the time.
   *
   * @param timestamp_without_time_zone $time
   */
  public function setTime($time) {
    $this->time = $time;
  }
}