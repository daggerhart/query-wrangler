<?php
global $wp_query;
$temp = $wp_query;
$wp_query = NULL;
// get the query options, force override
$options = qw_generate_query_options($query_id, $options, true);
// get formatted query arguments
$args = qw_generate_query_args($options);
// set the new query
$wp_query = new WP_Query($args);
// get the themed content
$preview = qw_template_query($wp_query, $options);
// args
$args = "<pre>".var_export($args, true)."</pre>";

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

print json_encode($preview);