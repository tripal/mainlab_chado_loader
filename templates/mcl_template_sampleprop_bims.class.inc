<?php
/**
 * The declaration of MCL_TEMPLATE_SAMPLEPROP class.
 *
 */
class MCL_TEMPLATE_SAMPLEPROP_BIMS extends MCL_TEMPLATE {

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
    $desc = 'The description for sampleprop sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'property_name' => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the propery of the sample."),
      'alias'         => array('req' => FALSE,  'width' => 15, 'desc' => "Alias of the cvterm."),
      'definition'    => array('req' => FALSE,  'width' => 15, 'desc' => "Definition of the cvterm."),
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
  public function runErrorCheckDataLine($line) {}

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Gets cv name.
    $cv_name = $this->getJob()->getParamByKey('cv');

    // Adds a cvterm.
    $cvterm = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv_name, $line['property_name'], $line['definition']);
    if ($cvterm) {

      // Adds aliases.
      $cvterm->addAlias($this, 'SITE_CV', 'alias', $line['alias'], '[,;]');
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Checks db.
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));

    // Checks cv name.
    $cv_name = $this->getJob()->getParamByKey('cv');
    MCL_CHADO_CV::checkCV($this, $cv_name, TRUE);
  }
}