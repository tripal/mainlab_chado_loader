<?php
/**
 * The declaration of MCL_TEMPLATE_CVTERM class.
 *
 */
class MCL_TEMPLATE_CVTERM extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'CVTERM';
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
    $desc = 'The description for cvterm sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'db_name'     => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the db."),
      'cv_name'     => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the control vocabulary."),
      'cvterm_name' => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the cvterm."),
      'accession'   => array('req' => TRUE,   'width' => 10, 'desc' => "Accession of the cvterm."),
      'definition'  => array('req' => FALSE,  'width' => 15, 'desc' => "Definition of the cvterm."),
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

    // Checks db.
    MCL_CHADO_DB::checkDB($this, $line['db_name']);

    // Checks cv.
    MCL_CHADO_CV::checkCV($this, $line['cv_name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a cvterm.
    $cvterm = MCL_CHADO_CVTERM::addCvterm($this, $line['db_name'], $line['cv_name'], $line['cvterm_name'], $line['definition'], $line['accession']);
  }
}