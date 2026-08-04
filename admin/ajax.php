<?php
/**
 * Capability required to use Query Wrangler's admin ajax endpoints.
 */
define( 'QW_AJAX_CAPABILITY', 'edit_others_posts' );

/**
 * Nonce action shared by Query Wrangler's admin ajax endpoints.
 */
define( 'QW_AJAX_NONCE_ACTION', 'qw-admin-ajax' );

/**
 * Stop an ajax request that isn't a permitted user acting from our own admin
 * screen. Sends a response and exits when the request is rejected.
 */
function qw_verify_ajax_request() {
	if ( ! current_user_can( QW_AJAX_CAPABILITY ) ) {
		wp_send_json_error( array( 'message' => 'You are not allowed to do that.' ), 403 );
	}

	check_ajax_referer( QW_AJAX_NONCE_ACTION, 'qw_nonce' );
}

/**
 * Reduce a posted handler or handler item name to a safe key.
 *
 * These values become array keys and form input names, so keep them to the
 * characters the editor actually generates.
 *
 * @param mixed $key
 *
 * @return string
 */
function qw_sanitize_handler_key( $key ) {
	if ( ! is_string( $key ) ) {
		return '';
	}

	return preg_replace( '/[^a-zA-Z0-9_\-]/', '', $key );
}

/*
 * Ajax form templates
 */
function qw_form_ajax() {
	// This endpoint builds handler forms and renders a live query preview,
	// which executes the query's handlers. Only users who can reach the query
	// editor may use it, and only from a page that carried our nonce.
	qw_verify_ajax_request();

	if ( empty( $_POST['form'] ) ) {
		wp_send_json_error( array( 'message' => 'No form was requested.' ), 400 );
	}

	switch ( $_POST['form'] ) {
		/*
		 * Preview, special case
		 */
		case 'preview':
			$decode  = urldecode( isset( $_POST['options'] ) ? $_POST['options'] : '' );
			$options = array();
			parse_str( $decode, $options );

			if ( ! isset( $options[ QW_FORM_PREFIX ] ) || ! is_array( $options[ QW_FORM_PREFIX ] ) ) {
				wp_send_json_error( array( 'message' => 'No query options were submitted.' ), 400 );
			}

			$options[ QW_FORM_PREFIX ]['args']['paged'] = 1;
			$args                                       = array(
				'options'  => $options[ QW_FORM_PREFIX ],
				'query_id' => isset( $_POST['query_id'] ) ? (int) $_POST['query_id'] : 0,
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

		default:
			wp_send_json_error( array( 'message' => 'Unknown form requested.' ), 400 );
	}

	/*
	   * Generate handler item forms and data
	   */
	$handler = qw_sanitize_handler_key( isset( $_POST['handler'] ) ? $_POST['handler'] : '' );
	$item    = array();

	$hook_key = qw_get_hook_key( $all, $_POST );

	if ( ! $handler || ! isset( $all[ $hook_key ] ) ) {
		wp_send_json_error( array( 'message' => 'Unknown handler item requested.' ), 400 );
	}

	$item                = $all[ $hook_key ];
	$item['name']        = qw_sanitize_handler_key( isset( $_POST['name'] ) ? $_POST['name'] : '' );
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
	qw_verify_ajax_request();

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