<?php
/**
 * @file
*/
/**
 * MCL form.
*
* @param $form
* @param $form_state
*/
function mcl_form($form, &$form_state) {
  global $user;

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // MCL.
  $form['mcl'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Mainlab Chado Loader',
  );

  // Descriptions of MCL.
  $desc_drush = '';
  if (in_array('administrator', $user->roles)) {
    $desc_drush = "<br />Click '<b><em>Drush commands</b><em> to see the list of drush commands that available for MCL.";
  }

  $desc = "
    MCL (Mainlab Chado Loader) is a module that enables a user to upload biological data to chado database schema. Users are required to transfer their biological data into various types of data template files. MCL, then, uploads these data template files into a chado schema.<br /><br />
    <div style='margin-left:20px;margin-bottom:20px;'>
      Click '<b><em>Templates</b><em> to see the list of templates that MCL currently supports.
      <br /> Click '<b><em>Upload Data</b><em> to upload your data or see status of your currently uploaing jobs.
      $desc_drush</div>
  ";
  $form['mcl']['desc'] = array(
    '#markup' => $desc,
  );

  // Lists of the page links.
  $items = array();
  $items[] = l('Templates', '/mcl/template_list');
  $items[] = l('Upload Data', '/mcl/upload_data');
  $form['mcl']['page_link'] = array(
    '#markup' => theme('item_list', array('items' => $items)),
  );

  // Admin menu.
  if (user_access('admin_mcl')) {
    $form['mcl']['mcl_admin'] = array(
      '#type'         => 'fieldset',
      '#collapsed'    => TRUE,
      '#collapsible'  => TRUE,
      '#title'        => 'Admin Menu',
    );
    $items = array();
    $items[] = l('Drush commands', '/mcl/drush');
    $items []= array(
      'data' => 'Data Queries',
      'children' => array(
        array(
          'data' => l('Feature', '/mcl/data_query/feature'),
          'style' => array('list-style-type:disc;'),
        ),
        array(
          'data' => l('Stock', '/mcl/data_query/stock'),
          'style' => array('list-style-type:disc;'),
        ),
      ),
    );
    $form['mcl']['mcl_admin']['page_link'] = array(
      '#markup' => theme('item_list', array('items' => $items)),
    );
  }

  // Sets form properties.
  $form['#prefix'] = '<div id="mcl-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_form_ajax_callback($form, $form_state) {
  return $form;
}

/**
 * Validates the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @param $form
 * @param $form_state
 */
function mcl_form_submit($form, &$form_state) {}
