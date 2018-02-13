<?php
/**
 * The declaration of MCL_CUSTOM_PHENOTYPE_CALL class.
 *
 */
class MCL_CUSTOM_PHENOTYPE_CALL extends MCL_CUSTOM_TABLE {

  /**
   *  Class data members.
   */
  protected $phenotype_call_id = NULL;

  /**
   * @see MCL_CUSTOM_TABLE::__construct()
   */
  public function __construct($table_name) {
     parent::__construct($table_name);
  }

  /**
   * @see MCL_CUSTOM_TABLE::getDataByKey()
   */
  public function getDataByKey($keys) {

    // Adds the conditions.
    $conditions = '';
    $args = array();
    foreach ($keys as $key => $value) {
      $conditions .= "AND $key = :$key ";
      $args[":$key"] = $value;
    }

    // Gets the data.
    $table_name = $this->getTableName();
    $sql = "
      SELECT T.*
      FROM {$table_name} T
      WHERE 1=1 $conditions
    ";
    $obj = db_query($sql, $args)->fetchObject();
    return ($obj) ? $obj : NULL;
  }

  /**
   * The next value of the sequence.
   *
   * @return integer
   */
  public function getNextval() {
    $sql = "select nextval('chado.phenotype_call_phenotype_call_id_seq'::regclass)";
    return db_query($sql)->fetchField();
  }

  /**
   * @see MCL_CUSTOM_TABLE::addData($details)
   */
  public function addData(MCL_TEMPLATE $mcl_tmpl = NULL, $details) {

    // Checks for duplication.
    $keys = array(
      'project_id'        => $details['project_id'],
      'nd_geolocation_id' => $details['nd_geolocation_id'],
      'stock_id'          => $details['stock_id'],
      'cvterm_id'         => $details['cvterm_id'],
    );
    $data = $this->getDataByKey($keys);
    if ($data) {
      $mcl_tmpl->addMsg('D', 'phenotype_call', $keys);
    }
    else {
      $transaction = db_transaction();
      try {

        // Gets the next value.
        $phenotype_call_id = $this->getNextval();

        // Adds the data.
        $fields = array(
          'phenotype_call_id' => $phenotype_call_id,
          'project_id'        => $details['project_id'],
          'nd_geolocation_id' => $details['nd_geolocation_id'],
          'stock_id'          => $details['stock_id'],
          'cvterm_id'         => $details['cvterm_id'],
          'phenotype_id'      => $details['phenotype_id'],
          'time'              => $details['time'],
        );

        // Adds contact_id (optioanl).
        if ($details['contact_id']) {
          $fields['contact_id'] = $details['contact_id'];
        }

        // Inserts the record.
        db_insert($this->table_name)
          ->fields($fields)
          ->execute();
        return $this->getDataByKey(array('phenotype_call_id' => $phenotype_call_id));
      }
      catch (Exception $e) {
        $transaction->rollback();
        watchdog('mod', $e->getMessage(), array(), WATCHDOG_ERROR);
        return NULL;
      }
    }
    return $data;
  }

  /**
   * Returns the sample.
   *
   * @param array $keys
   *
   * @return MCL_CHADO_STOCK
   */
  public function getSample($keys) {

    // Gets the sample.
    $args = array(
      ':project_id'       => $keys['project_id'],
      ':unique_id'        => $keys['unique_id'],
      ':primary_order'    => $keys['primary_order'],
      ':secondary_order'  => $keys['secondary_order'],
    );
    $table_name = $this->getTableName();
    $sql = "
      SELECT T.stock_id
      FROM {$table_name} T
      WHERE T.project_id = :project_id AND T.unique_id = :unique_id
        AND T.primary_order = :primary_order
        AND T.secondary_order = :secondary_order
    ";
    $stock_id = db_query($sql, $args)->fetchField();
    if ($stock_id) {
      return MCL_CHADO_STOCK::byID($stock_id);
    }
    return NULL;
  }

  /**
   * Adds contacts.
   *
   * @param MCL_TEMPLATE $mcl_tmpl
   * @param string $contact
   * @param string $separator
   */
  public function addContact(MCL_TEMPLATE $mcl_tmpl = NULL, $contact, $separator = '') {

    // Gets the contact names.
    $contact_names = preg_split(self::getSepRegex($separator), $contact, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($contact_names as $name) {
      $name = trim($name);
    }
  }
}