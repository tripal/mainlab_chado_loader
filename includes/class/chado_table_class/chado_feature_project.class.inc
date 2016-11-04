<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURE_PROJECT class.
 *
 */
class CHADO_FEATURE_PROJECT  {

 /**
  *  Data members for CHADO_FEATURE_PROJECT.
  */
  protected $member_arr         = NULL;
  protected $feature_project_id = NULL;
  protected $feature_id         = NULL;
  protected $project_id         = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'feature_project_id' => 'serial',
    'feature_id'         => 'integer',
    'project_id'         => 'integer',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr         = $details;
    $this->feature_project_id = array_key_exists('feature_project_id', $details) ? $details['feature_project_id'] : '';
    $this->feature_id         = array_key_exists('feature_id', $details)         ? $details['feature_id']         : '';
    $this->project_id         = array_key_exists('project_id', $details)         ? $details['project_id']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURE_PROJECT by key(s).
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
    $sql = "SELECT * FROM chado.feature_project WHERE " . implode(" AND " , $where_arr);
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
        $fields['project_id'] = $this->project_id;


        // Inserts the record.
        $this->feature_project_id = db_insert('chado.feature_project')
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
      $fields['project_id'] = $this->project_id;


      // Updates the record.
      db_update('chado.feature_project')
        ->fields($fields)
        ->condition('feature_project_id', $this->feature_project_id, '=')
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

      db_delete('chado.feature_project')
        ->condition('feature_project_id', $this->feature_project_id, '=')
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
   * Retrieves the feature_project_id.
   *
   * @retrun serial
   */
  public function getFeatureProjectID() {
    return $this->feature_project_id;
  }

  /**
   * Updates the feature_project_id.
   *
   * @param serial $feature_project_id
   */
  public function setFeatureProjectID($feature_project_id) {
    $this->feature_project_id = $feature_project_id;
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
}