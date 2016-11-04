<?php
/**
 * Manages MCL config page.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_config_form($form, &$form_state) {

  // Set the breadcrumb.
  $breadcrumb = array();
  $breadcrumb[] = l('Home', '<front>');
  $breadcrumb[] = l('Administration', 'admin');
  $breadcrumb[] = l('MCL', 'admin/mcl');

  // Create the setting form.
  $form = array();

  // MCL file / directory paths.
  $form['config_path'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'MCL file / diretory paths',
  );

  // Drupal file paths.
  $file_path_public   = drupal_realpath('public://');
  $file_path_private  = drupal_realpath('private://');

  // Uploading file directory.
  $form['config_path']['mcl_file_dir'] = array(
    '#markup' => t("<br /><b>Uploading File Directory</b><br />" .
        "Uploaded files are saved in drupal public direcotry ($file_path_public)<br /><br />")
  );

  // MCL working directory.
  $form['config_path']['mcl_working_dir'] = array(
    '#title'          => t('MCL Working Directory'),
    '#type'           => t('textfield'),
    '#description'    => t("Please specify the working directory for MCL."),
    '#required'       => TRUE,
    '#default_value'  => mcl_get_config_setting('mcl_working_dir'),
  );

  // MCL Library directory.
  $form['config_path']['mcl_library_dir'] = array(
    '#title'          => t('MCL Library Directasory'),
    '#type'           => t('textfield'),
    '#description'    => t("Please specify the library directory for MCL."),
    '#required'       => TRUE,
    '#default_value'  => mcl_get_config_setting('mcl_library_dir'),
  );
  return system_settings_form($form);
}

/**
 * Admin form validate.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_setting_validate($form, &$form_state) {

  // Gets variables.
  $mcl_working_dir  = trim($form_state['values']['mcl_working_dir']);
  $mcl_library_dir  = trim($form_state['values']['mcl_library_dir']);


  // Check the working directory.
  if (!is_writable($mcl_working_dir)) {
    form_set_error('mcl_working_dir', t("The working directory, $mcl_working_dir, does not exists or is not writeable by the web server."));
    return;
  }

  // Check the library directory.
  if (!is_writable($mcl_library_dir)) {
    form_set_error('mcl_working_dir', t("The library directory, $mcl_library_dir, does not exists."));
    return;
  }

  // Updates variables.
  variable_set('mcl_working_dir', $mcl_working_dir);
  variable_set('mcl_library_dir', $mcl_library_dir);
}
