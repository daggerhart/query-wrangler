<?php
// add default fields to the hook filter
add_filter('qw_fields', 'qw_callback_field');

/*
 * Add field to qw_fields
 */
function qw_callback_field($fields){

  $fields['callback_field'] = array(
    'title' => 'Callback',
    'description' => 'Arbitrarily execute a function.',
    'form_callback' => 'qw_callback_field_form',
    'output_callback' => 'qw_execute_the_callback',
    'output_arguments' => true,
  );
  return $fields;
}

/*
 * Execute callback function
 */
function qw_execute_the_callback($post, $field, $tokens){
  $returned = false;
  $echoed = false;
  
  ob_start();
    if (isset($field['custom_output_callback']) && function_exists($field['custom_output_callback'])){
      if (isset($field['include_output_arguments'])){
        $returned = $field['custom_output_callback']($post, $field, $tokens);
      }
      else {
        $returned = $field['custom_output_callback']();
      }
    }
  $echoed = ob_get_clean();

  // some functions both return and echo a value
  // so make sure to only show 1 instance of the callback
  if ($returned) {
    return $returned;
  }
  
  if ($echoed){
    return $echoed;
  }
}

/*
 * Custom callback settings form
 */
function qw_callback_field_form($field)
{
  $custom_output_callback = (isset($field['values']['custom_output_callback']) && !empty($field['values']['custom_output_callback'])) ? $field['values']['custom_output_callback'] : '';
  //$callback_exists =  ($custom_output_callback && function_exists($custom_output_callback)) ? true : false;
  $include_output_args = (isset($field['values']['include_output_arguments'])) ? 'checked="checked"' : '';
  ?>
    <div>
      <p>
        <label class="qw-label">Callback:</label>
        <input  class='qw-js-title' type="text" name="<?php print $field['form_prefix']; ?>[custom_output_callback]" value="<?php print $custom_output_callback; ?>" />
      </p>
      <p class="description">
        Provide an existing function name.  This function will be executed during the loop of this query.
      </p>
    </div>
    <div>
      <label class='qw-field-checkbox'>
        <input type='checkbox'
               name='<?php print $field['form_prefix']; ?>[include_output_arguments]'
               <?php print $include_output_args; ?> /> - Include additional information</label>
      <p class="description">If checked, the callback will be executed with the parameters $post, $field, and $tokens.  The $post parameter is a Wordpress $post object, and the $field paramater is the query wrangler field settings, and the $tokens parameter includes all the available token values.</p>
    </div>
  <?php
}

