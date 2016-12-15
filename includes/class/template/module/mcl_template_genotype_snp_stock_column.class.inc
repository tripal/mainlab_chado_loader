<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_SNP_STOCK_COLUMN class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_SNP_STOCK_COLUMN extends MCL_TEMPLATE {

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
    $desc = "
      This is the template to upload SNP genotyping data. The stock names are listed as headers with prefix '$'.
      There are no place to specify the organism for the stocks. So this template is used only when stock names
      are absolutely unique in the stock table.
    ";
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'  => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'marker_name'   => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker genotyped. It should match an 'marker_name' of the 'Marker' sheet."),
      'genus'         => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the marker"),
      'species'       => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the marker"),
      '$stock_name'   => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns ($) : followed by stock name"),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['sequence']['genetic_marker']  = -1;
    $cvterms['sequence']['SNP']             = -1;
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

    // Checks markers.
    MCL_CHADO_FEATURE::checkFeature($this, $line['marker_name'], $line['genus'], $line['species'], $this->cvterms['sequence']['genetic_marker']);

    // Checks stocks.
    $this->_checkStocks($line);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the dataset.
    $dataset = MCL_CHADO_DATASET::getDataset($line['dataset_name']);

    // Gets the marker.
    $marker = MCL_CHADO_FEATURE::getFeature($line['marker_name'], $line['genus'], $line['species'], $this->cvterms['sequence']['genetic_marker']);

    // Adds genotype_call for each stock.
    $stock_cols = $this->getJob()->getParamByKey('stock_cols');
    foreach ($stock_cols as $stock_name => $stock_id) {

      // Adds genotype.
      $genotype_value = $line['$' . $stock_name];
      $genotype = MCL_CHADO_GENOTYPE::addGenotype($this, $genotype_value, $this->cvterms['sequence']['SNP'], $genotype_value);
      if ($genotype) {

        // Adds a SNP into genotype_call.
        $details = array(
          'project_id'  => $dataset->getProjectID(),
          'stock_id'    => $stock_id,
          'feature_id'  => $marker->getFeatureID(),
          'genotype_id' => $genotype->getGenotypeID(),
        );
        MCL_CHADO_GENOTYPE_CALL::addGenotypeCall($this, $details);
      }
    }
  }

  /**
   * Checks the stock columns for stock_id. Save them in job.param.
   *
   * @param array $line
   */
  private function _checkStocks($line) {

    // This should be called once.
    if ($line['line_no'] != 3) {
      return;
    }

    // Gets stock_id for the stocks.
    $stocks = array();
    foreach ($line as $key => $value) {
      if (preg_match("/^\\$(.*)$/", $key, $matches)) {
        $stock_name = $matches[1];
        $stock = MCL_CHADO_STOCK::getStock($stock_name);
        if ($stock) {
          $stocks[$stock_name] = $stock->getStockID();
        }
        else {
          $this->updateMsg('E', "$stock_name not found in stock");
        }
      }
    }
    $this->getJob()->setParamByKey('stock_cols', $stocks);
    $this->getJob()->update();
  }
}