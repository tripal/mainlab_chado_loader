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
   * @param MCL_CHADO_FEATRUE $target_feature
   * @param MCL_CHADO_FEATRUEMAP $map
   * @param MCL_CHADO_FEATRUE $linkage_group
   *
   * @return MCL_CHADO_FEATUREPOS
   */
  public static function addFeaturepos(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_FEATRUE $feature, MCL_CHADO_FEATRUEMAP $map, MCL_CHADO_FEATRUE $map_feature) {

    // Sets the arguments.
    $args = array(
      'feature_id'     => $feature->getFeatureID(),
      'featuremap_id'  => $map->getFeaturemapID(),
      'map_feature_id' => $map_feature->getFeatureID(),
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $featurepos = MCL_CHADO_FEATUREPOS::byKey($args);
    if ($featurepos) {
      $mcl_tmpl->addMsg('D', 'featurepos', $args);
    }
    else {

      // Adds a new featurepos.
      $args['mappos'] = 0.0;
      $featurepos = new MCL_CHADO_FEATUREPOS($args);
      if ($featurepos->insert()) {
         $mcl_tmpl->addMsg('N', 'featurepos', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'featurepos', $args);
      }
    }
    return $featurepos;
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
    $mcl_tmpl->addProperty('featureposprop', 'featurepos_id', $this->featurepos_id, $type_id, $value, $separator);
  }
}