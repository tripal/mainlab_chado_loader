<?php
/**
 * @file
*/
/**
 * MCL Template List form.
*
* @param $form
* @param $form_state
*/
function mcl_template_list_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Template list.
  $form['template_list'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'MCL Data Template List',
  );

  // Template description.
  $form['template_list']['desc'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => 'Template Description',
  );
  $form['template_list']['desc']['text'] = array(
    '#markup' => MCL_TEMPLATE::getProperties(),
  );

  // Template list.
  $form['template_list']['table'] = array(
    '#markup' => _mcl_get_template_table(),
  );

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-template-list-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns a list of all data templates in a table.
 *
 * @return string
 */
function _mcl_get_template_table() {

  // Headers.
  $headers = array(
    array('data' => 'Template Type', 'style' => 'width:25%'),
    'Template', 'Details', 'Download');

  // Data templates.
  $rows = array();
  $mcl_templates = MCL_TEMPLATE::getTemplates();
  foreach ($mcl_templates as $mcl_template) {
    $template_type  = $mcl_template->getTemplateType();
    $template       = $mcl_template->getTemplate();
    $template_id    = $mcl_template->getTemplateID();
    $tmpl           = MCL_TEMPLATE::getTemplateByID($template_id);

    // Skips if the template is not public.
    if ($mcl_template->getPublic() != 1) {
      //continue;
    }

    // Checks if class has been defined.
    $link_details  = '<em>Not Defined</em>';
    $link_download = '<em>N/A</em>';
    if ($tmpl) {
      $link_details  = "<a href='/mcl/template/view/$template_id'>view</a>";
      $link_download = "<a href='/mcl/template/download/$template_id'>download</a>";
    }

    // Populates rows.
    $row = array(
      $mcl_template->getTemplateType(),
      $mcl_template->getTemplate(),
      $link_details,
      $link_download
    );
    $rows []= $row;
  }

  // Table variables.
  $table_vars = array(
    'header'      => $headers,
    'rows'        => $rows,
    'empty'       => 'No Data Template',
    'attributes'  => array('style' => 'width:100%'),
  );

  // Table Footer.
  $footer = "<div style=''><em>Returns to <a href='/mcl'>MCL home</a></em></div>";
  return theme('table', $table_vars) . $footer;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_template_list_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_template_list_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_template_list_form_submit($form, &$form_state) {}
