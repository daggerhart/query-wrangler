<?php
/*
 * Post statuses as a hook for contributions
 */
function qw_default_post_statuses($post_statuses){
  $post_statuses['publish'] = array(
    'title' => 'Published',
  );
  $post_statuses['pending'] = array(
    'title' => 'Pending',
  );
  $post_statuses['draft'] = array(
    'title' => 'Draft',
  );
  $post_statuses['future'] = array(
    'title' => 'Future (Scheduled)',
  );
  $post_statuses['trash'] = array(
    'title' => 'Trashed',
  );
  $post_statuses['private'] = array(
    'title' => 'Private',
  );
  $post_statuses['any'] = array(
    'title' => 'Any',
  );
  return $post_statuses;
}
// add default fields to the hook filter
add_filter('qw_post_statuses', 'qw_default_post_statuses', 0);

/*
 * All Field Styles and settings
 *
 * @return array Field Styles
 */
function qw_default_styles($styles)
{
  $styles['unformatted'] = array(
    'title' => 'Unformatted',
    'template' => 'query-unformatted',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['unordered_list'] = array(
    'title' => 'Unordered List',
    'template' => 'query-unordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['ordered_list'] = array(
    'title' => 'Ordered List',
    'template' => 'query-ordered_list',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );
  $styles['table'] = array(
    'title' => 'Table',
    'template' => 'query-table',
    'default_path' => QW_PLUGIN_DIR, // do not include last slash
  );

  return $styles;
}
// add default field styles to the filter
add_filter('qw_styles', 'qw_default_styles', 0);

/*
 * Default Row Styles
 */
function qw_default_row_styles($row_styles){
  $row_styles['posts'] = array(
    'title' => 'Posts',
    'settings_callback' => 'qw_row_style_posts_settings',
  );
  $row_styles['fields'] = array(
    'title' => 'Fields',
  );
  return $row_styles;
}
// add default field styles to the filter
add_filter('qw_row_styles', 'qw_default_row_styles', 0);

function qw_row_style_posts_settings()
{
  ob_start();
  ?>
    <p class="description">Select the amount of the post to be shown.</p>
    <select class="qw-title-ignore"
            name="qw-query-options[display][post_settings][size]">
      <option value="complete"
              <?php if($display['post_settings']['size'] == "complete") { print 'selected="selected"';} ?>>
        Complete Post
      </option>
      <option value="excerpt"
              <?php if($display['post_settings']['size'] == "excerpt") { print 'selected="selected"';} ?>>
        Excerpt
      </option>
    </select>
  <?php
  return ob_get_clean();
}


/*
 * Default Row 'Posts' Styles
 */
function qw_default_row_complete_styles($row_complete_styles){
  $row_complete_styles['complete'] = array(
    'title' => 'Complete Post',
  );
  $row_complete_styles['excerpt'] = array(
    'title' => 'Excerpt',
  );
  return $row_complete_styles;
}
// add default field styles to the filter
add_filter('qw_row_complete_styles', 'qw_default_row_complete_styles', 0);

/*
 * Default Row 'Fields' Styles
 */
function qw_default_row_fields_styles($row_fields_styles){
  $row_fields_styles['posts'] = array(
    'title' => 'Posts',
  );
  $row_fields_styles['fields'] = array(
    'title' => 'Fields',
  );
  return $row_styles;
}
// add default field styles to the filter
add_filter('qw_row_fields_styles', 'qw_default_row_fields_styles', 0);

/*
 * File Styles
 *
 * @return array of file styles
 */
function qw_default_file_styles($file_styles)
{
  $file_styles['link'] = array(
    'description' => 'Filename Link to File',
  );
  $file_styles['link_url'] = array(
    'description' => 'URL Link to File',
  );
  $file_styles['url'] = array(
    'description' => 'URL of File',
  );

  return $file_styles;
}
// add default file styles to the filter
add_filter('qw_file_styles', 'qw_default_file_styles', 0);

/*
 * Setup pager types
 */
function qw_default_pager_types($pagers) {
  $pagers['default'] = array(
    'title' => 'Default',
    'callback' => 'qw_theme_pager_default'
  );
  $pagers['numbers']	= array(
    'title' => 'Page Numbers',
    'callback' => 'qw_theme_pager_numbers'
  );

  // WP PageNavi Plugin
  if(function_exists('wp_pagenavi')) {
    $pagers['pagenavi'] = array(
      'title' => 'PageNavi',
      'callback' => 'wp_pagenavi'
    );
  }
  return $pagers;
}
// add default pager types
add_filter('qw_pager_types', 'qw_default_pager_types', 0);
