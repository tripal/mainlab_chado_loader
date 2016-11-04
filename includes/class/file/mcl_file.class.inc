<?php
/**
 * The declaration of MCL_FILE class.
 *
 */
class MCL_FILE extends PUBLIC_MCL_FILE {

 /**
  *  Class data members.
  */
  protected $prop_arr = NULL;

  /**
   * @see PUBLIC_MCL_FILE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);

    // Updates type by filename if not assigned.
    if (empty($this->type)) {
      $this->assignTypeByFilename();
    }

    // Updates property array ($this->prop_arr).
    if ($this->prop == '') {
      $this->prop_arr = array();
    }
    else {
      $this->prop_arr = json_decode($this->prop, TRUE);
    }
  }

  /**
   * @see PUBLIC_MCL_FILE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see PUBLIC_MCL_FILE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see PUBLIC_MCL_FILE::insert()
   */
  public function insert() {

    // Updates the parent:$prop fields.
    $this->prop = json_encode($this->prop_arr);

    // Insert a new file.
    return parent::insert();
  }

  /**
   * @see PUBLIC_MCL_FILE::update()
   */
  public function update() {

    // Updates the parent:$prop fields.
    $this->prop = json_encode($this->prop_arr);

    // Updates the user properties.
    return parent::update();
  }

  /**
   * @see PUBLIC_MCL_FILE::delete()
   */
  public function delete() {

    // Deletes the physical file.
    if (file_exists($this->filepath)) {
      unlink($this->filepath);
    }
    parent::delete();
  }

  /**
   * Assigns the type by filename.
   *
   */
  private function assignTypeByFilename() {
    if (preg_match("/(xlsx|xls)$/", $this->filename)) {
      $this->type = 'excel';
    }
    else if (preg_match("/\.zip$/", $this->filename)) {
      $this->type = 'zip';
    }
    else if (preg_match("/\.csv$/", $this->filename)) {
      $this->type = 'csv';
    }
    else {
      $this->type = 'unknown';
    }
  }

  /**
   * Returns files.
   *
   * @return array of MCL_FILE objects.
   */
  public static function getFiles($type = NULL, $user_id = NULL) {

    // Adds condition.
    $where_str = '';
    $args = array();
    if ($type) {
      $where_str .= ' AND LOWER(type) = LOWER(:type) ';
      $args[':type'] = $type;
    }
    if ($user_id) {
      $where_str .= ' AND user_id = :user_id ';
      $args[':user_id'] = $user_id;
    }
    $sql = "SELECT file_id FROM {mcl_file} WHERE 1=1 $where_str ORDER BY submit_date";
    $files = array();
    $results = db_query($sql, $args);
    while ($file_id = $results->fetchField()) {
      $files[] = MCL_FILE::byKey(array('file_id' => $file_id));
    }
    return $files;
  }

  /**
   * Deletes all files that associated with the given job from mcl_file.
   *
   * @param MCL_JOB $job
   *
   * @return boolean
   */
  public static function deleteByJob(MCL_JOB $job) {

    // Gets all files by the given job and call delete() to remove files.
    $sql = "SELECT file_id FROM {mcl_file} WHERE job_id = :job_id";
    $results = db_query($sql, array(':job_id' => $job->getJobID()));
    while ($file_id = $results->fetchField()) {
      $mcl_file = MCL_FILE::byKey(array('file_id' => $file_id));
      $mcl_file->delete();
    }
    return TRUE;
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Returns the value of the given key in prop.
   */
  public function getPropByKey($key) {
    if (array_key_exists($key, $this->prop_arr)) {
      return $this->prop_arr[$key];
    }
    return NULL;
  }

  /**
   * Updates the value of the given key in prop.
   */
  public function setPropByKey($key, $value) {
    $this->prop_arr[$key] = $value;
  }
}