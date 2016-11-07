<?php
/**
 * The declaration of MCL_TEMPLATE_LIBRARY class.
 *
 */
class MCL_TEMPLATE_LIBRARY extends MCL_TEMPLATE {

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
    $desc = 'The description for library sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'library_name'  => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the library (or SNP chip name)."),
      'type'          => array('req' => TRUE,   'width' => 10, 'desc' => "Type of the library ('SNP_chip' for the SNP chip data)."),
      'pi'            => array('req' => FALSE,  'width' => 10, 'desc' => "PI of the project."),
      'description'   => array('req' => FALSE,  'width' => 10, 'desc' => "Description of the library."),
      'reference'     => array('req' => FALSE,  'width' => 10, 'desc' => "pub_id for any reference."),
    );

    // Adds labels.
    $headers['pi']['label'] = 'PI';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['SNP_chip']     = -1;
    $cvterms['SITE_CV']['description']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks type.
    MCL_CHADO_CVTERM::checkCvterm($this, 'SITE_CV', $line['type']);

    // Checks PI.
    MCL_CHADO_CONTACT::checkContact($this, $line['pi']);

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a library.
    $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['type']);
    $library = MCL_CHADO_LIBRARY::addLibrary($this, $line['library_name'], $cvterm->getCvtermID());
    if ($library) {

      // Adds properties.
      $library->addProp($this, 'SITE_CV', 'description', $line['description']);

      // Adds PI.
      $library->addContact($this, $line['pi']);

      // Adds references.
      $library->addReference($this, $line['reference'], "[,;]");
    }
  }
}