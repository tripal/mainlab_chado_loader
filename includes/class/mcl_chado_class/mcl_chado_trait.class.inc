<?php
/**
 * The declaration of MCL_CHADO_TRAIT class.
 *
 */
class MCL_CHADO_TRAIT extends MCL_CHADO_CVTERM {

 /**
  *  Class data members.
  */
  /**
   * @see MCL_CHADO_CVTERM::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see MCL_CHADO_CVTERM::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see MCL_CHADO_CVTERM::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of trait. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cvterm_name
   * @param string $cv_name
   *
   * @return boolean
   */
  public static function checkTrait(MCL_TEMPLATE $mcl_tmpl = NULL, $cvterm_name, $cv_name = NULL) {

    // Updates cv.name
    if (!$cv_name) {
      $cv_name = MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
    }

    // Checks cvterm.
    if ($cv_name && $cvterm_name) {

      // Gets the cvterm.
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      if (!$cvterm) {
        self::updateMsg($mcl_tmpl, 'E', "(cv_name, cvterm_name) = ($cv_name, $cvterm_name) not found in cvterm");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the trait.
   *
   * @param string $cvterm_name
   * @param string $cv_name
   *
   * @return MCL_CHADO_TRAIT
   */
  public static function getTrait($cvterm_name, $cv_name = NULL) {

    // Updates cv.name
    if (!$cv_name) {
      $cv_name = MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
    }
    $cv = MCL_CHADO_CV::getCV($cv_name);
    if ($cv) {
      return MCL_CHADO_CVTERM::bykey(array('cv_id' => $cv->getCvID(), 'name' => $cvterm_name));
    }
    return NULL;
  }

  /**
   * Adds a trait.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cvterm_name
   * @param string $definition
   * @param string $accession
   *
   * @return MCL_CHADO_TRAIT
   */
  public static function addTrait(MCL_TEMPLATE $mcl_tmpl = NULL, $cvterm_name, $definition = '', $accession = '') {
    $db_name = MCL_VAR::getValueByName('SITE_DB');
    $cv_name = MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
    $cvterm = MCL_CHADO_CVTERM::addCvterm($mcl_tmpl, $db_name, $cv_name, $cvterm_name, $definition, $accession);
    if ($cvterm) {
      return MCL_CHADO_TRAIT::byKey(array('cvterm_id' => $cvterm->getCvtermID()));
    }
    return NULL;
  }

  /**
   * Adds a category.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $trait_category
   * @param integer $type_id
   * @param string $separator
   *
   * @return boolean
   */
  public function addCategory(MCL_TEMPLATE $mcl_tmpl = NULL, $trait_category, $type_id, $separator = '') {
    $flag = TRUE;
    if ($trait_category) {
      $categories = preg_split("/[,;]/", $trait_category, NULL, PREG_SPLIT_NO_EMPTY);
      $cv_name = MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY');
      foreach ($categories as $category) {
        $category = MCL_CHADO_CVTERM::getCvterm($cv_name, $category);
        if ($category) {
          if (!$this->addRelationship($mcl_tmpl, 'cvterm_relationship', 'subject_id', $this->cvterm_id, 'object_id', $category->getCvtermID(), $type_id)) {
            $flag = FALSE;
          }
        }
        else {
          $this->updateMsg($mcl_tmpl, 'E', "Warning trait_category : '$category' not found cvterm");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }
}