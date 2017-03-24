<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_SSR class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_SSR extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'GENOTYPE';
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
      'dataset_name'  => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'stock_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock genotyped. It should match an 'stock_name' of the 'Stock' sheet."),
      'genus'         => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the stock"),
      'species'       => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the stock"),
      'sample_id'     => array('req' => TRUE,  'width' => 10, 'desc' => "ID of a sample. Normally it is the same as ID unless the user want to keep tract of genotype of a specific sample."),
      'clone_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a clone if known."),
    );

    // Adds labels.
    $headers['clone_id']['label']   = 'clone_ID';
    $headers['sample_id']['label']  = 'sample_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['sample']           = -1;
    $cvterms['SITE_CV']['clone']            = -1;
    $cvterms['sequence']['genetic_marker']  = -1;
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

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);


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







  }
}