<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_SYNONYM class.
 *
 */
class CHADO_SYNONYM  {

 /**
  *  Data members for CHADO_SYNONYM.
  */
  protected $member_arr   = NULL;
  protected $synonym_id   = NULL;
  protected $name         = NULL;
  protected $type_id      = NULL;
  protected $synonym_sgml = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'synonym_id'   => 'serial',
    'name'         => 'character_varying(255)',
    'type_id'      => 'integer',
    'synonym_sgml' => 'character_varying(255)',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->synonym_id   = array_key_exists('synonym_id', $details)   ? $details['synonym_id']   : '';
    $this->name         = array_key_exists('name', $details)         ? $details['name']         : '';
    $this->type_id      = array_key_exists('type_id', $details)      ? $details['type_id']      : '';
    $this->synonym_sgml = array_key_exists('synonym_sgml', $details) ? $details['synonym_sgml'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_SYNONYM by key(s).
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
    $sql = "SELECT * FROM chado.synonym WHERE " . implode(" AND " , $where_arr);
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
        $fields['name']         = $this->name;
        $fields['type_id']      = $this->type_id;
        $fields['synonym_sgml'] = $this->synonym_sgml;


        // Inserts the record.
        $this->synonym_id = db_insert('chado.synonym')
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
      $fields['name']         = $this->name;
      $fields['type_id']      = $this->type_id;
      $fields['synonym_sgml'] = $this->synonym_sgml;


      // Updates the record.
      db_update('chado.synonym')
        ->fields($fields)
        ->condition('synonym_id', $this->synonym_id, '=')
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

      db_delete('chado.synonym')
        ->condition('synonym_id', $this->synonym_id, '=')
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
   * Retrieves the synonym_id.
   *
   * @retrun serial
   */
  public function getSynonymID() {
    return $this->synonym_id;
  }

  /**
   * Updates the synonym_id.
   *
   * @param serial $synonym_id
   */
  public function setSynonymID($synonym_id) {
    $this->synonym_id = $synonym_id;
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
   * Retrieves the synonym_sgml.
   *
   * @retrun character_varying(255)
   */
  public function getSynonymSgml() {
    return $this->synonym_sgml;
  }

  /**
   * Updates the synonym_sgml.
   *
   * @param character_varying(255) $synonym_sgml
   */
  public function setSynonymSgml($synonym_sgml) {
    $this->synonym_sgml = $synonym_sgml;
  }
}