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
   *
   * @return boolean
   */
  public static function checkDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $name) {
    if ($name) {

      // Gets the dataset.
      $mcl_dataset = MCL_CHADO_DATASET::getDataset($name);
      if (!$mcl_dataset) {
        self::updateMsg($mcl_tmpl, 'E', "(name) = ($name) not found in project");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks the existance of dataset by ID. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $project_id
   *
   * @return boolean
   */
  public static function checkDatasetByID(MCL_TEMPLATE $mcl_tmpl = NULL, $project_id) {
    if ($project_id) {

      // Gets the dataset.
      $mcl_dataset = MCL_CHADO_DATASET::byKey(array('project_id' => $project_id));
      if (!$mcl_dataset) {
        self::updateMsg($mcl_tmpl, 'E', "(project_id) = ($project_id) not found in project");
        return FALSE;
      }
    }
    return TRUE;
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
   * Gets property.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getProp($cv_name, $cvterm_name) {
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    return $this->getProperty('projectprop', 'project_id', $this->project_id, $type_id);
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
  public static function addDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $name) {

    // Sets the arguments.
    $args = array('name' => $name);

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_dataset = MCL_CHADO_DATASET::byKey($args);
    if ($mcl_dataset) {
      self::addMsg($mcl_tmpl, 'D', 'project', $args);
    }
    else {

      // Adds a new dataset.
      $mcl_dataset = new MCL_CHADO_DATASET($args);
      if ($mcl_dataset->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'project', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'project', $args);
        return NULL;
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
   *
   * @return boolean
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value, $separator = '') {
    if ($value) {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'projectprop', 'project_id', $this->project_id, $type_id, $value, $separator);
    }
    return TRUE;
   }

  /**
   * Adds contact to project_contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   *
   * @return boolean
   */
  public function addContact(MCL_TEMPLATE $mcl_tmpl = NULL, $contact, $separator = '') {
    $flag = TRUE;
    if ($contact) {
      $names = preg_split($this->getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $mcl_contact = MCL_CHADO_CONTACT::byKey(array('name' => trim($name)));
        if ($mcl_contact) {
          if (!$this->addLink($mcl_tmpl, 'project_contact', 'project_id', $this->project_id, 'contact_id', $mcl_contact->getContactID())) {
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
   * Adds a perent dataset.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   *
   * @return boolean
   */
  public function addParentDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $dataset_name) {
    if ($dataset_name) {
      $parent = MCL_CHADO_DATASET::getDataset($dataset_name);
      if ($parent) {
        $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'is_a_subproject_of')->getCvtermID();
        return $this->addLink($mcl_tmpl, 'project_relationship', 'subject_project_id', $this->project_id, 'object_project_id', $parent->getProjectID(), $type_id);
      }
    }
    return TRUE;
  }

  /**
   * Adds reference to project_pub.
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
          if ($this->addLink($mcl_tmpl, 'project_pub', 'project_id', $this->project_id, 'pub_id', $pub_id)) {
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