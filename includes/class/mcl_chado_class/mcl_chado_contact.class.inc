<?php
/**
 * The declaration of MCL_CHADO_CONTACT class.
 *
 */
class MCL_CHADO_CONTACT extends CHADO_CONTACT {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_CONTACT::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_CONTACT::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_CONTACT by ID.
   *
   * @param integer $contact_id
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function byID($contact_id) {
    return MCL_CHADO_CONTACT::byKey(array('contact_id' => $contact_id));
  }

  /**
   * Returns the contact by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function byName($name) {
    return MCL_CHADO_CONTACT::byKey(array('name' => trim($name)));
  }

  /**
   * @see CHADO_CONTACT::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existence of contact. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkContact(MCL_TEMPLATE $mcl_tmpl = NULL, $contact, $separator = '') {
    $flag = TRUE;
    if ($contact) {

      // Gets the contacts.
      $names = preg_split(self::getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $contact = MCL_CHADO_CONTACT::byName(trim($name));
        if (!$contact) {
          self::updateMsg($mcl_tmpl, 'E', "(name) = ($name) not found in contact");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function addContact(MCL_TEMPLATE $mcl_tmpl = NULL, $name, $type_id, $description) {

    // Sets the arguments.
    $args = array(
      'name' => $name,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $contact = MCL_CHADO_CONTACT::byKey($args);
    if ($contact) {
      self::addMsg($mcl_tmpl, 'D', 'contact', $args);
    }
    else {

      // Adds a new contact.
      $args['type_id']      = $type_id;
      $args['description']  = $description;
      $contact = new MCL_CHADO_CONTACT($args);
      if ($contact->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'contact', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'contact', $args);
        return NULL;
      }
    }
    return $contact;
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
      return $this->addProperty($mcl_tmpl, 'contactprop', 'contact_id', $this->contact_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'contactprop', 'contact_id', $this->contact_id, $type_id, $value, $separator);
    }
    return TRUE;
  }
}