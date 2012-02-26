<?php
/*
 * Primary function for bulding and displaying a query
 *
 * @param int $query_id Id for the query
 * @param array $options_override an array for changing or adding query data options
 * @param bool $reset_post_data Reset the $wp_query after execution
 * @return string Can return a string of html based on parameter $return
 */
function qw_execute_query($query_id, $options_override = array(), $reset_post_data = true)
{
  global $wp_query;
  // backup the old query
  $old_query = $wp_query;


  // get the query options
  $options = qw_generate_query_options($query_id, $options_override);

  // get formatted query arguments
  $args = qw_generate_query_args($options);

// pre_query hook
  do_action_ref_array('qw_pre_query', array(&$args));

  // set the new query
  $wp_query = new WP_Query($args);

// pre_render hook
  do_action_ref_array('qw_pre_render', array(&$options));

  // get the themed content
  $themed = qw_template_query($wp_query, $options);

  // Reset Post Data
  if($reset_post_data){
    wp_reset_postdata();
    $wp_query = $old_query;
  }

  return $themed;
}
/*
 * Get the Query, and set $options to defaults
 *
 * @param int
 *   $query_id - The unique ID of the query
 * @param array
 *   $options_override - An array of values to override in the retrieved set
 * @param array
 *   $full_override - force the options_override as all options
 *
 * @return array
 *   Query options
 */
function qw_generate_query_options($query_id, $options_override = array(), $full_override = false)
{
  global $wpdb, $wp_query;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name,type,slug,data FROM ".$table_name." WHERE id = ".$query_id;

  $rows = $wpdb->get_results($sql);

  // unserealize the stored data
  $options = qw_unserialize($rows[0]->data);

  // override options
  if ($full_override) {
    // force a full override
    $options = $options_override;
  } else {
    // combine options
    $options =  array_merge_recursive((array)$options, $options_override);
  }

  // build query_details
  $options['meta'] = array();
  $options['meta']['id'] = $rows[0]->id;
  $options['meta']['slug'] = $rows[0]->slug;
  $options['meta']['name'] = $rows[0]->name;
  $options['meta']['type'] = $rows[0]->type;
  $options['meta']['pagination'] = (isset($options['args']['paged'])) ? true : false;
  $options['meta']['header'] = $options['display']['header'];
  $options['meta']['footer'] = $options['display']['footer'];
  $options['meta']['empty'] = $options['display']['empty'];

  return $options;
}

/*
 * Generate the WP query itself
 *
 * @param array $options Query data
 * $param array $options_override can change the options array
 * @return array Query Arguments
 */
function qw_generate_query_args($options = array(), $options_override = array())
{
  $all_filters = qw_all_filters();
  $all_sorts = qw_all_sort_options();

  // standard arguments
  $args['posts_per_page'] = ($options['args']['posts_per_page']) ? $options['args']['posts_per_page']: 5;
  $args['offset']         = ($options['args']['offset']) ? $options['args']['offset']: 0;
  //$args['orderby']        = ($options['args']['orderby']) ? $options['args']['orderby']: 'none';
  //$args['order']          = ($options['args']['order']) ? $options['args']['order']: 'ASC';
  $args['paged']          = $options['args']['paged'];
  $args['post_status']    = $options['args']['post_status'];

  // handle sorts
  if(is_array($options['args']['sorts'])){

    foreach($options['args']['sorts'] as $sort)
    {
      $hook_key = qw_get_hook_key($all_sorts, $sort);
      // combine sort details
      if (isset($all_sorts[$hook_key])){
        $sort = array_merge($all_sorts[$hook_key], $sort);
      }

      // look for callback, and run it
      if (isset($sort['query_args_callback']) && function_exists($sort['query_args_callback'])){
        $sort['query_args_callback']($args, $sort);
      } else {
        // else, default to type as WP_Query argument key
        // arguments passed to query
        $args[$sort['orderby_key']] = $sort['type'];
        $args[$sort['order_key']] = $sort['order_value'];
      }
    }
  }

  // handle filters
  if(is_array($options['args']['filters'])){
    // loop through all filters
    foreach($options['args']['filters'] as $filter)
    {
      $hook_key = qw_get_hook_key($all_filters, $filter);
      // combine filter details
      if (isset($all_filters[$hook_key])){
        $filter = array_merge($all_filters[$hook_key], $filter);
      }

      // look for callback, and run it
      if (isset($filter['query_args_callback']) && function_exists($filter['query_args_callback'])){
        $filter['query_args_callback']($args, $filter);
      } else {
        // else, default to type as WP_Query argument key
        $args[$filter['type']] = $filter[$filter['type']];
      }
    }
  }
  return $args;
}
/*
 * Get all queries of the type widget
 *
 * @return array of query widgets with key as query id
 */
function qw_get_all_widgets()
{
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name FROM ".$table_name." WHERE type = 'widget'";
  $rows = $wpdb->get_results($sql);

  if(is_array($rows))
  {
    $widgets = array();
    foreach($rows as $row){
      $widgets[$row->id] = $row->name;
    }
    return $widgets;
  }
}
/*
 * Get all query pages
 *
 * @return array Query pages in WP post format
 */
function qw_get_all_pages()
{
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name,path FROM ".$table_name." WHERE type = 'page'";
  $rows = $wpdb->get_results($sql);

  if(is_array($rows)){
    $pages = array();
    $blog_url = get_bloginfo('wpurl');

    $i=0;
    foreach($rows as $row){
      $pages[$i] = new stdClass();
      $pages[$i]->ID = $row->id;
      $pages[$i]->title = $row-name;
      $pages[$i]->post_title = $row->name;
      $pages[$i]->guid = $blog_url.$row->path;
      $pages[$i]->post_type = 'page';
    }

    return $pages;
  }
}

/*
 * Helper function: Get the current page number
 * @param object $wp_query - the query being displayed
 *
 * @return
 *    int - the currentpage number
 */
function qw_get_page_number(&$wp_query){
  // help figure out the current page
  $path_array = explode('/page/', $_SERVER['REQUEST_URI']);

  // look for WP paging first
  if ($wp_query->query_vars['paged']){
    $page = $wp_query->query_vars['paged'];
  }
  // try wordpress method
  else if (get_query_var('paged')){
    $page = get_query_var('paged');
  }
  // paging with slashes
  else if (isset($path_array[1])) {
    $page = explode('/', $path_array[1]);
    $page = $page[0];
  }
  // paging with get variable
  else if (isset($_GET['page'])) {
    $page = $_GET['page'];
  }
  // paging with a different get variable
  else if (isset($_GET['paged'])) {
    $page = $_GET['paged'];
  }
  else {
    $page = 1;
  }
  return $page;
}
/*
 * Get full term data
 *
 * @param $term
 *   - either term id or term slug based on 2nd parameter
 * @param $by
 *   - either 'id' or 'slug': what you want the get term by
 * @param $return_type
 *  - OBJECT, ARRAY_A, ARRAY_N
 *
 * @return
 *  - term, format depending on 3rd parameter
 *  - false if not found
 */
function qw_get_term($term, $by = 'id' , $return_type = OBJECT){
  global $wpdb;
  switch ($by){
    case 'id':
      $where = 't.term_id = '.$term;
      break;

    case 'slug':
      $where = 't.slug = "'.$term.'"';
      break;
  }

  $sql = "SELECT
            t.term_id,t.name,t.slug,t.term_group,tax.taxonomy,tax.description,tax.parent,tax.count
          FROM ".$wpdb->prefix."terms as t
          LEFT JOIN ".$wpdb->prefix."term_taxonomy as tax ON t.term_id = tax.term_id
          WHERE ".$where;
  $t = $wpdb->get_row($sql, $return_type);

  if ($t->term_id){
    // http://web.archiveorange.com/archive/v/XZYvyS8D7kDM3sQgrvJF
    $t->link = get_term_link((int)$t->term_id, $t->taxonomy);
    // return term if found
    return $t;
  }
  else {
    return false;
  }
}

/*
 * Trim each item in an array w/ array_walk
 *   eg: array_walk($fruit, 'qw_trim');
 */
function qw_trim(&$value){
  $value = trim($value);
}
/*
 * Serialize wrapper functions for future changes.
 */
function qw_serialize($array){
  return serialize($array);
}
/*
 * Custom: Fix unserialize problem with quotation marks
 */
function qw_unserialize($serial_str) {
  $serial_str= preg_replace('!s:(\d+):"(.*?)";!se', "'s:'.strlen('$2').':\"$2\";'", $serial_str );
  $serial_str= str_replace("\r", "", $serial_str);
  //$serial_str= str_replace("\n", "", $serial_str);
  $array = array();
  $array = unserialize($serial_str);
  if(is_array($array)){
    // stripslashes twice for science
    $array = array_map('stripslashes_deep', $array);
    $array = array_map('stripslashes_deep', $array);
    return $array;
  }
}
/*
 * Support function for legacy, pre hook_keys discovery
 */
function qw_get_hook_key($all, $single)
{
  // see if hook key is set
  if (!empty($single['hook_key'])){
    $hook_key = $single['hook_key'];
  }
  // look for type as key
  else if (!empty($single['type']))
  {
    foreach ($all as $key => $item)
    {
      if($single['type'] == $all[$key]['type']){
        $hook_key = $all[$key]['hook_key'];
        break;
      }
      else if($single['type'] == $key){
        $hook_key = $key;
        break;
      }
    }
  }
  return $hook_key;
}

/*
 * Generate form prefixes for handlers
 *
 * @param string
 *    $type = sort, field, filter
 */
function qw_make_form_prefix($type, $name){
  $output = "qw-query-options";
  switch($type){
    case 'sort':
      $output.= "[args][sorts]";
      break;
    case 'field':
      $output.= "[display][field_settings][fields]";
      break;
    case 'filter':
      $output.= "[args][filters]";
      break;
  }
  $output.= "[".$name."]";
  return $output;
}
/*
 * Function for grabbing meta keys
 *
 * @return array All meta keys in WP
 */
function qw_get_meta_keys() {
	global $wpdb;

	$keys = $wpdb->get_col( "
			SELECT meta_key
			FROM $wpdb->postmeta
			GROUP BY meta_key
			ORDER BY meta_key" );

	return $keys;
}
/*
 * usort callback. I likely stole this from somewhere.. like php.net
 */
function qw_cmp($a,$b) {
  if ($a['weight'] == $b['weight']) return 0;
  return ($a['weight'] < $b['weight'])? -1 : 1;
}
