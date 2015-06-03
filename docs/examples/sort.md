# Example Query Wrangler sort option

This sort option example doesn't really do anything except send a key/value pair to WP_Query().

```php
<?php
// add default field styles to the filter
add_filter('qw_sort_options', 'qw_example_sort_options');

/*
 * Sorts are generally very generic.
 *
 * Most WP_Query sort arguments have a very simple values and structures.
 * Since in QW it is a handler, it has alot of other available options.
 */
function qw_example_sort_options($sort_options)
{
  $sort_options['example_sort'] = array(
    // the title displayed to the query-wrangler user
    'title' => 'Example: Sort Option',

    // some help text for the user about how this sort option works
    'description' => 'A description of how this sort option works.',

    // (optional) This is the value of the WP_Query argument orderby_key
    // defaults to: the hook_key
      // $args[$sort['orderby_key']] = $sort['type'];
    'type' => 'wp_query_argument_key',

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

    // (optional) a custom callback function for placing form values into a WP_Query as arguments
    // defaults to:
      //  $args[$sort['orderby_key']] = $sort['type'];
      //  $args[$sort['order_key']] = $sort['order_value'];
    'query_args_callback' => 'qw_sort_example_query_args',

    // (optional) a custom callback for sort options forms
    // if callback and template both aren't set,
    //   defaults to:  'qw_sorts_default_form_callback'
    'form_callback' => 'my_sort_option_form_callback',

    // (optional) a template wrangler form template
    'form_template' => 'my_tw_form_template',
  );
  return $sort_options;
}

/*
 * Doing this is so simple that qw will do it for you if you don't provide a callback.
 * But this is what it looks like if you were to do it yourself
 *
 *  @param  &$args - The WP_Query arguments we are building
 *  @param  $filter - This filter's settings and values
                      Values stored in $filter['values']
 */
function qw_sort_example_query_args(&$args, $sort){
  $args[$sort['orderby_key']] = $sort['type'];
  $args[$sort['order_key']] = $sort['order_value'];
}
````