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
    $db = MCL_CHADO_DB::addNull();

    // Checks if it has already exists.
    $dbxref = MCL_CHADO_DBXREF::byKey(array('accession' => 'local:null'));
    if (!$dbxref) {
      $details = array(
        'accession' => 'local:null',
        'db_id'     => $db->getDbID(),
      );
      $dbxref = new MCL_CHADO_CVTERM($details);
      if (!$dbxref->insert()) {
        return NULL;
      }
    }
    return $dbxref;
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
  public static function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, $db_id, $accession) {

    // Sets the arguments.
    $args = array(
      'db_id'     => $db_id,
      'accession' => $accession,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $dbxref = MCL_CHADO_DBXREF::byKey($args);
    if ($dbxref) {
      self::addMsg($mcl_tmpl, 'D', 'dbxref', $args);
    }
    else {

      // Adds a new dbxref.
      $dbxref = new MCL_CHADO_DBXREF($args);
      if ($dbxref->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'dbxref', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'dbxref', $args);
        return NULL;
      }
    }
    return $dbxref;
  }

  /**
   * Adds a property by type ID (cvterm ID).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addPropByID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $value, $separator = '') {
    if ($value != '') {
      return $this->addProperty($mcl_tmpl, 'dbxrefrop', 'dbxref_id', $this->dbxref_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value, $separator = '') {
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'dbxrefrop', 'dbxref_id', $this->dbxref_id, $type_id, $value, $separator);
    }
    return TRUE;
  }
}