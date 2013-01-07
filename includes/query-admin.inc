<?php
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

  $options = $post['qw-query-options'];

  // hook for presave
  do_action_ref_array('qw_pre_save', array(&$options));

  $new_data = qw_serialize($options);
  $query_id = $post['query-id'];

  // update for pages
  if($post['qw-query-options']['display']['page']['path']){
    $page_path = ($post['qw-query-options']['display']['page']['path']) ? $post['qw-query-options']['display']['page']['path'] : '';

    // handle opening slash
    // checking against $wp_query->query['pagename'], so, no slash
    if(substr($page_path, 0, 1) == '/'){
      $page_path = ltrim($page_path, '/');
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
 * Default values for  new query
 *
 * @return array Default query settings
 */
function qw_default_query_data(){
  return array(
    'args' => array(
      'posts_per_page'  => 5,
      'offset'          => 0,
      'cat_operator'    => 'cat',
      'post_status'     => 'publish',
      'filters'         => array(),
      'sorts'           => array(),
    ),
    'display' => array(
      'style' => 'unformatted',
      'row_style' => 'posts',
      'post_settings' => array(
        'size' => 'complete',
      ),
    ),
  );
}
/*
 * Slug creation
 */
function qw_make_slug($string){
  $search = array("!","@","#","$","%","^","&","*","(",")","-","+","=","{","}","[","]","\\","|",":",";","'","<",",",">",".","?","/","~","`");
  return str_replace(" ", "_", strtolower(str_replace($search, "", strip_tags($string))));
}

/*
 * Custom for outputting text/html into a textarea
 */
function qw_textarea($value){
  return stripcslashes(esc_textarea(str_replace("\\", "", $value)));
}

/*
 * Run init_callback for all Edit Themes
 */
function qw_init_edit_theme(){
  $themes = qw_all_edit_themes();
  $current = get_option('qw_edit_theme');
  if(isset($themes[$current])){
    $theme = $themes[$current];
  } else {
    $theme = $themes[QW_DEFAULT_THEME];
  }

  if(function_exists($theme['init_callback'])){
    $theme['init_callback']();
  }
}

// CSS
function qw_admin_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/css/jquery-ui.css" />';
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/css/query-wrangler.css" />';
}
// admin list page
function qw_admin_list_js(){
  wp_enqueue_script('qw-admin-list-js',
                  plugins_url('/js/query-wrangler-list.js', dirname(__FILE__)),
                  array(),
                  QW_VERSION, true);
}
// Base JS
function qw_admin_js(){
  //// remove whatever wordpress is doing
  //wp_deregister_script('jquery');
  //wp_deregister_script('jquery-ui-core');
  //wp_deregister_script('jquery-ui-dialog');
  //wp_deregister_script('jquery-ui-accordion');
  //wp_deregister_script('jquery-ui-draggable');
  //wp_deregister_script('jquery-ui-droppable');
  //
  //// add a more controlled source
  //wp_register_script('jquery', ("http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"), false, '1.7.2');
  //wp_enqueue_script('jquery');
  //wp_register_script('jquery-ui-core', ("http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.24/jquery-ui.min.js"), false, '1.8.24');
  //wp_enqueue_script('jquery-ui-core');
  
  
  // jquery unserialize form
  wp_enqueue_script('qw-unserialize-form',
                  plugins_url('/js/jquery.unserialize-form.js', dirname(__FILE__)),
                  array(),
                  QW_VERSION, true);
  wp_enqueue_script('jquery-ui-core', false, array('jquery'));
  wp_enqueue_script('jquery-ui-accordion');
  wp_enqueue_script('jquery-ui-dialog');
  wp_enqueue_script('jquery-ui-sortable');
  wp_enqueue_script('qw-admin-js',
                  plugins_url('/js/query-wrangler.js', dirname(__FILE__)),
                  array('jquery-ui-core'),
                  QW_VERSION, true);
  
  $data = qw_edit_json();
  wp_localize_script( 'qw-admin-js',
                      'QueryWrangler',
                      array(
                        'l10n_print_after' => 'QueryWrangler = ' . $data. ';'
                      )
                    );
}
// Base Json data for query edit page
function qw_edit_json(){
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
    'allSortOptions' => qw_all_sort_options(),
  );

  // editing a query
  if($query_id = $_GET['edit'])
  {
    // get the query
    global $wpdb;
    $table_name = $wpdb->prefix."query_wrangler";
    $sql = "SELECT name,type,data,path FROM ".$table_name." WHERE id = ".$query_id." LIMIT 1";
    $row = $wpdb->get_row($sql);

    $additional_data['query'] = array(
      'id' => $query_id,
      'options' => qw_unserialize($row->data),
      'name' => $row->name,
      'type' => $row->type,
    );

    $data = array_merge($data, $additional_data);
  }
  return json_encode( $data ) ;
}


/*
 * Checking current version of plugin to handle upgrades
 */
function qw_check_version()
{
  if ($last_version = get_option('qw_plugin_version')) {
    // compare versions
    if ($last_version < QW_VERSION){
      // include upgrade inc
      include_once QW_PLUGIN_DIR.'/upgrade.php';
      $upgrade_function = 'qw_upgrade_'.qw_make_slug($last_version).'_to_'.qw_make_slug(QW_VERSION);
      $upgrade_to_current = 'qw_upgrade_'.qw_make_slug($last_version).'_to_current';

      if(function_exists($upgrade_function)){
        $upgrade_function();
      } else if(function_exists($upgrade_to_current)) {
        $upgrade_to_current();
      }
      update_option('qw_plugin_version', QW_VERSION);
    }
  }
  else {
    // first upgrade
    include QW_PLUGIN_DIR.'/upgrade.php';
    qw_upgrade_12_to_13();
    // set our version numer
    update_option('qw_plugin_version', QW_VERSION);
  }
}
/*
 * Organize an existing filters and give it all the data they needs
 *
 * @param $type
 *   handler-type = 'filter', 'field', 'sort'
 */
function qw_preprocess_handlers($type, $handlers){
  if(is_array($handlers))
  {
    // load all our default handlers
    switch ($type){
      case 'sort':
        $all = qw_all_sort_options();
        break;
      case 'field':
        $all = qw_all_fields();
        break;
      case 'filter':
        $all = qw_all_filters();
        break;
    }

    // generate the form name prefixes
    foreach($handlers as $name => $values)
    {

      // load sort type data
      $hook_key = qw_get_hook_key($all, $values);

      $this_handler = $all[$hook_key];

      // move type, hook_key, and weight to top level of array
      $this_handler['type'] = $values['type']; unset($values['type']);
      $this_handler['weight'] = $values['weight']; unset($values['weight']);
      $this_handler['hook_key'] = $values['hook_key']; unset($values['hook_key']);

      // values are own array
      $this_handler['values'] = $values;
      $this_handler['name'] = $name;
      $this_handler['form_prefix'] = qw_make_form_prefix($type, $name);

      // this handler's form
      if (function_exists($this_handler['form_callback'])){
        $this_handler['form'] = $this_handler['form_callback']($this_handler);
      }
      // provide template wrangler support
      else if (isset($this_handler['form_template'])){
        $this_handler['form'] = theme($this_handler['form_template'], array('this' => $this_handler));
      }

      // set new sort
      $handlers[$name] = $this_handler;
    }
    // sort sorts according to weight
    if(is_array($handlers)){
      uasort($handlers,'qw_cmp');
    }

    return $handlers;
  }
}