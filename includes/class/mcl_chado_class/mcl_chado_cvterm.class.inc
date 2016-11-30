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
   * Checks the existance of cvterm. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   */
  public static function checkCvterm(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name) {
    if ($cv_name && $cvterm_name) {
      // Gets the cvterm.
      $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      if (!$mcl_cvterm) {
        $mcl_tmpl->updateMsg('E', "(cv_name, cvterm_name) = ($cv_name, $cvterm_name) not found in cvterm");
      }
    }
  }

  /**
   * Checks the format of the values for quantitative_code and code.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $data_type
   * @param string $values
   */
  public static function checkValues(MCL_TEMPLATE $mcl_tmpl, $data_type, $values) {

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
            $mcl_tmpl->updateMsg('E', "Rank should be an integer ($rank was given)");
          }

          // Checks a value.
          if (!$value) {
            $mcl_tmpl->updateMsg('E', "Value must be non-empty");
          }
        }
      }
      else {
        $mcl_tmpl->updateMsg('E', "values cannot be empty for $data_type.");
      }
    }
  }

  /**
   * Returns the cvterm.

   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return MCL_CHADO_CVTERM
   */
  public static function getCvterm($cv_name, $cvterm_name) {
    $mcl_cv = MCL_CHADO_CV::getCV($cv_name);
    if ($mcl_cv) {
      return MCL_CHADO_CVTERM::bykey(array('cv_id' => $mcl_cv->getCvID(), 'name' => $cvterm_name));
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
    $mcl_cv     = MCL_CHADO_CV::addNull();
    $mcl_dbxref = MCL_CHADO_DBXREF::addNull();

    // Checks if it has already exists.
    $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm('null', 'null');
    if (!$mcl_cvterm) {
      $details = array(
        'name'      => 'null',
        'cv_id'     => $mcl_cv->getCvID(),
        'dbxref_id' => $mcl_dbxref->getDbxrefID(),
      );
      $mcl_cvterm = new MCL_CHADO_CVTERM($details);
      if (!$mcl_cvterm->insert()) {
        return NULL;
      }
    }
    return $mcl_cvterm;
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
  public static function addCvterm(MCL_TEMPLATE $mcl_tmpl, $db_name, $cv_name, $cvterm_name, $definition, $accession = '') {

    // Checks parameters.
    $args = array(
      'db_name'     => $db_name,
      'cv_name'     => $cv_name,
      'cvterm_name' => $cvterm_name,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    if ($mcl_cvterm) {
      $mcl_tmpl->updateMsg('D', "(cv.name, cvterm.name) = ($cv_name, $cvterm_name) found in cvterm");
    }
    else {

       // Gets db.
      $mcl_db = MCL_CHADO_DB::getDB($db_name);
      if (!$mcl_db) {
        $mcl_tmpl->updateMsg('E', "(db.name) = ($db_name) not found in db");
        return NULL;
      }

      // Gets cv.
      $mcl_cv = MCL_CHADO_CV::getCV($cv_name);
      if (!$mcl_cv) {
        $mcl_tmpl->updateMsg('E', "(cv.name) = ($cv_name) not found in cv");
        return NULL;
      }

      // Sets the accession.
      if (!$accession) {
        $accession = strtoupper($mcl_db->getName()) . ":$cv_name:$cvterm_name";
      }

      // Gets dbxref.
      $args = array(
        'db_id'     => $mcl_db->getDbID(),
        'accession' => $accession,
      );
      $mcl_dbxref = MCL_CHADO_DBXREF::byKey($args);
      if (!$mcl_dbxref) {

        // Adds a new dbxref.
        $mcl_dbxref = new MCL_CHADO_DBXREF($args);
        if ($mcl_dbxref->insert()) {
          $mcl_tmpl->addMsg('N', 'dbxref', $args);
        }
        else {
          $mcl_tmpl->addMsg('E', 'dbxref', $args);
        }
      }

      // Adds a new cvterm.
      if ($mcl_dbxref) {
        $args = array(
          'dbxref_id'   => $mcl_dbxref->getDbxrefID(),
          'cv_id'       => $mcl_cv->getCvID(),
          'name'        => $cvterm_name,
          'definition'  => $definition,
        );
        $mcl_cvterm = new MCL_CHADO_CVTERM($args);
        if ($mcl_cvterm->insert()) {
          $mcl_tmpl->addMsg('N', 'cvterm', $args);
        }
        else {
          $mcl_tmpl->addMsg('E', 'cvterm', $args);
        }
      }
    }
    return $mcl_cvterm;
  }

  /**
   * Adds a property.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $value
   * @param boolean $multiple
   */
  public function addProp(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $value, $multiple = FALSE) {
    $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);
    $mcl_tmpl->addProperty('cvtermprop', 'cvterm_id', $this->cvterm_id, $type_id, $value, $multiple);
  }

  /**
   * Adds aliases.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $alias
   * @param string $separator
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl, $cv_name, $cvterm_name, $alias, $separator = '') {

    if ($alias) {
      $type_id = $mcl_tmpl->getCvtermID($cv_name, $cvterm_name);

      // Poluplates the array with aliases.
      $alias_arr = array();
      if ($separator) {
        $alias_arr = preg_split($mcl_tmpl->getSepRegex($separator), $value, NULL, PREG_SPLIT_NO_EMPTY);
      }
      else {
        $alias_arr []= $value;
      }

      // Adds each alias.
      $args = array(
        'cvterm_id' => $this->cvterm_id,
        'type_id' => $type_id,
      );
      $rank = 0;
      foreach ($alias_arr as $val) {
        $args['synonym'] = $val;
        $cvtermsynonym = CHADO_CVTERMSYNONYM::byKey($args);
        if ($cvtermsynonym) {
          $mcl_tmpl->addMsg('D', 'cvtermsynonym', $args);
        }
        else {
          $cvtermsynonym = new CHADO_CVTERMSYNONYM($args);
          if ($cvtermsynonym->insert()) {
            $mcl_tmpl->addMsg('N', 'cvtermsynonym', $args);
          }
          else {
            $mcl_tmpl->addMsg('E', 'cvtermsynonym', $args);
          }
        }
      }
    }
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
   */
  public function addValues(MCL_TEMPLATE $mcl_tmpl, $data_type, $values) {

    // Adds the values for quantitative_code and code.
    if (preg_match("/^(code|quantitative_code)$/i", $data_type)) {

      // Gets type_id.
      $cvterm_id_code = $mcl_tmpl->getCvtermID('SITE_CV', 'code');

      // Saves the all codes in cvtermprop table.
      $this->addProp($mcl_tmpl, 'SITE_CV', 'code_ref', $values);

      // Parses out the values to the codes.
      $codes = preg_split("/[;]/", $values, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($codes as $code) {
        $tmp    = preg_split("/[:=]/", $code, NULL, PREG_SPLIT_NO_EMPTY);
        $rank   = trim($tmp[0]);
        $value  = trim($tmp[1]);

        // Adds a property.
        $mcl_tmpl->addPropertyRanked('cvtermprop', 'cvterm_id', $cvterm_id_code, $type_id, $value, $rank);
      }
    }

    // Adds the values for qualitative.
    else if ($data_type == 'qualitative') {
      $this->addProp($this, 'SITE_CV', 'qualitative', $values, '[;,]');
    }
  }
}