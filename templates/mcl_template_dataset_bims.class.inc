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
      'dataset_name'    => array('req' => TRUE,  'width' => 20, 'desc' => "Names of the datasets."),
      'comments'        => array('req' => FALSE, 'width' => 15, 'desc' => "Any comments for the dataset."),
      'description'     => array('req' => FALSE, 'width' => 90, 'desc' => "General description of the dataset (purpose, target, how many crosses, etc)."),
      '##property_name' => array('req' => FALSE, 'width' => 10, 'desc' => "Special columns (##) : followed by name (cvterm name) of the property."),
    );
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
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks PI.
    MCL_CHADO_CONTACT::checkContact($this, $line['pi'], "[;]");
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the job paremters.
    $program_id = $this->getJob()->getParamByKey('program_id');
    $crop       = $this->getJob()->getParamByKey('crop');
    $pi         = $this->getJob()->getParamByKey('PI');

    // Adds a dataset for BIMS. Sets the database name [program_id + dataset_name].
    $dataset_name = $program_id . '.' . $line['dataset_name'];
    $dataset = MCL_CHADO_DATASET::addDataset($this, $dataset_name);
    if ($dataset) {

      // Adds the properties.
      $dataset->addProp($this, 'SITE_CV', 'project_type', 'breeding');
      $dataset->addProp($this, 'SITE_CV', 'crop', $crop);
      $dataset->addProp($this, 'SITE_CV', 'description', $line['description']);
      $dataset->addProp($this, 'SITE_CV', 'comments', $line['comments']);

      // Adds the user-specific properties.
      $this->addSpecialColumns('##', $dataset, $line);

      // Adds PI.
      $dataset->addContact($this, $pi, "[;]");
    }
  }

  /**
   * @see MCL_TEMPLATE::preProcess()
   */
  public function preProcess($data_lines) {

    // Gets the cv.
    $cv = MCL_CHADO_CV::getCV($this->getJob()->getParamByKey('cv'));
    if (!$cv) {
      $this->updateMsg('E', "cv does not exist.");
      return;
    }

    // Saves the user-specific properties.
    if (!$this->saveSpecialColumns('##', $cv->getCvID(), $data_lines[0])) {
      return;
    }
  }
}