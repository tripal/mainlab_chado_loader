<?php
/**
 * The declaration of MCL_TEMPLATE_DB class.
 *
 */
class MCL_TEMPLATE_DB extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }
  /**
   * @see MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for db sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'db_name'     => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the db."),
      'url_prefix'  => array('req' => FALSE,  'width' => 10, 'desc' => "Prefix of the URL."),
      'url'         => array('req' => FALSE,  'width' => 10, 'desc' => "URL of the db."),
      'description' => array('req' => FALSE,  'width' => 15, 'desc' => "Description of the db."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a db
    $db = MCL_CHADO_DB::addDB($this, $line['db_name'], $line['url_prefix'], $line['url'], $line['description']);
  }
}