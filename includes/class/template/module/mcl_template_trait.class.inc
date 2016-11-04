<?php
/**
 * The declaration of MCL_TEMPLATE_TRAIT class.
 *
 */
class MCL_TEMPLATE_TRAIT extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  public $valid_plant_trait_ontologies = array(
    'biochemical trait', 'growth and development trait',
    'anatomy and morphology trait', 'quality trait', 'stature or vigor trait',
    'sterility or fertility trait', 'stress trait', 'yield trait',
  );
//'biological process'

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
    $desc = 'The description for trait sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {;
    $headers = array(
      'trait_name'            => array('req' => TRUE,  'width' => 20, 'desc' => "Trait ontology term for the database or crop. QTLs, MTLs and phenotypic descriptors used in breeding programs can be associated with trait names."),
      'trait_descriptor_set'  => array('req' => TRUE,  'width' => 20, 'desc' => "The name of the crop trait ontology that is stored in cv table of chado (eg. 'rosaceae_trait_ontology' for GDR, 'cotton_trait_ontology' for cotton). In Main Lab, we develop crop trait ontology for Rosaceae, cool season food legumes, cotton and citrus, and map them to the Plant Trait Ontology."),
      'trait_category'        => array('req' => FALSE, 'width' => 20, 'desc' => "The root term of the Plant Tait Ontology for the trait name. Choose one from the list (biochemical trait | growth and development trait | anatomy and morphology trait | quality trait | stature or vigor trait | sterility or fertility trait | stress trait | yield trait)."),
      'abbreviation'          => array('req' => FALSE, 'width' => 14, 'desc' => "Abbreviation of the trait ontology term (trait_name)."),
      'definition'            => array('req' => FALSE, 'width' => 50, 'desc' => "Definition of trait ontology term (trait_name)."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['abbreviation']  = -1;
    $cvterms['relationship']['is_a']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks DB.
    MCL_CHADO_DB::checkDB($this, MCL_VAR::getValueByName('SITE_DB'));

    // Checks trait_descriptor_set
    MCL_CHADO_CV::checkCV($this, $line['trait_descriptor_set']);

    // Checks for trait_category.
    MCL_CHADO_TRAIT::checkCategory($this, $line['trait_category'], $line['trait_descriptor_set']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets db.
    $db = MCL_CHADO_DB::getDB(MCL_VAR::getValueByName('SITE_DB'));

    // Gets trait_descriptor_set.
    $cv = MCL_CHADO_CV::getCV($line['trait_descriptor_set']);

    // Adds a trait.
    $trait = MCL_CHADO_TRAIT::addTrait($this, $db->getName(), $cv->getName(), $line['trait_name'], $line['definition']);
    if ($trait) {

      // Adds properties.
      $trait->addProp($this, 'SITE_CV', 'abbreviation', $line['abbreviation']);

      // Adds categories.
      $trait->addCategory($this, $cv->getName(), $line['trait_category'], $this->cvterms['relationship']['is_a']);
    }
  }
}