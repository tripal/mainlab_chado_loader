<?php
/**
 * The declaration of MCL_TEMPLATE_TYPE class.
 *
 */
class MCL_TEMPLATE_TYPE extends PUBLIC_MCL_TEMPLATE_TYPE {

 /**
  *  Class data members.
  */
  /**
   * @see PUBLIC_MCL_TEMPLATE_TYPE::__construct()
   */
  public function __construct($details) {
    parent::__construct($details);
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE_TYPE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see PUBLIC_MCL_TEMPLATE_TYPE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Returns template information in array. Adds the information for
   * creating two excel sheets for header definition and data template.
   *
   * @param string $order_by
   *
   * @return array of MCL_TEMPLATE_TYPE
   */
  public static function getTemplateTypes($order_by = '') {

    // Updates $order_by.
    $order_by_str = 'ORDER BY TT.type';
    if ($order_by) {
      $order_by_str = 'ORDER BY TT.' . $order_by;
    }

    // Gets template types
    $sql = "SELECT template_type_id FROM {mcl_template_type} TT $order_by_str";
    $mcl_template_types = array();
    $results = db_query($sql);
    while ($template_type_id = $results->fetchField()) {
      $mcl_template_types []= MCL_TEMPLATE_TYPE::byKey(array('template_type_id' => $template_type_id));
    }
    return $mcl_template_types;
  }

  /*
   * Defines getters and setters below.
   */
}