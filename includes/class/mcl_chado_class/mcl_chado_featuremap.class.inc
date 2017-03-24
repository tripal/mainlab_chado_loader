<?php
/**
 * The declaration of MCL_CHADO_FEATUREMAP class.
 *
 */
class MCL_CHADO_FEATUREMAP extends CHADO_FEATUREMAP {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_FEATUREMAP::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_FEATUREMAP::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_FEATUREMAP::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of featuremap. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $map
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkMap(MCL_TEMPLATE $mcl_tmpl = NULL, $map, $separator = '') {
    $flag = TRUE;
    if ($map) {

      // Gets contact names.
      $names = preg_split(self::getSepRegex($separator), $map, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_map = MCL_CHADO_FEATUREMAP::getMap($name);
        if (!$mcl_map) {
          self::updateMsg($mcl_tmpl, 'E', "(name) = ($name) not found in featuremap");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the map unit for cvterm. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $map_unit
   *
   * @return boolean
   */
  public static function checkMapUnit(MCL_TEMPLATE $mcl_tmpl = NULL, $map_unit) {

    // Gets cvterm for the map unit.
    $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $map_unit);
    if (!$mcl_cvterm) {
      self::updateMsg($mcl_tmpl, 'E', "(map_unit) = ($map_unit) not found in cvterm");
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Returns the featuremap by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_FEATUREMAP
   */
  public static function getMap($name) {
    return MCL_CHADO_FEATUREMAP::bykey(array('name' => $name));
  }

  /**
   * Adds a featuremap.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   * @param integer $map_unit
   * @param integer $description
   *
   * @return MCL_CHADO_FEATUREMAP
   */
  public static function addMap(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $map_unit, $description) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_featuremap = MCL_CHADO_FEATUREMAP::byKey($args);
    if ($mcl_featuremap) {
      self::addMsg($mcl_tmpl, 'D', 'featuremap', $args);
    }
    else {

      // Gets unittype_id from map_unit.
      $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $map_unit);
      if (!$cvterm) {
        self::updateMsg($mcl_tmpl, 'E', "map_unit : '$map_unit' found in cvterm");
        return NULL;
      }

      // Adds a new map.
      $args['unittype_id'] = $cvterm->getCvtermID();
      $args['description'] = $description;
      $mcl_featuremap = new MCL_CHADO_FEATUREMAP($args);
      if ($mcl_featuremap->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'featuremap', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'featuremap', $args);
        return NULL;
      }
    }
    return $mcl_featuremap;
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
      return $this->addProperty($mcl_tmpl, 'featuremapprop', 'featuremap_id', $this->featuremap_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds stock to featuremap_organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $genus
   * @param integer $species
   * @param string $separator
   *
   * @return boolean
   */
  public function addOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, $genus, $species, $separator = '') {
    $flag = TRUE;
    if ($genus && $species) {
      $sps = preg_split($this->getSepRegex($separator), $species, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($sps as $sp) {
        $sp = trim($sp);

        // Gets the organism.
        $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $sp);
        if ($mcl_organism) {
          if (!$this->addLink($mcl_tmpl, 'featuremap_organism', 'featuremap_id', $this->featuremap_id, 'organism_id', $mcl_organism->getOrganismID())) {
            $flag = FALSE;
          }
        }
        else {
          $this->updateMsg($mcl_tmpl, 'E', "(genus, species) = ($genus, $sp) not found in organism");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds stock to featuremap_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param integer $organism_id
   * @param string $separator
   *
   * @return boolean
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl = NULL, $stock, $genus, $species, $separator = '') {
    $flag = TRUE;
    if ($stock) {

      // Gets the organism_id.
      $organism_id = NULL;
      if ($genus && $species) {
        $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if (!$organism) {
          $this->updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
          return FALSE;
        }
        $organism_id = $organism->getOrganismID();
      }

      // Adds a stock.
      $uniquenames = preg_split($this->getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($uniquenames as $uniquename) {
        $uniquename = trim($uniquename);

        // Sets the arguments.
        $args = array('uniquename' => $uniquename);
        if ($organism_id) {
          $args['organism_id'] = $organism_id;
        }

        // Gets the stock and add it.
        $mcl_stock = MCL_CHADO_STOCK::byKey($args);
        if ($mcl_stock) {
          if (!$this->addLink($mcl_tmpl, 'featuremap_stock', 'featuremap_id', $this->featuremap_id, 'stock_id', $mcl_stock->getStockID())) {
            $flag = FALSE;
          }
        }
        else {
          $this->updateMsg($mcl_tmpl, 'E', "(uniquename, organism_id) = ($uniquename, $organism_id) not found in stock");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds contact to featuremap_contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   *
   * @return boolean
   */
  public function addContact(MCL_TEMPLATE $mcl_tmpl = NULL, $contact, $separator = '') {
    $flag = TRUE;
    if ($contact) {
      $names = preg_split($this->getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_contact = MCL_CHADO_CONTACT::getContact($name);
        if ($mcl_contact) {
          if (!$this->addLink($mcl_tmpl, 'featuremap_contact', 'featuremap_id', $this->featuremap_id, 'contact_id', $mcl_contact->getContactID())) {
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
   * Adds reference to featuremap_pub.
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
          if (!$this->addLink($mcl_tmpl, 'featuremap_pub', 'featuremap_id', $this->featuremap_id, 'pub_id', $pub_id)) {
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
}