<?php
/*
 * Ajax form templates
 */
function qw_form_ajax() {
	switch ( $_POST['form'] ) {
		/*
		 * Preview, special case
		 */
		case 'preview':
			$decode  = urldecode( $_POST['options'] );
			$options = array();
			parse_str( $decode, $options );
			$options['qw-query-options']['args']['paged'] = 1;
			$args                                         = array(
				'options'  => $options['qw-query-options'],
				'query_id' => $_POST['query_id'],
			);
			print theme( 'query_preview', $args );
			exit;
			break;

		case 'sort_form':
			$template = 'query_sort';
			$all      = qw_all_sort_options();
			break;

		case 'field_form':
			$template = 'query_field';
			$all      = qw_all_fields();
			break;

		case 'filter_form':
			$template = 'query_filter';
			$all      = qw_all_filters();
			break;

		case 'override_form':
			$template = 'query_override';
			$all      = qw_all_overrides();
			break;

		case 'sort_sortable':
			$template = 'query_sort_sortable';
			$all      = qw_all_sort_options();
			break;

		case 'field_sortable':
			$template = 'query_field_sortable';
			$all      = qw_all_fields();
			break;

		case 'filter_sortable':
			$template = 'query_filter_sortable';
			$all      = qw_all_filters();
			break;
	}

	/*
	   * Generate handler item forms and data
	   */
	$handler = $_POST['handler'];
	$item    = array();

	$hook_key            = qw_get_hook_key( $all, $_POST );
	$item                = $all[ $hook_key ];
	$item['name']        = $_POST['name'];
	$item['form_prefix'] = qw_make_form_prefix( $handler, $item['name'] );

	// handler item's form
	if ( isset( $item['form_callback'] ) && function_exists( $item['form_callback'] ) ) {
		ob_start();
		$item['form_callback']( $item );
		$item['form'] = ob_get_clean();
	} // provide template wrangler support
	else if ( isset( $item['form_template'] ) ) {
		$item['form'] = theme( $item['form_template'],
			array( $handler => $item ) );
	}

	$args = array(
		$handler => $item,
	);
	// weight for sortable handler items
	if ( isset( $_POST['next_weight'] ) ) {
		$args['weight'] = $_POST['next_weight'];
	}

	wp_send_json( array( 'template' => theme( $template, $args ) ) );
}

/*
 * Random data grabs
 */
function qw_data_ajax() {
	if ( isset( $_POST['data'] ) ) {
		switch ( $_POST['data'] ) {
			case 'all_hooks':
				$query_id = isset( $_POST['queryId'] ) ? $_POST['queryId'] : NULL;
				$data     = qw_edit_json( $query_id );

				wp_send_json( $data );
				break;
		}
	}
}