<?php
/*
 * Controls for executing administrator-provided callback functions.
 *
 * Three handlers let a query name a PHP function to execute while the query
 * runs: the Callback field, the Callback filter, and the Post IDs filter.
 * Query options do not always come from the database - the query editor's
 * live preview builds them out of $_POST - so a function name found in an
 * options array cannot be trusted on its own.
 *
 * Every one of those executions is routed through qw_callback_is_allowed(),
 * which requires the name to be:
 *
 *   1. shaped like a plain function name,
 *   2. absent from the denylist of dangerous functions,
 *   3. present in the administrator's allow list,
 *   4. present in stored, administrator-authored query data,
 *   5. an existing function.
 *
 * Requirement 4 is what stops the live preview from introducing a callback:
 * $_POST can name any function it likes, but only names an administrator
 * actually saved will run.
 */

/**
 * Option keys whose value is treated as a callback function name.
 *
 * @return array
 */
function qw_callback_option_keys() {
	$keys = array(
		// includes/fields/callback_field.php
		'custom_output_callback',
		// includes/filters/callback.php
		'callback',
		// includes/filters/post_id.php
		'post_ids_callback',
	);

	return apply_filters( 'qw_callback_option_keys', $keys );
}

/**
 * Functions that may never run as a query callback, allow list or not.
 *
 * This is a backstop, not the primary control. It exists so that a polluted
 * allow list cannot immediately become code execution.
 *
 * @return array
 */
function qw_callback_denylist() {
	$denylist = array(
		// arbitrary code and command execution
		'eval',
		'assert',
		'create_function',
		'exec',
		'shell_exec',
		'system',
		'passthru',
		'proc_open',
		'popen',
		'pcntl_exec',
		// indirect execution, used to smuggle the above past an allow list
		'call_user_func',
		'call_user_func_array',
		'array_map',
		'array_filter',
		'array_walk',
		'array_walk_recursive',
		'array_reduce',
		'usort',
		'uasort',
		'uksort',
		'preg_replace_callback',
		'preg_replace_callback_array',
		'register_shutdown_function',
		'register_tick_function',
		'set_error_handler',
		'set_exception_handler',
		'forward_static_call',
		'forward_static_call_array',
		'iterator_apply',
		'add_action',
		'add_filter',
		'do_action',
		'apply_filters',
		// includes and filesystem writes
		'require',
		'require_once',
		'include',
		'include_once',
		'file_put_contents',
		'fwrite',
		'fputs',
		'copy',
		'rename',
		'unlink',
		'rmdir',
		'chmod',
		'move_uploaded_file',
		// deserialization
		'unserialize',
		'maybe_unserialize',
		// destructive or privilege-granting WordPress APIs
		'wp_insert_user',
		'wp_create_user',
		'wp_update_user',
		'wp_delete_user',
		'wp_set_password',
		'wp_set_current_user',
		'add_option',
		'update_option',
		'delete_option',
		'wp_delete_post',
		'wp_delete_attachment',
		'wp_remote_get',
		'wp_remote_post',
		'wp_remote_request',
	);

	return apply_filters( 'qw_callback_denylist', $denylist );
}

/**
 * Reduce a value to a plain function name, or an empty string.
 *
 * Deliberately narrow: no class methods, no closures, no "Class::method"
 * strings, nothing that call_user_func() would treat as dynamic.
 *
 * @param mixed $callback
 *
 * @return string
 */
function qw_sanitize_callback_name( $callback ) {
	if ( ! is_string( $callback ) ) {
		return '';
	}

	$callback = trim( $callback );

	if ( ! preg_match( '/^[a-zA-Z_\x80-\xff][a-zA-Z0-9_\x80-\xff]*$/', $callback ) ) {
		return '';
	}

	return $callback;
}

/**
 * Function names an administrator has approved for execution.
 *
 * @return array
 */
function qw_allowed_callbacks() {
	$allowed = QW_Settings::get_instance()->get( 'allowed_callbacks', array() );

	if ( ! is_array( $allowed ) ) {
		$allowed = array();
	}

	/*
	 * Filter the callbacks Query Wrangler is allowed to execute.
	 *
	 * Use this to approve callbacks from code instead of the settings screen:
	 *
	 *   add_filter( 'qw_allowed_callbacks', function( $allowed ) {
	 *     $allowed[] = 'my_theme_query_field_callback';
	 *     return $allowed;
	 *   } );
	 */
	$allowed = apply_filters( 'qw_allowed_callbacks', $allowed );

	$allowed = array_map( 'qw_sanitize_callback_name', (array) $allowed );

	return array_values( array_unique( array_filter( $allowed ) ) );
}

/**
 * Turn a newline or comma separated list of names into a clean array.
 *
 * Used to sanitize the allow list on its way into the settings option.
 *
 * @param mixed $value
 *
 * @return array
 */
function qw_sanitize_callback_list( $value ) {
	if ( is_string( $value ) ) {
		$value = preg_split( '/[\s,]+/', $value );
	}

	$value = array_map( 'qw_sanitize_callback_name', (array) $value );

	return array_values( array_unique( array_filter( $value ) ) );
}

/**
 * Every callback name defined in a single query's options array.
 *
 * @param array $options
 *
 * @return array
 */
function qw_extract_callbacks( $options ) {
	if ( ! is_array( $options ) ) {
		return array();
	}

	$keys  = qw_callback_option_keys();
	$found = array();

	array_walk_recursive( $options, function ( $value, $key ) use ( $keys, &$found ) {
		if ( in_array( $key, $keys, TRUE ) ) {
			$name = qw_sanitize_callback_name( $value );
			if ( $name ) {
				$found[] = $name;
			}
		}
	} );

	return array_values( array_unique( $found ) );
}

/**
 * Every callback name found in stored query data.
 *
 * Saving a query requires manage_options (admin/query-admin-pages.php), so
 * anything in this table was authored by an administrator. That makes it the
 * trust anchor for callback execution.
 *
 * @return array
 */
function qw_stored_callbacks() {
	$cached = get_transient( 'qw_stored_callbacks' );

	if ( is_array( $cached ) ) {
		return $cached;
	}

	global $wpdb;
	$table = $wpdb->prefix . 'query_wrangler';
	$found = array();

	// This runs while rendering the front end. If the table is missing we want
	// no callbacks rather than a database error on the page.
	$suppressing = $wpdb->suppress_errors();
	$rows        = $wpdb->get_col( "SELECT data FROM {$table}" );
	$wpdb->suppress_errors( $suppressing );

	foreach ( (array) $rows as $data ) {
		$found = array_merge( $found, qw_extract_callbacks( qw_unserialize( $data ) ) );
	}

	$found = array_values( array_unique( $found ) );

	set_transient( 'qw_stored_callbacks', $found, DAY_IN_SECONDS );

	return $found;
}

/**
 * Forget the cached list of stored callbacks.
 *
 * Called whenever query data changes.
 */
function qw_flush_stored_callbacks() {
	delete_transient( 'qw_stored_callbacks' );
}

/**
 * Whether a callback provided by query options may be executed.
 *
 * @param mixed $callback Function name from a query's options.
 * @param string $context Handler the callback came from, for debug logging.
 *
 * @return bool
 */
function qw_callback_is_allowed( $callback, $context = 'callback' ) {
	$name = qw_sanitize_callback_name( $callback );

	if ( ! $name ) {
		return FALSE;
	}

	$denied = array_map( 'strtolower', qw_callback_denylist() );

	if ( in_array( strtolower( $name ), $denied, TRUE ) ) {
		qw_callback_refused( $name, $context, 'the function is on Query Wrangler\'s denylist' );

		return FALSE;
	}

	if ( ! in_array( $name, qw_allowed_callbacks(), TRUE ) ) {
		qw_callback_refused( $name, $context, 'the function is not in the Allowed Callbacks setting' );

		return FALSE;
	}

	// Options can be built from $_POST by the editor's live preview. Only
	// names an administrator saved to the database are executable.
	if ( ! in_array( $name, qw_stored_callbacks(), TRUE ) ) {
		qw_callback_refused( $name, $context, 'the function is not saved in any query - save the query before previewing it' );

		return FALSE;
	}

	if ( ! function_exists( $name ) ) {
		qw_callback_refused( $name, $context, 'the function does not exist' );

		return FALSE;
	}

	return TRUE;
}

/**
 * Approve the callbacks a site was already using, once.
 *
 * The allow list was introduced after callbacks had been executable for years.
 * Starting empty would silently break working sites, so on the first run we
 * approve whatever is already saved in query data - which only an
 * administrator could have put there. Anything added after this runs has to be
 * approved deliberately.
 */
function qw_seed_allowed_callbacks() {
	if ( get_option( 'qw_allowed_callbacks_seeded' ) ) {
		return;
	}

	$settings = QW_Settings::get_instance();
	$existing = $settings->get( 'allowed_callbacks', array() );
	$denied   = array_map( 'strtolower', qw_callback_denylist() );

	$seeded = array();
	foreach ( qw_stored_callbacks() as $callback ) {
		if ( ! in_array( strtolower( $callback ), $denied, TRUE ) ) {
			$seeded[] = $callback;
		}
	}

	$settings->set( 'allowed_callbacks',
		array_merge( (array) $existing, $seeded ),
		'qw_sanitize_callback_list' );
	$settings->save();

	update_option( 'qw_allowed_callbacks_seeded', 1 );
}

/**
 * Explain a callback's status underneath a handler's form field.
 *
 * Callbacks silently doing nothing is confusing, so say plainly whether this
 * one will run and what to do about it if it won't.
 *
 * @param mixed $callback Function name currently entered in the form.
 */
function qw_callback_status_notice( $callback ) {
	$name = qw_sanitize_callback_name( $callback );

	if ( ! $name ) {
		?>
		<p class="description">
			Callbacks must be approved before Query Wrangler will execute them.
			Add the function name to <em>Allowed Callbacks</em> on the
			<a href="<?php print esc_url( admin_url( 'admin.php?page=qw-settings' ) ); ?>">Query
				Wrangler settings</a> screen.
		</p>
		<?php

		return;
	}

	$reasons = array();

	if ( in_array( strtolower( $name ), array_map( 'strtolower', qw_callback_denylist() ), TRUE ) ) {
		$reasons[] = 'it is on Query Wrangler\'s denylist and can never be executed';
	}
	else {
		if ( ! in_array( $name, qw_allowed_callbacks(), TRUE ) ) {
			$reasons[] = 'it is not listed in <em>Allowed Callbacks</em> on the Query Wrangler settings screen';
		}
		if ( ! in_array( $name, qw_stored_callbacks(), TRUE ) ) {
			$reasons[] = 'it has not been saved yet &mdash; save this query, then preview it';
		}
		if ( ! function_exists( $name ) ) {
			$reasons[] = 'no function by that name exists';
		}
	}

	if ( ! $reasons ) {
		?>
		<p class="description" style="color: #227122;">
			<strong>&#10003; <?php print esc_html( $name ); ?></strong> is
			approved and will be executed.
		</p>
		<?php

		return;
	}
	?>
	<p class="description" style="color: #a02222;">
		<strong>&#10007; <?php print esc_html( $name ); ?></strong> will not be
		executed because <?php print implode( ', and ', $reasons ); ?>.
	</p>
	<?php
}

/**
 * Note a refused callback when debugging is on.
 *
 * @param string $callback
 * @param string $context
 * @param string $reason
 */
function qw_callback_refused( $callback, $context, $reason ) {
	if ( ! defined( 'WP_DEBUG' ) || ! WP_DEBUG ) {
		return;
	}

	error_log( sprintf( 'Query Wrangler refused to execute the %s "%s" because %s.',
		$context,
		$callback,
		$reason ) );
}