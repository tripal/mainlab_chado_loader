<?php
/**
 * The declaration of MCL_TEMPLATE_MTL class.
 *
 */
class MCL_TEMPLATE_MTL extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MTL';
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
    $desc = 'The description for MTL sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'mtl_name'              => array('req' => TRUE,  'width' => 10, 'desc' => "Trait locus name given by curators."),
      'genus'                 => array('req' => TRUE,  'width' => 10, 'desc' => "The genus of the phenotyping material."),
      'species'               => array('req' => TRUE,  'width' => 10, 'desc' => "The species of the phenotyping material."),
      'trait_name'            => array('req' => FALSE, 'width' => 10, 'desc' => "Descriptive name for trait, it should be one of the crop trait ontology and should match a term in 'Trait' sheet."),
      'trait_descriptor_set'  => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the crop trait ontology. A site-wide crop trait ontology can be specified in the loader. If not provided, the site trait ontology will be used."),
      'gb_id'                 => array('req' => FALSE, 'width' => 10, 'desc' => "Associated genbank ID if available."),
      'alias'                 => array('req' => FALSE, 'width' => 10, 'desc' => "Other names used for the trait. Multiple aliases, separated by comma, can be added."),
      'screening_method'      => array('req' => FALSE, 'width' => 10, 'desc' => "Any specific method for the phenotypic inspection."),
      'description'           => array('req' => FALSE, 'width' => 10, 'desc' => "Desctiption for the trait. The more detail, the better. You can describe the phenotype of the various cultivar and provide the pictures."),
      'published_symbol'      => array('req' => FALSE, 'width' => 10, 'desc' => "Published symbol for the MTL."),
      'colocalized_marker'    => array('req' => FALSE, 'width' => 10, 'desc' => "Marker locus that colocalizes with the MTL."),
      'neighboring_marker'    => array('req' => FALSE, 'width' => 10, 'desc' => "Marker locus that is mapped near MTL. Multiple loci can be added, separated by a semicolon (;)."),
      'contact'               => array('req' => FALSE, 'width' => 10, 'desc' => "Contact person for the MTL. It should match 'contact_name' in Contact sheet."),
      'image'                 => array('req' => FALSE, 'width' => 10, 'desc' => "Image_ID of any associated image (Detailed info in 'Image' sheet.) Multiple images, separated by comma, can be added."),
      'reference'             => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id for the reference that describes MTL. Multiple references, separated by comma, can be added."),
      '##property_name'       => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );

    // Adds labels.
    $headers['mtl_name']['label'] = 'MTL_name';
    $headers['gb_id']['label']    = 'GB_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']                        = -1;
    $cvterms['SITE_CV']['screening_method']             = -1;
    $cvterms['SITE_CV']['description']                  = -1;
    $cvterms['SITE_CV']['published_symbol']             = -1;
    $cvterms['sequence']['marker_locus']                = -1;
    $cvterms['sequence']['heritable_phenotypic_marker'] = -1;
    $cvterms['relationship']['located_in']              = -1;
    $cvterms['relationship']['adjacent_to']             = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks DB for DB:genbank.
    MCL_CHADO_DB::checkDB($this, 'DB:genbank');

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks trait.
    MCL_CHADO_TRAIT::checkTrait($this, $line['trait_name'], $line['trait_descriptor_set']);

    // Checks colocalized_markers.
//    MCL_CHADO_FEATURE::checkFeature($this, $line['colocalized_marker'], $line['genus'], $line['species'], $this->cvterms['sequence']['marker_locus'], '[;,]');

    // Checks neighboring_markers.
//    MCL_CHADO_FEATURE::checkFeature($this, $line['neighboring_marker'], $line['genus'], $line['species'], $this->cvterms['sequence']['marker_locus'], '[;,]');

    // Checks images.
    MCL_CHADO_IMAGE::checkImageID($this, $line['image'], '[;,]');

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Adds a MTL.
    $mtl = MCL_CHADO_FEATURE::addFeature($this, $line['mtl_name'], $line['trait_name'], $this->cvterms['sequence']['heritable_phenotypic_marker'], $organism->getOrganismID());
    if ($mtl) {

      // Adds the properties.
      $mtl->addProp($this, 'SITE_CV', 'alias', $line['alias']);
      $mtl->addProp($this, 'SITE_CV', 'screening_method', $line['screening_method']);
      $mtl->addProp($this, 'SITE_CV', 'description', $line['description']);
      $mtl->addProp($this, 'SITE_CV', 'published_symbol', $line['published_symbol']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $mtl, $line);

      // Adds colocalized_markers.
      $mtl->addRelatedFeatures($this, $line['colocalized_marker'], $this->cvterms['sequence']['marker_locus'], FALSE, $this->cvterms['relationship']['adjacent_to'], "[,;]", TRUE);

      // Adds neighboring_markers.
      $mtl->addRelatedFeatures($this, $line['neighboring_marker'], $this->cvterms['sequence']['marker_locus'], FALSE, $this->cvterms['relationship']['located_in'], "[,;]", TRUE);

      // Adds dbxref.
      $db = MCL_CHADO_DB::getDB('DB:genbank');
      $mtl->addDBXref($this, $db->getDbID(), $line['gb_id'], "[,;]");

      // Gets trait.
      $trait = MCL_CHADO_TRAIT::getTrait($line['trait_name'], $line['trait_descriptor_set']);

      /// Adds a trait.
      $mtl->addTrait($this, $trait);

      // Adds contacts.
      $mtl->addContact($this, $line['contact'], "[;]");

      // Adds images.
      $mtl->addImageID($this, $line['image'], "[,;]");

      // Adds references.
      $mtl->addReference($this, $line['reference'], "[,;]");
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($this->getJob()->getParamByKey('cv'), 'SITE_CV');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $data_lines[0])) {
      return;
    }
  }
}