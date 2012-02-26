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

// args
$args = "<pre>".print_r($args, true)."</pre>";

// display
$display = "<pre>".htmlentities(print_r($options['display'], true))."</pre>";

$new_query = "<pre>".print_r($qp,true)."</pre>"."<pre>".print_r($wp_query,true)."</pre>";

// return
$preview = array(
  'preview' => $preview,
  'args' => $args,
  'display' => $display,
  'wpquery' => $new_query,
);

do_action_ref_array('qw_post_preview', array(&$preview));

print json_encode($preview);