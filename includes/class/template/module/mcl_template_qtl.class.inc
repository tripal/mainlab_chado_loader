<?php
/**
 * The declaration of MCL_TEMPLATE_QTL class.
 *
 */
class MCL_TEMPLATE_QTL extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'QTL';
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
    $desc = 'The description for QTL sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'                => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the QTL trait study, the dataset_name should exist in 'dataset_name' column of the 'Dataset' sheet."),
      'qtl_label'                   => array('req' => TRUE,  'width' => 10, 'desc' => "Official unique label of QTL given by the curator. Follow each database's naming convention."),
      'published_symbol'            => array('req' => FALSE, 'width' => 10, 'desc' => "Published Symbol for the QTL."),
      'genus'                       => array('req' => TRUE,  'width' => 10, 'desc' => "The genus of the phenotyping material."),
      'species'                     => array('req' => TRUE,  'width' => 10, 'desc' => "The species of the phenotyping material."),
      'trait_name'                  => array('req' => TRUE,  'width' => 10, 'desc' => "Descriptive name for trait, it should be one of the crop trait ontology and should match a term in 'Trait' sheet."),
      'trait_descriptor_set'        => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the crop trait ontology (entered by the curator who loads the data into the database). If not provided, the site trait ontology will be used."),
      'alias'                       => array('req' => FALSE, 'width' => 10, 'desc' => "Other names for the trait, multiple aliases, separated by a semicolon (;) can be added."),
      'r2'                          => array('req' => FALSE, 'width' => 10, 'desc' => "Fraction of the total variation explained by the QTLs."),
      'lod'                         => array('req' => FALSE, 'width' => 10, 'desc' => "QTL Likelihood of odds."),
      'bayes_factor'                => array('req' => FALSE, 'width' => 10, 'desc' => "Bayes factor."),
      'additive_effect'             => array('req' => FALSE, 'width' => 10, 'desc' => "The average phenotype effect of substituting a q allele for a Q allele."),
      'dominance_effect'            => array('req' => FALSE, 'width' => 10, 'desc' => "The deviation of the heterozygote phenotype from the mean of the homozygotes."),
      'additivity_dominance_ratio'  => array('req' => FALSE, 'width' => 10, 'desc' => "The average phenotype effect of substituting a q allele for a Q allele dominance effect The deviation of the heterozygote phenotype from the mean of the homozygotes."),
      'direction'                   => array('req' => FALSE, 'width' => 10, 'desc' => "	The direction of additive effect on the phenotype."),
      'colocalized_marker'          => array('req' => FALSE, 'width' => 10, 'desc' => "Marker that are mapped within the interval defined by the QTL. Multiple markers can be added, separated by a semicolon (;)."),
      'neighboring_marker'          => array('req' => FALSE, 'width' => 10, 'desc' => "Marker that is mapped in the nearest flanking position outside the interval defined by the QTL. Used to estimate QTL position if no appropriate colocalized markers are found. Multiple markers can be added, separated by a semicolon (;)."),
      'screening_method'            => array('req' => FALSE, 'width' => 10, 'desc' => "Any specific method for the phenotypic inspection."),
      'comments'                    => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments."),
      'mean'                        => array('req' => FALSE, 'width' => 10, 'desc' => "Mean value of the phenotype (if one QTL has multiple mean values based on the condition and/or population, enter them in QTL_trait_Data sheet."),
      'p_value'                     => array('req' => FALSE, 'width' => 10, 'desc' => "p-value."),
      'kw_score'                    => array('req' => FALSE, 'width' => 10, 'desc' => "kw score."),
      'variance'                    => array('req' => FALSE, 'width' => 10, 'desc' => "variance."),
      'population_low_value'        => array('req' => FALSE, 'width' => 10, 'desc' => "Lowest phenotypic value."),
      'population_high_value'       => array('req' => FALSE, 'width' => 10, 'desc' => "Maximum phenotypic value."),
      'reference'                   => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id of the QTL paper."),
    );

    // Adds labels.
    $headers['qtl_label']['label']  = 'QTL_label';
    $headers['r2']['label']         = 'R2';
    $headers['lod']['label']        = 'LOD';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']                      = -1;
    $cvterms['SITE_CV']['published_symbol']           = -1;
    $cvterms['SITE_CV']['r2']                         = -1;
    $cvterms['SITE_CV']['lod']                        = -1;
    $cvterms['SITE_CV']['additive_effect']            = -1;
    $cvterms['SITE_CV']['dominance_effect']           = -1;
    $cvterms['SITE_CV']['additivity_dominance_ratio'] = -1;
    $cvterms['SITE_CV']['direction']                  = -1;
    $cvterms['SITE_CV']['screening_method']           = -1;
    $cvterms['SITE_CV']['clone']                      = -1;
    $cvterms['SITE_CV']['source']                     = -1;
    $cvterms['SITE_CV']['bayes_factor']               = -1;
    $cvterms['SITE_CV']['p_value']                    = -1;
    $cvterms['SITE_CV']['kw_score']                   = -1;
    $cvterms['SITE_CV']['comments']                   = -1;
    $cvterms['SITE_CV']['population_low_value']       = -1;
    $cvterms['SITE_CV']['population_high_value']      = -1;
    $cvterms['sequence']['genetic_marker']            = -1;
    $cvterms['sequence']['qtl']                       = -1;
    $cvterms['relationship']['located_in']            = -1;
    $cvterms['relationship']['adjacent_to']           = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks trait.
    MCL_CHADO_TRAIT::checkTrait($this, $line['trait_name'], $line['trait_descriptor_set']);

    // Checks colocalized_markers.
    MCL_CHADO_FEATURE::checkFeature($this, $line['colocalized_marker'], '', '', $this->cvterms['sequence']['genetic_marker'], '[;,]');

    // Checks neighboring_markers.
    MCL_CHADO_FEATURE::checkFeature($this, $line['neighboring_marker'], '', '', $this->cvterms['sequence']['genetic_marker'], '[;,]');

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Adds or gets QTL.
    $qtl = MCL_CHADO_FEATURE::addFeature($this, $line['qtl_label'], $line['trait_name'], $this->cvterms['sequence']['qtl'], $organism->getOrganismID());
    if ($qtl) {

      // Adds properties.
      $qtl->addProp($this, 'SITE_CV', 'published_symbol', $line['published_symbol']);
      $qtl->addProp($this, 'SITE_CV', 'r2', $line['r2']);
      $qtl->addProp($this, 'SITE_CV', 'lod', $line['lod']);
      $qtl->addProp($this, 'SITE_CV', 'additive_effect', $line['additive_effect']);
      $qtl->addProp($this, 'SITE_CV', 'dominance_effect', $line['dominance_effect']);
      $qtl->addProp($this, 'SITE_CV', 'additivity_dominance_ratio', $line['additivity_dominance_ratio']);
      $qtl->addProp($this, 'SITE_CV', 'direction', $line['direction']);
      $qtl->addProp($this, 'SITE_CV', 'screening_method', $line['screening_method']);
      $qtl->addProp($this, 'SITE_CV', 'comments', $line['comments']);
      $qtl->addProp($this, 'SITE_CV', 'mean', $line['mean']);
      $qtl->addProp($this, 'SITE_CV', 'bayes_factor', $line['bayes_factor']);
      $qtl->addProp($this, 'SITE_CV', 'p_value', $line['p_value']);
      $qtl->addProp($this, 'SITE_CV', 'kw_score', $line['kw_score']);
      $qtl->addProp($this, 'SITE_CV', 'variance', $line['variance']);
      $qtl->addProp($this, 'SITE_CV', 'population_low_value', $line['population_low_value']);
      $qtl->addProp($this, 'SITE_CV', 'population_high_value', $line['population_high_value']);

      // Gets trait.
      $trait = MCL_CHADO_TRAIT::getTrait($line['trait_name'], $line['trait_descriptor_set']);

      /// Adds trait.
      $qtl->addTrait($this, $trait);

      // Adds alias.
      $qtl->addAlias($this, $this->cvterms['SITE_CV']['alias'], $line['alias'], "[,;]");

      // Adds dataset.
      $qtl->addDataset($this, $line['dataset_name']);

      // Adds colocalized_markers.
      $qtl->addRelatedFeatures($this, $line['colocalized_marker'], $this->cvterms['sequence']['genetic_marker'], $this->cvterms['relationship']['adjacent_to'], FALSE, "[,;]");

      // Adds neighboring_markers.
      $qtl->addRelatedFeatures($this, $line['neighboring_marker'], $this->cvterms['sequence']['genetic_marker'], $this->cvterms['relationship']['located_in'], FALSE, "[,;]");

      // Adds stocks.
      $qtl->addStock($this, $line['source'], $line['genus'], $line['species'], "[,;]");

      // Adds references.
      $qtl->addReference($this, $line['reference'], "[,;]");
    }
    else {
      $this->updateMsg('E', "Error : cannot get/add " . $line['qtl_label']);
    }
  }
}