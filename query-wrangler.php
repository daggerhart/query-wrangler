<?php
/*

******************************************************************

Contributors:      daggerhart
Plugin Name:       Query Wrangler
Plugin URI:        http://daggerhart.com
Description:       Query Wrangler provides an intuitive interface for creating complex WP queries as pages or widgets. Based on Drupal Views.
Author:            Jonathan Daggerhart
Author URI:        http://daggerhart.com
Version:           1.5.43

******************************************************************

Copyright 2010  Jonathan Daggerhart  (email : jonathan@daggerhart.com)
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as
published by the Free Software Foundation.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

******************************************************************

*/

// some useful definitions
define( 'QW_VERSION', 1.543 );
define( 'QW_PLUGIN_DIR', dirname( __FILE__ ) );
define( 'QW_PLUGIN_URL', plugins_url( '', __FILE__ ) );
define( 'QW_DEFAULT_THEME', 'views' );
define( 'QW_FORM_PREFIX', "qw-query-options" );


include_once QW_PLUGIN_DIR . '/includes/class-qw-settings.php';
include_once QW_PLUGIN_DIR . '/includes/class-qw-query.php';
include_once QW_PLUGIN_DIR . '/widget.query.php';

/*
 * Init functions
 */
function qw_init_frontend() {
	$settings = QW_Settings::get_instance();

	// some additional functions to support php 5.2-
	include_once QW_PLUGIN_DIR . '/includes/php-polyfill.php';

	// include Template Wrangler
	if ( ! function_exists( 'theme' ) ) {
		include_once QW_PLUGIN_DIR . '/template-wrangler.php';
	}
	// Wordpress hooks
	include_once QW_PLUGIN_DIR . '/includes/hooks.php';
	include_once QW_PLUGIN_DIR . '/includes/exposed.php';
	include_once QW_PLUGIN_DIR . '/includes/handlers.php';
	include_once QW_PLUGIN_DIR . '/includes/class-qw-shortcodes.php';
	QW_Shortcodes::register();

	// basics
	include_once QW_PLUGIN_DIR . '/includes/basics/display_title.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/template_styles.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/row_styles.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/posts_per_page.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/post_status.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/offset.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/header.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/footer.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/empty.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/wrapper_settings.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/page_path.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/page_template.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/pager.php';
	include_once QW_PLUGIN_DIR . '/includes/basics/ignore_sticky_posts.php';

	// fields
	include_once QW_PLUGIN_DIR . '/includes/fields/default_fields.php';
	include_once QW_PLUGIN_DIR . '/includes/fields/post_author.php';
	include_once QW_PLUGIN_DIR . '/includes/fields/post_author_avatar.php';
	include_once QW_PLUGIN_DIR . '/includes/fields/file_attachment.php';
	include_once QW_PLUGIN_DIR . '/includes/fields/image_attachment.php';
	include_once QW_PLUGIN_DIR . '/includes/fields/featured_image.php';
	include_once QW_PLUGIN_DIR . '/includes/fields/callback_field.php';
	include_once QW_PLUGIN_DIR . '/includes/fields/taxonomy_terms.php';

	// meta value field as a setting
	if ( $settings->get( 'meta_value_field_handler', 0 ) ) {
		include_once QW_PLUGIN_DIR . '/includes/fields/meta_value_new.php';
	}
	else {
		include_once QW_PLUGIN_DIR . '/includes/fields/meta_value.php';
	}

	// filters
	include_once QW_PLUGIN_DIR . '/includes/filters/author.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/callback.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/post_types.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/post_id.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/meta_key.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/meta_key_value.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/meta_query.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/meta_value.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/tags.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/categories.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/post_parent.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/taxonomies.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/taxonomy_relation.php';
	include_once QW_PLUGIN_DIR . '/includes/filters/search.php';

	// sorts
	include_once QW_PLUGIN_DIR . '/includes/sorts/default_sorts.php';

	// overrides
	include_once QW_PLUGIN_DIR . '/includes/overrides/categories.php';
	include_once QW_PLUGIN_DIR . '/includes/overrides/post_type_archive.php';
	include_once QW_PLUGIN_DIR . '/includes/overrides/tags.php';
	include_once QW_PLUGIN_DIR . '/includes/overrides/taxonomies.php';

	// Necessary functions to show a query
	include_once QW_PLUGIN_DIR . '/includes/query.php';
	include_once QW_PLUGIN_DIR . '/includes/theme.php';
	include_once QW_PLUGIN_DIR . '/includes/pages.php';
	include_once QW_PLUGIN_DIR . '/includes/class-qw-override.php';
	QW_Override::register();

}

function qw_admin_init() {
	$settings = QW_Settings::get_instance();

	if ( $settings->get( 'qw_live_preview', FALSE ) === FALSE ) {
		add_option( 'qw_live_preview', 'on' );
	}

	include_once QW_PLUGIN_DIR . '/admin/admin.php';
	include_once QW_PLUGIN_DIR . '/admin/query-admin-pages.php';
	include_once QW_PLUGIN_DIR . '/admin/ajax.php';
	include_once QW_PLUGIN_DIR . '/admin/default_editors.php';

	//add_action( 'wp_ajax_nopriv_qw_form_ajax', 'qw_form_ajax' );
	add_action( 'wp_ajax_qw_form_ajax', 'qw_form_ajax' );
	add_action( 'wp_ajax_qw_data_ajax', 'qw_data_ajax' );
	add_action( 'admin_head', 'qw_admin_css' );

	// js
	if ( isset( $_GET['page'] ) && $_GET['page'] == 'query-wrangler' ) {
		// edit page & not on export page
		if ( ! empty( $_GET['edit'] ) &&
		     empty( $_GET['export'] )
		) {
			add_filter( 'wp_enqueue_scripts', 'qw_admin_js', 0 );
			add_action( 'admin_enqueue_scripts', 'qw_admin_js' );
			qw_init_edit_theme();
		}

		// list page
		if ( empty( $_GET['edit'] ) ) {
			add_action( 'admin_enqueue_scripts', 'qw_admin_list_js' );
		}
	}
}

add_action( 'init', 'qw_init_frontend' );
add_action( 'admin_menu', 'qw_menu', 9999 );
add_action( 'admin_init', 'qw_admin_init' );
add_action( 'admin_init', 'qw_check_version', 901 );

/*
 * All my hook_menu implementations
 */
function qw_menu() {
	global $menu;
	// get the first available menu placement around 30, trivial, I know
	$menu_placement = 1000;
	for ( $i = 30; $i < 100; $i ++ ) {
		if ( ! isset( $menu[ $i ] ) ) {
			$menu_placement = $i;
			break;
		}
	}
	// http://codex.wordpress.org/Function_Reference/add_menu_page
	$list_page = add_menu_page( 'Query Wrangler',
		'Query Wrangler',
		'manage_options',
		'query-wrangler',
		'qw_page_handler',
		'',
		$menu_placement );
	// http://codex.wordpress.org/Function_Reference/add_submenu_page
	$create_page = add_submenu_page( 'query-wrangler',
		'Create New Query',
		'Add New',
		'manage_options',
		'qw-create',
		'qw_create_query_page' );
	$import_page = add_submenu_page( 'query-wrangler',
		'Import',
		'Import',
		'manage_options',
		'qw-import',
		'qw_import_page' );
	$settings_page = add_submenu_page( 'query-wrangler',
		'Settings',
		'Settings',
		'manage_options',
		'qw-settings',
		'qw_settings_page' );
	//$debug_page  = add_submenu_page( 'query-wrangler', 'Debug', 'Debug', 'manage_options', 'qw-debug', 'qw_debug');
}


/*===================================== DB TABLES =========================================*/
/*
 * Activation hooks for database tables
 */
function qw_query_wrangler_table() {
	global $wpdb;
	$table_name = $wpdb->prefix . "query_wrangler";
	$sql        = "CREATE TABLE " . $table_name . " (
id mediumint(9) NOT NULL AUTO_INCREMENT,
name varchar(255) NOT NULL,
slug varchar(255) NOT NULL,
type varchar(16) NOT NULL,
path varchar(255),
data text NOT NULL,
UNIQUE KEY id (id)
);";

	require_once( ABSPATH . 'wp-admin/includes/upgrade.php' );
	dbDelta( $sql );
}

register_activation_hook( __FILE__, 'qw_query_wrangler_table' );

// override terms table
function qw_query_override_terms_table() {
	global $wpdb;
	$table_name = $wpdb->prefix . "query_override_terms";
	$sql        = "CREATE TABLE " . $table_name . " (
query_id mediumint(9) NOT NULL,
term_id bigint(20) NOT NULL,
UNIQUE KEY query_term (query_id,term_id)
);";

	require_once( ABSPATH . 'wp-admin/includes/upgrade.php' );
	dbDelta( $sql );
}

register_activation_hook( __FILE__, 'qw_query_override_terms_table' );
