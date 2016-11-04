<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_VAR class.
 *
 */
class PUBLIC_MCL_VAR  {

 /**
  *  Data members for PUBLIC_MCL_VAR.
  */
  protected $member_arr  = NULL;
  protected $var_id      = NULL;
  protected $name        = NULL;
  protected $type        = NULL;
  protected $value       = NULL;
  protected $description = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'var_id'      => 'serial',
    'name'        => 'character_varying(255)',
    'type'        => 'character_varying(255)',
    'value'       => 'character_varying(1024)',
    'description' => 'text',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->var_id      = array_key_exists('var_id', $details)      ? $details['var_id']      : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->type        = array_key_exists('type', $details)        ? $details['type']        : '';
    $this->value       = array_key_exists('value', $details)       ? $details['value']       : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_VAR by key(s).
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
    $sql = "SELECT * FROM mcl_var WHERE " . implode(" AND " , $where_arr);
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
        $fields['name']        = $this->name;
        $fields['type']        = $this->type;
        $fields['value']       = $this->value;
        $fields['description'] = $this->description;


        // Inserts the record.
        $this->var_id = db_insert('mcl_var')
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
      $fields['name']        = $this->name;
      $fields['type']        = $this->type;
      $fields['value']       = $this->value;
      $fields['description'] = $this->description;


      // Updates the record.
      db_update('mcl_var')
        ->fields($fields)
        ->condition('var_id', $this->var_id, '=')
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

      db_delete('mcl_var')
        ->condition('var_id', $this->var_id, '=')
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
   * Retrieves the var_id.
   *
   * @retrun serial
   */
  public function getVarID() {
    return $this->var_id;
  }

  /**
   * Updates the var_id.
   *
   * @param serial $var_id
   */
  public function setVarID($var_id) {
    $this->var_id = $var_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(255)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Updates the name.
   *
   * @param character_varying(255) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the type.
   *
   * @retrun character_varying(255)
   */
  public function getType() {
    return $this->type;
  }

  /**
   * Updates the type.
   *
   * @param character_varying(255) $type
   */
  public function setType($type) {
    $this->type = $type;
  }

  /**
   * Retrieves the value.
   *
   * @retrun character_varying(1024)
   */
  public function getValue() {
    return $this->value;
  }

  /**
   * Updates the value.
   *
   * @param character_varying(1024) $value
   */
  public function setValue($value) {
    $this->value = $value;
  }

  /**
   * Retrieves the description.
   *
   * @retrun text
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Updates the description.
   *
   * @param text $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }
}