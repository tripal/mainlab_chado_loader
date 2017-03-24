<?php
/**
 * The declaration of MCL_TEMPLATE_COLUMN class.
 *
 */
class MCL_TEMPLATE_COLUMN extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'COLUMN';
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
    $desc = 'The description for column sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'name'        => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the cvterm."),
      'alias'       => array('req' => FALSE,  'width' => 15, 'desc' => "Alias of the cvterm."),
      'cv_name'     => array('req' => FALSE,  'width' => 10, 'desc' => "Name of the control vocabulary."),
      'definition'  => array('req' => FALSE,  'width' => 15, 'desc' => "Definition of the control vocabulary."),
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
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));

    // Checks cv.
    MCL_CHADO_CV::checkCV($this, $line['cv_name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the template name for this columns.
    $csv_filepath = $this->csv->getFilepath();
    if (preg_match("/\/([A-Za-z0-9\_]+)_column.csv$/", $csv_filepath, $matches)) {
      $template = $matches[1];
    }

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Adds a cvterm.
    $accession = $line['name'];
    $cvterm = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $line['cv_name'], $line['name'], $line['definition'], $accession);
    if ($cvterm) {

      // Gets cv.
      $cv = MCL_CHADO_CV::byKey(array('cv_id' => $cvterm->getCvID()));

      // Saves in job properties.
      $prop = $this->job->getPropByKey('columns');
      if (array_key_exists($template, $prop)) {
        $prop[$template][$cvterm->getName()] = $cv->getName();
      }
      else {
        $prop[$template] = array($cvterm->getName() => $cv->getName());
      }
      $this->job->setPropByKey('columns', $prop);
    }
  }
}