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
function qw_default_edit_themes($themes){
  $themes['tabs'] = array(
    'title' => 'Tabs',
    'init_callback' => 'qw_edit_theme_tabs',
  );
  $themes['views'] = array(
    'title' => 'Drupal Views',
    'init_callback' => 'qw_edit_theme_views',
  );
  return $themes;
}
// add default fields to the hook filter
add_filter('qw_edit_themes', 'qw_default_edit_themes', 0);

/*
 * Edit Theme: Jquery UI Tabs
 */
function qw_edit_theme_tabs(){
  add_action('admin_head', 'qw_edit_theme_tabs_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_edit_theme_tabs_js');
  }
}
// css
function qw_edit_theme_tabs_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/themes/tabs/query-wrangler-tabs.css" />';
}
// js
function qw_edit_theme_tabs_js(){
  wp_enqueue_script('qw-admin-theme-tabs',
                  plugins_url('/themes/tabs/query-wrangler-tabs.js', dirname(__FILE__)),
                  array(),
                  QW_VERSION, true);
}

/*
 * Edit Theme: Drupal Views clone
 */
function qw_edit_theme_views(){
  add_action('admin_head', 'qw_edit_theme_views_css');
  if($_GET['page'] == 'query-wrangler'){
    add_action('admin_enqueue_scripts', 'qw_edit_theme_views_js');
  }
}
// css
function qw_edit_theme_views_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/themes/views/query-wrangler-views.css" />';
}
// js
function qw_edit_theme_views_js(){
  // my js script
  wp_enqueue_script('qw-edit-theme-views',
                  plugins_url('/themes/views/query-wrangler-views.js', dirname(__FILE__) ),
                  array(),
                  QW_VERSION, true);
}
