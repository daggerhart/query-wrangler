<?php

add_filter( 'qw_filters', 'qw_filter_callback' );

/**
 * Add the callback filter to QW's list of filters
 *
 * @param $filters
 *
 * @return mixed
 */
function qw_filter_callback( $filters ) {
	$filters['callback'] = array(
		'title'               => 'Callback',
		'description'         => 'Provide a callback that can alter the query arguments in any way.',
		'form_callback'       => 'qw_filter_callback_form',
		'query_args_callback' => 'qw_filter_callback_execute',
		'query_display_types' => array( 'page', 'widget', 'override' ),
	);

	return $filters;
}

/**
 * Form for callback filter
 *
 * @param $filter
 */
function qw_filter_callback_form( $filter ) {
	if ( ! isset( $filter['values']['callback'] ) ) {
		$filter['values']['callback'] = '';
	}
	?>
	<p>
		<input class='qw-js-title'
		       type='text'
		       size="46"
		       name="<?php print $filter['form_prefix']; ?>[callback]"
		       value='<?php print $filter['values']['callback']; ?>'/>
	</p>
	<p class="description">
		The callback function will be provided the $args and $filter variables,
		and should return the modified $args array.
		<br/>Eg, <code>function my_filter_callback($args, $filter){ return
			$args; }</code>
	</p>
<?php
}

/**
 * Execute the callback filter
 *
 * @param $args
 * @param $filter
 */
function qw_filter_callback_execute( &$args, $filter ) {
	if ( isset( $filter['values']['callback'] ) && function_exists( $filter['values']['callback'] ) ) {
		$args = $filter['values']['callback']( $args, $filter );
	}
}