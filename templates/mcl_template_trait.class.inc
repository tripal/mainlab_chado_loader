<?php
/**
 * The declaration of MCL_TEMPLATE_TRAIT class.
 *
 */
class MCL_TEMPLATE_TRAIT extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'TRAIT';
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
      'trait_name'      => array('req' => TRUE,  'width' => 20, 'desc' => "Trait ontology term for the database or crop. QTLs, MTLs and phenotypic descriptors used in breeding programs can be associated with trait names."),
      'trait_category'  => array('req' => FALSE, 'width' => 20, 'desc' => "The root term of the Plant Trait Ontology for the trait name. Choose one from the list (biochemical trait | growth and development trait | anatomy and morphology trait | quality trait | stature or vigor trait | sterility or fertility trait | stress trait | yield trait)."),
      'abbreviation'    => array('req' => FALSE, 'width' => 14, 'desc' => "Abbreviation of the trait ontology term (trait_name)."),
      'definition'      => array('req' => FALSE, 'width' => 50, 'desc' => "Definition of trait ontology term (trait_name)."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['abbreviation'] = -1;
    $cvterms['relationship']['is_a']    = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks for the trait category.
    if (!MCL_DATA_VALID_TYPE::validate('trait_category', $line['trait_category'])) {
      $this->updateMsg('E', 'Invalid trait category : ' . $line['trait_category']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a trait.
    $trait = MCL_CHADO_TRAIT::addTrait($this, $line['trait_name'], $line['definition']);
    if ($trait) {

      // Adds properties.
      $trait->addProp($this, 'SITE_CV', 'abbreviation', $line['abbreviation']);

      // Adds trait categories.
      $trait->addCategory($this, $line['trait_category'], $this->cvterms['relationship']['is_a'], "[,;]");
    }
  }
}