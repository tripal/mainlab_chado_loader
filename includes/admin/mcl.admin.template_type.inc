<?php
/**
 * Manages MCL template types.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_type_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Add the instruction for managing template types in MCL.
  $form['manage_template_type'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data Template Types',
  );
  $form['manage_template_type']['desc'] = array(
    '#markup' => t('Manage template types for Mainlab Chado Loader') . '<br /><br />',
  );

  // Form for adding a template type.
  $form['template_type_add'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Add Template Type',
  );
  $form['template_type_add']['type'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Template Type',
    '#description'  => 'Enter a type of a template.',
    '#attributes'   => array('style' => 'width: 250px;'),
  );
  $form['template_type_add']['rank'] = array(
    '#type'         => 'textfield',
    '#title'        => 'Template Type Rank',
    '#description'  => 'Enter a rank of a template type.',
    '#attributes'   => array('style' => 'width: 250px;'),
  );
  $form['template_type_add']['add_template_type_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'add_template_type_btn',
    '#value'  => 'Add a template type',
    '#ajax'   => array(
      'callback' => "mcl_admin_template_type_form_ajax_callback",
      'wrapper'  => 'mcl-admin-template-type-form',
      'effect'   => 'fade',
      'method'   => 'replace',
    ),
  );

  // Shows the list of MCL template types.
  $form['template_type_list'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'MCL Data Template types',
  );

  // Edits a new template.
  $form['template_type_list']['table']= array(
    '#markup' => _get_mcl_template_type_list(),
  );

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-template-type-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns MCL template types in table.
 *
 * @return string
 */
function _get_mcl_template_type_list() {

  // Gets all template types.
  $mcl_template_types = MCL_TEMPLATE_TYPE::getTemplateTypes('rank');
  $options = array();
  foreach ($mcl_template_types as $mcl_template_type) {
    $type = $mcl_template_type->getType();
    $rank = $mcl_template_type->getRank();

    // Adds a delete link.
    $delete_link = 'delete';

    // Adds a row.
    $row = array(
      $type,
      $rank,
      //$delete_link,
    );
    $rows[] = $row;
  }

  // Sets the headers.
  $headers = array(
      array('data' => 'Template Type', 'style' => 'width:15%'),
      array('data' => 'Rank', 'style' => 'width:10%'),
      //array('data' => 'Action', 'style' => 'width:8%'),
  );

  $table_vars = array(
    'header'      => $headers,
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
function mcl_admin_template_type_form_ajax_callback($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "Add a template type" button is clicked.
  if ($trigger_elem == "add_template_type_btn") {

    // Gets the template info.
    $type = $form_state['values']['template_type_add']['type'];
    $rank = $form_state['values']['template_type_add']['rank'];

    // Checks for duplication.
    $mcl_template_type = MCL_TEMPLATE_TYPE::byKey(array('type' => $type));
    if ($mcl_template_type) {
      drupal_set_message("'$type' has exists. Please pick a different name");
    }
    else {

      // Adds a template type.
      $details = array(
        'type'  => $type,
        'rank'  => $rank,
      );
      $mcl_template_type = new MCL_TEMPLATE_TYPE($details);
      if ($mcl_template_type->insert()) {
        drupal_set_message("New template type '$type' has been added");
      }
      else {
        drupal_set_message("Error : Failed to add a new template type '$type'");
      }

      // Clears the textfield.
      $form_state['template_type_add']['type'] = '';
    }
  }
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_type_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_type_form_submit($form, &$form_state) {}