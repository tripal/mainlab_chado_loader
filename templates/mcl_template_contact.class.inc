<?php
/**
 * The declaration of MCL_TEMPLATE_CONTACT class.
 *
 */
class MCL_TEMPLATE_CONTACT extends MCL_TEMPLATE {

  /**
   *  Class data members.
   */
  /**
   * @see MCL_UPLOAD::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'CONTACT';
    parent::__construct($details);
  }

  /**
   * @see MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for contact sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'contact_name'  => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the contact (person, institution, etc)."),
      'alias'         => array('req' => FALSE,  'width' => 10, 'desc' => "Any alias of the contact."),
      'type'          => array('req' => TRUE,   'width' => 10, 'desc' => "Name of the contact - choose one from person, company, institution, lab or organization)."),
      'first_name'    => array('req' => FALSE,  'width' => 12, 'desc' => "First name of the person."),
      'last_name'     => array('req' => FALSE,  'width' => 12, 'desc' => "Last name of the person."),
      'institution'   => array('req' => FALSE,  'width' => 12, 'desc' => "Name of the institution the person belongs to."),
      'lab'           => array('req' => FALSE,  'width' => 10, 'desc' => "Name of the lab the person belongs to."),
      'address'       => array('req' => FALSE,  'width' => 10, 'desc' => "Address of the contact."),
      'email'         => array('req' => FALSE,  'width' => 10, 'desc' => "Email of the contact."),
      'phone'         => array('req' => FALSE,  'width' => 10, 'desc' => "Phone number of the contact."),
      'fax'           => array('req' => FALSE,  'width' => 10, 'desc' => "Fax number of the contact."),
      'title'         => array('req' => FALSE,  'width' => 10, 'desc' => "Job title of the person."),
      'country'       => array('req' => FALSE,  'width' => 10, 'desc' => "Country of the contact."),
      'source'        => array('req' => FALSE,  'width' => 10, 'desc' => "Source of the data when the contact information is obtained from other databases."),
      'last_update'   => array('req' => FALSE,  'width' => 12, 'desc' => "Date of the last update."),
      'url'           => array('req' => FALSE,  'width' => 10, 'desc' => "URL of home page."),
      'comments'      => array('req' => FALSE,  'width' => 14, 'desc' => "Any comments of the contact."),
      'keywords'      => array('req' => FALSE,  'width' => 15, 'desc' => "A maximum of 5 keywords that best describe the person's area of work."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['institution']   = -1;
    $cvterms['SITE_CV']['lab']           = -1;
    $cvterms['SITE_CV']['first_name']    = -1;
    $cvterms['SITE_CV']['last_name']     = -1;
    $cvterms['SITE_CV']['phone']         = -1;
    $cvterms['SITE_CV']['fax']           = -1;
    $cvterms['SITE_CV']['email']         = -1;
    $cvterms['SITE_CV']['address']       = -1;
    $cvterms['SITE_CV']['country']       = -1;
    $cvterms['SITE_CV']['title']         = -1;
    $cvterms['SITE_CV']['keywords']      = -1;
    $cvterms['SITE_CV']['alias']         = -1;
    $cvterms['SITE_CV']['source']        = -1;
    $cvterms['SITE_CV']['last_update']   = -1;
    $cvterms['SITE_CV']['comments']      = -1;
    $cvterms['SITE_CV']['url']           = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks contact type.
    $type = $line['type'];
    if ($type == 'person') {
      $first_name = $line['first_name'];
      $last_name  = $line['last_name'];
      if (!$first_name || !$last_name) {
        $this->updateMsg('E', "type : first_name and last_name must be not empty");
      }
    }

    // Checks for the contact type.
    if (!MCL_DATA_VALID_TYPE::validate('contact_type', $type)) {
      $this->updateMsg('E', "Invalid contact type : $type");
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets contact type.
    $type_id = MCL_CHADO_CVTERM::getCvterm('SITE_CV', $line['type'])->getCvtermID();

    // Adds a contact.
    $contact = MCL_CHADO_CONTACT::addContact($this, $line['contact_name'], $type_id, $line['description']);
    if ($contact) {

      // Adds properties.
      $contact->addProp($this, 'SITE_CV', 'institution', $line['institution']);
      $contact->addProp($this, 'SITE_CV', 'lab', $line['lab']);
      $contact->addProp($this, 'SITE_CV', 'first_name', $line['first_name']);
      $contact->addProp($this, 'SITE_CV', 'last_name', $line['last_name']);
      $contact->addProp($this, 'SITE_CV', 'email', $line['email']);
      $contact->addProp($this, 'SITE_CV', 'phone', $line['phone']);
      $contact->addProp($this, 'SITE_CV', 'fax', $line['fax']);
      $contact->addProp($this, 'SITE_CV', 'address', $line['address']);
      $contact->addProp($this, 'SITE_CV', 'country', $line['country']);
      $contact->addProp($this, 'SITE_CV', 'title', $line['title']);
      $contact->addProp($this, 'SITE_CV', 'last_update', $line['last_update']);
      $contact->addProp($this, 'SITE_CV', 'url', $line['url']);
      $contact->addProp($this, 'SITE_CV', 'comments', $line['comments']);
      $contact->addProp($this, 'SITE_CV', 'keywords', $line['keywords'], '[,;]');

      // Adds aliases.
      $contact->addProp($this, 'SITE_CV', 'alias', $line['alias'], '[,;]');
    }
  }
}