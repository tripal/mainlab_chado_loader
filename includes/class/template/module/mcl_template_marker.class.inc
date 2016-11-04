<?php
/**
 * The declaration of MCL_TEMPLATE_MARKER class.
 *
 */
class MCL_TEMPLATE_MARKER extends MCL_TEMPLATE {

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
    $desc = 'The description for marker sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'marker_name'               => array('req' => TRUE,  'width' => 10, 'desc' =>"Name of the marker."),
      'marker_type'               => array('req' => FALSE, 'width' => 10, 'desc' =>"Marker type such as SSR, RFLP, RAPD, STS, SCAR, overgo probe and Isozyme. Refer to the available marker_type page for each database."),
      'genus'                     => array('req' => TRUE,  'width' => 10, 'desc' =>"The genus name of the organism from which the marker was developed."),
      'species'                   => array('req' => TRUE,  'width' => 10, 'desc' =>"The species name of the organism from which the marker was developed. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'alias'                     => array('req' => FALSE, 'width' => 10, 'desc' =>"Other names used for the marker. Multiple aliases, separated by a semicolon (;), can be added."),
      'marker_sequence'           => array('req' => FALSE, 'width' => 10, 'desc' =>"Sequence of the marker when available. For SNP, it should be in the format of 5'flanking sequence[A/T] 3'flanking sequence."),
      'gb_id'                     => array('req' => FALSE, 'width' => 10, 'desc' =>"Associated GenBank ID of the marker or the source of the marker (eg. EST)."),
      'dbsnp_id'                  => array('req' => FALSE, 'width' => 10, 'desc' =>"Associated dbSNP_ID. #ss number should be used."),
      'five_prime_flanking_seq'   => array('req' => FALSE, 'width' => 10, 'desc' =>"5' flanking sequence of SNP or Indel."),
      'three_prime_flanking_seq'  => array('req' => FALSE, 'width' => 10, 'desc' =>"3' flanking sequence of SNP or Indel."),
      'shared_species'            => array('req' => FALSE, 'width' => 10, 'desc' =>"Species names of organisms that have been found to have the same SNP. If there are more than one other species, using ';' to separate."),
      'repeat_motif'              => array('req' => FALSE, 'width' => 10, 'desc' =>"Repeat motif for SSR markers."),
      'snp_chip_name'             => array('req' => FALSE, 'width' => 10, 'desc' => "SNP chip name when SNPs belong to a chip (the name should match 'name' in Library sheet."),
      'snp_chip_id'               => array('req' => FALSE, 'width' => 10, 'desc' => "SNP chip ID if there is a specific ID for the specific SNP chip entered in 'SNP_chip_ID'."),
      'restriction_enzyme'        => array('req' => FALSE, 'width' => 10, 'desc' =>"Restriction Enzyme used for CAPS marker or other markers that use restriction enzymes."),
      'product_length'            => array('req' => FALSE, 'width' => 10, 'desc' =>"The PCR product length from the source species of the marker."),
      'max_length'                => array('req' => FALSE, 'width' => 10, 'desc' =>"The maximum product size that have been observed."),
      'min_length'                => array('req' => FALSE, 'width' => 10, 'desc' =>"The minimum product size that have been observed."),
      'is_codominant'             => array('req' => FALSE, 'width' => 10, 'desc' =>"Yes if the marker is codominant, No otherwise."),
      'pcr_condition'             => array('req' => FALSE, 'width' => 10, 'desc' =>"PCR condition for PCR-based markers."),
      'screening_method'          => array('req' => FALSE, 'width' => 10, 'desc' =>"Gel type, % etc (eg. 2% agarose) for PCR product visualization and any other screening methods for other types of markers."),
      'monomorphic_population'    => array('req' => FALSE, 'width' => 10, 'desc' =>"The name of the mapping population of which the marker was monomorphic."),
      'source_marker_name'        => array('req' => FALSE, 'width' => 10, 'desc' =>"The name of the marker from which the marker is generated (STS, SSR or SCAR markers are often generated from other markers). The detail of the source marker should be given in a separate row."),
      'source_description'        => array('req' => FALSE, 'width' => 10, 'desc' =>"Whether the marker was developed from the sequence of EST, BAC, cDNA, genomic clone, or WGS."),
      'indel_seq'                 => array('req' => FALSE, 'width' => 10, 'desc' =>"sequences of indel (eg. ATT)."),
      'alleles'                   => array('req' => FALSE, 'width' => 10, 'desc' =>"Alleles for the SNP marker (eg. A/T)."),
      'gel_picture_file_name'     => array('req' => FALSE, 'width' => 10, 'desc' =>"File name of the gel picture if available."),
      'gel_picture_legend'        => array('req' => FALSE, 'width' => 10, 'desc' =>"Legend of the gel picture if available."),
      'comments'                  => array('req' => FALSE, 'width' => 10, 'desc' =>"Any comments about the marker."),
      'variety'                   => array('req' => FALSE, 'width' => 10, 'desc' =>"The variety from which the marker was developed. It should match a 'stock_name' in the Stock sheet."),
      'image'                     => array('req' => FALSE, 'width' => 10, 'desc' =>"Image_ID of any associated image (Detailed info in 'Image' sheet.) Multiple images, separated by comma, can be added."),
      'contact'                   => array('req' => FALSE, 'width' => 10, 'desc' =>"Contact person for the marker (Please provide the contact info of these researchers in 'Contact' sheet)."),
      'reference'                 => array('req' => FALSE, 'width' => 10, 'desc' =>"Reference where the marker is originally described (pub_id), Multiple references, separated by comma, can be added."),
    );

    // Adds labels.
    $headers['gb_id']['label']          = 'GB_ID';
    $headers['dbsnp_id']['label']       = 'dbSNP_ID';
    $headers['pcr_condition']['label']  = 'PCR_condition';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']                        = -1;
    $cvterms['SITE_CV']['marker_type']                  = -1;
    $cvterms['SITE_CV']['restriction_enzyme']           = -1;
    $cvterms['SITE_CV']['product_length']               = -1;
    $cvterms['SITE_CV']['max_length']                   = -1;
    $cvterms['SITE_CV']['min_length']                   = -1;
    $cvterms['SITE_CV']['is_codominant']                = -1;
    $cvterms['SITE_CV']['repeat_motif']                 = -1;
    $cvterms['SITE_CV']['source']                       = -1;
    $cvterms['SITE_CV']['pcr_condition']                = -1;
    $cvterms['SITE_CV']['screening_method']             = -1;
    $cvterms['SITE_CV']['monomorphic_population']       = -1;
    $cvterms['SITE_CV']['filename']                     = -1;
    $cvterms['SITE_CV']['indel_seq']                    = -1;
    $cvterms['SITE_CV']['legend']                       = -1;
    $cvterms['SITE_CV']['comments']                     = -1;
    $cvterms['SITE_CV']['SNP_chip']                     = -1;
    $cvterms['sequence']['genetic_marker']              = -1;
    $cvterms['sequence']['primer']                      = -1;
    $cvterms['sequence']['probe']                       = -1;
    $cvterms['sequence']['associated_with']             = -1;
    $cvterms['sequence']['five_prime_flanking_region']  = -1;
    $cvterms['sequence']['three_prime_flanking_region'] = -1;
    $cvterms['sequence']['allele']                      = -1;
    $cvterms['sequence']['SNP']                         = -1;
    $cvterms['relationship']['adjacent_to']             = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks db.
    MCL_CHADO_DB::checkDB($this, 'dbSNP');
    MCL_CHADO_DB::checkDB($this, 'DB:genbank');

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks SNP_chip_name.
//    MCL_CHADO_LIBRARY::checkLibrary($this, $line['snp_chip']);

    // Checks contacts.
    MCL_CHADO_CONTACT::checkContact($this, $line['contact'], '[;,]');

    // Checks images.
    MCL_CHADO_IMAGE::checkImage($this, $line['image'], '[;,]');

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Adds a marker.
    $marker = MCL_CHADO_FEATURE::addFeature($this, $line['marker_name'], $line['marker_name'], $this->cvterms['sequence']['genetic_marker'], $organism->getOrganismID());
    if ($marker) {

      // Updates feature.residues.
      $marker_sequence          = $line['marker_sequence'];
      $gb_id                    = $line['gb_id'];
      $five_prime_flanking_seq  = $line['five_prime_flanking_seq'];
      $three_prime_flanking_seq = $line['three_prime_flanking_seq'];
      $alleles                  = $line['alleles'];

      // Sets the marker_sequence to the residues.
      $residues = '';
      if ($marker_sequence) {
        $residues = $marker_sequence;
      }

      // Gets the sequence from GenBank.
      else if ($gb_id) {
        $residues = '';
      }

      // Concatenates five_prime_flanking_seq, alleles and three_prime_flanking_seq.
      else if ($five_prime_flanking_seq && $alleles && $three_prime_flanking_seq) {
        $residues = $five_prime_flanking_seq . $alleles . $three_prime_flanking_seq;
      }
      if ($residues) {
        $marker->setResidues($residues);
        $marker->update();
      }

      // Adds properties.
      $marker->addProp($this, 'SITE_CV', 'marker_type', $line['marker_type']);
      $marker->addProp($this, 'SITE_CV', 'repeat_motif', $line['repeat_motif']);
      $marker->addProp($this, 'SITE_CV', 'restriction_enzyme', $line['restriction_enzyme']);
      $marker->addProp($this, 'SITE_CV', 'product_length', $line['product_length']);
      $marker->addProp($this, 'SITE_CV', 'max_length', $line['max_length']);
      $marker->addProp($this, 'SITE_CV', 'min_length', $line['min_length']);
      $marker->addProp($this, 'SITE_CV', 'is_codominant', $line['is_codominant']);
      $marker->addProp($this, 'SITE_CV', 'pcr_condition', $line['pcr_condition']);
      $marker->addProp($this, 'SITE_CV', 'screening_method', $line['screening_method']);
      $marker->addProp($this, 'SITE_CV', 'monomorphic_population', $line['monomorphic_population']);
      $marker->addProp($this, 'SITE_CV', 'source', $line['source']);
      $marker->addProp($this, 'SITE_CV', 'filename', $line['filename']);
      $marker->addProp($this, 'SITE_CV', 'indel_seq', $line['indel_seq']);
      $marker->addProp($this, 'SITE_CV', 'legend', $line['legend']);
      $marker->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds prime flanking sequences.
      $marker->addProp($this, 'sequence', 'five_prime_flanking_region', $line['five_prime_flanking_seq']);
      $marker->addProp($this, 'sequence', 'three_prime_flanking_region', $line['three_prime_flanking_seq']);

      // Adds alleles.
      $type = ($line['marker_type'] == 'snp') ? 'SNP' : 'allele';
      $marker->addProp($this, 'SITE_CV', $type, $line['alleles']);

      // Adds aliases.
      $marker->addAlias($this, $this->cvterms['SITE_CV']['alias'], $line['alias'], "[;,]");

      // Adds gb_id (accession).
      $db = MCL_CHADO_DB::getDB('DB:genbank');
      $marker->addDBXref($this, $db->getDbID(), $line['gb_id'], "[;,]");

      // Adds dbsnp_id.
      $db = MCL_CHADO_DB::getDB('dbSNP');
      $marker->addDBXref($this, $db->getDbID(), $line['dbsnp_id'], "[;,]");

      // Adds varieties.
      $marker->addStock($this, $line['variety'], $line['genus'], $line['species'], "[;,]");

      // Adds contacts.
      $marker->addContact($this, $line['contact'], "[;,]");

      // Adds images.
      $marker->addImage($this, $line['image'], "[;,]");

      // Creates and adds snp_chip_id.
      $library = MCL_CHADO_LIBRARY::getLibrary($line['snp_chip_name'], $this->cvterms['SITE_CV']['SNP_chip']);
      if ($library) {

        // Adds snp_chip_id as a synonym.
        $synonym = MCL_CHADO_SYNONYM::addSynonym($this, $line['snp_chip_id'], $this->cvterms['SITE_CV']['SNP_chip']);

        // Adds the synonym to marker and library.
        if ($synonym) {
          $marker->addSynonym($this, $synonym);
          $library->addSynonym($this, $synonym);
        }
       }

      // Adds references.
      $marker->addReference($this, $line['reference'], "[;,]");

      // Adds primer sequences.
      $marker->addSequence($this, $line, '~primer', $this->cvterms['sequence']['primer'], $this->cvterms['relationship']['adjacent_to']);

      // Adds probe sequences.
      $marker->addSequence($this, $line, '~probe', $this->cvterms['sequence']['probe'], $this->cvterms['sequence']['associated_with']);
    }
  }
}