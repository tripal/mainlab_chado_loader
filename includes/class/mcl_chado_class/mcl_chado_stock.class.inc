<?php
/**
 * The declaration of MCL_CHADO_STOCK class.
 *
 */
class MCL_CHADO_STOCK extends CHADO_STOCK {

 /**
  *  Class data members.
  */
  // Valid stock types.
  public static $stock_types = array(
    'breeding_research_material',
    'cultivar',
    'landrace',
    'population',
    'species',
    'wild_unimproved',
  );

  /**
   * @see CHADO_STOCK::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_STOCK::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_STOCK::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Returns stock types.
   *
   * @return array of stock types.
   */
  public static function getStockTypeArray() {

    // Gets all stock types.
    $stock_arr = array();
    foreach (self::$stock_types as $stock_type) {
      $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $stock_type);
      if ($mcl_cvterm) {
        $stock_arr[$mcl_cvterm->getCvtermID()] = $stock_type;
      }
    }
    return $stock_arr;
  }

  /**
   * Checks the type of stock. If not, write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock_type
   */
  public static function checkType(MCL_TEMPLATE $mcl_tmpl, $stock_type) {
    if ($stock_type) {
      if (!in_array(strtolower($stock_type), $mcl_tmpl->valid_stock_types)) {
        $mcl_tmpl->updateMsg('E', "Error - germplasm_type : '$stock_type' is not valid germplasm type");
      }
      else {
        $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $stock_type);
        if (!$cvterm) {
          $mcl_tmpl->updateMsg('E', "Error - germplasm_type : '$stock_type' does not exist");
        }
      }
    }
  }

  /**
   * Checks the existance of stock. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param string $genus
   * @param string $species
   * @param string $separator
   */
  public static function checkStock(MCL_TEMPLATE $mcl_tmpl, $stock, $genus = '', $species = '', $separator = '') {
    if ($stock) {

      // Gets the organism_id.
      $organism_id = NULL;
      if ($genus && $species) {
        $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        $organism_id = $organism->getOrganismID();
      }

      // Gets the uniquenames.
      $uniquenames = preg_split($mcl_tmpl->getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($uniquenames as $uniquename) {
        $uniquename = trim($uniquename);

        // Sets the arguments.
        $args = array('uniquename' => $uniquename);
        if ($organism_id) {
          $args['organism_id'] = $organism_id;
        }

        // If '::' found in uniquename, update uniquename and organism_id.
        if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $uniquename, $matches)) {
          $genus          = trim($matches[1]);
          $species        = trim($matches[2]);
          $uniquename     = trim($matches[3]);
          $diff_organism  = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
          if ($diff_organism) {
            $args['organism_id'] = $diff_organism->getOrganismID();
          }
          else {
            $mcl_tmpl->updateMsg('E', "$genus $species not found in organism");
            return;
          }
        }

        // Checks the stock.
        $mcl_stock = MCL_CHADO_STOCK::byKey($args);
        if (!$mcl_stock) {
          $mcl_tmpl->updateMsg('E', $mcl_tmpl->arrStr($args) . " not found in stock");
        }
      }
    }
  }

  /**
   * Checks the format of alias. Write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $alias
   * @param string $separator
   */
  public static function checkAlias(MCL_TEMPLATE $mcl_tmpl, $alias, $separator = '') {
    if ($alias) {

      // Gets the alias.
      $aliases = preg_split($mcl_tmpl->getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($aliases as $value) {
        $tmp = preg_split("/:/", $value);
        $num_elem = sizeof($tmp);
        if ($num_elem > 2) {
          $mcl_tmpl->updateMsg('E', "$value is not a valid format. It should be alias_type:alias");
        }
      }
    }
  }

  /**
   * Returns the stock by uniquename, genus and species.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   *
   * @return MCL_CHADO_STOCK
   */
  public static function getStock($uniquename, $genus = '', $species = '') {

    // Sets the args.
    $args = array(
      'uniquename' => $uniquename,
    );

    // Adds the organism.
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      $args['organism_id'] = $organism->getOrganismID();
    }
    return MCL_CHADO_STOCK::bykey($args);
  }

  /**
   * Adds a stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $organism_id
   * @param integer $type_id
   * @param string $name
   *
   * @return MCL_CHADO_STOCK
   */
  public static function addStock(MCL_TEMPLATE $mcl_tmpl, $uniquename, $name, $organism_id, $type_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_stock = MCL_CHADO_STOCK::byKey($args);
    if ($mcl_stock) {
      $mcl_tmpl->addMsg('D', 'stock', $args);
    }
    else {

      // Adds a new stock.
      $args['type_id']  = $type_id;
      $args['name']     = $name;
      $mcl_stock = new MCL_CHADO_STOCK($args);
      if ($mcl_stock->insert()) {
        $mcl_tmpl->addMsg('N', 'stock', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'stock', $args);
      }
    }
    return $mcl_stock;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $value, $separator = '') {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    $mcl_tmpl->addProperty('stockprop', 'stock_id', $this->stock_id, $type_id, $value, $separator);
  }

  /**
   * Adds a related stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_STOCK $stock
   * @param integer $type_id_relationship
   */
  public function addRelatedStock(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_STOCK $stock, $type_id_relationship) {
    if ($stock) {
      $mcl_tmpl->addRelationship('stock_relationship', 'subject_id', $this->stock_id, 'object_id', $stock->getStockID(), $type_id_relationship);
    }
  }

  /**
   * Adds parents by dataset name and cross numbe.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_ND_EXPERIMENT $cross
   * @param integer $type_id
   */
  public function addCross(MCL_TEMPLATE $mcl_tmpl, $cross, $type_id) {
    if ($cross) {

      // Links this progeny and dataset cross_number.
      $mcl_tmpl->addLink('nd_experiment_stock', 'stock_id', $this->stock_id, 'nd_experiment_id', $cross->getNdExperimentID(), $type_id);
    }
  }

  /**
   * Adds a parent.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $organism_id
   * @param integer $type_id_relationship
   */
  public function addParent(MCL_TEMPLATE $mcl_tmpl, $uniquename, $organism_id, $type_id_relationship) {
    if ($uniquename && $type_id_relationship) {

      // If '::' found in uniquename, update uniquename and organism_id.
      if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $uniquename, $matches)) {
        $genus          = trim($matches[1]);
        $species        = trim($matches[2]);
        $uniquename     = trim($matches[3]);
        $diff_organism  = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if ($diff_organism) {
          $organism_id = $diff_organism->getOrganismID();
        }
      }

      // Adds a parent.
      $args = array('uniquename' => $uniquename);
      if ($organism_id) {
        $args['organism_id'] = $organism_id;
      }
      $parent = MCL_CHADO_STOCK::byKey($args);
      if ($parent) {
        $parent->addRelatedStock($mcl_tmpl, $this, $type_id_relationship);
      }
      else {
        $mcl_tmpl->updateMsg('E', "Parent '$uniquename' dose not exists in stock table.");
      }
    }
  }

  /**
   * Adds alias to stockprop.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl, $alias, $separator = '') {
    if ($alias) {
      $aliases = preg_split($mcl_tmpl->getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($aliases as $value) {
        $tmp = preg_split("/:/", $value);
        $alias_type = 'alias';
        if (sizeof($tmp) > 1) {
          $alias_type = $tmp[0];
          $value = $tmp[1];
        }
        $alias_type = (strtolower($alias_type) == 'cultivar') ? 'cultivar' : 'alias';
        $this->addProp($mcl_tmpl, 'SITE_CV', $alias_type, $value);
      }
    }
  }

  /**
   * Adds a dbxref to stock_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_DBXREF $dbxref
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_DBXREF $dbxref) {
    $mcl_tmpl->addLink('stock_dbxref', 'stock_id', $this->stock_id, 'dbxref_id', $dbxref->getDbxrefID());
  }

  /**
   * Adds contact to stock_contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator

  public function addContact(MCL_TEMPLATE $mcl_tmpl, $contact, $separator = '') {
    if ($contact) {
      $names = preg_split($mcl_tmpl->getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_contact = MCL_CHADO_CONTACT::getContact($name);
        if ($mcl_contact) {
          $mcl_tmpl->addLink('stock_contact', 'stock_id', $this->stock_id, 'contact_id', $mcl_contact->getContactID());
        }
      }
    }
  } */

  /**
   * Adds image to stock_image.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   */
  public function addImage(MCL_TEMPLATE $mcl_tmpl, $image, $separator = '') {
    if ($image) {
      $eimage_ids = preg_split($mcl_tmpl->getSepRegex($separator), $image, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $mcl_image = MCL_CHADO_IMAGE::byKey(array('eimage_id' => $eimage_id));
        if ($mcl_image) {
          $mcl_tmpl->addLink('stock_image', 'stock_id', $this->stock_id, 'eimage_id', $mcl_image->getEimageID());
        }
      }
    }
  }

  /**
   * Adds reference to stock_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   */
  public function addReference(MCL_TEMPLATE $mcl_tmpl, $reference, $separator = '') {
    if ($reference) {
      $pub_ids = preg_split($mcl_tmpl->getSepRegex($separator), $reference, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($pub_ids as $pub_id) {
        $pub_id = trim($pub_id);
        $mcl_pub = MCL_CHADO_PUB::getPub($pub_id);
        if ($mcl_pub) {
          $mcl_tmpl->addLink('stock_pub', 'stock_id', $this->stock_id, 'pub_id', $pub_id);
        }
      }
    }
  }

  /**
   * Adds reference to stock_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $previous_entry
   */
  public function addPreviousEntry(MCL_TEMPLATE $mcl_tmpl, $previous_entry) {
    if ($previous_entry) {

      // Checks the sample.
      $sample = MCL_CHADO_STOCK::byKey(array('uniquename' => $previous_entry));
      if ($sample) {

        // Adds the sample_id in stockprop.
        $this->addProp($mcl_tmpl, 'SITE_CV', 'previous_entry', $sample->getStockID());
      }
      else {
        $mcl_tmpl->updateMsg('E', "Error - sample : '$previous_entry' does not exist");
      }
    }
  }
}