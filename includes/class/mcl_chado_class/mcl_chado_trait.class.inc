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
   * Checks the plant trait ontology. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   */
  public static function checkCategory(MCL_TEMPLATE $mcl_tmpl, $trait_category, $trait_descriptor_set) {
    if ($trait_category) {
      if (!in_array(strtolower($trait_category), $mcl_tmpl->valid_plant_trait_ontologies)) {
        $mcl_tmpl->updateMsg('E', "Error - trait category : '$trait_category' is not a valid trait category");
      }
      else {
        $cvterm_trait_category = MCL_CHADO_CVTERM::getCvterm($trait_descriptor_set, $trait_category);
        if (!$cvterm_trait_category) {
          $mcl_tmpl->updateMsg('E', "Error - (trait_descriptor_set, trait category ) : ($trait_descriptor_set, $trait_category) does not exist");
        }
      }
    }
  }

  /**
   * Adds a trait.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $db_name
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $definition
   * @param string $accession
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function addTrait(MCL_TEMPLATE $mcl_tmpl, $db_name, $cv_name, $cvterm_name, $definition, $accession = '') {
    $mcl_cvterm = MCL_CHADO_CVTERM::addCvterm($mcl_tmpl, $db_name, $cv_name, $cvterm_name, $definition, $accession);
    return MCL_CHADO_TRAIT::byKey(array('cvterm_id' => $mcl_cvterm->getCvtermID()));
  }

  /**
   * Adds a category.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $trait_ontology
   * @param string $triat_category
   * @param integer $type_id
   * @param string $separator
   */
  public function addCategory(MCL_TEMPLATE $mcl_tmpl, $trait_descriptor_set, $triat_category, $type_id, $separator = '') {
    if ($triat_category) {
      $categories = preg_split("/[,;]/", $triat_category, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($categories as $category) {
        $mcl_category = MCL_CHADO_CVTERM::getCvterm($trait_descriptor_set, $category);
        if ($mcl_category) {
          $mcl_tmpl->addRelationship('cvterm_relationship', 'subject_id', $this->cvterm_id, 'object_id', $mcl_category->getCvtermID(), $type_id);
        }
        else {
          $this->updateMsg('W', "Warning trait_category : '$category' not found cvterm");
        }
      }
    }
  }
}