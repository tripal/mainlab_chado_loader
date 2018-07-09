<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_ORGANISM class.
 *
 */
class CHADO_ORGANISM  extends CHADO_TABLE {

  /**
   *  Data members for CHADO_ORGANISM.
   */
  protected $member_arr   = NULL;
  protected $organism_id  = NULL;
  protected $abbreviation = NULL;
  protected $genus        = NULL;
  protected $species      = NULL;
  protected $common_name  = NULL;
  protected $comment      = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'organism_id'  => 'serial',
    'abbreviation' => 'character_varying(255)',
    'genus'        => 'character_varying(255)',
    'species'      => 'character_varying(255)',
    'common_name'  => 'character_varying(255)',
    'comment'      => 'text',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr   = $details;
    $this->organism_id  = array_key_exists('organism_id', $details)  ? $details['organism_id']  : '';
    $this->abbreviation = array_key_exists('abbreviation', $details) ? $details['abbreviation'] : '';
    $this->genus        = array_key_exists('genus', $details)        ? $details['genus']        : '';
    $this->species      = array_key_exists('species', $details)      ? $details['species']      : '';
    $this->common_name  = array_key_exists('common_name', $details)  ? $details['common_name']  : '';
    $this->comment      = array_key_exists('comment', $details)      ? $details['comment']      : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_ORGANISM by key(s).
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
    $sql = "SELECT * FROM chado.organism WHERE " . implode(" AND " , $where_arr);
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
      'organism_id' => array(
        array('table' => 'feature', 'attr' => 'organism_id'),
        array('table' => 'featuremap_organism', 'attr' => 'organism_id'),
        array('table' => 'library', 'attr' => 'organism_id'),
        array('table' => 'organism_dbxref', 'attr' => 'organism_id'),
        array('table' => 'organism_image', 'attr' => 'organism_id'),
        array('table' => 'organism_pub', 'attr' => 'organism_id'),
        array('table' => 'organism_relationship', 'attr' => 'object_organism_id'),
        array('table' => 'organism_relationship', 'attr' => 'subject_organism_id'),
        array('table' => 'organismprop', 'attr' => 'organism_id'),
        array('table' => 'stock', 'attr' => 'organism_id'),
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
        $fields['abbreviation'] = $this->abbreviation;
        $fields['genus']        = $this->genus;
        $fields['species']      = $this->species;
        $fields['common_name']  = $this->common_name;
        $fields['comment']      = $this->comment;


        // Inserts the record.
        $this->organism_id = db_insert('chado.organism')
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
      $fields['abbreviation'] = $this->abbreviation;
      $fields['genus']        = $this->genus;
      $fields['species']      = $this->species;
      $fields['common_name']  = $this->common_name;
      $fields['comment']      = $this->comment;


      // Updates the record.
      db_update('chado.organism')
        ->fields($fields)
        ->condition('organism_id', $this->organism_id, '=')
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

      db_delete('chado.organism')
        ->condition('organism_id', $this->organism_id, '=')
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
   * Retrieves the organism_id.
   *
   * @retrun serial
   */
  public function getOrganismID() {
    return $this->organism_id;
  }

  /**
   * Updates the organism_id.
   *
   * @param serial $organism_id
   */
  public function setOrganismID($organism_id) {
    $this->organism_id = $organism_id;
  }

  /**
   * Retrieves the abbreviation.
   *
   * @retrun character_varying(255)
   */
  public function getAbbreviation() {
    return $this->abbreviation;
  }

  /**
   * Updates the abbreviation.
   *
   * @param character_varying(255) $abbreviation
   */
  public function setAbbreviation($abbreviation) {
    $this->abbreviation = $abbreviation;
  }

  /**
   * Retrieves the genus.
   *
   * @retrun character_varying(255)
   */
  public function getGenus() {
    return $this->genus;
  }

  /**
   * Updates the genus.
   *
   * @param character_varying(255) $genus
   */
  public function setGenus($genus) {
    $this->genus = $genus;
  }

  /**
   * Retrieves the species.
   *
   * @retrun character_varying(255)
   */
  public function getSpecies() {
    return $this->species;
  }

  /**
   * Updates the species.
   *
   * @param character_varying(255) $species
   */
  public function setSpecies($species) {
    $this->species = $species;
  }

  /**
   * Retrieves the common_name.
   *
   * @retrun character_varying(255)
   */
  public function getCommonName() {
    return $this->common_name;
  }

  /**
   * Updates the common_name.
   *
   * @param character_varying(255) $common_name
   */
  public function setCommonName($common_name) {
    $this->common_name = $common_name;
  }

  /**
   * Retrieves the comment.
   *
   * @retrun text
   */
  public function getComment() {
    return $this->comment;
  }

  /**
   * Updates the comment.
   *
   * @param text $comment
   */
  public function setComment($comment) {
    $this->comment = $comment;
  }
}