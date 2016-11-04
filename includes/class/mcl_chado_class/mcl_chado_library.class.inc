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
   */
  public static function checkLibrary(MCL_TEMPLATE $mcl_tmpl, $uniquename, $type_id, $organism_id) {
    if ($uniquename && $type_id) {

      // Gets the library.
      $mcl_library = MCL_CHADO_LIBRARY::getLibrary($uniquename, $type_id, $organism_id);
      if (!$mcl_library) {
        $args = array(
          'uniquename'  => $uniquename,
          'type_id'     => $type_id,
          'organism_id' => $organism_id,
        );
        $mcl_tmpl->addMsg('E', 'library', $args);
      }
    }
  }

  /**
   * Returns the library.
   *
   * @param string $uniquename
   * @param integer $type_id
   *
   * @return MCL_CHADO_LIBRARY
   */
  public static function getLibrary($uniquename, $type_id) {
    $keys = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
      'organism_id' => MCL_VAR::getValueByName('ORGANISM_ID'),
    );
    return  MCL_CHADO_LIBRARY::bykey($keys);
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
  public static function addLibrary(MCL_TEMPLATE $mcl_tmpl, $uniquename, $type_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'type_id'     => $type_id,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $args['organism_id'] = MCL_VAR::getValueByName('ORGANISM_ID');
    $mcl_library = MCL_CHADO_LIBRARY::byKey($args);
    if ($mcl_library) {
      $mcl_tmpl->addMsg('D', 'library', $args);
    }
    else {

      // Adds a new library.
      $args['name']  = $uniquename;
      $mcl_library = new MCL_CHADO_LIBRARY($args);
      if ($mcl_library->insert()) {
        $mcl_tmpl->addMsg('N', 'library', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'library', $args);
      }
    }
    return $mcl_library;
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
    $mcl_tmpl->addProperty('libraryprop', 'library_id', $this->library_id, $type_id, $value, $separator);
  }

  /**
   * Adds synoym and library_synoym.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_SYNONYM $mcl_synonym
   */
  public function addSynonym(MCL_TEMPLATE $mcl_tmpl, MCL_CHADO_SYNONYM $mcl_synonym) {
    if ($mcl_synonym) {

      // Checks for duplication.
      $args = array(
          'synonym_id'  => $mcl_synonym->getSynonymID(),
          'library_id'  => $this->library_id,
          '$pub_id'     => MCL_VAR::getValueByName('PUB_ID'),
      );
      $chado_library_synonym = CHADO_LIBRARY_SYNONYM::byKey($args);
      if ($chado_library_synonym) {
        $mcl_tmpl->addMsg('D', 'library_synonym', $args);
      }
      else {

        // Adds a new relationship.
        $chado_library_synonym = new CHADO_LIBRARY_SYNONYM($args);
        if ($chado_library_synonym->insert()) {
          $mcl_tmpl->addMsg('N', 'library_synonym', $args);
        }
        else {
          $mcl_tmpl->addMsg('E', 'library_synonym', $args);
        }
      }
    }
  }

  /**
   * Adds contact to library_contact.
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
          $mcl_tmpl->addLink('library_contact', 'library_id', $this->library_id, 'contact_id', $mcl_contact->getContactID());
        }
      }
    }
  }

  /**
   * Adds reference to library_pub.
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
          $mcl_tmpl->addLink('library_pub', 'library_id', $this->library_id, 'pub_id', $pub_id);
        }
      }
    }
  }
}