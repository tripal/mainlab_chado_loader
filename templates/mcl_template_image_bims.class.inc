<?php
/**
 * The declaration of MCL_TEMPLATE_IMAGE_BIMS class.
 *
 */
class MCL_TEMPLATE_IMAGE_BIMS extends MCL_TEMPLATE {

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
      'dataset_name'      => array('req' => TRUE,  'width' => 15, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'accession'         => array('req' => TRUE,  'width' => 15, 'var' => TRUE, 'desc' => "ID of the accession that has been phenotyped. It should match an 'accession' column entry in the 'Accession' sheet."),
      'unique_id'         => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "Unique ID of sample"),
      'primary_order'     => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The primary order of a sample"),
      'secondary_order'   => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "The secondary order of a sample"),
      'file_name'         => array('req' => TRUE,'width' => 10, 'desc' => "Name of the image file."),
      'image_type'        => array('req' => TRUE,'width' => 10, 'desc' => "The type of the image file. (It must be cvterm)"),
      'caption'           => array('req' => TRUE,'width' => 10, 'desc' => "Caption of the image file."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['photo']    = -1;
    $cvterms['SITE_CV']['caption']  = -1;
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




  }
}