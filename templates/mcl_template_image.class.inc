<?php
/**
 * The declaration of MCL_TEMPLATE_IMAGE class.
 *
 */
class MCL_TEMPLATE_IMAGE extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'IMAGE';
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
    $desc = 'The description for field book sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'file_name'       => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the image file."),
      'image_type'      => array('req' => TRUE,  'width' => 10, 'desc' => "The type of the image file."),
      'file_legend'     => array('req' => TRUE,  'width' => 10, 'desc' => "Legend of the image file."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the image file."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['photo_provider'] = -1;
    $cvterms['SITE_CV']['legend']         = -1;
    $cvterms['SITE_CV']['comments']       = -1;
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

    // Adds an eimage.
    $eimage = MCL_CHADO_IMAGE::addImage($this, $line['file_name'], $line['image_type'], $line['file_name']);
    if ($eimage) {

      // Adds the properties.
      $eimage->addProp($this, 'SITE_CV', 'legend', $line['file_legend']);
      $eimage->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $eimage, $line);
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Lists the job paremters.
    $cv_name = $this->getJob()->getParamByKey('cv');

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($cv_name, 'SITE_CV');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the special columns");
    }
  }
}