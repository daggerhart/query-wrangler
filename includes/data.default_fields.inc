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
    'form_callback' => 'qw_field_author',
    'output_callback' => 'qw_get_the_author',
    'output_arguments' => true,
  );
  $fields['post_author_avatar'] = array(
    'title' => 'Post Author Avatar',
    'description' => 'Avatar for the author of a post.',
    'form_callback' => 'qw_field_author_avatar',
    'output_callback' => 'qw_get_avatar',
    'output_arguments' => true,
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
    'output_callback' => 'qw_theme_image', // in theme.inc
    'output_arguments' => true,
    'form_callback' => 'qw_form_image_attachment',
  );
  $fields['file_attachment'] = array(
    'title' => 'File Attachment',
    'description' => 'Files that are attached to a post.',
    'output_callback' => 'qw_theme_file', // in theme.inc
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
        'description' => 'WP Custom Field data with key: '.$key,
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
 * Forms and Output Callbacks
 *
 * Forms: return form output
 *   $field: Combined field data and values saved
 *   $field['values']: Saved values for this field
 *   $field['form_prefix']: beginning of a form item's name attribute
 *
 * Output Callbacks: return the entire output of the field
 *   $field: field values
 */
/*
 * Post Meta form settings
 */
function qw_get_post_meta($post, $field){
  return get_post_meta($post->ID, $field['meta_key'], true);
}
/*
 * Author output callback
 */
function qw_get_the_author($post, $field){
  switch($field['output_type']){
    case 'ID':
      $author = $post->post_author;
      break;

    case 'name':
    default:
      $author = get_the_author();
      break;
  }

  if(isset($field['link_to_author'])){
    $author = '<a href="'.get_author_posts_url($post->post_author).'">'.$author.'</a>';
  }
  return $author;
}
/*
 * Author form callback
 */
function qw_field_author($field)
{
  $options = array(
    'name' => 'Author Name',
    'ID' => 'Author ID',
  );
  $link_selected = ($field['values']['link_to_author']) ? 'checked="checked"' : '';
  ob_start();
  ?>
      <label class="qw-label">Author Field Settings:</label>

      <select name="<?php print $field['form_prefix']; ?>[output_type]">
        <?php
          foreach($options as $value => $title)
          { ?>
            <option value="<?php print $value; ?>"
                    <?php print ($field['values']['output_type'] == $value) ? 'selected="selected"' : ''; ?>>
              <?php print $title; ?>
            </option>
            <?php
          }
        ?>
      </select>

      <p>
        <label class='qw-field-checkbox'>
          <input type='checkbox'
                 name='<?php print $field['form_prefix']; ?>[link_to_author]'
                 <?php print $link_selected; ?> />
          Link to author page (list of author posts)
        </label>
      </p>
  <?php
  return ob_get_clean();
}
/*
 * Avatar output callback
 *
 * get_avatar( $id_or_email, $size, $default, $alt );
 */
function qw_get_avatar($post, $field){
  if(isset($field['link_to_author'])){
    $output = '<a href="'.get_author_posts_url($post->post_author).'">'.get_avatar($post->post_author, $field['size']).'</a>';
  }
  else {
    $output = get_avatar($post->post_author, $field['values']['size']);
  }
  return $output;
}
/*
 * Avatar form callback
 */
function qw_field_author_avatar($field)
{
  $image_sizes = get_intermediate_image_sizes();
  $link_selected = ($field['values']['link_to_author']) ? 'checked="checked"' : '';
  ob_start();
  ?>
    <label class="qw-label">Avatar Size: </label>
    <input type="text"
           name="<?php print $field['form_prefix']; ?>[size]"
           value="<?php print $field['values']['size']; ?>" />
    (pixel width)
  <p>
    <label class='qw-field-checkbox'>
      <input type='checkbox'
             name='<?php print $field['form_prefix']; ?>[link_to_author]'
             <?php print $link_selected; ?> />
      Link to author page (list of author posts)
    </label>
  </p>
  <?php
  return ob_get_clean();
}

/*
 * Image attachment settings
 */
function qw_form_image_attachment($field)
{
  $image_sizes = get_intermediate_image_sizes();
  ob_start();
  ?>
    <!-- image display -->
    <label class="qw-label">Number of items to show:</label>
    <input class="qw-text-short qw-title-ignore"
           type="text"
           name='<?php print $field['form_prefix']; ?>[image_display_count]'
           value="<?php print ($field['values']['image_display_count']) ? $field['values']['image_display_count']: 0;?>" />

    <p>
      <label class="qw-label">Image Display Style:</label>
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
    </p>
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
      <label class="qw-label">Number of items to show:</label>
      <input class="qw-text-short qw-title-ignore"
             type="text"
             name='<?php print $field['form_prefix']; ?>[file_display_count]'
             value="<?php print ($field['values']['file_display_count']) ? $field['values']['file_display_count']: 0;?>" />

      <p>
        <label class="qw-label">File Display Style:</label>
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
      </p>
    <?php
  return ob_get_clean();
}