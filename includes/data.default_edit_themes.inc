<?php
/*
 * Default edit themes
 */
function qw_default_edit_themes($themes){
  $themes['tabs'] = array(
    'title' => 'UI Tabs',
    'init_callback' => 'qw_theme_tabs',
  );
  $themes['views'] = array(
    'title' => 'Drupal Views',
    'init_callback' => 'qw_theme_views',
  );
  return $themes;
}
// add default fields to the hook filter
add_filter('qw_edit_themes', 'qw_default_edit_themes', 0);

/*
 * Jquery UI Tabs
 */
function qw_theme_tabs(){
  add_action('admin_head', 'qw_theme_tabs_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_theme_tabs_js');
  }
}
// css
function qw_theme_tabs_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/themes/tabs/query-wrangler-tabs.css" />';
}
// js
function qw_theme_tabs_js(){
  wp_enqueue_script('qw-admin-theme-tabs',
                  plugins_url('/themes/tabs/query-wrangler-tabs.js', dirname(__FILE__)),
                  array('jquery-ui-core',
                        'jquery-ui-sortable',
                        'jquery-ui-tabs',
                        'jquery-ui-accordion',
                        'jquery-ui-dialog'),
                  QW_VERSION);
}

/*
 * Views clone
 */
function qw_theme_views(){
  add_action('admin_head', 'qw_theme_views_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_theme_views_js');
  }
}
// css
function qw_theme_views_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/themes/views/query-wrangler-views.css" />';
}
// js
function qw_theme_views_js(){
  // my js script
  wp_enqueue_script('qw-edit-theme-views',
                  plugins_url('/themes/views/query-wrangler-views.js', dirname(__FILE__) ),
                  array('jquery-ui-core',
                        'jquery-ui-sortable',
                        'jquery-ui-tabs',
                        'jquery-ui-accordion',
                        'jquery-ui-dialog'),
                  QW_VERSION);
}

