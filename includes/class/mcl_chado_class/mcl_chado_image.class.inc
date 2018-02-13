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
   * Checks the existence of eimage. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $image_id_list
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkImageID(MCL_TEMPLATE $mcl_tmpl = NULL, $image_id_list, $separator = '') {
    $flag = TRUE;
    if ($image_id_list) {

      // Gets eimage_id.
      $eimage_ids = preg_split(self::getSepRegex($separator), $image_id_list, NULL, PREG_SPLIT_NO_EMPTY);
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
   * Checks the existence of eimage. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $image_list
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkImage(MCL_TEMPLATE $mcl_tmpl = NULL, $image_list, $separator = '') {
    $flag = TRUE;
    if ($image_list) {

      // Gets eimage.
      $images = preg_split(self::getSepRegex($separator), $image_list, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($images as $image) {
        $image = trim($image);
        $image = MCL_CHADO_IMAGE::byKey(array('image_uri' => $image));
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
   * @param integer $cvterm_id
   *
   * @return MCL_CHADO_IMAGE
   */
  public static function addImage(MCL_TEMPLATE $mcl_tmpl = NULL, $eimage_data, $eimage_type, $image_uri, $cvterm_id = '') {

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

        // If cvterm_id is not empty save it in eimageprop. The image is
        // added as a trait.
        if ($cvterm_id) {
          if (!$image->addProp('SITE_CV', 'trait_descriptor', $cvterm_id)) {
            $flag = FALSE;
          }
        }
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