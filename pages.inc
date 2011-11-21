<?php
add_action('wp', 'qw_query_pages_handler');
/*
 * Handle Query pages
 * ref: http://www.blogseye.com/2010/05/creating-fake-wordpress-posts-on-the-fly/
 */
function qw_query_pages_handler() {
    global $wp_query,$wpdb;
    
    // Custom Permalinks method
    // $request = ltrim(substr($_SERVER['REQUEST_URI'], strlen($url)),'/');

    // Look for a 404 result
    if($wp_query->is_404 )
    {
      // TODO: how to look for query pages when /page/2
      //print_r($wp_query->query_vars);exit();
      
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
      
      // take into account trailing slash
      if(substr($path, strlen($path)-1, 1) != '/'){
        $alt_path = $path.'/';
      } else {
        $alt_path = substr($path, 0, strlen($path)-1);
      }
      
      // Look for the query path given
      $table_name = $wpdb->prefix."query_wrangler";
      $sql = "SELECT id,name,path,data FROM ".$table_name." WHERE type = 'page' AND (path = '".$path."' OR path = '".$alt_path."')";
      $rows = $wpdb->get_results($sql);
      
      // if we found a path, lets use it
      if(is_object($rows[0]) && $query = $rows[0])
      {
        // handle paging
        if ( get_query_var('paged') ) {
          $paged = get_query_var('paged');
        } elseif ( get_query_var('page') ) {
          $paged = get_query_var('page');
        } else {
          $paged = 1;
        }
        
        // pass the paged value into the query
        $options_override['args']['paged'] = $paged;
      
        // actual options 
        $options = unserialize($query->data);
        
        //print_r($options);exit();
        
        // resulting query
        $themed = qw_execute_query($query->id, $options_override, false);
        
        // chosen template path
        $template_path = TEMPLATEPATH."/".$options['display']['page']['template-file'];
        
        // Make the post object
        $post = new stdClass();
          // Arbitrary post id
          $post->ID= -42;
          // The title of the query
          $post->post_title  = ($options['display']['title']) ? $options['display']['title'] : $query->name;
          // Content is themed query 
          $post->post_content =  $themed;
          $post->post_status = 'publish'; 
          $post->post_type = 'page'; 
          $post->post_category = array('uncategorized');
          $post->post_excerpt = ''; 
          
        // set some query information
        $wp_query->queried_object = $post;
        $wp_query->post = $post;
        $wp_query->found_posts = 1;
        $wp_query->post_count = 1;
        //$wp_query->max_num_pages = 1;
        $wp_query->is_single = 1;
        $wp_query->is_404 = false;
        $wp_query->is_posts_page = 1;
        $wp_query->posts = array($post);
        $wp_query->is_post = false;
        $wp_query->is_page = true;
        $wp_query->is_home = false;
  	     $wp_query->is_archive = false;
  	     $wp_query->is_category = false;
        
        // According to http://codex.wordpress.org/Plugin_API/Action_Reference
        // we can safely exit here. The template will take care of the rest.
        include($template_path);
        exit();
      }
    }
}