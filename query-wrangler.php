<?php
/*
Contributors: daggerhart, forrest.livengood
Plugin Name: Query Wrangler
Plugin URI: http://www.widgetwrangler.com/query-wrangler
Tags: query, pages, widget, admin, widgets, administration, manage, views
Author URI: http://www.websmiths.co
Author: Jonathan Daggerhart, Forrest Livengood
Donate link: http://www.widgetwrangler.com/
Requires at least: 3
Tested up to: 3.2.1
Stable tag: trunk
Version: 1.4.1beta
*/
// Note: There are 3 places to change the version number; below, above, and in readme.txt
define('QW_VERSION', 1.4);

/*  Copyright 2010  Jonathan Daggerhart  (email : jonathan@daggerhart.com)
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
*/

// some useful definitions
define('QW_PLUGIN_DIR', dirname(__FILE__));
define('QW_PLUGIN_URL', plugins_url( '', __FILE__ ));

// include Query Widgets functions
include_once QW_PLUGIN_DIR.'/query.inc';
// Theme functions
include_once QW_PLUGIN_DIR.'/theme.inc';
// Query page handling
include_once QW_PLUGIN_DIR.'/pages.inc';
// Wordpress hooks
include_once QW_PLUGIN_DIR.'/data.hooks.inc';
// Field and field style definitions
include_once QW_PLUGIN_DIR.'/data.defaults.inc';
// Filter definitions and functions
include_once QW_PLUGIN_DIR.'/data.default_filters.inc';
// Filters forms
include_once QW_PLUGIN_DIR.'/forms/filters.inc';
// Query Widget
include_once QW_PLUGIN_DIR.'/widget.query.php';

// include Template Wrangler
if(!function_exists('theme')){
  include_once QW_PLUGIN_DIR.'/template-wrangler.inc';
}

/*
 * Checking current version of plugin to handle upgrades
 */
function qw_check_version()
{
  if($last_version = get_option('qw_plugin_version')){

    // compare versions
    if ($last_version < QW_VERSION)
    {
      // include upgrade inc
      include_once QW_PLUGIN_DIR.'/upgrade.php';
      $upgrade_function = 'qw_upgrade_'.qw_make_slug($last_version).'_to_'.qw_make_slug(QW_VERSION);

      if(function_exists($upgrade_function)){
        $upgrade_function();
      }
      update_option('qw_plugin_version', QW_VERSION);
    }
  }
  else
  {
    // first upgrade
    include QW_PLUGIN_DIR.'/upgrade.php';
    qw_upgrade_12_to_13();
    // set our version numer
    update_option('qw_plugin_version', QW_VERSION);
  }
}
add_action('admin_init', 'qw_check_version');

/*
 * Ajax form templates
 */
function qw_form_ajax(){
  // field
  if($_POST['form'] == 'field_form'){
    $fields = qw_all_fields();
    $field = $fields[$_POST['type']];
    $field['name'] = $_POST['name'];
    // set new form_prefix
    $field['form_prefix'] = 'qw-query-options[display][field_settings][fields]['.$field['name'].']';

    $args = array(
      'image_sizes' => get_intermediate_image_sizes(),
      'file_styles' => qw_all_file_styles(),
      'field' => $field,
    );
    print theme('query_field', $args);
  }
  // sortable field
  else if($_POST['form'] == 'field_sortable'){
    $fields = qw_all_fields();
    $field = $fields[$_POST['type']];
    $field['name'] = $_POST['name'];
    // set new form_prefix
    $field['form_prefix'] = 'qw-query-options[display][field_settings][fields]['.$field['name'].']';

    $args = array(
      'field' => $field,
      'weight' => $_POST['next_weight'],
    );
    print theme('query_field_sortable', $args);
  }
  // filter
  else if($_POST['form'] == 'filter_form')
  {
    $all_filters = qw_all_filters();
    $filter = $all_filters[$_POST['type']];
    $filter['name'] = $_POST['name'];
    // set new form_prefix
    $filter['form_prefix'] = "qw-query-options[args][filters][".$filter['name']."]";

    // filter's form
    // expect returned output
    if (function_exists($filter['form_callback'])){
      $filter['form'] = $filter['form_callback']($filter);
    }
    // provide template wrangler support
    else if (isset($filter['template'])){
      $filter['form'] = theme($filter['template'], array('filter' => $filter));
    }

    $args = array(
      'filter' => $filter,
      'query_type' => $_POST['query_type'],
    );

    print theme('query_filter', $args);
  }
  // sortable filter
  else if($_POST['form'] == 'filter_sortable') {
    $all_filters = qw_all_filters();
    $filter = $all_filters[$_POST['type']];
    $filter['name'] = $_POST['name'];
    // set new form_prefix
    $filter['form_prefix'] = "qw-query-options[args][filters][".$filter['name']."]";

    $args = array(
      'filter' => $filter,
      'weight' => $_POST['next_weight'],
    );
    print theme('query_filter_sortable', $args);
  }
  // preview
  else if($_POST['form'] == 'preview') {
    $decode = urldecode($_POST['options']);
    $options = array();
    parse_str($decode, $options);
    $args = array(
      'options' => $options['qw-query-options']
    );
    print theme('query_preview', $args);
  }

  exit;
}
add_action( 'wp_ajax_nopriv_qw_form_ajax', 'qw_form_ajax' );
add_action( 'wp_ajax_qw_form_ajax', 'qw_form_ajax' );

/*
 * Javascript for query page
 */
function qw_admin_js(){
  // my js script
  wp_enqueue_script('qw-admin-js',
                  plugins_url('/js/query-wrangler.js', __FILE__ ),
                  array('jquery-ui-core', 'jquery-ui-sortable'),
                  false,
                  true);
  // jquery unserialize form
  wp_enqueue_script('qw-unserialize-form',
                  plugins_url('/js/jquery.unserialize-form.js', __FILE__ ),
                  array('jquery-ui-core', 'jquery-ui-sortable'),
                  false,
                  true);
  // declare the URL to the file that handles the AJAX request (wp-admin/admin-ajax.php)

  // @TODO: make LOTS of things available as js objects.  almost everything
  $data = array(
    'ajaxForm' => admin_url( 'admin-ajax.php' ),
    'allFields' => qw_all_fields(),
    'allStyles' => qw_all_styles(),
    'allRowStyles' => qw_all_row_styles(),
    'allPostTypes' => qw_all_post_types,
    'allPagerTypes' => qw_all_pager_types(),
    'allImageSizes' => get_intermediate_image_sizes(),
    'allFileStyles' => qw_all_file_styles(),
    'allFilters'  => qw_all_filters(),
  );

  // editing a query
  if($query_id = $_GET['edit'])
  {
    // get the query
    global $wpdb;
    $table_name = $wpdb->prefix."query_wrangler";
    $sql = "SELECT name,type,data,path FROM ".$table_name." WHERE id = ".$query_id." LIMIT 1";
    $row = $wpdb->get_row($sql);

    $additional_data ['query'] = array(
      'id' => $query_id,
      'options' => qw_unserialize($row->data),
      'name' => $row->name,
      'type' => $row->type,
    );

    $data = array_merge($data, $additional_data);
  }

  wp_localize_script( 'qw-admin-js',
                      'QueryWrangler',
                      array(
                        'l10n_print_after' => 'QueryWrangler = ' . json_encode( $data ) . ';'
                      )
                    );
}
// Add js
if($_GET['page'] == 'query-wrangler'){
  add_action('admin_enqueue_scripts', 'qw_admin_js');
}

/*
 * Add css to admin interface
 */
function qw_admin_css(){
	print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/query-wrangler.css" />';
}
add_action('admin_head', 'qw_admin_css');

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


/*
 * All my hook_menu implementations
 */
function qw_menu()
{
  global $menu;
  // get the first available menu placement around 30, trivial, I know
  $menu_placement = 1000;
  for($i=30;$i<100;$i++){
    if(!isset($menu[$i])){
      $menu_placement = $i;
      break;
    }
  }
  // http://codex.wordpress.org/Function_Reference/add_menu_page
  $list_page    = add_menu_page( 'Query Wrangler', 'Query Wrangler', 'manage_options', 'query-wrangler', 'qw_page_handler', '', $menu_placement);
  // http://codex.wordpress.org/Function_Reference/add_submenu_page
  $create_page  = add_submenu_page( 'query-wrangler', 'Create New Query', 'Add New', 'manage_options', 'qw-create', 'qw_create_query');
  $import_page  = add_submenu_page( 'query-wrangler', 'Import', 'Import', 'manage_options', 'qw-import', 'qw_import_page');
  //$debug_page  = add_submenu_page( 'query-wrangler', 'Debug', 'Debug', 'manage_options', 'qw-debug', 'qw_debug');
}
// add menu very last so we don't get replaced by another menu item
add_action( 'admin_menu', 'qw_menu', 9999);

/*
 * Simple debugging location
 */

// */
/*
 * Export a query into code
 * @param
 *   $query_id - the query's id number
 */
function qw_query_export($query_id){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name,slug,type,path,data FROM ".$table_name." WHERE id = ".$query_id;

  $row = $wpdb->get_row($sql, ARRAY_A);
  unset($row['id']);
  // unserealize the stored data
  $row['data'] = qw_unserialize($row['data']);
  $export = var_export($row,1);

  return "\$query = ".$export.";";
}
/*
 * Import a query into the database
 *
 */
function qw_query_import($post){
  global $wpdb;
  $table = $wpdb->prefix."query_wrangler";

  eval(stripslashes($post['import-query']));

  if($post['import-name']){
    $query['name'] = $post['import-name'];
    $query['slug'] = qw_make_slug($post['import-name']);
  }
  $query['data'] = qw_serialize($query['data']);
  $wpdb->insert($table, $query);
  return $wpdb->insert_id;
}
/*
 * Handle the display of pages and actions
 */
function qw_page_handler(){

  // handle actions
  if(isset($_GET['action']))
  {
    $redirect = true;
    switch($_GET['action'])
    {
      case 'update':
        qw_update_query($_POST);
        // redirect to the edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$_GET['edit']);
        break;

      case 'delete':
        qw_delete_query($_GET['edit']);
        // redirect to the list page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler');
        break;

      case 'create':
        $new_query_id = qw_insert_new_query($_POST);
        // forward to the edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$new_query_id);
        break;

      case 'import':
        $new_query_id = qw_query_import($_POST);
        // forward to edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$new_query_id);
        break;
    }
  }

  // see if we're editng a page
  if(isset($_GET['edit']) &&
     is_numeric($_GET['edit']) &&
     !$redirect)
  {
    // show edit form
    qw_edit_query_form();
  }
  // export a query
  else if ($_GET['export'] && is_numeric($_GET['export'])){
    qw_export_page();
  }
  // else we need a list of queries
  else {
    include QW_PLUGIN_DIR.'/forms/form.query-list.inc';
    qw_list_queries_form();
  }
}
/*
 * Export Query page
 */
function qw_export_page(){
  global $wpdb;
  $table = $wpdb->prefix.'query_wrangler';
  $row = $wpdb->get_row('SELECT name FROM '.$table.' WHERE id = '.$_GET['export']);

  $args = array(
    'title' => 'Export Query: <em>'.$row->name.'</em>',
    'content' => theme('query_export', array('query_id' => $_GET['export'])),
  );
  print theme('admin_wrapper', $args);
}
/*
 * Import Query Page
 */
function qw_import_page(){
  // show import page
  $args = array(
    'title' => 'Import Query',
    'content' => theme('query_import'),
  );
  print theme('admin_wrapper', $args);
}
/*
 * Create Query Page
 */
function qw_create_query() {
  $args = array(
    'title' => 'Create Query',
    'content' => theme('query_create')
  );

  print theme('admin_wrapper', $args);
}
/*
 * Organize an existing filters and give it all the data it needs
 */
function qw_preprocess_filters($filters){
  if(is_array($filters)){
    $all_filters = qw_all_filters();

    foreach($filters as $filter_name => $filter_values){
      $filter = $all_filters[$filter_values['type']];
      unset($filter_values['type']);
      $filter['weight'] = $filter_values['weight'];
      unset($filter_values['weight']);

      $filter['values'] = $filter_values;
      $filter['name'] = $filter_name;
      // generate the form name prefixes
      $filter['form_prefix'] = "qw-query-options[args][filters][".$filter_name."]";

      // filter's form
      // expect returned output
      if (function_exists($filter['form_callback'])){
        $filter['form'] = $filter['form_callback']($filter);
      }
      // provide template wrangler support
      else if (isset($filter['template'])){
        $filter['form'] = theme($filter['template'], array('filter' => $filter));
      }

      $filters[$filter_name] = $filter;
    }
    // sort filters according to weight
    if(is_array($filters)){
      uasort($filters,'qw_cmp');
    }
    return $filters;
  }
}

/*
 * Organize an existing filters and give it all the data it needs
 */
function qw_preprocess_fields($fields){
  if(is_array($fields)){
    $all_fields = qw_all_fields();

    // generate the form name prefixes
    foreach($fields as $field_name => $field_values){
      // load field type data
      $field = $all_fields[$field_values['type']];
      // move type and weight to top level of array
      $field['type'] = $field_values['type'];
      unset($field_values['type']);
      $field['weight'] = $field_values['weight'];
      unset($field_values['weight']);
      // values are own array
      $field['values'] = $field_values;
      // name and prefix for forms
      $field['name'] = $field_name;
      $field['form_prefix'] = "qw-query-options[display][field_settings][fields][".$field_name."]";
      // set new field
      $fields[$field_name] = $field;
    }
    // sort fields according to weight
    if(is_array($fields)){
      uasort($fields,'qw_cmp');
    }
    return $fields;
  }
}
/*
 * Query Edit Page
 */
function qw_edit_query_form()
{
  if($_GET['edit'])
  {
    $query_id = $_GET['edit'];
    // get the query
    global $wpdb;
    $table_name = $wpdb->prefix."query_wrangler";
    $sql = "SELECT name,type,data,path FROM ".$table_name." WHERE id = ".$query_id." LIMIT 1";
    $row = $wpdb->get_row($sql);
    $post_types = qw_all_post_types();

    $options = qw_unserialize($row->data);

    $all_filters = qw_all_filters();
    $all_fields = qw_all_fields();

    // existing filters
    $filters = qw_preprocess_filters($options['args']['filters']);
    // existing fields
    $fields = qw_preprocess_fields($options['display']['field_settings']['fields']);

    $sort_orders = array(
      'ASC' => array(
        'type' => 'ASC',
        'title' => 'Ascending',
      ),
      'DESC' => array(
        'type' => 'DESC',
        'title' => 'Descending',
      )
    );

    // start building edit page data
    $edit_args = array(
      'query_id' => $query_id,
      'options' => $options,
      'args'    => $options['args'],
      'display' => $options['display'],
      'query_name' => $row->name,
      'query_type' => $row->type,
      // all filters
      'all_filters' => $all_filters,
      // existing filters
      'filters' => $filters,
      // all qw fields
      'all_fields' => $all_fields,
      // existing fields
      'fields' => $fields,
      // post statuses
      'post_statuses' => qw_all_post_statuses(),
      // categories
      'category_ids' => get_all_category_ids(),
      // tags
      'tags' => get_tags(array('hide_empty' => false)),
      // image sizes
      'image_sizes' => get_intermediate_image_sizes(),
      // file styles
      'file_styles' => qw_all_file_styles(),
      // all qw styles
      'styles' => qw_all_styles(),
      // all qw row styles
      'row_styles' => qw_all_row_styles(),
      'row_complete_styles' => qw_all_row_complete_styles(),
      'sort_options' => qw_all_sort_options(),
      'sort_orders' => $sort_orders,
      // all WP post types available for QWing
      'post_types' => $post_types,
      // all Pager Types
      'pager_types' => qw_all_pager_types(),
    );

    // query title
    $edit_args['query_page_title'] = $edit_args['options']['display']['title'];

    // sort fields according to weight
    if(is_array($edit_args['options']['display']['field_settings']['fields'])){
      uasort($edit_args['options']['display']['field_settings']['fields'],'qw_cmp');
    }

    // overrides
    if($row->type == 'override'){
      $edit_args['query_override_type'] = $row->override_type;
    }

    // Page Queries
    if($row->type == 'page'){
      $edit_args['query_page_path'] = $row->path;
      $edit_args['page_templates'] = get_page_templates();
    }

    // admin wrapper arguments
    $admin_args = array(
      'title' => 'Edit query <em>'.$edit_args['query_name'].'</em>',
      // content is the query_edit page
      'content' => theme('query_edit', $edit_args)
    );

    // add view link for pages
    if($row->type == 'page' && isset($row->path)){
      $admin_args['title'].= ' <a class="add-new-h2" target="_blank" href="'.get_bloginfo('wpurl').$row->path.'">View</a>';
    }

    // include the edit form
    print theme('admin_wrapper', $admin_args);
  }
}

/*
 * Create the new Query
 *
 * @param $_POST data
 * @return int New Query ID
 */
function qw_insert_new_query($post){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";

  $values = array(
    'name' => $post['qw-name'],
    'slug' => qw_make_slug($post['qw-name']),
    'type' => $post['qw-type'],
    'path' => ($post['page-path']) ? urlencode($post['page-path']) : NULL,
    'data' => qw_serialize(qw_default_query_data()),
  );

  $wpdb->insert($table_name, $values);
  return $wpdb->insert_id;
}

/*
 * Update existing query
 *
 * @param $_POST data
 */
function qw_update_query($post){

  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";

  // if you can't tell, i'm having a lot of trouble with slashes
  $post = array_map( 'stripslashes_deep', $post );

  // look for obvious errors
  if(empty($post['qw-query-options']['args']['posts_per_page'])){
    $post['qw-query-options']['args']['posts_per_page'] = 5;
  }
  if(empty($post['qw-query-options']['args']['offset'])){
    $post['qw-query-options']['args']['offset'] = 0;
  }
  if(empty($post['qw-query-options']['args']['post_status'])){
    $post['qw-query-options']['args']['post_status'] = 'publish';
  }

  // handle page settings
  if(isset($post['qw-query-options']['display']['page']['template-file']))
  {
    // handle template name
    if($post['qw-query-options']['display']['page']['template-file'] == 'index.php'){
      $post['qw-query-options']['display']['page']['template-name'] = 'Default';
    }
    else {
      $page_templates = get_page_templates();
      foreach($page_templates as $name => $file){
        if($post['qw-query-options']['display']['page']['template-file'] == $file){
          $post['qw-query-options']['display']['page']['template-name'] = $name;
        }
      }
    }
  }

  $new_data = qw_serialize($post['qw-query-options']);
  $query_id = $post['query-id'];

  // update for pages
  if($post['qw-query-options']['display']['page']['path']){
    $page_path = ($post['qw-query-options']['display']['page']['path']) ? $post['qw-query-options']['display']['page']['path'] : '';

    // handle opening slash
    // we check against REQUEST_URI, so it's easier with the slash
    if(substr($page_path, 0, 1) != '/'){
      $page_path = '/'.$page_path;
    }

    $sql = "UPDATE ".$table_name." SET data = '".$new_data."', path = '".$page_path."' WHERE id = ".$query_id;
  }

  // update for widgets
  else {
    $sql = "UPDATE ".$table_name." SET data = '".$new_data."' WHERE id = ".$query_id;
  }
  $wpdb->query($sql);

  // addition override work
  if(is_array($post['qw-query-options']['override']))
  {
    $terms = array();
    // merge categories
    if(is_array($post['qw-query-options']['override']['cats'])){
      $terms = array_merge($terms, array_keys($post['qw-query-options']['override']['cats']));
    }
    // merge tags
    if(is_array($post['qw-query-options']['override']['tags'])){
      $terms = array_merge($terms, array_keys($post['qw-query-options']['override']['tags']));
    }

    // delete all existing relationships
    $table = $wpdb->prefix."query_override_terms";
    $sql = "DELETE FROM ".$table." WHERE query_id = ".$query_id;
    $wpdb->query($sql);

    $data = array('query_id' => $query_id);
    // loop through all terms and insert them
    foreach($terms as $term_id){
      $data['term_id'] = $term_id;
      $wpdb->insert($table, $data);
    }
  }

  // send back to edit page
  wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$query_id);
}
/*
 * Delete an existing query
 *
 * @param query id
 */
function qw_delete_query($query_id){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "DELETE FROM ".$table_name." WHERE id = ".$query_id;
  $wpdb->query($sql);
}

/*
 * Shortcode support for all queries
 */
function qw_single_query_shortcode($atts) {
  $short_array = shortcode_atts(array('id' => ''), $atts);
  extract($short_array);

  // get the query options
  $options = qw_generate_query_options($id);

  // get formatted query arguments
  $args = qw_generate_query_args($options);

  // set the new query
  $wp_query = new WP_Query($args);

  // get the themed content
  $themed = qw_template_query($wp_query, $options);
  // reset because worpress hates programmers
  wp_reset_postdata();
  return $themed;
}
add_shortcode('query','qw_single_query_shortcode');

/*
 * Debug functions for delaying messages until output
 *
function qwdm($var){
  if (function_exists('krumo')){
    ob_start();
      krumo($var);
    $output = ob_get_clean();
  }
  else {
    $output = print_r($var,1);
  }
  $next = count($_SESSION['messages']);
  $backtrace = debug_backtrace();
  $output_bt = 'Called from '.$backtrace[0]['file'].', line '.$backtrace[0]['line'];
  $_SESSION['messages'][$next]['output'] = $output;
  $_SESSION['messages'][$next]['backtrace'] = $output_bt;
}
function qw_delayed_messages(){
  if (is_array($_SESSION['messages'])){
    foreach($_SESSION['messages'] as $m){
      print '<pre>'.$m['backtrace'].'</pre>';
      print $m['output'];
    }
  }
  $_SESSION['messages'] = array();
}
// */