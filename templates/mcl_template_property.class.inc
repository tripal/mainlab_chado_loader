<?php
/**
 * The declaration of MCL_TEMPLATE_PROPERTY class.
 *
 */
class MCL_TEMPLATE_PROPERTY extends MCL_TEMPLATE {

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
      'db_name'     => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the db."),
      'cv_name'     => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the control vocabulary."),
      'name'        => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the property."),
      'alias'       => array('req' => FALSE, 'width' => 15, 'desc' => "Alias of the property."),
      'accession'   => array('req' => FALSE, 'width' => 10, 'desc' => "Accession of the cvterm."),
      'definition'  => array('req' => FALSE, 'width' => 15, 'desc' => "Definition of the cvterm."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias'] = -1;
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

    // Checks property name.
    $this->checkAlpahNumeric($line['name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a cvterm.
    $cvterm = MCL_CHADO_CVTERM::addCvterm($this, $line['db_name'], $line['cv_name'], $line['name'], $line['definition'], $line['accession']);
    if ($cvterm) {

      // Adds aliases.
      $cvterm->addAlias($this, 'SITE_CV', 'alias', $line['alias'], '[,;]');
    }
  }
}