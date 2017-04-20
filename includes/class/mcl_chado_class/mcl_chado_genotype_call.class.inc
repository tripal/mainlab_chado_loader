<?php
/**
 * The declaration of MCL_CHADO_GENOTYPE_CALL class.
 *
 */
class MCL_CHADO_GENOTYPE_CALL extends CHADO_GENOTYPE_CALL {

  /**
   *  Class data members.
   */
  /**
   * @see CHADO_GENOTYPE_CALL::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_GENOTYPE_CALL::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_GENOTYPE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Adds a genotype_call.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $args
   *
   * @return MCL_CHADO_GENOTYPE_CALL
   */
  public static function addGenotypeCall(MCL_TEMPLATE $mcl_tmpl = NULL, $args) {

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $genotype_call = MCL_CHADO_GENOTYPE_CALL::byKey($args);
    if ($genotype_call) {
      self::addMsg($mcl_tmpl, 'D', 'genotype_call', $args);
    }
    else {

      // Adds a new genotype.
      $genotype_call = new MCL_CHADO_GENOTYPE_CALL($args);
      if ($genotype_call->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'genotype_call', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'genotype_call', $args);
        return NULL;
      }
    }
    return $genotype_call;
  }
}