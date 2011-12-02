<?php
/*
 * Fields Hook
 */
function qw_all_fields(){
  $fields = apply_filters('qw_fields', array());
  return $fields;
}
// add default fields to the filter
add_filter('qw_fields', 'qw_default_fields');

/*
 * Field Styles Hook
 */
function qw_all_field_styles(){
  $styles = apply_filters('qw_field_styles', array());
  return $styles;
}
// add default field styles to the filter
add_filter('qw_field_styles', 'qw_default_field_styles');

/*
 * File Styles Hook
 */
function qw_all_file_styles(){
  $styles = apply_filters('qw_file_styles', array());
  return $styles;
}
// add default file styles to the filter
add_filter('qw_file_styles', 'qw_default_file_styles');

/*
 * Pager types
 */
function qw_all_pager_types(){
  $pagers = apply_filters('qw_pager_types', array());
  return $pagers;
}
// add default pager types
add_filter('qw_pager_types', 'qw_default_pager_types');