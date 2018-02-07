<?php
/**
 * The declaration of MCL_TEMPLATE_ORGANISM class.
 *
 */
class MCL_TEMPLATE_ORGANISM extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'ORGANISM';
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
    $desc = 'The description for db sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'genus'                         => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the organism."),
      'species'                       => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the organism."),
      'family'                        => array('req' => FALSE, 'width' => 10, 'desc' => "Family of the organism."),
      'geographic_origin'             => array('req' => FALSE, 'width' => 10, 'desc' => "Geographic origin of the organism."),
      'genome_group'                  => array('req' => FALSE, 'width' => 10, 'desc' => "Genome group of the organism."),
      'haploid_chromosome_number'     => array('req' => FALSE, 'width' => 10, 'desc' => "Haploid chromosome number of the organism."),
      'genome_size'                   => array('req' => FALSE, 'width' => 10, 'desc' => "Genome size of the organism."),
      'ploidy'                        => array('req' => FALSE, 'width' => 10, 'desc' => "Ploidy of the organism."),
      'growth_habit'                  => array('req' => FALSE, 'width' => 10, 'desc' => "Growth Habit of the organism."),
      'propagation_method'            => array('req' => FALSE, 'width' => 10, 'desc' => "Propagation method of the organism."),
      'usage'                         => array('req' => FALSE, 'width' => 10, 'desc' => "Usage of the organism."),
      'hybrid_parentage'              => array('req' => FALSE, 'width' => 10, 'desc' => "Hybrid parentage of the organism."),
      'resistance_to_biotic_stress'   => array('req' => FALSE, 'width' => 10, 'desc' => "Resistance to biotic stress of the organism."),
      'resistance_to_abiotic_stress'  => array('req' => FALSE, 'width' => 10, 'desc' => "Resistance to abiotic stress of the organism."),
      'alias_scientific'              => array('req' => FALSE, 'width' => 10, 'desc' => "Alias scientific of the organism."),
      'alias_synonym'                 => array('req' => FALSE, 'width' => 10, 'desc' => "alias_synonym of the organism."),
      'alias_common'                  => array('req' => FALSE, 'width' => 10, 'desc' => "alias common of the organism."),
      'fertile_with'                  => array('req' => FALSE, 'width' => 10, 'desc' => "fertile with the organism."),
      'sterile_with'                  => array('req' => FALSE, 'width' => 10, 'desc' => "sterile with the organism."),
      'incompatible_with'             => array('req' => FALSE, 'width' => 10, 'desc' => "incompatible with the organism."),
      'grin_id'                       => array('req' => FALSE, 'width' => 10, 'desc' => "GRIN taxonomy ID of the organism."),
      'image'                         => array('req' => FALSE, 'width' => 10, 'desc' => "Image of the organism."),
      'reference'                     => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id if any publication is associated with the organism."),
      'comments'                      => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the organism."),
    );

    // Adds labels.
    $headers['grin_id']['label'] = 'GRIN Taxonomy ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['family']                       = -1;
    $cvterms['SITE_CV']['geographic_origin']            = -1;
    $cvterms['SITE_CV']['genome_group']                 = -1;
    $cvterms['SITE_CV']['haploid_chromosome_number']    = -1;
    $cvterms['SITE_CV']['genome_size']                  = -1;
    $cvterms['SITE_CV']['ploidy']                       = -1;
    $cvterms['SITE_CV']['growth_habit']                 = -1;
    $cvterms['SITE_CV']['propagation_method']           = -1;
    $cvterms['SITE_CV']['usage']                        = -1;
    $cvterms['SITE_CV']['hybrid_parentage']             = -1;
    $cvterms['SITE_CV']['resistance_to_biotic_stress']  = -1;
    $cvterms['SITE_CV']['resistance_to_abiotic_stress'] = -1;
    $cvterms['SITE_CV']['alias_scientific']             = -1;
    $cvterms['SITE_CV']['alias_synonym']                = -1;
    $cvterms['SITE_CV']['alias_common']                 = -1;
    $cvterms['SITE_CV']['comments']                     = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks DB for GRIN.
    MCL_CHADO_DB::checkDB($this, 'GRIN');

    // Checks images.
    MCL_CHADO_IMAGE::checkImageID($this, $line['image'], '[;,]');

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds an organism.
    $organism = MCL_CHADO_ORGANISM::addOrganism($this, $line['genus'], $line['species']);
    if ($organism) {

      // Adds properties.
      $organism->addProp($this, 'SITE_CV', 'family', $line['family']);
      $organism->addProp($this, 'SITE_CV', 'geographic_origin', $line['geographic_origin']);
      $organism->addProp($this, 'SITE_CV', 'genome_group', $line['genome_group']);
      $organism->addProp($this, 'SITE_CV', 'haploid_chromosome_number', $line['haploid_chromosome_number']);
      $organism->addProp($this, 'SITE_CV', 'genome_size', $line['genome_size']);
      $organism->addProp($this, 'SITE_CV', 'ploidy', $line['ploidy']);
      $organism->addProp($this, 'SITE_CV', 'growth_habit', $line['growth_habit']);
      $organism->addProp($this, 'SITE_CV', 'propagation_method', $line['propagation_method']);
      $organism->addProp($this, 'SITE_CV', 'usage', $line['usage']);
      $organism->addProp($this, 'SITE_CV', 'hybrid_parentage', $line['hybrid_parentage']);
      $organism->addProp($this, 'SITE_CV', 'resistance_to_biotic_stress', $line['resistance_to_biotic_stress']);
      $organism->addProp($this, 'SITE_CV', 'resistance_to_abiotic_stress', $line['resistance_to_abiotic_stress']);
      $organism->addProp($this, 'SITE_CV', 'alias_scientific', $line['alias_scientific'], "[;,]");
      $organism->addProp($this, 'SITE_CV', 'alias_synonym', $line['alias_synonym'], "[;,]");
      $organism->addProp($this, 'SITE_CV', 'alias_common', $line['alias_common'], "[;,]");

      // Adds related organisms.
      //$organism->addRelatedOrganism($this, 'SITE_CV', 'fertile_with', $line['fertile_with'], "[;,]");
      //$organism->addRelatedOrganism($this, 'SITE_CV', 'sterile_with', $line['sterile_with'], "[;,]");
      //$organism->addRelatedOrganism($this, 'SITE_CV', 'sterile_with', $line['incompatible_with'], "[;,]");

      // Gets db of GRIN and add a dbxref.
      $db = MCL_CHADO_DB::getDB('GRIN');
      $dbxref = MCL_CHADO_DBXREF::addDBXref($this, $db->getDbID(), $line['grin_id']);
      if ($dbxref) {

        // Adds dbxref to organism_dbxref table.
        $organism->addDBXref($this, $dbxref);
      }

      // Adds images.
      $organism->addImageID($this, $line['image'], "[,;]");

      // Adds references.
      $organism->addReference($this, $line['reference'], "[,;]");
    }
  }
}