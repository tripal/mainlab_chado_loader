<?php
/**
 * The declaration of MCL_TEMPLATE_PROGENY class.
 *
 */
class MCL_TEMPLATE_PROGENY extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
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
    $desc = 'The description for field book sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'progeny_name'    => array('req' => TRUE,  'width' => 14, 'desc' => ""),
      'alias'           => array('req' => FALSE, 'width' => 14, 'desc' => "Record all the aliases for the cross here.  The format is 'Alias type1:Alias1; Alias type2:Alias2'. When there is no specific type for an alias, just write the aliases without the specific type. (eg. Breeder:LG98-942; Institution:4321;2-F04-01)."),
      'genus'           => array('req' => TRUE,  'width' => 10, 'desc' => ""),
      'species'         => array('req' => TRUE,  'width' => 10, 'desc' => "When only the genus is known, write 'sp.' in the species."),
      'dataset_name'    => array('req' => TRUE,  'width' => 14, 'desc' => ""),
      'cross_number'    => array('req' => TRUE,  'width' => 10, 'desc' => "Specific ID of a cross that generated the progeny; Must also be found in the Cross table."),
      'description'     => array('req' => TRUE,  'width' => 14, 'desc' => "Any other descriptive data about the selection or seedling."),
      'advanced_to_2nd' => array('req' => TRUE,  'width' => 10, 'desc' => "Yes if it is advanced to 2nd phase, No otherwise. Leave it empty if the selection decision is yet to be made."),
      'advanced_to_3rd' => array('req' => TRUE,  'width' => 10, 'desc' => "Yes if it is advanced to 3rd phase, No otherwise."),
      'advanced_to_4th' => array('req' => TRUE,  'width' => 10, 'desc' => "Yes if it is advanced to 4th phase, No otherwise."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['progeny']                  = -1;
    $cvterms['SITE_CV']['cross_number']             = -1;
    $cvterms['SITE_CV']['is_a_maternal_parent_of']  = -1;
    $cvterms['SITE_CV']['is_a_paternal_parent_of']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks organism.
    MCL_CHADO_ORGANISM::checkOrganism($this, $line['genus'], $line['species']);

    // Checks cross_number. If cross_number is not empty, check dataset_name.
    MCL_CHADO_ND_EXPERIMENT::checkCrossNumber($this, $line['dataset_name'], $line['cross_number']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the stock.
    $stock = NULL;
    if ($stock) {
      // Adds a progeny.
      $stock->addProgeny($this, $line['dataset_name'], $line['cross_number']);
    }
  }
}