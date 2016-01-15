<?php

class QW_Settings {

	private $option_name = 'qw_settings';

	public $default_settings = array(
		'edit_theme'               => QW_DEFAULT_THEME,
		'widget_theme_compat'      => 0,
		'live_preview'             => 0,
		'show_silent_meta'         => 0,
		'meta_value_field_handler' => 0,
		'shortcode_compat'         => 0,
	);

	public $values = array();

	/**
	 * QW_Settings constructor.
	 */
	private function __construct() {
		$saved = get_option( $this->option_name, false );

		if ( !$saved ){
			$saved = $this->unify_old_settings();
		}

		$this->values = array_replace( $this->default_settings, $saved );
	}

	/**
	 * Singleton
	 *
	 * @return QW_Settings
	 */
	static public function get_instance(){
		static $instance = null;
		if ( is_null( $instance ) ) {
			$instance = new self();
		}

		return $instance;
	}

	/**
	 * Get a single setting
	 *
	 * @param $key
	 * @param bool|FALSE $default
	 *
	 * @return bool
	 */
	function get( $key, $default = false ){
		if ( isset( $this->values[ $key ] ) ){
			return $this->values[ $key ];
		}

		return $default;
	}

	/**
	 * Set a value
	 *
	 * @param $key
	 * @param $value
	 * @param string $sanitize_callback
	 */
	function set( $key, $value, $sanitize_callback = 'sanitize_text_field' ){
		if ( is_callable( $sanitize_callback ) ){
			$value = call_user_func( $sanitize_callback, $value );
		}

		$this->values[ $key ] = $value;
	}

	/**
	 * Save the current values
	 */
	function save(){
		update_option( $this->option_name, $this->values );
	}

	/**
	 * Update old multi-option settings to new single option array
	 */
	private function unify_old_settings(){
		$settings = array(
			'edit_theme'               => get_option( 'qw_edit_theme', QW_DEFAULT_THEME ),
			'live_preview'             => get_option( 'qw_live_preview', 0 ),
			'show_silent_meta'         => get_option( 'qw_show_silent_meta', 0 ),
			'meta_value_field_handler' => get_option( 'qw_meta_value_field_handler', 0 ),
			'widget_theme_compat'      => get_option( 'qw_widget_theme_compat', 0 ),
		);

		update_option( $this->option_name, $settings );

		delete_option( 'qw_edit_theme' );
		delete_option( 'qw_widget_theme_compat' );
		delete_option( 'qw_live_preview' );
		delete_option( 'qw_show_silent_meta' );
		delete_option( 'qw_meta_value_field_handler' );

		return $settings;
	}
}
