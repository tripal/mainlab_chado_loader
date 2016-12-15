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

  // Adds the instruction for managing data templates in MCL.
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

  // Updates templates.
  $form['template_list']['update_template_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'update_template_btn',
    '#value'  => 'Update Templates',
  );

  // Creates checkboxes for the MCL templates.
  _mcl_create_template_checkbox($form);

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-template-form">';
  $form['#suffix'] = '</div>';
  $form['#submit'][] = 'mcl_admin_template_form_submit';
  $form['#theme'] = 'mcl_admin_template_form';
  return $form;
}

/**
 * Creates checkboxes for the MCL templates.
 *
 * @param array $form
 */
function _mcl_create_template_checkbox(&$form) {

  // Gest all MCL templates.
  $mcl_templates = MCL_TEMPLATE::getTemplates();
  foreach ($mcl_templates as $mcl_template) {
    $form['templates'][$mcl_template->getTemplateID()] = array(
      '#title'        => '',
      '#type'         => 'checkbox',
      '#default_value' => $mcl_template->getPublic(),
      '#attributes'   => array('style' => 'margin-left:10px;'),
    );
  }
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_template_form_ajax_callback($form, &$form_state) {
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

  // If "Update" button is clicked.
  if ($trigger_elem == "update_template_btn") {


    // Updates the visibility (public or private) of templates.
    foreach ($form['templates'] as $template_id => $info) {
      if (!preg_match("/^(\d+)$/", $template_id)) {
        continue;
      }
      $template = MCL_TEMPLATE::byKey(array('template_id' => $template_id));
      $public = $form_state['values']['templates'][$template_id];
      $template->setPublic($public);
      $template->update();
    }

    // Scans the template directory and add new templates or remove non-existance
    // ones from the database.
    MCL_TEMPLATE::updateTemplates(TRUE);
    drupal_set_message('The templates has been updated');
  }
}

/**
 * Theme function for the form.
 */
function theme_mcl_admin_template_form($variables) {
  $form = $variables['form'];

  $layout = "<div style='width:100%;'>" . drupal_render($form['manage_template']) . '</div>';

  // Adds "Update" button.
  $layout.= "<div style='width:100%;'>" . drupal_render($form['template_list']) . '</div>';

  // Gets the template directory.
  $template_path = drupal_get_path('module', 'mcl') . '/includes/class/template/module';

  // Gets all templates and populates the table with the templates.
  $rows = array();
  foreach ($form['templates'] as $template_id => $info) {
    if (!preg_match("/^(\d+)$/", $template_id)) {
      continue;
    }
    $template = MCL_TEMPLATE::byKey(array('template_id' => $template_id));

    // Checks the existance of the class file.
    $class_file = 'mcl_template_' . strtolower($template->getTemplate()) . '.class.inc';
    $class_exists = (file_exists("$template_path/$class_file")) ? 'exists' : 'not exist';

    // Gets the definition.
    $description = '<em>N/A</em>';
    if ($class_exists == 'exists') {
      $template = MCL_TEMPLATE::getTemplateByID($template_id);
      $description = $template->defineDescription();
    }

     $row = array(
      drupal_render($form['templates'][$template_id]),
      $template->getTemplateType(FALSE),
      $template->getTemplate(),
      $class_exists,
      $description,
    );
    $rows []= $row;
  }

   // Sets the headers.
   $headers = array(
   array('data' => 'Public', 'style' => 'width:10px'),
   array('data' => 'Template Type', 'style' => 'width:15%'),
   array('data' => 'Template', 'style' => 'width:10%'),
   array('data' => 'Class File', 'style' => 'width:8%'),
   'Description',
   );

   $table_vars = array(
     'header'      => $headers,
     'rows'        => $rows,
     'attributes'  => array(),
   );

   // Adds the template table.
   $layout .= theme('table', $table_vars);
   $layout .= drupal_render_children($form);
   return $layout;
}