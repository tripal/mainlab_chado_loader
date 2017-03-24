<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_LIBRARY_PUB class.
 *
 */
class CHADO_LIBRARY_PUB  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_LIBRARY_PUB.
   */
  protected $member_arr     = NULL;
  protected $library_pub_id = NULL;
  protected $library_id     = NULL;
  protected $pub_id         = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'library_pub_id' => 'serial',
    'library_id'     => 'integer',
    'pub_id'         => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr     = $details;
    $this->library_pub_id = array_key_exists('library_pub_id', $details) ? $details['library_pub_id'] : '';
    $this->library_id     = array_key_exists('library_id', $details)     ? $details['library_id']     : '';
    $this->pub_id         = array_key_exists('pub_id', $details)         ? $details['pub_id']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_LIBRARY_PUB by key(s).
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
    $sql = "SELECT * FROM chado.library_pub WHERE " . implode(" AND " , $where_arr);
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
      array('library_id' => array('fk_table' => 'library', 'fk_attr' => 'library_id')),
      array('pub_id' => array('fk_table' => 'pub', 'fk_attr' => 'pub_id')),
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
        $fields['library_id'] = $this->library_id;
        $fields['pub_id']     = $this->pub_id;


        // Inserts the record.
        $this->library_pub_id = db_insert('chado.library_pub')
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
      $fields['library_id'] = $this->library_id;
      $fields['pub_id']     = $this->pub_id;


      // Updates the record.
      db_update('chado.library_pub')
        ->fields($fields)
        ->condition('library_pub_id', $this->library_pub_id, '=')
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

      db_delete('chado.library_pub')
        ->condition('library_pub_id', $this->library_pub_id, '=')
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
   * Retrieves the library_pub_id.
   *
   * @retrun serial
   */
  public function getLibraryPubID() {
    return $this->library_pub_id;
  }

  /**
   * Updates the library_pub_id.
   *
   * @param serial $library_pub_id
   */
  public function setLibraryPubID($library_pub_id) {
    $this->library_pub_id = $library_pub_id;
  }

  /**
   * Retrieves the library_id.
   *
   * @retrun integer
   */
  public function getLibraryID() {
    return $this->library_id;
  }

  /**
   * Updates the library_id.
   *
   * @param integer $library_id
   */
  public function setLibraryID($library_id) {
    $this->library_id = $library_id;
  }

  /**
   * Retrieves the pub_id.
   *
   * @retrun integer
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Updates the pub_id.
   *
   * @param integer $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
  }
}