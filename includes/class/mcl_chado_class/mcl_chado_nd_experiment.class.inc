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
   * @see CHADO_ND_EXPERIMENT::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of contact. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   */
  public static function checkCrossNumber(MCL_TEMPLATE $mcl_tmpl, $dataset_name, $cross_number) {
    if ($cross_number) {

      // Checks for dataset_name.
      if ($dataset_name) {
        $dataset = MCL_CHADO_DATASET::getDataset($dataset_name);
        if ($dataset) {

          // Checks cross_number.
          $sql = "
            SELECT NEP.nd_experiment_id
            FROM {chado.nd_experimentprop} NEP
              INNER JOIN {chado.nd_experiment_project} NEPR on NEPR.nd_experiment_id = NEP.nd_experiment_id
            WHERE NEPR.project_id = :project_id
              AND NEP.type_id = :type_id
              AND LOWER(NEP.value) = LOWER(:value)
          ";
          $args = array(
            ':project_id' => $dataset->getProjectID(),
            ':value'      => $cross_number,
            ':type_id'    => $mcl_tmpl->cvterms['SITE_CV']['cross_number'],
          );
          $results = db_query($sql, $args);
          $nd_experiment_id = $results->fetchField();
          if ($nd_experiment_id) {

            // Checks maternal parent.
            $args = array(
              'nd_experiment_id'  => $nd_experiment_id,
              'type_id'           => $mcl_tmpl->cvterms['SITE_CV']['maternal_parent'],
            );
            $nd_experiment_stock = CHADO_ND_EXPERIMENT_STOCK::byKey($args);

            if (!$nd_experiment_stock) {
              $mcl_tmpl->updateMsg('E', "The maternal parent of the cross ($cross_number) does not found");
            }

            // Checks paternal parent.
            $args = array(
              'nd_experiment_id'  => $nd_experiment_id,
              'type_id'           => $mcl_tmpl->cvterms['SITE_CV']['paternal_parent'],
            );
            $nd_experiment_stock = CHADO_ND_EXPERIMENT_STOCK::byKey($args);
            if (!$nd_experiment_stock) {
              $mcl_tmpl->updateMsg('E', "The paternal parent of the cross ($cross_number) does not found");
            }
          }
        }
        else {
          $mcl_tmpl->updateMsg('E', "dataset_name ($dataset_name) not found in project");
        }
      }
      else {
        $mcl_tmpl->updateMsg('E', "Since cross_number is not empty, dataset_name must be specified");
      }
    }
  }

  /**
   * Returns parent by cross number.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $cvterm_id_parent
   * @param string $dataset_name
   * @param string cross_number
   *
   * @return MCL_CHADO_STOCK
   */
  public static function getParentByCrossNumber(MCL_TEMPLATE $mcl_tmpl, $cvterm_id_parent, $dataset_name, $cross_number) {

    // Gets project_id.
    $dataset = MCL_CHADO_DATASET::getDataset($dataset_name);
    if ($dataset) {

      // Gets the stock_id of the specified parent.
      $sql= "
        SELECT NES.stock_id
        FROM {chado.nd_experiment_stock} NES
          INNER JOIN {chado.nd_experimentprop} NEP on NEP.nd_experiment_id = NES.nd_experiment_id
          INNER JOIN {chado.nd_experiment_project} NEPR on NEPR.nd_experiment_id = NES.nd_experiment_id
        WHERE NES.type_id = :type_id_nes
          AND NEP.type_id = :type_id_nep
          AND NEPR.project_id = :project_id
          AND LOWER(NEP.value) = LOWER(:cross_number)
      ";
      $args = array(
        ':type_id_nes'  => $cvterm_id_parent,
        ':type_id_nep'  => $mcl_tmpl->getCvtermID('SITE_CV', 'cross_number'),
        ':project_id'   => $dataset->getProjectID(),
        ':cross_number' => $cross_number,
      );
      $results = db_qery($sql, $args);
      $stock_id = $results->fetchField();
      if ($stock_id) {
        return MCL_CHADO_STOCK::byKey(array('stock_id' => $stock_id));
      }
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
  public static function addNdExperiment(MCL_TEMPLATE $mcl_tmpl, $type_id, $nd_geolocation_id, $cvterm_id, $dup_value) {

    // Sets the arguments.
    $args = array(
      'type_id'           => $type_id,
      'nd_geolocation_id' => $nd_geolocation_id,
      'cvterm_id'         => $cvterm_id,
      'dup_value'         => $dup_value,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $args = array(
      'type_id' => $cvterm_id,
      'value'   => $dup_value,
    );
    $chado_nd_experimentprop = CHADO_ND_EXPERIMENTPROP::byKey($args);
    if ($chado_nd_experimentprop) {
      $mcl_tmpl->updateMsg('D', "nd_experiment of the dup_value ($dup_value) for $cvterm_id already exists");
      $mcl_nd_experiment = MCL_CHADO_ND_EXPERIMENT::byKey(array('nd_experiment_id' => $chado_nd_experimentprop->getNdExperimentID()));
    }
    else {

      // Adds a new nd_experiment.
      $args = array(
        'type_id'           => $type_id,
        'nd_geolocation_id' => $nd_geolocation_id,
      );
      $mcl_nd_experiment = new MCL_CHADO_ND_EXPERIMENT($args);
      if ($mcl_nd_experiment->insert()) {
        $mcl_tmpl->addMsg('N', 'nd_experiment', $args);

        // Adds a new nd_experimentprop.
        $args = array(
          'nd_experiment_id'  => $mcl_nd_experiment->getNdExperimentID(),
          'type_id'           => $cvterm_id,
          'value'             => $dup_value,
        );
        $chado_nd_experimentprop = new CHADO_ND_EXPERIMENTPROP($args);
        if ($chado_nd_experimentprop->insert()) {
          $mcl_tmpl->addMsg('N', 'nd_experimentprop', $args);
        }
        else {
          $mcl_tmpl->addMsg('E', 'nd_experimentprop', $args);
        }
      }
      else {
        $mcl_tmpl->addMsg('E', 'nd_experiment', $args);
      }
    }
    return $mcl_nd_experiment;
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
    $mcl_tmpl->addProperty('featuremapprop', 'featuremap_id', $this->featuremap_id, $type_id, $value, $separator);
  }

  /**
   * Adds stock to featuremap_organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $genus
   * @param integer $species
   * @param string $separator
   */
  public function addOrganism(MCL_TEMPLATE $mcl_tmpl, $genus, $species, $separator = '') {
    if ($genus && $species) {
      $sps = preg_split($mcl_tmpl->getSepRegex($separator), $species, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($sps as $sp) {
        $sp = trim($sp);

        // Gets the organism.
        $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $sp);
        if ($mcl_organism) {
          $mcl_tmpl->addLink('featuremap_organism', 'featuremap_id', $this->featuremap_id, 'organism_id', $mcl_organism->getOrganismID());
        }
        else {
          $mcl_tmpl->updateMsg('E', "(genus, species) = ($genus, $sp) not found in organism");
        }
      }
    }
  }

  /**
   * Adds stock to nd_experiment_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_STOCK $stock
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_STOCK $mcl_stock, $type_id) {
    if ($mcl_stock && $type_id) {
      $mcl_tmpl->addLink('nd_experiment_stock', 'nd_experiment_id', $this->nd_experiment_id, 'stock_id', $mcl_stock->getStockID(), $type_id);
    }
  }

  /**
   * Adds phenotype to nd_experiment_phenotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_PHENOTYPE $phenotype
   */
  public function addPhenotype(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_PHENOTYPE $mcl_phenotype) {
    if ($mcl_phenotype) {
      $mcl_tmpl->addLink('nd_experiment_phenotype', 'nd_experiment_id', $this->nd_experiment_id, 'phenotype_id', $mcl_phenotype->getPhenotypeID());
    }
  }

  /**
   * Adds genotype to nd_experiment_genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_GENOTYPE $genotype
   */
  public function addGenotype(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_GENOTYPE $mcl_genotype) {
    if ($mcl_genotype) {
      $mcl_tmpl->addLink('nd_experiment_genotype', 'nd_experiment_id', $this->nd_experiment_id, 'genotype_id', $mcl_genotype->getGenotypeID());
    }
  }

  /**
   * Adds dataset to nd_experiment_project.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   */
  public function addDataset(MCL_TEMPLATE $mcl_tmpl, $dataset_name) {
    if ($dataset_name) {
      $mcl_dataset = MCL_CHADO_DATASET::getDataset($dataset_name);
      $mcl_tmpl->addLink('nd_experiment_project', 'nd_experiment_id', $this->nd_experiment_id, 'project_id', $mcl_dataset->getProjectID());
    }
  }

  /**
   * Adds contact to nd_experiment_contact.
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
        $mcl_contact = MCL_CHADO_CONTACT::getContact($name);
        if ($mcl_contact) {
          $mcl_tmpl->addLink('nd_experiment_contact', 'nd_experiment_id', $this->nd_experiment_id, 'contact_id', $mcl_contact->getContactID());
        }
      }
    }
  }

  /**
   * Adds reference to feature_pub.
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
          $mcl_tmpl->addLink('fnd_experiment__pub', 'nd_experiment_id', $this->nd_experiment_id, 'pub_id', $pub_id);
        }
      }
    }
  }
}