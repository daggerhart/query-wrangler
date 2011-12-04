<?php
// TODO:  fix category paging
// http://wordpress.org/support/topic/explanation-and-workaround-for-error-404-on-category-pagination?replies=3
// http://wordpress.org/extend/plugins/category-pagination-fix/

// this works for categories!
// taken from category-pagination-fix plugin and modified
function qw_remove_page_from_query_string($query_string)
{
  // fix tags 
  if(isset($query_string['tag']) && $query_string['paged']){
    unset($query_string['paged']);
  }
  // fix categories
  if ($query_string['name'] == 'page' && isset($query_string['page'])) {
      unset($query_string['name']);
      unset($query_string['page']);
  }
  
  return $query_string;
}
// I will kill you if you remove this. I died two days for this line 
add_filter('request', 'qw_remove_page_from_query_string');

// http://codex.wordpress.org/Conditional_Tags
function qw_get_page_op()
{
  $op = '';
  if (is_category()){
    $op = 'category';
  }
  else if (is_tag()){
    $op = 'tag';
  }
  else if (is_home()){
    $op = 'home';
  }
  else if (is_single()){
    // single is not pages
    $op = 'single';
  }
  else if (is_page()){
    $op = 'page';
  }
  else if (is_author()){
    $op = 'author';
  }
  else if (is_search()){
    $op = 'search';
  }
  else if (is_404()){
    $op = '404';
  }
  else if (is_paged()){
    $op = 'paged';
  }
  else {
    $op = 'default';
  }
  return $op;
}
/*
 * Query pages
 */
add_action('wp', 'qw_query_pages_handler');

/*
 * Handle Query pages
 * ref: http://www.blogseye.com/2010/05/creating-fake-wordpress-posts-on-the-fly/
 */
function qw_query_pages_handler()
{ 
  // page op is based on WP tags like is_category() && is_404()
  $op = qw_get_page_op();
  
  // we only handle sepcific types of pages and overrides
  if($op == 'category' ||
     $op == 'tag' ||
     $op == '404')
  {
    global $wp_query,$wpdb; 
    $table = $wpdb->prefix."query_wrangler";
    // We'll need to override query options
    $options_override = array();
    
    // Look at the requested uri w/o paginiation involved
    $dirty_path = explode('/page/', $_SERVER['REQUEST_URI']);
    
    // Clean the result for DB
    $path = $wpdb->escape($dirty_path[0]);
    
    // fix the query paging
    if(isset($dirty_path[1])){
      $paged = explode("/", $dirty_path[1]);
      $wp_query->query_vars['paged'] = $paged[0];
      $wp_query->query_vars['page'] = $paged[0];
    }
    
    // handle paging
    if ( get_query_var('paged') ) {
      $paged = get_query_var('paged');
    } elseif ( get_query_var('page') ) {
      $paged = get_query_var('page');
    } else {
      $paged = 1;
    }
    
    /*
     * 404 -> Query type = Page
     */
    if($op == '404')
    {
      // include Template Wrangler
      if(!function_exists('theme')){
        include_once QW_PLUGIN_DIR.'/template-wrangler.inc';
      }
      
      // take into account trailing slash
      if(substr($path, strlen($path)-1, 1) != '/'){
        $alt_path = $path.'/';
      } else {
        $alt_path = substr($path, 0, strlen($path)-1);
      }
      
      // Look for the query path given
      $table_name = $wpdb->prefix."query_wrangler";
      $sql = "SELECT id,name,path,data FROM ".$table." WHERE type = 'page' AND (path = '".$path."' OR path = '".$alt_path."')";
    }
    
    /*
     * Category & Tag -> Query type = Override
     */
    if($op == 'category' ||
       $op == 'tag')
    {
      // get first term
      if($op == 'category') {
        $terms = get_the_category();
        if(is_array($terms)){
           $term = array_pop($terms);  
          // override options
          $options_override['args']['filters']['categories']['type'] = 'categories';
          $options_override['args']['filters']['categories']['cat_operator'] = 'cat';
          $options_override['args']['filters']['categories']['cats'] = array($term->term_id => $term->name);
        }
      }
      else if ($op == 'tag'){
        $terms = get_the_tags();
        if(is_array($terms)){
           $term = array_pop($terms);  
          // override options
          $options_override['args']['filters']['tags']['type'] = 'tags';
          $options_override['args']['filters']['tags']['tag_operator'] = 'tag__in';
          $options_override['args']['filters']['tags']['tags'] = array($term->term_id => $term->name);
        }
      }
      
      if(isset($term)){
        $sql = "SELECT qw.id,qw.name,qw.slug,qw.type,qw.data
                FROM ".$table." as qw
                LEFT JOIN ".$wpdb->prefix."query_override_terms as ot ON ot.query_id = qw.id
                WHERE qw.type = 'override' AND ot.term_id = ".$term->term_id."
                LIMIT 1";
      }
    }
    
    if(isset($sql)) {
      $rows = $wpdb->get_results($sql);
    }
    
    // if we found a path, lets use it
    if(is_object($rows[0]) && $query = $rows[0])
    {
      // pass the paged value into the query
      $options_override['args']['paged'] = $paged;
    
      // actual options 
      $options = unserialize($query->data);
      
      // resulting query
      $themed_query = qw_execute_query($query->id, $options_override, false);
      
      // post title
      if($op == '404'){
        // The title of the query
        $post_title = ($options['display']['title']) ? $options['display']['title'] : $query->name;
      }
      else {
        // categories and tags use term name for title
        $post_title = $term->name;
      }
      
      // remove the autop filter to prevent paragraph randomly being added
      remove_filter( 'the_content', 'wpautop' );
      
      // Make the post object
      $post = new stdClass();
        $post->ID           = -42;  // Arbitrary post id
        $post->post_title   = $post_title;
        $post->post_content =  $themed_query;
        $post->post_status  = 'publish'; 
        $post->post_type    = 'page'; 
        $post->post_category= array('uncategorized');
        $post->post_excerpt = ''; 
        
      // set some query information
      $wp_query->queried_object = $post;
      $wp_query->post           = $post;
      $wp_query->found_posts    = true;
      $wp_query->post_count     = true;
      //$wp_query->max_num_pages = true;
      $wp_query->is_single      = true;
      $wp_query->is_posts_page  = true;
      $wp_query->is_page        = true;
      $wp_query->posts          = array($post);
      $wp_query->is_404         = false;
      $wp_query->is_post        = false;
      $wp_query->is_home        = false;
      $wp_query->is_archive     = false;
      $wp_query->is_category    = false;
      
      // According to http://codex.wordpress.org/Plugin_API/Action_Reference
      // we can safely exit here. The template will take care of the rest.
      // chosen template path
      $template_path = TEMPLATEPATH."/".$options['display']['page']['template-file'];
      include($template_path);
      exit();
    }
  }
}
