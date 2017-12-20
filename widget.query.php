<?php
/**
 * Add function to widgets_init that'll load our widget.
 *
 * @since 0.1
 */
add_action( 'widgets_init', 'qw_wp_widget' );
/**
 * Register our widget.
 * 'Query_Wrangler_Widget' is the widget class used below.
 *
 * @since 0.1
 */
function qw_wp_widget() {
	register_widget( 'Query_Wrangler_Widget' );
}

/**
 * Query Wrangler Widget class.
 * This class handles everything that needs to be handled with the widget:
 * the settings, form, display, and update.  Nice!
 *
 * @since 0.1
 */
class Query_Wrangler_Widget extends WP_Widget {
	/**
	 * Widget setup.
	 */
	function __construct() {
		// Widget settings.
		$widget_ops = array(
			'classname'   => 'query-wrangler-widget',
			'description' => __( 'A Query Wrangler Widget',
				'qw-widget' )
		);

		// Widget control settings.
		$control_ops = array( 'id_base' => 'query-wrangler-widget' );

		// Create the widget.
		parent::__construct( 'query-wrangler-widget',
			__( 'Query Wrangler Widget', 'querywranglerwidget' ),
			$widget_ops,
			$control_ops );
	}

	/**
	 * How to display the widget on the screen.
	 */
	function widget( $args, $instance ) {
		$output           = '';
		$options_override = array();
		$settings = QW_Settings::get_instance();

		if ( isset( $instance['qw-widget'] ) && ! empty( $instance['qw-widget'] ) ) {

			// shortcode args
			if ( isset( $instance['qw-shortcode-args'] ) && ! empty( $instance['qw-shortcode-args'] ) ) {
				if ( stripos( $instance['qw-shortcode-args'], '{{' ) !== FALSE ) {
					$instance['qw-shortcode-args'] = qw_contextual_tokens_replace( $instance['qw-shortcode-args'] );
				}
				$options_override['shortcode_args'] = html_entity_decode( $instance['qw-shortcode-args'] );
			}

			$options = qw_generate_query_options( $instance['qw-widget'] );
			$widget_content = qw_execute_query( $instance['qw-widget'], $options_override );

			// pre_render hook
			$options = apply_filters( 'qw_pre_render', $options );

			$show_title = ( isset( $instance['qw-show-widget-title'] ) && ! empty( $instance['qw-show-widget-title'] ) );
			$title      = ( $show_title && $options['display']['title'] ) ? $args['before_title'] . $options['display']['title'] . $args['after_title'] : '';


			if ( $settings->get( 'widget_theme_compat' ) ) {
				$output = $args['before_widget'] .
				            $title .
				            $widget_content .
				          $args['after_widget'];

			}
			else {
				$output = $title . $widget_content;
			}
		}
		print $output;
	}

	/**
	 * Update the widget settings.
	 */
	function update( $new_instance, $old_instance ) {
	    $instance = array_replace( array(
            'title' => '',
		    'qw-widget' => '',
		    'qw-shortcode-args' => '',
		    'qw-show-widget-title' => '',
        ), $old_instance, $new_instance );

		return $instance;
	}

	/**
	 * Displays the widget settings controls on the widget panel.
	 * Make use of the get_field_id() and get_field_name() function
	 * when creating your form elements. This handles the confusing stuff.
	 */
	function form( $instance ) {
		// Set up some default widget settings.
		$defaults          = array(
			'title'                => __( 'QW Widget',
				'querywranglerwidget' ),
			'qw-widget'            => '',
			'qw-shortcode-args'    => '',
			'qw-show-widget-title' => ''
		);
		$instance          = wp_parse_args( (array) $instance, $defaults );
		$widgets           = qw_get_all_widgets();
		$this_widget_value = isset( $widgets[ $instance['qw-widget'] ] ) ? $widgets[ $instance['qw-widget'] ] : '';
		?>
		<?php // Widget Title: Hidden Input
		?>
		<input type="hidden" id="<?php print $this->get_field_id( 'title' ); ?>"
		       name="<?php print $this->get_field_name( 'title' ); ?>"
		       value="<?php print $this_widget_value; ?>" style="width:100%;"/>
		<p>
			<label><input type="checkbox"
			              id="<?php print $this->get_field_id( 'qw-show-widget-title' ); ?>"
			              name="<?php print $this->get_field_name( 'qw-show-widget-title' ); ?>" <?php checked( $instance['qw-show-widget-title'],
					'on' ); ?> /> Show query's Display Title as Widget
				Title</label>
		</p>

		<?php // Widget: Select Box
		?>
		<p>
			<label for="<?php print $this->get_field_id( 'qw-widget' ); ?>">
				<?php _e( 'Query Widget:', 'qw-widget' ); ?>
			</label>
			<select id="<?php print $this->get_field_id( 'qw-widget' ); ?>"
			        name="<?php print $this->get_field_name( 'qw-widget' ); ?>"
			        class="widefat" style="width:100%;">
				<option value="-none-">- Select -</option>
				<?php
				$selected_query_id = 0;
				foreach ( $widgets as $query_id => $name ) {
					if ( $instance['qw-widget'] == $query_id ) {
						$query_selected    = 'selected="selected"';
						$selected_query_id = $query_id;
					} else {
						$query_selected = '';
					}
					?>
					<option <?php print $query_selected; ?>
						value="<?php print $query_id; ?>"><?php print $name; ?></option>
				<?php
				}
				?>
			</select>
		</p>
		<p>
			<a href="<?php print get_bloginfo( 'wpurl' ) . '/wp-admin/admin.php?page=query-wrangler&edit=' . $selected_query_id; ?>">Edit
				this Query</a>
		</p>
		<p>
			<label
				for="<?php print $this->get_field_id( 'qw-shortcode-args' ); ?>">
				<?php _e( 'Contextual Arguments:', 'qw-shortcode-args' ); ?>
			</label>
			<input
				id="<?php print $this->get_field_id( 'qw-shortcode-args' ); ?>"
				name="<?php print $this->get_field_name( 'qw-shortcode-args' ); ?>"
				class="widefat"
				style="width:100%;"
				value="<?php print sanitize_text_field( $instance['qw-shortcode-args'] ); ?>"/>
		</p>
	<?php
	}
}
