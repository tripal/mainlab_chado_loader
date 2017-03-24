<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURELOC class.
 *
 */
class CHADO_FEATURELOC  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATURELOC.
   */
  protected $member_arr      = NULL;
  protected $featureloc_id   = NULL;
  protected $feature_id      = NULL;
  protected $srcfeature_id   = NULL;
  protected $fmin            = NULL;
  protected $is_fmin_partial = NULL;
  protected $fmax            = NULL;
  protected $is_fmax_partial = NULL;
  protected $strand          = NULL;
  protected $phase           = NULL;
  protected $residue_info    = NULL;
  protected $locgroup        = NULL;
  protected $rank            = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'featureloc_id'   => 'serial',
    'feature_id'      => 'integer',
    'srcfeature_id'   => 'integer',
    'fmin'            => 'integer',
    'is_fmin_partial' => 'boolean',
    'fmax'            => 'integer',
    'is_fmax_partial' => 'boolean',
    'strand'          => 'smallint',
    'phase'           => 'integer',
    'residue_info'    => 'text',
    'locgroup'        => 'integer',
    'rank'            => 'integer',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr      = $details;
    $this->featureloc_id   = array_key_exists('featureloc_id', $details)   ? $details['featureloc_id']   : '';
    $this->feature_id      = array_key_exists('feature_id', $details)      ? $details['feature_id']      : '';
    $this->srcfeature_id   = array_key_exists('srcfeature_id', $details)   ? $details['srcfeature_id']   : '';
    $this->fmin            = array_key_exists('fmin', $details)            ? $details['fmin']            : '';
    $this->is_fmin_partial = array_key_exists('is_fmin_partial', $details) ? $details['is_fmin_partial'] : '';
    $this->fmax            = array_key_exists('fmax', $details)            ? $details['fmax']            : '';
    $this->is_fmax_partial = array_key_exists('is_fmax_partial', $details) ? $details['is_fmax_partial'] : '';
    $this->strand          = array_key_exists('strand', $details)          ? $details['strand']          : '';
    $this->phase           = array_key_exists('phase', $details)           ? $details['phase']           : '';
    $this->residue_info    = array_key_exists('residue_info', $details)    ? $details['residue_info']    : '';
    $this->locgroup        = array_key_exists('locgroup', $details)        ? $details['locgroup']        : 0;
    $this->rank            = array_key_exists('rank', $details)            ? $details['rank']            : 0;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURELOC by key(s).
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
    $sql = "SELECT * FROM chado.featureloc WHERE " . implode(" AND " , $where_arr);
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
      array('feature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id')),
      array('srcfeature_id' => array('fk_table' => 'feature', 'fk_attr' => 'feature_id')),
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
        $fields['feature_id']      = $this->feature_id;
        $fields['is_fmin_partial'] = (preg_match("/^(true|false)/i", $this->is_fmin_partial)) ? "'" . $this->is_fmin_partial . "'" : 'false';
        $fields['is_fmax_partial'] = (preg_match("/^(true|false)/i", $this->is_fmax_partial)) ? "'" . $this->is_fmax_partial . "'" : 'false';
        $fields['residue_info']    = $this->residue_info;
        $fields['locgroup']        = (is_numeric($this->locgroup))                            ? $this->locgroup                    : 0;
        $fields['rank']            = (is_numeric($this->rank))                                ? $this->rank                        : 0;
        if (is_numeric($this->srcfeature_id)) { $fields['srcfeature_id'] = $this->srcfeature_id; }
        if (is_numeric($this->fmin))          { $fields['fmin']          = $this->fmin; }
        if (is_numeric($this->fmax))          { $fields['fmax']          = $this->fmax; }
        if (is_numeric($this->strand))        { $fields['strand']        = $this->strand; }
        if (is_numeric($this->phase))         { $fields['phase']         = $this->phase; }

        // Inserts the record.
        $this->featureloc_id = db_insert('chado.featureloc')
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
      $fields['feature_id']      = $this->feature_id;
      $fields['is_fmin_partial'] = (preg_match("/^(true|false)/i", $this->is_fmin_partial)) ? "'" . $this->is_fmin_partial . "'" : 'false';
      $fields['is_fmax_partial'] = (preg_match("/^(true|false)/i", $this->is_fmax_partial)) ? "'" . $this->is_fmax_partial . "'" : 'false';
      $fields['residue_info']    = $this->residue_info;
      $fields['locgroup']        = (is_numeric($this->locgroup))                            ? $this->locgroup                    : 0;
      $fields['rank']            = (is_numeric($this->rank))                                ? $this->rank                        : 0;
      if (is_numeric($this->srcfeature_id)) { $fields['srcfeature_id'] = $this->srcfeature_id; }
      if (is_numeric($this->fmin))          { $fields['fmin']          = $this->fmin; }
      if (is_numeric($this->fmax))          { $fields['fmax']          = $this->fmax; }
      if (is_numeric($this->strand))        { $fields['strand']        = $this->strand; }
      if (is_numeric($this->phase))         { $fields['phase']         = $this->phase; }

      // Updates the record.
      db_update('chado.featureloc')
        ->fields($fields)
        ->condition('featureloc_id', $this->featureloc_id, '=')
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

      db_delete('chado.featureloc')
        ->condition('featureloc_id', $this->featureloc_id, '=')
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
   * Retrieves the featureloc_id.
   *
   * @retrun serial
   */
  public function getFeaturelocID() {
    return $this->featureloc_id;
  }

  /**
   * Updates the featureloc_id.
   *
   * @param serial $featureloc_id
   */
  public function setFeaturelocID($featureloc_id) {
    $this->featureloc_id = $featureloc_id;
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
   * Retrieves the srcfeature_id.
   *
   * @retrun integer
   */
  public function getSrcfeatureID() {
    return $this->srcfeature_id;
  }

  /**
   * Updates the srcfeature_id.
   *
   * @param integer $srcfeature_id
   */
  public function setSrcfeatureID($srcfeature_id) {
    $this->srcfeature_id = $srcfeature_id;
  }

  /**
   * Retrieves the fmin.
   *
   * @retrun integer
   */
  public function getFmin() {
    return $this->fmin;
  }

  /**
   * Updates the fmin.
   *
   * @param integer $fmin
   */
  public function setFmin($fmin) {
    $this->fmin = $fmin;
  }

  /**
   * Retrieves the is_fmin_partial.
   *
   * @retrun boolean
   */
  public function getIsFminPartial() {
    return $this->is_fmin_partial;
  }

  /**
   * Updates the is_fmin_partial.
   *
   * @param boolean $is_fmin_partial
   */
  public function setIsFminPartial($is_fmin_partial) {
    $this->is_fmin_partial = $is_fmin_partial;
  }

  /**
   * Retrieves the fmax.
   *
   * @retrun integer
   */
  public function getFmax() {
    return $this->fmax;
  }

  /**
   * Updates the fmax.
   *
   * @param integer $fmax
   */
  public function setFmax($fmax) {
    $this->fmax = $fmax;
  }

  /**
   * Retrieves the is_fmax_partial.
   *
   * @retrun boolean
   */
  public function getIsFmaxPartial() {
    return $this->is_fmax_partial;
  }

  /**
   * Updates the is_fmax_partial.
   *
   * @param boolean $is_fmax_partial
   */
  public function setIsFmaxPartial($is_fmax_partial) {
    $this->is_fmax_partial = $is_fmax_partial;
  }

  /**
   * Retrieves the strand.
   *
   * @retrun smallint
   */
  public function getStrand() {
    return $this->strand;
  }

  /**
   * Updates the strand.
   *
   * @param smallint $strand
   */
  public function setStrand($strand) {
    $this->strand = $strand;
  }

  /**
   * Retrieves the phase.
   *
   * @retrun integer
   */
  public function getPhase() {
    return $this->phase;
  }

  /**
   * Updates the phase.
   *
   * @param integer $phase
   */
  public function setPhase($phase) {
    $this->phase = $phase;
  }

  /**
   * Retrieves the residue_info.
   *
   * @retrun text
   */
  public function getResidueInfo() {
    return $this->residue_info;
  }

  /**
   * Updates the residue_info.
   *
   * @param text $residue_info
   */
  public function setResidueInfo($residue_info) {
    $this->residue_info = $residue_info;
  }

  /**
   * Retrieves the locgroup.
   *
   * @retrun integer
   */
  public function getLocgroup() {
    return $this->locgroup;
  }

  /**
   * Updates the locgroup.
   *
   * @param integer $locgroup
   */
  public function setLocgroup($locgroup) {
    $this->locgroup = $locgroup;
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