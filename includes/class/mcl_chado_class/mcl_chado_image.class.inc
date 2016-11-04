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
   */
  public static function checkImage(MCL_TEMPLATE $mcl_tmpl, $image, $separator = '') {
    if ($image) {

      // Gets eimage_id.
      $eimage_ids = preg_split($mcl_tmpl->getSepRegex($separator), $image, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($eimage_ids as $eimage_id) {
        $eimage_id = trim($eimage_id);
        $args = array('eimage_id' => $eimage_id);
        $mcl_image = MCL_CHADO_IMAGE::byKey($args);
        if (!$mcl_image) {
          $mcl_tmpl->addMsg('D', 'eimage', $args);
        }
      }
    }
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
  public static function addImage(MCL_TEMPLATE $mcl_tmpl, $eimage_data, $eimage_type, $image_uri) {

    // Sets the arguments.
    $args = array(
      'eimage_data' => $eimage_data,
      'eimage_type' => $eimage_type,
      'image_uri'   => $image_uri,
    );

    // Checks parameters.
    if ($eimage_data && $eimage_type && $image_uri) {
      return NULL;
    }

    // Checks for duplication.
    $mcl_image = MCL_CHADO_IMAGE::byKey($args);
    if ($mcl_image) {
      $mcl_tmpl->addMsg('D', 'eimage', $args);
    }
    else {
      // Adds a new image.
      $mcl_image = new MCL_CHADO_IMAGE($args);
      if ($mcl_image->insert()) {
        $mcl_tmpl->addMsg('N', 'eimage', $args);
      }
    }
    return $mcl_image;
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
    $mcl_tmpl->addProperty('eimageprop', 'eimage_id', $this->eimage_id, $type_id, $value, $separator);
  }
}