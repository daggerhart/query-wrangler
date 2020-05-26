<?php

namespace QueryWrangler;

class QueryPostType {

	const SLUG = 'qw_query';

	/**
	 * Post type labels.
	 *
	 * @return array
	 */
	public static function labels() {
		return [
			'name' => _x( 'Queries', 'post type general name', 'query-wrangler' ),
			'singular_name' => _x( 'Query', 'post type singular name', 'query-wrangler' ),
			'menu_name' => _x( 'Query Wrangler', 'admin menu', 'query-wrangler' ),
			'name_admin_bar' => _x( 'Query', 'add new on admin bar', 'query-wrangler' ),
			'add_new' => _x( 'Add New', 'query', 'query-wrangler' ),
			'add_new_item' => __( 'Add New Query', 'query-wrangler' ),
			'new_item' => __( 'New Query', 'query-wrangler' ),
			'edit_item' => __( 'Edit Query', 'query-wrangler' ),
			'view_item' => __( 'View Query', 'query-wrangler' ),
			'all_items' => __( 'All Queries', 'query-wrangler' ),
			'search_items' => __( 'Search Queries', 'query-wrangler' ),
			'parent_item_colon' => __( 'Parent Queries:', 'query-wrangler' ),
			'not_found' => __( 'No queries found.', 'query-wrangler' ),
			'not_found_in_trash' => __( 'No queries found in Trash.', 'query-wrangler' ),
		];
	}

	/**
	 * Post type complete configuration.
	 *
	 * @return array
	 */
	public static function config() {
		return [
			'labels' => static::labels(),
			'description' => __( 'Description.', 'query-wrangler' ),
			'public' => TRUE,
			'exclude_from_search' => TRUE,
			'publicly_queryable' => FALSE,
			'show_ui' => TRUE,
			'show_in_menu' => TRUE,
			'query_var' => FALSE,
			'rewrite' => FALSE,
			'capability_type' => 'page',
			'has_archive' => FALSE,
			'hierarchical' => FALSE,
			'menu_position' => NULL,
			'supports' => [
				'title',
				'custom-fields',
			],
		];
	}

	/**
	 * Register the post type.
	 */
	public static function register() {
		register_post_type( self::SLUG, static::config() );
	}

}
