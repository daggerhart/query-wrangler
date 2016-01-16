<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_post_id' );

function qw_filter_post_id( $filters ) {
	$filters['post_id'] = array(
		'title'               => 'Post IDs',
		'description'         => 'Provide a list of post_ids to show or not show.',
		'form_callback'       => 'qw_filter_post_id_form',
		'query_args_callback' => 'qw_generate_query_args_post_id',
		'query_display_types' => array( 'page', 'widget', 'override' ),
		// exposed
		'exposed_form'        => 'qw_filter_post_id_exposed_form',
		'exposed_process'     => 'qw_filter_post_id_exposed_process',
	);

	return $filters;
}

function qw_filter_post_id_form( $filter ) {
	if ( ! isset( $filter['values']['post_ids'] ) ) {
		$filter['values']['post_ids'] = '';
	}
	if ( ! isset( $filter['values']['post_ids_callback'] ) ) {
		$filter['values']['post_ids_callback'] = '';
	}
	if ( ! isset( $filter['values']['compare'] ) ) {
		$filter['values']['compare'] = '';
	}
	?>
    <p>
      <label>Provide post_ids as a comma separated list:</label>
      <div><input class="qw-js-title" type='text' size="46" name="<?php print $filter['form_prefix']; ?>[post_ids]" value='<?php print $filter['values']['post_ids']; ?>' /></div>
    </p>
    <p>
      <label>Or, provide a callback function name that returns an array of post_ids:</label>
      <div><input class="qw-js-title" type="text" size="46" name="<?php print $filter['form_prefix']; ?>[post_ids_callback]" value="<?php print $filter['values']['post_ids_callback']; ?>" /></div>
      <p class="description">Note: you cannot expose a filter if using a callback.</p>
    </p>
    <p>
      <label>How to treat these post IDs.</label>
      <select class="qw-js-title" name="<?php print $filter['form_prefix']; ?>[compare]">
        <option value="post__in" <?php if ( $filter['values']['compare'] == "post__in" ) {
		print 'selected="selected"';
	} ?>>Only these posts</option>
        <option value="post__not_in" <?php if ( $filter['values']['compare'] == "post__not_in" ) {
		print 'selected="selected"';
	} ?>>Not these posts</option>
      </select>
    </p>
  <?php
}

function qw_generate_query_args_post_id( &$args, $filter ) {
	if ( isset( $filter['values']['post_ids_callback'] ) && function_exists( $filter['values']['post_ids_callback'] ) ) {
		$pids = $filter['values']['post_ids_callback']( $args );
	} else {
	    $values = qw_contextual_tokens_replace( $filter['values']['post_ids'] );
		$pids = explode( ",", $values );
	}

	array_walk( $pids, 'qw_trim' );
	$args[ $filter['values']['compare'] ] = $pids;
}


/*
 * Process submitted exposed form values
 */
function qw_filter_post_id_exposed_process( &$args, $filter, $values ) {
	// default values if submitted is empty
	qw_filter_post_id_exposed_default_values( $filter, $values );

	// make into array
	$values = explode( ",", $values );
	array_walk( $values, 'qw_trim' );

	// check allowed values
	if ( isset( $filter['values']['exposed_limit_values'] ) ) {
		$allowed = explode( ",", $filter['values']['post_ids'] );
		// trim spaces
		array_walk( $allowed, 'qw_trim' );
		array_walk( $values, 'qw_trim' );
		// loop through and check allowed values
		foreach ( $values as $k => $value ) {
			if ( ! in_array( $value, $allowed ) ) {
				unset( $values[ $k ] );
			}
		}
	}
	// set the values
	$args[ $filter['values']['compare'] ] = $values;
}

/*
 * Exposed form
 */
function qw_filter_post_id_exposed_form( $filter, $values ) {
	// adjust for default values
	qw_filter_post_id_exposed_default_values( $filter, $values );
	?>
	<input type="text"
	       name="<?php print $filter['exposed_key']; ?>"
	       value="<?php print $values ?>"/>
<?php
}

/*
 * Simple helper function to handle default values
 */
function qw_filter_post_id_exposed_default_values( $filter, &$values ) {
	if ( isset( $filter['values']['exposed_default_values'] ) ) {
		if ( is_null( $values ) ) {
			$values = $filter['values']['post_ids'];
		}
	}
}
//function qw_filter_post_id_exposed_settings_form($filter){}