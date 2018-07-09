<?php
/**
 * The declaration of MCL_TEMPLATE_DESCRIPTOR_FB class.
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
      'format'          => array('req' => TRUE,  'width' => 15, 'desc' => "The format of the descriptor (numeric, categorical, date, text, etc.)."),
      'trait_name'      => array('req' => FALSE, 'width' => 20, 'desc' => "crop trait ontology term (no specific protocol or codes is involved with trait name) that corresponds to the trait descriptor (name). This should exist in 'Trait' sheet."),
      'categories'      => array('req' => FALSE, 'width' => 10, 'desc' => "The values of the categorical descriptor separated by '/'."),
      'data_unit'       => array('req' => FALSE, 'width' => 10, 'desc' => "Unit for the trait descriptor."),
      'minimum'         => array('req' => FALSE, 'width' => 10, 'desc' => "The minimum value of the descriptor."),
      'maximum'         => array('req' => FALSE, 'width' => 10, 'desc' => "The maximum value of the descriptor."),
      'defaultvalue'    => array('req' => FALSE, 'width' => 10, 'desc' => "The default value of the descriptor."),
      'codes'           => array('req' => FALSE, 'width' => 90, 'desc' => "The codes should have pairs of value and label separated by simi-colons. A pair should be separated value and label by a colon."),
      'definition'      => array('req' => FALSE, 'width' => 90, 'desc' => "The definition of the descriptor."),
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
    $cvterms['SITE_CV']['codes']                            = -1;
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

    // Checks format.
    MCL_CHADO_CVTERM::checkCvterm($this, 'BIMS_FIELD_BOOK_FORMAT', $line['format']);

    // Checks categories.
    MCL_CHADO_CVTERM::checkCategories($this, $line['format'], $line['categories']);

    // Checks trait_name.
    MCL_CHADO_CVTERM::checkCvterm($this, MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY'), $line['trait_name']);

    // Checks codes.
    MCL_CHADO_CVTERM::checkCodes($this, $line['codes']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Lists the job paremters.
    $cv_arr = $this->getJob()->getParamByKey('cv_arr');

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Adds a descriptor.
    $descriptor = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv_arr['descriptor']['name'], $line['descriptor_name'], $line['definition']);
    if ($descriptor) {

      // Adds properties.
      $descriptor->addProp($this, 'SITE_CV', 'format', strtolower($line['format']));
      $descriptor->addProp($this, 'SITE_CV', 'codes', $line['codes']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'categories', $line['categories']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'minimum', $line['minimum']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'maximum', $line['maximum']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'defaultValue', $line['defaultValue']);
      $descriptor->addProp($this, 'BIMS_FIELD_BOOK_FORMAT_PROP', 'data_unit', $line['data_unit']);

      // Adds aliases.
      $descriptor->addAlias($this, 'SITE_CV', 'alias', $line['alias'], '[,;]');

      // Links to the trait.
      $trait = MCL_CHADO_CVTERM::getCvterm(MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY'), $line['trait_name']);
      $descriptor->linkTrait($this, $trait);
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Checks db.
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));
  }
}