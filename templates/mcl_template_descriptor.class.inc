<?php
/**
 * The declaration of MCL_TEMPLATE_DESCRIPTOR class.
 *
 */
class MCL_TEMPLATE_DESCRIPTOR extends MCL_TEMPLATE {

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
      'trait_descriptor_set'  => array('req' => FALSE, 'width' => 20, 'desc' => "The name of the trait descriptor set. If the database has one trait descriptor set for all public trait evaluation data, it can be specified in the loader."),
      'descriptor_name'       => array('req' => TRUE,  'width' => 15, 'desc' => "Any trait descriptor that a breeder uses in their own program.  These descriptors would be used as column heads the Cross or Phenotype sheets, with a '#' prefix."),
      'alias'                 => array('req' => FALSE, 'width' => 15, 'desc' => "Alias of the descriptor."),
      'format'                => array('req' => TRUE,  'width' => 15, 'desc' => "The format of the descriptor (numeric, categorical, date, text, etc.)."),
      'trait_name'            => array('req' => FALSE, 'width' => 20, 'desc' => "crop trait ontology term (no specific protocol or code is involved with trait name) that corresponds to the trait descriptor (name). This should exist in 'Trait' sheet."),
      'data_unit'             => array('req' => FALSE, 'width' => 10, 'desc' => "Unit for the trait descriptor."),
      'definition'            => array('req' => FALSE, 'width' => 90, 'desc' => "Definition of the descriptor."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']  = -1;
    $cvterms['SITE_CV']['format'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks cv for trait_descriptor_set.
    MCL_CHADO_CV::checkCV($this, $line['trait_descriptor_set']);

    // Checks trait_name.
    MCL_CHADO_CVTERM::checkCvterm($this, MCL_VAR::getValueByName('SITE_TRAIT_ONTOLOGY'), $line['trait_name']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Gets cv.
    $name = $line['trait_descriptor_set'];
    if (!$name) {
      $name = 'SITE_TRAIT_ONTOLOGY';
    }
    $cv = MCL_CHADO_CV::getCV($name);

    // Adds a descriptor.
    $descriptor = MCL_CHADO_CVTERM::addCvterm($this, $db->getName(), $cv->getName(), $line['descriptor_name'], $line['definition']);
    if ($descriptor) {

      // Adds properties.
      $descriptor->addProp($this, 'SITE_CV', 'format', $line['format']);
      $descriptor->addProp($this, 'SITE_CV', 'data_unit', $line['data_unit']);

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