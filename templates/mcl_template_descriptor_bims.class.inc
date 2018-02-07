<?php
/**
 * The declaration of MCL_TEMPLATE_DESCRIPTOR_BIMS class.
 *
 */
class MCL_TEMPLATE_DESCRIPTOR_BIMS extends MCL_TEMPLATE {

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
    $desc = 'The description for descriptor sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'descriptor_name' => array('req' => TRUE,  'width' => 15, 'desc' => "Any trait descriptor that a breeder uses in their own program.  These descriptors would be used as column heads the Cross or Phenotype sheets, with a '#' prefix."),
      'alias'           => array('req' => FALSE, 'width' => 15, 'desc' => "Alias of the descriptor."),
      'trait_name'      => array('req' => FALSE, 'width' => 20, 'desc' => "crop trait ontology term (no specific protocol or code is involved with trait name) that corresponds to the trait descriptor (name). This should exist in 'Trait' sheet."),
      'format'          => array('req' => TRUE,  'width' => 15, 'desc' => "The format of the descriptor (numerical, categorical, date, text, etc.)."),
      'categories'      => array('req' => FALSE, 'width' => 10, 'desc' => "The values of the categorical descriptor separated by '/'."),
      'data_unit'       => array('req' => FALSE, 'width' => 10, 'desc' => "Unit for the trait descriptor."),
      'minimum'         => array('req' => FALSE, 'width' => 10, 'desc' => "The minimum value of the descriptor."),
      'maximum'         => array('req' => FALSE, 'width' => 10, 'desc' => "The maximum value of the descriptor."),
      'default_value'   => array('req' => FALSE, 'width' => 10, 'desc' => "The default value of the descriptor."),
      'definition'      => array('req' => FALSE, 'width' => 90, 'desc' => "The definition of the descriptor."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']          = -1;
    $cvterms['SITE_CV']['format']         = -1;
    $cvterms['SITE_CV']['categories']     = -1;
    $cvterms['SITE_CV']['min_value']      = -1;
    $cvterms['SITE_CV']['max_value']      = -1;
    $cvterms['SITE_CV']['default_value']  = -1;
    $cvterms['SITE_CV']['data_unit']      = -1;
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

    // Check categories.
    MCL_CHADO_CVTERM::checkCategories($this, $line['categories']);

    // Checks descirptor format.
    if (!MCL_DATA_VALID_TYPE::validate('descriptor_format', $line['format'])) {
      $this->updateMsg('E', 'Invalid descirptor format : ' . $line['format']);
    }
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

    // Adds a descriptor.
    $descriptor = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv->getName(), $line['descriptor_name'], $line['definition']);
    if ($descriptor) {

      // Adds properties.
      $descriptor->addProp($this, 'SITE_CV', 'format', $line['format']);
      $descriptor->addProp($this, 'SITE_CV', 'categories', $line['categories']);
      $descriptor->addProp($this, 'SITE_CV', 'min_value', $line['minimum']);
      $descriptor->addProp($this, 'SITE_CV', 'max_value', $line['maximum']);
      $descriptor->addProp($this, 'SITE_CV', 'default_value', $line['default_value']);
      $descriptor->addProp($this, 'SITE_CV', 'data_unit', $line['data_unit']);

      // Adds aliases.
      $descriptor->addAlias($this, 'SITE_CV', 'alias', $line['alias'], '[,;]');
    }
  }
}