<?php
/**
 * The declaration of MCL_CHADO_ND_EXPERIMENT class.
 *
 */
class MCL_CHADO_ND_EXPERIMENT extends CHADO_ND_EXPERIMENT {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_ND_EXPERIMENT::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_ND_EXPERIMENT::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_ND_EXPERIMENT by ID.
   *
   * @param integer $nd_experiment_id
   *
   * @return MCL_CHADO_ND_EXPERIMENT
   */
  public static function byID($nd_experiment_id) {
    return MCL_CHADO_ND_EXPERIMENT::byKey(array('nd_experiment_id' => $nd_experiment_id));
  }

  /**
   * @see CHADO_ND_EXPERIMENT::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existence of contact. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $project_id
   * @param string $cross_number
   * @param integer $ne_type_id
   * @param integer $nep_type_id
   * @param string key
   *   key of $GLOBALS variable for cross.
   *
   * @return boolean
   */
  public static function checkCrossNumber(MCL_TEMPLATE $mcl_tmpl = NULL, $project_id, $cross_number, $ne_type_id, $nep_type_id, $key = NULL) {
    if ($project_id && $cross_number) {

      // Gets the cross.
      $cross = MCL_CHADO_ND_EXPERIMENT::byCrossNumber($project_id, $cross_number, $ne_type_id, $nep_type_id);
      if (!$cross) {
        if ($key || !array_key_exists($cross_number, $GLOBALS[$key])) {
          self::updateMsg($mcl_tmpl, 'E', "cross ($project_id, $cross_number) not found in nd_experiment");
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Returns the related stock.
   *
   * @param integer $nes_type_id
   */
  public function getRelatedStock($nes_type_id) {

    // Gets nd_experiment.
    $args = array(
      'nd_experiment_id'  => $this->nd_experiment_id,
      'type_id'           => $nes_type_id,
    );
    $nd_experiment_stock = CHADO_ND_EXPERIMENT_STOCK::byKey($args);
    if ($nd_experiment_stock) {
      return MCL_CHADO_STOCK::byKey(array('stock_id' => $nd_experiment_stock->getStockID()));
    }
    return NULL;
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
    return $this->getProperty('nd_experimentprop', 'nd_experiment_id', $this->nd_experiment_id, $type_id);
  }

  /**
   * Returns the cross by cross number.
   *
   * @param integer $dataset_name
   * @param string $cross_number
   * @param integer $ne_type_id
   * @param integer $nep_type_id
   *
   * @return MCL_CHADO_ND_EXPERIMENT
   */
  public static function byCrossNumber($project_id, $cross_number, $ne_type_id, $nep_type_id) {

    if ($project_id && $cross_number) {

      // Gets the stock_id of the specified parent.
      $sql= "
        SELECT NE.nd_experiment_id
        FROM {chado.nd_experiment} NE
          INNER JOIN {chado.nd_experimentprop} NEP on NEP.nd_experiment_id = NE.nd_experiment_id
        WHERE NE.type_id = :ne_type_id
          AND NEP.type_id = :nep_type_id
          AND LOWER(NEP.value) = LOWER(:cross_number)
      ";
      $args = array(
        ':cross_number' => $project_id . '_' . $cross_number,
        ':ne_type_id'   => $ne_type_id,
        ':nep_type_id'  => $nep_type_id,
      );
      $results = db_query($sql, $args);
      $nd_experiment_id = $results->fetchField();
      if ($nd_experiment_id) {
        return MCL_CHADO_ND_EXPERIMENT::byKey(array('nd_experiment_id' => $nd_experiment_id));
      }
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_ND_EXPERIMENT by duplication key.
   *
   * @param string $cvterm_id
   * @param string $dup_value
   *
   * @return MCL_CHADO_ND_EXPERIMENT
   */
  public static function byDupKey($cvterm_id, $dup_value) {

    // Gets nd_experimentprop by duplication key value.
    $keys = array(
      'type_id' => $cvterm_id,
      'value'   => $dup_value,
    );
    $nd_experimentprop = CHADO_ND_EXPERIMENTPROP::byKey($keys);
    if ($nd_experimentprop) {
      return MCL_CHADO_ND_EXPERIMENT::byID($nd_experimentprop->getNdExperimentID());
    }
    return NULL;
  }

  /**
   * Adds a nd_experiment.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param integer $nd_geolocation_id
   * @param string $dup_value
   *
   * @return MCL_CHADO_ND_EXPERIMENT
   */
  public static function addNdExperiment(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $nd_geolocation_id, $cvterm_id, $dup_value) {

    // Sets the arguments.
    $args = array(
      'type_id'           => $type_id,
      'nd_geolocation_id' => $nd_geolocation_id,
      'cvterm_id'         => $cvterm_id,
      'dup_value'         => $dup_value,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $args = array(
      'type_id' => $cvterm_id,
      'value'   => $dup_value,
    );
    $chado_nd_experimentprop = CHADO_ND_EXPERIMENTPROP::byKey($args);
    if ($chado_nd_experimentprop) {
      self::updateMsg($mcl_tmpl, 'D', "nd_experiment of the dup_value ($dup_value) for $cvterm_id already exists");
      $nd_experiment = MCL_CHADO_ND_EXPERIMENT::byKey(array('nd_experiment_id' => $chado_nd_experimentprop->getNdExperimentID()));
    }
    else {

      // Adds a new nd_experiment.
      $args = array(
        'type_id'           => $type_id,
        'nd_geolocation_id' => $nd_geolocation_id,
      );
      $nd_experiment = new MCL_CHADO_ND_EXPERIMENT($args);
      if ($nd_experiment->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'nd_experiment', $args);

        // Adds a new nd_experimentprop.
        $args = array(
          'nd_experiment_id'  => $nd_experiment->getNdExperimentID(),
          'type_id'           => $cvterm_id,
          'value'             => $dup_value,
        );
        $chado_nd_experimentprop = new CHADO_ND_EXPERIMENTPROP($args);
        if ($chado_nd_experimentprop->insert()) {
          self::addMsg($mcl_tmpl, 'N', 'nd_experimentprop', $args);
        }
        else {
          self::addMsg($mcl_tmpl, 'E', 'nd_experimentprop', $args);
          return NULL;
        }
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'nd_experiment', $args);
        return NULL;
      }
    }
    return $nd_experiment;
  }

  /**
   * Adds a property by type ID (cvterm ID).
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $type_id
   * @param string $value
   * @param string $separator
   *
   * @return boolean
   */
  public function addPropByID(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $value, $separator = '') {
    if ($value != '') {
      return $this->addProperty($mcl_tmpl, 'nd_experimentprop', 'nd_experiment_id', $this->nd_experiment_id, $type_id, $value, $separator);
    }
    return TRUE;
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
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->addProperty($mcl_tmpl, 'nd_experimentprop', 'nd_experiment_id', $this->nd_experiment_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Updates the property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   *
   * @return boolean
   */
  public function updateProp(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $value) {
    if ($value != '') {
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
      return $this->updateProperty($mcl_tmpl, 'nd_experimentprop', 'nd_experiment_id', $this->nd_experiment_id, $type_id, $value);
    }
    return TRUE;
  }

  /**
   * Adds stock to nd_experiment_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_STOCK $stock
   *
   * @return boolean
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_STOCK $stock, $type_id) {
    if ($stock && $type_id) {
      return $this->addLink($mcl_tmpl, 'nd_experiment_stock', 'nd_experiment_id', $this->nd_experiment_id, 'stock_id', $stock->getStockID(), $type_id);
    }
    return TRUE;
  }

  /**
   * Updates the stock_id in nd_experiment_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $stock_id
   *
   * @return boolean
   */
  public function updateStockByID(MCL_TEMPLATE $mcl_tmpl = NULL, $stock_id, $type_id) {
    if ($type_id) {

      // Sets properties.
      $args = array(
        'nd_experiment_id'  => $this->nd_experiment_id,
        'type_id'           => $type_id,
      );
      $nes = CHADO_ND_EXPERIMENT_STOCK::byKey($args);
      if ($nes) {

        // Updates the relationship.
        if ($stock_id) {
          $nes->setStockID($stock_id);
          return $nes->update();
        }

        // Removes the relationship.
        else {
          return $nes->delete();
        }
      }
      else {
        $args['stock_id'] = $stock_id;
        $nes = new CHADO_ND_EXPERIMENT_STOCK($args);
        return $nes->insert();
      }
    }
    return FALSE;
  }

  /**
   * Adds phenotype to nd_experiment_phenotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_PHENOTYPE $phenotype
   *
   * @return boolean
   */
  public function addPhenotype(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_PHENOTYPE $phenotype) {
    if ($phenotype) {
      return $this->addLink($mcl_tmpl, 'nd_experiment_phenotype', 'nd_experiment_id', $this->nd_experiment_id, 'phenotype_id', $phenotype->getPhenotypeID());
    }
    return TRUE;
  }

  /**
   * Adds genotype to nd_experiment_genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_GENOTYPE $genotype
   *
   * @return boolean
   */
  public function addGenotype(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_GENOTYPE $genotype) {
    if ($genotype) {
      return $this->addLink($mcl_tmpl, 'nd_experiment_genotype', 'nd_experiment_id', $this->nd_experiment_id, 'genotype_id', $genotype->getGenotypeID());
    }
    return TRUE;
  }

  /**
   * Adds dataset to nd_experiment_project.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   *
   * @return boolean
   */
  public function addDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $dataset_name) {
    if ($dataset_name) {
      $dataset = MCL_CHADO_DATASET::byName($dataset_name);
      if (!$dataset) {
        return FALSE;
      }
      return $this->addLink($mcl_tmpl, 'nd_experiment_project', 'nd_experiment_id', $this->nd_experiment_id, 'project_id', $dataset->getProjectID());
    }
    return TRUE;
  }

  /**
   * Returns the evaluators.
   *
   * @param string $separator
   *
   * @return string
   */
  public function getEvaluators($separator = ';') {

    // Gets all contacts.
    $sql = "
      SELECT C.name
      FROM {chado.contact} C
        INNER JOIN {chado.nd_experiment_contact} NEC on NEC.contact_id = C.contact_id
      WHERE NEC.nd_experiment_id = :nd_experiment_id
    ";
    $results = db_query($sql, array(':nd_experiment_id' => $this->nd_experiment_id));
    $contact_arr = array();
    while ($name = $results->fetchField()) {
      $contact_arr []= $name;
    }

    // Returns the list of the contacts.
    return implode($separator, $contact_arr);
  }

  /**
   * Adds contact to nd_experiment_contact.
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
        $name = trim($name);
        $contact = MCL_CHADO_CONTACT::byName($name);
        if ($contact) {
          if (!$this->addLink($mcl_tmpl, 'nd_experiment_contact', 'nd_experiment_id', $this->nd_experiment_id, 'contact_id', $contact->getContactID())) {
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
   * Adds reference to feature_pub.
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
        $pub = MCL_CHADO_PUB::getPub($pub_id);
        if ($pub) {
          if (!$this->addLink($mcl_tmpl, 'nd_experiment_pub', 'nd_experiment_id', $this->nd_experiment_id, 'pub_id', $pub_id)) {
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