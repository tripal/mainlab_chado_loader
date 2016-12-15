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
  public static function addGenotypeCall(MCL_TEMPLATE $mcl_tmpl, $args) {

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_genotype_call = MCL_CHADO_GENOTYPE_CALL::byKey($args);
    if ($mcl_genotype_call) {
      $mcl_tmpl->addMsg('D', 'genotype_call', $args);
    }
    else {

      // Adds a new genotype.
      $mcl_genotype_call = new MCL_CHADO_GENOTYPE_CALL($args);
      if ($mcl_genotype_call->insert()) {
        $mcl_tmpl->addMsg('N', 'genotype_call', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'genotype_call', $args);
      }
    }
    return $mcl_genotype_call;
  }
}