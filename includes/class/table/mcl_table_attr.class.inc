<?php
/**
 * The declaration of MCL_TABLE_ATTR class.
 *
 */
class MCL_TABLE_ATTR {

  /**
   * Class data mebers.
   */
  private $name      = NULL;
  private $data_type = NULL;
  private $not_null  = FALSE;
  private $default   = NULL;

  /**
   * Implements the class constructor.
   *
   * @param string $def_line
   *  table definition line.
   */
  public function __construct($details) {
    $this->name       = array_key_exists('name', $details)      ? $details['name']      : '';
    $this->data_type  = array_key_exists('data_type', $details) ? $details['data_type'] : '';
    $this->not_null   = array_key_exists('not_null', $details)  ? $details['not_null']  : FALSE;
    $this->default    = array_key_exists('default', $details)   ? $details['default']   : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Print schema for this attribute.
   */
  public function printSchema() {
    $attr_info_str = sprintf("\t%s : %s", $this->name, $this->data_type);
    if ($this->not_null) {
      $attr_info_str .= ' NOT NULL';
    }
    if ($this->default) {
      $attr_info_str .= ' DEFAULT ' . $this->default;
    }
    print $attr_info_str . "\n";
  }

  // Defines getters and setters below.

  /**
   * Retrieves the name.
   *
   * @retrun character_varying(255)
   */
  public function getName() {
    return $this->name;
  }

  /**
   * Updates the name.
   *
   * @param character_varying(255) $name
   */
  public function setName($name) {
    $this->name = $name;
  }

  /**
   * Retrieves the data_type.
   *
   * @retrun string
   */
  public function getDataType() {
    return $this->data_type;
  }

  /**
   * Updates the data_type.
   *
   * @param string $data_type
   */
  public function setDataType($data_type) {
    $this->data_type = $data_type;
  }

 /**
   * Retrieves the not_null.
   *
   * @retrun boolean
   */
  public function getIsNull() {
    return $this->not_null;
  }

  /**
   * Updates the not_null.
   *
   * @param boolean $not_null
   */
  public function setIsNull($not_null) {
    $this->not_null = $not_null;
  }

   /**
   * Retrieves the default.
   *
   * @retrun string
   */
  public function getDefault() {
    return $this->default;
  }

  /**
   * Updates the default.
   *
   * @param string $default
   */
  public function setDefault($default) {
    $this->default = $default;
  }
}