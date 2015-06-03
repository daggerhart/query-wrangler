# Example Query Wrangler filter

This filter doesn't really accomplish anything.  It just places some random key/value pairs in the args sent to WP_Query().

```php
<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_example');

/*
 * Add filter to qw_filters
 */
function qw_filter_example($filters)
{
	// new filter
	$filters['example_filter'] = array(
		// title shown when selecting a filter
		'title' => 'Example: Filter',

		// help text for the user
		'description' => 'Description of this filter',

		// (optional) the query argument key
		// if doesn't exist, defaults to the hook_key.
		// if confused what this is for, don't use this
		'type' => 'filter_type',

		// ! This or a form_template must be used
		// * (optional) callback for form
		'form_callback' => 'qw_filter_example_form_callback',

		// * (optional) template wrangler theme function or template file
		'form_template' => 'my_tw_template_hook',

		// (optional) generate_args callback
		// determines how form data becomes WP_Query arguments
		// defaults to form key as WP_Query argument key
		'query_args_callback' => 'qw_filter_example_query_args',

		// (optional) the form exposed to a user above the query
		'exposed_form' => 'qw_filter_example_exposed_form',

		// (optional) process the exposed filter's values into the query args
		'exposed_process' => 'qw_filter_example_exposed_process',

		// (optional) a form for gather settings for the exposed filter
		'exposed_settings_form' => 'qw_filter_example_exposed_settings_form',
	);
	return $filters;
}

/*
 * Example of custom filter form.
 *
 * @param $filter - This filter's settings and saved values
 *                  Values stored in $filter['values']
 */
function qw_filter_example_form_callback($filter)
{ ?>
	<label class="qw-label">My filter setting</label>
	<input type='text'
	       name="<?php print $filter['form_prefix']; ?>[my_setting]"
	       value='<?php print $filter['values']['my_setting']; ?>' />
<?php
}

/*
 * Convert the filter settings into a WP_Query argument
 *
 *  @param  &$args - The WP_Query arguments being built
 *  @param  $filter - This filter's settings and saved values
 *                    Values stored in $filter['values']
 */
function qw_filter_example_query_args(&$args, $filter){
	$args['some_wp_query_argument'] = $filter['values']['my_setting'];
}

/*
 * Filter exposed form
 *
 *  @param  $filter - This filter's settings and values
 *                    Values stored in $filter['values']
 *  @param  $values - Submitted values for this exposed_key
 */
function qw_filter_example_exposed_form($filter, $values)
{
	// default values
	if (isset($filter['values']['exposed_default_values'])){
		if (is_null($values)){
			$values = $filter['values']['post_ids'];
		}
	}
	?>
	<input type="text"
	       name="<?php print $filter['exposed_key']; ?>"
	       value="<?php print $values ?>" />
<?php
}

/*
 * Example, processing exposed submitted values into the WP_Query
 *
 *  @param  &$args - The WP_Query args being built
 *  @param  $filter - This filter's settings and values
 *                    Values stored in $filter['values']
 *  @param  $values - The submitted value for this exposed filter
 *                   Equivalent to $d = qw_exposed_submitted_data(); $d[$filter['exposed_key']];
 */
function qw_filter_example_exposed_process(&$args, $filter, $values){
	// default values if submitted is empty
	if(isset($filter['values']['exposed_default_values'])){
		if (empty($values)){
			$values = $filter['values']['my_setting'];
		}
	}

	// check allowed values
	if (isset($filter['values']['exposed_limit_values'])){
		if ($values == $filter['values']['my_setting']){
			$args['some_wp_query_argument'] = $values;
		}
	}
	else {
		$args['some_wp_query_argument'] = $values;
	}
}

/*
 * Example additional settings for an exposed form
 *
 *  @param  $filter - This filter's settings and values
 *                    Values stored in $filter['values']
 */
function qw_filter_example_exposed_settings_form($filter)
{
	// use the default provided single/multiple exposed values
	// saves values to [exposed_settings][type]
	print qw_exposed_setting_type($filter);
}
````