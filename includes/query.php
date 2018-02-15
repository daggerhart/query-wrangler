<?php
/*
 * Primary function for building and displaying a query
 *
 * @param int $query_id Id for the query
 * @param array $options_override an array for changing or adding query data options
 * @param bool $reset_post_data Reset the $wp_query after execution
 * @return string Can return a string of html based on parameter $return
 */
function qw_execute_query(
	$query_id,
	$options_override = array(),
	$reset_post_data = TRUE
) {
	// get the query options
	$options = qw_generate_query_options( $query_id, $options_override );

	// get formatted query arguments
	$args = qw_generate_query_args( $options );

	// pre_query hook
	$args = apply_filters( 'qw_pre_query', $args, $options );

	// set the new query
	$qw_query = new WP_Query( $args );

	// pre_render hook
	$options = apply_filters( 'qw_pre_render', $options );

	// get the themed content
	$themed = qw_template_query( $qw_query, $options );

	// Reset Post Data
	if ( $reset_post_data ) {
		wp_reset_postdata();
	}

	return $themed;
}

/*
 * Get the Query, and set $options to defaults
 *
 * @param int
 *   $query_id - The unique ID of the query
 * @param array
 *   $options_override - An array of values to override in the retrieved set
 * @param array
 *   $full_override - force the options_override as all options
 *
 * @return array
 *   Query options
 */
function qw_generate_query_options(
	$query_id,
	$options_override = array(),
	$full_override = FALSE
) {
	global $wpdb;
	$table_name = $wpdb->prefix . "query_wrangler";
	$sql        = "SELECT id,name,type,slug,data FROM " . $table_name . " WHERE id = %d";

	$rows = $wpdb->get_results( $wpdb->prepare( $sql, $query_id ) );

	if ( empty( $rows ) ) {
		return array();
	}

	// unserialize the stored data
	$options = qw_unserialize( $rows[0]->data );

	// override options
	if ( $full_override ) {
		// force a full override
		$options = $options_override;
	} else {
		// combine options
		$options = array_replace_recursive( (array) $options, $options_override );
	}

	// build query_details
	$options['meta']               = array();
	$options['meta']['id']         = $rows[0]->id;
	$options['meta']['slug']       = $rows[0]->slug;
	$options['meta']['name']       = $rows[0]->name;
	$options['meta']['type']       = $rows[0]->type;
	$options['meta']['pagination'] = ( isset( $options['display']['page']['pager']['active'] ) ) ? 1 : 0;
	$options['meta']['header']     = $options['display']['header'];
	$options['meta']['footer']     = $options['display']['footer'];
	$options['meta']['empty']      = $options['display']['empty'];

	return $options;
}

/*
 * Generate the WP query itself
 *
 * @param array $options Query data
 * @return array Query Arguments
 */
function qw_generate_query_args( $options = array() ) {
	$handlers = qw_preprocess_handlers( $options );

	$paged = NULL;
	// if pager_key is enabled, trick qw_get_page_number
	if ( isset( $options['display']['page']['pager']['use_pager_key'] ) &&
	     isset( $options['display']['page']['pager']['pager_key'] ) &&
	     isset( $_GET[ $options['display']['page']['pager']['pager_key'] ] ) &&
	     is_numeric( $_GET[ $options['display']['page']['pager']['pager_key'] ] )
	) {
		$paged = $_GET[ $options['display']['page']['pager']['pager_key'] ];
	}

	// standard arguments
	$args['paged']               = ( $paged ) ? $paged : qw_get_page_number();
	$args['posts_per_page']      = ( $options['args']['posts_per_page'] ) ? $options['args']['posts_per_page'] : 5;
	$args['offset']              = ( $options['args']['offset'] ) ? $options['args']['offset'] : 0;
	$args['post_status']         = $options['args']['post_status'];
	$args['ignore_sticky_posts'] = isset( $options['args']['ignore_sticky_posts'] ) ? $options['args']['ignore_sticky_posts'] : 0;

	// having any offset will break pagination
	if ( $args['paged'] > 1 ){
		unset( $args['offset'] );
	}

	$submitted_data = qw_exposed_submitted_data();

	foreach ( $handlers as $handler_type => $handler ) {
		if ( is_array( $handler['items'] ) ) {
			foreach ( $handler['items'] as $name => $item ) {
				// Exposed items
				if ( isset( $item['exposed_form'] ) ) {
					if ( ! empty( $item['values']['exposed_key'] ) ) {
						// override exposed key
						$item['exposed_key'] = $item['values']['exposed_key'];
					} else {
						// default exposed key
						$item['exposed_key'] = 'exposed_' . $item['values']['name'];
					}
				}
				// */

				// Alter the query args
				// look for callback, and run it
				if ( isset( $item['query_args_callback'] ) && function_exists( $item['query_args_callback'] ) ) {
					$item['query_args_callback']( $args, $item );
				} else if ( isset( $item['orderby_key'] ) && isset( $item['order_key'] ) ) {
					// else, default to type as WP_Query argument key
					// arguments passed to query
					$args[ $item['orderby_key'] ] = $item['type'];
					$args[ $item['order_key'] ]   = isset( $item['values']['order_value'] ) ? $item['values']['order_value'] : 'ASC';
				}

				// Process submitted exposed values
				// exposed items
				if ( isset( $item['values']['is_exposed'], $submitted_data[ $item['exposed_key'] ] ) && function_exists( $item['exposed_process'] ) ) {
					$value = $submitted_data[ $item['exposed_key'] ];
					$item['exposed_process']( $args, $item, $value );
				}
				//*/
			}
		}
	}

	return $args;
}

/**
 * Get a query's id by using its slug
 */
function qw_get_query_by_slug( $slug ) {
	global $wpdb;

	return $wpdb->get_var( $wpdb->prepare( "SELECT id FROM " . $wpdb->prefix . "query_wrangler WHERE `slug` = '%s'",
		$slug ) );
}

/**
 * Get an unserialized query row from the database, using the query's id
 *
 * @param $id
 *
 * @return bool|mixed
 */
function qw_get_query_by_id( $id ) {
	global $wpdb;
	$table = $wpdb->prefix . 'query_wrangler';
	$sql   = "SELECT * FROM $table WHERE id = %d LIMIT 1";
	$query = $wpdb->get_row( $wpdb->prepare( $sql, $id ) );

	if ( $query ) {
		$query->data = qw_unserialize( $query->data );

		return $query;
	}

	return FALSE;
}

/**
 * Get a query's id by that is set to override a specific term_id
 *
 * @param $term_id
 *
 * @return bool
 */
function qw_get_query_by_override_term( $term_id ) {

	global $wpdb;
	$qw_table  = $wpdb->prefix . "query_wrangler";
	$qot_table = $wpdb->prefix . "query_override_terms";

	$sql = "SELECT qw.id FROM " . $qw_table . " as qw
              LEFT JOIN " . $qot_table . " as ot ON ot.query_id = qw.id
              WHERE qw.type = 'override' AND ot.term_id = %d
              LIMIT 1";

	$row = $wpdb->get_row( $wpdb->prepare( $sql, $term_id ) );

	if ( $row ) {
		return $row->id;
	}

	return FALSE;
}

/*
 * Get all queries of the type widget
 *
 * @return array of query widgets with key as query id
 */
function qw_get_all_widgets() {
	global $wpdb;
	$table_name = $wpdb->prefix . "query_wrangler";
	$sql        = "SELECT id,name FROM " . $table_name . " WHERE type = 'widget'";
	$rows       = $wpdb->get_results( $sql );

	if ( is_array( $rows ) ) {
		$widgets = array();
		foreach ( $rows as $row ) {
			$widgets[ $row->id ] = $row->name;
		}

		return $widgets;
	}
}

/*
 * Get all query pages
 *
 * @return array Query pages in WP post format
 */
function qw_get_all_pages() {
	global $wpdb;
	$table_name = $wpdb->prefix . "query_wrangler";
	$sql        = "SELECT id,name,path FROM " . $table_name . " WHERE type = 'page'";
	$rows       = $wpdb->get_results( $sql );

	if ( is_array( $rows ) ) {
		$pages    = array();
		$blog_url = get_bloginfo( 'wpurl' );

		$i = 0;
		foreach ( $rows as $row ) {
			$pages[ $i ]             = new stdClass();
			$pages[ $i ]->ID         = $row->id;
			$pages[ $i ]->title      = $row - name;
			$pages[ $i ]->post_title = $row->name;
			$pages[ $i ]->guid       = $blog_url . $row->path;
			$pages[ $i ]->post_type  = 'page';
		}

		return $pages;
	}
}

/*
 * Helper function: Get the current page number
 * @param object $qw_query - the query being displayed
 *
 * @return
 *    int - the currentpage number
 */
function qw_get_page_number( $qw_query = NULL ) {
	// help figure out the current page
	$path_array = explode( '/page/', $_SERVER['REQUEST_URI'] );

	// look for WP paging first
	if ( ! is_null( $qw_query ) && isset( $qw_query->query_vars['paged'] ) ) {
		$page = $qw_query->query_vars['paged'];
	} // try wordpress method
	else if ( ! is_null( $qw_query ) && get_query_var( 'paged' ) ) {
		$page = get_query_var( 'paged' );
	} // paging with slashes
	else if ( isset( $path_array[1] ) ) {
		$page = explode( '/', $path_array[1] );
		$page = $page[0];
	} // paging with get variable
	else if ( isset( $_GET['page'] ) ) {
		$page = $_GET['page'];
	} // paging with a different get variable
	else if ( isset( $_GET['paged'] ) ) {
		$page = $_GET['paged'];
	} else {
		$page = 1;
	}

	return $page;
}

/*
 * Get full term data
 *
 * @param $term
 *   - either term id or term slug based on 2nd parameter
 * @param $by
 *   - either 'id' or 'slug': what you want the get term by
 * @param $return_type
 *  - OBJECT, ARRAY_A, ARRAY_N
 *
 * @return
 *  - term, format depending on 3rd parameter
 *  - false if not found
 */
function qw_get_term( $term, $by = 'id', $return_type = OBJECT ) {
	global $wpdb;
	switch ( $by ) {
		case 'id':
			$where = 't.term_id = %d';
			break;

		case 'slug':
			$where = 't.slug = %s';
			break;
	}

	$sql = "SELECT
            t.term_id,t.name,t.slug,t.term_group,tax.taxonomy,tax.description,tax.parent,tax.count
          FROM {$wpdb->terms} as t
          LEFT JOIN {$wpdb->term_taxonomy} as tax ON t.term_id = tax.term_id
          WHERE {$where}";

	$term = $wpdb->get_row( $wpdb->prepare($sql, $term), $return_type );

	if ( $term->term_id ) {
		// http://web.archiveorange.com/archive/v/XZYvyS8D7kDM3sQgrvJF
		$term->link = get_term_link( (int) $term->term_id, $term->taxonomy );

		// return term if found
		return $term;
	}
	else {
		return FALSE;
	}
}

/*
 * Trim each item in an array w/ array_walk
 *   eg: array_walk($fruit, 'qw_trim');
 */
function qw_trim( &$value ) {
	$value = trim( $value );
}

/*
 * Serialize wrapper functions for future changes.
 */
function qw_serialize( $array ) {
	return serialize( $array );
}

/*
 * Custom: Fix unserialize problem with quotation marks
 */
function qw_unserialize( $serial_str ) {
	$data = maybe_unserialize( $serial_str );

	// if the string failed to unserialize, we may have a quotation problem
	if ( !is_array( $data ) ) {
		$serial_str = preg_replace_callback('!s:(\d+):"(.*?)";!s', 'qw_unserialize_extra_fix_callback', $serial_str);
		$data = maybe_unserialize( $serial_str );
	}

	if ( is_array( $data ) ) {
		// stripslashes twice for science
		$data = array_map( 'stripslashes_deep', $data );
		$data = array_map( 'stripslashes_deep', $data );

		return $data;
	}

	// if we're here the data wasn't unserialized properly.
	// return a modified version of the default query to prevent major failures.
	$default = qw_default_query_data();
	$default['display']['title'] = 'error unserializing query data';
	$default['args']['posts_per_page'] = 1;

	return $default;
}

/**
 * Attempt to fix issues with quotation marks in a serialized string.
 * This is a replacement for the previous preg_replace approach that used the
 * 'e' flag. The 'e' flag was removed in php 7.
 *
 * @param $matches
 *
 * @return string
 */
function qw_unserialize_extra_fix_callback($matches) {
	return 's:' . strlen($matches[2]) . ':"' . $matches[2] . '";';
}

/*
 * Support function for legacy, pre hook_keys discovery
 */
function qw_get_hook_key( $all, $single ) {
	// default to new custom_field (meta_value_new)
	$hook_key = 'custom_field';

	// see if hook key is set
	if ( ! empty( $single['hook_key'] ) && isset( $all[ $single['hook_key'] ] ) ) {
		$hook_key = $single['hook_key'];
	} // look for type as key
	else if ( ! empty( $single['type'] ) ) {
		foreach ( $all as $key => $item ) {
			if ( $single['type'] == $item['type'] ) {
				$hook_key = $item['hook_key'];
				break;
			} else if ( $single['type'] == $key ) {
				$hook_key = $key;
				break;
			}
		}
	}

	return $hook_key;
}

/*
 * Generate form prefixes for handlers
 *
 * @param string
 *    $type = sort, field, filter, override
 */
function qw_make_form_prefix( $type, $name ) {
	$handlers = qw_all_handlers();

	if ( isset( $handlers[ $type ]['form_prefix'] ) ) {
		$output = QW_FORM_PREFIX . $handlers[ $type ]['form_prefix'] . '[' . $name . ']';
	} else {
		$output = QW_FORM_PREFIX . "[" . $name . "]";
	}

	return $output;
}

/*
 * Function for grabbing meta keys
 *
 * @return array All meta keys in WP
 */
function qw_get_meta_keys() {
	global $wpdb;

	$keys = $wpdb->get_col( "
			SELECT meta_key
			FROM $wpdb->postmeta
			GROUP BY meta_key
			ORDER BY meta_key" );

	return $keys;
}

/*
 * Replace contextual tokens within a string
 *
 * @params string $args - a query argument string
 *
 * @return string - query argument string with tokens replaced with values
 */
function qw_contextual_tokens_replace( $args ) {
	$matches = array();
	preg_match_all( '/{{([^}]*)}}/', $args, $matches );
	if ( isset( $matches[1] ) ) {
		global $post;

		foreach ( $matches[1] as $i => $context_token ) {
			if ( stripos( $context_token, ':' ) !== FALSE ) {
				$a = explode( ':', $context_token );
				if ( $a[0] == 'post' && isset( $post->{$a[1]} ) ) {
					$args = str_replace( $matches[0][ $i ],
						$post->{$a[1]},
						$args );
				} else if ( $a[0] == 'query_var' && $replace = get_query_var( $a[1] ) ) {
					$args = str_replace( $matches[0][ $i ], $replace, $args );
				}
			}
		}
	}

	return $args;
}

/*
 * usort callback. I likely stole this from somewhere.. like php.net
 */
function qw_cmp( $a, $b ) {
	if ( $a['weight'] == $b['weight'] ) {
		return 0;
	}

	return ( $a['weight'] < $b['weight'] ) ? - 1 : 1;
}

/*
 * Default values for  new query
 *
 * @return array Default query settings
 */
function qw_default_query_data() {
	return array(
		'display' => array(
			'title'           => '',
			'style'           => 'unformatted',
			'row_style'       => 'posts',
			'post_settings'   => array(
				'size' => 'complete',
			),
			'header'          => '',
			'footer'          => '',
			'empty'           => '',
			'wrapper-classes' => '',
			'page'            => array(
				'pager' => array(
					'type'     => 'default',
					'previous' => '',
					'next'     => '',
				),
			),
		),
		'args'    => array(
			'posts_per_page' => '5',
			'offset'         => 0,
			'post_status'    => 'publish',
			'filters'        => array(
				'post_types' => array(
					'type'       => 'post_types',
					'hook_key'   => 'post_types',
					'name'       => 'post_types',
					'weight'     => '0',
					'post_types' =>
						array(
							'post' => 'post',
						),
				),
			),
			'sorts'          => array(
				'date' => array(
					'type'        => 'date',
					'hook_key'    => 'post_date',
					'name'        => 'date',
					'weight'      => '0',
					'order_value' => 'DESC',
				),
			),
		),
	);
}

/**
 * Get an existing query as QW_Query object
 *
 * @param $id
 *
 * @return null|QW_Query
 */
function qw_get_query( $id ) {
	if ( ! empty( $id ) ) {
		$query = new QW_Query( $id );

		if ( $query && is_a( $query, 'QW_Query' ) && ! $query->is_new ) {
			return $query;
		}
	}

	return NULL;
}

/**
 * Create a new empty QW_Query
 */
function qw_create_query() {
	return new QW_Query();
}
