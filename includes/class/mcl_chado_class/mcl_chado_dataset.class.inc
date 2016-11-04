<?php
/**
 * The declaration of MCL_CHADO_DATASET class.
 *
 */
class MCL_CHADO_DATASET extends CHADO_PROJECT {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_PROJECT::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_PROJECT::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_PROJECT::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of dataset. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   */
  public static function checkDataset(MCL_TEMPLATE $mcl_tmpl, $name) {
    if ($name) {

      // Gets the dataset.
      $mcl_dataset = MCL_CHADO_DATASET::getDataset($name);
      if (!$mcl_dataset) {
        $mcl_tmpl->updateMsg('E', "(name) = ($name) not found in project");
      }
    }
  }

  /**
   * Returns the dataset by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_DATASET
   */
  public static function getDataset($name) {
    return MCL_CHADO_DATASET::bykey(array('name' => $name));
  }

  /**
   * Adds a dataset.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   * @param integer $organism_id
   * @param string $name
   *
   * @return MCL_CHADO_DATASET
   */
  public static function addDataset(MCL_TEMPLATE $mcl_tmpl, $name) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_dataset = MCL_CHADO_DATASET::byKey($args);
    if ($mcl_dataset) {
      $mcl_tmpl->addMsg('D', 'project', $args);
    }
    else {

      // Adds a new dataset.
      $mcl_dataset = new MCL_CHADO_DATASET($args);
      if ($mcl_dataset->insert()) {
        $mcl_tmpl->addMsg('N', 'project', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'project', $args);
      }
    }
    return $mcl_dataset;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param string $separator
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $value, $separator = '') {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    $mcl_tmpl->addProperty('projectprop', 'project_id', $this->project_id, $type_id, $value, $separator);
  }

  /**
   * Gets property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name) {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    return $mcl_tmpl->getProperty('projectprop', 'project_id', $this->project_id, $type_id);
  }

  /**
   * Adds contact to project_contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   */
  public function addContact(MCL_TEMPLATE $mcl_tmpl, $contact, $separator = '') {
    if ($contact) {
      $names = preg_split($mcl_tmpl->getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_contact = MCL_CHADO_CONTACT::byKey(array('name' => $name));
        if ($mcl_contact) {
          $mcl_tmpl->addLink('project_contact', 'project_id', $this->project_id, 'contact_id', $mcl_contact->getContactID());
        }
      }
    }
  }

  /**
   * Adds a perent dataset.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   */
  public function addParentDataset(MCL_TEMPLATE $mcl_tmpl, $dataset_name) {
    if ($dataset_name) {
      $parent = MCL_CHADO_DATASET::getDataset($dataset_name);
      if ($parent) {
        $type_id = $mcl_tmpl->getCvtermID('SITE_CV', 'is_a_subproject_of');
        $mcl_tmpl->addLink('project_relationship', 'subject_project_id', $this->project_id, 'object_project_id', $parent->getProjectID(), $type_id);
      }
    }
  }

  /**
   * Adds reference to project_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   */
  public function addReference(MCL_TEMPLATE $mcl_tmpl, $reference, $separator = '') {
    if ($reference) {
      $pub_ids = preg_split($mcl_tmpl->getSepRegex($separator), $reference, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($pub_ids as $pub_id) {
        $pub_id = trim($pub_id);
        $mcl_pub = MCL_CHADO_PUB::getPub($pub_id);
        if ($mcl_pub) {
          $mcl_tmpl->addLink('project_pub', 'project_id', $this->project_id, 'pub_id', $pub_id);
        }
      }
    }
  }
}