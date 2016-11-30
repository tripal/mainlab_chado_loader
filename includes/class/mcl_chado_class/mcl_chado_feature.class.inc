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
   */
  public static function checkFeature(MCL_TEMPLATE $mcl_tmpl, $feature, $genus, $species, $type_id, $separator = '') {

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($mcl_organism) {
        $organism_id  = $mcl_organism->getOrganismID();
      }
      else {
        $mcl_tmpl->updateMsg('E', "$genus $species not found in organism");
        return;
      }
    }

    // Gets the uniquenames.
    $uniquenames = preg_split($mcl_tmpl->getSepRegex($separator), $feature, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($uniquenames as $uniquename) {
      $uniquename   = trim($uniquename);

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
          $mcl_tmpl->updateMsg('E', "$genus $species not found in organism");
          return;
        }
      }

      // Checks the feature.
      $mcl_feature = MCL_CHADO_FEATURE::byKey($args);
      if (!$mcl_feature) {
        $mcl_tmpl->updateMsg('E', $mcl_tmpl->arrStr($args) . " not found in feature");
      }
    }
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
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
  public static function addFeature(MCL_TEMPLATE $mcl_tmpl, $uniquename, $name, $type_id, $organism_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $feature = MCL_CHADO_FEATURE::byKey($args);
    if ($feature) {
      $mcl_tmpl->addMsg('D', 'feature', $args);
    }
    else {

      // Adds a new feature.
      $args['name'] = $name;
      $feature = new MCL_CHADO_FEATURE($args);
      if ($feature->insert()) {
        $mcl_tmpl->addMsg('N', 'feature', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'feature', $args);
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
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $value, $separator = '') {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    $mcl_tmpl->addProperty('featureprop', 'feature_id', $this->feature_id, $type_id, $value, $separator);
  }

  /**
   * Adds related feature to feature_relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_FEATURE $mcl_feature
   * @param integer $type_id
   */
  public function addRelatedFeature(MCL_TEMPLATE $mcl_tmpl, $mcl_feature, $type_id) {
    if ($mcl_feature) {
      $mcl_tmpl->addRelationship('feature_relationship', 'subject_id', $this->feature_id, 'object_id', $mcl_feature->getFeatureID(), $type_id);
    }
  }

  /**
   * Adds related features to feature_relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $marker
   * @param integer $feature_type_id
   * @param integer $relationship_type_id
   * @param boollean $use_organism_id
   * @param string $separator
   */
  public function addRelatedFeatures(MCL_TEMPLATE $mcl_tmpl, $features, $feature_type_id, $use_organism_id, $relationship_type_id, $separator = '') {
    if ($features) {
      $uniquenames = preg_split($mcl_tmpl->getSepRegex($separator), $features, NULL, PREG_SPLIT_NO_EMPTY);
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
          $mcl_tmpl->addRelationship('feature_relationship', 'subject_id', $this->feature_id, 'object_id', $mcl_feature->getFeatureID(), $relationship_type_id);
        }
      }
    }
  }

  /**
   * Adds a sequnce to this feature.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param array $line
   * @param string $prefix
   * @param integer $type_id_feature
   * @param integer $type_id_relationship
   *
   */
  public function addSequence(MCL_TEMPLATE $mcl_tmpl, $line, $prefix, $type_id_feature, $type_id_relationship) {

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
          $mcl_tmpl->updateMsg('W', "$col_name columns is missing. So it will be skipped");
        }
        else if (array_key_exists('name', $info) && $info['name']) {
          $mcl_tmpl->updateMsg('W', "$col_seq columns is missing. So it will be skipped");
        }
      }
      else {

        // If both matching columns are empty, just skip without warning.
        if (empty($info['seq']) && empty($info['name']))  {
          continue;
        }

        // If one of matching columns is empty, adds warning.
        else if (empty($info['seq']) || empty($info['name'])) {
          $mcl_tmpl->updateMsg('W', "Either $col_name or $col_seq are missing. So it will be skipped");
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
        $mcl_tmpl->addMsg('D', 'feature', $args);
      }
      else {

        // Adds a sequence.
        $args['residues'] = $seq;
        $mcl_feature = new MCL_CHADO_FEATURE($args);
        if ($mcl_feature->insert()) {
          $mcl_tmpl->addMsg('N', 'feature', $args);

          // Adds a relationship.
          $this->addRelatedFeature($mcl_tmpl, $mcl_feature, $type_id_relationship);
        }
        else {
          $mcl_tmpl->addMsg('E', 'feature', $args);
        }
      }
    }
  }

  /**
   * Adds accessions to dbxref and feature_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $db_id
   * @param string $dbxref
   * @param string $separator
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl, $db_id, $dbxref, $separator = '') {
    if ($dbxref) {
      $accessions = preg_split($mcl_tmpl->getSepRegex($separator), $dbxref, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($accessions as $accession) {
        $accession = trim($accession);
        $mcl_dbxref = MCL_CHADO_DBXREF::addDBXref($mcl_tmpl, $db_id, $accession);
        if ($mcl_dbxref) {
          $mcl_tmpl->addLink('feature_dbxref', 'feature_id', $this->getFeatureID(), 'dbxref_id', $mcl_dbxref->getDbxrefID());
        }
      }
    }
  }

  /**
   * Adds trait to feature_cvterm.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_TRAIT $trait
   */
  public function addTrait(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_TRAIT $trait) {

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
        $mcl_tmpl->addMsg('D', 'feature_cvterm', $args);
      }
      else {

        // Adds a new relationship.
        $chado_feature_cvterm = new CHADO_FEATURE_CVTERM($args);
        if ($chado_feature_cvterm->insert()) {
          $mcl_tmpl->addMsg('N', 'feature_cvterm', $args);
        }
        else {
          $mcl_tmpl->addMsg('E', 'feature_cvterm', $args);
        }
      }
    }
  }

  /**
   * Adds alias to synoym and feature_synoym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $alias
   * @param integer $type_id
   * @param string $separator
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl, $type_id, $alias, $separator = '') {

    // Sets the arguments.
    $args = array(
      'type_id' => $type_id,
      'alias'   => $alias,
    );

    // Checks arguements.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Sets the default values.
    $synonym_sgml = '';

    // Adds a synonym.
    $names = preg_split($mcl_tmpl->getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($names as $name) {
      $name = trim($name);

      // Checks for duplication.
      $args = array(
        'type_id' => $type_id,
        'name'    => $name,
      );
      $chado_synonym = CHADO_SYNONYM::byKey($args);
      if ($chado_synonym) {
        $mcl_tmpl->addMsg('D', 'synonym', $args);
      }
      else {

        // Adds a new synonym.
        $args['synonym_sgml'] = $synonym_sgml;
        $chado_synonym = new CHADO_SYNONYM($args);
        if ($chado_synonym->insert()) {
          $mcl_tmpl->addMsg('N', 'synonym', $args);

          // Checks for duplication.
          $args = array(
            'pub_id'      => MCL_VAR::getValueByName('PUB_ID'),
            'feature_id'  => $this->feature_id,
            'synonym_id'  => $chado_synonym->getSynonymID(),
          );
          $chado_feature_synonym = CHADO_FEATURE_SYNONYM::byKey($args);
          if ($chado_feature_synonym) {
            $mcl_tmpl->addMsg('D', 'feature_synonym', $args);
          }
          else {

            // Adds a new relationship.
            $chado_feature_synonym = new CHADO_FEATURE_SYNONYM($args);
            if ($chado_feature_synonym->insert()) {
              $mcl_tmpl->addMsg('N', 'feature_synonym', $args);
            }
            else {
              $mcl_tmpl->addMsg('E', 'feature_synonym', $args);
            }
          }
        }
        else {
          $mcl_tmpl->addMsg('E', 'synonym', $args);
        }
      }
    }
  }

  /**
   * Adds synoym and feature_synoym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_SYNONYM $mcl_synonym
   */
  public function addSynonym(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_SYNONYM $mcl_synonym) {
    if ($mcl_synonym) {

      // Checks for duplication.
      $args = array(
        'synonym_id'  => $mcl_synonym->getSynonymID(),
        'feature_id'  => $this->feature_id,
        'pub_id'      => MCL_VAR::getValueByName('PUB_ID'),
      );
      $chado_feature_synonym = CHADO_FEATURE_SYNONYM::byKey($args);
      if ($chado_feature_synonym) {
        $mcl_tmpl->addMsg('D', 'feature_synonym', $args);
      }
      else {

        // Adds a new relationship.
        $chado_feature_synonym = new CHADO_FEATURE_SYNONYM($args);
        if ($chado_feature_synonym->insert()) {
          $mcl_tmpl->addMsg('N', 'feature_synonym', $args);
        }
        else {
          $mcl_tmpl->addMsg('E', 'feature_synonym', $args);
        }
      }
    }
  }

  /**
   * Adds contact to feature_contact.
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
          $mcl_tmpl->addLink('feature_contact', 'feature_id', $this->feature_id, 'contact_id', $mcl_contact->getContactID());
        }
      }
    }
  }

  /**
   * Adds dataset to feature_project.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $dataset_name
   * @param string $separator
   */
  public function addDataset(MCL_TEMPLATE $mcl_tmpl, $dataset_name, $separator = '') {
    if ($dataset_name) {
      $names = preg_split($mcl_tmpl->getSepRegex($separator), $dataset_name, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_dataset = MCL_CHADO_DATASET::getDataset($name);
        if ($mcl_dataset) {
          $mcl_tmpl->addLink('feature_project', 'feature_id', $this->feature_id, 'project_id', $mcl_dataset->getProjectID());
        }
      }
    }
  }

  /**
   * Adds stock to feature_stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param string $genus
   * @param string $species
   * @param string $separator
   */
  public function addStock(MCL_TEMPLATE $mcl_tmpl, $stock, $genus, $species, $type_id, $separator = '') {
    if ($stock) {

      // Gets orgaims_id.
      $mcl_organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($mcl_organism) {

        $uniquenames = preg_split($mcl_tmpl->getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
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
            $mcl_tmpl->addLink('feature_stock', 'feature_id', $this->feature_id, 'stock_id', $mcl_stock->getStockID(), $type_id);
          }
        }
      }
      else {
        $this->updateMsg('E', "(genus, species) = ($genus, $species) not found in organism");
      }
    }
  }

  /**
   * Adds genotype to feature_genotype.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param integer $cvterm_id
   * @param MCL_CHADO_GENOTYPE $genotype
   */
  public function addGenotype(MCL_TEMPLATE $mcl_tmpl, $stock, $genus, $cvterm_id, MCL_CHADO_GENOTYPE $genotype) {

    // Checks feature_genotype for duplication.
    $args = array(
      'feature_id'  => $this->feature_id,
      'genotype_id' => $genotype->getGenotypeID(),
      'cvterm_id'   => $cvterm_id,
    );
    $chado_feature_genotype = CHADO_FEATURE_GENOTYPE::byKey($args);
    if ($chado_feature_genotype) {
      $mcl_tmpl->addMsg('D', 'feature_genotype', $args);
    }
    else {

      // Adds feature_genotype.
      $args['cgroup'] = 1;
      $chado_feature_genotype = new CHADO_FEATURE_GENOTYPE($args);
      if ($chado_feature_genotype->insert()) {
        $mcl_tmpl->addMsg('N', 'feature_genotype', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'feature_genotype', $args);
      }
    }
  }

  /**
   * Adds image to feature_image.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   */
  public function addImage(MCL_TEMPLATE $mcl_tmpl, $image, $separator = '') {
    if ($image) {
      $eimage_ids = preg_split($mcl_tmpl->getSepRegex($separator), $image, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $mcl_image = MCL_CHADO_IMAGE::byKey(array('eimage_id' => $eimage_id));
        if ($mcl_image) {
          $mcl_tmpl->addLink('feature_image', 'feature_id', $this->feature_id, 'eimage_id', $mcl_image->getEimageID());
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
          $mcl_tmpl->addLink('feature_pub', 'feature_id', $this->feature_id, 'pub_id', $pub_id);
        }
      }
    }
  }
}