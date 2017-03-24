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
   * Checks the existance of stock. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param string $genus
   * @param string $species
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkStock(MCL_TEMPLATE $mcl_tmpl = NULL, $stock, $genus = '', $species = '', $separator = '') {
    $flag = TRUE;
    if ($stock) {

      // Gets the organism_id.
      $organism_id = NULL;
      if ($genus && $species) {
        $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if (!$organism) {
          self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
          return FALSE;
        }
        $organism_id = $organism->getOrganismID();
      }

      // Gets the uniquenames.
      $uniquenames = preg_split(self::getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
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
            self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
            $flag = FALSE;
          }
        }

        // Checks the stock.
        $mcl_stock = MCL_CHADO_STOCK::byKey($args);
        if (!$mcl_stock) {
          self::updateMsg($mcl_tmpl, 'E', self::arrStr($args) . " not found in stock");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the format of alias. Write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $alias
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $alias, $separator = '') {
    $flag = TRUE;
    if ($alias) {

      // Gets the alias.
      $aliases = preg_split(self::getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($aliases as $value) {
        $tmp = preg_split("/:/", $value);
        $num_elem = sizeof($tmp);
        if ($num_elem > 2) {
          self::updateMsg($mcl_tmpl, 'E', "$value is not a valid format. It should be alias_type:alias");
          $flag = FALSE;
        }
      }
    }
    return $flag;
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
   * @param string $name
   * @param integer $organism_id
   * @param integer $type_id
   *
   * @return MCL_CHADO_STOCK
   */
  public static function addStock(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $name, $organism_id, $type_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_stock = MCL_CHADO_STOCK::byKey($args);
    if ($mcl_stock) {
      self::addMsg($mcl_tmpl, 'D', 'stock', $args);
    }
    else {

      // Adds a new stock.
      $args['type_id']  = $type_id;
      $args['name']     = $name;
      $mcl_stock = new MCL_CHADO_STOCK($args);
      if ($mcl_stock->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'stock', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'stock', $args);
        return NULL;
      }
    }
    return $mcl_stock;
  }

  /**
   * Adds a stock by a secondary ID.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $secondary_id
   * @param integer $organism_id
   * @param integer $type_id
   *
   * @return MCL_CHADO_STOCK
   */
  public static function addStockBy2ndID(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $secondary_id, $organism_id, $type_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'name'        => $secondary_id,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication with the secondary ID.
    $args = array(
      'name'        => $secondary_id,
      'organism_id' => $organism_id,
    );
    $mcl_stock = MCL_CHADO_STOCK::byKey($args);
    if ($mcl_stock) {
      self::addMsg($mcl_tmpl, 'D', 'stock', $args);
    }
    else {

      // Checks for duplication with the uniquename.
      $args = array(
        'uniquename'  => $uniquename,
        'organism_id' => $organism_id,
      );
      $mcl_stock = MCL_CHADO_STOCK::byKey($args);
      if ($mcl_stock) {
        self::updateMsg($mcl_tmpl, 'E', "The uniquename ($uniquename) has a different secondary ID");
      }
      else {

        // Adds a new stock.
        $args = array(
          'uniquename'  => $uniquename,
          'name'        => $secondary_id,
          'organism_id' => $organism_id,
          'type_id'     => $type_id,
        );
        $mcl_stock = new MCL_CHADO_STOCK($args);
        if ($mcl_stock->insert()) {
          self::addMsg($mcl_tmpl, 'N', 'stock', $args);
        }
        else {
          self::addMsg($mcl_tmpl, 'E', 'stock', $args);
          return NULL;
        }
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
   *
   * @return boolean
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value, $separator = '') {
    if ($value) {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'stockprop', 'stock_id', $this->stock_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds a related stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_STOCK $stock
   * @param integer $type_id_relationship
   *
   * @return boolean
   */
  public function addRelatedStock(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_STOCK $stock, $type_id_relationship) {
    if ($stock) {
      return $this->addRelationship($mcl_tmpl, 'stock_relationship', 'subject_id', $this->stock_id, 'object_id', $stock->getStockID(), $type_id_relationship);
    }
    return TRUE;
  }

  /**
   * Adds parents by dataset name and cross numbe.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_ND_EXPERIMENT $cross
   * @param integer $type_id
   *
   * @return boolean
   */
  public function addCross(MCL_TEMPLATE $mcl_tmpl = NULL, $cross, $type_id) {
    if ($cross) {
      return $this->addLink($mcl_tmpl, 'nd_experiment_stock', 'stock_id', $this->stock_id, 'nd_experiment_id', $cross->getNdExperimentID(), $type_id);
    }
    return TRUE;
  }

  /**
   * Adds a parent.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $organism_id
   * @param integer $type_id_relationship
   *
   * @return boolean
   */
  public function addParent(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $organism_id, $type_id_relationship) {
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
        if (!$parent->addRelatedStock($mcl_tmpl, $this, $type_id_relationship)) {
          return FALSE;
        }
      }
      else {
        $this->updateMsg($mcl_tmpl, 'E', "Parent '$uniquename' dose not exists in stock table.");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Adds alias to stockprop.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $alias, $separator = '') {
    $flag = TRUE;
    if ($alias) {
      $aliases = preg_split($this->getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($aliases as $value) {
        $tmp = preg_split("/:/", $value);
        $alias_type = 'alias';
        if (sizeof($tmp) > 1) {
          $alias_type = $tmp[0];
          $value = $tmp[1];
        }
        $alias_type = (strtolower($alias_type) == 'cultivar') ? 'cultivar' : 'alias';
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', $alias_type, $value)) {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a dbxref to stock_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_DBXREF $dbxref
   *
   * @return boolean
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_DBXREF $dbxref) {
    if ($dbxref) {
      return $this->addLink($mcl_tmpl, 'stock_dbxref', 'stock_id', $this->stock_id, 'dbxref_id', $dbxref->getDbxrefID());
    }
    return TRUE;
  }

  /**
   * Adds contact to stock_contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   *
   * @return boolean

  public function addContact(MCL_TEMPLATE $mcl_tmpl = NULL, $contact, $separator = '') {
    $flag = TRUE;
    if ($contact) {
      $names = preg_split($this->getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_contact = MCL_CHADO_CONTACT::getContact($name);
        if ($mcl_contact) {
          if (!$this->addLink($mcl_tmpl, 'stock_contact', 'stock_id', $this->stock_id, 'contact_id', $mcl_contact->getContactID())) {
            $flag = FALSE;
          }
        }
        else {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  } */

  /**
   * Adds image to stock_image.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public function addImage(MCL_TEMPLATE $mcl_tmpl = NULL, $image, $separator = '') {
    $flag = TRUE;
    if ($image) {
      $eimage_ids = preg_split($this->getSepRegex($separator), $image, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $mcl_image = MCL_CHADO_IMAGE::byKey(array('eimage_id' => $eimage_id));
        if ($mcl_image) {
          if (!$this->addLink($mcl_tmpl, 'stock_image', 'stock_id', $this->stock_id, 'eimage_id', $mcl_image->getEimageID())) {
            $flag = FALSE;
          }
        }
        else {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds reference to stock_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public function addReference(MCL_TEMPLATE $mcl_tmpl = NULL, $reference, $separator = '') {
    $flag = TRUE;
    if ($reference) {
      $pub_ids = preg_split($this->getSepRegex($separator), $reference, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($pub_ids as $pub_id) {
        $pub_id = trim($pub_id);
        $mcl_pub = MCL_CHADO_PUB::getPub($pub_id);
        if ($mcl_pub) {
          if (!$this->addLink($mcl_tmpl, 'stock_pub', 'stock_id', $this->stock_id, 'pub_id', $pub_id)) {
            $flag = FALSE;
          }
        }
        else {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds reference to stock_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $previous_entry
   *
   * @return boolean
   */
  public function addPreviousEntry(MCL_TEMPLATE $mcl_tmpl = NULL, $previous_entry) {
    $flag = TRUE;
    if ($previous_entry) {

      // Checks the sample.
      $sample = MCL_CHADO_STOCK::byKey(array('uniquename' => $previous_entry));
      if ($sample) {

        // Adds the sample_id in stockprop.
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'previous_entry', $sample->getStockID())) {
          $flag = FALSE;
        }
      }
      else {
        $this->updateMsg($mcl_tmpl, 'E', "Error - sample : '$previous_entry' does not exist");
        $flag = FALSE;
      }
    }
    return $flag;
  }
}