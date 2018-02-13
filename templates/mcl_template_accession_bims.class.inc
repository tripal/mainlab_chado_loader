<?php
/**
 * The declaration of MCL_TEMPLATE_ACCESSION_BIMS class.
 *
 */
class MCL_TEMPLATE_ACCESSION_BIMS extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'STOCK';
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
    $desc = 'The description for accession sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'accession'         => array('req' => TRUE,  'width' => 10, 'var' => TRUE, 'desc' => "Primary ID or name of accession. The most important ID or name (unique and stable at least within the breeding program) should be in this column. For mapping population, follow the naming convention of each database."),
      'type'              => array('req' => TRUE,  'width' => 10, 'desc' => "Select from the three choices (cultivar, breeding_research_material, or wild_unimproved) for an individual accession or 'population' for a group of individual."),
      'genus'             => array('req' => TRUE,  'width' => 10, 'desc' => "Genus to which the accession belongs to."),
      'species'           => array('req' => TRUE,  'width' => 10, 'desc' => "Species name. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'grin_id'           => array('req' => FALSE, 'width' => 10, 'desc' => "Accession ID (ACID) in the GRIN database, if it is known."),
      'subspecies'        => array('req' => FALSE, 'width' => 10, 'desc' => "Subspecies name."),
      'secondary_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "Alternate ID or name that is commonly used to refer to the entry."),
      'maternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "accession for the seed parent of this entry.  This accession must also have its own record in this table (it should be entered before being listed as a parent)."),
      'paternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "accession for the pollen parent of this entry.  This accession must also have its own record in this table (it should be entered before being listed as a parent)."),
      'mutation_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "accession of the mutaion parent. This accession must also have its own record in this table (it should be entered before being listed as a parent)."),
      'selfing_parent'    => array('req' => FALSE, 'width' => 10, 'desc' => "accession of the selfing parent. This accession must also have its own record in this table (it should be entered before being listed as a parent)."),
      'alias'             => array('req' => FALSE, 'width' => 10, 'desc' => "A concatenation of all other less commonly used names or designations for the entry.  the aliases. The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for aliases, just write the aliases without the specific type.  (eg. Collector:98HT-227; Site:W6 21306; ABC-1; 21306) The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for an alias, just write the aliases without the specific type. (eg. Collector:98HT-227; Site:W6 21306; ABC-1; 21306)."),
      'cultivar'          => array('req' => FALSE, 'width' => 10, 'desc' => "Cultivar name if It is a named, cultivated variety."),
      'pedigree'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any additional pedigree information: When the exact parents are not known, or any additional information beyond just parents (eg. Red-fruit sport of Jonathan, Pyrus sp. X Cydonia sp., etc)."),
      'origin'            => array('req' => FALSE, 'width' => 10, 'desc' => "The original country for the variety (especially for wild variety)."),
      'population_size'   => array('req' => FALSE, 'width' => 10, 'desc' => "Only for type 'population'."),
      'germplasm_center'  => array('req' => FALSE, 'width' => 10, 'desc' => "Germplasm center or lab where the accession is distributed.  Otherwise, provide the details of the contact person in contact column (from whom the material can be obtained) in the 'Contact' sheet."),
      'image'             => array('req' => FALSE, 'width' => 10, 'desc' => "Image_ID of any associated image (Detailed info in 'Image' sheet.) Multiple images, separated by comma, can be added."),
      'description'       => array('req' => FALSE, 'width' => 10, 'desc' => "any description for the accession."),
      'comments'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments on the accession."),
      '##property_name'   => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );

    // Adds labels.
    $headers['grin_id']['label']      = 'GRIN_ID';
    $headers['secondary_id']['label'] = 'secondary_ID';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']                      = -1;
    $cvterms['SITE_CV']['secondary_id']               = -1;
    $cvterms['SITE_CV']['germplasm_center']           = -1;
    $cvterms['SITE_CV']['cultivar']                   = -1;
    $cvterms['SITE_CV']['is_a_maternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_paternal_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_mutation_parent_of']    = -1;
    $cvterms['SITE_CV']['is_a_selfing_parent_of']     = -1;
    $cvterms['SITE_CV']['maternal_parent']            = -1;
    $cvterms['SITE_CV']['paternal_parent']            = -1;
    $cvterms['SITE_CV']['mutation_parent']            = -1;
    $cvterms['SITE_CV']['selfing_parent']             = -1;
    $cvterms['SITE_CV']['pedigree']                   = -1;
    $cvterms['SITE_CV']['description']                = -1;
    $cvterms['SITE_CV']['origin']                     = -1;
    $cvterms['SITE_CV']['comments']                   = -1;
    $cvterms['SITE_CV']['population_size']            = -1;
    $cvterms['SITE_CV']['subspecies']                 = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');
    if (!$program_id) {
      $this->updateMsg('E', "program_id not found in job.param");
    }

    // Checks DB for GRIN.
    MCL_CHADO_DB::checkDB($this, 'GRIN');

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks aliases.
    MCL_CHADO_STOCK::checkAlias($this, $line['alias'], '[;,]');

    // Checks images.
    MCL_CHADO_IMAGE::checkImageID($this, $line['image'], '[;,]');

    // Checks accession type.
    if (!MCL_DATA_VALID_TYPE::validate('stock_type', $line['type'])) {
      $this->updateMsg('E', 'Invalid accession type : ' . $line['type']);
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $bims_cols  = $this->getJob()->getParamByKey('bims_cols');

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($line['genus'], $line['species']);
    $organism_id = $organism->getOrganismID();

    // Gets accession type.
    $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['type'])->getCvtermID();

    // Sets the uniquename [program_id + accession].
    $name = $line[strtolower($bims_cols['accession'])];
    $uniquename = $program_id . '.' . $name;

    // Adds an accession.
    $accession = MCL_CHADO_STOCK::addStock($this, $uniquename, $name, $organism_id, $type_id);
    if ($accession) {

      // Gets db of GRIN and add a dbxref.
      $db = MCL_CHADO_DB::getDB('GRIN');
      $dbxref = MCL_CHADO_DBXREF::addDBXref($this, $db->getDbID(), $line['grin_id']);
      if ($dbxref) {

        // Adds dbxref.
        $accession->addDBXref($this, $dbxref);

        // Updates dbxref_id.
        $accession->setDbxrefID($dbxref->getDbxrefID());
        $accession->update();
      }

      // Adds properties.
      $accession->addProp($this, 'SITE_CV', 'secondary_id', $line['secondary_id']);
      $accession->addProp($this, 'SITE_CV', 'description', $line['description']);
      $accession->addProp($this, 'SITE_CV', 'cultivar', $line['cultivar']);
      $accession->addProp($this, 'SITE_CV', 'pedigree', $line['pedigree']);
      $accession->addProp($this, 'SITE_CV', 'origin', $line['origin']);
      $accession->addProp($this, 'SITE_CV', 'population_size', $line['population_size']);
      $accession->addProp($this, 'SITE_CV', 'subspecies', $line['subspecies']);
      $accession->addProp($this, 'SITE_CV', 'germplasm_center', $line['germplasm_center']);
      $accession->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $accession, $line);

      // Adds aliases.
      $accession->addAlias($this, $line['alias'], "[,;]");

      // Adds parents.
      if ($line['paternal_parent']) {
        $accession->addParent($this, $program_id . '.' . $line['paternal_parent'], '', $this->cvterms['SITE_CV']['is_a_paternal_parent_of']);
      }
      if ($line['maternal_parent']) {
        $accession->addParent($this, $program_id . '.' . $line['maternal_parent'], '', $this->cvterms['SITE_CV']['is_a_maternal_parent_of']);
      }
      if ($line['mutation_parent']) {
        $accession->addParent($this, $program_id . '.' . $line['mutation_parent'], '', $this->cvterms['SITE_CV']['is_a_mutation_parent_of']);
      }
      if ($line['selfing_parent']) {
        $accession->addParent($this, $program_id . '.' . $line['selfing_parent'], '', $this->cvterms['SITE_CV']['is_a_selfing_parent_of']);
      }

      // Adds images.
      $accession->addImageID($this, $line['image'], "[,;]");
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($this->getJob()->getParamByKey('cv'));
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $data_lines[0])) {
      return;
    }

    // Saves the parents info.
    $parents = array();
    $types = array('paternal_parent', 'maternal_parent', 'mutation_parent', 'selfing_parent');
    foreach ($data_lines as $data_line) {
      $accession = $data_line['accession'];
      foreach ($types as $type) {
        if (array_key_exists($type, $data_line) && $data_line[$type]) {
          $parents[$accession][$type] = $data_line[$type];
        }
      }
    }
    $this->getJob()->setParamByKey('parents', $parents);
    $this->getJob()->update();
  }

  /**
   * @see MCL_TEMPLATE::postProcess()
   */
  public function postProcess() {

    // Gets the program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Adds all missing parents now.
    $parents = $this->getTmpByKey('stock_parents');
    foreach ((array)$parents as $stock_id => $info) {

      // Gets the parent info.
      $uniquename   = $info['uniquename'];
      $organism_id  = $info['organism_id'];
      $type_id      = $info['type_id'];

      // Adds the parent.
      $accession = MCL_CHADO_STOCK::byKey(array('stock_id' => $stock_id));
      $accession->addParent($this, $program_id . '.' . $uniquename, $organism_id, $type_id);
    }
  }

  /**
   * @see MCL_TEMPLATE::getTemplateInfo()
   */
  public function getTemplateInfo() {

    // Gets BIMS_USER and BIMS_PROGRAM.
    $bims_user    = getBIMS_USER();
    $bims_program = $bims_user->getProgram();

    // Updates the headers.
    $bims_cols = $bims_program->getBIMSCols();
    $this->updateHeaders($bims_cols);

    // Adds info. for header definition.
    $def_sheet = array();
    $def_sheet['name']  = 'Headers';

    // Sets headers.
    $headers = array();
    $headers['A'] = array('heading' => 'Name',        'width' => 25);
    $headers['B'] = array('heading' => 'Required',    'width' => 10);
    $headers['C'] = array('heading' => 'Definition',  'width' => 100);
    $def_sheet['headers'] = $headers;

    // Sets data.
    $data = array();
    foreach ($this->headers as $heading => $prop) {
      $name = array(
          'col'   => 'A',
          'type'  => 'text',
          'value' => $heading,
      );
      $req = array(
          'col'   => 'B',
          'type'  => 'text',
          'value' => ($prop['req']) ? 'REQ' : ''
      );
      $def = array(
          'col'   => 'C',
          'type'  => 'text',
          'value' => $prop['desc'],
      );
      $data[] = array($name, $req, $def);
    }
    $def_sheet['data'] = $data;

    // Adds info. for data.
    $data_sheet = array();
    $data_sheet['name'] = $this->template;

    // Sets headers.
    $headers = array();
    $col_char = 'A';
    $regex = $this->getRegExSpCol();
    foreach ($this->headers as $heading => $prop) {
      $label  = $heading;
      $header = array();

      if (!preg_match("/^~/", $heading)) {
        if (preg_match($regex, $heading)) {
          continue;
        }
      }

      // Adds a label if exists.
      if (array_key_exists('label', $prop)) {
        $label = $prop['label'];
      }

      // Adds a prefix to heading.
      $prefix = ($prop['req']) ? '*' : '';
      $header['heading'] = $prefix . $label;

      // Adds width if defined.
      if (array_key_exists('width', $prop)) {
        $header['width'] = $prop['width'];
      }
      $headers[$col_char++] = $header;
    }
    $data_sheet['headers'] = $headers;

    // Adds both header definition and data sheets.
    return array($def_sheet, $data_sheet);
  }
}