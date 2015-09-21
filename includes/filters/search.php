<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_search' );

function qw_filter_search( $filters ) {

	$filters['search'] = array(
		'title'               => 'Search',
		'description'         => 'Searches for keywords',
		'form_callback'       => 'qw_filter_search_form',
		'query_args_callback' => 'qw_generate_query_args_search',
		'query_display_types' => array( 'page', 'widget' ),
		// exposed
		'exposed_form'        => 'qw_filter_search_exposed_form',
		'exposed_process'     => 'qw_filter_search_exposed_process',
	);

	return $filters;
}

function qw_generate_query_args_search( &$args, $filter ) {
	$args['s'] = $filter['values']['search'];
}

function qw_filter_search_form( $filter ) {
	if ( ! isset( $filter['values']['search'] ) ) {
		$filter['values']['search'] = '';
	}
	?>
	<p>
		<input class="qw-js-title"
		       type="text"
		       name="<?php print $filter['form_prefix']; ?>[search]"
		       value="<?php print $filter['values']['search']; ?>"/>
	</p>
<?php
}

/*
 * Process submitted exposed form values
 */
function qw_filter_search_exposed_process( &$args, $filter, $values ) {
	// default values if submitted is empty
	qw_filter_search_exposed_default_values( $filter, $values );

	// check allowed values
	if ( isset( $filter['values']['exposed_limit_values'] ) ) {
		if ( $values == $filter['values']['search'] ) {
			$args['s'] = $values;
		}
	} else {
		$args['s'] = $values;
	}
}

/*
 * Exposed form
 */
function qw_filter_search_exposed_form( $filter, $values ) {
	// default values
	qw_filter_search_exposed_default_values( $filter, $values );
	?>
	<input type="text"
	       name="<?php print $filter['exposed_key']; ?>"
	       value="<?php print esc_attr( $values ); ?>"/>
<?php
}

//function qw_filter_search_exposed_settings_form($filter){}

/*
 * Simple helper function to handle default values
 */
function qw_filter_search_exposed_default_values( $filter, &$values ) {
	if ( isset( $filter['values']['exposed_default_values'] ) ) {
		if ( is_null( $values ) ) {
			$values = $filter['values']['search'];
		}
	}
}