<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURE_CVTERM class.
 *
 */
class CHADO_FEATURE_CVTERM  {

 /**
  *  Data members for CHADO_FEATURE_CVTERM.
  */
  protected $member_arr        = NULL;
  protected $feature_cvterm_id = NULL;
  protected $feature_id        = NULL;
  protected $cvterm_id         = NULL;
  protected $pub_id            = NULL;
  protected $is_not            = NULL;
  protected $rank              = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'feature_cvterm_id' => 'serial',
    'feature_id'        => 'integer',
    'cvterm_id'         => 'integer',
    'pub_id'            => 'integer',
    'is_not'            => 'boolean',
    'rank'              => 'integer',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr        = $details;
    $this->feature_cvterm_id = array_key_exists('feature_cvterm_id', $details) ? $details['feature_cvterm_id'] : '';
    $this->feature_id        = array_key_exists('feature_id', $details)        ? $details['feature_id']        : '';
    $this->cvterm_id         = array_key_exists('cvterm_id', $details)         ? $details['cvterm_id']         : '';
    $this->pub_id            = array_key_exists('pub_id', $details)            ? $details['pub_id']            : '';
    $this->is_not            = array_key_exists('is_not', $details)            ? $details['is_not']            : '';
    $this->rank              = array_key_exists('rank', $details)              ? $details['rank']              : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURE_CVTERM by key(s).
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
    $sql = "SELECT * FROM chado.feature_cvterm WHERE " . implode(" AND " , $where_arr);
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
        $fields['feature_id'] = $this->feature_id;
        $fields['cvterm_id']  = $this->cvterm_id;
        $fields['pub_id']     = $this->pub_id;
        $fields['is_not']     = (preg_match("/^(true|false)/i", $this->is_not)) ? "'" . $this->is_not . "'" : 'false';
        $fields['rank']       = (is_numeric($this->rank))                       ? $this->rank               : 0;


        // Inserts the record.
        $this->feature_cvterm_id = db_insert('chado.feature_cvterm')
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
      $fields['feature_id'] = $this->feature_id;
      $fields['cvterm_id']  = $this->cvterm_id;
      $fields['pub_id']     = $this->pub_id;
      $fields['is_not']     = (preg_match("/^(true|false)/i", $this->is_not)) ? "'" . $this->is_not . "'" : 'false';
      $fields['rank']       = (is_numeric($this->rank))                       ? $this->rank               : 0;


      // Updates the record.
      db_update('chado.feature_cvterm')
        ->fields($fields)
        ->condition('feature_cvterm_id', $this->feature_cvterm_id, '=')
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

      db_delete('chado.feature_cvterm')
        ->condition('feature_cvterm_id', $this->feature_cvterm_id, '=')
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
   * Retrieves the feature_cvterm_id.
   *
   * @retrun serial
   */
  public function getFeatureCvtermID() {
    return $this->feature_cvterm_id;
  }

  /**
   * Updates the feature_cvterm_id.
   *
   * @param serial $feature_cvterm_id
   */
  public function setFeatureCvtermID($feature_cvterm_id) {
    $this->feature_cvterm_id = $feature_cvterm_id;
  }

  /**
   * Retrieves the feature_id.
   *
   * @retrun integer
   */
  public function getFeatureID() {
    return $this->feature_id;
  }

  /**
   * Updates the feature_id.
   *
   * @param integer $feature_id
   */
  public function setFeatureID($feature_id) {
    $this->feature_id = $feature_id;
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

  /**
   * Retrieves the is_not.
   *
   * @retrun boolean
   */
  public function getIsNot() {
    return $this->is_not;
  }

  /**
   * Updates the is_not.
   *
   * @param boolean $is_not
   */
  public function setIsNot($is_not) {
    $this->is_not = $is_not;
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