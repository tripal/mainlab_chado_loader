<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_USER class.
 *
 */
class PUBLIC_MCL_USER  {

  /**
   *  Data members for PUBLIC_MCL_USER.
   */
  protected $member_arr = NULL;
  protected $user_id    = NULL;
  protected $name       = NULL;
  protected $uid        = NULL;
  protected $mail       = NULL;
  protected $prop       = NULL;

  /**
   *  Data types for the data members.
   */
  protected static $data_types = array(
    'user_id' => 'integer',
    'name'    => 'character_varying(255)',
    'uid'     => 'integer',
    'mail'    => 'character_varying(255)',
    'prop'    => 'text',
  );

  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr = $details;
    $this->user_id    = array_key_exists('user_id', $details) ? $details['user_id'] : '';
    $this->name       = array_key_exists('name', $details)    ? $details['name']    : '';
    $this->uid        = array_key_exists('uid', $details)     ? $details['uid']     : '';
    $this->mail       = array_key_exists('mail', $details)    ? $details['mail']    : '';
    $this->prop       = array_key_exists('prop', $details)    ? $details['prop']    : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_USER by key(s).
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
    $sql = "SELECT * FROM mcl_user WHERE " . implode(" AND " , $where_arr);
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
        $fields['user_id'] = $this->user_id;
        $fields['name']    = $this->name;
        $fields['uid']     = $this->uid;
        $fields['mail']    = $this->mail;
        $fields['prop']    = $this->prop;


        // Inserts the record.
        $this->user_id = db_insert('mcl_user')
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
      $fields['user_id'] = $this->user_id;
      $fields['name']    = $this->name;
      $fields['uid']     = $this->uid;
      $fields['mail']    = $this->mail;
      $fields['prop']    = $this->prop;


      // Updates the record.
      db_update('mcl_user')
        ->fields($fields)
        ->condition('user_id', $this->user_id, '=')
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

      db_delete('mcl_user')
        ->condition('user_id', $this->user_id, '=')
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
   * Retrieves the user_id.
   *
   * @retrun integer
   */
  public function getUserID() {
    return $this->user_id;
  }

  /**
   * Updates the user_id.
   *
   * @param integer $user_id
   */
  public function setUserID($user_id) {
    $this->user_id = $user_id;
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
   * Retrieves the uid.
   *
   * @retrun integer
   */
  public function getUid() {
    return $this->uid;
  }

  /**
   * Updates the uid.
   *
   * @param integer $uid
   */
  public function setUid($uid) {
    $this->uid = $uid;
  }

  /**
   * Retrieves the mail.
   *
   * @retrun character_varying(255)
   */
  public function getMail() {
    return $this->mail;
  }

  /**
   * Updates the mail.
   *
   * @param character_varying(255) $mail
   */
  public function setMail($mail) {
    $this->mail = $mail;
  }

  /**
   * Retrieves the prop.
   *
   * @retrun text
   */
  public function getProp() {
    return $this->prop;
  }

  /**
   * Updates the prop.
   *
   * @param text $prop
   */
  public function setProp($prop) {
    $this->prop = $prop;
  }
}