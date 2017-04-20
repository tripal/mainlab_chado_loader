<?php
/**
 * The declaration of MCL_TEMPLATE_DATASET_BIMS class.
 *
 */
class MCL_TEMPLATE_DATASET_BIMS extends MCL_TEMPLATE {

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
      'pi'                    => array('req' => FALSE, 'width' => 12, 'desc' => "Principal Investigator (eg. breeder, corresponding author of the QTL paper) of the Dataset.   It should match a 'contact_name' column in 'Contact' sheet."),
      'crop'                  => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the crop for the dataset."),
      'comments'              => array('req' => FALSE, 'width' => 15, 'desc' => "Any comments for the dataset."),
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
    $cvterms['SITE_CV']['description']        = -1;
    $cvterms['SITE_CV']['comments']           = -1;
    $cvterms['SITE_CV']['crop']               = -1;
    $cvterms['SITE_CV']['permission']         = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks super_datatset.
    MCL_CHADO_DATASET::checkDataset($this, $line['super_datatset']);

    // Checks PI.
    MCL_CHADO_CONTACT::checkContact($this, $line['pi']);
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Adds a dataset for BIMS. Sets the database name [program_id + dataset_name].
    $program_id = $this->getJob()->getParamByKey('program_id');
    $dataset_name = $program_id . '.' . $line['dataset_name'];
    $dataset = MCL_CHADO_DATASET::addDataset($this, $dataset_name);
    if ($dataset) {

      // Adds properties.
      $dataset->addProp($this, 'SITE_CV', 'project_type', $line['type']);
      $dataset->addProp($this, 'SITE_CV', 'description', $line['description']);
      $dataset->addProp($this, 'SITE_CV', 'crop', $line['crop']);
      $dataset->addProp($this, 'SITE_CV', 'comments', $line['comments']);
      $dataset->addProp($this, 'SITE_CV', 'permission', $line['permission']);

      // Adds PI.
      $dataset->addContact($this, $line['pi']);
    }
  }
}