<?php
/**
 * The declaration of MCL_CHADO_DBXREF class.
 *
 */
class MCL_CHADO_DBXREF extends CHADO_DBXREF {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_DBXREF::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_DBXREF::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_DBXREF::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Adds null dbxref.
   *
   * @return MCL_CHADO_DBXREF
   */
  public static function addNull() {

    // Gets null db.
    $mcl_db = MCL_CHADO_DB::addNull();

    // Checks if it has already exists.
    $mcl_dbxref = MCL_CHADO_DBXREF::byKey(array('accession' => 'local:null'));
    if (!$mcl_dbxref) {
      $details = array(
        'accession' => 'local:null',
        'db_id'     => $mcl_db->getDbID(),
      );
      $mcl_dbxref = new MCL_CHADO_CVTERM($details);
      if (!$mcl_dbxref->insert()) {
        return NULL;
      }
    }
    return $mcl_dbxref;
  }

  /**
   * Adds a dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $db_id
   * @param string $accession
   *
   * @return MCL_CHADO_DBXREF
   */
  public static function addDBXref(MCL_TEMPLATE $mcl_tmpl, $db_id, $accession) {

    // Sets the arguments.
    $args = array(
      'db_id'     => $db_id,
      'accession' => $accession,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_dbxref = MCL_CHADO_DBXREF::byKey($args);
    if ($mcl_dbxref) {
      $mcl_tmpl->addMsg('D', 'dbxref', $args);
    }
    else {

      // Adds a new dbxref.
      $mcl_dbxref = new MCL_CHADO_DBXREF($args);
      if ($mcl_dbxref->insert()) {
        $mcl_tmpl->addMsg('N', 'dbxref', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'dbxref', $args);
      }
    }
    return $mcl_dbxref;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $value, $separator = '') {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    $mcl_tmpl->addProperty('dbxrefrop', 'dbxref_id', $this->dbxref_id, $type_id, $value, $separator);
  }
}