<?php
/**
 * The declaration of MCL_CHADO_PHENOTYPE class.
 *
 */
class MCL_CHADO_PHENOTYPE extends CHADO_PHENOTYPE {

 /**
  *  Class data members.
  */
  /**
   * @see CHADO_PHENOTYPE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_PHENOTYPE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Adds a phenotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $attr_id
   * @param string $value
   *
   * @return MCL_CHADO_PHENOTYPE
   */
  public static function addPhenotype(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $attr_id, $value) {

    // Sets the arguments.
    $args = array('uniquename' => $uniquename);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_phenotype = MCL_CHADO_PHENOTYPE::byKey($args);
    if ($mcl_phenotype) {
      self::addMsg($mcl_tmpl, 'D', 'phenotype', $args);
    }
    else {

      // Adds a new phenotype.
      $args['value']    = $value;
      $args['attr_id']  = $attr_id;
      $mcl_phenotype = new MCL_CHADO_PHENOTYPE($args);
      if ($mcl_phenotype->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'phenotype', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'phenotype', $args);
        return NULL;
      }
    }
    return $mcl_phenotype;
  }
}