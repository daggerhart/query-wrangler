# Example Query Wrangler field

### Simple Field

This field will output: "This post is authored by: (the author's name)";

```php
add_filter('qw_fields', 'custom_qw_field');
 
function custom_qw_field($fields)
{
  $fields['custom_qw_field'] = array(
    'title' => 'Custom QW Field',
    'description' => 'Just an example of making your own custom fields with callbacks.',
    'output_callback' => 'get_custom_qw_field',
    'output_arguments' => true, // this provides the $post and $field parameters to the output_callback
  );
  return $fields;
}
 
/*
 * This is just a random custom function that returns some html.
 *
 * @param object $this_post - the WP post object
 * @param array $field - the QW field.  Useful if a field has custom settings (not shown in this example).
 * @param array $tokens - Available output values from other fields
 */
function get_custom_qw_field($this_post, $field, $tokens){
  // since this field is executed with "the loop", you can use most WP "template tags" as normal
  // you can do anything you need here, as long as you return (not echo) the HTML you want to display.
  
  $author = get_the_author();
  // this would provide the same results
  // $author = get_the_author_meta('display_name', $this_post->post_author);
  
  return "This post is authored by: ".$author;
}
```

### Field with settings

This field has a setting that changes the output.  The user can choose to show the post title, or the post status.

```php
// hook
add_filter('qw_fields', 'qw_field_example');

/*
 * My new field definition
 */
function qw_field_example($fields)
{
  // new field
  $fields['example_field'] = array(

    // title displayed to query-wrangler user
    'title' => 'Example: Field',

    // description on the field form
    'description' => 'Just a useful description of this field',

    // optional) callback for outputting a field, must return the results
    'output_callback' => 'qw_field_example_output',

    // (optional) where or not to pass $post and $field into the output_callback
    //    useful for custom functions
    'output_arguments' => true,

    // (optional) callback function for field forms
    'form_callback' => 'qw_field_example_form_callback',
  );
  return $fields;
}

/*
 * Example output callback with output_arguments = true
 *
 * @param $post  The WP $post object
 * @param $field This field's settings and values. Values stored in $field['values']
 */
function qw_field_example_output($post, $field){
  $output = '';
  // adjust output according to my custom field settings
  if ($field['values']['my_setting'] == 'title'){
    $output = $post->post_title;
  }
  else if ($field['values']['my_setting'] == 'status'){
    $output = $post->post_status;
  }
  return $output;
}

/*
 * Provide a settings form for this field
 *
 * Output is expected of all forms, because they are executed within a buffer
 *
 * @param $field  - this field's settings and values
                    Values stored in $field['values']
 */
function qw_field_example_form_callback($field)
{
  // retrieve the value from the field for retaining settings values
  $value = $field['values']['my_setting'];
  ?>
  <select name="<?php print $field['form_prefix']; ?>[my_setting]">
    <option value="title" <?php selected( $value, 'title' ); ?>>Show Post Title</option>
    <option value="status" <?php selected( $value, 'status' ); ?>>Show Post Status</option>
  </select>
  <?php
}
````

### Coauthors field

This field will output the coauthors' links provided by the Coauthors plus plugin

```php 
add_filter('qw_fields', 'coauthors_plus_qw_field');

function coauthors_plus_qw_field($fields)
{
  $fields['coauthors_post_links'] = array(
    'title' => 'Coauthor Posts Links',
    'description' => 'Outputs the co-authors display names, with links to their posts.',
    // If the coauthors plugin didn't echo the data by default we wouldn't even need this custom function,
    // we could have set the output_callback to 'coauthors_posts_links' instead.
    'output_callback' => 'get_coauthors_posts_links',
  );
  return $fields;
}

/*
 * http://vip.wordpress.com/documentation/incorporate-co-authors-plus-template-tags-into-your-theme/#available-template-tags
 */
function get_coauthors_posts_links(){
  return coauthors_posts_links(null,null,null,null,false); 
}
````