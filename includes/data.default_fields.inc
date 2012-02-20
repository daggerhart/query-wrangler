<?php
/*
 * All Fields and Settings
 *
 * Template

  $fields['hook_key'] = array(
    // title displayed to query-wrangler user
    'title' => 'File Attachment',

    // description on the field form
    'description' => 'Just a useful description of this field'

    // optional) callback for outputting a field, must return the results
    'output_callback' => 'qw_theme_file',

    // (optional) where or not to pass $post and $field into the output_callback
    //    useful for custom functions
    'output_arguments' => true,

    // (optional) callback function for field forms
    'form_callback' => 'qw_form_file_attachment',
  );

 */
function qw_default_fields($fields)
{
  $fields['ID'] = array(
    'title' => 'Post ID',
    'description' => 'The post ID.',
  );
  $fields['post_title'] = array(
    'title' => 'Post Title',
    'description' => 'The title of a post.',
    'output_callback' => 'get_the_title',
  );
  $fields['post_content'] = array(
    'title' => 'Post Content',
    'description' => 'The full content body of a post.',
    'output_callback' => 'get_the_content',
  );
  $fields['post_excerpt'] = array(
    'title' => 'Post Excerpt',
    'description' => 'The excerpt of a post.',
    'output_callback' => 'get_the_excerpt',
  );
  $fields['post_author'] = array(
    'title' => 'Post Author',
    'description' => 'Information relating to the author of a post.',
    'output_callback' => 'get_the_author',
  );
  $fields['post_date'] = array(
    'title' => 'Post Date',
    'description' => 'Published date of a post.',
    'output_callback' => 'get_the_date',
  );
  $fields['post_status'] = array(
    'title' => 'Post Status',
    'description' => 'Status of a post.',
  );
  $fields['post_parent'] = array(
    'title' => 'Post Parent',
    'description' => 'Parent page ID for a page.',
  );
  $fields['post_modified'] = array(
    'title' => 'Post Modified',
    'description' => 'Last date a post was modified.',
  );
  $fields['guid'] = array(
    'title' => 'GUID',
    'description' => 'Global Unique ID for a post (url).',
  );
  $fields['post_type'] = array(
    'title' => 'Post Type',
    'description' => 'The type of a post.',
  );
  $fields['comment_count'] = array(
    'title' => 'Comment Count',
    'description' => 'Number of comments for a post.',
  );
  $fields['permalink'] = array(
    'title' => 'Permalink',
    'description' => 'Pretty URL for a post.',
    'output_callback' => 'get_permalink',
  );
  $fields['image_attachment'] = array(
    'title' => 'Image Attachment',
    'description' => 'Image files that are attached to a post.',
    'output_callback' => 'qw_theme_image',
    'output_arguments' => true,
    'form_callback' => 'qw_form_image_attachment',
  );
  $fields['file_attachment'] = array(
    'title' => 'File Attachment',
    'description' => 'Files that are attached to a post.',
    'output_callback' => 'qw_theme_file',
    'output_arguments' => true,
    'form_callback' => 'qw_form_file_attachment',
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
        'output_callback' => 'qw_get_post_meta',
        'output_arguments' => true,
        'meta_key' => $key,
      );
    }
  }

  return $fields;
}
// add default fields to the hook filter
add_filter('qw_fields', 'qw_default_fields');

/*
 * Image attachment settings
 */
function qw_form_image_attachment($field)
{
  $image_sizes = get_intermediate_image_sizes();
  ob_start();
  ?>
    <!-- image display -->
    <div class="qw-options-group">
      <strong>Number of items to show:</strong> <br />
      <input type="text"
          name='<?php print $field['form_prefix']; ?>[image_display_count]'
          value="<?php print ($field['values']['image_display_count']) ? $field['values']['image_display_count']: 0;?>" />
      <br />
      <strong>Image Display Style:</strong><br />
      <select name='<?php print $field['form_prefix']; ?>[image_display_style]'>
        <?php
          foreach($image_sizes as $key => $size)
          {
            $size_selected = ($field['values']['image_display_style'] == $size) ? 'selected="selected"' : '';
            ?>
              <option value="<?php print $size; ?>" <?php print $size_selected; ?>><?php print $size; ?></option>
            <?php
          }
        ?>
      </select>
    </div>
  <?php
  return ob_get_clean();
}
/*
 * File attachment settings
 */
function qw_form_file_attachment($field)
{
  $file_styles = qw_all_file_styles();
  ob_start();
  ?>
    <!-- file display -->
    <div class="qw-options-group">
      <strong>Number of items to show:</strong> <br />
      <input type="text"
          name='<?php print $field['form_prefix']; ?>[file_display_count]'
          value="<?php print ($field['values']['file_display_count']) ? $field['values']['file_display_count']: 0;?>" />
      <br />
      <strong>File Display Style:</strong><br />
      <select name='<?php print $field['form_prefix']; ?>[file_display_style]'>
        <?php
          foreach($file_styles as $key => $file_style_details)
          {
            $style_selected = ($field['values']['file_display_style'] == $key) ? 'selected="selected"' : '';
            ?>
              <option value="<?php print $key; ?>" <?php print $style_selected; ?>><?php print $file_style_details['description']; ?></option>
            <?php
          }
        ?>
      </select>
    </div>
    <?php
  return ob_get_clean();
}