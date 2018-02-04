<?php
/**
 * The declaration of MCL_CHADO_CV class.
 *
 */
class MCL_CHADO_CV extends CHADO_CV {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_CV::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_CV::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_CV::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existence of cv by name. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   * @param boolean $req
   *
   * @return boolean
   */
  public static function checkCV(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $req = FALSE) {
    if ($name) {

      // Gets the cv.
      $cv = MCL_CHADO_CV::getCV($name);
      if (!$cv) {
        self::updateMsg($mcl_tmpl, 'E', "(name) = ($name) not found in cv");
        return FALSE;
      }
    }
    else {
      if ($req) {
        self::updateMsg($mcl_tmpl, 'E', "name is empty");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks the existence of cv by cv_id. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $cv_id
   * @param boolean $req
   *
   * @return boolean
   */
  public static function checkCVByID(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_id, $req = FALSE) {
    if ($cv_id) {

      // Gets the cv.
      $cv = MCL_CHADO_CV::byKey(array('cv_id' => $cv_id));
      if (!$cv) {
        self::updateMsg($mcl_tmpl, 'E', "(cv_id) = ($cv_id) not found in cv");
        return FALSE;
      }
    }
    else {
      if ($req) {
        self::updateMsg($mcl_tmpl, 'E', "cv_id is empty");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the cv by cv ID.
   *
   * @param integer $cv_id
   *
   * @return MCL_CHADO_CV
   */
  public static function getCVByID($cv_id) {
    return MCL_CHADO_CV::bykey(array('cv_id' => $cv_id));
  }

  /**
   * Returns the cv by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_CV
   */
  public static function getCV($name) {

    // Updates the name if name is 'SITE_CV' or 'SITE_TRAIT_ONTOLOGY'.
    if ($name == 'SITE_CV') {
      $name = MCL_VAR::getValueByName($name);
    }
    else if ($name == 'SITE_TRAIT_ONTOLOGY') {
      $name = MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
    }
    return MCL_CHADO_CV::bykey(array('name' => $name));
  }

  /**
   * Adds null cv.
   *
   * @return MCL_CHADO_CV
   */
  public static function addNull() {

    // Checks if it already exists.
    $cv = MCL_CHADO_CV::getCV('null');
    if (!$cv) {
      $details = array('name' => 'null');
      $cv = new MCL_CHADO_CV($details);
      if (!$cv->insert()) {
        return NULL;
      }
    }
    return $cv;
  }

  /**
   * Adds a new cv.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   * @param string $definition
   *
   * return MCL_CHADO_CV
   */
  public static function addCV(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $definition) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $cv = MCL_CHADO_CV::byKey($args);
    if ($cv) {
      self::addMsg($mcl_tmpl, 'D', 'cv', $args);
    }
    else {

      // Adds a new cv.
      $args['definition'] = $definition;
      $cv = new MCL_CHADO_CV($args);
      if ($cv->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'cv', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'cv', $args);
        return NULL;
      }
    }
    return $cv;
  }
}