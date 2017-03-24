<?php
/**
 * Adds namespace.
 */
/**
 * The declaration of CHADO_TABLE class.
 *
 */
class CHADO_TABLE  {

  /**
   *  Data members for CHADO_TABLE.
   */
  /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {}

  /**
   * Implements the class destructor.
   */
  public function __destruct() {}

  /**
   * Generate CHADO_TABLE object by key(s).
   *
   * @return descendance of CHADO_TABLE object.
   */
  public static function byKey($keys) {
    // To be overridden by Child class.
  }

  /**
   * Returns the related tables.
   *
   * @return array
   */
  public static function getRelTable() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Returns the foreign keys.
   *
   * @return array
   */
  public static function getFK() {
    // To be overridden by Child class.
    return array();
  }

  /**
   * Adds a new record.
   *
   * return boolean
   */
  public function insert() {
    // To be overridden by Child class.
      return FALSE;
  }

  /**
   * Updates the record.
   *
   * return boolean
   */
  public function update() {
    // To be overridden by Child class.
      return FALSE;
  }

  /**
   * Deletes the record.
   *
   * return boolean
   */
  public function delete() {
    // To be overridden by Child class.
    return FALSE;
  }

  /**
   * Returns the regular expression of the separator.
   *
   * @param string
   */
  public static function getSepRegex($separator) {
    return ($separator) ? "/$separator/" : '/@@@@@/';
  }

  /**
   * Checks argments for empty.
   *
   * @param array $args
   *
   * @return boolean
   */
  public static function checkReqArgs($args) {
    foreach ($args as $key => $value) {
      if ($value == '') {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Creates a string from array.
   *
   * @param assoc array $args
   * @param string $field_1
   * @param integer $id_1
   * @param string $field_2
   * @param string $id_2
   */
  public static function arrStr($args) {
    if (empty($args)) {
      return '';
    }
    $keys = '';
    $values = '';
    $flag = TRUE;
    foreach ($args as $key => $value) {
      $prefix = ', ';
      if ($flag) {
        $prefix = '';
        $flag = FALSE;
      }
      $keys .= $prefix . $key;
      $values .= $prefix . $value;
    }
    return "($keys) = ($values)";
  }

  /**
   * Adds the message.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $key
   * @param string $table
   * @param array $args
   */
  public static function addMsg(MCL_TEMPLATE $mcl_tmpl = NULL, $key, $table, $args) {
    if ($mcl_tmpl) {
      $mcl_tmpl->addMsg($key, $table, $args);
    }
  }

  /**
   * Updates the message.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $key
   * @param string $msg
   */
  public static function updateMsg(MCL_TEMPLATE $mcl_tmpl = NULL, $key, $msg) {
    if ($mcl_tmpl) {
      $mcl_tmpl->updateMsg($key, $msg);
    }
  }

  /**
   * Gets the property.
   *
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   *
   * @return string
   */
  public function getProperty($table_name, $target_field, $target_id, $type_id) {

    // Gets the value of the property table.
    $args = array(
      $target_field => $target_id,
      'type_id'     => $type_id,
    );
    $class_name = strtoupper('CHADO_' . $table_name);
    $obj = $class_name::byKey($args);
    if ($obj) {
      return $obj->getValue();
    }
    else {
      return '';
    }
  }

  /**
   * Adds one or more properties
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addProperty(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $target_field, $target_id, $type_id, $value, $separator = '') {
    if ($value || $value == '0') {

      // Checks duplication before adding a new property.
      $args = array(
        $target_field => $target_id,
        'type_id'     => $type_id,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
      }
      else {

        // Poluplates the array with values.
        $value_arr = preg_split($this->getSepRegex($separator), $value, NULL, PREG_SPLIT_NO_EMPTY);

        // Adds each property.
        $rank = 0;
        foreach ($value_arr as $val) {
          $args['value']  = $val;
          $args['rank']   = $rank++;

          $obj = new $class_name($args);
          if ($obj->insert()) {
            $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
            return TRUE;
          }
          else {
            $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
          }
        }
      }
    }
    return FALSE;
  }

  /**
   * Adds one property with the specified rank.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $target_field
   * @param integer $target_id
   * @param integer $type_id
   * @param string $value
   * @param integer $rank
   *
   * @return boolean
   */
  public function addPropertyRanked(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $target_field, $target_id, $type_id, $value, $rank) {
    if ($value) {

      // Checks duplication before adding a new property.
      $args = array(
        $target_field => $target_id,
        'type_id'     => $type_id,
        'rank'        => $rank,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
      }
      else {

        // Adds a property.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
          return TRUE;
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
        }
      }
    }
  }

  /**
   * Adds a relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $subject_field
   * @param integer $subject_id
   * @param string $object_field
   * @param string $object_id
   * @param integer $type_id
   *
   * @return boolean
   */
  public function addRelationship(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $subject_field, $subject_id, $object_field, $object_id, $type_id) {
    if ($subject_id && $object_id && $type_id) {

      // Checks duplication before adding a new relationship.
      $args = array(
        $subject_field  => $subject_id,
        $object_field   => $object_id,
        'type_id'       => $type_id,
      );
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
      }
      else {

        // Adds a new relationship.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
          return TRUE;
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
        }
      }
    }
    return FALSE;
  }

  /**
   * Adds a link (Adding to a linking table).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $table_name
   * @param string $field_1
   * @param integer $id_1
   * @param string $field_2
   * @param string $id_2
   *
   * @return boolean
   */
  public function addLink(MCL_TEMPLATE $mcl_tmpl = NULL, $table_name, $field_1, $id_1, $field_2, $id_2, $type_id = NULL) {
    if ($id_1 && $id_2) {

      // Checks for a duplication before adding a new relationship.
      $args = array(
        $field_1  => $id_1,
        $field_2  => $id_2,
      );
      if ($type_id) {
        $args['type_id'] = $type_id;
      }
      $class_name = strtoupper('CHADO_' . $table_name);
      $obj = $class_name::byKey($args);
      if ($obj) {
        $this->addMsg($mcl_tmpl, 'D', $table_name, $args);
      }
      else {

        // Adds a new link.
        $obj = new $class_name($args);
        if ($obj->insert()) {
          $this->addMsg($mcl_tmpl, 'N', $table_name, $args);
          return TRUE;
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', $table_name, $args);
        }
      }
    }
    return FALSE;
  }
}