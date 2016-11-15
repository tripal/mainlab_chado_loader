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
      'file_name'   => array('req' => TRUE,'width' => 10, 'desc' => "Name of the image file."),
      'image_type'  => array('req' => TRUE,'width' => 10, 'desc' => "The type of the image file."),
      'file_legend' => array('req' => TRUE,'width' => 10, 'desc' => "Legend of the image file."),
      'comments'    => array('req' => TRUE,'width' => 10, 'desc' => "Any comments for the image file."),
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




  }
}