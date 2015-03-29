<?php

/**
 * Class QW_Override
 *
 * Controls the execution of an override.  After the main query has been executed
 * this loops through all override types and allows them to detect if an override
 * should be executed.  The override types are expected to return a QW_Query object
 */
class QW_Override {

  function __construct() {
    if ( ! is_admin() ) {
      //add_action('parse_query', 'qw_parse_query');
      add_action('wp', array( $this, 'action_wp' ) );
    }
  }

  /**
   * WordPress action 'wp'
   *
   * After the main query has been executed, but no output has been generated
   * @param $wp
   */
  function action_wp( $wp ){
    global $wp_query;

    $overrides = qw_all_overrides();

    // Loop through all override types and let them look for their own active overrides
    foreach ( $overrides as $override ){
      if ( isset( $override['get_query_callback'] ) && is_callable($override['get_query_callback'] ) ) {

        // override get_query_callbacks should return a QW_Query object
        $qw_query = call_user_func( $override['get_query_callback'] );

        if ( $qw_query && is_a( $qw_query, 'QW_Query' ) ) {
          // execute the override
          $this->execute( $qw_query );
        }
      }
    }
  }

  /**
   * Inject our $qw_query into the global $wp_query as a single post. That way
   * we have full control over the output of the content, while not interrupting
   * the theme's template hierarchy.
   *
   * @param $qw_query
   */
  function execute( $qw_query ){
    global $wp_query;

    $themed_query = $qw_query->execute();

    // The title of the query
    $title = ($qw_query->options['display']['title']) ? $qw_query->options['display']['title'] : $qw_query->name;


    // Make the post object
    $faux_post = new stdClass();
    $faux_post->ID           = -42;  // Arbitrary post id
    $faux_post->post_title   =  $title;
    $faux_post->post_content =  $themed_query;
    $faux_post->post_status  = 'publish';
    $faux_post->post_type    = 'post';
    $faux_post->post_category= array('uncategorized');
    $faux_post->post_excerpt = '';
    $faux_post->ancestors    = array();


    // hack the gibson
    $wp_query->posts          = array( $faux_post );
    $wp_query->post           = $faux_post;
    $wp_query->post_count     = 1;
    $wp_query->found_posts    = true;


//    $wp_query->is_single      = true;
//    $wp_query->is_posts_page  = true;
//    $wp_query->is_page        = true;
//
//    $wp_query->is_404         = false;
//    $wp_query->is_post        = false;
//    $wp_query->is_home        = false;
//    $wp_query->is_archive     = false;
//    $wp_query->is_category    = false;
  }
}