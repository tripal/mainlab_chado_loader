<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_FEATURE class.
 *
 */
class CHADO_FEATURE  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_FEATURE.
   */
  protected $member_arr       = NULL;
  protected $feature_id       = NULL;
  protected $dbxref_id        = NULL;
  protected $organism_id      = NULL;
  protected $name             = NULL;
  protected $uniquename       = NULL;
  protected $residues         = NULL;
  protected $seqlen           = NULL;
  protected $md5checksum      = NULL;
  protected $type_id          = NULL;
  protected $is_analysis      = NULL;
  protected $is_obsolete      = NULL;
  protected $timeaccessioned  = NULL;
  protected $timelastmodified = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'feature_id'       => 'serial',
    'dbxref_id'        => 'integer',
    'organism_id'      => 'integer',
    'name'             => 'character_varying(255)',
    'uniquename'       => 'text',
    'residues'         => 'text',
    'seqlen'           => 'integer',
    'md5checksum'      => 'character(32)',
    'type_id'          => 'integer',
    'is_analysis'      => 'boolean',
    'is_obsolete'      => 'boolean',
    'timeaccessioned'  => 'timestamp_without_time_zone',
    'timelastmodified' => 'timestamp_without_time_zone',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->feature_id       = array_key_exists('feature_id', $details)       ? $details['feature_id']       : '';
    $this->dbxref_id        = array_key_exists('dbxref_id', $details)        ? $details['dbxref_id']        : '';
    $this->organism_id      = array_key_exists('organism_id', $details)      ? $details['organism_id']      : '';
    $this->name             = array_key_exists('name', $details)             ? $details['name']             : '';
    $this->uniquename       = array_key_exists('uniquename', $details)       ? $details['uniquename']       : '';
    $this->residues         = array_key_exists('residues', $details)         ? $details['residues']         : '';
    $this->seqlen           = array_key_exists('seqlen', $details)           ? $details['seqlen']           : '';
    $this->md5checksum      = array_key_exists('md5checksum', $details)      ? $details['md5checksum']      : '';
    $this->type_id          = array_key_exists('type_id', $details)          ? $details['type_id']          : '';
    $this->is_analysis      = array_key_exists('is_analysis', $details)      ? $details['is_analysis']      : '';
    $this->is_obsolete      = array_key_exists('is_obsolete', $details)      ? $details['is_obsolete']      : '';
    $this->timeaccessioned  = array_key_exists('timeaccessioned', $details)  ? $details['timeaccessioned']  : '';
    $this->timelastmodified = array_key_exists('timelastmodified', $details) ? $details['timelastmodified'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_FEATURE by key(s).
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
    $sql = "SELECT * FROM chado.feature WHERE " . implode(" AND " , $where_arr);
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
      array('dbxref_id' => array('fk_table' => 'dbxref', 'fk_attr' => 'dbxref_id')),
      array('organism_id' => array('fk_table' => 'organism', 'fk_attr' => 'organism_id')),
      array('type_id' => array('fk_table' => 'cvterm', 'fk_attr' => 'cvterm_id')),
    );
  }

  /**
   * Returns the related tables.
   */
  public static function getRelTable() {
    return array(
      'feature_id' => array(
        array('table' => 'featureloc', 'attr' => 'feature_id'),
        array('table' => 'featureloc', 'attr' => 'srcfeature_id'),
        array('table' => 'feature_cvterm', 'attr' => 'feature_id'),
        array('table' => 'feature_dbxref', 'attr' => 'feature_id'),
        array('table' => 'feature_pub', 'attr' => 'feature_id'),
        array('table' => 'feature_synonym', 'attr' => 'feature_id'),
        array('table' => 'featureprop', 'attr' => 'feature_id'),
        array('table' => 'feature_contact', 'attr' => 'feature_id'),
        array('table' => 'feature_genotype', 'attr' => 'chromosome_id'),
        array('table' => 'feature_genotype', 'attr' => 'feature_id'),
        array('table' => 'feature_image', 'attr' => 'feature_id'),
        array('table' => 'feature_project', 'attr' => 'feature_id'),
        array('table' => 'feature_relationship', 'attr' => 'object_id'),
        array('table' => 'feature_relationship', 'attr' => 'subject_id'),
        array('table' => 'feature_stock', 'attr' => 'feature_id'),
        array('table' => 'featurepos', 'attr' => 'feature_id'),
        array('table' => 'featurepos', 'attr' => 'map_feature_id'),
        array('table' => 'genotype_call', 'attr' => 'feature_id'),
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
        $fields['organism_id']      = $this->organism_id;
        $fields['name']             = $this->name;
        $fields['uniquename']       = $this->uniquename;
        $fields['residues']         = $this->residues;
        $fields['md5checksum']      = $this->md5checksum;
        $fields['type_id']          = $this->type_id;
        $fields['is_analysis']      = (preg_match("/^(true|false)/i", $this->is_analysis)) ? "'" . $this->is_analysis . "'" : 'false';
        $fields['is_obsolete']      = (preg_match("/^(true|false)/i", $this->is_obsolete)) ? "'" . $this->is_obsolete . "'" : 'false';
        $fields['timeaccessioned']  = (!empty($this->timeaccessioned))                     ? $this->timeaccessioned         : 'now()';
        $fields['timelastmodified'] = (!empty($this->timelastmodified))                    ? $this->timelastmodified        : 'now()';
        if (is_numeric($this->dbxref_id)) { $fields['dbxref_id'] = $this->dbxref_id; }
        if (is_numeric($this->seqlen))    { $fields['seqlen']    = $this->seqlen; }

        // Inserts the record.
        $this->feature_id = db_insert('chado.feature')
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
      $fields['organism_id']      = $this->organism_id;
      $fields['name']             = $this->name;
      $fields['uniquename']       = $this->uniquename;
      $fields['residues']         = $this->residues;
      $fields['md5checksum']      = $this->md5checksum;
      $fields['type_id']          = $this->type_id;
      $fields['is_analysis']      = (preg_match("/^(true|false)/i", $this->is_analysis)) ? "'" . $this->is_analysis . "'" : 'false';
      $fields['is_obsolete']      = (preg_match("/^(true|false)/i", $this->is_obsolete)) ? "'" . $this->is_obsolete . "'" : 'false';
      $fields['timeaccessioned']  = (!empty($this->timeaccessioned))                     ? $this->timeaccessioned         : 'now()';
      $fields['timelastmodified'] = (!empty($this->timelastmodified))                    ? $this->timelastmodified        : 'now()';
      if (is_numeric($this->dbxref_id)) { $fields['dbxref_id'] = $this->dbxref_id; }
      if (is_numeric($this->seqlen))    { $fields['seqlen']    = $this->seqlen; }

      // Updates the record.
      db_update('chado.feature')
        ->fields($fields)
        ->condition('feature_id', $this->feature_id, '=')
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

      db_delete('chado.feature')
        ->condition('feature_id', $this->feature_id, '=')
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
   * Retrieves the feature_id.
   *
   * @retrun serial
   */
  public function getFeatureID() {
    return $this->feature_id;
  }

  /**
   * Updates the feature_id.
   *
   * @param serial $feature_id
   */
  public function setFeatureID($feature_id) {
    $this->feature_id = $feature_id;
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
   * Retrieves the uniquename.
   *
   * @retrun text
   */
  public function getUniquename() {
    return $this->uniquename;
  }

  /**
   * Updates the uniquename.
   *
   * @param text $uniquename
   */
  public function setUniquename($uniquename) {
    $this->uniquename = $uniquename;
  }

  /**
   * Retrieves the residues.
   *
   * @retrun text
   */
  public function getResidues() {
    return $this->residues;
  }

  /**
   * Updates the residues.
   *
   * @param text $residues
   */
  public function setResidues($residues) {
    $this->residues = $residues;
  }

  /**
   * Retrieves the seqlen.
   *
   * @retrun integer
   */
  public function getSeqlen() {
    return $this->seqlen;
  }

  /**
   * Updates the seqlen.
   *
   * @param integer $seqlen
   */
  public function setSeqlen($seqlen) {
    $this->seqlen = $seqlen;
  }

  /**
   * Retrieves the md5checksum.
   *
   * @retrun character(32)
   */
  public function getMd5checksum() {
    return $this->md5checksum;
  }

  /**
   * Updates the md5checksum.
   *
   * @param character(32) $md5checksum
   */
  public function setMd5checksum($md5checksum) {
    $this->md5checksum = $md5checksum;
  }

  /**
   * Retrieves the type_id.
   *
   * @retrun integer
   */
  public function getTypeID() {
    return $this->type_id;
  }

  /**
   * Updates the type_id.
   *
   * @param integer $type_id
   */
  public function setTypeID($type_id) {
    $this->type_id = $type_id;
  }

  /**
   * Retrieves the is_analysis.
   *
   * @retrun boolean
   */
  public function getIsAnalysis() {
    return $this->is_analysis;
  }

  /**
   * Updates the is_analysis.
   *
   * @param boolean $is_analysis
   */
  public function setIsAnalysis($is_analysis) {
    $this->is_analysis = $is_analysis;
  }

  /**
   * Retrieves the is_obsolete.
   *
   * @retrun boolean
   */
  public function getIsObsolete() {
    return $this->is_obsolete;
  }

  /**
   * Updates the is_obsolete.
   *
   * @param boolean $is_obsolete
   */
  public function setIsObsolete($is_obsolete) {
    $this->is_obsolete = $is_obsolete;
  }

  /**
   * Retrieves the timeaccessioned.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTimeaccessioned() {
    return $this->timeaccessioned;
  }

  /**
   * Updates the timeaccessioned.
   *
   * @param timestamp_without_time_zone $timeaccessioned
   */
  public function setTimeaccessioned($timeaccessioned) {
    $this->timeaccessioned = $timeaccessioned;
  }

  /**
   * Retrieves the timelastmodified.
   *
   * @retrun timestamp_without_time_zone
   */
  public function getTimelastmodified() {
    return $this->timelastmodified;
  }

  /**
   * Updates the timelastmodified.
   *
   * @param timestamp_without_time_zone $timelastmodified
   */
  public function setTimelastmodified($timelastmodified) {
    $this->timelastmodified = $timelastmodified;
  }
}