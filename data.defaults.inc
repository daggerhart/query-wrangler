<?php
/*
 * Post statuses as a hook for contributions
 */
function qw_default_post_statuses($post_statuses){
  $post_statuses['publish'] = array(
    'title' => 'Published',
  );
  $post_statuses['pending'] = array(
    'title' => 'Pending',
  );
  $post_statuses['draft'] = array(
    'title' => 'Draft',
  );
  $post_statuses['future'] = array(
    'title' => 'Future (Scheduled)',
  );
  $post_statuses['trash'] = array(
    'title' => 'Trashed',
  );
  $post_statuses['private'] = array(
    'title' => 'Private',
  );
  $post_statuses['any'] = array(
    'title' => 'Any',
  );
  return $post_statuses;
}
// add default fields to the hook filter
add_filter('qw_post_statuses', 'qw_default_post_statuses');

/*
 * All Fields and Settings
 *
 * @return array Available query fields
 */
function qw_default_fields($fields)
{
  $fields['ID'] = array(
    'title' => 'Post ID',
  );
  $fields['post_title'] = array(
    'title' => 'Post Title',
    'callback' => 'get_the_title',
  );
  $fields['post_content'] = array(
    'title' => 'Post Content',
    'callback' => 'get_the_content',
  );
  $fields['post_excerpt'] = array(
    'title' => 'Post Excerpt',
    'callback' => 'get_the_excerpt',
  );
  $fields['post_author'] = array(
    'title' => 'Post Author',
    'callback' => 'get_the_author',
  );
  $fields['post_date'] = array(
    'title' => 'Post Date',
    'callback' => 'get_the_date',
  );
  $fields['post_status'] = array(
    'title' => 'Post Status',
  );
  $fields['post_parent'] = array(
    'title' => 'Post Parent',
  );
  $fields['post_modified'] = array(
    'title' => 'Post Modified',
  );
  $fields['guid'] = array(
    'title' => 'GUID',
  );
  $fields['post_type'] = array(
    'title' => 'Post Type',
  );
  $fields['comment_count'] = array(
    'title' => 'Comment Count',
  );
  $fields['permalink'] = array(
    'title' => 'Permalink',
    'callback' => 'get_permalink',
  );
  $fields['image_attachment'] = array(
    'title' => 'Image Attachment',
    'callback' => 'qw_theme_image',
    'arguments' => array(
      'post_id' => 'this:ID',
      'style' => 'setting:image_display_style',
      'count' => 'setting:image_display_count',
    ),
  );
  $fields['file_attachment'] = array(
    'title' => 'File Attachment',
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
        'title' => 'Custom Field: '.$key,
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
    'title' => 'Unformatted',
    'template' => 'query-unformatted',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['unordered_list'] = array(
    'title' => 'Unordered List',
    'template' => 'query-unordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['ordered_list'] = array(
    'title' => 'Ordered List',
    'template' => 'query-ordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['table'] = array(
    'title' => 'Table',
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
    'title' => 'Posts',
  );
  $row_styles['fields'] = array(
    'title' => 'Fields',
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
    'title' => 'Complete Post',
  );
  $row_complete_styles['excerpt'] = array(
    'title' => 'Excerpt',
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
    'title' => 'Posts',
  );
  $row_fields_styles['fields'] = array(
    'title' => 'Fields',
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
    'title' => 'None',
  );
  $sort_options['ID'] = array(
    'title' => 'Post ID',
  );
  $sort_options['author'] = array(
    'title' => 'Author',
  );
  $sort_options['title'] = array(
    'title' => 'Title',
  );
  $sort_options['date'] = array(
    'title' => 'Date',
  );
  $sort_options['modified'] = array(
    'title' => 'Last Modified',
  );
  $sort_options['parent'] = array(
    'title' => 'Parent',
  );
  $sort_options['rand'] = array(
    'title' => 'Random',
  );
  $sort_options['comment_count'] = array(
    'title' => 'Comment Count',
  );
  $sort_options['menu_order'] = array(
    'title' => 'Menu Order (for Page post_types)',
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
    'title' => 'Default',
    'callback' => 'qw_theme_pager_default'
  );
  $pagers['numbers']	= array(
    'title' => 'Page Numbers',
    'callback' => 'qw_theme_pager_numbers'
  );

  // WP PageNavi Plugin
  if(function_exists('wp_pagenavi')) {
    $pagers['pagenavi'] = array(
      'title' => 'PageNavi',
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