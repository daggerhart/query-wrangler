<?php
/*
 * Post Statuses
 */
function qw_all_post_statuses(){
  $post_statuses = apply_filters('qw_post_statuses', array());
  return $post_statuses;
}

/*
 * Fields Hook
 */
function qw_all_fields(){
  $fields = apply_filters('qw_fields', array());
  foreach($fields as $type => $field){
    $fields[$type]['type'] = $type;
  }
  return $fields;
}

/*
 * filters Hook
 */
function qw_all_filters(){
  $filters = apply_filters('qw_filters', array());

  foreach($filters as $type => $filter){
    // set filter's type as a value
    $filters[$type]['type'] = $type;
  }
  return $filters;
}

/*
 * Styles Hook
 */
function qw_all_styles(){
  $styles = apply_filters('qw_styles', array());
  return $styles;
}

/*
 * Row Styles Hook
 */
function qw_all_row_styles(){
  $row_styles = apply_filters('qw_row_styles', array());
  return $row_styles;
}

/*
 * Sort Options Hook
 */
function qw_all_sort_options(){
  $row_styles = apply_filters('qw_sort_options', array());
  return $row_styles;
}

/*
 * Row Style 'Posts' Styles Hook
 */
function qw_all_row_complete_styles(){
  $row_complete_styles = apply_filters('qw_row_complete_styles', array());

  return $row_complete_styles;
}

/*
 * Row Style 'Fields' Stlyes Hook
 */
function qw_all_row_fields_styles(){
  $row_fields_styles = apply_filters('qw_row_fields_styles', array());
  return $row_fields_styles;
}

/*
 * File Styles Hook
 */
function qw_all_file_styles(){
  $styles = apply_filters('qw_file_styles', array());
  return $styles;
}

/*
 * Post types
 */
function qw_all_post_types(){
  $post_types = apply_filters('qw_post_types', array());

  // Get all verified post types
  $post_types = get_post_types(array('public' => true, '_builtin' => false), 'names', 'and');
  // Add standard types
  $post_types['post'] = 'post';
  $post_types['page'] = 'page';
  // sort types
  ksort($post_types);

  return $post_types;
}

/*
 * Pager types
 */
function qw_all_pager_types(){
  $pagers = apply_filters('qw_pager_types', array());
  return $pagers;
}

/*
 * Return Default Template File
 */
function qw_default_template_file() {
	return apply_filters('qw_default_template_file','index.php');
}