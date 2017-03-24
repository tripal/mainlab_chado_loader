<?php
/**
 * @file
 */
/**
 * MCL data query form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form($form, &$form_state, $type = NULL) {
  global $user;

  // Gets MCL_USER.
  $mcl_user = MCL_USER::byKey(array('user_id' => $user->uid));

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Saves the type.
  $form['query_type'] = array(
    '#type'   => 'value',
    '#value'  => $type,
  );

  // Data query.
  $form['data_query'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data Query : ' . $type,
  );
  $form['data_query']['uniquename'] = array(
    '#type'       => 'textfield',
    '#title'      => 'uniquename',
    '#attributes' => array('style' => 'width:400px;'),
  );
  $form['data_query']['feature_id'] = array(
    '#type'       => 'textfield',
    '#title'      => 'feature_id',
    '#attributes' => array('style' => 'width:400px;'),
  );
  $form['data_query']['search_btn'] = array(
    '#type'       => 'button',
    '#value'      => 'Search',
    '#name'       => 'search_btn',
    '#suffix'     => '<br /><br />',
    '#attributes' => array('style' => 'width:100px;'),
    '#ajax'       => array(
      'callback' => "mcl_data_query_form_ajax_callback",
      'wrapper'  => 'mcl-data-query-form',
      'effect'   => 'fade',
      'method'   => 'replace',
    ),
  );

  // Adds form elements for search results.
  if (array_key_exists('values', $form_state)) {
    $query_type = $form_state['values']['query_type'];

    // Gets the results.
    if ($query_type == 'feature') {

      // Search the related tables for the features.
      $cvterm = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker');
      $type_id = $cvterm->getCvtermID();

      // Gets uniquename.
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      if (!$uniquename) {
        $feature_id = trim($form_state['values']['data_query']['feature_id']);
        $feature = CHADO_FEATURE::byKey(array('feature_id' => $feature_id));
        $uniquename = $feature->getUniquename();
      }
      _mcl_get_result_feature($form, $uniquename, $type_id);
    }
    else if ($query_type == 'stock') {
      _mcl_get_result_stock($form, $uniquename);
    }
  }

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-data-query-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Returns the results.
 *
 * @param arary $form
 * @param string $uniquename
 * @param integer $type_id
 *
 * @return string
 */
function _mcl_get_result_feature(&$form, $uniquename, $type_id) {

  // Adds a fieldset for the feature.
  $form['data_query']['results']['feature'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => "Feature : $uniquename",
  );

  // Gets all features with the same uniquename.
  $sql = "
    SELECT F.feature_id, F.uniquename, O.genus, O.species
    FROM chado.feature F
    INNER JOIN chado.organism O on O.organism_id = F.organism_id
    WHERE F.type_id = :type_id AND uniquename = :uniquename
  ";
  $features = array();
  $args = array(
    ':type_id'    => $type_id,
    ':uniquename' => $uniquename,
  );
  $results = db_query($sql, $args);
  while ($obj = $results->fetchObject()) {
    $features []= $obj;
  }

  if (empty($features)) {
    $form['data_query']['results']['feature']['table'] = array(
      '#markup' => "<em>'<b>$uniquename</b>' not found in feature</em>",
    );
  }
  else {
    $form['data_query']['results']['feature']['table'] = array(
      '#markup' => sizeof($features) . " features have the same uniquename.<br /><br />",
    );

    // Sets the search_path to chado, public for create_point function.
    $sql = "SET SEARCH_PATH TO chado, public;";
    db_query($sql);

    // Searches for related tables for each feature.
    foreach ($features as $feature) {
      $feature_id = $feature->feature_id;
      $genus      = $feature->genus;
      $species    = $feature->species;

      // Adds a fieldset for the feature.
      $form['data_query']['results']['feature'][$feature_id] = array(
        '#type'         => 'fieldset',
        '#collapsed'    => TRUE,
        '#collapsible'  => TRUE,
        '#title'        => "Feature ID : $feature_id ($genus $species)",
      );

      $sql = "SET SEARCH_PATH TO chado, public;";
      db_query($sql);

      // Checks feature_id for the related tables.
      $rel_tables = MCL_CHADO_FEATURE::getRelTable();
      $no_rel_table = TRUE;
      foreach ($rel_tables['feature_id'] as $table_info) {
        $fk_table   = $table_info['table'];
        $fk_attr    = $table_info['attr'];

        // Checks if feature_id exists in the related table.
        $sql = "SELECT COUNT($fk_attr) FROM chado.$fk_table WHERE $fk_attr = :feature_id";
        $num = db_query($sql, array(':feature_id' => $feature_id))->fetchField();
        if ($num) {
          $no_rel_table = FALSE;
          $form['data_query']['results']['feature'][$feature_id][$fk_table] = array(
            '#type'         => 'fieldset',
            '#collapsed'    => TRUE,
            '#collapsible'  => TRUE,
            '#title'        => "$fk_table ($num)",
          );
          $sql = "SELECT * FROM chado.$fk_table WHERE $fk_attr = :feature_id";
          $results = db_query($sql, array(':feature_id' => $feature_id));
          $tables = '';
          while ($arr = $results->fetch(PDO::FETCH_ASSOC)) {
            $rows = array();
            foreach ($arr as $key => $value) {
              $rows []= array($key, $value);
            }
            $table_vars = array(
              'header'      => array('attr', 'value'),
              'rows'        => $rows,
              'attributes'  => array(),
            );
            $tables .= theme('table', $table_vars);
          }
          $form['data_query']['results']['feature'][$feature_id][$fk_table]['tables'] = array(
            '#markup' => $tables,
          );
        }
      }
      if ($no_rel_table) {
        $form['data_query']['results']['feature'][$feature_id]['tables'] = array(
          '#markup' => 'Not found in related tables.',
        );
      }
    }
  }
}

/**
 * Returns the results.
 *
 * @param array $uniquenames
 *
 * @return string
 */
function _mcl_get_result_stock($uniquenames) {
  $results = '';

  foreach ($uniquenames as $uniquename) {
    $feature = MCL_CHADO_STOCK::byKey(array('uniquename' => $uniquename));
    if ($feature) {

    }
    else {
      $results .= "<b>$uniquename</b> not found in $type<br />";
    }
  }
  return $results;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form_validate($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If the 'program_list' was changed.
  if ($trigger_elem == 'search_btn') {
    $query_type = $form_state['values']['query_type'];

    // Feature.
    if ($query_type == 'feature') {
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      $feature_id = trim($form_state['values']['data_query']['feature_id']);
      if (($uniquename && $feature_id) || (!$uniquename && !$feature_id)) {
        form_set_error('data_query][uniquename', t("Please type uniquename or feature_id"));
        return;
      }
    }
  }
}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_data_query_form_submit($form, &$form_state) {}
