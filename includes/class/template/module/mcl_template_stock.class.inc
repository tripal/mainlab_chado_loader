<?php
/**
 * The declaration of MCL_TEMPLATE_STOCK class.
 *
 */
class MCL_TEMPLATE_STOCK extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  // Valid stock types.
  public $valid_stock_types = array(
    'variety', 'cultivar', 'breeding_research_material', 'population',
    'wild_unimproved', 'tbd', 'landrace', 'germplasm', 'species'
  );

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
    $desc = 'The description for stock sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'stock_name'        => array('req' => TRUE,  'width' => 10, 'desc' => "Primary ID or name of stock. The most important ID or name (unique and stable at least within the breeding program) should be in this column. For mapping population, follow the naming convention of each database."),
      'germplasm_type'    => array('req' => TRUE,  'width' => 10, 'desc' => "Select from the three choices (cultivar, breeding/research material, or wild/unimproved) for an individual germplasm or 'population' for a group of individual."),
      'genus'             => array('req' => TRUE,  'width' => 10, 'desc' => "Genus to which the stock belongs to."),
      'species'           => array('req' => TRUE,  'width' => 10, 'desc' => "Species name. Enter 'sp.' to represent one unknown species, 'spp.' to represent multiple unknown species."),
      'grin_id'           => array('req' => FALSE, 'width' => 10, 'desc' => "Accession ID (ACID) in the GRIN database, if it is known."),
      'subspecies'        => array('req' => FALSE, 'width' => 10, 'desc' => "Subspecies name."),
      'description'       => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name for the pollen parent of this entry.  This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'secondary_id'      => array('req' => FALSE, 'width' => 10, 'desc' => "Alternate ID or name that is commonly used to refer to the entry."),
      'paternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name for the pollen parent of this entry.  This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'maternal_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name for the seed parent of this entry.  This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'mutation_parent'   => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name of the mutaion parent. This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'selfing_parent'    => array('req' => FALSE, 'width' => 10, 'desc' => "Stock_name of the selfing parent. This stock_name must also have its own record in this table (it should be entered before being listed as a parent)."),
      'alias'             => array('req' => FALSE, 'width' => 10, 'desc' => "A concatenation of all other less commonly used names or designations for the entry.  the aliases. The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for aliases, just write the aliases without the specific type.  (eg. Collector:98HT-227; Site:W6 21306; ABC-1; 21306) The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for an alias, just write the aliases without the specific type. (eg. Collector:98HT-227; Site:W6 21306; ABC-1; 21306)."),
      'cultivar'          => array('req' => FALSE, 'width' => 10, 'desc' => "Cultivar name if It is a named, cultivated variety."),
      'pedigree'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any additional pedigree information: When the exact parents are not known, or any additional information beyond just parents (eg. Red-fruit sport of Jonathan, Pyrus sp. X Cydonia sp., etc)."),
      'origin'            => array('req' => FALSE, 'width' => 10, 'desc' => "The original country for the variety (especially for wild variety)."),
      'population_size'   => array('req' => FALSE, 'width' => 10, 'desc' => "Only for type 'population'."),
      'germplasm_center'  => array('req' => FALSE, 'width' => 10, 'desc' => "Stock center or lab where the stock is distributed.  Otherwise, provide the details of the contact person in contact column (from whom the material can be obtained) in the 'Contact' sheet."),
      'description'       => array('req' => FALSE, 'width' => 10, 'desc' => "any description for the stock."),
      'comments'          => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments on the stock."),
      'image'             => array('req' => FALSE, 'width' => 10, 'desc' => "Image_ID of any associated image (Detailed info in 'Image' sheet.) Multiple images, separated by comma, can be added."),
      'reference'         => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id if any publication is associated with the stock."),
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
    $cvterms['SITE_CV']['variety']                    = -1;
    $cvterms['SITE_CV']['cultivar']                   = -1;
    $cvterms['SITE_CV']['breeding_research_material'] = -1;
    $cvterms['SITE_CV']['wild_unimproved']            = -1;
    $cvterms['SITE_CV']['tbd']                        = -1;
    $cvterms['SITE_CV']['landrace']                   = -1;
    $cvterms['SITE_CV']['germplasm']                  = -1;
    $cvterms['SITE_CV']['population']                 = -1;
    $cvterms['SITE_CV']['species']                    = -1;
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

    // Checks DB for GRIN.
    MCL_CHADO_DB::checkDB($this, 'GRIN');

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks germplasm type.
    MCL_CHADO_STOCK::checkType($this, $line['germplasm_type']);

    // Checks aliases.
    MCL_CHADO_STOCK::checkAlias($this, $line['alias'], '[;,]');

    // Checks parents.
    MCL_CHADO_STOCK::checkStock($this, $line['paternal_parent'], $line['genus'], $line['species']);
    MCL_CHADO_STOCK::checkStock($this, $line['maternal_parent'], $line['genus'], $line['species']);
    MCL_CHADO_STOCK::checkStock($this, $line['mutation_parent'], $line['genus'], $line['species']);
    MCL_CHADO_STOCK::checkStock($this, $line['selfing_parent'],  $line['genus'], $line['species']);

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
    $organism_id = $organism->getOrganismID();

    // Gets type_id.
    $type_id = $this->cvterms['SITE_CV'][strtolower($line['germplasm_type'])];

    // Updates name.
    $name = ($line['secondary_id']) ? $line['secondary_id'] : '';

    // Adds a stock.
    $stock = MCL_CHADO_STOCK::addStock($this, $line['stock_name'], $name, $organism_id, $type_id);
    if ($stock) {

      // Gets db.
      $db = MCL_CHADO_DB::getDB('GRIN');

      // Adds a dbxref.
      $dbxref = MCL_CHADO_DBXREF::addDBXref($this, $db->getDbID(), $line['grin_id']);
      if ($dbxref) {

        // Adds dbxref to stock_dbxref table.
        $stock->addDBXref($this, $dbxref);

        // Updates stock.dbxref_id.
        $stock->setDbxrefID($dbxref->getDbxrefID());
        $stock->update();
      }

      // Adds properties.
      $stock->addProp($this, 'SITE_CV', 'secondary_id', $line['secondary_id']);
      $stock->addProp($this, 'SITE_CV', 'description', $line['description']);
      $stock->addProp($this, 'SITE_CV', 'cultivar', $line['cultivar']);
      $stock->addProp($this, 'SITE_CV', 'pedigree', $line['pedigree']);
      $stock->addProp($this, 'SITE_CV', 'origin', $line['origin']);
      $stock->addProp($this, 'SITE_CV', 'population_size', $line['population_size']);
      $stock->addProp($this, 'SITE_CV', 'subspecies', $line['subspecies']);
      $stock->addProp($this, 'SITE_CV', 'germplasm_center', $line['germplasm_center']);
      $stock->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds aliases.
      $stock->addAlias($this, $line['alias'], "[,;]");

      // Adds parents.
      $stock->addParent($this, $line['paternal_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_maternal_parent_of']);
      $stock->addParent($this, $line['maternal_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_paternal_parent_of']);
      $stock->addParent($this, $line['mutation_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_mutation_parent_of']);
      $stock->addParent($this, $line['selfing_parent'], $organism_id, $this->cvterms['SITE_CV']['is_a_selfing_parent_of']);

      // Adds images.
      $stock->addImage($this, $line['image'], "[,;]");

      // Adds references.
      $stock->addReference($this, $line['reference'], "[,;]");
    }
  }
}