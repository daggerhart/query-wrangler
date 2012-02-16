<?php
/*
 * Default Filters
 *
 * Template

  $filters['filter_type'] = array(
    // title shown when selecting a filter
    'title' => 'Title',

    // help text for the user
    'description' => 'Describe this filter',

    // which types of queries to allow this filter on
    // some filters don't make sense on some query types
    'query_display_types' => array('page','widget','override'),

    // (optional) callback for form
    // * This or a template must be used
    // callback function should actually output the form
    'form_callback' => 'callback_function_form_inner_form',

    // (optional) template
    // template wrangler theme function or template file
    'template' => 'my_tw_template_hook',

    // (optional) generate_args callback
    // determines how form data becomes WP_Query arguments
    // defaults to form key as WP_Query argument key
    'query_args_callback' => 'qw_generate_query_args_post_types',
  );

 */
function qw_default_filters($filters){

  $filters['post_types'] = array(
    'title' => 'Post Type',
    'description' => 'Select which post types should be shown.',
    'template' => 'qw_filter_post_types',
    'query_args_callback' => 'qw_generate_query_args_post_types',
    'query_display_types' => array('page','widget','override'),
  );
  $filters['post_parent'] = array(
    'title' => 'Post Parent',
    'description' => 'Use only with post type "Page" to show results with the chosen parent ID.',
    'template' => 'qw_filter_post_parent',
    'query_display_types' => array('page','widget'),
  );
  $filters['post_id'] = array(
    'title' => 'Post IDs',
    'description' => 'Provide a list of post_ids to show or not show.',
    'template' => 'qw_filter_post_id',
    'query_args_callback' => 'qw_generate_query_args_post_id',
     'query_display_types' => array('page','widget','override'),
  );
  $filters['categories'] = array(
    'title' => 'Categories',
    'description' => 'Select which categories to pull posts from, and how to treat those categories.',
    'template' => 'qw_filter_categories',
    'query_args_callback' => 'qw_generate_query_args_categories',
    'query_display_types' => array('page','widget'),
  );
  $filters['tags'] = array(
    'title' => 'Tags',
    'description' => 'Select which tags to use.',
    'template' => 'qw_filter_tags',
    'query_args_callback' => 'qw_generate_query_args_tags',
    'query_display_types' => array('page','widget'),
  );
  $filters['meta_key_value'] = array(
    'title' => 'Meta Key/Value Compare',
    'description' => 'Filter for a specific meta_key / meta_value pair.',
    'template' => 'qw_filter_meta_key_value',
    'query_args_callback' => 'qw_generate_query_args_meta_key_value',
    'query_display_types' => array('page','widget','override'),
  );
  $filters['meta_key'] = array(
    'title' => 'Meta Key',
    'description' => 'Filter for a specific meta_key.',
    'template' => 'qw_filter_meta_key',
    'query_args_callback' => 'qw_generate_query_args_meta_key',
    'query_display_types' => array('page','widget','override'),
  );
  $filters['meta_value'] = array(
    'title' => 'Meta Value',
    'description' => 'Filter for a specific meta_value.',
    'template' => 'qw_filter_meta_value',
    'query_args_callback' => 'qw_generate_query_args_meta_value',
    'query_display_types' => array('page','widget','override'),
  );
  return $filters;
}
// add default filters to the filter
add_filter('qw_filters', 'qw_default_filters');

/*
 * Filter query_args_callbacks
 * These functions convert form data into WP_Query arguments
 */
function qw_generate_query_args_post_types(&$args, $filter){
  $args['post_type'] = $filter['post_types'];
}
function qw_generate_query_args_post_id(&$args, $filter){
  $pids = explode(",", $filter['post_ids']);
  array_walk($pids, 'qw_trim');
  $args[$filter['compare']] = $pids;
}
function qw_generate_query_args_categories(&$args, $filter){
  if($filter['cat_operator'] == 'cat' && is_array($filter['cats'])){
    $args[$filter['cat_operator']] = implode(",", array_keys($filter['cats']));
  }
  else {
    $args[$filter['cat_operator']] = $filter['cats'];
  }
}
function qw_generate_query_args_tags(&$args, $filter){
 $args[$filter['tag_operator']] = array_keys($filter['tags']);
}
function qw_generate_query_args_meta_key_value(&$args, $filter){
  $args['meta_key'] = $filter['meta_key'];
  $args['meta_value'] = stripslashes($filter['meta_value']);
  $args['meta_compare'] = $filter['meta_compare'];
}
function qw_generate_query_args_meta_value(&$args, $filter){
  $args['meta_value'] = stripslashes($filter['meta_value']);
}
