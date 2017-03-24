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
   * Checks the existance of orgainsm. If not, write the error messasge
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
        $mcl_orgainsm = MCL_CHADO_ORGANISM::getOrganism($genus, $sp);
        if (!$mcl_orgainsm) {
          self::updateMsg($mcl_tmpl, 'E', "(genus, species) = ($genus, $sp) not found in organism");
          $flag = FALSE;
        }
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
    $mcl_orgainsm = MCL_CHADO_ORGANISM::byKey($args);
    if ($mcl_orgainsm) {
      self::addMsg($mcl_tmpl, 'D', 'orgainsm', $args);
    }
    else {

      // Adds a new orgainsm.
      $args['type_id']      = $type_id;
      $args['description']  = $description;
      $mcl_orgainsm = new MCL_CHADO_ORGANISM($args);
      if ($mcl_orgainsm->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'orgainsm', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'orgainsm', $args);
        return NULL;
      }
    }
    return $mcl_orgainsm;
  }

  /**
   * Adds N/A organism.
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function addNA() {

    // Checks if it has already exists.
    $mcl_organism = MCL_CHADO_ORGANISM::getOrganism('N/A', 'N/A');
    if (!$mcl_organism) {
      $details = array(
        'genus'   => 'N/A',
        'species' => 'N/A',
      );
      $mcl_organism = new MCL_CHADO_ORGANISM($details);
      if (!$mcl_organism->insert()) {
        return NULL;
      }
    }
    return $mcl_organism;
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
          if (!$this->addLink($mcl_tmpl, 'organism_image', 'organism_id', $this->organism_id, 'eimage_id', $mcl_image->getEimageID())) {
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
        $mcl_pub = MCL_CHADO_PUB::getPub($pub_id);
        if ($mcl_pub) {
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