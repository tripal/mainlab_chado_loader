<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_B class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_B extends MCL_TEMPLATE {

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
    $desc = 'The description for GENOTYPE_B sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'stock_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock genotyped. It should match a 'stock_name' of the 'Stock' sheet."),
      'clone_id'        => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a clone if known."),
      'genotyping_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Any size standard used."),
      'experimenter'    => array('req' => FALSE, 'width' => 10, 'desc' => "Person who conducted genotyping. It should match 'contact_name' in the Contact sheet. Multiple names can be added with a semicolon (;) in between."),
      '$marker_name'    => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns ($) : followed by marker_name_N where N is an integer."),
    );

    // Adds labels.
    $headers['clone_id']['label']   = 'clone_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['genotyping']       = -1;
    $cvterms['SITE_CV']['sample']           = -1;
    $cvterms['SITE_CV']['sample_of']        = -1;
    $cvterms['SITE_CV']['sample_id']        = -1;
    $cvterms['SITE_CV']['clone']            = -1;
    $cvterms['SITE_CV']['genotyping_date']  = -1;
    $cvterms['sequence']['genetic_marker']  = -1;
    $cvterms['sequence']['allele']          = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $line['dataset_name']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name']);

    // Checks experimenter.
    MCL_CHADO_CONTACT::checkContact($this, $line['experimenter'], '[;]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets nd_geolocation_id for genotyping data.
    $nd_geolocation_id = MCL_VAR::getValueByName('ND_GEOLOCATION_ID');

    // Gets the dataset.
    $dataset = MCL_CHADO_DATASET::byName($line['dataset_name']);

    // Gets the stock.
    $stock = MCL_CHADO_STOCK::getStock($line['stock_name']);

    // Adds genotyping data.
    $genotype_columns = $this->getJob()->getParamByKey('genotype_columns');
    foreach ($genotype_columns as $feature_id => $columns) {

      // Gets the marker.
      $marker = MCL_CHADO_FEATURE::byKey(array('feature_id' => $feature_id));

      // Gets the genotype.
      $tmp = array();
      foreach ($columns as $column) {
        $tmp []= $line[$column];
      }
      sort($tmp);
      $allele_str = implode('|', $tmp);
      $uniquename = $marker->getUniquename() . '_' . $allele_str;

      // Adds a genotype.
      $genotype = MCL_CHADO_GENOTYPE::addGenotype($this, $uniquename, $this->cvterms['sequence']['allele'], $allele_str);

      // Adds a sample.
      $sample_id = sprintf("%d-%d-%d", $dataset->getProjectID(), $stock->getStockID(), $feature_id);
      $sample = MCL_CHADO_STOCK::addStock($this, $sample_id, '', $stock->getOrganismID(), $this->cvterms['SITE_CV']['sample']);
      if ($sample) {

        // Adds stock relationship between sample and stock.
        $sample->addRelatedStock($this, $stock, $this->cvterms['SITE_CV']['sample_of']);

        // Adds a nd_eperiment.
        $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['genotyping'], $nd_geolocation_id, $this->cvterms['SITE_CV']['sample_id'], $sample->getStockID());
        if ($nd_experiment) {

          // Adds nd_experiment properties.
          $nd_experiment->addProp($this, 'SITE_CV', 'genotyping_date', $line['genotyping_date']);

          // Adds a stock.
          $nd_experiment->addStock($this, $sample, $this->cvterms['SITE_CV']['genotyping']);

          // Adds evaluators.
          $nd_experiment->addContact($this, $line['experimenter'], "[;]");

          // Adds a dataset.
          $nd_experiment->addDataset($this, $line['dataset_name']);

          // Adds a genotype for nd_experiment.
          $nd_experiment->addGenotype($this, $genotype);

          // Adds a genotype for marker.
          $marker->addGenotype($this, $genotype, $this->cvterms['sequence']['allele']);
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Saves the genotyping columns.
    $genotype_columns = array();
    foreach ((array)$data_lines[0] as $column => $value) {
      if (preg_match("/^\\$(.*)_\d+$/", $column, $matches)) {
        $marker_name = $matches[1];
        $marker = MCL_CHADO_FEATURE::byKey(array('uniquename' => $marker_name));
        if (!$marker) {
          $this->updateMsg('E', "$marker_name does not exist in database.");
          continue;
        }
        $feature_id = $marker->getFeatureID();
        if (array_key_exists($feature_id, $genotype_columns)) {
          $genotype_columns[$feature_id][] = $column;
        }
        else {
          $genotype_columns[$feature_id] = array($column);
        }
      }
    }
    $this->getJob()->setParamByKey('genotype_columns', $genotype_columns);
    $this->getJob()->update();
  }
}