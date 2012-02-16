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
  $wp_query = null;

  // get the query options
  $options = qw_generate_query_options($query_id, $options_override);

  // get formatted query arguments
  $args = qw_generate_query_args($options);

  // set the new query
  $wp_query = new WP_Query($args);

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
  $filters = qw_all_filters();

  // standard arguments
  $args['posts_per_page'] = ($options['args']['posts_per_page']) ? $options['args']['posts_per_page']: 5;
  $args['offset']         = ($options['args']['offset']) ? $options['args']['offset']: 0;
  $args['orderby']        = ($options['args']['orderby']) ? $options['args']['orderby']: 'none';
  $args['order']          = ($options['args']['order']) ? $options['args']['order']: 'ASC';
  $args['paged']          = $options['args']['paged'];
  $args['post_status']    = $options['args']['post_status'];

  // handle filters
  if(is_array($options['args']['filters'])){
    // loop through all filters
    foreach($options['args']['filters'] as $filter)
    {
      // combine filter details
      if (isset($filters[$filter['type']])){
        $filter = array_merge($filters[$filter['type']], $filter);
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
 * Default values for  new query
 *
 * @return array Default query settings
 */
function qw_default_query_data(){
  return array(
    'args' => array(
      'posts_per_page'  => 5,
      'offset'          => 0,
      'cat_operator'    => 'cat',
      'post_status'     => 'publish',
      'orderby'         => 'date',
      'order'           => 'ASC',
      'filters'         => array(),
    ),
    'display' => array(
      'style' => 'unformatted',
      'row_style' => 'posts',
      'post_settings' => array(
        'size' => 'complete',
      ),
    ),
  );
}
/*
 * Slug creation
 */
function qw_make_slug($string){
  $search = array("!","@","#","$","%","^","&","*","(",")","-","+","=","{","}","[","]","\\","|",":",";","'","<",",",">",".","?","/","~","`");
  return str_replace(" ", "_", strtolower(str_replace($search, "", strip_tags($string))));
}
/*
 * usort callback. I likely stole this from somewhere.. like php.net
 */
function qw_cmp($a,$b) {
  if ($a['weight'] == $b['weight']) return 0;
  return ($a['weight'] < $b['weight'])? -1 : 1;
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
function qw_handle_html(&$value, $key = ''){
  if(is_string($value)){
    // for some reason stripslashes isn't getting all slashes
    $value = str_replace("\\", "", stripslashes(stripslashes($value)));
  }
}

/*
 * Custom for outputting text/html into a textarea
 */
function qw_textarea($value){
  return esc_textarea(str_replace("\\", "", $value));
}
/*
 * Serialize wrapper functions for future changes.
 */
function qw_serialize($array){
  return serialize($array);
}
// fix unserialize problem with quotation marks
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