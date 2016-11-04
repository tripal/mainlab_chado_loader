<?php
/**
 * The declaration of MCL_CHADO_GENOTYPE class.
 *
 */
class MCL_CHADO_GENOTYPE extends CHADO_GENOTYPE {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_GENOTYPE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_GENOTYPE::byKey()
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
   * Adds a genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   * @param string $description
   *
   * @return MCL_CHADO_PHENOTYPE
   */
  public static function addGenotype(MCL_TEMPLATE $mcl_tmpl, $uniquename, $type_id, $description) {

    // Sets the arguments.
    $args = array(
      'uniquename' => $uniquename,
      'type_id'    => $type_id,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_genotype = MCL_CHADO_GENOTYPE::byKey($args);
    if ($mcl_genotype) {
      $mcl_tmpl->addMsg('D', 'genotype', $args);
    }
    else {

      // Adds a new genotype.
      $args['description'] = $description;
      $mcl_genotype = new MCL_CHADO_GENOTYPE($args);
      if ($mcl_genotype->insert()) {
        $mcl_tmpl->addMsg('N', 'genotype', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'genotype', $args);
      }
    }
    return $mcl_genotype;
  }
}