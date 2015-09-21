<?php
/*
 * Default edit themes
 *
 *  template

    $themes['hook_key'] = array(
      // pretty theme name
      'title' => 'Nice Name for Theme',

      // callback for admin_init actions when theme is loaded
      // useful for loading js and css files
      'init_callback' => 'my_init_callback',
    );
 */
function qw_default_edit_themes( $themes ) {
//  $themes['picnic'] = array(
//    'title' => 'Picnic',
//    'init_callback' => 'qw_edit_theme_picnic',
//  );
	$themes['views'] = array(
		'title'         => 'Drupal Views',
		'init_callback' => 'qw_edit_theme_views',
	);

	return $themes;
}

// add default fields to the hook filter
add_filter( 'qw_edit_themes', 'qw_default_edit_themes', 0 );

/**
 * load jquery ui style from cdn
 */
function qw_load_jquery_ui_css_from_cdn() {
	global $wp_scripts;
	$ui       = $wp_scripts->query( 'jquery-ui-core' );
	$protocol = is_ssl() ? 'https' : 'http';
	$url      = "$protocol://ajax.googleapis.com/ajax/libs/jqueryui/{$ui->ver}/themes/smoothness/jquery-ui.min.css";
	wp_enqueue_style( 'jquery-ui-smoothness', $url, FALSE, NULL );
}

/*
 * Edit Theme: Drupal Views clone
 */
function qw_edit_theme_views() {
	qw_load_jquery_ui_css_from_cdn();

	add_action( 'admin_head', 'qw_edit_theme_views_css' );

	if ( $_GET['page'] == 'query-wrangler' ) {
		add_action( 'admin_enqueue_scripts', 'qw_edit_theme_views_js' );
	}
}

// views css
function qw_edit_theme_views_css() {
	print '<link rel="stylesheet" type="text/css" href="' . QW_PLUGIN_URL . '/admin/editors/views/views.css" />';
}

// views js
function qw_edit_theme_views_js() {
	wp_enqueue_script( 'qw-edit-theme-views',
		plugins_url( '/admin/editors/views/views.js', dirname( __FILE__ ) ),
		array(),
		QW_VERSION,
		TRUE );
}

/* --------------------------
 * Edit Theme: New editor with focus on division of "type" or choices
 */
function qw_edit_theme_picnic() {
	qw_load_jquery_ui_css_from_cdn();

	add_action( 'admin_head', 'qw_edit_theme_picnic_css' );

	if ( $_GET['page'] == 'query-wrangler' ) {
		add_action( 'admin_enqueue_scripts', 'qw_edit_theme_picnic_js' );
	}
}

// css
function qw_edit_theme_picnic_css() {
	print '<link rel="stylesheet" type="text/css" href="' . QW_PLUGIN_URL . '/admin/editors/picnic/picnic.css" />';
}

// js
function qw_edit_theme_picnic_js() {
	// my js script
	wp_enqueue_script( 'qw-edit-theme-new',
		plugins_url( '/admin/editors/picnic/picnic.js', dirname( __FILE__ ) ),
		array(),
		QW_VERSION,
		TRUE );
}
