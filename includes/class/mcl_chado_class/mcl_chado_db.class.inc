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
   * @see CHADO_DB::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of db. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $db
   */
  public static function checkDB(MCL_TEMPLATE $mcl_tmpl, $db) {
    if ($db) {

      // Gets the db.
      $mcl_db = MCL_CHADO_DB::getDB($db);
      if (!$mcl_db) {
        $mcl_tmpl->updateMsg('E', "(name) = ($db) not found in db");
      }
    }
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
    return MCL_CHADO_DB::bykey(array('name' => $name));
  }

  /**
   * Adds null db.
   *
   * @return MCL_CHADO_DB
   */
  public static function addNull() {

    // Checks if it has already exists.
    $mcl_db = MCL_CHADO_DB::getDB('null');
    if (!$mcl_db) {
      $details = array('name' => 'null');
      $mcl_db = new MCL_CHADO_DB($details);
      if (!$mcl_db->insert()) {
        return NULL;
      }
    }
    return $mcl_db;
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
  public static function addDB(MCL_TEMPLATE $mcl_tmpl, $name, $prefix_url, $url, $definition) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_db = MCL_CHADO_DB::byKey($args);
    if ($mcl_db) {
      $mcl_tmpl->addMsg('D', 'db', $args);
    }
    else {

      // Adds a new db.
      $args['urlprefix']  = $prefix_url;
      $args['url']        = $url;
      $args['definition'] = $definition;
      $mcl_db = new MCL_CHADO_DB($args);
      if ($mcl_db->insert()) {
        $mcl_tmpl->addMsg('N', 'db', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'db', $args);
      }
    }
    return $mcl_db;
  }
}