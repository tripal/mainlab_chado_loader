<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PROJECT class.
 *
 */
class CHADO_PROJECT  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_PROJECT.
   */
  protected $member_arr  = NULL;
  protected $project_id  = NULL;
  protected $name        = NULL;
  protected $description = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'project_id'  => 'serial',
    'name'        => 'character_varying(255)',
    'description' => 'character_varying(255)',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->project_id  = array_key_exists('project_id', $details)  ? $details['project_id']  : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PROJECT by key(s).
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
    $sql = "SELECT * FROM chado.project WHERE " . implode(" AND " , $where_arr);
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
    return NULL;
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'project_id' => array(
        array('table' => 'feature_project', 'attr' => 'project_id'),
        array('table' => 'genotype_call', 'attr' => 'project_id'),
        array('table' => 'nd_experiment_project', 'attr' => 'project_id'),
        array('table' => 'project_contact', 'attr' => 'project_id'),
        array('table' => 'project_pub', 'attr' => 'project_id'),
        array('table' => 'project_relationship', 'attr' => 'object_project_id'),
        array('table' => 'project_relationship', 'attr' => 'subject_project_id'),
        array('table' => 'projectprop', 'attr' => 'project_id'),
      ),
    );
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
        $fields['description'] = $this->description;


        // Inserts the record.
        $this->project_id = db_insert('chado.project')
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
      $fields['description'] = $this->description;


      // Updates the record.
      db_update('chado.project')
        ->fields($fields)
        ->condition('project_id', $this->project_id, '=')
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

      db_delete('chado.project')
        ->condition('project_id', $this->project_id, '=')
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
   * Retrieves the project_id.
   *
   * @retrun serial
   */
  public function getProjectID() {
    return $this->project_id;
  }

  /**
   * Updates the project_id.
   *
   * @param serial $project_id
   */
  public function setProjectID($project_id) {
    $this->project_id = $project_id;
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
   * Retrieves the description.
   *
   * @retrun character_varying(255)
   */
  public function getDescription() {
    return $this->description;
  }

  /**
   * Updates the description.
   *
   * @param character_varying(255) $description
   */
  public function setDescription($description) {
    $this->description = $description;
  }
}