<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ND_EXPERIMENT_PHENOTYPE class.
 *
 */
class CHADO_ND_EXPERIMENT_PHENOTYPE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ND_EXPERIMENT_PHENOTYPE.
   */
  protected $member_arr                 = NULL;
  protected $nd_experiment_phenotype_id = NULL;
  protected $nd_experiment_id           = NULL;
  protected $phenotype_id               = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'nd_experiment_phenotype_id' => 'serial',
    'nd_experiment_id'           => 'integer',
    'phenotype_id'               => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr                 = $details;
    $this->nd_experiment_phenotype_id = array_key_exists('nd_experiment_phenotype_id', $details) ? $details['nd_experiment_phenotype_id'] : '';
    $this->nd_experiment_id           = array_key_exists('nd_experiment_id', $details)           ? $details['nd_experiment_id']           : '';
    $this->phenotype_id               = array_key_exists('phenotype_id', $details)               ? $details['phenotype_id']               : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ND_EXPERIMENT_PHENOTYPE by key(s).
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
    $sql = "SELECT * FROM chado.nd_experiment_phenotype WHERE " . implode(" AND " , $where_arr);
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
      array('nd_experiment_id' => array('fk_table' => 'nd_experiment', 'fk_attr' => 'nd_experiment_id')),
      array('phenotype_id' => array('fk_table' => 'phenotype', 'fk_attr' => 'phenotype_id')),
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
        $fields['nd_experiment_id'] = $this->nd_experiment_id;
        $fields['phenotype_id']     = $this->phenotype_id;


        // Inserts the record.
        $this->nd_experiment_phenotype_id = db_insert('chado.nd_experiment_phenotype')
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
      $fields['nd_experiment_id'] = $this->nd_experiment_id;
      $fields['phenotype_id']     = $this->phenotype_id;


      // Updates the record.
      db_update('chado.nd_experiment_phenotype')
        ->fields($fields)
        ->condition('nd_experiment_phenotype_id', $this->nd_experiment_phenotype_id, '=')
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

      db_delete('chado.nd_experiment_phenotype')
        ->condition('nd_experiment_phenotype_id', $this->nd_experiment_phenotype_id, '=')
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
   * Retrieves the nd_experiment_phenotype_id.
   *
   * @retrun serial
   */
  public function getNdExperimentPhenotypeID() {
    return $this->nd_experiment_phenotype_id;
  }

  /**
   * Updates the nd_experiment_phenotype_id.
   *
   * @param serial $nd_experiment_phenotype_id
   */
  public function setNdExperimentPhenotypeID($nd_experiment_phenotype_id) {
    $this->nd_experiment_phenotype_id = $nd_experiment_phenotype_id;
  }

  /**
   * Retrieves the nd_experiment_id.
   *
   * @retrun integer
   */
  public function getNdExperimentID() {
    return $this->nd_experiment_id;
  }

  /**
   * Updates the nd_experiment_id.
   *
   * @param integer $nd_experiment_id
   */
  public function setNdExperimentID($nd_experiment_id) {
    $this->nd_experiment_id = $nd_experiment_id;
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
}