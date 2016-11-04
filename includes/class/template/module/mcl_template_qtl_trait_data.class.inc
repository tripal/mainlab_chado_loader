<?php
/**
 * The declaration of MCL_TEMPLATE_QTL_TRAIT_DATA class.
 *
 */
class MCL_TEMPLATE_QTL_TRAIT_DATA extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
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
    $desc = 'The description for field book sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $dataset_name_desc = "
name of the QTL trait study, the dataset_name should exist in 'dataset_name' column of the 'Dataset' sheet.
  A-B-C-D system<br />
  A: common name of species (scientific name if not available and genus name if cross-species)<br />
  B: trait_name (usually an article report multiple QTL for a trait)<br />
  C: Last name of the corresponding author<br />
  D: publish year";

    // Sets the headers.
    $headers = array(
      'dataset_name'              => array('req' => TRUE,  'width' => 10, 'desc' => $dataset_name_desc),
      'stock_name'                => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the population or parent of the population, The name should exist in the 'stock_name' column of the 'Stock' sheet."),
      'genus'                     => array('req' => TRUE,  'width' => 10, 'desc' => "Genus to which the stock belongs to."),
      'species'                   => array('req' => TRUE,  'width' => 10, 'desc' => "Species name. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'descriptor'                => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the trait. It should exist in 'trait_name' column of the 'Trait' sheet and 'QTL' sheet."),
      'trait_descriptor_set'      => array('req' => TRUE,  'width' => 20, 'desc' => "The name of the crop trait ontology that is stored in cv table of chado (eg. 'rosaceae_trait_ontology' for GDR, 'cotton_trait_ontology' for cotton). In Main Lab, we develop crop trait ontology for Rosaceae, cool season food legumes, cotton and citrus, and map them to the Plant Trait Ontology."),
      'site_name'                 => array('req' => FALSE, 'width' => 10, 'desc' => "Location/environment where the plant was grown. Site_name should exist in 'site_name' column of the 'Site' sheet. If the phenotypic measurement has bee done in multiple years, create multiple site_code even if it is done in the same location."),
      'mean'                      => array('req' => TRUE,  'width' => 10, 'desc' => "Mean value of the traits for each parent and progeny."),
      'standard_deviation'        => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'coefficient_of_variation'  => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'skewness'                  => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'h2'                        => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'cl_90'                     => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'comments'                  => array('req' => FALSE, 'width' => 10, 'desc' => ""),
      'reference'                 => array('req' => FALSE, 'width' => 10, 'desc' => ""),
    );

    // Adds labels.
    $headers['cl_90']['label'] = 'Cl_90';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['phenotyping']              = -1;
    $cvterms['SITE_CV']['statistical']              = -1;
    $cvterms['SITE_CV']['standard_deviation']       = -1;
    $cvterms['SITE_CV']['coefficient_of_variation'] = -1;
    $cvterms['SITE_CV']['skewness']                 = -1;
    $cvterms['SITE_CV']['h2']                       = -1;
    $cvterms['SITE_CV']['cl_90']                    = -1;
    $cvterms['SITE_CV']['comments']                 = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Cehcks descriptor.
    MCL_CHADO_CVTERM::checkCvterm($this, $line['trait_descriptor_set'], $line['descriptor']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);

    // Checks site.
    MCL_CHADO_LOCATION::checkSite($this, $line['site_name']);

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the descriptor.
    $descriptor = MCL_CHADO_CVTERM::getCvterm($line['trait_descriptor_set'], $line['descriptor']);

    // Gets the organims.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Gets the stock.
    $args = array(
      'uniquename' => $line['stock_name'],
      'organism_id' => $organism->getOrganismID(),
    );
    $stock = MCL_CHADO_STOCK::byKey($args);

    // Gets nd_geolocation_id.
    $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite($line['site_name']);

    // Adds a phenotype (mean).
    $uniquename = $line['descriptor'] . '_' . $line['mean'];
    $phenotype = MCL_CHADO_PHENOTYPE::addPhenotype($this, $uniquename, $descriptor->getCvtermID(), $line['mean']);

    // Adds a nd_experiment.
    $value = $line['dataset_name'] . '_' . $line['site_name'] . '_' . $line['stock_name'] . '_' . $descriptor->getCvtermID();
    $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['statistical'], $nd_geolocation_id, $this->cvterms['SITE_CV']['statistical'], $value);

    // Adds properties.
    $nd_experiment->addProp($this, 'SITE_CV', 'standard_deviation', $line['standard_deviation']);
    $nd_experiment->addProp($this, 'SITE_CV', 'coefficient_of_variation', $line['coefficient_of_variation']);
    $nd_experiment->addProp($this, 'SITE_CV', 'skewness', $line['skewness']);
    $nd_experiment->addProp($this, 'SITE_CV', 'h2', $line['h2']);
    $nd_experiment->addProp($this, 'SITE_CV', 'cl_90', $line['cl_90']);
    $nd_experiment->addProp($this, 'SITE_CV', 'comments', $line['comments']);

    // Adds a phenotype.
    $nd_experiment->addPhenotype($this, $phenotype);

    // Adds a stock.
    $nd_experiment->addStock($this, $stock, $stock->getTypeID());

    // Adds a dataset.
    $nd_experiment->addDataset($this, $line['dataset_name']);

    // Adds references.
    $nd_experiment->addReference($this, $line['references'], "[,;]");
  }
}