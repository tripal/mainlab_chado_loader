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
    return MCL_CHADO_SYNONYM::byKey($args);
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
  public static function addSynonym(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $type_id) {

    // Sets the arguments.
    $args = array(
      'name'    => $name,
      'type_id' => $type_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $synonym = MCL_CHADO_SYNONYM::byKey($args);
    if ($synonym) {
      self::addMsg($mcl_tmpl, 'D', 'synonym', $args);
    }
    else {

      // Adds a new synonym.
      $synonym = new MCL_CHADO_SYNONYM($args);
      if ($synonym->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'synonym', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'synonym', $args);
        return NULL;
      }
    }
    return $synonym;
  }
}