<?php
/**
 * The declaration of MCL_TEMPLATE_CROSS_BIMS class.
 *
 */
class MCL_TEMPLATE_IMAGE_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
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
    $desc = 'The description for image_bims sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'type'        => array('req' => TRUE,  'width' => 17, 'desc' => "The type of the image. The valid type are 'stock' and 'trait'."),
      'target'      => array('req' => TRUE,  'width' => 17, 'desc' => "The target should be a name of an accession if the type is 'stock' or a name of a descriptor if the type is 'trait'."),
      'filename'    => array('req' => TRUE,  'width' => 13, 'desc' => "The filename of the iamge."),
      'caption'     => array('req' => FALSE, 'width' => 15, 'desc' => "The caption of the image."),
      'description' => array('req' => FALSE, 'width' => 15, 'desc' => "The description of the image."),
      'tag'         => array('req' => FALSE, 'width' => 15, 'desc' => "The tags of the image. Multiple tags, separated by a semicolon (;), can be added."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['image_tag'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets BIMS_CHADO tables.
    $bc_image = new BIMS_CHADO_IMAGE($program_id);

    // Checks type.
    $bc_image->checkTarget($this, $line['type'], $line['target']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Gets BIMS_CHADO.
    $bc_image = new BIMS_CHADO_IMAGE($program_id);

    // Adds a image.
    $details = array(
      'image_uri' => $bc_image->getPrefix($line['type'], $line['target']) . basename($line['filename']),
      'type'      => $line['type'],
    );
    $eimage_id = $bc_image->addImage($this, $details);
    if ($eimage_id) {
      $bc_image->setEimageID($eimage_id);

      // Adds properties.
      $bc_image->addProp($this, 'SITE_CV', 'caption', $line['caption']);
      $bc_image->addProp($this, 'SITE_CV', 'description', $line['description']);
      $bc_image->addProp($this, 'SITE_CV', 'image_tag', $line['tag'], "[,;]");

      // Links the image with target.
      $bc_image->linkTarget($this, $line['type'], $line['target']);
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {}
}