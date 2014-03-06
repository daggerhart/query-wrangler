<?php

// add default filters to the filter
add_filter('qw_filters', 'qw_filter_post_types');

function qw_filter_post_types($filters){
  $filters['post_types'] = array(
    'title' => 'Post Types',
    'description' => 'Select which post types should be shown.',
    'form_callback' => 'qw_filter_post_types_form',
    'query_args_callback' => 'qw_generate_query_args_post_types',
    'query_display_types' => array('page','widget','override'),
  );
  return $filters;
}

function qw_filter_post_types_form($filter)
{
  ?>
  <div class="qw-checkboxes">
    <?php
      $post_types = qw_all_post_types();
      // loop through post types
      foreach ($post_types  as $post_type )
      {
        $post_type_checked = (isset($filter['values']['post_types'][$post_type])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input class="qw-js-title"
                 type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[post_types][<?php print $post_type; ?>]"
                 value="<?php print $post_type; ?>"
                 <?php print $post_type_checked; ?> />
          <?php print ucfirst($post_type); ?>
        </label>
        <?php
      }
    ?>
  </div>
  <?php
}

function qw_generate_query_args_post_types(&$args, $filter){
  $args['post_type'] = $filter['values']['post_types'];
}