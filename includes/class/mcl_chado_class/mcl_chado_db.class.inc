<?php
/**
 * The declaration of MCL_CHADO_DB class.
 *
 */
class MCL_CHADO_DB extends CHADO_DB {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_DB::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_DB::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_DB by ID.
   *
   * @param integer $db_id
   *
   * @return MCL_CHADO_DB
   */
  public static function byID($db_id) {
    return MCL_CHADO_DB::bykey(array('db_id' => $db_id));
  }

  /**
   * Returns the db by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_DB
   */
  public static function byName($name) {
    return MCL_CHADO_DB::bykey(array('name' => $name));
  }

  /**
   * @see CHADO_DB::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existence of db. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $db
   *
   * @return boolean
   */
  public static function checkDB(MCL_TEMPLATE $mcl_tmpl = NULL, $db) {
    if ($db) {

      // Gets the db.
      $obj = MCL_CHADO_DB::getDB($db);
      if (!$obj) {
        self::updateMsg($mcl_tmpl, 'E', "(name) = ($db) not found in db");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the db by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_DB
   */
  public static function getDB($name) {

    // Updates the name if name is 'SITE_DB'.
    if ($name == 'SITE_DB') {
      $name = MCL_VAR::getValueByName($name);
    }
    return MCL_CHADO_DB::byName($name);
  }

  /**
   * Adds null db.
   *
   * @return MCL_CHADO_DB
   */
  public static function addNull() {

    // Checks if it already exists.
    $db = MCL_CHADO_DB::getDB('null');
    if (!$db) {
      $details = array('name' => 'null');
      $db = new MCL_CHADO_DB($details);
      if (!$db->insert()) {
        return NULL;
      }
    }
    return $db;
  }

  /**
   * Adds a new db.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   * @param string $prefix_url
   * @param string $url
   * @param string $definition
   *
   * @return MCL_CHADO_DB
   */
  public static function addDB(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $prefix_url, $url, $definition) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $db = MCL_CHADO_DB::byKey($args);
    if ($db) {
      self::addMsg($mcl_tmpl, 'D', 'db', $args);
    }
    else {

      // Adds a new db.
      $args['urlprefix']  = $prefix_url;
      $args['url']        = $url;
      $args['definition'] = $definition;
      $db = new MCL_CHADO_DB($args);
      if ($db->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'db', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'db', $args);
        return NULL;
      }
    }
    return $db;
  }
}