<?php
/**
 * The declaration of MCL_CHADO_ORGANISM class.
 *
 */
class MCL_CHADO_ORGANISM extends CHADO_ORGANISM {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_ORGANISM::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_ORGANISM::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_ORGANISM by ID.
   *
   * @param integer $organism_id
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function byID($organism_id) {
    return MCL_CHADO_ORGANISM::bykey(array('organism_id' => $organism_id));
  }

  /**
   * @see CHADO_ORGANISM::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Returns all organism_id for the given crop.
   *
   * @param integer $crop_id
   *
   * @return array of organism objects
   */
  public static function getOrganismsByCropID($crop_id) {

    // Gets all orgainsm for the selected crop.
    $crop_organism_ids = MCL_CROP::getOrganismIDs($crop_id);
    $organism_arr = array();
    foreach ($crop_organism_ids as $organism_id) {
      $organism = CHADO_ORGANISM::byKey(array('organism_id' => $organism_id));
      $organism_arr[$organism->getOrganismID()] = $organism->getGenus() . ' ' . $organism->GetSpecies();
    }
    return $organism_arr;
  }

  /**
   * Checks the existence of orgainsm. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $genus
   * @param string $species
   * @param string separator
   *
   * @return boolean
   */
  public static function checkOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, $genus, $species, $separator = '') {
    $flag = TRUE;
    if ($genus && $species) {

      $sps = preg_split(self::getSepRegex($separator), $species, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($sps as $sp) {
        $sp = trim($sp);

        // Gets the orgainsm.
        $orgainsm = MCL_CHADO_ORGANISM::getOrganism($genus, $sp);
        if (!$orgainsm) {
          self::updateMsg($mcl_tmpl, 'E', "(genus, species) = ($genus, $sp) not found in organism");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the existence of orgainsm by ID. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $organism_id
   *
   * @return boolean
   */
  public static function checkOrganismByID(MCL_TEMPLATE $mcl_tmpl = NULL, $organism_id) {
    $flag = TRUE;
    if ($organism_id) {

      // Gets the orgainsm.
      $orgainsm = MCL_CHADO_ORGANISM::getOrganismByID($organism_id);
      if (!$orgainsm) {
        self::updateMsg($mcl_tmpl, 'E', "(organism_id) = ($organism_id) not found in organism");
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Returns the orgainsm by genus and species.
   *
   * @param string $genus
   * @param string $species
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function getOrganism($genus, $species) {
    $args = array(
      'genus'   => $genus,
      'species' => $species,
    );
    return MCL_CHADO_ORGANISM::bykey($args);
  }

  /**
   * Returns the orgainsm by organism ID.
   *
   * @param integer $organism_id
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function getOrganismByID($organism_id) {
    return MCL_CHADO_ORGANISM::bykey(array('organism_id' => $organism_id));
  }

  /**
   * Adds a orgainsm.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $genus
   * @param string $species
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function addOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, $genus, $species) {

    // Sets the arguments.
    $args = array(
      'genus'   => $genus,
      'species' => $species,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $orgainsm = MCL_CHADO_ORGANISM::byKey($args);
    if ($orgainsm) {
      self::addMsg($mcl_tmpl, 'D', 'orgainsm', $args);
    }
    else {

      // Adds a new orgainsm.
      $args['type_id']      = $type_id;
      $args['description']  = $description;
      $orgainsm = new MCL_CHADO_ORGANISM($args);
      if ($orgainsm->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'orgainsm', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'orgainsm', $args);
        return NULL;
      }
    }
    return $orgainsm;
  }

  /**
   * Adds N/A organism.
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function addNA() {

    // Checks if it already exists.
    $organism = MCL_CHADO_ORGANISM::getOrganism('N/A', 'N/A');
    if (!$organism) {
      $details = array(
        'genus'   => 'N/A',
        'species' => 'N/A',
      );
      $organism = new MCL_CHADO_ORGANISM($details);
      if (!$organism->insert()) {
        return NULL;
      }
    }
    return $organism;
  }

  /**
   * Adds a property by type ID (cvterm ID).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addPropByID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $value, $separator = '') {
    if ($value != '') {
      return $this->addProperty($mcl_tmpl, 'organismprop', 'organism_id', $this->organism_id, $type_id, $value, $separator);
    }
    return TRUE;
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
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'organismprop', 'organism_id', $this->organism_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds a related organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_ORGANISM $organism
   * @param integer $type_id_relationship
   *
   * @return boolean
   */
  public function addRelatedOrganism(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_ORGANISM $organism, $type_id_relationship) {
    if ($organism) {
      return $this->addRelationship($mcl_tmpl, 'organism_relationship', 'subject_organism_id', $this->organism_id, 'object_organism_id', $organism->getOrganismID(), $type_id_relationship);
    }
    return TRUE;
  }

  /**
   * Adds a dbxref to organism_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_DBXREF $dbxref
   *
   * @return boolean
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_DBXREF $dbxref) {
    return $this->addLink($mcl_tmpl, 'organism_dbxref', 'organism_id', $this->organism_id, 'dbxref_id', $dbxref->getDbxrefID());
  }

  /**
   * Adds image to organism_image.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $image_id_list
   * @param string $separator
   *
   * @return boolean
   */
  public function addImageID(MCL_TEMPLATE $mcl_tmpl = NULL, $image_id_list, $separator = '') {
    $flag = TRUE;
    if ($image_id_list) {
      $eimage_ids = preg_split($this->getSepRegex($separator), $image_id_list, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $image = MCL_CHADO_IMAGE::byKey(array('eimage_id' => $eimage_id));
        if ($image) {
          if (!$this->addLink($mcl_tmpl, 'organism_image', 'organism_id', $this->organism_id, 'eimage_id', $image->getEimageID())) {
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
   * Adds reference to organism_pub.
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
        $pub = MCL_CHADO_PUB::getPub($pub_id);
        if ($pub) {
          if (!$this->addLink($mcl_tmpl, 'organism_pub', 'organism_id', $this->organism_id, 'pub_id', $pub_id)) {
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