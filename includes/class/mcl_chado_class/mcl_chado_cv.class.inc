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
   * Checks the existance of cv. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv
   */
  public static function checkCV(MCL_TEMPLATE $mcl_tmpl, $cv) {
    if ($cv) {

      // Gets the cv.
      $mcl_cv = MCL_CHADO_CV::getCV($cv);
      if (!$mcl_cv) {
        $mcl_tmpl->updateMsg('E', "(name) = ($cv) not found in cv");
      }
    }
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
    else if ($cv_name == 'SITE_TRAIT_ONTOLOGY') {
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

    // Checks if it has already exists.
    $mcl_cv = MCL_CHADO_CV::getCV('null');
    if (!$mcl_cv) {
      $details = array('name' => 'null');
      $mcl_cv = new MCL_CHADO_CV($details);
      if (!$mcl_cv->insert()) {
        return NULL;
      }
    }
    return $mcl_cv;
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
  public static function addCV(MCL_TEMPLATE $mcl_tmpl, $name, $definition) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_cv = MCL_CHADO_CV::byKey($args);
    if ($mcl_cv) {
      $mcl_tmpl->addMsg('D', 'cv', $args);
    }
    else {

      // Adds a new cv.
      $args['definition'] = $definition;
      $mcl_cv = new MCL_CHADO_CV($args);
      if ($mcl_cv->insert()) {
        $mcl_tmpl->addMsg('N', 'cv', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'cv', $args);
      }
    }
    return $mcl_cv;
  }
}