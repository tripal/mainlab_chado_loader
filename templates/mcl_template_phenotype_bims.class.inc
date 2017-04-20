<?php
/**
 * The declaration of MCL_TEMPLATE_PHENOTYPE_BIMS class.
 *
 */
class MCL_TEMPLATE_PHENOTYPE_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  // $descriptor array will contain descriptor (trait name) as key and
  // cvterm_id as value.
  private $descriptors = array();

  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'PHENOTYPE';
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
    $desc = 'The description for phenotype sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the phenotyping dataset. It should match a 'dataset_name' column entry in the 'Dataset' sheet."),
      'stock_name'      => array('req' => TRUE,  'width' => 10, 'desc' => "ID of the stock that has been phenotyped. It should match a 'stock_name' column entry in the 'Stock' sheet."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => ""),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => ""),
      'sample_id'       => array('req' => TRUE,  'width' => 10, 'desc' => "The ID of the smallest sample or sub-sample that was tested (Each row should have unique sample_ID). If there is no unique sample ID, users can concatenate a set of columns that gives a unique phenotypic value and those columns become required fields. For example, if a combination of dataset_name, ID, phenotyping_date is unique for the specific phenotype, those three can be concatenated with '_' in between."),
      'clone_id'        => array('req' => FALSE, 'width' => 10, 'desc' => "ID of a spefic clone if available (eg. individual tree)."),
      'evaluator'       => array('req' => FALSE, 'width' => 10, 'desc' => "Person who did the phenotyping. Multiple person can be entered with ';' in between. It should match 'contact_name' of the Contact sheet."),
      'site_name'       => array('req' => FALSE, 'width' => 10, 'desc' => "Site information where the stock for the phenotyping is planted. It should match 'site_name' in the 'Site' sheet."),
      'rep'             => array('req' => FALSE, 'width' => 10, 'desc' => "Any repetition number."),
      'rootstock'       => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the rootstock if the plant are grafted to a rootstock. It should match an 'stock_name' column of the 'Stock' sheet."),
      'plot'            => array('req' => FALSE, 'width' => 10, 'desc' => "Plot name."),
      'row'             => array('req' => FALSE, 'width' => 10, 'desc' => "Row."),
      'position'        => array('req' => FALSE, 'width' => 10, 'desc' => "Position of the plant."),
      'plant_date'      => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the planting."),
      'data_year'       => array('req' => FALSE, 'width' => 10, 'desc' => "Phenotyping date if only year is known."),
      'evaluation_date' => array('req' => FALSE, 'width' => 10, 'desc' => "Date of phenotype evaluation."),
      'pick_date'       => array('req' => FALSE, 'width' => 10, 'desc' => "Date of the sample collection if the collection is done on a different date than the phenotype evaluation."),
      'previous_entry'  => array('req' => FALSE, 'width' => 10, 'desc' => "stock ID of the previous entry."),
      'barcode'         => array('req' => FALSE, 'width' => 10, 'desc' => "Barcode"),
      'fiber_pkg'       => array('req' => FALSE, 'width' => 10, 'desc' => "Group of samples for phenotyping, can contain samples from multiple germplasm."),
      'storage_time'    => array('req' => FALSE, 'width' => 10, 'desc' => "Time between collection and phenotyping."),
      'storage_regime'  => array('req' => FALSE, 'width' => 10, 'desc' => "The condition of sample storage between the collection and phenotyping."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the phenotyping."),
      '$$'              => array('req' => FALSE, 'width' => 10, 'desc' => "Followed by cvterm name. The data will be stored in stockprop table."),
      '##'              => array('req' => FALSE, 'width' => 10, 'desc' => "Followed by cvterm name. The data will be stored in nd_experiementprop table."),
    );

    // Adds labels.
    $headers['sample_id']['label']  = 'sample_ID';
    $headers['clone_id']['label']   = 'clone_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['phenotyping']        = -1;
    $cvterms['SITE_CV']['rootstock_id']       = -1;
    $cvterms['SITE_CV']['nd_geolocation_id']  = -1;
    $cvterms['SITE_CV']['sample']             = -1;
    $cvterms['SITE_CV']['sample_of']          = -1;
    $cvterms['SITE_CV']['clone']              = -1;
    $cvterms['SITE_CV']['clone_of']           = -1;
    $cvterms['SITE_CV']['clone_id']           = -1;
    $cvterms['SITE_CV']['plot']               = -1;
    $cvterms['SITE_CV']['row']                = -1;
    $cvterms['SITE_CV']['position']           = -1;
    $cvterms['SITE_CV']['data_year']          = -1;
    $cvterms['SITE_CV']['plant_date']         = -1;
    $cvterms['SITE_CV']['pick_date']          = -1;
    $cvterms['SITE_CV']['evaluation_date']    = -1;
    $cvterms['SITE_CV']['storage_time']       = -1;
    $cvterms['SITE_CV']['storage_regime']     = -1;
    $cvterms['SITE_CV']['previous_entry']     = -1;
    $cvterms['SITE_CV']['barcode']            = -1;
    $cvterms['SITE_CV']['fiber_pkg']          = -1;
    $cvterms['SITE_CV']['comments']           = -1;
    $cvterms['SITE_CV']['source']             = -1;
    $cvterms['SITE_CV']['previous_entry']     = -1;

    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Adds prefix.
    $dataset_name   = $program_id . '.' . $line['dataset_name'];
    $stock_name     = $program_id . '.' . $line['stock_name'];
    $clone_id       = $program_id . '.' . $line['clone_id'];
    $site_name      = $program_id . '.' . $line['site_name'];

    // Checks dataset.
    MCL_CHADO_DATASET::checkDataset($this, $dataset_name);

    // Checks organisms.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $stock_name, $line['genus'], $line['species']);

    // Checks rootstock.
    if ($line['rootstock']) {
      $rootstock_name = $program_id . '.' . $line['rootstock'];
      MCL_CHADO_STOCK::checkStock($this, $rootstock_name, $line['genus'], $line['species']);
    }

    // Checks evaluator for contact.
    MCL_CHADO_CONTACT::checkContact($this, $line['evaluator'], '[;]');

    // Checks site.
    MCL_CHADO_LOCATION::checkSite($this, $site_name);

    // Checks phenotypic descriptor.
    $this->_checkDescriptors($line);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Checks the data column. Skip this line if no data presents.
    if ($this->isEmptyRow($line, '^#')) {
      return TRUE;
    }

    // Gets the organims.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);

    // Gets the program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Adds prefix.
    $dataset_name   = $program_id . '.' . $line['dataset_name'];
    $stock_name     = $program_id . '.' . $line['stock_name'];
    $site_name      = $program_id . '.' . $line['site_name'];
    $sample_id      = $program_id . '.' . $line['sample_id'];

    // Gets the stock.
    $args = array(
      'uniquename'  => $stock_name,
      'organism_id' => $organism->getOrganismID(),
    );
    $stock = MCL_CHADO_STOCK::byKey($args);
    if ($stock) {

      // Adds sample.
      $sample = MCL_CHADO_STOCK::addStock($this, $sample_id, $line['sample_id'], $organism->getOrganismID(), $this->cvterms['SITE_CV']['sample']);
      if (!$sample) {
        return FALSE;
      }

      // Adds clone.
      $clone_id = $line['clone_id'];
      $clone = NULL;
      if ($clone_id) {
        $clone_id = $program_id . '.' . $clone_id;
        $clone = MCL_CHADO_STOCK::addStock($this, $clone_id, '', $organism->getOrganismID(), $this->cvterms['SITE_CV']['clone']);
      }

      // Adds relationships between stock, sample and clone.
      if ($clone) {
        $clone->addRelatedStock($this, $stock, $this->cvterms['SITE_CV']['clone_of']);
        $sample->addRelatedStock($this, $clone, $this->cvterms['SITE_CV']['sample_of']);
      }
      $sample->addRelatedStock($this, $stock, $this->cvterms['SITE_CV']['sample_of']);

      // Gets rootstock_id.
      $rootstock_id = '';
      if ($line['rootstock']) {
        $rootstock_name = $program_id . '.' . $line['rootstock'];
        $rootstock = MCL_CHADO_STOCK::getStock($rootstock_name, $line['genus'], $line['species']);
        if ($rootstock) {
          $rootstock_id = $rootstock->getStockID();
        }

        // Adds rootstock.
        $sample->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock_id);
        if ($clone) {
          $clone->addProp($this, 'SITE_CV', 'rootstock_id', $rootstock_id);
        }
      }

      // Gets nd_geolocation_id.
      $nd_geolocation_id = MCL_CHADO_LOCATION::getLocationIDBySite($site_name);

      // Adds clone properties.
      if ($clone) {
        $clone->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
        $clone->addProp($this, 'SITE_CV', 'clone_id', $clone_id);
        $clone->addProp($this, 'SITE_CV', 'rep', $line['rep']);
        $clone->addProp($this, 'SITE_CV', 'plot', $line['plot']);
        $clone->addProp($this, 'SITE_CV', 'row', $line['row']);
        $clone->addProp($this, 'SITE_CV', 'position', $line['position']);
        $clone->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      }

      // Adds sample properties.
      $sample->addProp($this, 'SITE_CV', 'nd_geolocation_id', $nd_geolocation_id);
      $sample->addProp($this, 'SITE_CV', 'clone_id', $clone_id);
      $sample->addProp($this, 'SITE_CV', 'rep', $line['rep']);
      $sample->addProp($this, 'SITE_CV', 'plot', $line['plot']);
      $sample->addProp($this, 'SITE_CV', 'row', $line['row']);
      $sample->addProp($this, 'SITE_CV', 'position', $line['position']);
      $sample->addProp($this, 'SITE_CV', 'plant_date', $line['plant_date']);
      $sample->addProp($this, 'SITE_CV', 'pick_date', $line['pick_date']);
      $sample->addProp($this, 'SITE_CV', 'storage_time', $line['storage_time']);
      $sample->addProp($this, 'SITE_CV', 'barcode', $line['barcode']);
      $sample->addProp($this, 'SITE_CV', 'fiber_pkg', $line['fiber_pkg']);
      $sample->addProp($this, 'SITE_CV', 'storage_regime', $line['storage_regime']);
      $sample->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds special columns.
      $this->addSpecialColumns('$$', $sample, $line);

      // Adds previous-entry.
      $sample->addPreviousEntry($this, $line['previous-entry']);

      // Gets phenotyping data.
      $descriptors = $this->getJob()->getParamByKey('descriptors');
      foreach ($descriptors as $cvterm_id => $descriptor) {
        $name = strtolower($descriptor['name']);

        // Adds the value.
        $value = $line['#' . $name];

        // Sets the default value if provided.
        if (!$value && $descriptor['default_value']) {
          $value = $descriptor['default_value'];
        }

        // Adds the value.
        if ($value || $value == '0') {

          // Checks the value for min and max.
          if ($descriptor['max_value']) {}
          if ($descriptor['min_value']) {}

          // Adds a phenotype.
          $uniquename = $program_id . '.' . $name . '_' . $value;
          $phenotype = MCL_CHADO_PHENOTYPE::addPhenotype($this, $uniquename, $cvterm_id, $value);

          // Adds a nd_experiment.
          $nd_experiment = MCL_CHADO_ND_EXPERIMENT::addNdExperiment($this, $this->cvterms['SITE_CV']['phenotyping'], $nd_geolocation_id, $cvterm_id, $sample->getStockID());

          if ($nd_experiment) {

            // Adds properties.
            $nd_experiment->addProp($this, 'SITE_CV', 'data_year', $line['data_year']);
            $nd_experiment->addProp($this, 'SITE_CV', 'evaluation_date', $line['evaluation_date']);

            // Adds special columns.
            $this->addSpecialColumns('##', $nd_experiment, $line);

            // Adds evaluators.
            $nd_experiment->addContact($this, $line['evaluator'], "[;]");

            // Adds a stock.
            $nd_experiment->addStock($this, $sample, $sample->getTypeID());

            // Adds a dataset.
            $nd_experiment->addDataset($this, $dataset_name);

            // Adds a phenotype.
            $nd_experiment->addPhenotype($this, $phenotype);
          }
        }
      }
    }
  }

  /**
   * Checks trait descriptors for cvterm_id. Save them in job.param.
   *
   * @param array $line
   */
  private function _checkDescriptors($line) {

    // This should be called once.
    if ($line['line_no'] != 3) {
      return;
    }

    // Gets the cv for the descriptors.
    $cv = $this->getCvByPrefix('#');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist for prefix '#'.");
      return;
    }
    $cv_id = $cv->getCvID();

    // Go through headers stored in the line array.
    $descriptors = array();
    foreach ($line as $column => $value) {
      if (preg_match("/^#[A-Za-z0-9\_]/", $column)) {

        $cvterm_name = str_replace('#', '', $column);
        $cvterm = MCL_CHADO_CVTERM::getCvtermByCvID($cv_id, $cvterm_name);
        if ($cvterm) {
          $cvterm_id = $cvterm->getCvtermID();
          $sql = "
            SELECT C.cvterm_id, C.name, FORMAT.value AS format,
              MIN_VAL.value AS min_value, MAX_VAL.value AS max_value,
              DEF_VAL.value AS default_value, CAT.value AS categories
            FROM {chado.cvterm} C
              LEFT JOIN (
                SELECT CP.cvterm_id, CP.value
                FROM chado.cvtermprop CP
                  INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
                  INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
                WHERE LOWER(C.name) = :format AND LOWER(CV.name) = :cv_name
              ) FORMAT on FORMAT.cvterm_id = C.cvterm_id
              LEFT JOIN (
                SELECT CP.cvterm_id, CP.value
                FROM chado.cvtermprop CP
                  INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
                  INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
                WHERE LOWER(C.name) = :min_value AND LOWER(CV.name) = :cv_name
              ) MIN_VAL on MIN_VAL.cvterm_id = C.cvterm_id
              LEFT JOIN (
                SELECT CP.cvterm_id, CP.value
                FROM chado.cvtermprop CP
                  INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
                  INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
                WHERE LOWER(C.name) = :max_value AND LOWER(CV.name) = :cv_name
              ) MAX_VAL on MAX_VAL.cvterm_id = C.cvterm_id
              LEFT JOIN (
                SELECT CP.cvterm_id, CP.value
                FROM chado.cvtermprop CP
                  INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
                  INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
                WHERE LOWER(C.name) = :default_value AND LOWER(CV.name) = :cv_name
              ) DEF_VAL on DEF_VAL.cvterm_id = C.cvterm_id
              LEFT JOIN (
                SELECT CP.cvterm_id, CP.value
                FROM chado.cvtermprop CP
                  INNER JOIN chado.cvterm C on C.cvterm_id = CP.type_id
                  INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
                WHERE LOWER(C.name) = :categories AND LOWER(CV.name) = :cv_name
              ) CAT on CAT.cvterm_id = C.cvterm_id
            WHERE C.cvterm_id = :cvterm_id
          ";
          $args = array(
            ':cv_name'        => 'main',
            ':format'         => 'format',
            ':min_value'      => 'min_value',
            ':max_value'      => 'max_value',
            ':default_value'  => 'default_value',
            ':categories'     => 'categories',
            ':cvterm_id'      => $cvterm_id
          );
          $obj = db_query($sql, $args)->fetch(PDO::FETCH_ASSOC);
          $descriptors[$cvterm_id] = $obj;
        }
        else {
          $this->updateMsg('E', "$cvterm_name not found in cvterm");
        }
      }
    }
    $this->getJob()->setParamByKey('descriptors', $descriptors);
    $this->getJob()->update();
  }
}