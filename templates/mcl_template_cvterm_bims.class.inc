<?php
/**
 * The declaration of MCL_TEMPLATE_CVTERM class.
 *
 */
class MCL_TEMPLATE_CVTERM_BIMS extends MCL_TEMPLATE {

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
      'cvterm_name' => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the cvterm."),
      'alias'       => array('req' => FALSE,  'width' => 15, 'desc' => "Alias of the cvterm."),
      'definition'  => array('req' => FALSE,  'width' => 15, 'desc' => "Definition of the cvterm."),
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

    // Checks cv_id.
    $cv_id = $this->getJob()->getParamByKey('cv_id');
    MCL_CHADO_CV::checkCVByID($this, $cv_id, TRUE);

    // Checks db.
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Gets cv.
    $cv_id = $this->getJob()->getParamByKey('cv_id');
    $cv = MCL_CHADO_CV::byKey(array('cv_id' => $cv_id));

    // Adds a cvterm.
    $cvterm = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv->getName(), $line['cvterm_name'], $line['definition'], $line['accession']);
    if ($cvterm) {

      // Adds aliases.
      $cvterm->addAlias($this, 'SITE_CV', 'alias', $line['alias'], '[,;]');
    }
  }
}