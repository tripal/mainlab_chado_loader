<?php
/**
 * @file
*/
/**
 * MCL Template form.
*
* @param $form
* @param $form_state
*/
function mcl_template_form($form, &$form_state, $template_id = NULL) {
  global $user;

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Template.
  $form['template'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Data Template',
  );

  // Gets the information about the template.
  _get_mcl_template_table($form, $template_id);

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-template-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns a list of all data templates in a table.
 *
 * @return string
 */
function _get_mcl_template_table(&$form, $template_id) {


  // $template_id is empty.
  if (!$template_id) {
    $form['template']['table'] = array(
      '#markup' => "<div>Plese select template in <a href='/mcl'><b>this page</b></a></div>",
    );
    return;
  }

  // Gets MCL_TEMPLATE.
  $mcl_template = MCL_TEMPLATE::getTemplatebyID($template_id);

  // No template found for $template_id.
  if (!$mcl_template) {
    $form['template']['table'] = array(
      '#markup' => "<div>Could not found a template for the template ID ($template_id). Plese select a different template in <a href='/mcl'><b>this page</b></a></div>",
    );
    return;
  }

  // Gets the template information.
  $rows = array();
  $rows []= array(array('data' => '<b>Template Type</b>', 'style' => 'width:90px;'), $mcl_template->getTemplateType());
  $rows []= array('<b>Template</b>', $mcl_template->getTemplate());
  $rows []= array('&nbsp;', "<a href='/mcl/template/download/$template_id'>download</a>");
  $table_vars = array(
    'header'      => NULL,
    'rows'        => $rows,
    'attributes'  => array('style' => 'width:40%'),
  );
  $form['template']['info'] = array(
    '#markup' => theme('table', $table_vars),
  );

  // The header columns of the template.
  $form['template']['headers'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => TRUE,
    '#title'        => "Headers",
  );
  $form['template']['headers']['table'] = array(
    '#markup' => $mcl_template->getHeaderTable(),
  );

  // The cvterms of the template.
  $form['template']['cvterms'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => TRUE,
    '#collapsible'  => TRUE,
    '#title'        => "cvterms",
  );
  $form['template']['cvterms']['table'] = array(
    '#markup' => $mcl_template->getCvtermTable(),
  );

  // Table Footer.
  $form['template']['footer'] = array(
    '#markup' => "<div style=''><em>Returns to <a href='/mcl/template_list'>the template list page</a></em></div>",
  );
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_template_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_template_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_template_form_submit($form, &$form_state) {}
