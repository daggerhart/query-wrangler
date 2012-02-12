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
function qw_default_styles($styles)
{
  $styles['unformatted'] = array(
    'type' => 'fields',
    'label' => 'Unformatted',
    'template' => 'query-unformatted',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['unordered_list'] = array(
    'type' => 'fields',
    'label' => 'Unordered List',
    'template' => 'query-unordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['ordered_list'] = array(
    'type' => 'fields',
    'label' => 'Ordered List',
    'template' => 'query-ordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['table'] = array(
    'type' => 'fields',
    'label' => 'Table',
    'template' => 'query-table',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );

  return $styles;
}
// add default field styles to the filter
add_filter('qw_styles', 'qw_default_styles');
/*
 * Default Row Styles
 */
function qw_default_row_styles($row_styles){
  $row_styles['posts'] = array(
    'type' => 'posts',
    'label' => 'Posts',
  );
  $row_styles['fields'] = array(
    'type' => 'fields',
    'label' => 'Fields',
  );
  return $row_styles;
}
// add default field styles to the filter
add_filter('qw_row_styles', 'qw_default_row_styles');

/*
 * Default Row 'Posts' Styles
 */
function qw_default_row_complete_styles($row_complete_styles){
  $row_complete_styles['complete'] = array(
    'type' => 'complete',
    'label' => 'Complete Post',
  );
  $row_complete_styles['excerpt'] = array(
    'type' => 'excerpt',
    'label' => 'Excerpt',
  );
  return $row_complete_styles;
}
// add default field styles to the filter
add_filter('qw_row_complete_styles', 'qw_default_row_complete_styles');

/*
 * Default Row 'Fields' Styles
 */
function qw_default_row_fields_styles($row_fields_styles){
  $row_fields_styles['posts'] = array(
    'type' => 'posts',
    'label' => 'Posts',
  );
  $row_fields_styles['fields'] = array(
    'type' => 'fields',
    'label' => 'Fields',
  );
  return $row_styles;
}
// add default field styles to the filter
add_filter('qw_row_fields_styles', 'qw_default_row_fields_styles');

/*
 * Default Sort Options
 */
function qw_default_sort_options($sort_options){
  $sort_options['none'] = array(
    'type' => 'none',
    'label' => 'Nonde',
  );
  $sort_options['ID'] = array(
    'type' => 'ID',
    'label' => 'Post ID',
  );
  $sort_options['author'] = array(
    'type' => 'author',
    'label' => 'Author',
  );
  $sort_options['title'] = array(
    'type' => 'title',
    'label' => 'Title',
  );
  $sort_options['date'] = array(
    'type' => 'date',
    'label' => 'Date',
  );
  $sort_options['modified'] = array(
    'type' => 'modified',
    'label' => 'Last Modified',
  );
  $sort_options['parent'] = array(
    'type' => 'parent',
    'label' => 'Parent',
  );
  $sort_options['rand'] = array(
    'type' => 'rand',
    'label' => 'Random',
  );
  $sort_options['comment_count'] = array(
    'type' => 'comment_count',
    'label' => 'Comment Count',
  );
  $sort_options['menu_order'] = array(
    'type' => 'menu_order',
    'label' => 'Menu Order (for Page post_types)',
  );
  return $sort_options;
}
// add default field styles to the filter
add_filter('qw_sort_options', 'qw_default_sort_options');

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