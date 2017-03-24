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
      'stock_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock genotyped. It should match an 'stock_name' of the 'Stock' sheet."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the stock"),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "Species of the stock"),
      'sample_id'       => array('req' => TRUE,  'width' => 10, 'desc' => "ID of a sample. Normally it is the same as ID unless the user want to keep tract of genotype of a specific sample."),
      'clone_id'        => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a clone if known."),
      'marker'          => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker used for genotyping. It should match a marker_name in the 'Marker' sheet."),
      'storage_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "Any storage ID for the DNA sample."),
      'plot'            => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'row'             => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'position'        => array('req' => FALSE, 'width' => 10, 'desc' => "Field position where the plant material was collected from."),
      'collector'       => array('req' => FALSE, 'width' => 10, 'desc' => "People who collected the plant material."),
      'collection_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Date when the plant material was collected.  It should match 'contact_name' in the Contact sheet. Multiple names can be added with a semicolon (;) in between."),
      'genotyping_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Any size standard used."),
      'instrument'      => array('req' => FALSE, 'width' => 10, 'desc' => "Any instrument used for genotyping."),
      'size_standard'   => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the genotyping experiment."),
      'data_year'       => array('req' => FALSE, 'width' => 10, 'desc' => "Genotyping date if only year is known."),
      'experimenter'    => array('req' => FALSE, 'width' => 10, 'desc' => "Person who conducted genotyping. It should match 'contact_name' in the Contact sheet. Multiple names can be added with a semicolon (;) in between."),
      'dye_color'       => array('req' => FALSE, 'width' => 10, 'desc' => "Dye color used for genotyping."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments on the specific genotype (of a specific sample with a specific marker)."),
    );

    // Adds labels.
    $headers['clone_id']['label']   = 'clone_ID';
    $headers['sample_id']['label']  = 'sample_ID';
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

    // Checks organisms.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['marker_genus'], $line['marker_species']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['stock_name'], $line['genus'], $line['species']);

    // Checks marker.
    MCL_CHADO_FEATURE::checkFeature($this, $line['marker'], $line['marker_genus'], $line['marker_species'], $this->cvterms['sequence']['genetic_marker']);

    // Checks experimenter for contacts.
    MCL_CHADO_CONTACT::checkContact($this, $line['experimenter'], '[;]');
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

    while (FALSE) {

      // Creates uniquename and description for genotype.
      $description = '';
      $uniquename = $marker->getUniquename() . '_' . $description;

      // Adds a genotype.
      $genotype = MCL_CHADO_GENOTYPE::addGenotype($this, $uniquename, $this->cvterms['sequence']['allele'], $description);

      // Adds a sample.
      $sample = NULL;
//      $sample = MCL_CHADO_STOCK::addStock($this, $line['sample_id'], '', $organism->getOrganismID(), $this->cvterms['SITE_CV']['sample']);

      // Adds a nd_eperiment.
      $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['genotyping'], 0, $this->cvterms['SITE_CV']['sample_id'], $sample->getStockID());

      // Adds clone properties.
      if ($clone) {
        $clone->addProp($this, 'SITE_CV', 'storage_id', $line['storage_id']);
        $clone->addProp($this, 'SITE_CV', 'plot', $line['plot']);
        $clone->addProp($this, 'SITE_CV', 'row', $line['row']);
        $clone->addProp($this, 'SITE_CV', 'position', $line['position']);
      }

      // Adds sample properties.
      $sample->addProp($this, 'SITE_CV', 'storage_id', $line['storage_id']);
      $sample->addProp($this, 'SITE_CV', 'plot', $line['plot']);
      $sample->addProp($this, 'SITE_CV', 'row', $line['row']);
      $sample->addProp($this, 'SITE_CV', 'position', $line['position']);
      $sample->addProp($this, 'SITE_CV', 'collector', $line['collector']);
      $sample->addProp($this, 'SITE_CV', 'collection_date', $line['collection_date']);

      // Adds properties.
      $nd_experiment->addProp($this, 'SITE_CV', 'genotyping_date', $line['genotyping_date']);
      $nd_experiment->addProp($this, 'SITE_CV', 'instrument', $line['instrument']);
      $nd_experiment->addProp($this, 'SITE_CV', 'size_standard', $line['size_standard']);
      $nd_experiment->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
      $nd_experiment->addProp($this, 'SITE_CV', 'dye_color', $line['dye_color']);
      $nd_experiment->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds a stock.
      $nd_experiment->addStock($this, $sample, '');

      // Adds evaluators.
      $nd_experiment->addContact($this, $line['experimenter'], "[;]");

      // Adds a genotype.
      $nd_experiment->addGenotype($this, $genotype);

      // Adds a genotype for marker.
      $marker->addGenotype($this, $genotype);
    }
  }
}