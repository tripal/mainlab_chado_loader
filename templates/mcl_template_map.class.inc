<?php
/**
 * The declaration of MCL_TEMPLATE_MAP class.
 *
 */
class MCL_TEMPLATE_MAP extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MAP';
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
    $desc = 'The description for map sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'map_name'        => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the map given by the database curator - follow each database's naming convention."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => "Genus of the mapping population."),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "species of the mapping population. Multiple species can be entered with ';' in between for the mapping populations from inter-specific crosses."),
      'map_unit'        => array('req' => TRUE,  'width' => 10, 'desc' => "cM, bp, etc."),
      'analysis_method' => array('req' => FALSE, 'width' => 10, 'desc' => "Analysis method for the mapping."),
      'software'        => array('req' => FALSE, 'width' => 10, 'desc' => "Software used for map construction."),
      'map_type'        => array('req' => FALSE, 'width' => 10, 'desc' => "genetic, physical, cytogenetic, sequence."),
      'population_type' => array('req' => FALSE, 'width' => 10, 'desc' => "F2, BC1, RIL, etc."),
      'population_size' => array('req' => FALSE, 'width' => 10, 'desc' => "Size of population."),
      'population_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the mapping population. It should match 'stock_name' column of the Stock sheet."),
      'source_url'      => array('req' => FALSE, 'width' => 10, 'desc' => "Map data source website."),
      'comments'        => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the map."),
      'genome_group'    => array('req' => FALSE, 'width' => 10, 'desc' => "Specific genome groups if available (eg. A, D etc for cotton)."),
      'contact'         => array('req' => FALSE, 'width' => 10, 'desc' => "Contact person for the map."),
      'reference'       => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id of the reference."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['alias']           = -1;
    $cvterms['SITE_CV']['analysis_method'] = -1;
    $cvterms['SITE_CV']['software']        = -1;
    $cvterms['SITE_CV']['map_type']        = -1;
    $cvterms['SITE_CV']['population_type'] = -1;
    $cvterms['SITE_CV']['population_size'] = -1;
    $cvterms['SITE_CV']['source_url']      = -1;
    $cvterms['SITE_CV']['comments']        = -1;
    $cvterms['SITE_CV']['genome_group']    = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks organism for map.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species'], '[;]');

    // Checks map_unit.
    MCL_CHADO_FEATUREMAP::checkMapUnit($this, $line['map_unit']);

    // Checks stock.
    MCL_CHADO_STOCK::checkStock($this, $line['population_name'], '', '', "[;]");

    // Checks contacts.
    MCL_CHADO_CONTACT::checkContact($this, $line['contact'], '[;]');

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a map.
    $map = MCL_CHADO_FEATUREMAP::addMap($this, $line['map_name'], $line['map_unit'], $line['description']);
    if ($map) {

      // Adds the properties.
      $map->addProp($this, 'SITE_CV', 'analysis_method', $line['analysis_method']);
      $map->addProp($this, 'SITE_CV', 'software', $line['software']);
      $map->addProp($this, 'SITE_CV', 'map_type', $line['map_type']);
      $map->addProp($this, 'SITE_CV', 'population_type', $line['population_type']);
      $map->addProp($this, 'SITE_CV', 'population_size', $line['population_size']);
      $map->addProp($this, 'SITE_CV', 'source_url', $line['source_url']);
      $map->addProp($this, 'SITE_CV', 'genome_group', $line['genome_group']);
      $map->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $map, $line);

      // Adds the organisms.
      $map->addOrganism($this, $line['genus'], $line['species'], "[;]");

      // Adds the stocks.
      $map->addStock($this, $line['population_name'], '', '', "[;]");

      // Adds the contacts.
      $map->addContact($this, $line['contact'], "[;]");

      // Adds the references.
      $map->addReference($this, $line['reference'], "[,;]");
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Lists the job paremters.
    $cv_name = $this->getJob()->getParamByKey('cv');

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($cv_name, 'SITE_CV');
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $data_lines[0])) {
      $this->updateMsg('E', "Failed to save the special columns");
    }
  }
}