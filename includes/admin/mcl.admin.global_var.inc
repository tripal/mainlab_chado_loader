<?php
/**
 * Manages MCL global variables.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Site variables.
  $form['site_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => 'Site varibles',
  );
  $form['site_var']['table'] = array(
    '#markup' => _mcl_get_mcl_global_vars_table('SITE'),
  );

  // Default variables.
  $form['default_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => 'Default varibles',
  );

  // Adds default varibles.
  $form['default_var']['table'] = array(
    '#markup' => _mcl_get_mcl_global_vars_table('DEFAULT'),
  );

  // Adds a new variables.
  $form['add_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Add a new varibles',
  );
  $form['add_var']['type'] = array(
    '#type'         => 'select',
    '#title'        => 'Name',
    '#options'      => MCL_VAR::getTypes(),
    '#description'  => 'Select a type of a variable.',
    '#attributes'   => array('style' => 'width: 250px;'),
  );
  $form['add_var']['name'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Name',
    '#description'  => 'Type name of a variable.',
    '#attributes'   => array('style' => 'width: 250px;'),
  );
  $form['add_var']['value'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Value',
    '#description'  => 'Type value of a variable.',
    '#attributes'   => array('style' => 'width: 250px;'),
  );
  $form['add_var']['description'] = array(
    '#type'         => 'textarea',
    '#title'        => 'Description',
    '#rows'         => 4,
    '#cols'         => 50,
    '#description'  => 'Type description of a variable.',
    '#attributes'   => array('style' => ''),
  );
  $form['add_var']['add_var_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'add_var_btn',
    '#value'  => 'Add a variable',
  );

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-global-var-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns all variables of the provided type in a table.
 *
 * @param string $type
 *
 * @return string
 */
function _mcl_get_mcl_global_vars_table($type) {

  // Gets the site varibles.
  $mcl_vars = MCL_VAR::getGlobalVars($type);
  $rows = array();
  foreach ($mcl_vars as $mcl_var) {
    $row = array(
      $mcl_var->getName(),
      $mcl_var->getValue(),
      $mcl_var->getDescription(),
      l('Edit', '/admin/mcl/global_var/edit/' . $mcl_var->getVarID()),
    );
    $rows[] = $row;
  }
  $table_vars = array(
    'header'      => array('Name', 'Value', 'Description', 'Action'),
    'rows'        => $rows,
    'attributes'  => array(),
  );
  return theme('table', $table_vars);
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_global_var_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_form_submit($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "add_var_btn" button is clicked.
  if ($trigger_elem == "add_var_btn") {

    // Gets the variable info.
    $type   = trim($form_state['values']['add_var']['type']);
    $name   = trim($form_state['values']['add_var']['name']);
    $value  = trim($form_state['values']['add_var']['value']);
    $desc   = trim($form_state['values']['add_var']['description']);

    // Checks for duplication.
    $mcl_var = MCL_VAR::byKey(array('name' => $name, 'type' => $type));
    if ($mcl_var) {
      drupal_set_message("Name : $name has already existed. Please pick other name.");
    }
    else {

      // Adds a new variable.
      $details = array(
        'type'        => $type,
        'name'        => $name,
        'value'       => $value,
        'description' => $description,
      );
      $mcl_var = new MCL_VAR($details);
      if ($mcl_var->insert()) {
        drupal_set_message("A new variable has been added.");
      }
      else {
        drupal_set_message("Error : Failed to add a new variable.");
      }
    }
  }
}

/**
 * Edits MCL global variables.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_edit_form($form, &$form_state, $var_id) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Global variables.
  $form['global_var'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => 'Edit MCL global varible',
  );

  // Gets MCL_VAR.
  $mcl_var = MCL_VAR::byKey(array('var_id' => $var_id));
  if ($mcl_var) {
    $form['global_var']['mcl_var'] = array(
      '#value' => $mcl_var,
    );
    $form['global_var']['type'] = array(
      '#title'          => 'Type',
      '#type'           => 'select',
      '#options'        => MCL_VAR::getTypes(),
      '#description'    => "The type of the variable.",
      '#default_value'  => $mcl_var->getType(),
    );

    // If name start with 'SITE_', make it readonly.
    $name = $mcl_var->getName();
    if (preg_match("/^SITE\_/", $name)) {
      $form['global_var']['name'] = array(
        '#title'        => 'Name',
        '#type'         => 'textfield',
        '#description'  => "The name of the variable.",
        '#value'        => $name,
        '#description'  => 'The name of this variable cannot be changed.',
        '#attributes'   => array(
          'style' => 'background-color:lightgray;',
          'readonly' => 'readonly'
        ),
      );
    }
    else {
      $form['global_var']['name'] = array(
        '#title'          => 'Name',
        '#type'           => 'textfield',
        '#description'    => "The name of the variable.",
        '#required'       => TRUE,
        '#default_value'  => $name,
      );
    }
    $form['global_var']['value'] = array(
      '#title'          => 'Value',
      '#type'           => 'textfield',
      '#description'    => "The value of the variable.",
      '#required'       => TRUE,
      '#default_value'  => $mcl_var->getValue(),
    );
    $form['global_var']['description'] = array(
      '#title'          => 'Description',
      '#type'           => 'textarea',
      '#description'    => "The description of the variable.",
      '#default_value'  => $mcl_var->getDescription(),
    );
    $form['global_var']['update_var_btn'] = array(
      '#type'   => 'submit',
      '#name'   => 'update_var_btn',
      '#value'  => 'Update the variable',
    );
  }
  else {
    $form['global_var']['error'] = array(
      '#markup' => "Could not find MCL_VAR for var ID = $var_id",
    );
  }

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-global-var-edit-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_global_var_edit_form_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_edit_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_global_var_edit_form_submit($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "Update" button is clicked.
  if ($trigger_elem == "update_var_btn") {

    // Gets the variable info.
    $mcl_var  = $form['global_var']['mcl_var']['#value'];
    $type     = trim($form_state['values']['global_var']['type']);
    $name     = trim($form_state['values']['global_var']['name']);
    $value    = trim($form_state['values']['global_var']['value']);
    $desc     = trim($form_state['values']['global_var']['description']);

    // Checks for duplication.
    $tmp_mcl_var = MCL_VAR::byKey(array('name' => $name, 'type' => $type));
    if ($tmp_mcl_var && ($mcl_var->getVarID() != $tmp_mcl_var->getVarID())) {
      drupal_set_message("Name : $name has already existed..");
    }
    else {

      // Updates the variable.
      if ($mcl_var) {
        $mcl_var->setType($type);
        $mcl_var->setName($name);
        $mcl_var->setValue($value);
        $mcl_var->setDescription($desc);
        if (!$mcl_var->update()) {
          drupal_set_message("Failed to update the varibale.");
        }
        else {
          drupal_set_message("The variable has been updated.");
          drupal_goto("admin/mcl/global_var");
        }
      }
    }
  }
}
