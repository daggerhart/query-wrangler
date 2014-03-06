<?php

// add default filters to the filter
add_filter('qw_filters', 'qw_filter_post_id');

function qw_filter_post_id($filters){
  $filters['post_id'] = array(
    'title' => 'Post IDs',
    'description' => 'Provide a list of post_ids to show or not show.',
    'form_callback' => 'qw_filter_post_id_form',
    'query_args_callback' => 'qw_generate_query_args_post_id',
    'query_display_types' => array('page','widget','override'),
  );
  return $filters;
}

function qw_filter_post_id_form($filter)
{
  ?>
    <p>
      <input class="qw-js-title" type='text' name="<?php print $filter['form_prefix']; ?>[post_ids]" value='<?php print $filter['values']['post_ids']; ?>' />
    </p>
    <p class="description">How to treat these post IDs.</p>
    <p>
      <select class="qw-js-title" name="<?php print $filter['form_prefix']; ?>[compare]">
        <option value="post__in" <?php if($filter['values']['compare'] == "post__in") { print 'selected="selected"';} ?>>Only these posts</option>
        <option value="post__not_in" <?php if($filter['values']['compare'] == "post__not_in") { print 'selected="selected"';} ?>>Not these posts</option>
      </select>
    </p>
  <?php
}

function qw_generate_query_args_post_id(&$args, $filter){
  $pids = explode(",", $filter['values']['post_ids']);
  array_walk($pids, 'qw_trim');
  $args[$filter['values']['compare']] = $pids;
}