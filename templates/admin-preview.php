<?php
// get the query options, force override
$options = qw_generate_query_options($query_id, $options, true);
// get formatted query arguments
$args = qw_generate_query_args($options);
// set the new query
$new_query = new WP_Query($args);
// get the themed content
$preview = qw_template_query($new_query, $options);


// args
$new_query_args = qw_generate_query_args($options);
$args = "<pre>".print_r($new_query_args, true)."</pre>";

// display
$display = "<pre>".htmlentities(print_r($options['display'], true))."</pre>";

// return
$preview = array(
  'preview' => $preview,
  'args' => $args,
  'display' => $display,
  'wpquery' => $new_query,
);

print json_encode($preview);