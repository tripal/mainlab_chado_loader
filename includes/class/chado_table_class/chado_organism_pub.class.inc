<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ORGANISM_PUB class.
 *
 */
class CHADO_ORGANISM_PUB  {

 /**
  *  Data members for CHADO_ORGANISM_PUB.
  */
  protected $member_arr      = NULL;
  protected $organism_pub_id = NULL;
  protected $organism_id     = NULL;
  protected $pub_id          = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'organism_pub_id' => 'serial',
    'organism_id'     => 'integer',
    'pub_id'          => 'integer',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr      = $details;
    $this->organism_pub_id = array_key_exists('organism_pub_id', $details) ? $details['organism_pub_id'] : '';
    $this->organism_id     = array_key_exists('organism_id', $details)     ? $details['organism_id']     : '';
    $this->pub_id          = array_key_exists('pub_id', $details)          ? $details['pub_id']          : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ORGANISM_PUB by key(s).
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
    $sql = "SELECT * FROM chado.organism_pub WHERE " . implode(" AND " , $where_arr);
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
        $fields['organism_id'] = $this->organism_id;
        $fields['pub_id']      = $this->pub_id;


        // Inserts the record.
        $this->organism_pub_id = db_insert('chado.organism_pub')
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
      $fields['organism_id'] = $this->organism_id;
      $fields['pub_id']      = $this->pub_id;


      // Updates the record.
      db_update('chado.organism_pub')
        ->fields($fields)
        ->condition('organism_pub_id', $this->organism_pub_id, '=')
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

      db_delete('chado.organism_pub')
        ->condition('organism_pub_id', $this->organism_pub_id, '=')
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
   * Retrieves the organism_pub_id.
   *
   * @retrun serial
   */
  public function getOrganismPubID() {
    return $this->organism_pub_id;
  }

  /**
   * Updates the organism_pub_id.
   *
   * @param serial $organism_pub_id
   */
  public function setOrganismPubID($organism_pub_id) {
    $this->organism_pub_id = $organism_pub_id;
  }

  /**
   * Retrieves the organism_id.
   *
   * @retrun integer
   */
  public function getOrganismID() {
    return $this->organism_id;
  }

  /**
   * Updates the organism_id.
   *
   * @param integer $organism_id
   */
  public function setOrganismID($organism_id) {
    $this->organism_id = $organism_id;
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