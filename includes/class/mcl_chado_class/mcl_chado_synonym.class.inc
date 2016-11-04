<?php
/**
 * The declaration of MCL_CHADO_SYNONYM class.
 *
 */
class MCL_CHADO_SYNONYM extends CHADO_SYNONYM {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_SYNONYM::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_SYNONYM::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_SYNONYM::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Returns the synonym by name and type_id.
   *
   * @param string $name
   * @param string $type_id
   *
   * @return MCL_CHADO_SYNONYM
   */
  public static function getSynonym($name, $type_id) {
    $args = array(
      'name'    => $name,
      'type_id' => $type_id,
    );
    return MCL_CHADO_SYNONYM::bykey($args);
  }

  /**
   * Adds a synonym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   * @param string $type_id
   *
   * @return MCL_CHADO_SYNONYM
   */
  public static function addSynonym(MCL_TEMPLATE $mcl_tmpl, $name, $type_id) {

    // Sets the arguments.
    $args = array(
      'name'    => $name,
      'type_id' => $type_id,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_synonym = MCL_CHADO_SYNONYM::byKey($args);
    if ($mcl_synonym) {
      $mcl_tmpl->addMsg('D', 'synonym', $args);
    }
    else {

      // Adds a new synonym.
      $mcl_synonym = new MCL_CHADO_SYNONYM($args);
      if ($mcl_synonym->insert()) {
        $mcl_tmpl->addMsg('N', 'synonym', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'synonym', $args);
      }
    }
    return $mcl_synonym;
  }
}