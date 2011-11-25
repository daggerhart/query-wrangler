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
        'label' => 'Meta: '.$key,
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
/*
 * All Field Styles and settings
 * 
 * @return array Field Styles
 */
function qw_default_field_styles($field_styles)
{
  $field_styles['complete'] = array(
    'template' => 'query-complete',
    'default_path' => QW_PLUGIN_DIR.'/templates', // do not include last slash
  );
  $field_styles['excerpt'] = array(
    'template' => 'query-excerpt',
    'default_path' => QW_PLUGIN_DIR.'/templates', // do not include last slash
  );
  $field_styles['unformatted'] = array(
    'template' => 'query-unformatted',
    'default_path' => QW_PLUGIN_DIR.'/templates', // do not include last slash
  );
  $field_styles['unordered_list'] = array(
    'template' => 'query-unordered_list',
    'default_path' => QW_PLUGIN_DIR.'/templates', // do not include last slash
  );
  $field_styles['ordered_list'] = array(
    'template' => 'query-ordered_list',
    'default_path' => QW_PLUGIN_DIR.'/templates', // do not include last slash
  );
  $field_styles['table'] = array(
    'template' => 'query-table',
    'default_path' => QW_PLUGIN_DIR.'/templates', // do not include last slash
  );
  
  return $field_styles;
}
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