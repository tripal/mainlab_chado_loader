<?php
/**
 * The declaration of MCL_TEMPLATE_GENOTYPE_SNP class.
 *
 */
class MCL_TEMPLATE_GENOTYPE_SNP extends MCL_TEMPLATE {

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
    $desc = 'The description for genotype-snp sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'  => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genotyping dataset. It should match a dataset_name in 'Dataset' sheet."),
      'stock_name'    => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the stock genotyped. It should match an 'stock_namme' of the 'Stock' sheet."),
      'clone_id'      => array('req' => FALSE, 'width' => 10, 'label' => 'clone_ID', 'desc' => "ID of a clone if known."),
      'sample_id'     => array('req' => TRUE,  'width' => 10, 'label' => 'sample_ID', 'desc' => "ID of a sample. Normally it is the same as ID unless the user want to keep tract of genotype of a specific sample."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['clone']   = -1;
    $cvterms['SITE_CV']['sample']  = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {
  }
}