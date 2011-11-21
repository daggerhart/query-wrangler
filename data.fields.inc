<?php 

/*
 * All Fields and Settings
 * 
 * @return array Available query fields
 */
/*
  Fields to add:
    author (int) - use author id.
    author_name (string) - use 'user_nicename' (NOT name). 

*/
function qw_all_fields(){
  $fields = array(
    'ID'  => array(
      'label' => 'Post ID',
    ),
    'post_title' => array(
      'label' => 'Post Title',
      'callback' => 'get_the_title',
    ),
    'post_content' => array(
      'label' => 'Post Content',
      'callback' => 'get_the_content',
    ),
    'post_excerpt' => array(
      'label' => 'Post Excerpt',
      'callback' => 'get_the_excerpt',
    ),
    'post_author' => array(
      'label' => 'Post Author',
      'callback' => 'get_the_author',
    ),
    'post_date' => array(
      'label' => 'Post Date',
      'callback' => 'get_the_date',
    ),
    'post_status' => array(
      'label' => 'Post Status',
    ),
    'post_parent' => array(
      'label' => 'Post Parent',
    ),
    'post_modified' => array(
      'label' => 'Post Modified',
    ),
    'guid' => array(
      'label' => 'GUID',
    ),
    'post_type' => array(
      'label' => 'Post Type',
    ),
    'comment_count' => array(
      'label' => 'Comment Count',
    ),
    'permalink' => array(
      'label' => 'Permalink',
      'callback' => 'get_permalink',
    ),
    'image_attachment' => array(
      'label' => 'Image Attachment',
      'callback' => 'qw_theme_image',
    ),
    'file_attachment' => array(
      'label' => 'File Attachment',
      'callback' => 'qw_theme_file'
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
          'key' => $key,
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
function qw_all_field_styles(){
  $field_styles = array(
    'unformatted' => array(),
    'unordered_list' => array(),
    'ordered_list' => array(),
    'table' => array(),
  );
  
  return $field_styles;
}
/*
 * File Styles
 * 
 * @return array of file styles
 */
function qw_file_styles(){
  $file_styles = array(
    'link' => 'Filename Link to File',
    'link_url' => 'URL Link to File',
    'url'  => 'URL of File',
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