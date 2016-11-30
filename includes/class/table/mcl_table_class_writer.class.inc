<?php
/**
 * The declaration of MCL_TABLE_CLASS_WRITER class.
 *
 */
class MCL_TABLE_CLASS_WRITER {

  /**
   * Class data mebers.
   */
  private $table              = NULL;
  private $output_dir         = NULL;
  private $namespace          = NULL;
  private $extends            = NULL;
  private $data_type_numeric  = NULL;
  private $data_type_date     = NULL;

  /**
   * Implements the class constructor.
   *
   * @param MCL_TABLE $table
   * @param MCL_TABLE_WRITER $writer
   * @param string $extends
   */
  public function __construct($table, $writer, $extends = '') {
    $this->table      = $table;
    $this->output_dir = $writer->getOutputDir();
    $this->namespace  = $writer->getNamespace();
    $this->extends    = $extends;

    // Updates the data type arrays.
    $this->data_type_numeric  = $table->getDataTypes('NUMERIC');
    $this->data_type_date     = $table->getDataTypes('DATE');
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Align the all columns with the longest string in the column.
   */
  private function _alignCol($rows, $indent) {

    // Gets the length of the longest string in each column.
    $lens = array();
    foreach ($rows as $i => $row) {
      foreach ($row as $j => $col) {
        $len = strlen($col);

        // Assigns zero if not assigned.
        if (empty($lens[$j])) {
          $lens[$j] = 0;
        }
        if ($lens[$j] < $len) {
          $lens[$j] = $len;
        }
      }
    }

    // Aligns them.
    $aligned = '';
    foreach ($rows as $i => $row) {
      $row_line = '';
      foreach ($row as $j => $col) {
        $row_line .= ($j == 0) ? str_repeat(' ', $indent) : ' ';

        $longet_col = $lens[$j];
        $row_line .= $col . str_repeat(' ', $longet_col - strlen($col));
      }
      $aligned .= rtrim($row_line) . "\n";
    }
    return $aligned;
  }

  /**
   * Lists all attributes. Skips the attribute with 'NOT NULL' if no value
   * has been assigned.
   *
   * @return string
   */
  private function _getAttrList($table, $indent) {
    $attrs = $table->getAttrs();
    $rows_not_null = array();
    $rows_null = array();
    foreach ($attrs as $name => $attr_obj) {
      $data_type = $attr_obj->getDataType();
      $not_null  = $attr_obj->getIsNull();
      if ($data_type != 'serial') {
        if (in_array($data_type, $this->data_type_numeric)) {
          if ($not_null) {
            $default = $attr_obj->getDefault();
            if ($default != '') {
              $row = array("\$fields['$name']", '=', "(is_numeric(\$this->$name))", "? \$this->$name", ": $default;");
            }
            else {
              $row = array("\$fields['$name']", '=', "\$this->$name;");
            }
            $rows_not_null[] = $row;
          }
          else {
            $row = array("if (is_numeric(\$this->$name))", "{ \$fields['$name']", '=', "\$this->$name; }");
            $rows_null[] = $row;
          }
        }
        else if ($data_type == 'boolean') {
          $regex = '"/^(true|false)/i"';
          if ($not_null) {
            $row = array("\$fields['$name']", '=', "(preg_match($regex, \$this->$name))", "? \"'\" . \$this->$name . \"'\"", ": '" . $attr_obj->getDefault() . "';");
            $rows_not_null[] = $row;
          }
          else {
            $row = array("if (preg_match($regex, \$this->$name))", "{ \$fields['$name']", '=', "'\$this->$name'; }");
            $rows_null[] = $row;
          }
        }
        else if (preg_match("/^timestamp/", $data_type)) {
          if ($not_null) {
            $default = $attr_obj->getDefault();
            if ($default != '') {
              $row = array("\$fields['$name']", '=', "(!empty(\$this->$name))", "? \$this->$name", ": '$default';");
            }
            else {
              $row = array("\$fields['$name']", '=', "\$this->$name;");
            }
            $rows_not_null[] = $row;
          }
          else {
            $row = array("if (!empty(\$this->$name))", "{ \$fields['$name']", '=', "\$this->$name; }");
            $rows_null[] = $row;
          }
        }
        else {
          $row = array("\$fields['$name']", '=', "\$this->$name;");
          $rows_not_null[] = $row;
        }
      }
    }
    $attr_list = rtrim($this->_alignCol($rows_not_null, $indent), "\n") . "\n";
    $attr_list .= rtrim($this->_alignCol($rows_null, $indent), "\n");
    return $attr_list;
  }

  /**
   * Generate a class definition file.
   *
   * @return boolean
   */
  public function generateClassFile() {

    // Gets class definition.
    $code = $this->getClassDef($this->table);

    // Opens the table class file.
    $table_name   = $this->table->getName();
    $schema_name  = $this->table->getSchemaName();
    $class_file_name = $schema_name . '_' . $table_name . '.class.inc';
    if (!($fdw = fopen($this->output_dir . "/$class_file_name", 'w'))) {
      print "Error : opening file ($table_name) for writing\n";
      RETURN FALSE;
    }
    fputs($fdw, $code);
    fclose($fdw);
    return TRUE;
  }

  /**
   * Generate a table definition file.
   *
   * @return string
   */
  public function generateTableFile() {

    // Gets class definition.
    $code = $this->getTableDef($this->table);

    // Opens the table class file.
    $table_name   = $this->table->getName();
    $schema_name  = $this->table->getSchemaName();
    $table_file_name = $schema_name . '.' . $table_name . '.table.sql';
    if (!($fdw = fopen($this->output_dir . "/$table_file_name", 'w'))) {
      print "Error : opening file ($table_name) for writing\n";
      RETURN FALSE;
    }
    fputs($fdw, $code);
    fclose($fdw);
    return TRUE;
  }

  /**
   * Get the table definition.
   *
   * @param MCL_TABLE $table
   * @param string $type
   *
   * @return string
   *   SQL statement of the provided type.`
   */
  public static function getTableDef(MCL_TABLE $table, $type = 'ALL') {
    $table_def = '';

    // Gets table info.
    $table_name   = $table->getName();
    $schema_name  = $table->getSchemaName();

    // CREATE TABLE.
    if ($type == 'ALL' || $type == 'TABLE') {
      $table_def .= "--\n";
      $table_def .= "-- Name: $table_name; Type: TABLE; Schema: $schema_name; Owner: -; Tablespace: \n";
      $table_def .= "--\n";
      $table_def .= $table->getSQL('TABLE');
    }

    // PRIMARY KEY.
    if ($type == 'ALL' || $type == 'PRIMARY') {
      $primary_key = $table->getSQL('PRIMARY');
      if ($primary_key) {
        $table_def .= "--\n";
        $table_def .= "-- Name: --; Type: CONSTRAINT; Schema: $schema_name; Owner: -; Tablespace: \n";
        $table_def .= "--\n";
        $table_def .= $primary_key;
      }
    }

    // UNIQUE KEY.
    if ($type == 'ALL' || $type == 'UNIQUE') {
      $unique_key = $table->getSQL('UNIQUE');
      if ($unique_key) {
        $table_def .= "--\n";
        $table_def .= "-- Name: --; Type: CONSTRAINT; Schema: $schema_name; Owner: -; Tablespace: \n";
        $table_def .= "--\n";
        $table_def .= $unique_key;
      }
    }

    // FOREIGN KEY.
    if ($type == 'ALL' || $type == 'FOREIGN') {
      $foreign_key = $table->getSQL('FOREIGN');
      if ($foreign_key) {
        $table_def .= "--\n";
        $table_def .= "-- Name: --; Type: FK CONSTRAINT; Schema: $schema_name; Owner: -; Tablespace: \n";
        $table_def .= "--\n";
        $table_def .= $foreign_key;
      }
    }
    return $table_def;
  }

  /**
   * Get the class definition.
   *
   * @return string
   */
  private function getClassDef($table) {

  // Sets class name.
    $table_name = $table->getName();
    $class_name = strtoupper($table->getClassName());

    // Sets base table.
    $base_class = '';
    if ($this->extends) {
      $base_class = ' extends ' . $this->extends;
    }

    // Gets the all sections.
    $namespace                = $this->getNamespaceLine();
    $data_members             = $this->getDataMembers();
    $data_types               = $this->getDataTypes();
    $constructor              = $this->getConstructor();
    $destructor               = $this->getDestructor();
    $generate_by_key          = $this->getGenerateByKey('drupal');
    $func_insert              = $this->getFuncInsert('drupal');
    $func_update              = $this->getFuncUpdate('drupal');
    $func_delete              = $this->getFuncDelete('drupal');
    $getter_data_type         = $this->getGetterDataType();
    $getter_setter_member_arr = $this->getGetterSetterMemberArr();
    $getters_setters          = $this->getGettersSetters();

    // Generates the class codes.
    $code = <<< CLASS_CODE
<?php
/**
 * Adds namespace.
 */
$namespace
/**
 * The declaration of $class_name class.
 *
 */
class $class_name $base_class {

  /**
   *  Data members for $class_name.
   */
$data_members

  /**
   *  Data types for the data members.
   */
$data_types

  /**
   * Implements the class constructor.
   *
   * @param \$details
   */
$constructor

  /**
   * Implements the class destructor.
   */
$destructor

  /**
   * Generate $class_name by key(s).
   */
$generate_by_key

  /**
   * Adds a new record.
   */
$func_insert

  /**
   * Updates the record.
   */
$func_update

  /**
   * Delete the record.
   */
$func_delete

  /*
   * Defines getters and setters below.
   */
$getter_data_type

$getter_setter_member_arr

$getters_setters
}
CLASS_CODE;
    return $code;
  }

  /**
   * Return the name of the namespace.
   */
  function getNamespaceLine() {
    $namespace = '';
    return ($namespace) ? "use $namespace;" : '';
  }

  /**
   * Return the data members of this class.
   */
  function getDataMembers() {

    // Gets the data members from table attributes.
    $attrs = $this->table->getAttrs();

    // Adds data members.
    $rows = array();

    // Adds $member_arr at the top.
    $rows[] = array('protected', '$member_arr', '= NULL;');

    // Adds table attributes.
    foreach ($attrs as $name => $attr_obj) {
      $row = array(
        'protected', "\$$name", '= NULL;'
      );
      $rows[] = $row;
    }
    return rtrim($this->_alignCol($rows, 2), "\n");
  }

  /**
   * Return the data types.
   */
  function getDataTypes() {

    // Gets the data members from table attributes.
    $attrs = $this->table->getAttrs();

    // Creates table array,
    $data_types = "  protected static \$data_types = array(\n";
    $rows = array();
    foreach ($attrs as $name => $attr_obj) {
      $data_type = $attr_obj->getDataType();
      $row = array("'$name'", '=>', "'$data_type',");
      $rows[] = $row;
    }
    $data_types .= $this->_alignCol($rows, 4);
    $data_types .= '  );';
    return $data_types;
  }

  /**
   * Returns the constructor of this class.
   */
  // Returns the constructor of this class.
  function getConstructor() {

    // Initializes data members.
    $attrs = $this->table->getAttrs();
    $rows = array();

    // Copies $details to $member_arr.
    $rows[] = array(
      '$this->member_arr', '=', '$details;'
    );
    // Initializes data members.
    foreach ($attrs as $name => $attr_obj) {

      // Checks for default value.
      $default = $attr_obj->getDefault();
      $default_value = "''";
      if ($default != '') {
        if (in_array($attr_obj->getDataType(), $this->data_type_numeric)) {
          $default_value = $default;
        }
        else {
          //TODO : Handles default value is non-numeric.
          //$default_value = sprintf("'%s'", $default);
        }
      }
      else {
        // Sets default value to be NULL if data type is timestamp.
        if (in_array($attr_obj->getDataType(), $this->data_type_date)) {
          $default_value = "NULL";
        }
      }
      $row = array(
        '$this->' . $name, '=', "array_key_exists('$name', \$details)", '?', "\$details['$name']", ": $default_value;"
      );
      $rows[] = $row;
    }
    $init_data_members = rtrim($this->_alignCol($rows, 4), "\n");

    $code = <<< CLASS_CODE
  public function __construct(\$details = array()) {
    // Initializes data members.
$init_data_members
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Return the desctructor of this class.
   */
  function getDestructor() {
    $code = <<< CLASS_CODE
  public function __destruct() {
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Returns the generate by key(s) of this class.
   */
   function getGenerateByKey($type = NULL) {
    $table        = $this->table;
    $table_name   = $table->getName();
    $schema_name  = $this->table->getSchemaName();

    // Updates table name. Adds schema if not 'public'.
    if ($schema_name != 'public') {
      $table_name = $schema_name . '.' . $table_name;
    }

    // By key.
    $code = <<< CLASS_CODE
  public static function byKey(\$keys) {

    // Returns NULL if non-integer value are given to the data member whose
    // data type is 'integer'.
    \$where_arr = array();
    \$args      = array();
    foreach (\$keys as \$key => \$value) {

      // Returns NULL if \$value is empty string.
      if (\$value === '') {
        return NULL;
      }

      // Gets data type.
      \$data_type = self::\$data_types[\$key];

      // Checks the value if data type is 'integer'.
      if (in_array(\$data_type, array('serial', 'integer'))) {
        if (!is_int((int)\$value)) {
          return NULL;
        }
      }

      // Adds LOWER() function to the characters.
      if (preg_match("/^(char|text)/", \$data_type)) {
        \$where_arr []= " LOWER(\$key) = LOWER(:\$key) ";
      }
      else {
        \$where_arr []= " \$key = :\$key ";
      }
      \$args[":\$key"] = \$keys[\$key];
    }

    // Gets the table properties.
    \$sql = "SELECT * FROM {$table_name} WHERE " . implode(" AND " , \$where_arr);
    \$details = db_query(\$sql, \$args)->fetch(PDO::FETCH_ASSOC);
    if (\$details) {
      return new self(\$details);
    }
    return NULL;
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Returns insert function().
   */
  function getFuncInsert($type = NULL) {
    $module_name = 'mod';
    $table = $this->table;
    $table_name = $table->getName();
    $primary_key = $table->getPrimaryKey();
    $schema_name  = $this->table->getSchemaName();

    // Updates table name. Adds schema if not 'public'.
    if ($schema_name != 'public') {
      $table_name = $schema_name . '.' . $table_name;
    }

    // Sets primary key if exist.
    $db_insert = "db_insert('$table_name')";
    if ($primary_key) {
      $db_insert = "\$this->$primary_key = " . $db_insert;
    }

    // Lists all attributes.
    $attr_list = $this->_getAttrList($table, 8);

    $code = <<< CLASS_CODE
  public function insert() {

    \$transaction = db_transaction();
    try {

      // TODO:Checks for duplication.
      \$dup = FALSE;

      // Inserts the record if not duplicated.
      if (!\$dup) {

        // Populates all fields. The attribute with 'NOT NULL' would be skipped
        // if the value is empty.
        \$fields = array();
$attr_list

        // Inserts the record.
        $db_insert
          ->fields(\$fields)
          ->execute();
      }
    }
    catch (Exception \$e) {
      \$transaction->rollback();
      watchdog('$module_name', \$e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Return update function().
   */
  function getFuncUpdate($type = NULL) {
    $module_name    = 'mod';
    $table          = $this->table;
    $table_name     = $table->getName();
    $primary_key    = $table->getPrimaryKey();
    $unique_keys    = $table->getUniqueKeys();
    $num_unique_key = $table->getNumUniqueKeys();
    $schema_name    = $this->table->getSchemaName();

    // Updates table name. Adds schema if not 'public'.
    if ($schema_name != 'public') {
      $table_name = $schema_name . '.' . $table_name;
    }

    // Lists all attributes.
    $attr_list = $this->_getAttrList($table, 6);

    $attrs = $table->getAttrs();
    $rows = array();
    foreach ($attrs as $name => $attr_obj) {
      if ($name != $primary_key) {
        $row = array("'$name'", '=>', "\$this->$name,");
        $rows[] = $row;
      }
    }
    $attrs_data = rtrim($this->_alignCol($rows, 10), "\n");

    // Adds conditions.
    $conditions = '';
    if ($primary_key) {
      $conditions = "        ->condition('$primary_key', \$this->$primary_key, '=')\n\n";
    }
    else if ($num_unique_key == 1) {
      $keys = array_pop($unique_keys);
      foreach ($keys as $name) {
        $conditions .= "        ->condition('$name', \$this->$name, '=')\n";
      }
    }
    $conditions = rtrim($conditions, "\n");

    // Writes update function.
    $code = <<< CLASS_CODE
  public function update() {

    \$transaction = db_transaction();
    try {

      // Populates all fields. The attribute with 'NOT NULL' would be skipped
      // if the value is empty.
      \$fields = array();
$attr_list

      // Updates the record.
      db_update('$table_name')
        ->fields(\$fields)
$conditions
        ->execute();
    }
    catch (Exception \$e) {
      \$transaction->rollback();
      watchdog('$module_name', \$e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Return delete function().
   */
  function getFuncDelete($type = NULL) {
    $module_name    = 'mod';
    $table          = $this->table;
    $table_name     = $table->getName();
    $primary_key    = $table->getPrimaryKey();
    $unique_keys    = $table->getUniqueKeys();
    $num_unique_key = $table->getNumUniqueKeys();
    $schema_name    = $this->table->getSchemaName();

    // Updates table name. Adds schema if not 'public'.
    if ($schema_name != 'public') {
      $table_name = $schema_name . '.' . $table_name;
    }

    // Checks the data type of the primary key. Quotes if not numeric.
    $is_data_type_numeric = FALSE;//$table->isDataTypeNumeric($priamry_key_attr);
    $priamry_key_value = ($is_data_type_numeric) ? '$this->' . $primary_key : "'\$this->$primary_key'";

    // Adds conditions.
    $conditions = '';
    if ($primary_key) {
      $conditions = "        ->condition('$primary_key', \$this->$primary_key, '=')\n\n";
    }
    else if ($num_unique_key == 1) {
      $keys = array_pop($unique_keys);
      foreach ($keys as $name) {
        $conditions .= "        ->condition('$name', \$this->$name, '=')\n";
      }
    }
    $conditions = rtrim($conditions, "\n");

    // Default.
    if (!$type) {
      $code = <<< CLASS_CODE
  public function delete() {
    \$sql = "DELETE FROM $table_name WHERE $primary_key = $priamry_key_value";
  }
CLASS_CODE;
    }
    else if ($type == 'drupal') {
      $code = <<< CLASS_CODE
  public function delete() {

    \$transaction = db_transaction();
    try {

      db_delete('$table_name')
$conditions
        ->execute();
    }
    catch (Exception \$e) {
      \$transaction->rollback();
      watchdog('$module_name', \$e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }
CLASS_CODE;
    }
    return $code;
  }

  /**
   * Returns the getter for data type of a member.
   */
  function getGetterDataType() {

    $code = <<< CLASS_CODE
  /**
   * Retrieves the data type of the member.
   *
   * @retrun various
   */
  public function getDataType(\$member) {
    return \$this->data_types[\$member];
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Return the getter and setter for the data member array.
   */
  function getGetterSetterMemberArr() {

    $code = <<< CLASS_CODE
  /**
   * Retrieves the data member array.
   *
   * @retrun array
   */
  public function getMemberArr() {
    return \$this->member_arr;
  }

  /**
   * Updates the data member array.
   *
   * @param array \$member_arr
   */
  public function setMemberArr(\$member_arr) {
    \$this->member_arr = \$member_arr;
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Return the setters and getters for all data members.
   */
  function getGettersSetters() {
    $table = $this->table;

    // Gets all attributes.
    $attrs = $table->getAttrs();
    $getGettersSetters = "";
    foreach ($attrs as $name => $attr_obj) {

      // Replaces id with ID.
      $tmp = preg_replace('/_id$/', '_ID', $name);
      $tmp = ucwords(str_replace('_', ' ', $tmp));
      $func_name = str_replace(' ', '', $tmp);
      $getter = $this->_getGetter($name, $func_name, $attr_obj->getDataType());
      $setter = $this->_getSetter($name, $func_name, $attr_obj->getDataType());
      $getGettersSetters .= "$getter\n\n$setter\n\n";
    }
    return rtrim($getGettersSetters, "\n");
  }

  /**
   * Returns the getter.
   */
  private function _getGetter($var_name, $func_name, $type) {

    // Updates the function name.
    $func_name = 'get' . $func_name;
    $code = <<< CLASS_CODE
  /**
   * Retrieves the $var_name.
   *
   * @retrun $type
   */
  public function $func_name() {
    return \$this->$var_name;
  }
CLASS_CODE;
    return $code;
  }

  /**
   * Returns the setter.
   */
  private function _getSetter($var_name, $func_name, $type) {

    // Updates the function name.
    $func_name = 'set' . $func_name;
    $code = <<< CLASS_CODE
  /**
   * Updates the $var_name.
   *
   * @param $type \$$var_name
   */
  public function $func_name(\$$var_name) {
    \$this->$var_name = \$$var_name;
  }
CLASS_CODE;
    return $code;
  }

  // Defines getters and setters below.

  /**
   * Retrieves the table.
   *
   * @retrun MCL_TABLE object
   */
  public function getTable() {
    return $this->table;
  }

  /**
   * Updates the table.
   *
   * @param MCL_TABLE $table
   */
  public function setTable($table) {
    $this->table = $table;
  }

  /**
   * Retrieves the output dir.
   *
   * @retrun string
   */
  public function getOutputDir() {
    return $this->output_dir;
  }

  /**
   * Updates the output dir.
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
}