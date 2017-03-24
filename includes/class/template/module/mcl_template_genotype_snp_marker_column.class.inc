<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_SNP_MARKER_COLUMN class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_SNP_MARKER_COLUMN extends MCL_TEMPLATE {

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
      This is the template to upload SNP genotyping data. The marker names are listed as headers with prefix '$'.
      There are no place to specify the organism for the markers. So this template is used only when marker names
      are absolutely unique in the feature table.
    ";
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'  => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'stock_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock genotyped. It should match an 'stock_name' of the 'Stock' sheet."),
      'genus'         => array('req' => FALSE, 'width' => 10, 'desc' => "Genus of the stock"),
      'species'       => array('req' => FALSE, 'width' => 10, 'desc' => "Species of the stock"),
      '$marker_name'  => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns ($) : followed by marker name"),
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

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);

    // Checks markers.
    $this->_checkMarkers($line);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the dataset.
    $dataset = MCL_CHADO_DATASET::getDataset($line['dataset_name']);

    // Gets the stock.
    $stock = MCL_CHADO_STOCK::getStock($line['stock_name'], $line['genus'], $line['species']);

    // Adds genotype_call for each marker.
    $marker_cols = $this->getJob()->getParamByKey('marker_cols');

    foreach ($marker_cols as $marker_name => $feature_id) {

      // Adds genotype.
      $genotype_value = $line['$' . $marker_name];
      $genotype = MCL_CHADO_GENOTYPE::addGenotype($this, $genotype_value, $this->cvterms['sequence']['SNP'], $genotype_value);
      if ($genotype) {

        // Adds a SNP into genotype_call.
        $details = array(
          'project_id'  => $dataset->getProjectID(),
          'stock_id'    => $stock->getStockID(),
          'feature_id'  => $feature_id,
          'genotype_id' => $genotype->getGenotypeID(),
        );
        MCL_CHADO_GENOTYPE_CALL::addGenotypeCall($this, $details);
      }
    }
  }

  /**
   * Checks the markers for feature_id. Save them in job.param.
   *
   * @param array $line
   */
  private function _checkMarkers($line) {

    // This should be called once.
    if ($line['line_no'] != 3 || $this->getJob()->getParamByKey('marker_cols')) {
      return;
    }

    // Gets feature_id for the markers.
    $markers = array();
    foreach ($line as $key => $value) {
      if (preg_match("/^\\$(.*)$/", $key, $matches)) {
        $marker_name = $matches[1];
         $marker = MCL_CHADO_FEATURE::getFeature($marker_name, '', '', $this->cvterms['sequence']['genetic_marker']);
        if ($marker) {
          $markers[$marker_name] = $marker->getFeatureID();
        }
        else {
          $this->updateMsg('E', "$marker_name not found in feature");
        }
      }
    }
    $this->getJob()->setParamByKey('marker_cols', $markers);
    $this->getJob()->update();
  }
}