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

  // Sets the maximum rows.
  $max_rows = 50;

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
  if ($type == 'cvterm') {
    $form['data_query']['cvterm_names'] = array(
      '#type'       => 'textarea',
      '#title'      => 'cvterm names',
      '#rows'       => 3,
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['cvterm_ids'] = array(
      '#type'       => 'textarea',
      '#title'      => 'cvterm IDs',
      '#rows'       => 3,
      '#attributes' => array('style' => 'width:400px;'),
    );
  }
  else if ($type == 'feature') {
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
  }
  else if ($type == 'stock') {
    $form['data_query']['uniquename'] = array(
      '#type'       => 'textfield',
      '#title'      => 'uniquename',
      '#attributes' => array('style' => 'width:400px;'),
    );
    $form['data_query']['stock_id'] = array(
      '#type'       => 'textfield',
      '#title'      => 'stock_id',
      '#attributes' => array('style' => 'width:400px;'),
    );
  }
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

    // Cvterm results.
    if ($query_type == 'cvterm') {
      $cvterm_names = trim($form_state['values']['data_query']['cvterm_names']);
      $cvterm_ids   = trim($form_state['values']['data_query']['cvterm_ids']);
      _mcl_get_result_cvterm($form, $cvterm_names, $cvterm_ids);
    }

    // Feature results.
    else if ($query_type == 'feature') {

      // Gets cvterm ID of 'genetic_marker'.
      $cvterm = MCL_CHADO_CVTERM::getCvterm('sequence', 'genetic_marker');
      $type_id = $cvterm->getCvtermID();

      // Gets uniquename.
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      if (!$uniquename) {
        $feature_id = trim($form_state['values']['data_query']['feature_id']);
        $feature    = CHADO_FEATURE::byKey(array('feature_id' => $feature_id));
        $uniquename = $feature->getUniquename();
      }
      _mcl_get_result_feature($form, $max_rows, $uniquename, $type_id);
    }

    // Stock results.
    else if ($query_type == 'stock') {

      // Gets uniquename.
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      if (!$uniquename) {
        $stock_id   = trim($form_state['values']['data_query']['stock_id']);
        $stock      = MCL_CHADO_STOCK::byKey(array('stock_id' => $stock_id));
        $uniquename = $stock->getUniquename();
      }
      _mcl_get_result_stock($form, $max_rows, $uniquename);
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
 * @param string $cvterm_names
 * @param string $cvterm_ids
 */
function _mcl_get_result_cvterm(&$form, $cvterm_names, $cvterm_ids) {

  // Gets cvterms by name.
  $cvterms = array();
  if ($cvterm_names) {
    $tmp = preg_split("/\n/", $cvterm_names, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($tmp as $cvterm_name) {
      $sql = "
        SELECT C.cvterm_id, C.name AS cvterm_name, CV.cv_id, CV.name AS cv_name
        FROM chado.cvterm C
          INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
        WHERE LOWER(C.name) = LOWER(:name)
      ";
      $results = db_query($sql, array(':name' => $cvterm_name));
      while ($obj = $results->fetchObject()) {
        $key = $obj->cvterm_name. $obj->cvterm_id;
        $cvterms[] = array($obj->cvterm_id, $obj->cvterm_name, $obj->cv_id, $obj->cv_name);
      }
    }
  }

  // Gets cvterms by ID.
  if ($cvterm_ids) {
    $tmp = preg_split("/\n/", $cvterm_ids, NULL, PREG_SPLIT_NO_EMPTY);
    foreach ($tmp as $cvterm_id) {
      $sql = "
        SELECT C.cvterm_id, C.name AS cvterm_name, CV.cv_id, CV.name AS cv_name
        FROM chado.cvterm C
          INNER JOIN chado.cv CV on CV.cv_id = C.cv_id
        WHERE C.cvterm_id = :cvterm_id
      ";
      $obj = db_query($sql, array(':cvterm_id' => $cvterm_id))->fetchObject();
      if ($obj) {
        $key = $obj->cvterm_name. $obj->cvterm_id;
        $cvterms[] = array($obj->cvterm_id, $obj->cvterm_name, $obj->cv_id, $obj->cv_name);
      }
    }
  }

  // Sorts the cvterms.
  ksort($cvterms);

  // Lists cvterms.
  $rows = array();
  foreach ((array)$cvterms as $key => $row) {
    $rows []= $row;
  }

  // Creates the cvterm table.
  $form['data_query']['results']['cvterm'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'cvterms',
  );
  $table_vars = array(
    'header'      => array('cvterm ID', 'cvterm name', 'cv ID', 'cv name'),
    'rows'        => $rows,
    'empty'       => 'No matched cvterm found',
    'attributes'  => array(),
  );
  $form['data_query']['results']['cvterm']['table'] = array(
    '#markup' => theme('table', $table_vars),
  );
}

/**
 * Returns the results.
 *
 * @param arary $form
 * @param string $uniquename
 * @param integer $type_id
 */
function _mcl_get_result_feature(&$form, $max_rows, $uniquename, $type_id) {

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
    WHERE F.type_id = :type_id AND LOWER(F.uniquename) = LOWER(:uniquename)
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
      $count = 1;
      foreach ($rel_tables['feature_id'] as $table_info) {
        $fk_table   = $table_info['table'];
        $fk_attr    = $table_info['attr'];
        $fieldset = $fk_table . $count++;

        // Checks if feature_id exists in the related table.
        $sql = "SELECT COUNT($fk_attr) FROM chado.$fk_table WHERE $fk_attr = :feature_id";
        $num = db_query($sql, array(':feature_id' => $feature_id))->fetchField();
        if ($num) {
          $no_rel_table = FALSE;
          $form['data_query']['results']['feature'][$feature_id][$fieldset] = array(
            '#type'         => 'fieldset',
            '#collapsed'    => TRUE,
            '#collapsible'  => TRUE,
            '#title'        => "$fk_table ($num)",
            '#description'  => "<div style='margin:10px;'><b>Foreign Key : $fk_attr</b></div>",
          );

          // Default message.
          $markup = "$fk_table has more than the maximum rows ($max_rows) to be displayed.";

          // Creates a data table only if the number of the rows is less than 50.
          if ($num < $max_rows) {

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
            $markup = $tables;
          }
          $form['data_query']['results']['feature'][$feature_id][$fieldset]['tables'] = array(
            '#markup' => $markup,
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
 * @param arary $form
 * @param string $uniquename
 */
function _mcl_get_result_stock(&$form, $max_rows, $uniquename) {

  // Adds a fieldset for the stock.
  $form['data_query']['results']['stock'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => "Stock : $uniquename",
  );

  // Gets all stocks with the same uniquename.
  $sql = "
    SELECT S.stock_id, S.uniquename, O.genus, O.species
    FROM chado.stock S
      INNER JOIN chado.organism O on O.organism_id = S.organism_id
    WHERE LOWER(S.uniquename) = LOWER(:uniquename)
  ";
  $stocks = array();
  $results = db_query($sql, array(':uniquename' => $uniquename));
  while ($obj = $results->fetchObject()) {
    $stocks []= $obj;
  }

  if (empty($stocks)) {
    $form['data_query']['results']['stock']['table'] = array(
      '#markup' => "<em>'<b>$uniquename</b>' not found in stock</em>",
    );
  }
  else {
    $form['data_query']['results']['stock']['table'] = array(
      '#markup' => sizeof($stocks) . " stocks have the same uniquename.<br /><br />",
    );

    // Sets the search_path to chado, public for create_point function.
    $sql = "SET SEARCH_PATH TO chado, public;";

    // Searches for related tables for each stock.
    foreach ($stocks as $stock) {
      $stock_id = $stock->stock_id;
      $genus    = $stock->genus;
      $species  = $stock->species;

      // Adds a fieldset for the stock.
      $form['data_query']['results']['stock'][$stock_id] = array(
        '#type'         => 'fieldset',
        '#collapsed'    => TRUE,
        '#collapsible'  => TRUE,
        '#title'        => "Stock ID : $stock_id ($genus $species)",
      );

      // Checks stock for the related tables.
      $rel_tables = MCL_CHADO_STOCK::getRelTable();
      $no_rel_table = TRUE;
      $count = 1;
      foreach ($rel_tables['stock_id'] as $table_info) {
        $fk_table = $table_info['table'];
        $fk_attr  = $table_info['attr'];
        $fieldset = $fk_table . $count++;

        // Checks if stock_id exists in the related table.
        $sql = "SELECT COUNT($fk_attr) FROM chado.$fk_table WHERE $fk_attr = :stock_id";
        $num = db_query($sql, array(':stock_id' => $stock_id))->fetchField();

        // Creates fieldset.
        if ($num) {
          $no_rel_table = FALSE;
          $form['data_query']['results']['stock'][$stock_id][$fieldset] = array(
            '#type'         => 'fieldset',
            '#collapsed'    => TRUE,
            '#collapsible'  => TRUE,
            '#title'        => "$fk_table ($num)",
            '#description'  => "<div style='margin:10px;'><b>Foreign Key : $fk_attr</b></div>",
          );

          // Default message.
          $markup = "$fk_table has more than the maximum rows ($max_rows) to be displayed.";

          // Creates a data table only if the number of the rows is less than 50.
          if ($num < $max_rows) {
            $sql = "SELECT * FROM chado.$fk_table WHERE $fk_attr = :stock_id";
            $results = db_query($sql, array(':stock_id' => $stock_id));
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
            $markup = $tables;
          }
          $form['data_query']['results']['stock'][$stock_id][$fieldset]['tables'] = array(
            '#markup' => $markup,
          );
        }
      }
      if ($no_rel_table) {
        $form['data_query']['results']['stock'][$stock_id]['tables'] = array(
          '#markup' => 'Not found in related tables.',
        );
      }
    }
  }
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
    $type = $form_state['values']['query_type'];

    // Cvterm.
    if ($type == 'cvterm') {
      $cvterm_names = trim($form_state['values']['data_query']['cvterm_names']);
      $cvterm_ids   = trim($form_state['values']['data_query']['cvterm_ids']);
      if (!$cvterm_names && !$cvterm_ids) {
        form_set_error('data_query][cvterm_names', t("Please type cvterm names or IDs"));
        return;
      }
    }

    // Feature.
    else if ($type == 'feature') {
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      $feature_id = trim($form_state['values']['data_query']['feature_id']);
      if (($uniquename && $feature_id) || (!$uniquename && !$feature_id)) {
        form_set_error('data_query][uniquename', t("Please type uniquename or feature ID"));
        return;
      }
    }

    // Stock.
    else if ($type == 'stock') {
      $uniquename = trim($form_state['values']['data_query']['uniquename']);
      $stock_id   = trim($form_state['values']['data_query']['stock_id']);
      if (($uniquename && $stock_id) || (!$uniquename && !$stock_id)) {
        form_set_error('data_query][uniquename', t("Please type uniquename or stock ID"));
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
