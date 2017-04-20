<?php
/**
 * The declaration of MCL_CHADO_LIBRARY class.
 *
 */
class MCL_CHADO_LIBRARY extends CHADO_LIBRARY {

 /**
  *  Class data members.
  */
  /**
   * @see CHADO_LIBRARY::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_LIBRARY::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_LIBRARY::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of library. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   * @param integer $organism_id
   *
   * @return boolean
   */
  public static function checkLibrary(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $type_id, $organism_id = NULL) {
    if ($uniquename && $type_id) {

      // Updates the organism_id if not provided.
      if (!$organism_id) {
        $organism_id = MCL_VAR::getValueByName('ORGANISM_ID');
      }

      // Gets the library.
      $library = MCL_CHADO_LIBRARY::getLibrary($uniquename, $type_id, $organism_id);
      if (!$library) {
         self::updateMsg($mcl_tmpl, 'E', "(uniquename, type_id, organism_id) = ($uniquename, $type_id, $organism_id) not found in library");
         return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Returns the library.
   *
   * @param string $uniquename
   * @param integer $type_id
   * @param integer $organism_id
   *
   * @return MCL_CHADO_LIBRARY
   */
  public static function getLibrary($uniquename, $type_id, $organism_id = NULL) {

    // Updates the organism_id if not provided.
    if (!$organism_id) {
      $organism_id = MCL_VAR::getValueByName('ORGANISM_ID');
    }
    $keys = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
      'organism_id' => $organism_id,
    );
    return MCL_CHADO_LIBRARY::bykey($keys);
  }

  /**
   * Adds a new library.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $type_id
   *
   * @return MCL_CHADO_LIBRARY
   */
  public static function addLibrary(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $type_id, $organism_id = NULL) {

    // Updates the organism_id if not provided.
    if (!$organism_id) {
      $organism_id = MCL_VAR::getValueByName('ORGANISM_ID');
    }

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
    $library = MCL_CHADO_LIBRARY::byKey($args);
    if ($library) {
      self::addMsg($mcl_tmpl, 'D', 'library', $args);
    }
    else {

      // Adds a new library.
      $args['name']  = $uniquename;
      $library = new MCL_CHADO_LIBRARY($args);
      if ($library->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'library', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'library', $args);
        return NULL;
      }
    }
    return $library;
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
      return $this->addProperty($mcl_tmpl, 'libraryprop', 'library_id', $this->library_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'libraryprop', 'library_id', $this->library_id, $type_id, $value, $separator);
    }
    return TRUE;
  }

  /**
   * Adds synoym and library_synoym.
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
        'library_id'  => $this->library_id,
        'pub_id'      => MCL_VAR::getValueByName('PUB_ID'),
      );
      $chado_library_synonym = CHADO_LIBRARY_SYNONYM::byKey($args);
      if ($chado_library_synonym) {
        $this->addMsg($mcl_tmpl, 'D', 'library_synonym', $args);
      }
      else {

        // Adds a new relationship.
        $chado_library_synonym = new CHADO_LIBRARY_SYNONYM($args);
        if ($chado_library_synonym->insert()) {
          $this->addMsg($mcl_tmpl, 'N', 'library_synonym', $args);
        }
        else {
          $this->addMsg($mcl_tmpl, 'E', 'library_synonym', $args);
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Adds contact to library_contact.
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
        $contact = MCL_CHADO_CONTACT::byKey(array('name' => $name));
        if ($contact) {
          if (!$this->addLink($mcl_tmpl, 'library_contact', 'library_id', $this->library_id, 'contact_id', $contact->getContactID())) {
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
   * Adds reference to library_pub.
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
          $this->addLink($mcl_tmpl, 'library_pub', 'library_id', $this->library_id, 'pub_id', $pub_id);
        }
        else {
          $this->updateMsg($mcl_tmpl, 'E', "(pub_id) = ($pub_id) not found in pub");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }
}