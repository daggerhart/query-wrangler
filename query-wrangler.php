<?php
/*

******************************************************************

Contributors:      daggerhart
Plugin Name:       Query Wrangler
Plugin URI:        http://www.widgetwrangler.com/query-wrangler
Description:       Query Wrangler provides an intuitive interface for creating complex WP queries as pages or widgets. Based on Drupal Views.
Author:            Jonathan Daggerhart
Author URI:        http://www.websmiths.co
Version:           1.5.23

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
define('QW_VERSION', 1.5);
define('QW_PLUGIN_DIR', dirname(__FILE__));
define('QW_PLUGIN_URL', plugins_url( '', __FILE__ ));
define('QW_DEFAULT_THEME', 'views');

// Query Widget
include_once QW_PLUGIN_DIR.'/widget.query.php';

/*
 * Init functions
 */
function qw_init_frontend(){
  // include Template Wrangler
  if(!function_exists('theme')){
    include_once QW_PLUGIN_DIR.'/template-wrangler.inc';
  }
  // Wordpress hooks
  include_once QW_PLUGIN_DIR.'/includes/hooks.inc';
  include_once QW_PLUGIN_DIR.'/includes/exposed.inc';
  include_once QW_PLUGIN_DIR.'/includes/handlers.inc';
  
  //include_once QW_PLUGIN_DIR.'/includes/data.defaults.inc';
  
  //include_once QW_PLUGIN_DIR.'/includes/data.default_basics.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/display_title.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/template_styles.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/row_styles.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/posts_per_page.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/post_status.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/offset.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/header.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/footer.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/empty.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/wrapper_settings.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/page_path.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/page_template.inc';
  include_once QW_PLUGIN_DIR.'/includes/basics/pager.inc';
  
  include_once QW_PLUGIN_DIR.'/includes/fields/default_fields.inc';
  include_once QW_PLUGIN_DIR.'/includes/fields/post_author.inc';
  include_once QW_PLUGIN_DIR.'/includes/fields/post_author_avatar.inc';
  include_once QW_PLUGIN_DIR.'/includes/fields/file_attachment.inc';
  include_once QW_PLUGIN_DIR.'/includes/fields/image_attachment.inc';
  include_once QW_PLUGIN_DIR.'/includes/fields/meta_value.inc';
  include_once QW_PLUGIN_DIR.'/includes/fields/featured_image.inc';
  
  //include_once QW_PLUGIN_DIR.'/includes/data.default_filters.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/author.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/post_types.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/post_id.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/meta_key.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/meta_key_value.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/meta_value.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/tags.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/categories.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/post_parent.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/taxonomies.inc';
  include_once QW_PLUGIN_DIR.'/includes/filters/taxonomy_relation.inc';
  
  include_once QW_PLUGIN_DIR.'/includes/sorts/default_sorts.inc';
  
  // Necessary functions to show a query
  include_once QW_PLUGIN_DIR.'/includes/query.inc';
  include_once QW_PLUGIN_DIR.'/includes/theme.inc';
  include_once QW_PLUGIN_DIR.'/includes/pages.inc';
  
}
function qw_init(){
  qw_init_frontend();

  // admin only
  if(is_admin())
  {
    if (get_option('qw_live_preview') === FALSE){
      add_option('qw_live_preview', 'on');
    }
    include_once QW_PLUGIN_DIR.'/admin/query-admin.inc';
    include_once QW_PLUGIN_DIR.'/admin/query-admin-pages.inc';
    include_once QW_PLUGIN_DIR.'/admin/ajax.inc';
    include_once QW_PLUGIN_DIR.'/admin/default_editors.inc';

    add_action( 'wp_ajax_nopriv_qw_form_ajax', 'qw_form_ajax' );
    add_action( 'wp_ajax_qw_form_ajax', 'qw_form_ajax' );

    // js
    if(isset($_GET['page']) && $_GET['page'] == 'query-wrangler')
    {
      // edit page & not on export page
      if(!empty($_GET['edit']) &&
         empty($_GET['export']))
      {
        add_filter( 'wp_enqueue_scripts', 'qw_admin_js', 0 );
        add_action( 'admin_enqueue_scripts', 'qw_admin_js' );
        qw_init_edit_theme();
      }

      // list page
      if(empty($_GET['edit'])){
        add_action( 'admin_enqueue_scripts', 'qw_admin_list_js' );
      }
    }
  }
}

if (!is_admin()){
  add_action('init', 'qw_init');
}
add_action('admin_init', 'qw_init', 900);
add_action('admin_init', 'qw_check_version', 901); // in query-admin.php
add_action('admin_head', 'qw_admin_css'); // in query-admin.php
// add menu very last so we don't get replaced by another menu item
add_action( 'admin_menu', 'qw_menu', 9999);

/*
 * All my hook_menu implementations
 */
function qw_menu()
{
  global $menu;
  // get the first available menu placement around 30, trivial, I know
  $menu_placement = 1000;
  for($i=30;$i<100;$i++){
    if(!isset($menu[$i])){ $menu_placement = $i; break; }
  }
  // http://codex.wordpress.org/Function_Reference/add_menu_page
  $list_page    = add_menu_page( 'Query Wrangler', 'Query Wrangler', 'manage_options', 'query-wrangler', 'qw_page_handler', '', $menu_placement);
  // http://codex.wordpress.org/Function_Reference/add_submenu_page
  $create_page  = add_submenu_page( 'query-wrangler', 'Create New Query', 'Add New', 'manage_options', 'qw-create', 'qw_create_query');
  $import_page  = add_submenu_page( 'query-wrangler', 'Import', 'Import', 'manage_options', 'qw-import', 'qw_import_page');
  $settings_page= add_submenu_page( 'query-wrangler', 'Settings', 'Settings', 'manage_options', 'qw-settings', 'qw_settings_page');
  //$debug_page  = add_submenu_page( 'query-wrangler', 'Debug', 'Debug', 'manage_options', 'qw-debug', 'qw_debug');
}

/*
 * Shortcode support for all queries
 */
function qw_single_query_shortcode($atts) {
  $short_array = shortcode_atts(array('id' => '', 'slug' => '', 'args' => ''), $atts);
  extract($short_array);
  
  if (!$id && $slug){
    $id = qw_get_query_by_slug($slug);
  }
  
  $options_override = array();
  if (isset($args) && !empty($args)){
    $options_override['shortcode_args'] = html_entity_decode($args);
  }
  
  $themed = qw_execute_query($id, $options_override);
  return $themed;
}
add_shortcode('query','qw_single_query_shortcode');

/*===================================== DB TABLES =========================================*/
/*
 * Activation hooks for database tables
 */
function qw_query_wrangler_table(){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "CREATE TABLE " . $table_name . " (
	 id mediumint(9) NOT NULL AUTO_INCREMENT,
   name varchar(255) NOT NULL,
   slug varchar(255) NOT NULL,
   type varchar(16) NOT NULL,
   path varchar(255),
	  data text NOT NULL,
	  UNIQUE KEY id (id)
	);";

  require_once(ABSPATH . 'wp-admin/includes/upgrade.php');
  dbDelta($sql);
}
register_activation_hook(__FILE__,'qw_query_wrangler_table');

// override terms table
function qw_query_override_terms_table(){
  global $wpdb;
  $table_name = $wpdb->prefix."query_override_terms";
  $sql = "CREATE TABLE " . $table_name . " (
	  query_id mediumint(9) NOT NULL,
   term_id bigint(20) NOT NULL
	);";

  require_once(ABSPATH . 'wp-admin/includes/upgrade.php');
  dbDelta($sql);
}
register_activation_hook(__FILE__,'qw_query_override_terms_table');
/*/
function _d($v){
  print '<hr/><pre>'.print_r($v,1).'</pre>';
}
// */