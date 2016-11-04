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
   * @see CHADO_CONTACT::__destruct()
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
  public static function checkContact(MCL_TEMPLATE $mcl_tmpl, $contact, $separator = '') {
    if ($contact) {

      // Gets contacts.
      $names = preg_split($mcl_tmpl->getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);
        $mcl_contact = MCL_CHADO_CONTACT::getContact($name);
        if (!$mcl_contact) {
          $mcl_tmpl->updateMsg('E', "(name) = ($name) not found in contact");
        }
      }
    }
  }

  /**
   * Returns the contact by name.
   *
   * @param string $name
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function getContact($name) {
    return MCL_CHADO_CONTACT::bykey(array('name' => $name));
  }

  /**
   * Adds a contact.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $name
   *
   * @return MCL_CHADO_CONTACT
   */
  public static function addContact(MCL_TEMPLATE $mcl_tmpl, $name, $type_id, $description) {

    // Sets the arguments.
    $args = array(
      'name' => $name,
    );

    // Checks the arguments.
    if (!$mcl_tmpl->checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_contact = MCL_CHADO_CONTACT::byKey($args);
    if ($mcl_contact) {
      $mcl_tmpl->addMsg('D', 'contact', $args);
    }
    else {

      // Adds a new contact.
      $args['type_id'] = $type_id;
      $args['description'] = $description;
      $mcl_contact = new MCL_CHADO_CONTACT($args);
      if ($mcl_contact->insert()) {
        $mcl_tmpl->addMsg('N', 'contact', $args);
      }
      else {
        $mcl_tmpl->addMsg('E', 'contact', $args);
      }
    }
    return $mcl_contact;
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
    $mcl_tmpl->addProperty('contactprop', 'contact_id', $this->contact_id, $type_id, $value, $separator);
  }
}