<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_PUB class.
 *
 */
class CHADO_PUB  {

  /**
   *  Data members for CHADO_PUB.
   */
  protected $member_arr  = NULL;
  protected $pub_id      = NULL;
  protected $title       = NULL;
  protected $volumetitle = NULL;
  protected $volume      = NULL;
  protected $series_name = NULL;
  protected $issue       = NULL;
  protected $pyear       = NULL;
  protected $pages       = NULL;
  protected $miniref     = NULL;
  protected $uniquename  = NULL;
  protected $type_id     = NULL;
  protected $is_obsolete = NULL;
  protected $publisher   = NULL;
  protected $pubplace    = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'pub_id'      => 'serial',
    'title'       => 'text',
    'volumetitle' => 'text',
    'volume'      => 'character_varying(255)',
    'series_name' => 'character_varying(255)',
    'issue'       => 'character_varying(255)',
    'pyear'       => 'character_varying(255)',
    'pages'       => 'character_varying(255)',
    'miniref'     => 'character_varying(255)',
    'uniquename'  => 'text',
    'type_id'     => 'integer',
    'is_obsolete' => 'boolean',
    'publisher'   => 'character_varying(255)',
    'pubplace'    => 'character_varying(255)',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->pub_id      = array_key_exists('pub_id', $details)      ? $details['pub_id']      : '';
    $this->title       = array_key_exists('title', $details)       ? $details['title']       : '';
    $this->volumetitle = array_key_exists('volumetitle', $details) ? $details['volumetitle'] : '';
    $this->volume      = array_key_exists('volume', $details)      ? $details['volume']      : '';
    $this->series_name = array_key_exists('series_name', $details) ? $details['series_name'] : '';
    $this->issue       = array_key_exists('issue', $details)       ? $details['issue']       : '';
    $this->pyear       = array_key_exists('pyear', $details)       ? $details['pyear']       : '';
    $this->pages       = array_key_exists('pages', $details)       ? $details['pages']       : '';
    $this->miniref     = array_key_exists('miniref', $details)     ? $details['miniref']     : '';
    $this->uniquename  = array_key_exists('uniquename', $details)  ? $details['uniquename']  : '';
    $this->type_id     = array_key_exists('type_id', $details)     ? $details['type_id']     : '';
    $this->is_obsolete = array_key_exists('is_obsolete', $details) ? $details['is_obsolete'] : '';
    $this->publisher   = array_key_exists('publisher', $details)   ? $details['publisher']   : '';
    $this->pubplace    = array_key_exists('pubplace', $details)    ? $details['pubplace']    : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_PUB by key(s).
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
    $sql = "SELECT * FROM chado.pub WHERE " . implode(" AND " , $where_arr);
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
        $fields['title']       = $this->title;
        $fields['volumetitle'] = $this->volumetitle;
        $fields['volume']      = $this->volume;
        $fields['series_name'] = $this->series_name;
        $fields['issue']       = $this->issue;
        $fields['pyear']       = $this->pyear;
        $fields['pages']       = $this->pages;
        $fields['miniref']     = $this->miniref;
        $fields['uniquename']  = $this->uniquename;
        $fields['type_id']     = $this->type_id;
        $fields['publisher']   = $this->publisher;
        $fields['pubplace']    = $this->pubplace;
        if (preg_match("/^(true|false)/i", $this->is_obsolete)) { $fields['is_obsolete'] = '$this->is_obsolete'; }

        // Inserts the record.
        $this->pub_id = db_insert('chado.pub')
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
      $fields['title']       = $this->title;
      $fields['volumetitle'] = $this->volumetitle;
      $fields['volume']      = $this->volume;
      $fields['series_name'] = $this->series_name;
      $fields['issue']       = $this->issue;
      $fields['pyear']       = $this->pyear;
      $fields['pages']       = $this->pages;
      $fields['miniref']     = $this->miniref;
      $fields['uniquename']  = $this->uniquename;
      $fields['type_id']     = $this->type_id;
      $fields['publisher']   = $this->publisher;
      $fields['pubplace']    = $this->pubplace;
      if (preg_match("/^(true|false)/i", $this->is_obsolete)) { $fields['is_obsolete'] = '$this->is_obsolete'; }

      // Updates the record.
      db_update('chado.pub')
        ->fields($fields)
        ->condition('pub_id', $this->pub_id, '=')
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

      db_delete('chado.pub')
        ->condition('pub_id', $this->pub_id, '=')
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
   * Retrieves the pub_id.
   *
   * @retrun serial
   */
  public function getPubID() {
    return $this->pub_id;
  }

  /**
   * Updates the pub_id.
   *
   * @param serial $pub_id
   */
  public function setPubID($pub_id) {
    $this->pub_id = $pub_id;
  }

  /**
   * Retrieves the title.
   *
   * @retrun text
   */
  public function getTitle() {
    return $this->title;
  }

  /**
   * Updates the title.
   *
   * @param text $title
   */
  public function setTitle($title) {
    $this->title = $title;
  }

  /**
   * Retrieves the volumetitle.
   *
   * @retrun text
   */
  public function getVolumetitle() {
    return $this->volumetitle;
  }

  /**
   * Updates the volumetitle.
   *
   * @param text $volumetitle
   */
  public function setVolumetitle($volumetitle) {
    $this->volumetitle = $volumetitle;
  }

  /**
   * Retrieves the volume.
   *
   * @retrun character_varying(255)
   */
  public function getVolume() {
    return $this->volume;
  }

  /**
   * Updates the volume.
   *
   * @param character_varying(255) $volume
   */
  public function setVolume($volume) {
    $this->volume = $volume;
  }

  /**
   * Retrieves the series_name.
   *
   * @retrun character_varying(255)
   */
  public function getSeriesName() {
    return $this->series_name;
  }

  /**
   * Updates the series_name.
   *
   * @param character_varying(255) $series_name
   */
  public function setSeriesName($series_name) {
    $this->series_name = $series_name;
  }

  /**
   * Retrieves the issue.
   *
   * @retrun character_varying(255)
   */
  public function getIssue() {
    return $this->issue;
  }

  /**
   * Updates the issue.
   *
   * @param character_varying(255) $issue
   */
  public function setIssue($issue) {
    $this->issue = $issue;
  }

  /**
   * Retrieves the pyear.
   *
   * @retrun character_varying(255)
   */
  public function getPyear() {
    return $this->pyear;
  }

  /**
   * Updates the pyear.
   *
   * @param character_varying(255) $pyear
   */
  public function setPyear($pyear) {
    $this->pyear = $pyear;
  }

  /**
   * Retrieves the pages.
   *
   * @retrun character_varying(255)
   */
  public function getPages() {
    return $this->pages;
  }

  /**
   * Updates the pages.
   *
   * @param character_varying(255) $pages
   */
  public function setPages($pages) {
    $this->pages = $pages;
  }

  /**
   * Retrieves the miniref.
   *
   * @retrun character_varying(255)
   */
  public function getMiniref() {
    return $this->miniref;
  }

  /**
   * Updates the miniref.
   *
   * @param character_varying(255) $miniref
   */
  public function setMiniref($miniref) {
    $this->miniref = $miniref;
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
   * Retrieves the publisher.
   *
   * @retrun character_varying(255)
   */
  public function getPublisher() {
    return $this->publisher;
  }

  /**
   * Updates the publisher.
   *
   * @param character_varying(255) $publisher
   */
  public function setPublisher($publisher) {
    $this->publisher = $publisher;
  }

  /**
   * Retrieves the pubplace.
   *
   * @retrun character_varying(255)
   */
  public function getPubplace() {
    return $this->pubplace;
  }

  /**
   * Updates the pubplace.
   *
   * @param character_varying(255) $pubplace
   */
  public function setPubplace($pubplace) {
    $this->pubplace = $pubplace;
  }
}