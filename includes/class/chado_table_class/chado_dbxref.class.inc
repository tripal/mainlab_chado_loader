<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_DBXREF class.
 *
 */
class CHADO_DBXREF  {

  /**
   *  Data members for CHADO_DBXREF.
   */
  protected $member_arr  = NULL;
  protected $dbxref_id   = NULL;
  protected $db_id       = NULL;
  protected $accession   = NULL;
  protected $version     = NULL;
  protected $description = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'dbxref_id'   => 'serial',
    'db_id'       => 'integer',
    'accession'   => 'character_varying(255)',
    'version'     => 'character_varying(255)',
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
    $this->dbxref_id   = array_key_exists('dbxref_id', $details)   ? $details['dbxref_id']   : '';
    $this->db_id       = array_key_exists('db_id', $details)       ? $details['db_id']       : '';
    $this->accession   = array_key_exists('accession', $details)   ? $details['accession']   : '';
    $this->version     = array_key_exists('version', $details)     ? $details['version']     : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_DBXREF by key(s).
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
    $sql = "SELECT * FROM chado.dbxref WHERE " . implode(" AND " , $where_arr);
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
        $fields['db_id']       = $this->db_id;
        $fields['accession']   = $this->accession;
        $fields['version']     = $this->version;
        $fields['description'] = $this->description;


        // Inserts the record.
        $this->dbxref_id = db_insert('chado.dbxref')
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
      $fields['db_id']       = $this->db_id;
      $fields['accession']   = $this->accession;
      $fields['version']     = $this->version;
      $fields['description'] = $this->description;


      // Updates the record.
      db_update('chado.dbxref')
        ->fields($fields)
        ->condition('dbxref_id', $this->dbxref_id, '=')
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

      db_delete('chado.dbxref')
        ->condition('dbxref_id', $this->dbxref_id, '=')
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
   * Retrieves the dbxref_id.
   *
   * @retrun serial
   */
  public function getDbxrefID() {
    return $this->dbxref_id;
  }

  /**
   * Updates the dbxref_id.
   *
   * @param serial $dbxref_id
   */
  public function setDbxrefID($dbxref_id) {
    $this->dbxref_id = $dbxref_id;
  }

  /**
   * Retrieves the db_id.
   *
   * @retrun integer
   */
  public function getDbID() {
    return $this->db_id;
  }

  /**
   * Updates the db_id.
   *
   * @param integer $db_id
   */
  public function setDbID($db_id) {
    $this->db_id = $db_id;
  }

  /**
   * Retrieves the accession.
   *
   * @retrun character_varying(255)
   */
  public function getAccession() {
    return $this->accession;
  }

  /**
   * Updates the accession.
   *
   * @param character_varying(255) $accession
   */
  public function setAccession($accession) {
    $this->accession = $accession;
  }

  /**
   * Retrieves the version.
   *
   * @retrun character_varying(255)
   */
  public function getVersion() {
    return $this->version;
  }

  /**
   * Updates the version.
   *
   * @param character_varying(255) $version
   */
  public function setVersion($version) {
    $this->version = $version;
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