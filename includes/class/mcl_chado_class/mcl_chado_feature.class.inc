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
   * Checks the existence of feature. If not, write the error messasge
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
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
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
      $num = MCL_CHADO_FEATURE::countFeature($mcl_tmpl, $uniquename, $genus, $species, $type_id);
      if ($num > 1) {
        self::updateMsg($mcl_tmpl, 'E', "The uniquename '$uniquename' is ambiguous. Please not use this uniquename");
        $flag = FALSE;
      }
      else if ($num < 1) {
        self::updateMsg($mcl_tmpl, 'E', "The uniquename '$uniquename' does not found in chado.feature");
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Count the number of the feature with the same uniquename, type_id and/or
   * organism_id
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   */
  public static function countFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $genus, $species, $type_id) {

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
        return 0;
      }
    }

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );

    // Searches for alias.
    $sql = "
      SELECT COUNT(F.feature_id)
      FROM {chado.feature} F
      WHERE LOWER(F.uniquename) = LOWER(:uniquename) AND F.type_id = :type_id
    ";
    if ($organism_id) {
      $args['organism_id'] = $organism_id;
      $sql .= " AND F.organism_id = :organism_id";
    }
    return db_query($sql, $args)->fetchField();
  }

  /**
   * Checks the feature uniquename for uniqueness for the provided type. Search for the
   * same uniquename with different organism. Log the error if the uniquename
   * is used with different organism.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   * @param integer $type_id
   *
   * @return boolean
   */
  public static function checkFeatureUniquenameForUniqueness(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $genus, $species, $type_id) {

    // Gets the organism.
    $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
    if ($organism) {

      // Gets all the features with the same uniquename.
      $sql = "
        SELECT F.* FROM {chado.feature} F
        WHERE LOWER(F.uniquename) = LOWER(:uniquename) AND F.type_id = :type_id
      ";
      $args =  array(
        ':uniquename' => $uniquename,
        ':type_id'    => $type_id,
      );
      $results = db_query($sql,$args);
      $found = FALSE;
      $count = 0;
      while ($feature = $results->fetchObject()) {
        $count++;

        // Checks for organism_id.
        if ($feature->organism_id == $organism->getOrganismID()) {
          $found = TRUE;
        }
      }
      if ($count && !$found) {
        self::updateMsg($mcl_tmpl, 'E', "$uniquename is used with different organism. Please change the name.");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks the existence of feature. If not, write the error messasge
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
  public static function checkFeatureAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $feature, $genus, $species, $type_id, $separator = '') {
    $flag = TRUE;

    // Checks the organism_id.
    $organism_id = '';
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id  = $organism->getOrganismID();
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in chado.organism");
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
      $feature = MCL_CHADO_FEATURE::byKey($args);
      if (!$feature) {

        // Searches for alias.
        $sql = "
          SELECT COUNT(S.synonym_id)
          FROM {chado.synonym} S
            INNER JOIN {chado.feature_synonym} FS on FS.synonym_id = S.synonym_id
          WHERE LOWER(S.name) = LOWER(:name)
        ";
        $num = db_query($sql, array(':name' => $uniquename))->fetchField();
        if ($num > 1) {
          self::updateMsg($mcl_tmpl, 'E', "The alias '$uniquename' is ambiguous. Please not use this alias");
          $flag = FALSE;
        }
        else if ($num < 1) {
          self::updateMsg($mcl_tmpl, 'E', self::arrStr($args) . " not found in feature");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the residues. If the residues has allele (e.g. [A|T]), check if
   * it meets the following conditions.
   *   1. it has only one allele.
   *   2. it has only one pair of brackets ([]).
   *   3. The allele has a valid IUPAC code.
   *
   * @param MCL_TEMPLATE MCL_TEMPLATE
   * @param string $residues
   *
   * @return boolean
   */
  public static function checkResidues(MCL_TEMPLATE $mcl_tmpl = NULL, $residues) {

    // Checks if the residues has allele.
    if (preg_match("/\[/", $residues)) {
      $count = preg_match_all('/\[/', $residues, $matches);
      if ($count == 1) {
        $count = preg_match_all("/\]/", $residues, $matches);
        if ($count == 1) {

          // Checks if the allele has the corresponding IUPAC code.
          if (preg_match("/\[(.+)\]/", $residues, $matches)) {
            $allele = $matches[1];
            if (self::getIUPAC($allele)) {
              return TRUE;
            }
            else {
              self::updateMsg($mcl_tmpl, 'E', "The allele [$allele] does not have valid IUPAC code");
              return FALSE;
            }
          }
        }
      }
      self::updateMsg($mcl_tmpl, 'E', "The allele has invalid format");
      return FALSE;
    }
    return TRUE;
  }

  /**
   * Parses the residues. If the residues has allele (e.g. [A|T]), it does
   * the followings.
   *   1. Replace it with IUPAC code.
   *   2. Gets five prime flanking sequence
   *   3. Gets three prime flanking sequence
   *   4. Gets allele
   *
   * @param string $residues
   *
   * @return array
   */
  public static function parseResidues($residues) {
    $seq_info = array();

    // Checks if the residues has allele.
    if (preg_match('/^(.*)\[(.*)\](.*)$/', $residues, $matches)) {

      // Parses out the sequence information.
      $five_prime   = trim($matches[1]);
      $allele       = trim($matches[2]);
      $three_prime  = trim($matches[3]);
      $iupac_code   = self::getIUPAC($allele);

      // Sets the info.
      $seq_info['allele']       = str_replace(' ', '', $allele);
      $seq_info['iupac_code']   = $iupac_code;
      $seq_info['five_prime']   = $five_prime;
      $seq_info['three_prime']  = $three_prime;
      $seq_info['residues']     = $five_prime . $iupac_code . $three_prime;
    }
    else {
      $seq_info['residues'] = $residues;
    }
    return $seq_info;
  }

  /**
   * Returns IUPAC code of the given string.
   *
   * @param string $nucleotides
   *
   * IUPAC codes
   *
   * Nucleotide Code:  Base:
   * ==================
   * A.................Adenine
   * C.................Cytosine
   * G.................Guanine
   * T (or U)..........Thymine (or Uracil)
   * R.................A or G
   * Y.................C or T
   * S.................G or C
   * W.................A or T
   * K.................G or T
   * M.................A or C
   * B.................C or G or T
   * D.................A or G or T
   * H.................A or C or T
   * V.................A or C or G
   * N.................any base
   * . or -............gap
   *
   * @return string
   */
  public static function getIUPAC($nucleotides) {
    $iupac_code = '';
    if ($nucleotides) {

      // Cleans up and splits the nucleotides to bases.
      $sep = array(' ', '/', '\\', '|');
      $nucleotides = str_replace($sep, '', $nucleotides);

      // Sorts them and finds matched IUPAC code.
      $temp = str_split($nucleotides);
      asort($temp);
      $code = implode('', $temp);
      if ($code == 'AG') {
        $iupac_code = 'R';
      }
      else if ($code == 'CT') {
        $iupac_code = 'Y';
      }
      else if ($code == 'CG') {
        $iupac_code = 'S';
      }
      else if ($code == 'AT') {
        $iupac_code = 'W';
      }
      else if ($code == 'GT') {
        $iupac_code = 'K';
      }
      else if ($code == 'AC') {
        $iupac_code = 'M';
      }
      else if ($code == 'CGT') {
        $iupac_code = 'B';
      }
      else if ($code == 'AGT') {
        $iupac_code = 'D';
      }
      else if ($code == 'ACT') {
        $iupac_code = 'H';
      }
      else if ($code == 'ACG') {
        $iupac_code = 'V';
      }
    }
    return $iupac_code;
  }

  /**
   * Returns IUPAC code of the given string.
   *
   * @param string $nucleotides
   *
   * IUPAC codes
   *
   * Nucleotide Code:  Base:
   * ==================
   * A.................Adenine
   * C.................Cytosine
   * G.................Guanine
   * T (or U)..........Thymine (or Uracil)
   * R.................A or G
   * Y.................C or T
   * S.................G or C
   * W.................A or T
   * K.................G or T
   * M.................A or C
   * B.................C or G or T
   * D.................A or G or T
   * H.................A or C or T
   * V.................A or C or G
   * N.................any base
   * . or -............gap
   *
   * @return string
   */
  public static function getBases($iupac_code, $separator = '/') {
    $bases = '';
    if ($iupac_code) {
      if ($iupac_code == 'R') {
        $bases = 'A' . $separator . 'G';
      }
      else if ($iupac_code == 'Y') {
        $bases = 'C' . $separator . 'T';
      }
      else if ($iupac_code == 'S') {
        $bases = 'C' . $separator . 'G';
      }
      else if ($iupac_code == 'W') {
        $bases = 'A' . $separator . 'T';
      }
      else if ($iupac_code == 'K') {
        $bases = 'G' . $separator . 'T';
      }
      else if ($iupac_code == 'M') {
        $bases = 'A' . $separator . 'G';
      }
      else if ($iupac_code == 'B') {
        $bases = 'C' . $separator . 'G'. $separator . 'T';
      }
      else if ($iupac_code == 'D') {
        $bases = 'A' . $separator . 'G'. $separator . 'T';
      }
      else if ($iupac_code == 'H') {
        $bases = 'A' . $separator . 'C'. $separator . 'T';
      }
      else if ($iupac_code == 'V') {
        $bases = 'A' . $separator . 'C' . $separator . 'G';
      }
    }
    return $bases;
  }

  /**
   * Returns the feature by uniquename, genus, species and type_id.
   *
   * @param string $uniquename
   * @param array $type_ids
   *
   * @return MCL_CHADO_FEATURE
   */
  public static function checkMarkerMtlQtl(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $type_ids) {

    // Checks the uniquename for MTL.
    $mtl = self::getFeature($uniquename, '', '', $type_ids['mtl']);
    if (!$mtl) {

      // Checks the uniquename for QTL.
      $qtl = self::getFeature($uniquename, '', '', $type_ids['qtl']);
      if (!$qtl) {

        // Checks the uniquename for marker.
        $marker = self::checkFeatureAlias($mcl_tmpl, $uniquename, '', '', $type_ids['marker']);
        if (!$marker) {
          $mcl_tmpl->updateMsg('E', "Error - $uniquename is not 'Genetic marker', 'QTL' or 'MTL'");
        }
      }
    }
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
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id = $organism->getOrganismID();
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
    return MCL_CHADO_FEATURE::byKey($args);
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
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {
        $organism_id = $organism->getOrganismID();
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
    $feature = MCL_CHADO_FEATURE::byKey($args);
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
      return MCL_CHADO_FEATURE::byKey(array('feature_id' => $feature_id));
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
      return $this->addProperty($mcl_tmpl, 'featureprop', 'feature_id', $this->feature_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'featureprop', 'feature_id', $this->feature_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds related feature to feature_relationship.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_FEATURE $feature
   * @param integer $type_id
   * @param boolean $swap
   *
   * @return boolean
   */
  public function addRelatedFeature(MCL_TEMPLATE $mcl_tmpl = NULL, $feature, $type_id, $swap = FALSE) {
    if ($feature) {
      if ($swap) {
        return $this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $feature->getFeatureID(), 'object_id', $this->feature_id, $type_id);
      }
      else {
        return $this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $this->feature_id, 'object_id', $feature->getFeatureID(), $type_id);
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
   * @param boolean $swap
   *
   * @return boolean
   */
  public function addRelatedFeatures(MCL_TEMPLATE $mcl_tmpl = NULL, $features, $feature_type_id, $use_organism_id, $relationship_type_id, $separator = '', $swap = FALSE) {
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
        $feature = MCL_CHADO_FEATURE::byKey($args);
        if ($feature) {
          if ($swap) {
            if (!$this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $feature->getFeatureID(), 'object_id', $this->feature_id, $relationship_type_id)) {
              $flag = FALSE;
            }
          }
          else {
            if (!$this->addRelationship($mcl_tmpl, 'feature_relationship', 'subject_id', $this->feature_id, 'object_id', $feature->getFeatureID(), $relationship_type_id)) {
              $flag = FALSE;
            }
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
      $feature = MCL_CHADO_FEATURE::byKey($args);
      if ($feature) {
        $this->addMsg($mcl_tmpl, 'D', 'feature', $args);
      }
      else {

        // Adds a sequence.
        $args['residues'] = $seq;
        $feature = new MCL_CHADO_FEATURE($args);
        if ($feature->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'feature', $args);

          // Adds a relationship.
          $this->addRelatedFeature($mcl_tmpl, $feature, $type_id_relationship, $swap);
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
        $dbxref = MCL_CHADO_DBXREF::addDBXref($mcl_tmpl, $db_id, $accession);
        if ($dbxref) {
          if (!$this->addLink($mcl_tmpl, 'feature_dbxref', 'feature_id', $this->getFeatureID(), 'dbxref_id', $dbxref->getDbxrefID())) {
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
   * @param MCL_CHADO_SYNONYM $synonym
   *
   * @return boolean
   */
  public function addSynonym(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_SYNONYM $synonym) {
    if ($synonym) {

      // Checks for duplication.
      $args = array(
        'synonym_id'  => $synonym->getSynonymID(),
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
        $contact = MCL_CHADO_CONTACT::byName($name);
        if ($contact) {
          if (!$this->addLink($mcl_tmpl, 'feature_contact', 'feature_id', $this->feature_id, 'contact_id', $contact->getContactID())) {
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
        $dataset = MCL_CHADO_DATASET::byName(trim($name));
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
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      if ($organism) {

        $uniquenames = preg_split($this->getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
        foreach ($uniquenames as $uniquename) {
          $uniquename = trim($uniquename);
          $organism_id = $organism->getOrganismID();

          // If '::' found in uniquename, update uniquename and organism_id.
          if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $uniquename, $matches)) {
            $genus          = trim($matches[1]);
            $species        = trim($matches[2]);
            $uniquename     = trim($matches[3]);
            $diff_organism  = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
            $organism_id    = $diff_organism->getOrganismID();
          }

          // Adds a stock.
          $stock = MCL_CHADO_STOCK::byKey(array('uniquename' => $uniquename, 'organism_id' => $organism_id));
          if ($mcl_stock) {
            if (!$this->addLink($mcl_tmpl, 'feature_stock', 'feature_id', $this->feature_id, 'stock_id', $stock->getStockID(), $type_id)) {
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
   * @param MCL_CHADO_GENOTYPE $genotype
   * @param integer $cvterm_id
   *
   * @return boolean
   */
  public function addGenotype(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_GENOTYPE $genotype, $cvterm_id) {

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
      $args['rank']   = 0;
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
   * @param string $image_id_list
   * @param string $separator
   *
   * @return boolean
   */
  public function addImageID(MCL_TEMPLATE $mcl_tmpl = NULL, $image_id_list, $separator = '') {
    $flag = TRUE;
    if ($image_id_list) {
      $eimage_ids = preg_split($this->getSepRegex($separator), $image_id_list, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $image = MCL_CHADO_IMAGE::byKey(array('eimage_id' => $eimage_id));
        if ($image) {
          if (!$this->addLink($mcl_tmpl, 'feature_image', 'feature_id', $this->feature_id, 'eimage_id', $image->getEimageID())) {
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