<?php
/*
 * All Handlers
 *
 * Handlers are groups of items that can be added and removed from a query
 * eg: filters, sorts, fields
 */
function qw_all_handlers() {
	$handlers = apply_filters( 'qw_handlers', array() );
	foreach ( $handlers as $hook_key => $handler ) {
		$handlers[ $hook_key ]['hook_key']  = $hook_key;
		$handlers[ $hook_key ]['all_items'] = $handler['all_callback']();
	}

	return $handlers;
}

/*
 * Editor themes
 */
function qw_all_edit_themes() {
	$themes = apply_filters( 'qw_edit_themes', array() );

	return $themes;
}

/*
 * Basic Settings
 */
function qw_all_basic_settings() {
	$basics = apply_filters( 'qw_basics', array() );

	foreach ( $basics as $hook_key => $basic ) {
		$basics[ $hook_key ]['form_prefix'] = QW_FORM_PREFIX . '[' . $basic['option_type'] . ']';
		$basics[ $hook_key ]['hook_key']    = $hook_key;
	}

	return $basics;
}

/*
 * Fields Hook
 */
function qw_all_fields() {
	$fields = apply_filters( 'qw_fields', array() );
	foreach ( $fields as $type => $field ) {
		if ( ! isset( $field['type'] ) ) {
			$fields[ $type ]['type'] = $type;
		}
		// maintain the hook's key
		$fields[ $type ]['hook_key'] = $type;
	}

	// sort them by title
	$titles = array();
	foreach ( $fields as $key => $field ) {
		$titles[ $key ] = $field['title'];
	}
	array_multisort( $titles, SORT_ASC, $fields );

	return $fields;
}

/*
 * filters Hook
 */
function qw_all_filters() {
	$filters = apply_filters( 'qw_filters', array() );

	foreach ( $filters as $type => $filter ) {
		// set filter's type as a value if not provided by filter
		if ( ! isset( $filter['type'] ) ) {
			$filters[ $type ]['type'] = $type;
		}
		// maintain the hook's key
		$filters[ $type ]['hook_key'] = $type;
	}

	// sort them by title
	$titles = array();
	foreach ( $filters as $key => $filter ) {
		$titles[ $key ] = $filter['title'];
	}
	array_multisort( $titles, SORT_ASC, $filters );

	return $filters;
}

/*
 * overrides Hook
 */
function qw_all_overrides() {
	$overrides = apply_filters( 'qw_overrides', array() );

	foreach ( $overrides as $type => $override ) {
		// set override's type as a value if not provided by override
		if ( empty( $override['type'] ) ) {
			$overrides[ $type ]['type'] = $type;
		}

		// maintain the hook's key
		$overrides[ $type ]['hook_key'] = $type;
	}

	// sort them by title
	$titles = array();
	foreach ( $overrides as $key => $override ) {
		$titles[ $key ] = $override['title'];
	}
	array_multisort( $titles, SORT_ASC, $overrides );

	return $overrides;
}

/*
 * Sort Options Hook
 */
function qw_all_sort_options() {
	$sort_options = apply_filters( 'qw_sort_options', array() );

	// set some defaults for very simple hooks
	foreach ( $sort_options as $type => $option ) {
		if ( ! isset( $option['type'] ) ) {
			$sort_options[ $type ]['type'] = $type;
		}
		if ( ! isset( $option['orderby_key'] ) ) {
			$sort_options[ $type ]['orderby_key'] = 'orderby';
		}
		if ( ! isset( $option['order_key'] ) ) {
			$sort_options[ $type ]['order_key'] = 'order';
		}
		if ( ! isset( $option['order_options'] ) ) {
			$sort_options[ $type ]['order_options'] = array(
				'ASC'  => 'Ascending',
				'DESC' => 'Descending',
			);
		}

		// default form_callback
		if ( ! isset( $option['form_callback'] ) && ! isset( $option['form_template'] ) ) {
			$sort_options[ $type ]['form_callback'] = 'qw_form_default_sort_order_options';
		}

		// maintain hook's key
		$sort_options[ $type ]['hook_key'] = $type;
	}

	// sort them by title
	$titles = array();
	foreach ( $sort_options as $key => $sort ) {
		$titles[ $key ] = $sort['title'];
	}
	array_multisort( $titles, SORT_ASC, $sort_options );

	return $sort_options;
}

/*
 * Post Statuses
 */
function qw_all_post_statuses() {
	$post_statuses = apply_filters( 'qw_post_statuses', array() );

	return $post_statuses;
}

/*
 * Styles Hook
 */
function qw_all_styles() {
	$styles = apply_filters( 'qw_styles', array() );

	foreach ( $styles as $hook_key => $style ) {
		$styles[ $hook_key ]['hook_key']    = $hook_key;
		$styles[ $hook_key ]['form_prefix'] = QW_FORM_PREFIX . '[display][' . $hook_key . '_settings]';

		if ( ! isset( $style['settings_key'] ) ) {
			$styles[ $hook_key ]['settings_key'] = $hook_key . '_settings';
		}
	}

	return $styles;
}

/*
 * Row Styles Hook
 */
function qw_all_row_styles() {
	$row_styles = apply_filters( 'qw_row_styles', array() );
	foreach ( $row_styles as $k => $row_style ) {
		$row_styles[ $k ]['hook_key'] = $k;
	}

	return $row_styles;
}

/*
 * Row Style 'Posts' Styles Hook
 */
function qw_all_row_complete_styles() {
	$row_complete_styles = apply_filters( 'qw_row_complete_styles', array() );

	return $row_complete_styles;
}

/*
 * Row Style 'Fields' Stlyes Hook
 */
function qw_all_row_fields_styles() {
	$row_fields_styles = apply_filters( 'qw_row_fields_styles', array() );

	return $row_fields_styles;
}

/*
 * File Styles Hook
 */
function qw_all_file_styles() {
	$styles = apply_filters( 'qw_file_styles', array() );

	return $styles;
}

/*
 * Post types
 */
function qw_all_post_types() {
	$post_types = apply_filters( 'qw_post_types', array() );

	// Get all verified post types
	$post_types += get_post_types( array(
		'public'   => TRUE,
		'_builtin' => FALSE
	),
		'names',
		'and' );
	// Add standard types
	$post_types['post'] = 'post';
	$post_types['page'] = 'page';
	// sort types
	ksort( $post_types );

	return $post_types;
}

/*
 * Pager types
 */
function qw_all_pager_types() {
	$pagers = apply_filters( 'qw_pager_types', array() );

	return $pagers;
}

/*
 * Return Default Template File
 */
function qw_default_template_file() {
	return apply_filters( 'qw_default_template_file', 'index.php' );
}


/**
 * default custom_field (meta_value_new) field display handlers
 */
// add default meta value handlers
add_filter( 'qw_meta_value_display_handlers',
	'qw_meta_value_display_handlers_default' );

/*
 * Default meta value handlers
 */
function qw_meta_value_display_handlers_default( $handlers ) {
	$handlers['none'] = array(
		'title'    => '-none-',
		'callback' => 'qw_get_post_meta',
	);
	// advanced custom fields: http://wordpress.org/plugins/advanced-custom-fields/
	if ( function_exists( 'get_field' ) ) {
		$handlers['acf_default'] = array(
			'title'    => 'Advanced Custom Fields: get_field',
			'callback' => 'qw_get_acf_field',
		);
	}
	// cctm: https://wordpress.org/plugins/custom-content-type-manager/
	if ( function_exists( 'get_custom_field' ) ) {
		$handlers['cctm_default'] = array(
			'title'    => 'CCTM: get_custom_field',
			'callback' => 'qw_get_cctm_field',
		);
	}

	return $handlers;
}

/*
 * return simple get_post_meta array
 */
function qw_get_post_meta( $post, $field ) {
	return get_post_meta( $post->ID, $field['meta_key'] );
}

/*
 * Advanced custom field generic handler
 */
function qw_get_acf_field( $post, $field ) {
	$output = '';
	if ( function_exists( 'get_field' ) ) {
		$output = get_field( $field['meta_key'], $post->ID );
	}

	return $output;
}

/*
 * Custom Content Type Manager generic field handler
 */
function qw_get_cctm_field( $post, $field ) {
	$output = '';
	if ( function_exists( 'get_custom_field' ) ) {
		$field_name = $field['meta_key'];
		if ( isset( $field['cctm_chaining'] ) && ! empty( $field['cctm_chaining'] ) ) {
			$field_name = $field_name . $field['cctm_chaining'];
		}

		$output = get_custom_field( $field_name );
	}

	return $output;
}