<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_meta_key' );

function qw_filter_meta_key( $filters ) {
	$filters['meta_key'] = array(
		'title'               => 'Meta Key',
		'description'         => 'Filter for a specific meta_key.',
		'form_callback'       => 'qw_filter_meta_key_form',
		'query_args_callback' => 'qw_generate_query_args_meta_key',
		'query_display_types' => array( 'page', 'widget', 'override' ),
	);

	return $filters;
}

function qw_filter_meta_key_form( $filter ) {
	if ( ! isset( $filter['values']['meta_key'] ) ) {
		$filter['values']['meta_key'] = '';
	}
	?>
	<p>
		<input class='qw-js-title'
		       type='text'
		       name="<?php print $filter['form_prefix']; ?>[meta_key]"
		       value='<?php print $filter['values']['meta_key']; ?>'/>
	</p>
<?php
}

function qw_generate_query_args_meta_key( &$args, $filter ) {
	$args['meta_key'] = $filter['values']['meta_key'];
}