<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_USERS class.
 *
 */
class PUBLIC_USERS  {

  /**
   *  Data members for PUBLIC_USERS.
   */
  protected $member_arr       = NULL;
  protected $uid              = NULL;
  protected $name             = NULL;
  protected $pass             = NULL;
  protected $mail             = NULL;
  protected $theme            = NULL;
  protected $signature        = NULL;
  protected $signature_format = NULL;
  protected $created          = NULL;
  protected $access           = NULL;
  protected $login            = NULL;
  protected $status           = NULL;
  protected $timezone         = NULL;
  protected $language         = NULL;
  protected $picture          = NULL;
  protected $init             = NULL;
  protected $data             = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'uid'              => 'bigint',
    'name'             => 'character_varying(60)',
    'pass'             => 'character_varying(128)',
    'mail'             => 'character_varying(254)',
    'theme'            => 'character_varying(255)',
    'signature'        => 'character_varying(255)',
    'signature_format' => 'character_varying(255)',
    'created'          => 'integer',
    'access'           => 'integer',
    'login'            => 'integer',
    'status'           => 'smallint',
    'timezone'         => 'character_varying(32)',
    'language'         => 'character_varying(12)',
    'picture'          => 'integer',
    'init'             => 'character_varying(254)',
    'data'             => 'bytea',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->uid              = array_key_exists('uid', $details)              ? $details['uid']              : 0;
    $this->name             = array_key_exists('name', $details)             ? $details['name']             : '';
    $this->pass             = array_key_exists('pass', $details)             ? $details['pass']             : '';
    $this->mail             = array_key_exists('mail', $details)             ? $details['mail']             : '';
    $this->theme            = array_key_exists('theme', $details)            ? $details['theme']            : '';
    $this->signature        = array_key_exists('signature', $details)        ? $details['signature']        : '';
    $this->signature_format = array_key_exists('signature_format', $details) ? $details['signature_format'] : '';
    $this->created          = array_key_exists('created', $details)          ? $details['created']          : 0;
    $this->access           = array_key_exists('access', $details)           ? $details['access']           : 0;
    $this->login            = array_key_exists('login', $details)            ? $details['login']            : 0;
    $this->status           = array_key_exists('status', $details)           ? $details['status']           : 0;
    $this->timezone         = array_key_exists('timezone', $details)         ? $details['timezone']         : '';
    $this->language         = array_key_exists('language', $details)         ? $details['language']         : '';
    $this->picture          = array_key_exists('picture', $details)          ? $details['picture']          : 0;
    $this->init             = array_key_exists('init', $details)             ? $details['init']             : '';
    $this->data             = array_key_exists('data', $details)             ? $details['data']             : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_USERS by key(s).
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
    $sql = "SELECT * FROM users WHERE " . implode(" AND " , $where_arr);
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
        $fields['uid']              = (is_numeric($this->uid))     ? $this->uid     : 0;
        $fields['name']             = $this->name;
        $fields['pass']             = $this->pass;
        $fields['mail']             = $this->mail;
        $fields['theme']            = $this->theme;
        $fields['signature']        = $this->signature;
        $fields['signature_format'] = $this->signature_format;
        $fields['created']          = (is_numeric($this->created)) ? $this->created : 0;
        $fields['access']           = (is_numeric($this->access))  ? $this->access  : 0;
        $fields['login']            = (is_numeric($this->login))   ? $this->login   : 0;
        $fields['status']           = (is_numeric($this->status))  ? $this->status  : 0;
        $fields['timezone']         = $this->timezone;
        $fields['language']         = $this->language;
        $fields['picture']          = (is_numeric($this->picture)) ? $this->picture : 0;
        $fields['init']             = $this->init;
        $fields['data']             = $this->data;


        // Inserts the record.
        $this->uid = db_insert('users')
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
      $fields['uid']              = (is_numeric($this->uid))     ? $this->uid     : 0;
      $fields['name']             = $this->name;
      $fields['pass']             = $this->pass;
      $fields['mail']             = $this->mail;
      $fields['theme']            = $this->theme;
      $fields['signature']        = $this->signature;
      $fields['signature_format'] = $this->signature_format;
      $fields['created']          = (is_numeric($this->created)) ? $this->created : 0;
      $fields['access']           = (is_numeric($this->access))  ? $this->access  : 0;
      $fields['login']            = (is_numeric($this->login))   ? $this->login   : 0;
      $fields['status']           = (is_numeric($this->status))  ? $this->status  : 0;
      $fields['timezone']         = $this->timezone;
      $fields['language']         = $this->language;
      $fields['picture']          = (is_numeric($this->picture)) ? $this->picture : 0;
      $fields['init']             = $this->init;
      $fields['data']             = $this->data;


      // Updates the record.
      db_update('users')
        ->fields($fields)
        ->condition('uid', $this->uid, '=')
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

      db_delete('users')
        ->condition('uid', $this->uid, '=')
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
   * Retrieves the uid.
   *
   * @retrun bigint
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * Updates the uid.
   *
   * @param bigint $uid
   */
  public function setUid($uid) {
    $this->uid = $uid;
  }

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(60)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Updates the name.
   *
   * @param character_varying(60) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the pass.
   *
   * @retrun character_varying(128)
   */
  public function getPass() {
    return $this->pass;
  }

  /**
   * Updates the pass.
   *
   * @param character_varying(128) $pass
   */
  public function setPass($pass) {
    $this->pass = $pass;
  }

  /**
   * Retrieves the mail.
   *
   * @retrun character_varying(254)
   */
  public function getMail() {
    return $this->mail;
  }

  /**
   * Updates the mail.
   *
   * @param character_varying(254) $mail
   */
  public function setMail($mail) {
    $this->mail = $mail;
  }

  /**
   * Retrieves the theme.
   *
   * @retrun character_varying(255)
   */
  public function getTheme() {
    return $this->theme;
  }

  /**
   * Updates the theme.
   *
   * @param character_varying(255) $theme
   */
  public function setTheme($theme) {
    $this->theme = $theme;
  }

  /**
   * Retrieves the signature.
   *
   * @retrun character_varying(255)
   */
  public function getSignature() {
    return $this->signature;
  }

  /**
   * Updates the signature.
   *
   * @param character_varying(255) $signature
   */
  public function setSignature($signature) {
    $this->signature = $signature;
  }

  /**
   * Retrieves the signature_format.
   *
   * @retrun character_varying(255)
   */
  public function getSignatureFormat() {
    return $this->signature_format;
  }

  /**
   * Updates the signature_format.
   *
   * @param character_varying(255) $signature_format
   */
  public function setSignatureFormat($signature_format) {
    $this->signature_format = $signature_format;
  }

  /**
   * Retrieves the created.
   *
   * @retrun integer
   */
  public function getCreated() {
    return $this->created;
  }

  /**
   * Updates the created.
   *
   * @param integer $created
   */
  public function setCreated($created) {
    $this->created = $created;
  }

  /**
   * Retrieves the access.
   *
   * @retrun integer
   */
  public function getAccess() {
    return $this->access;
  }

  /**
   * Updates the access.
   *
   * @param integer $access
   */
  public function setAccess($access) {
    $this->access = $access;
  }

  /**
   * Retrieves the login.
   *
   * @retrun integer
   */
  public function getLogin() {
    return $this->login;
  }

  /**
   * Updates the login.
   *
   * @param integer $login
   */
  public function setLogin($login) {
    $this->login = $login;
  }

  /**
   * Retrieves the status.
   *
   * @retrun smallint
   */
  public function getStatus() {
    return $this->status;
  }

  /**
   * Updates the status.
   *
   * @param smallint $status
   */
  public function setStatus($status) {
    $this->status = $status;
  }

  /**
   * Retrieves the timezone.
   *
   * @retrun character_varying(32)
   */
  public function getTimezone() {
    return $this->timezone;
  }

  /**
   * Updates the timezone.
   *
   * @param character_varying(32) $timezone
   */
  public function setTimezone($timezone) {
    $this->timezone = $timezone;
  }

  /**
   * Retrieves the language.
   *
   * @retrun character_varying(12)
   */
  public function getLanguage() {
    return $this->language;
  }

  /**
   * Updates the language.
   *
   * @param character_varying(12) $language
   */
  public function setLanguage($language) {
    $this->language = $language;
  }

  /**
   * Retrieves the picture.
   *
   * @retrun integer
   */
  public function getPicture() {
    return $this->picture;
  }

  /**
   * Updates the picture.
   *
   * @param integer $picture
   */
  public function setPicture($picture) {
    $this->picture = $picture;
  }

  /**
   * Retrieves the init.
   *
   * @retrun character_varying(254)
   */
  public function getInit() {
    return $this->init;
  }

  /**
   * Updates the init.
   *
   * @param character_varying(254) $init
   */
  public function setInit($init) {
    $this->init = $init;
  }

  /**
   * Retrieves the data.
   *
   * @retrun bytea
   */
  public function getData() {
    return $this->data;
  }

  /**
   * Updates the data.
   *
   * @param bytea $data
   */
  public function setData($data) {
    $this->data = $data;
  }
}