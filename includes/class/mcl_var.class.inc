<?php
/**
 * The declaration of MCL_VAR class.
 *
 */
class MCL_VAR extends PUBLIC_MCL_VAR {

 /**
  *  Class data members.
  */

  /**
   * @see PUBLIC_MCL_VAR::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see PUBLIC_MCL_VAR::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see PUBLIC_MCL_VAR::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Adds a new variable.
   *
   * @param string $name
   * @param string $value
   *
   * @return boolean
   */
  public static function addVar($type, $name, $value, $desc) {

    // Checks for duplication.
    $details = array('name' => $name);
    $mcl_var = MCL_VAR::byKey($details);
    if (!$mcl_var) {

      // Adds a new variable.
      $details['type']        = $type;
      $details['value']       = $value;
      $details['description'] = $desc;
      $mcl_var = new MCL_VAR($details);
      if (!$mcl_var->insert()) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the value of the provided name of the site variable.
   *
   * @param string $name
   *
   * @return string
   */
  public static function getValueByName($name) {
    $mcl_var = MCL_VAR::byKey(array('name' => $name));
    if ($mcl_var) {
      return $mcl_var->getValue();
    }
    return '';
  }

  /**
   * Returns the types of MCL global variable.
   *
   * @return array
   */
  public static function getTypes() {
    return array(
      'site' => 'SITE',
      'default' => 'DEFAULT',
    );
  }

  /**
   * Returns MCL global variable.
   *
   * @param string $type
   *
   * @return array of MCL_VAR
   */
  public static function getGlobalVars($type) {
    $sql = "
      SELECT V.var_id
      FROM {mcl_var} V
      WHERE LOWER(V.type) = LOWER(:type)
      ORDER BY V.name
    ";
    $args = ($type) ? array(':type' => $type) : array();
    $results = db_query($sql, $args);
    $mcl_vars = array();
    while ($var_id = $results->fetchField()) {
      $mcl_vars[] = MCL_VAR::byKey(array('var_id' => $var_id));
    }
    return $mcl_vars;
  }

  /*
   * Defines getters and setters below.
   */
}