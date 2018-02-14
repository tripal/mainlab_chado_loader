<?php
/**
 * The declaration of MCL_CHADO_STOCK class.
 *
 */
class MCL_CHADO_STOCK extends CHADO_STOCK {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_STOCK::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_STOCK::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * Returns MCL_CHADO_STOCK by ID.
   *
   * @param integer $stock_id
   *
   * @return MCL_CHADO_STOCK
   */
  public static function byID($stock_id) {
    return MCL_CHADO_STOCK::byKey(array('stock_id' => $stock_id));
  }

  /**
   * @see CHADO_STOCK::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existence of stock. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $stock
   * @param string $genus
   * @param string $species
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkStock(MCL_TEMPLATE $mcl_tmpl = NULL, $stock, $genus = '', $species = '', $separator = '') {
    $flag = TRUE;
    if ($stock) {

      // Gets the organism_id.
      $organism_id = NULL;
      if ($genus && $species) {
        $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if (!$organism) {
          self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
          return FALSE;
        }
        $organism_id = $organism->getOrganismID();
      }

      // Gets the uniquenames.
      $uniquenames = preg_split(self::getSepRegex($separator), $stock, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($uniquenames as $uniquename) {
        $uniquename = trim($uniquename);

        // Sets the arguments.
        $args = array('uniquename' => $uniquename);
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

        // Checks the stock.
        $stock = MCL_CHADO_STOCK::byKey($args);
        if (!$stock) {
          self::updateMsg($mcl_tmpl, 'E', self::arrStr($args) . " not found in stock");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the existence of the secondary ID. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $second_id
   * @param string $genus
   * @param string $species
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkSecondaryID(MCL_TEMPLATE $mcl_tmpl = NULL, $second_id, $genus = '', $species = '', $separator = '') {
    $flag = TRUE;
    if ($second_id) {

      // Gets the organism_id.
      $organism_id = NULL;
      if ($genus && $species) {
        $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if (!$organism) {
          self::updateMsg($mcl_tmpl, 'E', "$genus $species not found in organism");
          return FALSE;
        }
        $organism_id = $organism->getOrganismID();
      }

      // Gets the uniquenames.
      $names = preg_split(self::getSepRegex($separator), $second_id, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($names as $name) {
        $name = trim($name);

        // Sets the arguments.
        $args = array('name' => $name);
        if ($organism_id) {
          $args['organism_id'] = $organism_id;
        }

        // If '::' found in name, update uniquename and organism_id.
        if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $name, $matches)) {
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

        // Checks the stock.
        $stock = MCL_CHADO_STOCK::byKey($args);
        if (!$stock) {
          self::updateMsg($mcl_tmpl, 'E', self::arrStr($args) . " not found in stock");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Checks the format of alias. Write the error messasge to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $alias
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $alias, $separator = '') {
    $flag = TRUE;
    if ($alias) {

      // Gets the alias.
      $aliases = preg_split(self::getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($aliases as $value) {
        $tmp = preg_split("/:/", $value);
        $num_elem = sizeof($tmp);
        if ($num_elem > 2) {
          self::updateMsg($mcl_tmpl, 'E', "$value is not a valid format. It should be alias_type:alias");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Returns the stock by uniquename, genus and species.
   *
   * @param string $uniquename
   * @param string $genus
   * @param string $species
   *
   * @return MCL_CHADO_STOCK
   */
  public static function getStock($uniquename, $genus = '', $species = '') {

    // Sets the args.
    $args = array(
      'uniquename' => $uniquename,
    );

    // Adds the organism.
    if ($genus && $species) {
      $organism = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
      $args['organism_id'] = $organism->getOrganismID();
    }
    return MCL_CHADO_STOCK::byKey($args);
  }

  /**
   * Adds a stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $name
   * @param integer $organism_id
   * @param integer $type_id
   *
   * @return MCL_CHADO_STOCK
   */
  public static function addStock(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $name, $organism_id, $type_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $stock = MCL_CHADO_STOCK::byKey($args);
    if ($stock) {
      self::addMsg($mcl_tmpl, 'D', 'stock', $args);
    }
    else {

      // Adds a new stock.
      $args['type_id']  = $type_id;
      $args['name']     = $name;
      $stock = new MCL_CHADO_STOCK($args);
      if ($stock->insert()) {
        self::addMsg($mcl_tmpl, 'N', 'stock', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'stock', $args);
        return NULL;
      }
    }
    return $stock;
  }

  /**
   * Adds a stock by a secondary ID.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param string $secondary_id
   * @param integer $organism_id
   * @param integer $type_id
   *
   * @return MCL_CHADO_STOCK
   */
  public static function addStockBy2ndID(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $secondary_id, $organism_id, $type_id) {

    // Sets the arguments.
    $args = array(
      'uniquename'  => $uniquename,
      'name'        => $secondary_id,
      'organism_id' => $organism_id,
    );

    // Checks the arguments.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication with the secondary ID.
    $args = array(
      'name'        => $secondary_id,
      'organism_id' => $organism_id,
    );
    $stock = MCL_CHADO_STOCK::byKey($args);
    if ($stock) {
      self::addMsg($mcl_tmpl, 'D', 'stock', $args);
    }
    else {

      // Checks for duplication with the uniquename.
      $args = array(
        'uniquename'  => $uniquename,
        'organism_id' => $organism_id,
      );
      $stock = MCL_CHADO_STOCK::byKey($args);
      if ($stock) {
        self::updateMsg($mcl_tmpl, 'E', "The uniquename ($uniquename) has a different secondary ID");
      }
      else {

        // Adds a new stock.
        $args = array(
          'uniquename'  => $uniquename,
          'name'        => $secondary_id,
          'organism_id' => $organism_id,
          'type_id'     => $type_id,
        );
        $stock = new MCL_CHADO_STOCK($args);
        if ($stock->insert()) {
          self::addMsg($mcl_tmpl, 'N', 'stock', $args);
        }
        else {
          self::addMsg($mcl_tmpl, 'E', 'stock', $args);
          return NULL;
        }
      }
    }
    return $stock;
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
      return $this->addProperty($mcl_tmpl, 'stockprop', 'stock_id', $this->stock_id, $type_id, $value, $separator);
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
      $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
      if ($cvterm) {
        $type_id = $cvterm->getCvtermID();
        return $this->addProperty($mcl_tmpl, 'stockprop', 'stock_id', $this->stock_id, $type_id, $value, $separator);
      }
    }
    return TRUE;
  }

  /**
   * Returns if this stock is sample.
   *
   * @return boolean
   */
  public function isSample() {
    return TRUE;
  }

  /**
   * Returns if this stock is clone.
   *
   * @return boolean
   */
  public function isClone() {
    return TRUE;
  }

  /**
   * Returns the clone ID.
   *
   * @param string $separator
   *
   * @return string
   */
  public function getCloneID($separator = ';') {

    // Gets the type_id.
    $cvterm_name  = $this->isSample() ? 'sample_of' : 'clone_of';
    $type_id      =  MCL_CHADO_CVTERM::getCvterm('SITE_CV', $cvterm_name)->getCvtermID();

    // Gets clone IDs.
    $sql = '';
    if ($this->isSample()) {
      $sql = "
        SELECT SR.object_id
        FROM {chado.stock_relationship} SR
        WHERE SR.type_id = :type_id AND SR.subject_id = :target_id
      ";
      $args[':target_id'] = $this->stock_id;
    }
    else {
      $sql = "
        SELECT SR.subject_id
        FROM {chado.stock_relationship} SR
        WHERE SR.type_id = :type_id AND SR.object_id = :target_id
      ";
    }
    $args = array(
      ':target_id'  => $this->stock_id,
      ':type_id'     => $type_id,
    );
    $results = db_query($sql, $args);
    $clones = array();
    while ($stock_id = $results->fetchField()) {
      $s = MCL_CHADO_STOCK::byID($stock_id);
      if ($s) {
        $clones []= $s->getName();
      }
    }
    return implode($separator, $clones);
  }

  /**
   * Returns nd_experiment_id of all experiments belong to this sample.
   *
   * @return array
   */
  public function getExperiments() {
    $nd_experiment_ids = array();
    if ($this->isSample()) {
      $sql = "
        SELECT NES.nd_experiment_id
        FROM {chado.nd_experiment_stock} NES
        WHERE NES.stock_id = :stock_id
      ";
      $results = db_query($sql, array(':stock_id' => $this->stock_id));
      while ($nd_experiment_id = $results->fetchField()) {
        $nd_experiment_ids []= $nd_experiment_id;
      }
    }
    return nd_experiment_ids;
  }

  /**
   * Adds a related stock.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_STOCK $stock
   * @param integer $type_id_relationship
   *
   * @return boolean
   */
  public function addRelatedStock(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_STOCK $stock, $type_id_relationship) {
    if ($stock) {
      return $this->addRelationship($mcl_tmpl, 'stock_relationship', 'subject_id', $this->stock_id, 'object_id', $stock->getStockID(), $type_id_relationship);
    }
    return TRUE;
  }

  /**
   * Adds parents by dataset name and cross number.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_ND_EXPERIMENT $cross
   * @param integer $type_id
   *
   * @return boolean
   */
  public function addCross(MCL_TEMPLATE $mcl_tmpl = NULL, $cross, $type_id) {
    if ($cross) {
      return $this->addLink($mcl_tmpl, 'nd_experiment_stock', 'stock_id', $this->stock_id, 'nd_experiment_id', $cross->getNdExperimentID(), $type_id);
    }
    return TRUE;
  }

  /**
   * Adds a parent.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $uniquename
   * @param integer $organism_id
   * @param integer $type_id_relationship
   *
   * @return boolean
   */
  public function addParent(MCL_TEMPLATE $mcl_tmpl = NULL, $uniquename, $organism_id, $type_id_relationship) {
    if ($uniquename && $type_id_relationship) {

      // If '::' found in uniquename, update uniquename and organism_id.
      if (preg_match("/^(.*?)\s+(.*?)::(.*?)$/", $uniquename, $matches)) {
        $genus          = trim($matches[1]);
        $species        = trim($matches[2]);
        $uniquename     = trim($matches[3]);
        $diff_organism  = MCL_CHADO_ORGANISM::getOrganism($genus, $species);
        if ($diff_organism) {
          $organism_id = $diff_organism->getOrganismID();
        }
      }

      // Adds a parent.
      $args = array('uniquename' => $uniquename);
      if ($organism_id) {
        $args['organism_id'] = $organism_id;
      }
      $parent = MCL_CHADO_STOCK::byKey($args);
      if ($parent) {
        if (!$parent->addRelatedStock($mcl_tmpl, $this, $type_id_relationship)) {
          return FALSE;
        }
      }
      else {

        if ($mcl_tmpl) {

          // Saves the parent that could not find in stock table. They may be
          // listed in the later row of the same Excel file.
          // Adds them after processing the stock sheet.
          $missing_parent = array(
            'uniquename'  => $uniquename,
            'organism_id' => $organism_id,
            'type_id'     => $type_id_relationship,
          );
          $parents = $mcl_tmpl->getTmpByKey('stock_parents');
          if (!is_array($parents)) {
            $parents = array();
          }
          $parents[$this->stock_id] = $missing_parent;
          $mcl_tmpl->setTmpByKey('stock_parents', $parents);
        }
        else {
          return FALSE;
        }
      }
    }
    return TRUE;
  }

  /**
   * Updates the parent by stock ID.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param integer $stock_id
   * @param integer $type_id
   *
   * @return boolean
   */
  public function updateParentByID(MCL_TEMPLATE $mcl_tmpl = NULL, $stock_id, $type_id) {
    if ($type_id) {

      // Sets properties.
      $args = array(
        'object_id' => $this->stock_id,
        'type_id'   => $type_id,
      );
      $rel = CHADO_STOCK_RELATIONSHIP::byKey($args);
      if ($rel) {

        // Updates the relationship.
        if ($stock_id) {
          $rel->setSubjectID($stock_id);
          return $rel->update();
        }

        // Removes the relationship if stock ID is empty.
        else {
          return $rel->delete();
        }
      }
      else {
        $args['subject_id'] = $stock_id;
        $rel = new CHADO_STOCK_RELATIONSHIP($args);
        return $rel->insert();
      }
    }
    return FALSE;
  }

  /**
   * Adds alias to stockprop.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public function addAlias(MCL_TEMPLATE $mcl_tmpl = NULL, $alias, $separator = '') {
    $flag = TRUE;
    if ($alias) {
      $arr_alias    = array();
      $arr_cultivar = array();
      $aliases = preg_split($this->getSepRegex($separator), $alias, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($aliases as $value) {
        $tmp = preg_split("/:/", $value);
        if (sizeof($tmp) > 1) {
          $arr_cultivar []= $tmp[1];
        }
        else {
          $arr_alias []= $value;
        }
      }

      // Adds the values as aliases.
      if (!empty($arr_alias)) {
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'alias', implode(';', $arr_alias), $separator)) {
          $flag = FALSE;
        }
      }

      // Adds the values as cultivars.
      if (!empty($arr_cultivar)) {
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'cultivar', implode(';', $arr_cultivar), $separator)) {
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds a dbxref to stock_dbxref.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param MCL_CHADO_DBXREF $dbxref
   *
   * @return boolean
   */
  public function addDBXref(MCL_TEMPLATE $mcl_tmpl = NULL, MCL_CHADO_DBXREF $dbxref) {
    if ($dbxref) {
      return $this->addLink($mcl_tmpl, 'stock_dbxref', 'stock_id', $this->stock_id, 'dbxref_id', $dbxref->getDbxrefID());
    }
    return TRUE;
  }

  /**
   * Adds contact to stock_contact.
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
          if (!$this->addLink($mcl_tmpl, 'stock_contact', 'stock_id', $this->stock_id, 'contact_id', $contact->getContactID())) {
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
   * Adds image IDs to stock_image.
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
          if (!$this->addLink($mcl_tmpl, 'stock_image', 'stock_id', $this->stock_id, 'eimage_id', $image->getEimageID())) {
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
   * Adds images to stock_image as a trait.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $type
   * @param string $image_list
   * @param integer $cvterm_id
   * @param string $separator
   *
   * @return boolean
   */
  public function addImage(MCL_TEMPLATE $mcl_tmpl = NULL, $type, $image_list, $cvterm_id = '', $separator = '') {
    $flag = TRUE;
    if ($image_list) {
      $eimages = preg_split($this->getSepRegex($separator), $image_list, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimages as $eimage_filename) {
        $eimage_filename = trim($eimage_filename);

        // Add image.
        $eimage = MCL_CHADO_IMAGE::addImage($eimage_filename, $type, $eimage_filename, $cvterm_id);
        if ($eimage) {

          // Associated with stock.
          if (!$this->addLink($mcl_tmpl, 'stock_image', 'stock_id', $this->stock_id, 'eimage_id', $eimage->getEimageID())) {
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
   * Adds reference to stock_pub.
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
          if (!$this->addLink($mcl_tmpl, 'stock_pub', 'stock_id', $this->stock_id, 'pub_id', $pub_id)) {
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
   * Adds reference to stock_pub.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $previous_entry
   *
   * @return boolean
   */
  public function addPreviousEntry(MCL_TEMPLATE $mcl_tmpl = NULL, $previous_entry) {
    $flag = TRUE;
    if ($previous_entry) {

      // Checks the sample.
      $sample = MCL_CHADO_STOCK::byKey(array('uniquename' => $previous_entry));
      if ($sample) {

        // Adds the sample_id in stockprop.
        if (!$this->addProp($mcl_tmpl, 'SITE_CV', 'previous_entry', $sample->getStockID())) {
          $flag = FALSE;
        }
      }
      else {
        $this->updateMsg($mcl_tmpl, 'E', "Error - sample : '$previous_entry' does not exist");
        $flag = FALSE;
      }
    }
    return $flag;
  }

  /**
   * Gets property.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getProp($cv_name, $cvterm_name) {
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    return $this->getProperty('stockprop', 'stock_id', $this->stock_id, $type_id);
  }

  /**
   * Retrieves the properties of the stock.
   *
   * @param string $cv_name
   * @param string $cvterm_name
   * @param string $separator
   *
   * @return string
   */
  public function getProps($cv_name, $cvterm_name, $separator = ':') {
    $type_id = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name)->getCvtermID();
    $sql = "
      SELECT SP.value
      FROM {chado.stockprop} SP
      WHERE SP.stock_id = :stock_id AND SP.type_id = :type_id
    ";
    $args = array(
      ':type_id' => $type_id,
      ':stock_id' => $this->stock_id,
    );
    $results = db_query($sql, $args);
    $values = array();
    while ($value = $results->fetchField()) {
      $values []= $value;
    }
    return implode($separator, $values);
  }

  /**
   * Retrieves the parent of the stock.
   *
   * @param string $cvterm_name
   *
   * @return string
   */
  public function getParent($cv_name, $cvterm_name) {
    $cvterm = MCL_CHADO_CVTERM::getCvterm($cv_name, $cvterm_name);
    $keys = array(
      'object_id' => $this->stock_id,
      'type_id' => $cvterm->getCvtermID(),
    );
    $rel = CHADO_STOCK_RELATIONSHIP::byKey($keys);
    if ($rel) {
      $parent = MCL_CHADO_STOCK::byID($rel->getSubjectID());
      return $parent->getName();
    }
    return '';
  }

  /**
   * Retrieves the aliases of the stock.
   *
   * @param string $separator
   *
   * @return string
   */
  public function getAlias($separator = ':') {
    return $this->getProps('SITE_CV', 'alias', $separator);
  }

  /**
   * Retrieves the type of the stock.
   *
   * @return string
   */
  public function getType() {
    $cvterm = MCL_CHADO_CVTERM::byID($this->type_id);
    return $cvterm->getName();
  }

  /**
   * Retrieves the genus of the stock.
   *
   * @return string
   */
  public function getGenus() {
    $organism = MCL_CHADO_ORGANISM::byID($this->organism_id);
    return $organism->getGenus();
  }

  /**
   * Retrieves the species of the stock.
   *
   * @return string
   */
  public function getSpecies() {
    $organism = MCL_CHADO_ORGANISM::byID($this->organism_id);
    return $organism->getSpecies();
  }

}