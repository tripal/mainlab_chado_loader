<?php
/**
 * The declaration of MCL_TEMPLATE_DATASET class.
 *
 */
class MCL_TEMPLATE_DATASET extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'DATASET';
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
    $desc = 'The description for dataset sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'dataset_name'          => array('req' => TRUE,  'width' => 20, 'desc' => "Names of the datasets."),
      'type'                  => array('req' => TRUE,  'width' => 12, 'desc' => "Breeding, growing (for cultivar performance data), diversity (for study of genetic diversity) or QTL."),
      'sub_type'              => array('req' => FALSE, 'width' => 12, 'desc' => "To specify whether the markers used were SSR or SNP when the dataset is SSR or SNP genotyping data."),
      'super_dataset'         => array('req' => FALSE, 'width' => 16, 'desc' => "Larger dataset that this sub_dataset belngs to."),
      'trait_descriptor_set'  => array('req' => FALSE, 'width' => 16, 'desc' => "The name of the trait descriptor set that each dataset or breeder uses. This is a required column if a type of a dataset is 'breeding'."),
      'pi'                    => array('req' => FALSE, 'width' => 12, 'desc' => "Principal Investigator (eg. breeder, corresponding author of the QTL paper) of the Dataset.   It should match a 'contact_name' column in 'Contact' sheet."),
      'crop'                  => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the crop for the dataset."),
      'comments'              => array('req' => FALSE, 'width' => 15, 'desc' => "Any comments for the dataset."),
      'reference'             => array('req' => FALSE, 'width' => 10, 'desc' => "pub_id for any reference if the dataset is from publication."),
      'permission'            => array('req' => FALSE, 'width' => 10, 'desc' => "Permission of the dataset. '0' or empty indicates public. Other positive integer indicates private."),
      'description'           => array('req' => FALSE, 'width' => 90, 'desc' => "General description of the dataset (purpose, target, how many crosses, etc)."),
    );

    // Adds labels.
    $headers['pi']['label'] = 'PI';
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['project_type']       = -1;
    $cvterms['SITE_CV']['trait_descriptor']   = -1;
    $cvterms['SITE_CV']['sub_type']           = -1;
    $cvterms['SITE_CV']['description']        = -1;
    $cvterms['SITE_CV']['comments']           = -1;
    $cvterms['SITE_CV']['crop']               = -1;
    $cvterms['SITE_CV']['is_a_subproject_of'] = -1;
    $cvterms['SITE_CV']['permission']         = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks 'trait_descriptor_set' column. The column becomes required column
    // if dataset type is 'breeding'.
    if ($line['type'] == 'breeding') {
      MCL_CHADO_CV::checkCV($this, $line['trait_descriptor_set']);
    }

    // Checks super_datatset.
    MCL_CHADO_DATASET::checkDataset($this, $line['super_datatset']);

    // Checks PI.
    MCL_CHADO_CONTACT::checkContact($this, $line['pi']);

    // Checks references.
    MCL_CHADO_PUB::checkPub($this, $line['reference'], '[;,]');
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a dataset.
    $dataset = MCL_CHADO_DATASET::addDataset($this, $line['dataset_name']);
    if ($dataset) {

      // Adds properties.
      $dataset->addProp($this, 'SITE_CV', 'project_type', $line['type']);
      $dataset->addProp($this, 'SITE_CV', 'trait_descriptor', $line['trait_descriptor_set']);
      $dataset->addProp($this, 'SITE_CV', 'sub_type', $line['sub_type']);
      $dataset->addProp($this, 'SITE_CV', 'description', $line['description']);
      $dataset->addProp($this, 'SITE_CV', 'crop', $line['crop']);
      $dataset->addProp($this, 'SITE_CV', 'comments', $line['comments']);
      $dataset->addProp($this, 'SITE_CV', 'permission', $line['permission']);

      // Adds PI.
      $dataset->addContact($this, $line['pi']);

      // Adds a parent.
      $dataset->addParentDataset($this, $line['super_datatset']);

      // Adds references.
      $dataset->addReference($this, $line['reference'], "[;,]");
    }
  }
}