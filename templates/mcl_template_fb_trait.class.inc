<?php
/**
 * The declaration of MCL_TEMPLATE_FB_TRAIT class.
 *
 */
class MCL_TEMPLATE_FB_TRAIT extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'DESCRIPTOR';
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
    $desc = 'The description for FB_TRAIT sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'trait'         => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the trait."),
      'format'        => array('req' => TRUE,   'width' => 10, 'desc' => "Format of the trait."),
      'defaultValue'  => array('req' => FALSE,  'width' => 10, 'desc' => "Default value of the trait."),
      'minimum'       => array('req' => FALSE,  'width' => 10, 'desc' => "minimum value."),
      'maximum'       => array('req' => FALSE,  'width' => 10, 'desc' => "maximum value."),
      'details'       => array('req' => FALSE,  'width' => 15, 'desc' => "Details of the trait."),
      'categories'    => array('req' => FALSE,  'width' => 15, 'desc' => "categories of the trait."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']                            = -1;
    $cvterms['SITE_CV']['format']                           = -1;
    $cvterms['BIMS_FIELD_BOOK_FORMAT_PROP']['categories']   = -1;
    $cvterms['BIMS_FIELD_BOOK_FORMAT_PROP']['minimum']      = -1;
    $cvterms['BIMS_FIELD_BOOK_FORMAT_PROP']['maximum']      = -1;
    $cvterms['BIMS_FIELD_BOOK_FORMAT_PROP']['defaultValue'] = -1;
    $cvterms['BIMS_FIELD_BOOK_FORMAT_PROP']['data_unit']    = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Check categories.
    MCL_CHADO_CVTERM::checkCategories($this, $line['categories']);

    // Checks descirptor format.
    MCL_CHADO_CVTERM::checkCvterm($this, 'BIMS_FIELD_BOOK_FORMAT', $line['format']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Gets cv.
    $cv_id = $this->getJob()->getParamByKey('cv_id');
    $cv = MCL_CHADO_CV::byID($cv_id);

    // Adds a descriptor.
    $descriptor = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv->getName(), $line['trait'], $line['details']);
    if ($descriptor) {

      // Adds properties.
      $descriptor->addProp($this, 'SITE_CV', 'format', $line['format']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'categories', $line['categories']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'minimum', $line['minimum']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'maximum', $line['maximum']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'defaultValue', $line['defaultValue']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'data_unit', $line['data_unit']);
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Checks db.
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));

    // Checks cv name.
    $cv_id = $this->getJob()->getParamByKey('cv_id');
    MCL_CHADO_CV::checkCVByID($this, $cv_id, TRUE);
  }
}