<?php
/**
 * The declaration of MCL_TEMPLATE_MAP_POSITION class.
 *
 */
class MCL_TEMPLATE_MAP_POSITION extends MCL_TEMPLATE {

 /**
  *  Class data members.
  */
  /**
   * @see MCL_TEMPLATE::__construct()
   */
  public function __construct($details = array()) {
    $details['template_type'] = 'MAP_POSITION';
    parent::__construct($details);
  }

  /**
   * @see MCL_TEMPLATE::byKey()
   */
  public static function byKey($keys) {
    $parent = parent::byKey($keys);
    if ($parent) {
      return new self($parent->getMemberArr());
    }
  }

  /**
   * @see MCL_TEMPLATE::defineDescription()
   */
  public function defineDescription() {
    $desc = 'The description for map_position sheet';
    return $desc;
  }

  /**
   * @see MCL_TEMPLATE::defineHeaders()
   */
  public function defineHeaders() {
    $headers = array(
      'marker_qtl_mtl'      => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the marker, QTL or MTL. Please provide the detailr data in 'Marker', 'QTL', or 'MTL' sheet."),
      'locus_name'          => array('req' => FALSE, 'width' => 10, 'desc' => "When a RFLP (eg. AA07) corresponds to more than one locus, the marker name (eg. AA07) is recorded in marker/qtl/mtl column and the specific name for each map position (eg. AA07A, AA07B) is recorded in this 'locus_name' column. Use AA07 as a marker name and the loci names as aliases for the marker name in 'Marker' Sheet. When a MTL is mapped to multiple locations, use the MTL_name in 'MTL' sheet in 'marker/qtl/mtl' column and the locus name of the specific map position in 'locus_name'' colum."),
      'map_name'            => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the genetic map."),
      'linkage_group'       => array('req' => TRUE,  'width' => 10, 'desc' => "Name of the linkage group."),
      'linkage_group_order' => array('req' => FALSE, 'width' => 10, 'desc' => "The order of linkage groups for display. Some maps have linkage groups orders of which are not obvious such as G1V, G1G, G2V, etc."),
      'chr_name'            => array('req' => FALSE, 'width' => 10, 'desc' => "Chromosome name for the linkage group when it\'s known (eg. At1, Dt1, etc in cotton)."),
      'chr_number'          => array('req' => FALSE, 'width' => 10, 'desc' => "Chromosome number for the linkage group when it\'s known (eg. chr1, chr26, etc)."),
      'chromosome_arm'      => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the chromosome_arm for fish map."),
      'bin_name'            => array('req' => FALSE, 'width' => 10, 'desc' => "Name of the bin if the locus is belongs to a bin. If a molecular marker belongs to multiple bins, list all of them in separate rows."),
      'start'               => array('req' => FALSE, 'width' => 10, 'desc' => "The position of the locus (cM) or start position of the QTL."),
      'stop'                => array('req' => FALSE, 'width' => 10, 'desc' => "The end position of the QTL or marker when a marker corresponds to a region in a genome."),
      'qtl_peak'            => array('req' => FALSE, 'width' => 10, 'desc' => "Center position or the most likely position of the QTL."),
      'comments'            => array('req' => FALSE, 'width' => 10, 'desc' => "Any comments for the map position."),
    );
    return $headers;
  }

  /**
   * @see MCL_TEMPLATE::defineCvterms()
   */
  public function defineCvterms() {
    $cvterms = array();
    $cvterms['SITE_CV']['bin']                            = -1;
    $cvterms['SITE_CV']['start']                          = -1;
    $cvterms['SITE_CV']['stop']                           = -1;
    $cvterms['SITE_CV']['qtl_peak']                       = -1;
    $cvterms['SITE_CV']['comments']                       = -1;
    $cvterms['SITE_CV']['chr_name']                       = -1;
    $cvterms['SITE_CV']['chr_number']                     = -1;
    $cvterms['sequence']['marker_locus']                  = -1;
    $cvterms['sequence']['genetic_marker']                = -1;
    $cvterms['sequence']['qtl']                           = -1;
    $cvterms['sequence']['heritable_phenotypic_marker']   = -1;
    $cvterms['sequence']['linkage_group']                 = -1;
    $cvterms['sequence']['chromosome_arm']                = -1;
    $cvterms['relationship']['contained_in']              = -1;
    $cvterms['relationship']['instance_of']               = -1;
    return $cvterms;
  }

  /**
   * @see MCL_TEMPLATE::runErrorCheckDataLine()
   */
  public function runErrorCheckDataLine($line) {

    // Checks map.
    MCL_CHADO_FEATUREMAP::checkMap($this, $line['map_name']);

    // Checks marker_qtl_mtl. It must be generic marker, MTL or QTL.
    $marker_qtl_mtl = $line['marker_qtl_mtl'];
    $args = array(
      'uniquename'  => $marker_qtl_mtl,
      'type_id'     => $this->cvterms['sequence']['genetic_marker'],
    );
    $feature = MCL_CHADO_FEATURE::byKey($args);
    if (!$feature) {

      $args['type_id'] = $this->cvterms['sequence']['qtl'];
      $feature = MCL_CHADO_FEATURE::byKey($args);
      if (!$feature) {

        $args['type_id'] = $this->cvterms['sequence']['heritable_phenotypic_marker'];
        $feature = MCL_CHADO_FEATURE::byKey($args);
        if (!$feature) {
          $this->updateMsg('E', "Error - $marker_qtl_mtl is not 'Marker, 'QTL' or 'MTL'");
        }
      }
    }
  }

  /**
   * @see MCL_TEMPLATE::uploadDataLine()
   */
  public function uploadDataLine($line) {

    // Gets the feature. It must be marker, QTL or MTL.
    $marker_qtl_mtl = $line['marker_qtl_mtl'];
    $args = array(
      'uniquename'  => $marker_qtl_mtl,
      'type_id'     => $this->cvterms['sequence']['genetic_marker'],
    );
    $feature = MCL_CHADO_FEATURE::byKey($args);
    if (!$feature) {
      $args['type_id'] = $this->cvterms['sequence']['qtl'];
      $feature = MCL_CHADO_FEATURE::byKey($args);
      if (!$feature) {
        $args['type_id'] = $this->cvterms['sequence']['heritable_phenotypic_marker'];
        $feature = MCL_CHADO_FEATURE::byKey($args);
      }
    }
    $organism_id = $feature->getOrganismID();

    // Adds a locus if this is a genetic marker. If locus name is empty,
    // use $marker_qtl_mtl.
    if ($feature->getTypeID() == $this->cvterms['sequence']['genetic_marker']) {
      $locus_name = ($line['locus_name']) ? $line['locus_name'] : $marker_qtl_mtl;
      $locus = MCL_CHADO_FEATURE::addFeature($this, $locus_name, $locus_name, $this->cvterms['sequence']['marker_locus'], $organism_id);
      if ($locus) {

        // Adds a relationship.
        $locus->addRelatedFeature($this, $feature, $this->cvterms['relationship']['instance_of']);
      }
    }

    // Gets the map.
    $map = MCL_CHADO_FEATUREMAP::byKey(array('name' => $line['map_name']));

    // Adds a linkage group.
    $linkage_group = $link['linkage_group'];

    // Gets organism ID for a linkage group.
    $organism_id_lg = MCL_VAR::getValueByName('ORGANISM_ID');

    // Adds a linkage group.
    $uniquename = $map->getName() . '.' . $linkage_group;
    $linkage_group = MCL_CHADO_FEATURE::addFeature($this, $uniquename, $linkage_group, $this->cvterms['sequence']['linkage_group'], $organism_id_lg);

    // Adds properties.
    $linkage_group->addProp($this, 'SITE_CV', 'chr_name', $line['chr_name']);
    $linkage_group->addProp($this, 'SITE_CV', 'chr_number', $line['chr_number']);

    // Adds a chromosome arm
    $chromosome_arm_name = $line['chromosome_arm'];
    if ($chromosome_arm_name) {
      $chromosome_arm = MCL_CHADO_FEATURE::addFeature($this, $chromosome_arm_name, $chromosome_arm_name, $this->cvterms['sequence']['chromosome_arm'], $organism_id);
      if ($chromosome_arm) {

        // Adds a relationship.
        $feature->addRelatedFeature($this, $chromosome_arm, $this->cvterms['relationship']['contained_in']);
      }
    }

    // Adds a bin.
    $bin_name = $line['bin_name'];
    if ($bin_name) {
      $uniquename = $map->getName() . '.' . $bin_name;
      $bin = MCL_CHADO_FEATURE::addFeature($this, $uniquename, '', $chromosome_arm_name, $this->cvterms['SITE_CV']['bin'], $organism_id);
      if ($bin) {

        // Adds a relationship.
        if ($locus) {
          $locus->addRelatedFeature($this, $bin, $this->cvterms['relationship']['contained_in']);
        }
        else {
          $feature->addRelatedFeature($this, $bin, $this->cvterms['relationship']['contained_in']);
        }
      }
    }

    // Adds a featurepos if the linkage_group and any of start, stop or qtl_peak exist.
    $start    = $line['start'];
    $stop     = $line['stop'];
    $qtl_peak = $line['qtl_peak'];
    $data     = trim($start.$stop.$qtl_peak);
    if ($data && $linkage_group) {

      // Adds a featurepos.
      $featurepos = MCL_CHADO_FEATUREPOS::addFeaturepos($this, $feature, $map, $linkage_group);
      if ($featurepos) {

        // Adds properties.
        $featurepos->addProp($this, 'SITE_CV', 'start', $line['start']);
        $featurepos->addProp($this, 'SITE_CV', 'stop', $line['stop']);
        $featurepos->addProp($this, 'SITE_CV', 'qtl_peak', $line['qtl_peak']);
        $featurepos->addProp($this, 'SITE_CV', 'comments', $line['comments']);


        // Adds a featurepos if the linkage_group, the bin and either start or stop exist.
        $data = trim($start.$stop);
        if ($data && $linkage_group && $bin) {

          // Adds a featurepos.
          $featurepos = MCL_CHADO_FEATUREPOS::addFeaturepos($this, $bin, $map, $linkage_group);
          if ($featurepos) {

            // Adds properties.
            $featurepos->addProp($this, 'SITE_CV', 'start', $line['start']);
            $featurepos->addProp($this, 'SITE_CV', 'stop', $line['stop']);
          }
        }
      }
    }
  }
}