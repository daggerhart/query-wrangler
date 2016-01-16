<?php

/**
 * Class QW_Override
 *
 * Controls the execution of an override.  After the main query has been
 * executed this loops through all override types and allows them to detect if
 * an override should be executed.  The override types are expected to return a
 * QW_Query object
 */
class QW_Override {

	private $override_query = NULL;

	/**
	 * Hook into WordPress
	 */
	public static function register() {
		$self = new self();

		if ( ! is_admin() ) {
			add_action( 'pre_get_posts', array( $self, 'action_pre_get_posts' ) );
			add_action( 'wp', array( $self, 'action_wp' ) );
		}
	}

	/**
	 * WordPress action 'pre_get_post'
	 *
	 * Before the main query is executed, look for an active override.
	 *
	 * @param $wp_query
	 */
	function action_pre_get_posts( $wp_query ) {
		if ( ! $wp_query->is_main_query() ) {
			return;
		}

		$overrides = qw_all_overrides();

		// Loop through all override types and let them look for their own active overrides
		foreach ( $overrides as $override ) {
			if ( isset( $override['get_query_callback'] ) && is_callable( $override['get_query_callback'] ) ) {

				// override get_query_callbacks should return a QW_Query object
				$qw_query = call_user_func( $override['get_query_callback'],
					$wp_query );

				if ( $qw_query && is_a( $qw_query, 'QW_Query' ) ) {
					$this->override_query = $qw_query;

					// go ahead and correct pagination
					$wp_query->set( 'posts_per_page',
						$qw_query->data['args']['posts_per_page'] );

					// !first one wins
					break;
				}
			}
		}
	}

	/**
	 * WordPress action 'wp'
	 *
	 * After the main query has been executed but no output has been generated,
	 * execute an override query that was found during pre_get_posts
	 *
	 * @param $wp
	 */
	function action_wp( $wp ) {
		if ( $this->override_query ) {
			// execute the override
			$this->execute( $this->override_query );
		}
	}

	/**
	 * Inject our $qw_query into the global $wp_query as a single post. That
	 * way
	 * we have full control over the output of the content, while not
	 * interrupting the theme's template hierarchy.
	 *
	 * @param $qw_query
	 */
	function execute( $qw_query ) {
		// process the query and get the output
		$themed_query = $qw_query->execute()->output;

		// The title of the query
		$title = ( $qw_query->options['display']['title'] ) ? $qw_query->options['display']['title'] : $qw_query->name;

		// Make the post object
		$faux_post                = new stdClass();
		$faux_post->ID            = -42;  // Arbitrary post id
		$faux_post->post_title    = $title;
		$faux_post->post_content  = $themed_query;
		$faux_post->post_status   = 'publish';
		$faux_post->post_type     = 'qw-override';
		$faux_post->post_category = array( 'uncategorized' );
		$faux_post->post_excerpt  = '';
		$faux_post->ancestors     = array();

		// hack the gibson
		global $wp_query;
		$wp_query->posts = array( $faux_post );
//    $wp_query->post           = $faux_post;
		$wp_query->found_posts = 1;
		$wp_query->post_count  = 1;

		// allow for page templates
		if ( $qw_query->options['display']['page']['template-file'] !== '__none__' ) {
			add_filter( 'template_include', array( $this, 'hijack_template' ), 99 );
		}
	}

	/**
	 * Take over the page with a given template file.
	 * Include the template and exit immediately
	 *
	 * @param $template_file
	 *
	 * @return string
	 */
	function hijack_template( $template_file ) {
		$template_file = locate_template( $this->override_query->options['display']['page']['template-file'] );

		if ( ! file_exists( $template_file ) ) {
			$template_file = locate_template( qw_default_template_file() );
		}

		return $template_file;
	}
}