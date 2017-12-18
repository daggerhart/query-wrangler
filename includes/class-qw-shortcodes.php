<?php

class QW_Shortcodes {

	/**
	 * Register hooks with wordpress
	 */
	static public function register(){
		$self = new self();

		$settings = QW_Settings::get_instance();

		if ( $settings->get('shortcode_compat') ){
			add_shortcode( 'qw_query', array( $self, 'query_shortcode' ) );
		}
		else {
			add_shortcode( 'query', array( $self, 'query_shortcode' ) );
		}

		add_filter( 'qw_shortcode_default_attributes', array( $self, 'arguments_default_attributes') );
		add_filter( 'qw_shortcode_options', array( $self, 'arguments_contextual_tokens' ), 10, 2 );
		add_filter( 'qw_pre_query', array( $self, 'arguments_pre_query' ), 10, 2 );
	}

	/**
	 * Shortcode support for all queries
	 *
	 * @param $input_atts
	 *
	 * @return string
	 */
	function query_shortcode( $input_atts ) {
		$options_override = array();

		// provide additional default shortcode attributes
		$default_atts = apply_filters( 'qw_shortcode_default_attributes', array( 'id' => '', 'slug' => '' ) );

		$atts = shortcode_atts( $default_atts, $input_atts );

		if ( ! $atts['id'] && $atts['slug'] ) {
			$atts['id'] = qw_get_query_by_slug( $atts['slug'] );
		}

		// alter the attributes
		$atts = apply_filters( 'qw_shortcode_attributes', $atts, $options_override );

		// alter the options provided to the query
		$options_override = apply_filters( 'qw_shortcode_options', $options_override, $atts );

		$themed = qw_execute_query( $atts['id'], $options_override );

		return $themed;
	}

	/**
	 * Allow arguments to be passed into query shortcodes.
	 * This way, users have control over individual shortcode instances.
	 *
	 * @param $default_atts
	 *
	 * @return array
	 */
	function arguments_default_attributes( $default_atts ) {
		$default_atts['args'] = '';

		return $default_atts;
	}

	/**
	 * Replace contextual tokens with their values
	 *
	 * @param $options
	 * @param $attributes
	 *
	 * @return array
	 */
	function arguments_contextual_tokens( $options, $attributes ) {
		if ( isset( $attributes['args'] ) && ! empty( $attributes['args'] ) ) {

			if ( stripos( $attributes['args'], '{{' ) !== FALSE ) {
				$attributes['args'] = qw_contextual_tokens_replace( $attributes['args'] );
			}

			$options['shortcode_args'] = html_entity_decode( $attributes['args'] );
		}

		return $options;
	}

	/**
	 * Modify the query by parsing shortcode arguments and merge into query args
	 *
	 * @param $query_args
	 * @param $options
	 *
	 * @return array
	 */
	function arguments_pre_query( $query_args, $options ) {

		if ( isset( $options['shortcode_args'] ) ) {
			$shortcode_args = wp_parse_args( $options['shortcode_args'] );
			$query_args     = array_replace_recursive( (array) $query_args,
					$shortcode_args );
		}

		return $query_args;
	}
}
