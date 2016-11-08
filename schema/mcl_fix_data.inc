<?php
/**
 * @file
 * Contains functions that used in mcl-fix-data command.
 */
function mcl_fix_data_func($input_file = NULL) {

  $dir_name = dirname($input_file);
  $output_file = "$dir_name/link_marker_snp_array.sql";

  // Gets cvterms.
  $cvterm = MCL_CHADO_CVTERM::getCvterm('SITE_CV', 'SNP_chip');
  if (!$cvterm) {
    throw new Exception("SNP_chip not found in cvterm\n");
  }

  // Opens the files for reading and writing.
  $fdr = fopen($input_file, "r");
  $fdw = fopen($output_file, "w");
  $transaction = db_transaction();
  try {
    while (!feof($fdr)) {
      $line = trim(fgets($fdr));
      if ($line == '') {
        continue;
      }
      $tmp = explode("\t", $line);
      $uniquename = trim($tmp[0]);
      $chip_name  = trim($tmp[1]);
      $chip_id    = $uniquename;

      $feature = MCL_CHADO_FEATURE::byKey(array('uniquename' => $uniquename));
      if (!$feature) {
        throw new Exception("$uniquename not found in feature\n");
      }
      $library = MCL_CHADO_LIBRARY::byKey(array('uniquename' => $chip_name));
      if (!$library) {
        throw new Exception("$chip_name not found in library\n");
      }
      $details = array(
        'name' => $chip_id,
        'type_id' => $cvterm->getCvtermID(),
      );
      $synonym = new MCL_CHADO_SYNONYM($details);
      if ($synonym->insert()) {
        $synonym_id = $synonym->getSynonymID();
        $sql = "INSERT INTO feature_synonym(feature_id, synonym_id) VALUES($feature->getFeatureID(), $synonym_id);";
        fputs($fdw, "$sql\n");
        $sql = "INSERT INTO library_synonym(library_id, synonym_id) VALUES($library->getLibraryID(), $synonym_id);";
        fputs($fdw, "$sql\n");
      }
      else {
        throw new Exception("Fail to insert synonym\n");
      }
    }
    fclose($fdr);
    fclose($fdw);
  } catch (Exception $e) {
    $transaction->rollback();
    mcl_print("Error : " . $e->getMessage(), 1, 3);
    watchdog('mcl', $e->getMessage(), array(), WATCHDOG_ERROR);
    return array('flag' => FALSE);
  }
  print "\nDONE\n";
}
