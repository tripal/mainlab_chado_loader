<?php
/**
 * The declaration of MCL_USER class.
 *
 */
class MCL_USER extends PUBLIC_MCL_USER {

 /**
  *  Data members for MCL_USER.
  */
  protected $prop_arr = NULL;

 /**
   * @see PUBLIC_MCL_USER::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);

    // Updates property array ($this->prop_arr).
    if ($this->prop == '') {
      $this->prop_arr = array();
    }
    else {
      $this->prop_arr = json_decode($this->prop, TRUE);
    }
  }

  /**
   * @see PUBLIC_MCL_USER::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return $parent;
  }

  /**
   * @see PUBLIC_MCL_USER::insert()
   */
  public function insert() {

    // Updates the parent:$prop field.
    $this->prop = json_encode($this->prop_arr);

    // Insert a new user.
    return parent::insert();
  }

  /**
   * @see PUBLIC_MCL_USER::update()
   */
  public function update() {

    // Updates the parent:$prop field.
    $this->prop = json_encode($this->prop_arr);

    // Updates the user properties.
    return parent::update();
  }


  /**
   * Returns a list of MCL users. Return all non MCL user if the flag is FALSE.
   *
   * @param boolean $flag
   *
   * @return array of objects
   */
  public static function getUserObjects($flag = TRUE) {

    // Get the MLC or non-MLC users..
    $sql = "SELECT MU.* FROM {mcl_user} MU  ORDER BY MU.name";
    if (!$flag) {
      $sql = "
        SELECT U.*
        FROM {users} U
        WHERE U.uid NOT IN (SELECT MU.uid FROM {mcl_user} MU)
        ORDER BY U.name
      ";
    }

    // Gets users.
    $users = array();
    $results = db_query($sql);
    while ($user_obj = $results->fetchObject()) {
      if ($user_obj->uid > 0 && $user_obj->name)
      $users[$user_obj->uid] = $user_obj->name;
    }
    return $users;
  }

  /**
   * Returns Drupal user object.
   *
   * @param integer $user_id
   *
   * @return object
   */
  public static function getDrupalUser($user_id) {
    $sql = "SELECT U.* FROM {users} U WHERE U.uid = :uid";
    $results = db_query($sql, array(':uid' => $user_id));
    return $results->fetchObject();
  }

  /**
   * Adds a new user to mcl_user table.
   *
   * @param integer $user_id
   *
   * @return MCL_USER|NULL
   */
  public static function addUser($user_id) {

    // Checks if the user is alreay in system.
    $mcl_user = MCL_USER::byKey(array('user_id' => $user_id));
    if ($mcl_user) {
      return $mcl_user;
    }

    // Gets the user properties.
    $sql = "SELECT * FROM {users} WHERE uid = :id";
    $args = array(':id' => $user_id);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    $details['user_id'] = $user_id;
    $new_user = new MCL_USER($details);
    if ($new_user->insert()) {
      return $new_user;
    }
    return NULL;
  }

  /**
   * Returns the user directory of this user.
   *
   * @return string
   */
  public function getUserDir() {

    // Builds the user directory path.
    $user_dir = mcl_get_config_setting('mcl_working_dir') . '/user/' . $this->getName();

    // Creates the directory if not exists.
    mcl_create_dir($user_dir);
    return $user_dir;
  }

  /**
   * Returns the URL of the user directory of this user.
   *
   * @return string
   */
  public function getUserDirURL() {

    // Gets the user directory.
    $user_dir = $this->getUserDir();

    // Converts into URL
    $user_url = 'public://mcl/user/' . $this->getName();
    return $user_url;
  }

  /*
   * Defines getters and setters below.
   */
   /**
   * Returns the value of the given key in prop.
   */
  public function getPropByKey($key) {
    if (array_key_exists($key, $this->prop_arr)) {
      return $this->prop_arr[$key];
    }
    return NULL;
  }

  /**
   * Updates the value of the given key in prop.
   */
  public function setPropByKey($key, $value) {
    $this->prop_arr[$key] = $value;
  }
}