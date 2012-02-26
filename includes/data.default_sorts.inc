<?php
/*
 * Default Sort Options
 *
 * Template

  $sort_options['hook_key'] = array(
    // the title displayed to the query-wrangler user
    'title' => 'The Select Option Label',

    // some help text for the user about how this key works
    'description' => 'A description of how this sort works',

    // (optional) the WP_Query argument's value (essentially 'orderby_value')
    // defaults to: the hook_key
      // $args[$sort['orderby_key']] = $sort['type'];
    'type' => 'wp_query_argument_key'

    // (optional) a custom callback function for placing form values into a WP_Query as arguments
    // defaults to:
      //  $args[$sort['orderby_key']] = $sort['type'];
      //  $args[$sort['order_key']] = $sort['order_value'];
    'query_args_callback' => 'my_sort_option_args_callback',

    // (optional) a custom callback for sort options forms
    // if callback and template both aren't set,
    //   defaults to:  'qw_form_default_sort_order_options'
    'form_callback' => 'my_sort_option_form_callback',

    // (optional) a template wrangler form template
    'form_template' => 'my_tw_form_template',

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
    'title' => 'Date Modified',
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
 * Default sort options 'order' options form
 */
function qw_form_default_sort_order_options($sort)
{
  ob_start();
  ?>
    <!-- sort options -->
    <label class="qw-label">Order by <?php print $sort['title']; ?>:</label>
    <select name="<?php print $sort['form_prefix']; ?>[order_value]">
      <?php
        foreach ($sort['order_options'] as $value => $label)
        { ?>
          <option value="<?php print $value; ?>"
                  <?php if($sort['values']['order_value'] == $value) { print 'selected="selected"';} ?>>
            <?php print $label; ?>
          </option>
          <?php
        }
      ?>
    </select>
    <p class="description">Select how to order the results.</p>
  <?php
  return ob_get_clean();
}