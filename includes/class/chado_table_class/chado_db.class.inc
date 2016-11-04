<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of CHADO_DB class.
 *
 */
class CHADO_DB  {

 /**
  *  Data members for CHADO_DB.
  */
  protected $member_arr  = NULL;
  protected $db_id       = NULL;
  protected $name        = NULL;
  protected $description = NULL;
  protected $urlprefix   = NULL;
  protected $url         = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'db_id'       => 'serial',
    'name'        => 'character_varying(255)',
    'description' => 'character_varying(255)',
    'urlprefix'   => 'character_varying(255)',
    'url'         => 'character_varying(255)',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr  = $details;
    $this->db_id       = array_key_exists('db_id', $details)       ? $details['db_id']       : '';
    $this->name        = array_key_exists('name', $details)        ? $details['name']        : '';
    $this->description = array_key_exists('description', $details) ? $details['description'] : '';
    $this->urlprefix   = array_key_exists('urlprefix', $details)   ? $details['urlprefix']   : '';
    $this->url         = array_key_exists('url', $details)         ? $details['url']         : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate CHADO_DB by key(s).
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
    $sql = "SELECT * FROM chado.db WHERE " . implode(" AND " , $where_arr);
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
        $fields['name']        = $this->name;
        $fields['description'] = $this->description;
        $fields['urlprefix']   = $this->urlprefix;
        $fields['url']         = $this->url;


        // Inserts the record.
        $this->db_id = db_insert('chado.db')
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
      $fields['urlprefix']   = $this->urlprefix;
      $fields['url']         = $this->url;


      // Updates the record.
      db_update('chado.db')
        ->fields($fields)
        ->condition('db_id', $this->db_id, '=')
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

      db_delete('chado.db')
        ->condition('db_id', $this->db_id, '=')
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
   * Retrieves the db_id.
   *
   * @retrun serial
   */
  public function getDbID() {
    return $this->db_id;
  }

  /**
   * Updates the db_id.
   *
   * @param serial $db_id
   */
  public function setDbID($db_id) {
    $this->db_id = $db_id;
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

  /**
   * Retrieves the urlprefix.
   *
   * @retrun character_varying(255)
   */
  public function getUrlprefix() {
    return $this->urlprefix;
  }

  /**
   * Updates the urlprefix.
   *
   * @param character_varying(255) $urlprefix
   */
  public function setUrlprefix($urlprefix) {
    $this->urlprefix = $urlprefix;
  }

  /**
   * Retrieves the url.
   *
   * @retrun character_varying(255)
   */
  public function getUrl() {
    return $this->url;
  }

  /**
   * Updates the url.
   *
   * @param character_varying(255) $url
   */
  public function setUrl($url) {
    $this->url = $url;
  }
}