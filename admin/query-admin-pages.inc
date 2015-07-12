<?php
/*
 * Handle the display of pages and actions
 */
function qw_page_handler(){
  $redirect = false;
  // handle actions
  if(isset($_GET['action']))
  {
    $redirect = true;
    switch($_GET['action'])
    {
      case 'update':
        qw_update_query($_POST);
        // redirect to the edit page
        qw_admin_redirect( $_GET['edit'] );
        break;

      case 'delete':
        qw_delete_query($_GET['edit']);
        // redirect to the list page
        qw_admin_redirect();
        break;

      case 'create':
        $new_query_id = qw_insert_new_query($_POST);
        // forward to the edit page
        qw_admin_redirect($new_query_id);
        break;

      case 'import':
        $new_query_id = qw_query_import($_POST);
        // forward to edit page
        qw_admin_redirect($new_query_id);
        break;

      case 'save_settings':
        qw_save_settings($_POST);
        // forward to edit page
        qw_admin_redirect( NULL, 'qw-settings');
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
  else if (isset($_GET['export']) && is_numeric($_GET['export'])){
    qw_export_page();
  }
  // else we need a list of queries
  else {
    include QW_PLUGIN_DIR . '/admin/templates/page-query-list.inc';
    qw_list_queries_form();
  }
}


/**
 * Simple redirect helper
 *
 * @param null $query_id
 * @param string $page
 */
function qw_admin_redirect( $query_id = NULL, $page = 'query-wrangler' ) {
  $url = admin_url("admin.php?page=$page");

  if ( $query_id ){
    $url.= "&edit=".(int)$query_id;
  }
  wp_redirect($url);
  exit();
}

/*
 * Query Edit Page
 */
function qw_edit_query_form(){
  if ( $query_id = qw_admin_get_current_query_id() ){
    $row = qw_get_query_by_id( $query_id );
  }
  if ( !$row ){ return; }

  $options = $row->data;
  $display = array_map('stripslashes_deep',$options['display']);
  $image_sizes = get_intermediate_image_sizes();
  $file_styles = qw_all_file_styles();

  $theme = get_option('qw_edit_theme', QW_DEFAULT_THEME);
  if($theme == ""){
    $theme = QW_DEFAULT_THEME;
  }

  // preprocess existing handlers
  $handlers = qw_preprocess_handlers($options);

  // go ahead and make existing items wrapper forms
  // filters
  foreach($handlers['filter']['items'] as $k => &$filter){
    $args = array(
      'filter' => $filter,
      'weight' => $filter['weight'],
    );
    $filter['wrapper_form'] = theme('query_filter', $args);
  }
  // sorts
  foreach($handlers['sort']['items'] as $k => &$sort){
    $args = array(
      'sort' => $sort,
      'weight' => $sort['weight'],
    );
    $sort['wrapper_form'] = theme('query_sort', $args);
  }

  $tokens = array();
  // fields
  foreach($handlers['field']['items'] as $k => &$field){
    $tokens[$field['name']] = '{{'.$field['name'].'}}';
    $args = array(
      'image_sizes' => $image_sizes,
      'file_styles' => $file_styles,
      'field' => $field,
      'weight' => $field['weight'],
      'options' => $options,
      'display' => $display,
      'tokens' => $tokens,
    );
    $field['wrapper_form'] = theme('query_field', $args);
  }

  // overrides
  foreach($handlers['override']['items'] as $k => &$override){
    $args = array(
      'override' => $override,
      'weight' => $override['weight'],
    );
    $override['wrapper_form'] = theme('query_override', $args);
  }

  // start building edit page data
  $edit_args = array(
    // editor theme
    'theme' => $theme,
    // query data
    'query_id'  => $row->id,
    'options'   => $options,
    'args'      => $options['args'],
    'display'   => $display,
    'query_name' => $row->name,
    'query_type' => $row->type,
    'query_page_title' => isset($options['display']['title']) ? $options['display']['title'] : '',
    'basics'    => qw_all_basic_settings(),
    'filters'   => $handlers['filter']['items'],
    'fields'    => $handlers['field']['items'],
    'sorts'     => $handlers['sort']['items'],
    'overrides' => $handlers['override']['items'],

    // all datas
    'post_statuses' => qw_all_post_statuses(),
    'styles'        => qw_all_styles(),
    'row_styles'    => qw_all_row_styles(),
    'row_complete_styles' => qw_all_row_complete_styles(),
    'page_templates'=> get_page_templates(),
    'post_types'    => qw_all_post_types(),
    'pager_types'   => qw_all_pager_types(),
    //'category_ids'  => get_terms('category', array('fields' => 'ids', 'hide_empty' => 0)),
    //'tags'          => get_tags(array('hide_empty' => false)),
    'all_overrides' => qw_all_overrides(),
    'all_filters'   => qw_all_filters(),
    'all_fields'    => qw_all_fields(),
    'all_sorts'     => qw_all_sort_options(),
    'image_sizes'   => $image_sizes,
    'file_styles'   => $file_styles,
  );

  // Page Queries
  if($row->type == 'page'){
    $edit_args['query_page_path'] = $row->path;
  }

  // overrides
  if($row->type == 'override'){
    $edit_args['query_override_type'] = $row->override_type;
  }

  $edit_wrapper_args = array(
    'query_id' => $row->id,
    'theme' => $theme,
    'editor' => theme('query_edit', $edit_args),
  );

  // admin wrapper arguments
  $admin_args = array(
    'title' => 'Edit query <em>'.$edit_args['query_name'].'</em>',
    'description' => '[query slug="'.$row->slug.'"]',
    // content is the query_edit page
    'content' => theme('query_edit_wrapper', $edit_wrapper_args )
  );

  // add view link for pages
  if($row->type == 'page' && isset($row->path)){
    $admin_args['title'].= ' <a class="add-new-h2" target="_blank" href="'.get_bloginfo('wpurl').'/'.$row->path.'">View</a>';
  }

  // include the edit form
  print theme('admin_wrapper', $admin_args);
}

/**
 * Settings!
 */
function qw_save_settings($post){
  $live_preview = (isset($post['qw-live-preview'])) ? $post['qw-live-preview']: 0;
  $show_silent_meta = (isset($post['qw-show-silent-meta'])) ? $post['qw-show-silent-meta']: 0;

  update_option('qw_edit_theme', $post['qw-theme']);
  update_option('qw_widget_theme_compat', $post['qw-widget-theme-compat']);
  update_option('qw_live_preview', $live_preview);
  update_option('qw_show_silent_meta', $show_silent_meta);
  update_option('qw_meta_value_field_handler', $post['qw-meta-value-field-handler']);
}
function qw_settings_page()
{
  $settings_args = array(
    'theme' => get_option('qw_edit_theme'),
    'edit_themes' => qw_all_edit_themes(),
    'widget_theme_compat' => get_option('qw_widget_theme_compat'),
    'live_preview' => get_option('qw_live_preview'),
    'show_silent_meta' => get_option('qw_show_silent_meta'),
    'meta_value_field_handler' => get_option('qw_meta_value_field_handler', 0 ),
    'meta_value_field_options' => array(
      0 => 'Default handler',
      1 => 'New handler (beta)',
    ),
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
function qw_create_query_page() {
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