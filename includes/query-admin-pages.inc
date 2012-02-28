<?php
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

      case 'save_settings':
        qw_save_settings($_POST);
        // forward to edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=qw-settings');
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
    include QW_PLUGIN_DIR.'/forms/query-list.inc';
    qw_list_queries_form();
  }
}

/*
 * Query Edit Page
 */
function qw_edit_query_form(){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $query_id = $_GET['edit'];
  $sql = "SELECT name,type,data,path FROM ".$table_name." WHERE id = ".$query_id." LIMIT 1";
  $row = $wpdb->get_row($sql);

  $options = qw_unserialize($row->data);

  // preprocess existing handlers
  $filters = qw_preprocess_handlers('filter', $options['args']['filters']);
  $fields  = qw_preprocess_handlers('field', $options['display']['field_settings']['fields']);
  $sorts   = qw_preprocess_handlers('sort', $options['args']['sorts']);

  $theme = get_option('qw_edit_theme');
  if($theme == ""){
    $theme = QW_DEFAULT_THEME;
  }
  // start building edit page data
  $edit_args = array(
    // editor theme
    'theme' => $theme,
    // query data
    'query_id' => $query_id,
    'options' => $options,
    'args'    => $options['args'],
    'display' => $options['display'],
    'query_name' => $row->name,
    'query_type' => $row->type,
    'query_page_title' => $options['display']['title'],
    'filters' => $filters,
    'fields' => $fields,
    'sorts' => $sorts,

    // all datas
    'post_statuses' => qw_all_post_statuses(),
    'styles' => qw_all_styles(),
    'row_styles' => qw_all_row_styles(),
    'row_complete_styles' => qw_all_row_complete_styles(),
    'page_templates' => get_page_templates(),
    'post_types' => qw_all_post_types(),
    'pager_types' => qw_all_pager_types(),
    'category_ids' => get_all_category_ids(),
    'tags' => get_tags(array('hide_empty' => false)),
    'all_filters' => qw_all_filters(),
    'all_fields' => qw_all_fields(),
    'all_sorts' => qw_all_sort_options(),
  );

  // Page Queries
  if($row->type == 'page'){
    $edit_args['query_page_path'] = $row->path;
  }

  // overrides
  if($row->type == 'override'){
    $edit_args['query_override_type'] = $row->override_type;
  }

  // admin wrapper arguments
  $admin_args = array(
    'title' => 'Edit query <em>'.$edit_args['query_name'].'</em>',
    // content is the query_edit page
    'content' => theme('query_edit', $edit_args)
  );

  // add view link for pages
  if($row->type == 'page' && isset($row->path)){
    $admin_args['title'].= ' <a class="add-new-h2" target="_blank" href="'.get_bloginfo('wpurl').'/'.$row->path.'">View</a>';
  }

  // include the edit form
  print theme('admin_wrapper', $admin_args);
}

/*
 * Settings!
 */
function qw_save_settings($post){
  update_option('qw_edit_theme', $post['qw-theme']);
}
function qw_settings_page()
{
  $settings_args = array(
    'theme' => get_option('qw_edit_theme'),
    'edit_themes' => qw_all_edit_themes(),
  );
  $args = array(
    'title' => 'Query Wrangler Settings',
    'content' => theme('query_settings', $settings_args)
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