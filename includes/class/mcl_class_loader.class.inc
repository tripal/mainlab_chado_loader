<?php
/**
 * The declaration of MCL_CLASS_LOADER class.
 *
 */
class MCL_CLASS_LOADER {

  /**
   *  Class data members.
   *
   */
  private $module_path    = NULL;
  private $class_path     = NULL;
  private $template_path  = NULL;

  /**
   * Implements the class constructor.
   */
  public function __construct() {
    $module_path          = drupal_get_path('module', 'mcl');
    $this->module_path    = $module_path;
    $this->class_path     = $module_path . '/includes/class';
    $this->template_path  = $module_path . '/templates';
  }

  /**
   * Implements the class destructor.
   */
  public function __destruct() {}

  /**
   * Register autoloader.
   *
   * @param string $class_name
   *
   * @return boolean
   */
  public function register() {
    spl_autoload_register(array($this, 'loadClass'));
  }

  /**
   * Loads a class file.
   *
   * @param string $class_name
   *
   * @return boolean
   */
  public function loadClass($class_name) {

    // Sets the default class path.
    $class_path = $this->class_path;

    // Template classes.
    if (preg_match("/^MCL_TEMPLATE/", $class_name, $matches)) {
      $path =  '/';
      if (preg_match("/^MCL_(TEMPLATE|TEMPLATE_TYPE)$/", $class_name, $matches)) {
        $path = '/core/';
      }
      $class_filepath = $this->template_path . $path . strtolower($class_name) . '.class.inc';
      if (file_exists($class_filepath)) {
        require_once($class_filepath);
        return TRUE;
      }
      return FALSE;
    }

    // Gets the path to the class file.
    $path = '/';
    if (preg_match("/^PUBLIC_MCL_/", $class_name, $matches)) {
      $path .= 'public_table_class/';
    }
    else if (preg_match("/^CHADO_/", $class_name, $matches)) {
      $path .= 'chado_table_class/';
    }
    else if (preg_match("/^MCL_CUSTOM_/", $class_name, $matches)) {
      $path .= 'mcl_custom_class/';
    }
    else if (preg_match("/^MCL_CHADO/", $class_name, $matches)) {
      $path .= 'mcl_chado_class/';
    }
    else if (preg_match("/^MCL_EXCEL/", $class_name, $matches)) {
      $path .= 'excel/';
    }
    else if (preg_match("/^MCL_FILE/", $class_name, $matches)) {
      $path .= 'file/';
    }
    else if (preg_match("/^MCL_JOB/", $class_name, $matches)) {
      $path .= 'job/';
    }
    else if (preg_match("/^MCL_DATA_VALID/", $class_name, $matches)) {
      $path .= 'data_valid/';
    }
    else if (preg_match("/^MCL_TABLE/", $class_name, $matches)) {
      $path .= 'table/';
    }
    else if (preg_match("/^MCL_([A-Z\_]+)/", $class_name, $matches)) {
      // Does nothing. The file is in the root of class directory.
    }
    // External (module) class.
    else if (preg_match("/^BIMS_CAHDO_/", $class_name, $matches)) {
      $class_path = drupal_get_path('module', 'bims') . "/includes/class";
      $path .= 'bims_chado_class/';
    }
    else {
      return FALSE;
    }

    // Includes the class file.
    $class_filepath = $class_path . $path . strtolower($class_name) . '.class.inc';
    if (file_exists($class_filepath)) {
      require_once($class_filepath);
      return TRUE;
    }
    return FALSE;
  }

  /*
   * Defines getters and setters below.
   */
  /**
   * Returns the module path.
   */
  public function getModulePath() {
    return $this->module_path;
  }
  /**
   * Returns the class path.
   */
  public function getClassPath() {
    return $this->class_path;
  }
  /**
   * Returns the template path.
   */
  public function getTemplatePath() {
    return $this->template_path;
  }
}