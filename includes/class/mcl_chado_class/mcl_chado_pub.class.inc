<?php
/**
 * The declaration of MCL_CHADO_PUB class.
 *
 */
class MCL_CHADO_PUB extends CHADO_PUB {

 /**
  *  Class data members.
  */

  /**
   * @see CHADO_PUB::__construct()
   */
  public function __construct($details = array()) {
    parent::__construct($details);
  }

  /**
   * @see CHADO_PUB::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
       return new self($parent->getMemberArr());
    }
    return NULL;
  }

  /**
   * @see CHADO_PUB::__destruct()
   */
  public function __destruct() {
    parent::__destruct();
  }

  /**
   * Checks the existance of db. If not, write the error messasge
   * to the log.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $reference
   * @param string $separator
   *
   * @return boolean
   */
  public static function checkPUB(MCL_TEMPLATE $mcl_tmpl = NULL, $reference, $separator) {
    $flag = TRUE;
    if ($reference) {

      // Checks the pub.
      $pub_ids = preg_split(self::getSepRegex($separator), $reference, NULL, PREG_SPLIT_NO_EMPTY);
      foreach ($pub_ids as $pub_id) {
        $pub_id = trim($pub_id);
        $pub = MCL_CHADO_PUB::getPub($pub_id);
        if (!$pub) {
          self::updateMsg($mcl_tmpl, 'E', "(pub_id) = ($pub_id) not found in pub");
          $flag = FALSE;
        }
      }
    }
    return $flag;
  }

  /**
   * Returns the pub by pub_id.
   *
   * @param integer pub_id
   *
   * @return MCL_CHADO_PUB
   */
  public static function getPub($pub_id) {
    return MCL_CHADO_PUB::bykey(array('pub_id' => $pub_id));
  }

  /**
   * Adds null pub.
   *
   * @return MCL_CHADO_PUB
   */
  public static function addNull() {

    // Gets null cvterm.
    $cvterm = MCL_CHADO_CVTERM::addNull();

    // Checks if it has already exists.
    $pub = MCL_CHADO_PUB::byKey(array('uniquename' => 'null'));
    if (!$pub) {
      $details = array(
        'uniquename'  => 'null',
        'type_id'     => $cvterm->getCvtermID(),
      );
      $pub = new MCL_CHADO_PUB($details);
      if (!$pub->insert()) {
        return NULL;
      }
    }
    return $pub;
  }
}