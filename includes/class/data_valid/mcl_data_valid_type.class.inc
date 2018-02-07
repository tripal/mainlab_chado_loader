<?php
/**
 * The declaration of MCL_DATA_VALID_TYPE class.
 *
 */
class MCL_DATA_VALID_TYPE extends PUBLIC_MCL_DATA_VALID_TYPE {

 /**
  *  Class data members.
  */
  protected $prop_arr = NULL;

  /**
   * @see MCL_DATA_VALID_TYPE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see MCL_DATA_VALID_TYPE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see MCL_DATA_VALID_TYPE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see MCL_DATA_VALID_TYPE::insert()
   */
  public function insert() {

    // Insert a new file.
    return parent::insert();
  }

  /**
   * @see PUBLIC_MCL_FILE::update()
   */
  public function update() {

    // Updates the user properties.
    return parent::update();
  }

  /**
   * @see PUBLIC_MCL_FILE::delete()
   */
  public function delete() {

    // Deletes all records in mcl_data_valid fist.
    db_delete('mcl_data_valid')
      ->condition('data_valid_type_id', $this->data_valid_type_id)
      ->execute();
    parent::delete();
  }

  /**
   * Returns the options for the data valid types.
   *
   * @param string $type
   *
   * @return array
   */
  public static function getOptions($type) {

    // Gets MCL_DATA_VALID_TYPE.
    $mcl_data_valid_type  = MCL_DATA_VALID_TYPE::byKey(array('type' => $type));
    $data_valid_type_id   = $mcl_data_valid_type->getDataValidTypeID();
    $cv_id                = $mcl_data_valid_type->getCvID();

    // Gets all cvterm ID or values.
    $sql = "
      SELECT name, cvterm_id
      FROM {mcl_data_valid}
      WHERE data_valid_type_id = :data_valid_type_id
      ORDER BY name
    ";
    $result = db_query($sql, array('data_valid_type_id' => $data_valid_type_id));
    $options = array();
    while ($obj = $result->fetchObject()) {
      if ($cv_id) {
        $options[$obj->cvterm_id] = $obj->name;
      }
      else {
        $options[$obj->name] = $obj->name;
      }
    }
    return $options;
  }

  /**
   * Returns all data valid types.
   *
   * @return array of MCL_DATA_VALID_TYPE objects.
   */
  public static function getDataValidTypes() {
    $data_valid_types = array();

    $sql = "SELECT DVT.data_valid_type_id FROM {mcl_data_valid_type} DVT ORDER BY DVT.type";
    $result = db_query($sql);
    while ($data_valid_type_id = $result->fetchField()) {
      $data_valid_types[] = MCL_DATA_VALID_TYPE::byKey(array('data_valid_type_id' => $data_valid_type_id));
    }
    return $data_valid_types;
  }

  /**
   * Returns all data valid values of this type.
   *
   * @return array of objects.
   */
  public function getDataValidValues() {
    $objects = array();

    // Gets all values.
    $sql = "
      SELECT DV.*
      FROM {mcl_data_valid} DV
      WHERE DV.data_valid_type_id = :data_valid_type_id
      ORDER BY DV.name
    ";
    $args = array(
      'data_valid_type_id' => $this->data_valid_type_id,
    );
    $result = db_query($sql, $args);
    while ($obj = $result->fetchObject()) {
      $objects []= $obj;
    }
    return $objects;
  }

  /**
   * Validates the value for the provided data valid type.
   *
   * @param string $data_valid_type
   * @param string $value
   *
   * @return boolean
   */
  public static function validate($data_valid_type, $value) {
    if ($value != '') {

      // Gets data_valid_type_id.
      $mcl_data_valid_type  = MCL_DATA_VALID_TYPE::byKey(array('type' => $data_valid_type));
      if (!$mcl_data_valid_type) {
        return FALSE;
      }
      else {
        $data_valid_type_id = $mcl_data_valid_type->getDataValidTypeID();

        // Checks the value.
        $sql = "
          SELECT COUNT(data_valid_id)
          FROM {mcl_data_valid}
          WHERE data_valid_type_id = :data_valid_type_id AND LOWER(name) = LOWER(:name)
        ";
        $args = array(
          ':data_valid_type_id' => $data_valid_type_id,
          ':name' => $value,
        );
        $num = db_query($sql, $args)->fetchField();
        return ($num) ? TRUE : FALSE;
      }
    }
    return TRUE;
  }

  /*
   * Defines getters and setters below.
   */
}