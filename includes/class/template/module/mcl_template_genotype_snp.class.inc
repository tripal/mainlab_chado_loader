<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_SNP class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_SNP extends MCL_TEMPLATE {

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
    $desc = 'The description for genotype sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE, 'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'stock_name'      => array('req' => TRUE, 'width' => 10, 'desc' => "Name of the stock genotyped. It should match an 'stock_name' of the 'Stock' sheet."),
      'stock_genus'     => array('req' => TRUE, 'width' => 10, 'desc' => "Genus of the stock"),
      'stockr_species'  => array('req' => TRUE, 'width' => 10, 'desc' => "Species of the stock"),
      'marker'          => array('req' => TRUE, 'width' => 10, 'desc' => "Name of the marker used for genotyping. It should match a marker_name in the 'Marker' sheet."),
      'marker_genus'    => array('req' => TRUE, 'width' => 10, 'desc' => "Genus of the marker"),
      'marker_species'  => array('req' => TRUE, 'width' => 10, 'desc' => "Species of the marker"),
      'genotype'        => array('req' => TRUE, 'width' => 10, 'desc' => ""),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['SNP'] = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks organisms.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['marker_genus'], $line['marker_species']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);

    // Checks marker.
    MCL_CHADO_FEATURE::checkFeature($this, $line['marker'], $line['marker_genus'], $line['marker_species'], $this->cvterms['sequence']['genetic_marker']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the dataset.
    $dataset = MCL_CHADO_DATASET::getDataset($line['dataset']);

    // Gets the organims.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Gets the stock.
    $stock = MCL_CHADO_STOCK::getStock($line['stock_name'], $line['genus'], $line['species']);

    // Gets the marker.
    $marker = MCL_CHADO_FEATURE::getFeature($line['marker'], $line['marker_genus'], $line['marker_species'], $this->cvterms['sequence']['genetic_marker']);

    // Adds a new genotype or gets the genotype if exists.
    $genotype = MCL_CHADO_GENOTYPE::addGenotype($line['genotype'], $this->cvterms['sequence']['SNP'], $line['genotype']);

    // Adds a SNP genotype.
    $details = array(
      'project_id'  => $dataset->getProjectID(),
      'stock_id'    => $stock->getStockID(),
      'feature_id'  => $marker->getFeatureID(),
      'genotype_id' => $genotype->getGenotypeID(),
    );
//    $snp_genotype = new CHADO_GENOTYPE_CALL($details);
//    $snp_genotype->insert();
  }
}