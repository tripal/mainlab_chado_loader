<?php
/**
 * Adds namespace.
 */

/**
 * The declaration of PUBLIC_MCL_TEMPLATE class.
 *
 */
class PUBLIC_MCL_TEMPLATE  {

 /**
  *  Data members for PUBLIC_MCL_TEMPLATE.
  */
  protected $member_arr       = NULL;
  protected $template_id      = NULL;
  protected $template         = NULL;
  protected $template_type_id = NULL;

 /**
  *  Data types for the data members..
  */
  protected static $data_types = array(
    'template_id'      => 'serial',
    'template'         => 'character_varying(255)',
    'template_type_id' => 'integer',
  );

 /**
   * Implements the class constructor.
   *
   * @param $details
   */
  public function __construct($details = array()) {
    // Initializes data members.
    $this->member_arr       = $details;
    $this->template_id      = array_key_exists('template_id', $details)      ? $details['template_id']      : '';
    $this->template         = array_key_exists('template', $details)         ? $details['template']         : '';
    $this->template_type_id = array_key_exists('template_type_id', $details) ? $details['template_type_id'] : '';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {
  }

  /**
   * Generate PUBLIC_MCL_TEMPLATE by key(s).
   */
  public static function byKey($keys) {

    // Returns NULL if non-integer value are given to the data member whose
    // data type is 'integer'.
    $where_arr = array();
    $args      = array();
    foreach ($keys as $key => $value) {

      // Returns NULL if $value is empty string.
      if ($value === '') {
        return NULL;
      }

      // Gets data type.
      $data_type = self::$data_types[$key];

      // Checks the value if data type is 'integer'.
      if (in_array($data_type, array('serial', 'integer'))) {
        if (!is_int((int)$value)) {
          return NULL;
        }
      }

      // Adds LOWER() function to the characters.
      if (preg_match("/^(char|text)/", $data_type)) {
        $where_arr []= " LOWER($key) = LOWER(:$key) ";
      }
      else {
        $where_arr []= " $key = :$key ";
      }
      $args[":$key"] = $keys[$key];
    }

    // Gets the table properties.
    $sql = "SELECT * FROM mcl_template WHERE " . implode(" AND " , $where_arr);
    $details = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
    if ($details) {
      return new self($details);
    }
    return NULL;
  }

  /**
   * Adds a new record.
   */
  public function insert() {

    $transaction = db_transaction();
    try {

      // TODO:Checks for duplication.
      $dup = FALSE;

      // Inserts the record if not duplicated.
      if (!$dup) {

        // Populates all fields. The attribute with 'NOT NULL' would be skipped
        // if the value is empty.
        $fields = array();
        $fields['template']         = $this->template;
        $fields['template_type_id'] = $this->template_type_id;


        // Inserts the record.
        $this->template_id = db_insert('mcl_template')
          ->fields($fields)
          ->execute();
      }
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Updates the record.
   */
  public function update() {

    $transaction = db_transaction();
    try {

      // Populates all fields. The attribute with 'NOT NULL' would be skipped
      // if the value is empty.
      $fields = array();
      $fields['template']         = $this->template;
      $fields['template_type_id'] = $this->template_type_id;


      // Updates the record.
      db_update('mcl_template')
        ->fields($fields)
        ->condition('template_id', $this->template_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Delete the record.
   */
  public function delete() {

    $transaction = db_transaction();
    try {

      db_delete('mcl_template')
        ->condition('template_id', $this->template_id, '=')
        ->execute();
    }
    catch (Exception $e) {
      $transaction->rollback();
      watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
      return FALSE;
    }
    return TRUE;
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the data type of the member.
   *
   * @retrun various
   */
  public function getDataType($member) {
    return $this->data_types[$member];
  }

  /**
   * Retrieves the data member array.
   *
   * @retrun array
   */
  public function getMemberArr() {
    return $this->member_arr;
  }

  /**
   * Updates the data member array.
   *
   * @param array $member_arr
   */
  public function setMemberArr($member_arr) {
    $this->member_arr = $member_arr;
  }

  /**
   * Retrieves the template_id.
   *
   * @retrun serial
   */
  public function getTemplateID() {
    return $this->template_id;
  }

  /**
   * Updates the template_id.
   *
   * @param serial $template_id
   */
  public function setTemplateID($template_id) {
    $this->template_id = $template_id;
  }

  /**
   * Retrieves the template.
   *
   * @retrun character_varying(255)
   */
  public function getTemplate() {
    return $this->template;
  }

  /**
   * Updates the template.
   *
   * @param character_varying(255) $template
   */
  public function setTemplate($template) {
    $this->template = $template;
  }

  /**
   * Retrieves the template_type_id.
   *
   * @retrun integer
   */
  public function getTemplateTypeID() {
    return $this->template_type_id;
  }

  /**
   * Updates the template_type_id.
   *
   * @param integer $template_type_id
   */
  public function setTemplateTypeID($template_type_id) {
    $this->template_type_id = $template_type_id;
  }
}