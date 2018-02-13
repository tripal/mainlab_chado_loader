<?php
/**
 * The declaration of MCL_TEMPLATE_PROPERTY_BIMS class.
 *
 */
class MCL_TEMPLATE_PROPERTY_BIMS extends MCL_TEMPLATE {

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
      'name'        => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the property."),
      'type'        => array('req' => TRUE, 'width' => 15, 'desc' => "Type of the property. (e.g.) sample, cross or accession"),
      'alias'       => array('req' => FALSE, 'width' => 15, 'desc' => "Alias of the property."),
      'definition'  => array('req' => FALSE, 'width' => 15, 'desc' => "Definition of the cvterm."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['type']   = -1;
    $cvterms['SITE_CV']['alias']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks property name.
    $this->checkAlpahNumeric($line['name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets cv name.
    $cv_name = $this->getJob()->getParamByKey('cv');

    // Adds a cvterm.
    $cvterm = MCL_CHADO_CVTERM::addCvterm($this, 'SITE_DB', $cv_name, $line['name'], $line['definition'], '', $line['type']);
    if ($cvterm) {

      // Adds the aliases.
      $cvterm->addAlias($this, 'SITE_CV', 'alias', $line['alias'], '[,;]');
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Checks the cv name.
    $cv_name = $this->getJob()->getParamByKey('cv');
    $cv = MCL_CHADO_CV::getCV($cv_name);
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }
  }
}