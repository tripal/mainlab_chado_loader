<?php
/**
 * The declaration of MCL_CHADO_FEATURE class.
 *
 */
class MCL_CHADO_FEATURE extends CHADO_FEATURE {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_FEATURE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_FEATURE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_FEATURE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of feature. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $feature
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $feature, $genus, $species, $type_id, $separator = '') {
    $flag = TRUE;

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($mcl_organism) {
        $organism_id  = $mcl_organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
        return FALSE;
      }
    }

    // Gets the uniquenames.
    $uniquenames = preg_split(self::getSepRegex($separator), $feature, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($uniquenames as $uniquename) {
      $uniquename = trim($uniquename);

      // Sets the arguments.
      $args = array(
        'uniquename'  => $uniquename,
        'type_id'     => $type_id,
      );
      if ($organism_id) {
        $args['organism_id'] = $organism_id;
      }

      // If '::' found in uniquename, update uniquename and organism_id.
      if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $uniquename, $matches)) {
        $genus          = trim($matches[1]);
        $species        = trim($matches[2]);
        $uniquename     = trim($matches[3]);
        $diff_organism  = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if ($diff_organism) {
          $args['organism_id'] = $diff_organism->getOrganismID();
        }
        else {
          self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
          $flag = FALSE;
        }
      }

      // Checks the feature.
      $mcl_feature = MCL_CHADO_FEATURE::byKey($args);
      if (!$mcl_feature) {
        self::updateMsg($mcl_tmpl, 'E', self::arrStr($args) . " not found in feature");
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function getFeature($uniquename, $genus, $species, $type_id) {

    // Gets the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($mcl_organism) {
        $organism_id = $mcl_organism->getOrganismID();
      }
      else {
       return NULL;
      }
    }

    // Gets the feature.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
    }
    return MCL_CHADO_FEATURE::bykey($args);
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   * Search uniquename for alias if it does not exist in feature table.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function getFeatureAlias($uniquename, $genus, $species, $type_id) {

    // Gets the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($mcl_organism) {
        $organism_id = $mcl_organism->getOrganismID();
      }
      else {
        return NULL;
      }
    }

    // Gets the feature.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
    }
    $feature = MCL_CHADO_FEATURE::bykey($args);
    if ($feature) {
      return $feature;
    }

    // Search for alias.
    $sql = "
      SELECT COUNT(S.synonym_id)
      FROM {chado.synonym} S
        INNER JOIN {chado.feature_synonym} FS on FS.synonym_id = S.synonym_id
      WHERE LOWER(S.name) = LOWER(:name)
    ";
    $num = db_query($sql, array(':name' => $uniquename))->fetchField();
    if ($num == 1) {
      $sql = "
        SELECT FS.feature_id
        FROM {chado.synonym} S
          INNER JOIN {chado.feature_synonym} FS on FS.synonym_id = S.synonym_id
        WHERE LOWER(S.name) = LOWER(:name)
      ";
      $feature_id = db_query($sql, array(':name' => $uniquename))->fetchField();
      return MCL_CHADO_FEATURE::bykey(array('feature_id' => $feature_id));
    }
    return NULL;
  }

  /**
   * Returns the number of features with the same uniquename and type_id but
   * different organism_id.
   *
   * @param string $uniquename
   * @param integer $type_id
   *
   * @return integer
   */
  public static function countFeatures($uniquename, $type_id) {

    // Gets the features.
    $sql = "
      SELECT COUNT(F.feature_id)
      FROM chado.feature F
      WHERE LOWER(F.uniquename) = LOWER(:uniquename)
        AND F.type_id = :type_id
    ";
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );
    $result = db_query($sql, $args);
    return $result->fetchField();
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getFeatureprop($cv_name, $cvterm_name) {

    // Gets cvterm.
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($cvterm) {
      $details = array(
        'feature_id' => $this->feature_id,
        'type_id'    => $cvterm->getCvtermID(),
      );
      $featureprop = CHADO_FEATUREPROP::byKey($details);
      if ($featureprop) {
        return $featureprop->getValue();
      }
    }
    return '';
  }

  /**
   * Adds a feature.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   * @param integer $organism_id
   * @param string $name
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function addFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $name, $type_id, $organism_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $feature = MCL_CHADO_FEATURE::byKey($args);
    if ($feature) {
      self::addMsg($mcl_tmpl, 'D', 'feature', $args);
    }
    else {

      // Adds a new feature.
      $args['name'] = $name;
      $feature = new MCL_CHADO_FEATURE($args);
      if ($feature->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'feature', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'feature', $args);
        return NULL;
      }
    }
    return $feature;
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
      return $this->addProperty($mcl_tmpl, 'featureprop', 'feature_id', $this->feature_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds related feature to feature_relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_FEATURE $mcl_feature
   * @param integer $type_id
   *
   * @return boolean
   */
  public function addRelatedFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $mcl_feature, $type_id, $swap = FALSE) {
    if ($mcl_feature) {
      if ($swap) {
        return $this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $mcl_feature->getFeatureID(), 'object_id', $this->feature_id, $type_id);
      }
      else {
        return $this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $this->feature_id, 'object_id', $mcl_feature->getFeatureID(), $type_id);
      }
    }
    return TRUE;
  }

  /**
   * Adds related features to feature_relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $features
   * @param integer $feature_type_id
   * @param integer $relationship_type_id
   * @param boollean $use_organism_id
   * @param string $separator
   *
   * @return boolean
   */
  public function addRelatedFeatures(MCL_TEMPLATE $mcl_tmpl = NULL, $features, $feature_type_id, $use_organism_id, $relationship_type_id, $separator = '') {
    $flag = TRUE;
    if ($features) {
      $uniquenames = preg_split($this->getSepRegex($separator), $features, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($uniquenames as $uniquename) {
        $uniquename = trim($uniquename);

        // Sets the arguments.
        $args = array(
          'uniquename'  => $uniquename,
          'type_id'     => $feature_type_id,
        );
        if ($use_organism_id) {
          $organism_id = $this->organism_id;

           // If '::' found in uniquename, update uniquename and organism_id.
          if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $uniquename, $matches)) {
            $genus          = trim($matches[1]);
            $species        = trim($matches[2]);
            $uniquename     = trim($matches[3]);
            $diff_organism  = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
            $organism_id    = $diff_organism->getOrganismID();
          }
          $args['organism_id'] = $organism_id;
        }

        // Adds a feature.
        $mcl_feature = MCL_CHADO_FEATURE::byKey($args);
        if ($mcl_feature) {
          if (!$this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $this->feature_id, 'object_id', $mcl_feature->getFeatureID(), $relationship_type_id)) {
            $flag = FALSE;
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a sequnce to this feature.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   * @param string $prefix
   * @param integer $type_id_feature
   * @param integer $type_id_relationship
   * @param boolean $swap
   *   Swaps subject_id and object_id.
   *
   * @return boolean
   */
  public function addSequence(MCL_TEMPLATE $mcl_tmpl = NULL, $line, $prefix, $type_id_feature, $type_id_relationship, $swap = FALSE) {
    $flag = TRUE;

    // Builds the pattern.
    $pattern = "^$prefix(\d+)_(seq|name)";

    // Gets name and seq columns.
    $sequences = array();
    foreach ($line as $column => $value) {
      if (preg_match("/$pattern/", $column, $matches)) {
        $num  = $matches[1];
        $type = $matches[2];
        $name = $prefix . $num . '_' . $type;
        $sequences[$num][$type] = $line[$name];
      }
    }

    // Skips the unmatched sequences.
    $matched_seqs = array();
    foreach ($sequences as $num => $info) {
      $name = $prefix . $num;
      $col_name = $name . '_name';
      $col_seq = $name . '_seq';

      // Checks the matching columns.
      if (!array_key_exists('seq', $info) || !array_key_exists('name', $info)) {
        if (array_key_exists('seq', $info) && $info['seq']) {
          $this->updateMsg($mcl_tmpl, 'W', "$col_name columns is missing. So it will be skipped");
        }
        else if (array_key_exists('name', $info) && $info['name']) {
          $this->updateMsg($mcl_tmpl, 'W', "$col_seq columns is missing. So it will be skipped");
        }
      }
      else {

        // If both matching columns are empty, just skip without warning.
        if (empty($info['seq']) && empty($info['name']))  {
          continue;
        }

        // If one of matching columns is empty, adds warning.
        else if (empty($info['seq']) || empty($info['name'])) {
          $this->updateMsg($mcl_tmpl, 'W', "Either $col_name or $col_seq are missing. So it will be skipped");
        }

        // Adds the matching columns.
        else {
          $matched_seqs[$info['name']] = $info['seq'];
        }
      }
    }

    // Adds sequences.
    foreach ($matched_seqs as $seq_name => $seq) {
      $args = array(
        'uniquename'  => $this->getUniquename() . '.' . $seq_name,
        'name'        => $seq_name,
        'type_id'     => $type_id_feature,
        'organism_id' => $this->organism_id,
      );
      $mcl_feature = MCL_CHADO_FEATURE::byKey($args);
      if ($mcl_feature) {
        $this->addMsg($mcl_tmpl, 'D', 'feature', $args);
      }
      else {

        // Adds a sequence.
        $args['residues'] = $seq;
        $mcl_feature = new MCL_CHADO_FEATURE($args);
        if ($mcl_feature->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature', $args);

          // Adds a relationship.
          $this->addRelatedFeature($mcl_tmpl, $mcl_feature, $type_id_relationship, $swap);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'feature', $args);
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds accessions to dbxref and feature_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $db_id
   * @param string $dbxref
   * @param string $separator
   *
   * @return boolean
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, $db_id, $dbxref, $separator = '') {
    $flag = TRUE;
    if ($dbxref) {
      $accessions = preg_split($this->getSepRegex($separator), $dbxref, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($accessions as $accession) {
        $accession = trim($accession);
        $mcl_dbxref = MCL_CHADO_DBXREF::addDBXref($mcl_tmpl, $db_id, $accession);
        if ($mcl_dbxref) {
          if (!$this->addLink($mcl_tmpl, 'feature_dbxref', 'feature_id', $this->getFeatureID(), 'dbxref_id', $mcl_dbxref->getDbxrefID())) {
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
   * Adds trait to feature_cvterm.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_TRAIT $trait
   *
   * @return boolean
   */
  public function addTrait(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_TRAIT $trait) {

    // Adds the trait.
    if ($trait) {
      $args = array(
        'pub_id'      => MCL_VAR::getValueByName('PUB_ID'),
        'feature_id'  => $this->feature_id,
        'cvterm_id'   => $trait->getCvtermID(),
      );

      // Checks for duplication.
      $chado_feature_cvterm = CHADO_FEATURE_CVTERM::byKey($args);
      if ($chado_feature_cvterm) {
        $this->addMsg($mcl_tmpl, 'D', 'feature_cvterm', $args);
      }
      else {

        // Adds a new relationship.
        $chado_feature_cvterm = new CHADO_FEATURE_CVTERM($args);
        if ($chado_feature_cvterm->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature_cvterm', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'feature_cvterm', $args);
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Adds alias to synoym and feature_synoym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $alias
   * @param integer $type_id
   * @param string $separator
   *
   * @return boolean
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $type_id, $alias, $separator = '') {
    $flag = TRUE;

    // Sets the arguments.
    $args = array(
      'type_id' => $type_id,
      'alias'   => $alias,
    );

    // Checks arguements.
    if (!$this->checkReqArgs($args)) {
      return NULL;
    }

    // Sets the default values.
    $synonym_sgml = '';

    // Adds a synonym.
    $names = preg_split($this->getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($names as $name) {
      $name = trim($name);

      // Checks for duplication [synonym].
      $args = array(
        'type_id' => $type_id,
        'name'    => $name,
      );
      $chado_synonym = CHADO_SYNONYM::byKey($args);
      if ($chado_synonym) {
        $this->addMsg($mcl_tmpl, 'D', 'synonym', $args);
      }
      else {

        // Adds a new synonym.
        $args['synonym_sgml'] = $synonym_sgml;
        $chado_synonym = new CHADO_SYNONYM($args);
        if ($chado_synonym->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'synonym', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'synonym', $args);
          $flag = FALSE;
          continue;
        }
      }

      // Checks for duplication [feature_synonym].
      $args = array(
        'pub_id'      => MCL_VAR::getValueByName('PUB_ID'),
        'feature_id'  => $this->feature_id,
        'synonym_id'  => $chado_synonym->getSynonymID(),
      );
      $chado_feature_synonym = CHADO_FEATURE_SYNONYM::byKey($args);
      if ($chado_feature_synonym) {
        $this->addMsg($mcl_tmpl, 'D', 'feature_synonym', $args);
      }
      else {

        // Adds a new relationship.
        $chado_feature_synonym = new CHADO_FEATURE_SYNONYM($args);
        if ($chado_feature_synonym->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature_synonym', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'feature_synonym', $args);
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds synoym and feature_synoym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_SYNONYM $mcl_synonym
   *
   * @return boolean
   */
  public function addSynonym(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_SYNONYM $mcl_synonym) {
    if ($mcl_synonym) {

      // Checks for duplication.
      $args = array(
        'synonym_id'  => $mcl_synonym->getSynonymID(),
        'feature_id'  => $this->feature_id,
        'pub_id'      => MCL_VAR::getValueByName('PUB_ID'),
      );
      $chado_feature_synonym = CHADO_FEATURE_SYNONYM::byKey($args);
      if ($chado_feature_synonym) {
        $this->addMsg($mcl_tmpl, 'D', 'feature_synonym', $args);
      }
      else {

        // Adds a new relationship.
        $chado_feature_synonym = new CHADO_FEATURE_SYNONYM($args);
        if ($chado_feature_synonym->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature_synonym', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'feature_synonym', $args);
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Adds contact to feature_contact.
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
        $mcl_contact = MCL_CHADO_CONTACT::getContact($name);
        if ($mcl_contact) {
          if (!$this->addLink($mcl_tmpl, 'feature_contact', 'feature_id', $this->feature_id, 'contact_id', $mcl_contact->getContactID())) {
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
   * Adds dataset to feature_project.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   * @param string $separator
   *
   * @return boolean
   */
  public function addDataset(MCL_TEMPLATE $mcl_tmpl = NULL, $dataset_name, $separator = '') {
    $flag = TRUE;
    if ($dataset_name) {
      $names = preg_split($this->getSepRegex($separator), $dataset_name, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $dataset = MCL_CHADO_DATASET::getDataset(trim($name));
        if ($dataset) {
          if (!$this->addLink($mcl_tmpl, 'feature_project', 'feature_id', $this->feature_id, 'project_id', $dataset->getProjectID())) {
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
   * Adds site to feature_nd_geolocation.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $site_name
   * @param string $separator
   *
   * @return boolean
   */
  public function addSite(MCL_TEMPLATE $mcl_tmpl = NULL, $site_name, $separator = '') {
    $flag = TRUE;
    if ($dataset_name) {
      $names = preg_split($this->getSepRegex($separator), $site_name, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $nd_geolocation_id =  MCL_CHADO_LOCATION::getLocationIDBySite(trim($name));
        if ($nd_geolocation_id) {
          if (!$this->addLink($mcl_tmpl, 'feature_nd_geolocation', 'feature_id', $this->feature_id, 'nd_geolocation_id', $nd_geolocation_id)) {
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
   * Adds stock to feature_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param string $genus
   * @param string $species
   * @param string $separator
   *
   * @return boolean
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl = NULL, $stock, $genus, $species, $type_id, $separator = '') {
    $flag = TRUE;
    if ($stock) {

      // Gets orgaims_id.
      $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($mcl_organism) {

        $uniquenames = preg_split($this->getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
        foreach ($uniquenames as $uniquename) {
          $uniquename = trim($uniquename);
          $organism_id = $mcl_organism->getOrganismID();

          // If '::' found in uniquename, update uniquename and organism_id.
          if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $uniquename, $matches)) {
            $genus          = trim($matches[1]);
            $species        = trim($matches[2]);
            $uniquename     = trim($matches[3]);
            $diff_organism  = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
            $organism_id    = $diff_organism->getOrganismID();
          }

          // Adds a stock.
          $mcl_stock = MCL_CHADO_STOCK::byKey(array('uniquename' => $uniquename, 'organism_id' => $organism_id));
          if ($mcl_stock) {
            if (!$this->addLink($mcl_tmpl, 'feature_stock', 'feature_id', $this->feature_id, 'stock_id', $mcl_stock->getStockID(), $type_id)) {
              $flag = FALSE;
            }
          }
          else {
            $flag = FALSE;
          }
        }
      }
      else {
        $this->updateMsg('E', "(genus, species) = ($genus, $species) not found in organism");
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Adds genotype to feature_genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param integer $cvterm_id
   * @param MCL_CHADO_GENOTYPE $genotype
   *
   * @return boolean
   */
  public function addGenotype(MCL_TEMPLATE $mcl_tmpl = NULL, $stock, $genus, $cvterm_id, MCL_CHADO_GENOTYPE $genotype) {

    // Checks feature_genotype for duplication.
    $args = array(
      'feature_id'  => $this->feature_id,
      'genotype_id' => $genotype->getGenotypeID(),
      'cvterm_id'   => $cvterm_id,
    );
    $chado_feature_genotype = CHADO_FEATURE_GENOTYPE::byKey($args);
    if ($chado_feature_genotype) {
      $this->addMsg($mcl_tmpl, 'D', 'feature_genotype', $args);
    }
    else {

      // Adds feature_genotype.
      $args['cgroup'] = 1;
      $chado_feature_genotype = new CHADO_FEATURE_GENOTYPE($args);
      if ($chado_feature_genotype->insert()) {
        $this->addMsg($mcl_tmpl, 'N', 'feature_genotype', $args);
      }
      else {
        $this->addMsg($mcl_tmpl, 'E', 'feature_genotype', $args);
        return FALSE;;
      }
    }
    return TRUE;
  }

  /**
   * Adds image to feature_image.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public function addImage(MCL_TEMPLATE $mcl_tmpl = NULL, $image, $separator = '') {
    $flag = TRUE;
    if ($image) {
      $eimage_ids = preg_split($this->getSepRegex($separator), $image, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $mcl_image = MCL_CHADO_IMAGE::byKey(array('eimage_id' => $eimage_id));
        if ($mcl_image) {
          if (!$this->addLink($mcl_tmpl, 'feature_image', 'feature_id', $this->feature_id, 'eimage_id', $mcl_image->getEimageID())) {
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
        $mcl_pub = MCL_CHADO_PUB::getPub($pub_id);
        if ($mcl_pub) {
          if (!$this->addLink($mcl_tmpl, 'feature_pub', 'feature_id', $this->feature_id, 'pub_id', $pub_id)) {
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