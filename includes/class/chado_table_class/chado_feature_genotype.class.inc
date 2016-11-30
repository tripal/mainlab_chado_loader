<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURE_GENOTYPE class.
 *
 */
class CHADO_FEATURE_GENOTYPE  {

  /**
   *  Data members for CHADO_FEATURE_GENOTYPE.
   */
  protected $member_arr          = NULL;
  protected $feature_genotype_id = NULL;
  protected $feature_id          = NULL;
  protected $genotype_id         = NULL;
  protected $chromosome_id       = NULL;
  protected $rank                = NULL;
  protected $cgroup              = NULL;
  protected $cvterm_id           = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'feature_genotype_id' => 'serial',
    'feature_id'          => 'integer',
    'genotype_id'         => 'integer',
    'chromosome_id'       => 'integer',
    'rank'                => 'integer',
    'cgroup'              => 'integer',
    'cvterm_id'           => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr          = $details;
    $this->feature_genotype_id = array_key_exists('feature_genotype_id', $details) ? $details['feature_genotype_id'] : '';
    $this->feature_id          = array_key_exists('feature_id', $details)          ? $details['feature_id']          : '';
    $this->genotype_id         = array_key_exists('genotype_id', $details)         ? $details['genotype_id']         : '';
    $this->chromosome_id       = array_key_exists('chromosome_id', $details)       ? $details['chromosome_id']       : '';
    $this->rank                = array_key_exists('rank', $details)                ? $details['rank']                : '';
    $this->cgroup              = array_key_exists('cgroup', $details)              ? $details['cgroup']              : '';
    $this->cvterm_id           = array_key_exists('cvterm_id', $details)           ? $details['cvterm_id']           : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURE_GENOTYPE by key(s).
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
    $sql = "SELECT * FROM chado.feature_genotype WHERE " . implode(" AND " , $where_arr);
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
        $fields['feature_id']  = $this->feature_id;
        $fields['genotype_id'] = $this->genotype_id;
        $fields['rank']        = $this->rank;
        $fields['cgroup']      = $this->cgroup;
        $fields['cvterm_id']   = $this->cvterm_id;
        if (is_numeric($this->chromosome_id)) { $fields['chromosome_id'] = $this->chromosome_id; }

        // Inserts the record.
        $this->feature_genotype_id = db_insert('chado.feature_genotype')
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
      $fields['feature_id']  = $this->feature_id;
      $fields['genotype_id'] = $this->genotype_id;
      $fields['rank']        = $this->rank;
      $fields['cgroup']      = $this->cgroup;
      $fields['cvterm_id']   = $this->cvterm_id;
      if (is_numeric($this->chromosome_id)) { $fields['chromosome_id'] = $this->chromosome_id; }

      // Updates the record.
      db_update('chado.feature_genotype')
        ->fields($fields)
        ->condition('feature_genotype_id', $this->feature_genotype_id, '=')
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

      db_delete('chado.feature_genotype')
        ->condition('feature_genotype_id', $this->feature_genotype_id, '=')
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
   * Retrieves the feature_genotype_id.
   *
   * @retrun serial
   */
  public function getFeatureGenotypeID() {
    return $this->feature_genotype_id;
  }

  /**
   * Updates the feature_genotype_id.
   *
   * @param serial $feature_genotype_id
   */
  public function setFeatureGenotypeID($feature_genotype_id) {
    $this->feature_genotype_id = $feature_genotype_id;
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
   * Retrieves the genotype_id.
   *
   * @retrun integer
   */
  public function getGenotypeID() {
    return $this->genotype_id;
  }

  /**
   * Updates the genotype_id.
   *
   * @param integer $genotype_id
   */
  public function setGenotypeID($genotype_id) {
    $this->genotype_id = $genotype_id;
  }

  /**
   * Retrieves the chromosome_id.
   *
   * @retrun integer
   */
  public function getChromosomeID() {
    return $this->chromosome_id;
  }

  /**
   * Updates the chromosome_id.
   *
   * @param integer $chromosome_id
   */
  public function setChromosomeID($chromosome_id) {
    $this->chromosome_id = $chromosome_id;
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

  /**
   * Retrieves the cgroup.
   *
   * @retrun integer
   */
  public function getCgroup() {
    return $this->cgroup;
  }

  /**
   * Updates the cgroup.
   *
   * @param integer $cgroup
   */
  public function setCgroup($cgroup) {
    $this->cgroup = $cgroup;
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
}