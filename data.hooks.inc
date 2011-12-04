<?php
/*
 * Fields Hook
 */
function qw_all_fields(){
  $fields = apply_filters('qw_fields', array());
  return $fields;
}

/*
 * Field Styles Hook
 */
function qw_all_field_styles(){
  $styles = apply_filters('qw_field_styles', array());
  return $styles;
}

/*
 * filters Hook
 */
function qw_all_filters(){
  $filters = apply_filters('qw_filters', array());
  return $filters;
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