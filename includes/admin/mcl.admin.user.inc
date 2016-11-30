<?php
/**
 * Manages MCL user.
 *
 * @param array $form
 * @param array $form_state
 *
 * @ingroup mcl_admin
 */
function mcl_admin_user_form($form, &$form_state) {

  // Creates a form.
  $form = array();
  $form['#tree'] = TRUE;

  // Adds the instruction for managing users in MCL.
  $form['manage_user'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'MCL Users',
  );
  $form['manage_user']['desc'] = array(
    '#markup' => t('Manage data users') . '<br /><br />',
  );

  // Adds a user.
  $form['mcl_user'] = array(
    '#type'         => 'fieldset',
    '#collapsed'    => FALSE,
    '#collapsible'  => FALSE,
    '#title'        => 'Add / Remove MCL user',
  );

  // Shows MCL users.
  $form['mcl_user']['mcl'] = array(
    '#type'       => 'select',
    '#title'      => t('MCL users'),
    '#options'    => _get_opt_mcl_user_list(TRUE),
    '#validated'  => TRUE,
    '#size'       => 5,
    '#attributes' => array('style' => 'width: 200px;'),
  );
  $form['mcl_user']['remove_user_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'remove_user_btn',
    '#value'  => 'Remove user',
    '#ajax'   => array(
      'callback' => "mcl_admin_user_form_ajax_callback",
      'wrapper'  => 'mcl-admin-user-form',
      'effect'   => 'fade',
      'method'   => 'replace',
    ),
  );

  // Shows Druapl users.
  $form['mcl_user']['non_mcl'] = array(
    '#type'       => 'select',
    '#title'      => t('Non-MCL users'),
    '#options'    => _get_opt_mcl_user_list(FALSE),
    '#validated'  => TRUE,
    '#size'       => 5,
    '#attributes' => array('style' => 'width: 200px;'),
  );
  $form['mcl_user']['add_user_btn'] = array(
    '#type'   => 'submit',
    '#name'   => 'add_user_btn',
    '#value'  => 'Add user',
    '#ajax'   => array(
      'callback' => "mcl_admin_user_form_ajax_callback",
      'wrapper'  => 'mcl-admin-user-form',
      'effect'   => 'fade',
      'method'   => 'replace',
    ),
  );

  // Sets properties of the form.
  $form['#prefix'] = '<div id="mcl-admin-user-form">';
  $form['#suffix'] = '</div>';
  return $form;
}

/**
 * Returns a list of MCL users. Return all non MCL user if the flag is FALSE.
 *
 * @param boolean $flag
 *
 * @return array
 */
function _get_opt_mcl_user_list($flag = TRUE) {
  $user_list = array();
  return MCL_USER::getUserObjects($flag);
}

/**
 * Ajax function which returns the form via ajax.
 *
 * @param $form
 * @param $form_state
 */
function mcl_admin_user_form_ajax_callback($form, &$form_state) {
  global $user;

  // Gets the trigger element.
  $trigger_elem = $form_state['triggering_element']['#name'];

  // If "Remove User" button is clicked.
  if ($trigger_elem == "remove_user_btn") {


    // Gets user ID.
    $user_id = $form_state['values']['mcl_user']['mcl'];
    if ($user_id) {
      $mcl_user = MCL_USER::byKey(array('uid' => $user_id));
      if ($mcl_user->delete()) {

        // Updates user lists.
        $form['mcl_user']['non_mcl']['#options']  = _get_opt_mcl_user_list(FALSE);
        $form['mcl_user']['mcl']['#options']      = _get_opt_mcl_user_list(TRUE);
      }
      else {
        drupal_set_message("Error : Removing the MCL user [$user_id].");
      }
    }
    else {
      drupal_set_message("Please select a user from MCL user list.");
    }
  }

  // If the 'Add User' button was clicked.
  if ($trigger_elem == 'add_user_btn') {

    // Gets the Drupal user.
    $user_id = $form_state['values']['mcl_user']['non_mcl'];
    if ($user_id) {
      $drupal_user = MCL_USER::getDrupalUser($user_id);
      if ($drupal_user) {

        // Adds a new MCL user.
        $details = array(
          'user_id' => $drupal_user->uid,
          'name'    => $drupal_user->name,
          'uid'     => $drupal_user->uid,
          'mail'    => $drupal_user->mail,
        );
        $mcl_user = new MCL_USER($details);
        if ($mcl_user->insert()) {

          // Updates user lists.
          $form['mcl_user']['non_mcl']['#options']  = _get_opt_mcl_user_list(FALSE);
          $form['mcl_user']['mcl']['#options']      = _get_opt_mcl_user_list(TRUE);

          // Creates directories for a user.
          $user_dir = $mcl_user->getUserDir();
          if (!file_exists($user_dir)) {
            mcl_create_dir($user_dir);
          }
          $user_tmp_dir = "$user_dir/tmp";
          if (!file_exists($user_tmp_dir)) {
            mcl_create_dir($user_tmp_dir);
          }
        }
        else {
          drupal_set_message("Error : Failed to add a new MCL user [$user_name].");
        }
      }
      else {
        drupal_set_message("Error : Drupal user [$user_id] not found.");
      }
    }
    else {
      drupal_set_message("Please select a user from Drupal user list.");
    }
  }
  return $form;
}

/**
 * Validates the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_user_form_validate($form, &$form_state) {}

/**
 * Submits the form.
 *
 * @ingroup mcl_admin
 */
function mcl_admin_user_form_submit($form, &$form_state) {}