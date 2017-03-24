<?php
/**
 * The declaration of MCL_TABLE_WRITER class.
 *
 */
class MCL_TABLE_WRITER {

  /**
   * Class data mebers.
   */
  private $output_dir = NULL;
  private $table_info = NULL;
  private $namespace  = NULL;
  private $extends    = NULL;

  /**
   * Implements the class constructor.
   *
   * @param MCL_TABLE_INFO $table_info
   * @param string $output_dir
   */
  public function __construct(MCL_TABLE_INFO $table_info, $output_dir, $namespace = '', $extends = '') {
    $this->table_info = $table_info;
    $this->output_dir = $output_dir;
    $this->namespace  = $namespace;
    $this->extends    = $extends;
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generates the class files for all the tables.
   *
   * @return boolean
   */
  public function generateClassFiles() {
    $flag = TRUE;

    // Generates class files for all the tables.
    $tables = $this->table_info->getTableInfo();
    foreach ($tables as $table_name => $table) {
      print "Generating class file for $table_name\n";
      $class_writer = new MCL_TABLE_CLASS_WRITER($table, $this);
      if (!$class_writer->generateClassFile()) {
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Generates the table definition files for all the tables.
   */
  public function generateTableFiles($all = FALSE) {

    // Generates the definition files for all the tables.
    $tables = $this->table_info->getTableInfo();

    // Writes the table definitions to one file.
    if ($all) {

      // Gets the table definitions for all tables.
      // We need to place the statements for creating tables first and then
      // places the statements for adding constraints.
      $code = '';
      $table = NULL;

      // SQL : TABLE.
      foreach ($tables as $table_name => $table) {
        $code .= MCL_TABLE_CLASS_WRITER::getTableDef($table, 'TABLE');
      }

      // SQL : PRIMARY, UNIQUE and FOREIGN.
      foreach ($tables as $table_name => $table) {
        $class_writer = new MCL_TABLE_CLASS_WRITER($table, $this);
        $code .= MCL_TABLE_CLASS_WRITER::getTableDef($table, 'PRIMARY');
        $code .= MCL_TABLE_CLASS_WRITER::getTableDef($table, 'UNIQUE');
        $code .= MCL_TABLE_CLASS_WRITER::getTableDef($table, 'FOREIGN');
      }

      // Opens the table file.
      $table_file_name = $table->getSchemaName() . '.all.table.sql';
      if (!($fdw = fopen($this->output_dir . "/$table_file_name", 'w'))) {
        print "Error : opening file for writing\n";
        RETURN FALSE;
      }
      fputs($fdw, $code);
      fclose($fdw);
    }

    // Writes the table definition to an individula file.
    else {
      foreach ($tables as $table_name => $table) {
        print "Generating definition file for $table_name\n";
        $class_writer = new MCL_TABLE_CLASS_WRITER($table, $this);
        $class_writer->generateTableFile();
      }
    }
  }

  // Defines getters and setters below.
  /**
   * Retrieves the table_info.
   *
   * @retrun TASK_INFO object
   */
  public function getTaskInfo() {
    return $this->table_info;
  }

  /**
   * Updates the table_info.
   *
   * @param TASK_INFO $table_info
   */
  public function setTaskInfo($table_info) {
    $this->table_info = $table_info;
  }

  /**
   * Retrieves the output_dir.
   *
   * @retrun string
   */
  public function getOutputDir() {
    return $this->output_dir;
  }

  /**
   * Updates the output_dir.
   *
   * @param string $output_dir
   */
  public function setOutputDir($output_dir) {
    $this->output_dir = $output_dir;
  }

  /**
   * Retrieves the namespace.
   *
   * @retrun string
   */
  public function getNamespace() {
    return $this->namespace;
  }

  /**
   * Updates the namespace.
   *
   * @param string $namespace
   */
  public function setNamespace($namespace) {
    $this->namespace = $namespace;
  }

  /**
   * Retrieves the extends.
   *
   * @retrun string
   */
  public function getExtends() {
    return $this->extends;
  }

  /**
   * Updates the extends.
   *
   * @param string $extends
   */
  public function setExtends($extends) {
    $this->extends = $extends;
  }
}