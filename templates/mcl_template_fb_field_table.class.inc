<?php
/**
 * The declaration of MCL_TEMPLATE_FB_FIELD_TABLE class.
 *
 */
class MCL_TEMPLATE_FB_FIELD_TABLE extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'PHENOTYPE';
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
    $desc = 'The description for FB_FIELD_TABLE sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'stock_name'  => array('req' => FALSE,  'width' => 10, 'desc' => "Name of stock."),
      'trial'       => array('req' => FALSE,  'width' => 15, 'desc' => "Name of the trial."),
      'location'    => array('req' => FALSE,  'width' => 10, 'desc' => "Name of the location."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['phenotyping']              = -1;
    $cvterms['SITE_CV']['sample']                   = -1;
    $cvterms['SITE_CV']['sample_of']                = -1;
    $cvterms['BIMS_FIELD_BOOK']['unique_id']        = -1;
    $cvterms['BIMS_FIELD_BOOK']['primary_order']    = -1;
    $cvterms['BIMS_FIELD_BOOK']['secondary_order']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Gets the program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');

    // Adds prefix.
    $trial_name   = $program_id . '.' . $line['trial'];
    $location_nam = $program_id . '.' . $line['location'];

    // Checks trial.
    MCL_CHADO_DATASET::checkDataset($this, $trial_name);

    // Checks location.
    MCL_CHADO_LOCATION::checkSite($this, $location_name);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the program_id.
    $program_id = $this->getJob()->getParamByKey('program_id');







   }
}