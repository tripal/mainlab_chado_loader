<?php
/**
 * The declaration of MCL_CHADO_CVTERM class.
 *
 */
class MCL_CHADO_CVTERM extends CHADO_CVTERM {

 /**
  *  Class data members.
  */
  /**
   * @see CHADO_CVTERM::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_CVTERM::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_CVTERM::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * @see CHADO_CVTERM::delete()
   */
  public function delete() {

    // Deletes the dbxref first.
    db_delete('chado.dbxref')
      ->condition('dbxref_id', $this->dbxref_id, '=')
      ->execute();
    parent::delete();
  }

  /**
   * Checks the existance of cvterm. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return boolean
   */
  public static function checkCvterm(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name) {
    if ($cv_name && $cvterm_name) {

      // Gets the cvterm.
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      if (!$cvterm) {
        self::updateMsg($mcl_tmpl, 'E', "(cv_name, cvterm_name) = ($cv_name, $cvterm_name) not found in cvterm");
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Checks the format of the values for quantitative_code and code.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $data_type
   * @param string $values
   *
   * @return boolean
   */
  public static function checkValues(MCL_TEMPLATE $mcl_tmpl = NULL, $data_type, $values) {
    $flag = TRUE;

    // Checks only for quantitative_code and code.
    if (preg_match("/^(code|quantitative_code)$/i", $data_type)) {
      if ($values) {

        // Parses out the values to the codes.
        $codes = preg_split("/[;]/", $values, NULL, PREG_SPLIT_NO_EMPTY);
        foreach ($codes as $code) {
          $tmp    = preg_split("/[:=]/", $code, NULL, PREG_SPLIT_NO_EMPTY);
          $rank   = trim($tmp[0]);
          $value  = trim($tmp[1]);

          // Checks a rank.
          if (is_infinite($rank)) {
            self::updateMsg($mcl_tmpl, 'E', "Rank should be an integer ($rank was given)");
            $flag = FALSE;
          }

          // Checks a value.
          if (!$value) {
            self::updateMsg($mcl_tmpl, 'E', "Value must be non-empty");
            $flag = FALSE;
          }
        }
      }
      else {
        self::updateMsg($mcl_tmpl, 'E', "values cannot be empty for $data_type.");
        $flag = FALSE;
      }
    }
    return $flag;
  }


  /**
   * Check categorical values.
   *
   * Examples of categorical values.
   *   White/Yellow/Red
   *   melting/non-melting/slow softening
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $categories
   *
   * @return boolean
   */
  public static function checkCategories(MCL_TEMPLATE $mcl_tmpl = NULL, $categories) {
    if ($categories) {}
    return TRUE;
  }

  /**
   * Returns the cvterm by cv and cvterm names.

   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function getCvterm($cv_name, $cvterm_name) {
    $cv = MCL_CHADO_CV::getCV($cv_name);
    if ($cv) {
      return MCL_CHADO_CVTERM::bykey(array('cv_id' => $cv->getCvID(), 'name' => $cvterm_name));
    }
    return NULL;
  }

  /**
   * Returns the cvterm by cvterm ID.

   * @param integer $cvterm_id
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function getCvtermByID($cvterm_id) {
    return MCL_CHADO_CVTERM::bykey(array('cvterm_id' => $cvterm_id));
  }

  /**
   * Returns the cvterm by cv ID and cvterm name.

   * @param integer $cvterm_id
   * @param string $cvterm_name
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function getCvtermByCvID($cv_id, $cvterm_name) {
    $cv = MCL_CHADO_CV::getCVByID($cv_id);
    if ($cv) {
      return MCL_CHADO_CVTERM::bykey(array('cv_id' => $cv_id, 'name' => $cvterm_name));
    }
    return NULL;
  }

  /**
   * Returns the cvtermprop.

   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getCvtermprop($cv_name, $cvterm_name) {

    // Gets Updates cv.name
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($cvterm) {
      $args = array(
        'cvterm_id' => $this->cvterm_id,
        'type_id'   => $cvterm->getCvtermID(),
      );
      $prop = CHADO_CVTERMPROP::byKey($args);
      if ($prop) {
        return $prop->getValue();
      }
    }
    return '';
  }

  /**
   * Adds null cvterm.
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function addNull() {

    // Gets null.
    $cv     = MCL_CHADO_CV::addNull();
    $dbxref = MCL_CHADO_DBXREF::addNull();

    // Checks if it has already exists.
    $cvterm = MCL_CHADO_CVTERM::getCvterm('null', 'null');
    if (!$cvterm) {
      $details = array(
        'name'      => 'null',
        'cv_id'     => $cv->getCvID(),
        'dbxref_id' => $dbxref->getDbxrefID(),
      );
      $cvterm = new MCL_CHADO_CVTERM($details);
      if (!$cvterm->insert()) {
        return NULL;
      }
    }
    return $cvterm;
  }

  /**
   * Adds a cvterm.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $db_name
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $definition
   * @param string $accession
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function addCvterm(MCL_TEMPLATE $mcl_tmpl = NULL, $db_name, $cv_name, $cvterm_name, $definition, $accession = '') {

    // Updates DB and CV names.
    if ($db_name == '') {
      $db_name = 'SITE_DB';
    }
    if ($cv_name == '') {
      $cv_name = 'SITE_CV';
    }

    // Checks parameters.
    $args = array(
      'db_name'     => $db_name,
      'cv_name'     => $cv_name,
      'cvterm_name' => $cvterm_name,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($cvterm) {
      self::updateMsg($mcl_tmpl, 'D', "(cv.name, cvterm.name) = ($cv_name, $cvterm_name) found in cvterm");
    }
    else {

       // Gets db.
      $db = MCL_CHADO_DB::getDB($db_name);
      if (!$db) {
        self::updateMsg($mcl_tmpl, 'E', "(db.name) = ($db_name) not found in db");
        return NULL;
      }

      // Gets cv.
      $cv = MCL_CHADO_CV::getCV($cv_name);
      if (!$cv) {
        self::updateMsg($mcl_tmpl, 'E', "(cv.name) = ($cv_name) not found in cv");
        return NULL;
      }

      // Sets the accession.
      if (!$accession) {
        $accession = $cv->getCvID() . ":$cvterm_name";
      }

      // Gets dbxref.
      $args = array(
        'db_id'     => $db->getDbID(),
        'accession' => $accession,
      );
      $dbxref = MCL_CHADO_DBXREF::byKey($args);
      if (!$dbxref) {

        // Adds a new dbxref.
        $dbxref = new MCL_CHADO_DBXREF($args);
        if ($dbxref->insert()) {
          self::addMsg($mcl_tmpl, 'N', 'dbxref', $args);
        }
        else {
          self::addMsg($mcl_tmpl, 'E', 'dbxref', $args);
          return NULL;
        }
      }
      // Checks for duplication.
      $dbxref_id = $dbxref->getDbxrefID();
      $cvterm = MCL_CHADO_CVTERM::byKey(array('dbxref_id' => $dbxref_id));
      if ($cvterm) {
        self::updateMsg($mcl_tmpl, 'D', "(dbxref_id) = ($dbxref_id) found in cvterm");
      }
      else {
        $args = array(
          'dbxref_id'   => $dbxref_id,
          'cv_id'       => $cv->getCvID(),
          'name'        => $cvterm_name,
          'definition'  => $definition,
        );
        $cvterm = new MCL_CHADO_CVTERM($args);
        dpm($args);
        if ($cvterm->insert()) {
          self::addMsg($mcl_tmpl, 'N', 'cvterm', $args);
        }
        else {
          self::addMsg($mcl_tmpl, 'E', 'cvterm', $args);
          return NULL;
        }
      }
    }
    return $cvterm;
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
      return $this->addProperty($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value, $separator);
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
      return $this->updateProperty($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value);
    }
    return TRUE;
  }

  /**
   * Adds aliases.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $alias
   * @param string $separator
   *
   * @return boolean
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $alias, $separator = '') {
    $flag = TRUE;
    if ($alias) {

      // Gets cvterm.
      if (!MCL_CHADO_CVTERM::checkCvterm($mcl_tmpl, $cv_name, $cvterm_name)) {
        $this->updateMsg($mcl_tmpl, 'E', "(cv_name, cvterm_name) = ($cv_name, $cvterm_name) not found in cvterm");
        return FALSE;
      }
      $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();

      // Poluplates the array with aliases.
      $alias_arr = array();
      if ($separator) {
        $alias_arr = preg_split($this->getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
      }
      else {
        $alias_arr []= $value;
      }

      // Adds each alias.
      $args = array(
        'cvterm_id' => $this->cvterm_id,
        'type_id'   => $type_id,
      );
      foreach ($alias_arr as $val) {
        $args['synonym']  = trim($val);
        $cvtermsynonym    = CHADO_CVTERMSYNONYM::byKey($args);
        if ($cvtermsynonym) {
          $this->addMsg($mcl_tmpl, 'D', 'cvtermsynonym', $args);
        }
        else {
          $cvtermsynonym = new CHADO_CVTERMSYNONYM($args);
          if ($cvtermsynonym->insert()) {
            $this->addMsg($mcl_tmpl, 'N', 'cvtermsynonym', $args);
          }
          else {
            $this->addMsg($mcl_tmpl, 'E', 'cvtermsynonym', $args);
            $flag = FALSE;
          }
        }
      }
    }
    return $flag;
  }

  /**
   * Updates aliases.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $alias
   * @param string $separator
   *
   * @return boolean
   */
  public function updateAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $cv_name, $cvterm_name, $alias, $separator = '') {
    if ($alias) {

      // Removes the existing properties first and add new ones.
      if (!$this->deleteAlias($cv_name, $cvterm_name)) {
        return FALSE;
      }
      return $this->addAlias($mcl_tmpl, $cv_name, $cvterm_name, $alias, $separator);
    }
    return TRUE;
  }

  /**
   * Deletes all aliases for this cvterm.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return boolean
   */
  public function deleteAlias($cv_name, $cvterm_name) {

    // Gets type_id.
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    $args = array(
      'cvterm_id' => $this->cvterm_id,
      'type_id'   => $type_id,
    );

    // Deletes the aliases.
    $sql = "DELETE FROM {chado.cvtermsynonym} WHERE cvterm_id = :cvterm_id AND type_id = :type_id";
    db_query($sql, $args);
    return TRUE;
  }

  /**
   * Adds the provided values for quantitative_code, code and qualitative.
   *
   * Examples of quantitative_code.
   *   1:none;2:slight;3:medium;4:very greasy
   *   1:immature (starchy,granular);2:mature (no detectable startch);3:over mature (soft, lacks acid and contains overmature flavors)
   *
   * Examples of code.
   *   0:none;1:present
   *   1=UMN; 2=Cornell-Geneva; 3=WSU
   *
   * Examples of qualitative.
   *   Site; State;
   *   tier; field;
   *   tier; block;
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $data_type
   * @param string $values
   *
   * @return boolean
   */
  public function addValues(MCL_TEMPLATE $mcl_tmpl = NULL, $data_type, $values) {
    $flag = TRUE;
    if ($values) {

      // Adds the values for quantitative_code and code.
      if (preg_match("/^(code|quantitative_code)$/i", $data_type)) {

        // Gets type_id.
        $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'code');
        if (!$cvterm) {
          $this->updateMsg($mcl_tmpl, 'E', "(cv_name, cvterm_name) = ('SITE_CV', 'code') does not exist in cvterm");
          return FALSE;
        }
        $cvterm_id_code = $cvterm->getCvtermID();

        // Saves the all codes in cvtermprop table.
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'code_ref', $values)) {
          $flag = FALSE;
        }

        // Parses out the values to the codes.
        $codes = preg_split("/[;]/", $values, NULL, PREG_SPLIT_NO_EMPTY);
        foreach ($codes as $code) {
          $tmp    = preg_split("/[:=]/", $code, NULL, PREG_SPLIT_NO_EMPTY);
          $rank   = trim($tmp[0]);
          $value  = trim($tmp[1]);

          // Adds a property.
          if(!$this->addPropertyRanked($mcl_tmpl, 'cvtermprop', 'cvterm_id', $this->cvterm_id, $cvterm_id_code, $value, $rank)) {
            $flag = FALSE;
          }
        }
      }

      // Adds the values for qualitative.
      else if ($data_type == 'qualitative') {
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'qualitative', $values, '[;,]')) {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Linds between a descript and a trait.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_CVTERM $cv_name
   *
   * @return boolean
   */
  public function linkTrait(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_CVTERM $trait) {
    if ($trait) {

      // Links a descriptor and a trait.
      $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'belongs_to')->getCvtermID();
      return $this->addLink($mcl_tmpl, 'cvterm_relationship', 'subject_id', $this->cvterm_id, 'object_id', $trait->getCvtermID(), $type_id);
    }
    return TRUE;
  }
}