<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE class.
 *
 */
class MCL_TEMPLATE_GENOTYPE extends MCL_TEMPLATE {

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
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'stock_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock genotyped. It should match a 'stock_name' of the 'Stock' sheet."),
      'clone_id'        => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a clone if known."),
      'marker'          => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker used for genotyping. It should match a marker_name in the 'Marker' sheet."),
      'storage_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "Any storage ID for the DNA sample."),
      'plot'            => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'row'             => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'position'        => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'collector'       => array('req' => FALSE, 'width' => 10, 'desc' => "People who collected the plant material. It should match 'contact_name' in the Contact sheet. Multiple names can be added with a semicolon (;) in between."),
      'collection_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Date when the plant material was collected."),
      'genotyping_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Date when genotypying."),
      'instrument'      => array('req' => FALSE, 'width' => 10, 'desc' => "Any instrument used for genotyping."),
      'size_standard'   => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the genotyping experiment."),
      'data_year'       => array('req' => FALSE, 'width' => 10, 'desc' => "Genotyping date if only year is known."),
      'experimenter'    => array('req' => FALSE, 'width' => 10, 'desc' => "Person who conducted genotyping. It should match 'contact_name' in the Contact sheet. Multiple names can be added with a semicolon (;) in between."),
      'dye_color'       => array('req' => FALSE, 'width' => 10, 'desc' => "Dye color used for genotyping."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments on the specific genotype (of a specific sample with a specific marker)."),
      '#allele'         => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (#) : followed by allele_N where N is an integer."),
    );

    // Adds labels.
    $headers['clone_id']['label']   = 'clone_ID';
    $headers['storage_id']['label'] = 'storage_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['genotyping']       = -1;
    $cvterms['SITE_CV']['clone']            = -1;
    $cvterms['SITE_CV']['clone_of']         = -1;
    $cvterms['SITE_CV']['sample']           = -1;
    $cvterms['SITE_CV']['sample_of']        = -1;
    $cvterms['SITE_CV']['sample_id']        = -1;
    $cvterms['SITE_CV']['storage_id']       = -1;
    $cvterms['SITE_CV']['plot']             = -1;
    $cvterms['SITE_CV']['row']              = -1;
    $cvterms['SITE_CV']['position']         = -1;
    $cvterms['SITE_CV']['pick_date']        = -1;
    $cvterms['SITE_CV']['evaluation_date']  = -1;
    $cvterms['SITE_CV']['genotyping_date']  = -1;
    $cvterms['SITE_CV']['collector']        = -1;
    $cvterms['SITE_CV']['instrument']       = -1;
    $cvterms['SITE_CV']['size_standard']    = -1;
    $cvterms['SITE_CV']['dye_color']        = -1;
    $cvterms['SITE_CV']['comments']         = -1;
    $cvterms['SITE_CV']['data_year']        = -1;
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

    // Checks marker.
    MCL_CHADO_FEATURE::checkFeature($this, $line['marker'], '', '', $this->cvterms['sequence']['genetic_marker']);

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

    // Gets the marker.
    $marker = MCL_CHADO_FEATURE::getFeature($line['marker'], '', '', $this->cvterms['sequence']['genetic_marker']);

    // Gets the genotype.
    $genotype_columns = $this->getJob()->getParamByKey('genotype_columns');
 print_r($genotype_columns);
    $tmp = array();
    foreach ($genotype_columns as $column) {
      if ($line[$column]) {
        $tmp []= $line[$column];
      }
    }
    sort($tmp);
    $allele_str = implode('|', $tmp);
    $uniquename = $marker->getUniquename() . '_' . $allele_str;

    // Adds a genotype.
    $genotype = MCL_CHADO_GENOTYPE::addGenotype($this, $uniquename, $this->cvterms['sequence']['allele'], $allele_str);

    // Adds a sample.
    $sample_id = sprintf("%d-%d-%d", $dataset->getProjectID(), $stock->getStockID(), $marker->getFeatureID());
    $sample = MCL_CHADO_STOCK::addStock($this, $sample_id, '', $stock->getOrganismID(), $this->cvterms['SITE_CV']['sample']);
    if ($sample) {

      // Adds stock relationship between sample and stock.
      $sample->addRelatedStock($this, $stock, $this->cvterms['SITE_CV']['sample_of']);

      // Adds a nd_eperiment.
      $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['genotyping'], $nd_geolocation_id, $this->cvterms['SITE_CV']['sample_id'], $sample->getStockID());
      if ($nd_experiment) {

        // Adds sample properties.
        $sample->addProp($this, 'SITE_CV', 'storage_id', $line['storage_id']);
        $sample->addProp($this, 'SITE_CV', 'plot', $line['plot']);
        $sample->addProp($this, 'SITE_CV', 'row', $line['row']);
        $sample->addProp($this, 'SITE_CV', 'position', $line['position']);
        $sample->addProp($this, 'SITE_CV', 'collector', $line['collector']);
        $sample->addProp($this, 'SITE_CV', 'collection_date', $line['collection_date']);

        // Adds nd_experiment properties.
        $nd_experiment->addProp($this, 'SITE_CV', 'genotyping_date', $line['genotyping_date']);
        $nd_experiment->addProp($this, 'SITE_CV', 'instrument', $line['instrument']);
        $nd_experiment->addProp($this, 'SITE_CV', 'size_standard', $line['size_standard']);
        $nd_experiment->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
        $nd_experiment->addProp($this, 'SITE_CV', 'dye_color', $line['dye_color']);
        $nd_experiment->addProp($this, 'SITE_CV', 'comments', $line['comments']);

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

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Saves the genotyping columns.
    $genotype_columns = array();
    foreach ($data_lines[0] as $column => $value) {
      //print ">$value\n";
      if (preg_match("/^#(allele\_\d+)$/i", $column, $matches)) {
        $genotype_columns []= $column;
      }
    }
    $this->getJob()->setParamByKey('genotype_columns', $genotype_columns);
    $this->getJob()->update();
  }
}