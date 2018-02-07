<?php
/**
 * The declaration of MCL_CHADO_FEATUREPOS class.
 *
 */
class MCL_CHADO_FEATUREPOS extends CHADO_FEATUREPOS {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_FEATUREPOS::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_FEATUREPOS::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_FEATUREPOS::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Adds a featurepos.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_FEATURE $target_feature
   * @param MCL_CHADO_FEATUREMAP $map
   * @param MCL_CHADO_FEATURE $linkage_group
   *
   * @return MCL_CHADO_FEATUREPOS
   */
  public static function addFeaturepos(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_FEATURE $feature, MCL_CHADO_FEATUREMAP $map, MCL_CHADO_FEATURE $map_feature) {

    // Sets the arguments.
    $args = array(
      'feature_id'     => $feature->getFeatureID(),
      'featuremap_id'  => $map->getFeaturemapID(),
      'map_feature_id' => $map_feature->getFeatureID(),
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $featurepos = MCL_CHADO_FEATUREPOS::byKey($args);
    if ($featurepos) {
      self::addMsg($mcl_tmpl, 'D', 'featurepos', $args);
    }
    else {

      // Adds a new featurepos.
      $args['mappos'] = 0.0;
      $featurepos     = new MCL_CHADO_FEATUREPOS($args);
      if ($featurepos->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'featurepos', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'featurepos', $args);
        return NULL;
      }
    }
    return $featurepos;
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
      return $this->addProperty($mcl_tmpl, 'featureposprop', 'featurepos_id', $this->featurepos_id, $type_id, $value, $separator);
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
    if ($value || $value == '0') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'featureposprop', 'featurepos_id', $this->featurepos_id, $type_id, $value, $separator);
    }
    return TRUE;
  }
}