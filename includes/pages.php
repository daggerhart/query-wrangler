<?php

// http://codex.wordpress.org/Conditional_Tags
function qw_get_page_op() {
	global $wp_query;
	$op = '';

	// look for categories and tags in the query_vars
	if ( is_category() || ! empty( $wp_query->query_vars['category_name'] ) ) {
		$op = 'category';
	} else if ( is_tag() || ! empty( $wp_query->query_vars['tag'] ) ) {
		$op = 'tag';
	} else if ( is_404() ) {
		$op = '404';
	} else if ( is_home() ) {
		$op = 'home';
	} else if ( is_single() ) {
		// single is not pages
		$op = 'single';
	} else if ( is_page() ) {
		$op = 'page';
	} else if ( is_author() ) {
		$op = 'author';
	} else if ( is_search() ) {
		$op = 'search';
	} else if ( is_paged() ) {
		$op = 'paged';
	} else {
		$op = 'default';
	}

	// fix incorrectly identifying query pages as categories or tags
	// depending on permalink structures
	if ( $op == 'category' ||
	     $op == 'tag'
	) {
		if ( $wp_query->found_posts == 0 ) {
			$op = '404';
		}
	}

	return $op;
}

/*
 * Handle the qw path for pages
 */
function qw_get_path() {
	global $wpdb;

	$request_uri = $_SERVER['REQUEST_URI'];
	$request_uri = explode( "?", $request_uri );
	// Look at the requested uri w/o paginiation involved
	$dirty_path = explode( '/page/', $request_uri[0] );

	// Clean the result for DB
	$path = esc_sql( ltrim( $dirty_path[0], '/' ) );

	return $path;
}

//********* New page handling ********************/

//  http://stackoverflow.com/questions/12133200/how-do-i-create-a-route-in-wordpress
add_action( 'parse_request', 'qw_route_requests' );
function qw_route_requests( $wp ) {
	qw_execute_query_page();
}

function qw_execute_query_page() {

	global $wp_query, $wpdb;
	// page op is based on WP tags like is_category() && is_404()
	$op = qw_get_page_op();

	$qw_table  = $wpdb->prefix . "query_wrangler";
	$qot_table = $wpdb->prefix . "query_override_terms";

	// We'll need to override query options
	$options_override = array();

	// get current page query-like path
	$path = qw_get_path();

	// make sure a path exists
	if ( empty( $path ) ) {
		return;
	}

	// fix the query paging
	if ( $paged = qw_get_page_number( $wp_query ) ) {
		$wp_query->query_vars['paged'] = $paged;
		$wp_query->query_vars['page']  = $paged;
	}

	/*
	   * 404 -> Query type = Page
	   */
	if ( $op == '404' ||
	     $op == 'default'
	) {
		// include Template Wrangler
		if ( ! function_exists( 'theme' ) ) {
			include_once QW_PLUGIN_DIR . '/template-wrangler.php';
		}

		// take into account trailing slash
		if ( substr( $path, strlen( $path ) - 1, 1 ) != '/' ) {
			$alt_path = $path . '/';
		} else {
			$alt_path = substr( $path, 0, strlen( $path ) - 1 );
		}

		// Look for the query path given
		$sql  = "SELECT id,name,path,data FROM " . $qw_table . " WHERE type = 'page' AND (path = '" . $path . "' OR path = '" . $alt_path . "')";
		$rows = $wpdb->get_results( $sql );
	}

	// got results for a query
	if ( isset( $rows[0] ) && is_object( $rows[0] ) && $query = $rows[0] ) {
		//
		// pass the paged value into the query
		$options_override['args']['paged'] = $paged;

		// actual options
		$options = qw_unserialize( $query->data );

		// resulting query
		$themed_query = qw_execute_query( $query->id,
			$options_override,
			FALSE );

		// The title of the query
		$post_title = ( $options['display']['title'] ) ? $options['display']['title'] : $query->name;

		// Make the post object
		$post                = new stdClass();
		$post->ID            = - 42;  // Arbitrary post id
		$post->post_title    = $post_title;
		$post->post_content  = $themed_query;
		$post->post_status   = 'publish';
		$post->post_type     = 'page';
		$post->post_category = array( 'uncategorized' );
		$post->post_excerpt  = '';
		$post->ancestors     = array();

		// set some query information
		$wp_query->queried_object = $post;
		$wp_query->post           = $post;
		$wp_query->found_posts    = TRUE;
		$wp_query->post_count     = TRUE;
		//$wp_query->max_num_pages = true;
		$wp_query->is_single     = TRUE;
		$wp_query->is_posts_page = TRUE;
		$wp_query->is_page       = TRUE;
		$wp_query->posts         = array( $post );
		$wp_query->is_404        = FALSE;
		$wp_query->is_post       = FALSE;
		$wp_query->is_home       = FALSE;
		$wp_query->is_archive    = FALSE;
		$wp_query->is_category   = FALSE;

		// According to http://codex.wordpress.org/Plugin_API/Action_Reference
		// we can safely exit here. The template will take care of the rest.
		// chosen template path
		$template_path = locate_template( array( $options['display']['page']['template-file'] ) );
		if ( ! file_exists( $template_path ) ) {
			$template_path = locate_template( array( qw_default_template_file() ) );
		}

		include( $template_path );

		exit();
		// */
	}
}