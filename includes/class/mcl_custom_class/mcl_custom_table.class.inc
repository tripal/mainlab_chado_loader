<?php
/**
 * The declaration of MCL_CUSTOM_TABLE class.
 *
 */
class MCL_CUSTOM_TABLE {

 /**
  *  Class data members.
  */
  protected $table_name = NULL;

  /**
   * Implements the class constructor.
   */
  public function __construct($table_name) {
    $this->table_name = $table_name;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {}

  /**
   * Returns the row of data by keys.
   *
   * @param array $details
   *
   * @return object
   */
  public function getDataByKey($keys) {
    // To be overridden by Child class.
    return FALSE;
  }

  /**
   * Checks if the table exists.
   *
   * @param array $args
   *
   * @return boolean
   */
  public function exists() {
    return db_table_exists($this->table_name);
  }

  /**
   * Adds data to the table.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $details
   *
   * @return object
   */
  public function addData(MCL_TEMPLATE $mcl_tmpl = NULL, $details = array()) {
    // To be overridden by Child class.
    return NULL;
  }

  /**
   * Creates a table.
   */
  public function createTable() {
    if ($this->exists()) {
      $this->dropTable();
    }
    db_create_table($this->getTableName(), $this->getTableSchema());
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the table name.
   *
   * @retrun various
   */
  public function getTableName() {
    return $this->table_name;
  }

  /**
   * Updates the table name.
   *
   * @param string $table_name
   */
  public function setTableName($table_name) {
    $this->table_name = $table_name;
  }
}