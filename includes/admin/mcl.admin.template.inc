<?php
/**
 * Manages MCL templates.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Add the instruction for managing data templates in MCL.
  $form['manage_template'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data Templates',
  );
  $form['manage_template']['desc'] = array(
    '#markup' => t('Manage data templates for Mainlab Chado Loader') . '<br /><br />',
  );

  // MCL templates.
  $form['template_list'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'MCL Data Templates',
  );

  // Adds new template.
  $form['template_list']['add_template_btn'] = array(
    '#type'       => 'submit',
    '#name'       => 'add_template_btn',
    '#value'      => 'Add New Templates',
    '#attributes' => array('style' => 'margin:20px 0px 30px 0px;'),
  );
/*
  $form['template_list']['update_template_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'update_template_btn',
    '#value'  => 'Update Templates',
    '#prefix' => '<br />Update templates<br />',
  );
*/
  // Shows the list of MCL templates.
  $form['template_list']['table']= array(
    '#markup' => _get_mcl_template_list(),
  );

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-template-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns template types as options.
 *
 * @return array
 */
function _get_opt_mcl_template_type() {

  // Gets all template types.
  $mcl_template_types = MCL_TEMPLATE_TYPE::getTemplateTypes();
  $options = array();
  foreach ($mcl_template_types as $mcl_template_type) {
    $options[$mcl_template_type->getTemplateTypeID()] = $mcl_template_type->getType();
  }
  return $options;
}

/**
 * Returns MCL templates in table.
 *
 * @return string
 */
function _get_mcl_template_list() {

  // Populates the table with MCL templates.
  $rows = array();
  $mcl_templates = MCL_TEMPLATE::getTemplates();
  foreach ($mcl_templates as $mcl_template) {
    $template_type  = $mcl_template->getTemplateType();
    $template       = $mcl_template->getTemplate();
    $tmpl           = MCL_TEMPLATE::getTemplateByID($mcl_template->getTemplateID());

    // Checks if class has been defined.
    $class  = '<em>Not Defined</em>';
    $desc   = '<em>N/A</em>';
    if ($tmpl) {
      $class  = 'Defined';
      $desc   = $tmpl->defineDescription();
    }

    // Adds the delete link.
    $delete_link = '';

    // Adds a row.
    $row = array(
      $template_type,
      $template,
      $class,
//      $delete_link,
      $desc,
    );
    $rows[] = $row;
  }

  // Sets the headers.
  $headers = array(
    array('data' => 'Template Type', 'style' => 'width:15%'),
    array('data' => 'Template', 'style' => 'width:10%'),
    array('data' => 'Class', 'style' => 'width:8%'),
    //array('data' => 'Action', 'style' => 'width:8%'),
    'Description',
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
function mcl_admin_template_form_ajax_callback($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "Add Template" button is clicked.
  if ($trigger_elem == "add_template_btn") {

    // Gets the template info.
    $template_type_id = $form_state['values']['template_add']['template_type'];
    $template         = $form_state['values']['template_add']['template'];

    // Adds a template.
    if ($template) {

      // Checks for duplication.
      $mcl_template = MCL_TEMPLATE::byKey(array('template' => $template));
      if ($mcl_template) {
        drupal_set_message("'$template' has exists. Please pick a different name");
      }
      else {

        // Adds a template.
        $details = array(
          'template_type_id'  => $template_type_id,
          'template'          => $template,
        );
        $mcl_template = new MCL_TEMPLATE($details);
        if ($mcl_template->insert()) {
          drupal_set_message("New template '$template' has been added");
        }
        else {
          drupal_set_message("Error : Failed to add a new template '$template'");
        }

        // Clears the textfield.
        $form_state['template_add']['template'] = '';
      }
    }
    else {
      drupal_set_message('Please type template name');
    }
  }
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_template_form_submit($form, &$form_state) {

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "Add New Templates" button is clicked.
  if ($trigger_elem == "add_template_btn") {
    MCL_TEMPLATE::updateTemplates(TRUE);
  }

  // If "Update Templates" button is clicked.
  else if ($trigger_elem == "update_template_btn") {
    MCL_TEMPLATE::updateTemplates(FALSE);
  }
}