<?php
/**
 * @file
*/
require_once(drupal_get_path('module', 'mcl') . "/mcl.drush.inc");

/**
 * MCL Drush form.
*
* @param $form
* @param $form_state
*/
function mcl_drush_form($form, &$form_state) {
  global $user;

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // drush.
  $form['drush'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Drush commands',
  );
  $form['drush']['note'] = array(
    '#markup' => '<div style="margin:5px 0px 5px 30px">&nbsp;</div>',
  );

  // Lists drush commands to be shown to users.
  $cmds = array(
    'uploading' => array('mcl-upload-data', 'mcl-rerun-job'),
    'job'       => array('mcl-list-jobs','mcl-job-info','mcl-delete-job'),
  );

  // Gets all drush commands for MCL.
  $info = mcl_drush_command();
  $collapsed = FALSE;
  foreach ($cmds as $type => $list) {
    foreach ($list as $cmd) {
      $form['drush'][$cmd] = array(
        '#type'         => 'fieldset',
        '#collapsed'    => $collapsed,
        '#collapsible'  => TRUE,
        '#title'        => $cmd,
      );
      if (!$collapsed) {
        $collapsed = TRUE;
      }
      $form['drush'][$cmd]['table'] = array(
        '#markup' => _get_mcl_drush_cmd_details($info[$cmd]),
      );
    }
  }

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-drush-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns the details of the drush command.
 *
 * @param array $info
 *
 * @return string
 */
function _get_mcl_drush_cmd_details($info) {

  // Populates the table with the details.
  $rows = array();
  _get_mcl_drush_cmd_details_helper($rows, $info, 'text', 'description');
  _get_mcl_drush_cmd_details_helper($rows, $info, 'array', 'arguments');
  _get_mcl_drush_cmd_details_helper($rows, $info, 'array', 'options');
  _get_mcl_drush_cmd_details_helper($rows, $info, 'array', 'examples');
  $table_vars = array(
    'header'      => array(),
    'rows'        => $rows,
    'attributes'  => array('style' => 'width:90%'),
  );
  return theme('table', $table_vars);
}

/**
 * Populates $row with the command information.
 *
 * @param array $rows
 * @param string $type
 * @param string $tag
 * @param array $info
 *
 * @return array
 */
function _get_mcl_drush_cmd_details_helper(&$rows, $info, $type, $tag) {
  if (array_key_exists($tag, $info)) {
    if ($type == 'text') {
      $rows[] = array(ucfirst($tag), $info[$tag]);
    }
    else if ($type == 'array') {
      $items = array();
      foreach ($info[$tag] as $key => $val) {
        $items[] = "$key : $val";
      }
      $rows[] = array(ucfirst($tag), implode('<br />', $items));
    }
  }
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_drush_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_drush_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_drush_form_submit($form, &$form_state) {}
