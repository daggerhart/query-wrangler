<?php
/*
 * Default Filters
 */
function qw_default_filters($filters){

  $filters['post_types'] = array(
    'label' => 'Post Type',
    'type' => 'post_types',
    'description' => 'Select which post types should be shown.',
    // tw theme function or template file
    'template' => 'qw_filter_post_types',
    // generate_args callback
    'query_args_callback' => 'qw_generate_query_args_post_types',
  );
  $filters['post_status'] = array(
    'label' => 'Post Status',
    'type' => 'post_status',
    'description' => 'Select the status of the posts to show.',
    // tw theme function or template file
    'template' => 'qw_filter_post_status',
  );
  $filters['post_parent'] = array(
    'label' => 'Post Parent',
    'type' => 'post_parent',
    'description' => 'Use only with post type "Page" to show results with the chosen parent ID.',
    // tw theme function or template file
    'template' => 'qw_filter_post_parent',
  );
  $filters['post_id'] = array(
    'label' => 'Post IDs',
    'type' => 'post_id',
    'description' => 'Provide a list of post_ids to show or not show.',
    // tw theme function or template file
    'template' => 'qw_filter_post_id',
    // generate_args callback
    'query_args_callback' => 'qw_generate_query_args_post_id',
  );
  $filters['categories'] = array(
    'label' => 'Categories',
    'type' => 'categories',
    'description' => 'Select which categories to pull posts from, and how to treat those categories.',
    // tw theme function or template file
    'template' => 'qw_filter_categories',
    // generate_args callback
    'query_args_callback' => 'qw_generate_query_args_categories',
  );
  $filters['tags'] = array(
    'label' => 'Tags',
    'type' => 'tags',
    'description' => 'Select which tags to use.',
    // tw theme function or template file
    'template' => 'qw_filter_tags',
    // generate_args callback
    'query_args_callback' => 'qw_generate_query_args_tags',
  );
  $filters['meta_key_value'] = array(
    'label' => 'Meta Key/Value Compare',
    'type' => 'meta_key_value',
    'description' => 'Filter for a specific meta_key / meta_value pair.',
    // tw theme function or template file
    'template' => 'qw_filter_meta_key_value',
    // generate_args callback
    'query_args_callback' => 'qw_generate_query_args_meta_key_value',
  );
  $filters['meta_key'] = array(
    'label' => 'Meta Key',
    'type' => 'meta_key',
    'description' => 'Filter for a specific meta_key.',
    // tw theme function or template file
    'template' => 'qw_filter_meta_key',
    // generate_args callback
    'query_args_callback' => 'qw_generate_query_args_meta_key',
  );
  $filters['meta_value'] = array(
    'label' => 'Meta Value',
    'type' => 'meta_value',
    'description' => 'Filter for a specific meta_value.',
    // tw theme function or template file
    'template' => 'qw_filter_meta_value',
    // generate_args callback
    'query_args_callback' => 'qw_generate_query_args_meta_value',
  );
  return $filters;
}
// add default filters to the filter
add_filter('qw_filters', 'qw_default_filters');

function qw_default_filters_templates($templates){
  $filters = qw_all_filters();
  foreach ($filters as $name => $filter){
    $templates[$filter['template']] = array(
      'arguments' => array(
        'filter' => array()
      )
    );
  }
  return $templates;
}
// tw hook
add_filter('tw_templates', 'qw_default_filters_templates');

/*
 * Filter query_args_callbacks
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