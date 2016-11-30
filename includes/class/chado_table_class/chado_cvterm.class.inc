<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_CVTERM class.
 *
 */
class CHADO_CVTERM  {

  /**
   *  Data members for CHADO_CVTERM.
   */
  protected $member_arr          = NULL;
  protected $cvterm_id           = NULL;
  protected $cv_id               = NULL;
  protected $name                = NULL;
  protected $definition          = NULL;
  protected $dbxref_id           = NULL;
  protected $is_obsolete         = NULL;
  protected $is_relationshiptype = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'cvterm_id'           => 'serial',
    'cv_id'               => 'integer',
    'name'                => 'character_varying(1024)',
    'definition'          => 'text',
    'dbxref_id'           => 'integer',
    'is_obsolete'         => 'integer',
    'is_relationshiptype' => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->cvterm_id           = array_key_exists('cvterm_id', $details)           ? $details['cvterm_id']           : '';
    $this->cv_id               = array_key_exists('cv_id', $details)               ? $details['cv_id']               : '';
    $this->name                = array_key_exists('name', $details)                ? $details['name']                : '';
    $this->definition          = array_key_exists('definition', $details)          ? $details['definition']          : '';
    $this->dbxref_id           = array_key_exists('dbxref_id', $details)           ? $details['dbxref_id']           : '';
    $this->is_obsolete         = array_key_exists('is_obsolete', $details)         ? $details['is_obsolete']         : 0;
    $this->is_relationshiptype = array_key_exists('is_relationshiptype', $details) ? $details['is_relationshiptype'] : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_CVTERM by key(s).
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
    $sql = "SELECT * FROM chado.cvterm WHERE " . implode(" AND " , $where_arr);
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
        $fields['cv_id']               = $this->cv_id;
        $fields['name']                = $this->name;
        $fields['definition']          = $this->definition;
        $fields['dbxref_id']           = $this->dbxref_id;
        $fields['is_obsolete']         = (is_numeric($this->is_obsolete))         ? $this->is_obsolete         : 0;
        $fields['is_relationshiptype'] = (is_numeric($this->is_relationshiptype)) ? $this->is_relationshiptype : 0;


        // Inserts the record.
        $this->cvterm_id = db_insert('chado.cvterm')
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
      $fields['cv_id']               = $this->cv_id;
      $fields['name']                = $this->name;
      $fields['definition']          = $this->definition;
      $fields['dbxref_id']           = $this->dbxref_id;
      $fields['is_obsolete']         = (is_numeric($this->is_obsolete))         ? $this->is_obsolete         : 0;
      $fields['is_relationshiptype'] = (is_numeric($this->is_relationshiptype)) ? $this->is_relationshiptype : 0;


      // Updates the record.
      db_update('chado.cvterm')
        ->fields($fields)
        ->condition('cvterm_id', $this->cvterm_id, '=')
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

      db_delete('chado.cvterm')
        ->condition('cvterm_id', $this->cvterm_id, '=')
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
   * Retrieves the cvterm_id.
   *
   * @retrun serial
   */
  public function getCvtermID() {
    return $this->cvterm_id;
  }

  /**
   * Updates the cvterm_id.
   *
   * @param serial $cvterm_id
   */
  public function setCvtermID($cvterm_id) {
    $this->cvterm_id = $cvterm_id;
  }

  /**
   * Retrieves the cv_id.
   *
   * @retrun integer
   */
  public function getCvID() {
    return $this->cv_id;
  }

  /**
   * Updates the cv_id.
   *
   * @param integer $cv_id
   */
  public function setCvID($cv_id) {
    $this->cv_id = $cv_id;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(1024)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Updates the name.
   *
   * @param character_varying(1024) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the definition.
   *
   * @retrun text
   */
  public function getDefinition() {
    return $this->definition;
  }

  /**
   * Updates the definition.
   *
   * @param text $definition
   */
  public function setDefinition($definition) {
    $this->definition = $definition;
  }

  /**
   * Retrieves the dbxref_id.
   *
   * @retrun integer
   */
  public function getDbxrefID() {
    return $this->dbxref_id;
  }

  /**
   * Updates the dbxref_id.
   *
   * @param integer $dbxref_id
   */
  public function setDbxrefID($dbxref_id) {
    $this->dbxref_id = $dbxref_id;
  }

  /**
   * Retrieves the is_obsolete.
   *
   * @retrun integer
   */
  public function getIsObsolete() {
    return $this->is_obsolete;
  }

  /**
   * Updates the is_obsolete.
   *
   * @param integer $is_obsolete
   */
  public function setIsObsolete($is_obsolete) {
    $this->is_obsolete = $is_obsolete;
  }

  /**
   * Retrieves the is_relationshiptype.
   *
   * @retrun integer
   */
  public function getIsRelationshiptype() {
    return $this->is_relationshiptype;
  }

  /**
   * Updates the is_relationshiptype.
   *
   * @param integer $is_relationshiptype
   */
  public function setIsRelationshiptype($is_relationshiptype) {
    $this->is_relationshiptype = $is_relationshiptype;
  }
}