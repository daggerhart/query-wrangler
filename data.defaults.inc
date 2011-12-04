<?php 
/*
 * All Fields and Settings
 * 
 * @return array Available query fields
 */
function qw_default_fields($fields)
{
  $fields['ID'] = array(
    'label' => 'Post ID',
  );
  $fields['post_title'] = array(
    'label' => 'Post Title',
    'callback' => 'get_the_title',
  );
  $fields['post_content'] = array(
    'label' => 'Post Content',
    'callback' => 'get_the_content',
  );
  $fields['post_excerpt'] = array(
    'label' => 'Post Excerpt',
    'callback' => 'get_the_excerpt',
  );
  $fields['post_author'] = array(
    'label' => 'Post Author',
    'callback' => 'get_the_author',
  );
  $fields['post_date'] = array(
    'label' => 'Post Date',
    'callback' => 'get_the_date',
  );
  $fields['post_status'] = array(
    'label' => 'Post Status',
  );
  $fields['post_parent'] = array(
    'label' => 'Post Parent',
  );
  $fields['post_modified'] = array(
    'label' => 'Post Modified',
  );
  $fields['guid'] = array(
    'label' => 'GUID',
  );
  $fields['post_type'] = array(
    'label' => 'Post Type',
  );
  $fields['comment_count'] = array(
    'label' => 'Comment Count',
  );
  $fields['permalink'] = array(
    'label' => 'Permalink',
    'callback' => 'get_permalink',
  );
  $fields['image_attachment'] = array(
    'label' => 'Image Attachment',
    'callback' => 'qw_theme_image',
    'arguments' => array(
      'post_id' => 'this:ID',
      'style' => 'setting:image_display_style',
      'count' => 'setting:image_display_count',
    ),
  );
  $fields['file_attachment'] = array(
    'label' => 'File Attachment',
    'callback' => 'qw_theme_file',
    'arguments' => array(
      'post_id' => 'this:ID',
      'style' => 'setting:file_display_style',
      'count' => 'setting:file_display_count',
    ),
  );
  
  // add meta keys to field list
  $meta = qw_get_meta_keys();
  foreach($meta as $key){
    $field_key = 'meta_'.str_replace(" ", "_", $key);
    // ignore WP keys and Widget Wangler keys
    if(substr($key, 0, 1) != '_' &&
       substr($key,0,3) != 'ww-' &&
       substr($key,0,3) != 'ww_')
    {
      $fields[$field_key] = array(
        'label' => 'Custom Field: '.$key,
        'callback' => 'get_post_meta',
        'arguments' => array(
          'post_id' => 'this:ID',
          'meta_key' => $key,
          'single' => 1, // true
        ),
      );
    }
  }
  
  return $fields;
}
// add default fields to the hook filter
add_filter('qw_fields', 'qw_default_fields');
/*
 * All Field Styles and settings
 * 
 * @return array Field Styles
 */
function qw_default_field_styles($field_styles)
{
  $field_styles['complete'] = array(
    'type' => 'full',
    'template' => 'query-complete',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $field_styles['excerpt'] = array(
    'type' => 'full',
    'template' => 'query-excerpt',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $field_styles['unformatted'] = array(
    'type' => 'fields',
    'template' => 'query-unformatted',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $field_styles['unordered_list'] = array(
    'type' => 'fields',
    'template' => 'query-unordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $field_styles['ordered_list'] = array(
    'type' => 'fields',
    'template' => 'query-ordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $field_styles['table'] = array(
    'type' => 'fields',
    'template' => 'query-table',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  
  return $field_styles;
}
// add default field styles to the filter
add_filter('qw_field_styles', 'qw_default_field_styles');

/*
 * Default Filters
 */
function qw_default_filters($filters){
  $filters['post_types'] = array(
    'label' => 'Post Type',
    'type' => 'post_types',
  );
  $filters['post_status'] = array(
    'label' => 'Post Status',
    'type' => 'post_status',
  );
  $filters['post_parent'] = array(
    'label' => 'Post Parent',
    'type' => 'post_parent',
  );
  $filters['categories'] = array(
    'label' => 'Categories',
    'type' => 'categories',
  );
  $filters['tags'] = array(
    'label' => 'Tags',
    'type' => 'tags',
  );
  
  return $filters;
}
// add default filters to the filter
add_filter('qw_filters', 'qw_default_filters');
/*
 * File Styles
 * 
 * @return array of file styles
 */
function qw_default_file_styles($file_styles)
{
  $file_styles['link'] = array(
    'description' => 'Filename Link to File',
  );
  $file_styles['link_url'] = array(
    'description' => 'URL Link to File',
  );
  $file_styles['url'] = array(
    'description' => 'URL of File',
  );

  return $file_styles;
}
// add default file styles to the filter
add_filter('qw_file_styles', 'qw_default_file_styles');

/*
 * Setup pager types 
 */
function qw_default_pager_types($pagers) {
  $pagers['default'] = array(
    'label' => 'Default',
    'callback' => 'qw_theme_pager_default'
  );
  $pagers['numbers']	= array(
    'label' => 'Page Numbers',
    'callback' => 'qw_theme_pager_numbers'
  );
  
  // WP PageNavi Plugin
  if(function_exists('wp_pagenavi')) {
    $pagers['pagenavi'] = array(
      'label' => 'PageNavi',
      'callback' => 'wp_pagenavi'
    );
  }
  return $pagers;
}
// add default pager types
add_filter('qw_pager_types', 'qw_default_pager_types');


/*
 * Function for grabbing meta keys
 * 
 * @return array All meta keys in WP
 */
function qw_get_meta_keys() {
	global $wpdb;

	$keys = $wpdb->get_col( "
			SELECT meta_key
			FROM $wpdb->postmeta
			GROUP BY meta_key
			ORDER BY meta_key" );

	return $keys;
}