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
add_filter('qw_post_statuses', 'qw_default_post_statuses');

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
add_filter('qw_styles', 'qw_default_styles');
/*
 * Default Row Styles
 */
function qw_default_row_styles($row_styles){
  $row_styles['posts'] = array(
    'title' => 'Posts',
  );
  $row_styles['fields'] = array(
    'title' => 'Fields',
  );
  return $row_styles;
}
// add default field styles to the filter
add_filter('qw_row_styles', 'qw_default_row_styles');

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
add_filter('qw_row_complete_styles', 'qw_default_row_complete_styles');

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
add_filter('qw_row_fields_styles', 'qw_default_row_fields_styles');

/*
 * Default Sort Options
 *
 * example option

  $sort_options['hook_key'] = array(
    // the title displayed to the query-wrangler user
    'title' => 'The Select Option Label',

    // some help text for the user about how this key works
    'description' => 'A description of how this sort works',

    // (optional) the WP_Query argument's value (essentially 'orderby_value')
    // defaults to the hook_key
      // $args[$sort['orderby_key']] = $sort['type'];
    'type' => 'wp_query_argument_key'

    // (optional) a custom callback function for placing form values into a WP_Query as arguments
    // defaults to:
      //  $args[$sort['orderby_key']] = $sort['type'];
      //  $args[$sort['order_key']] = $sort['order_value'];
    'query_args_callback' => 'my_sort_option_args_callback',

    // (optional) the WP_Query argument key equivalent to WP_Query's 'orderby'
    // defaults to: 'orderby'
    'orderby_key' => 'my_orderby_key',

    // (optional) the WP_Query argument key equivalent to WP_Query's 'order'
    // defaults to: 'order'
    'order_key' => 'my_order_key',

    // (optional) order options provided in a select menu
    // defaults to:  below values
    'order_options' => array(
      'ASC' => 'Ascending',
      'DESC' => 'Descending',
    ),
  );

 */
function qw_default_sort_options($sort_options){

  $sort_options['author_id'] = array(
    'title' => 'Author',
    'description' => 'The content author ID.',
    'type' => 'author',
  );
  $sort_options['post_date'] = array(
    'title' => 'Date',
    'description' => 'The posted date of content.',
    'type' => 'date',
  );
  $sort_options['post_ID'] = array(
    'title' => 'Post ID',
    'description' => 'The ID of the content.',
    'type' => 'ID',
  );
  $sort_options['post_modified'] = array(
    'title' => 'Modified Date',
    'description' => 'Date content was last modified.',
    'type' => 'modified',
  );
  $sort_options['post_parent'] = array(
    'title' => 'Parent',
    'description' => 'The parent post for content.',
    'type' => 'parent',
  );
  $sort_options['post_title'] = array(
    'title' => 'Title',
    'description' => 'The title of the content.',
    'type' => 'title',
  );
  $sort_options['comment_count'] = array(
    'title' => 'Comment Count',
    'description' => 'Total number of comments on a piece of content.',
  );
  $sort_options['menu_order'] = array(
    'title' => 'Menu Order (for Page post_types)',
    'description' => 'Menu Order of a Page.',
  );
  $sort_options['none'] = array(
    'title' => 'None',
    'description' => 'No sort order.',
  );
  $sort_options['rand'] = array(
    'title' => 'Random',
    'description' => 'Random order.',
  );
  return $sort_options;
}
// add default field styles to the filter
add_filter('qw_sort_options', 'qw_default_sort_options');


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
add_filter('qw_file_styles', 'qw_default_file_styles');

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
add_filter('qw_pager_types', 'qw_default_pager_types');

