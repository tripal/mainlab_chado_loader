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
   */
  public static function checkOrganism(MCL_TEMPLATE $mcl_tmpl, $genus, $species, $separator = '') {
    if ($genus && $species) {

      $sps = preg_split($mcl_tmpl->getSepRegex($separator), $species, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($sps as $sp) {
        $sp = trim($sp);

        // Gets the orgainsm.
        $mcl_orgainsm = MCL_CHADO_ORGANISM::getOrganism($genus, $sp);
        if (!$mcl_orgainsm) {
          $mcl_tmpl->updateMsg('E', "(genus, species) = ($genus, $sp) not found in organism");
        }
      }
    }
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
   * Adds null organism.
   *
   * @return MCL_CHADO_ORGANISM
   */
  public static function addNull() {

    // Gets null cvterm.
    $mcl_cvterm = MCL_CHADO_CVTERM::addNull();

    // Checks if it has already exists.
    $mcl_organism = MCL_CHADO_ORGANISM::getOrganism('null', 'null');
    if (!$mcl_organism) {
      $details = array(
        'genus'   => 'null',
        'species' => 'null',
      );
      $mcl_organism = new MCL_CHADO_ORGANISM($details);
      if (!$mcl_organism->insert()) {
        return NULL;
      }
    }
    return $mcl_organism;
  }
}