<?php
/**
 * The declaration of MCL_TABLE_INFO class.
 *
*/
class MCL_TABLE_INFO {

  /**
   * Class data mebers.
   */
  private $table_info   = NULL;
  private $data_format  = NULL;

  /**
   * Implements the class constructor.
   */
  public function __construct($schema_file, $table_file) {

    // Sets data format types.
    $this->data_format = array(
      'TYPE_1' => array('text', 'smallint', 'bigint', 'serial', 'integer', 'double', 'real', 'boolean', 'date', 'bytea'),
      'TYPE_2' => array('character'),
      'TYPE_3' => array(),
      'TYPE_4' => array('timestamp'),
    );

    // Reads the table file.
    $tables = $this->readTableFile($table_file);

    // Reads the schema file.
    $this->readSchemaFile($schema_file, $tables);
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Read table file.
   */
  public function readTableFile($table_file) {

    // Opens the table file.
    if (!($fdr = fopen($table_file, 'r'))) {
      print "Error : opening file ($table_file) for reading\n";
      exit;
    }

    // Gets all the table name from the table file.
    $tables = array();
    while (!feof($fdr)) {
      $line = trim(fgets($fdr));
      if ($line == '') {
        continue;
      }
      $tmp = preg_split("/\s+/", $line);
      $table_name = $class_name = trim($tmp[0]);
      if (!empty($tmp[1])) {
        $class_name = trim($tmp[1]);
      }
      $tables[$table_name] = array('CLASS_NAME' => $class_name, 'EXISTS' => FALSE);
    }
    fclose($fdr);
    return $tables;
  }

  /**
   * Reads schema file.
   *
   * @param string $schema_file
   * @param array $tables
   */
  private function readSchemaFile($schema_file, $tables) {

    // Opens the schema file.
    if (!($fdr = fopen($schema_file, 'r'))) {
      print "Error : Opening file ($schema_file) for reading\n";
      exit;
    }

    // Gets all the table info. from the schema file.
    $table_name       = '';
    $schema_name      = '';
    $table_info       = array();
    $flag_table_def   = FALSE;
    $flag_constraint  = FALSE;
    $flag_comment     = FALSE;
    $cur_table        = NULL;
    $target_tables    = array_keys($tables);
    $sql              = '';
    while (!feof($fdr)) {
      $line_orig = fgets($fdr);
      $line = trim($line_orig);
      if ($line == '') {
        $flag_constraint = FALSE;
        continue;
      }
      if (preg_match("/^SET search_path = (\S+),/i", $line, $matches)) {
        $schema_name = strtolower($matches[1]);
      }
      else if (preg_match("/^CREATE TABLE (\S+)\s/i", $line, $matches)) {
        $table_name = strtolower($matches[1]);
        if (in_array($table_name, $target_tables)) {
          $table_info[$table_name] = new MCL_TABLE($table_name, $schema_name, $tables[$table_name]['CLASS_NAME']);
          $flag_table_def = TRUE;
          $sql = str_replace('CREATE TABLE ', 'CREATE TABLE IF NOT EXISTS ', $line_orig);
          $tables[$table_name]['EXISTS'] = TRUE;
        }
        continue;
      }
      else if (preg_match("/^ALTER SEQUENCE ([A-Za-z0-9_]+) OWNED BY ([A-Za-z0-9_]+)\.([A-Za-z0-9_]+)/i", $line, $matches)) {
        $seq_table_name = strtolower($matches[2]);
        $seq_attr_name = strtolower($matches[3]);
        if (in_array($seq_table_name, $target_tables)) {
          $table_info[$seq_table_name]->setDataType($seq_attr_name, 'serial');
        }
      }
      else if (preg_match("/^ALTER TABLE ONLY (\S+)$/i", $line, $matches)) {
        $table_name = strtolower($matches[1]);

        if (in_array($table_name, $target_tables)) {
          $sql = $line_orig;
          $cur_table = $table_info[$table_name];
          $flag_constraint = TRUE;
          continue;
        }
      }
      else if ($flag_table_def && $line == ');') {

        // Adds statement to the table.
        $table_info[$table_name]->addSQL('TABLE', $sql . "$line\n");

        // Marks as End of TABLE.
        $flag_table_def = FALSE;
        $sql = '';
        continue;
      }

      // Reads table definition line.
      if ($flag_table_def) {
        $attr = $this->parseTableDef($line);
        if ($attr) {
          $table_info[$table_name]->addAttr($attr);
        }
        $sql .= $line_orig;
      }

      // Reads constraint line.
      if ($flag_constraint) {
        if ($cur_table) {
          $const = $this->parseConstraint($line);
          if ($const['type'] == 'PRIMARY') {
            $cur_table->setPrimaryKey($const['attr']);
            $cur_table->addSQL($const['type'], $sql . $line_orig);
          }
          else if ($const['type'] == 'UNIQUE') {
            $cur_table->addUniqueKey($const);
            $cur_table->addSQL($const['type'], $sql . $line_orig);
          }
          else if ($const['type'] == 'FOREIGN') {
            $cur_table->addSQL($const['type'], $sql . $line_orig);
          }
        }
        $sql = '';
        $cur_table = NULL;
        $flag_constraint = FALSE;
      }
    }
    fclose($fdr);

    // Updates primary key.
    $this->updatePrimaryKey($table_info);

    // Sets the table information.
    $this->setTableInfo($table_info);

    // Displays all the names of tables that do not exists in the current
    // schema.
    $warn = '';
    foreach ($tables as $table_name => $info) {
      if (!$info['EXISTS']) {
        $warn .= "    >$table_name does not exists\n";
      }
    }
    if ($warn) {
      print "\n  =============================================\n";
      print "  The following tables do not exist in $schema_name.\n";
      print "  --------------------------------------------\n";
      print "$warn\n\n";
    }
  }

  /**
   * Updates primary key. It sometimes primary key is not set. So finds a
   * field (attribute) that satisfies the following conditions and marks it
   * as primary key.
   *  Conditions:
   *  1. no primvary key exists.
   *  2. no unique keys exist.
   *  3. one sequence exists.
   */
  private function updatePrimaryKey(&$table_info) {

    foreach ($table_info as $table_name => $info) {

      // Checks (1) no primvary key exists.
      $primary_key = $info->getPrimaryKey();
      if ($primary_key) {
        continue;
      }

      // Checks (2) no unique keys exist.
      $unique_keys = $info->getUniqueKeys();
      if (!empty($unique_keys)) {
        continue;
      }

      // Checks (3) one sequence exists.
      $attrs = $info->getAttrs();
      $num_serial = 0;
      $primary_key = '';
      foreach ($attrs as $attr_name => $attr) {
        if ($attr->getDataType() == 'serial') {
          $num_serial++;
          $primary_key = $attr_name;
        }
      }

      // If the number of seiral is one, sets it as a priamry key.
      if ($num_serial == 1) {
        $table_info[$table_name]->setPrimaryKey($primary_key);
      }
    }
  }

  /**
   * Parse constraint line.
   */
  private function parseConstraint($line) {
    $const = array();

    if (preg_match("/^ADD CONSTRAINT (.*) PRIMARY KEY \((.*)\);$/", $line, $matches)) {
      $const_name = strtolower($matches[1]);
      $attr       = strtolower($matches[2]);
      $const = array(
        'name' => $const_name,
        'type' => 'PRIMARY',
        'attr' => $attr,
      );
    }
    else if (preg_match("/^ADD CONSTRAINT (.*) UNIQUE \((.*)\);$/", $line, $matches)) {
      $const_name = strtolower($matches[1]);
      $attrs      = strtolower($matches[2]);
      $const = array(
        'name'  => $const_name,
        'type'  => 'UNIQUE',
        'attrs' => preg_split("/\s*,\s*/", $attrs),
      );
    }
    else if (preg_match("/^ADD CONSTRAINT (.*) FOREIGN KEY \((.*)\) REFERENCES (.*)\((.*)\)/", $line, $matches)) {
      $const_name = strtolower($matches[1]);
      $attr       = strtolower($matches[2]);
      $fk_table   = strtolower($matches[3]);
      $fk_attr    = strtolower($matches[4]);
      $const = array(
        'name'     => $const_name,
        'type'     => 'FOREIGN',
        'fk_table' => $fk_table,
        'fk_attr'  => $fk_attr,
      );
    }
    else if (preg_match("/^ADD CONSTRAINT/", $line)) {
        print "\n\n\n\tError : Unknown constraint type\n\n\t$line\n\n\n";
        exit;
    }
    return $const;
  }

  /**
   * Parses table definition line.
   */
  private function parseTableDef($line) {
    $details = array();

    // Removes the last comma if exists.
    $line = preg_replace("/\s*,$/", '', $line);

    // Checks if it is NOT NULL.
    $not_null = FALSE;
    $trimed_line = preg_replace("/\s+NOT NULL\$/", '', $line);
    if ($trimed_line != $line) {
      $not_null = TRUE;
    }
    $details['not_null'] = $not_null;

    // Parses out table defintion line.
    $tmp = preg_split("/\s/", $line);

    // Skips if the first column is 'CONSTRAINT'.
    $attr_name = trim(array_shift($tmp));

    // Removes double quotation on attribute name if exists.
    if (preg_match('/^"(.*)"$/', $attr_name, $matches)) {
      $attr_name = $matches[1];
    }

    if ($attr_name != 'CONSTRAINT') {
      $details['name']      = $attr_name;
      $details['data_type'] = $this->getDataType($tmp);

      // Adds the default value if exists.
      if (preg_match("/\s+DEFAULT\s+(.*?)\s+/", $line, $matches)) {
        $details['default'] = $matches[1];
      }
      return new MCL_TABLE_ATTR($details);
    }
    return NULL;
  }

  /**
   * Get data type.
   */
  private function getDataType ($arr) {
    if (preg_match("/character\(/", $arr[0])) {
      return $arr[0];
    }
    else if (in_array($arr[0], $this->data_format['TYPE_1'])) {
      return $arr[0];
    }
    else if (in_array($arr[0], $this->data_format['TYPE_2'])) {
      return $arr[0] . '_' . $arr[1];
    }
    else if (in_array($arr[0], $this->data_format['TYPE_3'])) {
      return $arr[0] . '_' . $arr[1] . '_' . $arr[2];
    }
    else if (in_array($arr[0], $this->data_format['TYPE_4'])) {
      return $arr[0] . '_' . $arr[1] . '_' . $arr[2] . '_' . $arr[3];
    }
    else {
      print "\n\n\t\tUnknow data type : " . $arr[0] . "\t\t\n";
      exit;
    }
  }

  /**
   * Prints schema for all tables.
   */
  public function printSchema() {
    $table_info = $this->getTableInfo();
    foreach ($table_info as $table_name => $table_obj) {
      $table_obj->printSchema();
      print "\n";
    }
  }

  // Defines setters and getters below.

  /**
   * Retrieves the table_info.
   *
   * @retrun array
   */
  public function getTableInfo() {
    return $this->table_info;
  }

  /**
   * Updates the table_info.
   *
   * @param array $table_info
   */
  public function setTableInfo($table_info) {
    $this->table_info = $table_info;
  }
}