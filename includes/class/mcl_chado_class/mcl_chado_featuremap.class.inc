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
   */
  public static function checkMap(MCL_TEMPLATE $mcl_tmpl, $map, $separator = '') {

    // Gets contact names.
    $names = preg_split($mcl_tmpl->getSepRegex($separator), $map, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($names as $name) {
      $name = trim($name);
      $mcl_map = MCL_CHADO_FEATUREMAP::getMap($name);
      if (!$mcl_map) {
        $mcl_tmpl->updateMsg('E', "(name) = ($name) not found in featuremap");
      }
    }
  }

  /**
   * Checks the map unit for cvterm. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $map_unit
   * @param string $separator
   */
  public static function checkMapUnit(MCL_TEMPLATE $mcl_tmpl, $map_unit, $separator = '') {

    // Gets cvterm for the map unit.
    $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $map_unit);
    if (!$mcl_cvterm) {
      $mcl_tmpl->updateMsg('E', "(map_unit) = ($map_unit) not found in cvterm");
    }
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
  public static function addMap(MCL_TEMPLATE $mcl_tmpl, $name, $map_unit, $description) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_featuremap = MCL_CHADO_FEATUREMAP::byKey($args);
    if ($mcl_featuremap) {
      $mcl_tmpl->addMsg('D', 'featuremap', $args);
    }
    else {

      // Gets unittype_id from map_unit.
      $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $map_unit);
      if (!$cvterm) {
        $mcl_tmpl->updateMsg('E', "map_unit : '$map_unit' found in cvterm");
        return NULL;
      }

      // Adds a new map.
      $args['unittype_id'] = $cvterm->getCvtermID();
      $args['description'] = $description;
      $mcl_featuremap = new MCL_CHADO_FEATUREMAP($args);
      if ($mcl_featuremap->insert()) {
        $mcl_tmpl->addMsg('N', 'featuremap', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'featuremap', $args);
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
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $value, $separator = '') {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    $mcl_tmpl->addProperty('featuremapprop', 'featuremap_id', $this->featuremap_id, $type_id, $value, $separator);
  }

  /**
   * Adds stock to featuremap_organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $genus
   * @param integer $species
   * @param string $separator
   */
  public function addOrganism(MCL_TEMPLATE $mcl_tmpl, $genus, $species, $separator = '') {
    if ($genus && $species) {
      $sps = preg_split($mcl_tmpl->getSepRegex($separator), $species, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($sps as $sp) {
        $sp = trim($sp);

        // Gets the organism.
        $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $sp);
        if ($mcl_organism) {
          $mcl_tmpl->addLink('featuremap_organism', 'featuremap_id', $this->featuremap_id, 'organism_id', $mcl_organism->getOrganismID());
        }
        else {
          $mcl_tmpl->updateMsg('E', "(genus, species) = ($genus, $sp) not found in organism");
        }
      }
    }
  }

  /**
   * Adds stock to featuremap_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param integer $organism_id
   * @param string $separator
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl, $stock, $genus, $species, $separator = '') {
    if ($stock) {

      // Gets orgaims_id.
      $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($mcl_organism) {
        $organism_id = $mcl_organism->getOrganismID();

        // Adds a stock.
        $uniquenames = preg_split($mcl_tmpl->getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
        foreach ($uniquenames as $uniquename) {
          $uniquename = trim($uniquename);
          $mcl_stock = MCL_CHADO_STOCK::byKey(array('uniquename' => $uniquename, 'organism_id' => $organism_id));
          if ($mcl_stock) {
            $mcl_tmpl->addLink('featuremap_stock', 'featuremap_id', $this->featuremap_id, 'stock_id', $mcl_stock->getStockID());
          }
          else {
            $this->updateMsg('E', "(uniquename, organism_id) = ($uniquename, $organism_id) not found in stock");
          }
        }
      }
      else {
        $this->updateMsg('E', "(genus, species) = ($genus, $species) not found in organism");
      }
    }
  }

  /**
   * Adds contact to featuremap_contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   */
  public function addContact(MCL_TEMPLATE $mcl_tmpl, $contact, $separator = '') {
    if ($contact) {
      $names = preg_split($mcl_tmpl->getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_contact = MCL_CHADO_CONTACT::getContact($name);
        if ($mcl_contact) {
          $mcl_tmpl->addLink('featuremap_contact', 'featuremap_id', $this->featuremap_id, 'contact_id', $mcl_contact->getContactID());
        }
      }
    }
  }

  /**
   * Adds reference to featuremap_pub.
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
          $mcl_tmpl->addLink('featuremap_pub', 'featuremap_id', $this->featuremap_id, 'pub_id', $pub_id);
        }
      }
    }
  }
}