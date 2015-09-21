<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_meta_value' );

function qw_filter_meta_value( $filters ) {
	$filters['meta_value'] = array(
		'title'               => 'Meta Value',
		'description'         => 'Filter for a specific meta_value.',
		'form_callback'       => 'qw_filter_meta_value_form',
		'query_args_callback' => 'qw_generate_query_args_meta_value',
		'query_display_types' => array( 'page', 'widget', 'override' ),
	);

	return $filters;
}

function qw_filter_meta_value_form( $filter ) {
	if ( ! isset( $filter['values']['meta_value'] ) ) {
		$filter['values']['meta_value'] = '';
	}
	?><p>
	<textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
	          class="qw-meta-value qw-js-title"><?php print stripcslashes( $filter['values']['meta_value'] ); ?></textarea>
	</p>
<?php
}

function qw_generate_query_args_meta_value( &$args, $filter ) {
	if ( isset( $filter['values']['meta_value'] ) ) {
		$args['meta_value'] = stripslashes( $filter['values']['meta_value'] );
	}
}