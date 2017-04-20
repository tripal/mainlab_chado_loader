<?php
/**
 * The declaration of MCL_CHADO_IMAGE class.
 *
 */
class MCL_CHADO_IMAGE extends CHADO_EIMAGE {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_EIMAGE::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_EIMAGE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_EIMAGE::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of eimage. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $image
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkImage(MCL_TEMPLATE $mcl_tmpl = NULL, $image, $separator = '') {
    $flag = TRUE;
    if ($image) {

      // Gets eimage_id.
      $eimage_ids = preg_split(self::getSepRegex($separator), $image, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        if (!is_int($eimage_id)) {
          self::updateMsg($mcl_tmpl, 'E', "$eimage_id is not an integer. It must be eimage_id");
          $flag = FALSE;
          continue;
        }
        $args = array('eimage_id' => $eimage_id);
        $image = MCL_CHADO_IMAGE::byKey($args);
        if (!$image) {
          self::addMsg($mcl_tmpl, 'E', 'eimage', $args);
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Adds an image.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $eimage_data
   * @param string $eimage_type
   * @param string $image_uri
   *
   * @return MCL_CHADO_DBXREF
   */
  public static function addImage(MCL_TEMPLATE $mcl_tmpl = NULL, $eimage_data, $eimage_type, $image_uri) {

    // Sets the arguments.
    $args = array(
      'eimage_data' => $eimage_data,
      'eimage_type' => $eimage_type,
      'image_uri'   => $image_uri,
    );

    // Checks parameters.
    if (!self::checkReqArgs($args)) {
      return NULL;
    }

    // Checks for duplication.
    $image = MCL_CHADO_IMAGE::byKey($args);
    if ($image) {
      self::updateMsg($mcl_tmpl, 'D', 'eimage', $args);
    }
    else {
      // Adds a new image.
      $image = new MCL_CHADO_IMAGE($args);
      if ($image->insert()) {
        self::updateMsg($mcl_tmpl, 'N', 'eimage', $args);
      }
      else {
        self::addMsg($mcl_tmpl, 'E', 'eimage', $args);
        return NULL;
      }
    }
    return $image;
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
      return $this->addProperty($mcl_tmpl, 'eimageprop', 'eimage_id', $this->eimage_id, $type_id, $value, $separator);
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
      return $this->addProperty($mcl_tmpl, 'eimageprop', 'eimage_id', $this->eimage_id, $type_id, $value, $separator);
    }
    return TRUE;
  }
}