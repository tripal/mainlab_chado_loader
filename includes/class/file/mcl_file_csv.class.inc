<?php
/**
 * The declaration of MCL_FILE_CSV class.
 *
 */
class MCL_FILE_CSV extends MCL_FILE {

 /**
  *  Class data members.
  */
  private $template = NULL;

  /**
   * @see MCL_FILE::__construct()
   */
  public function __construct($details = array()) {
    $details['type'] = 'CSV';
    parent::__construct($details);

    // Sets the template.
    $this->template = $this->getPropByKey("template");
  }

  /**
   * @see MCL_FILE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see MCL_FILE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see MCL_FILE::insert()
   */
  public function insert() {
    return parent::insert();
  }

  /**
   * @see MCL_FILE::update()
   */
  public function update() {
    return parent::update();
  }

  /**
   * Checks if the provided job stage has been passed.
   *
   * @param integer $job_stage
   *
   * @return boolean
   */
  public function hasPassed($job_stage) {
    $passed_stage = $this->getPropByKey('passed_stage');
    if (is_array($passed_stage)) {
      if (array_key_exists($job_stage, $passed_stage)) {
        return $passed_stage[$job_stage];
      }
    }
    return FALSE;
  }

  /**
   * Sets passed value.
   *
   * @param integer $job_stage
   * @param boolean $bool
   */
  public function setPassed($job_stage, $bool) {
    $passed_stage = $this->getPropByKey('passed_stage');
    if (!is_array($passed_stage)) {
      $passed_stage = array();
    }
    $passed_stage[$job_stage] = $bool;
    $this->setPropByKey('passed_stage', $passed_stage);
    $this->update();
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Retrieves the template.
   *
   * @retrun string
   */
  public function getTemplate() {
    return $this->template;
  }

  /**
   * Updates the template.
   *
   * @param string $template
   */
  public function setTemplate($template) {
    $this->template = $template;
    $this->setPropByKey('template', $template);
  }
}