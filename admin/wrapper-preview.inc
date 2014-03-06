<?php
global $wp_query;
$temp = $wp_query;
$wp_query = NULL;

ob_start();
  // get the query options, force override
  $options = qw_generate_query_options($query_id, $options, true);

  do_action_ref_array('qw_pre_preview', array(&$options));

  // get formatted query arguments
  $args = qw_generate_query_args($options);
  // set the new query
  $wp_query = new WP_Query($args);

  // get the themed content
  print qw_template_query($wp_query, $options);
$preview = ob_get_clean();

$templates = "These template files will be searched for relative to your theme folder.<br />
              To override a query's template, copy the corresponding template from the <span style='font-family: monospace;'>query-wrangler/templates</span> folder to your theme folder (or THEME/templates) and rename it.
              <pre>".print_r(qw_template_scan($options),1)."</pre>";

// php wp_query
$php_wpquery = '<pre>$query = '.var_export($args,1).';</pre>';
// args
$args = "<pre>".print_r($args, true)."</pre>";
// display
$display = "<pre>".htmlentities(print_r($options['display'], true))."</pre>";

$new_query = "<pre>".print_r($qp,true)."</pre>"."<pre>".print_r($wp_query,true)."</pre>";

// return
$preview = array(
  'preview' => $preview,
  'php_wpquery' => $php_wpquery,
  'args' => $args,
  'display' => $display,
  'wpquery' => $new_query,
  'templates' => $templates,
);

do_action_ref_array('qw_post_preview', array(&$preview));

print json_encode($preview);

/*
 * Scan for all templates used by a single query
 */
function qw_template_scan($options)
{
  global $wpdb;
  $query_id = $options['meta']['id'];
  $slug = $options['meta']['slug'];
  $all_styles = qw_all_styles();
  $all_row_styles = qw_all_row_styles();
  $style = $all_styles[$options['display']['style']];
  $row_style = $all_row_styles[$options['display']['row_style']];
  $output = array();
  $templates = array();

  // start building theme arguments
  $wrapper_args = array(
    'slug' => $slug,
    'type' => $options['display']['types']['this_instance'],
    'tw_action' => 'find_only',
  );
  // template with wrapper
  $templates['wrapper'] = theme('query_display_wrapper', $wrapper_args, true);

  $style_settings = array();
  if (isset($options['display']['style_settings'][$style['hook_key']])){
    $style_settings = $options['display']['style_settings'][$style['hook_key']];
  }
  // setup row template arguments
  $template_args = array(
    'template' => $style['template'],
    'slug' => $slug,
    'type' => $options['display']['types']['this_instance'],
    'style' => $style['hook_key'],
    'style_settings' => $style_settings,
    'tw_action' => 'find_only',
  );
  // template the query rows
  $templates['style'] = theme('query_display_rows', $template_args);

  if ($row_style['hook_key'] == "posts"){
    $row_style_settings = $options['display']['row_style_settings'][$row_style['hook_key']];

    $template_args = array(
      'template' => 'query-'.$row_style_settings['size'],
      'slug' => $slug,
      'type' => $options['display']['types']['this_instance'],
      'style' => $row_style_settings['size'],
      'tw_action' => 'find_only',
    );
    $templates['row_style'] = theme('query_display_rows', $template_args);
  }

  if ($row_style['hook_key'] == "fields"){
    $row_style_settings = $options['display']['row_style_settings'][$row_style['hook_key']];

    $template_args = array(
      'template' => 'query-field',
      'slug' => $slug,
      'type' => $options['display']['types']['this_instance'],
      'style' => $options['display']['row_style'],
      'tw_action' => 'find_only',
    );
    $templates['row_style'] = theme('query_display_rows', $template_args);
  }

  foreach($templates as $k => $template)
  {
    foreach($template['suggestions'] as $suggestion){
      if($suggestion == $template['found_suggestion']){
        $output[$k][] = '<strong>'.$suggestion.'</strong>';
      }
      else{
        $output[$k][] = $suggestion;
      }
    }

    // see if this is the default template
    if (stripos($template['found_path'], QW_PLUGIN_DIR) !== false){
      $output[$k]['found'] = '<em>(default) '.$template['found_path'].'</em>';
    } else {
      $output[$k]['found'] = '<strong>'.$template['found_path'].'</strong>';
    }
    //$output[$k]['template'] = $template;
  }

  return $output;
}