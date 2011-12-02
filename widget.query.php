<?php
/**
 * Add function to widgets_init that'll load our widget.
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
  function Query_Wrangler_Widget()
  {
    // Widget settings. 
    $widget_ops = array( 'classname' => 'query-wrangler-widget', 'description' => __('A Query Wrangler Widget', 'qw-widget') );
    
    // Widget control settings. 
    $control_ops = array( 'id_base' => 'query-wrangler-widget' );
    
    // Create the widget. 
    $this->WP_Widget( 'query-wrangler-widget', __('QW Widget', 'querywranglerwidget'), $widget_ops, $control_ops );
  }
  /**
   * How to display the widget on the screen.
   */
  function widget( $args, $instance )
  {
    extract( $args );
    print qw_execute_query($instance['qw-widget']);
  }
	/**
	 * Update the widget settings.
	 */
	function update( $new_instance, $old_instance )
 {
    $instance = $old_instance; 
    $instance['title'] = $new_instance['title'];
    $instance['qw-widget'] = $new_instance['qw-widget'];
    return $instance;
	}
  /**
   * Displays the widget settings controls on the widget panel.
   * Make use of the get_field_id() and get_field_name() function
   * when creating your form elements. This handles the confusing stuff.
   */
  function form( $instance )
  {
    // Set up some default widget settings. 
    $defaults = array( 'title' => __('QW Widget', 'querywranglerwidget'), 'qw-widget' => '' );
    $instance = wp_parse_args( (array) $instance, $defaults );
    $widgets = qw_get_all_widgets();
    ?>
    <?php // Widget Title: Hidden Input ?>
    <input type="hidden" id="<?php print $this->get_field_id( 'title' ); ?>" name="<?php print $this->get_field_name( 'title' ); ?>" value="<?php print $widgets[$instance['qw-widget']]; ?>" style="width:100%;" />
    
    <?php // Widget: Select Box ?>
    <p>
      <label for="<?php print $this->get_field_id( 'qw-widget' ); ?>">
        <?php _e('Query Widget:', 'qw-widget'); ?>
      </label> 
     <select id="<?php print $this->get_field_id( 'qw-widget' ); ?>" name="<?php print $this->get_field_name( 'qw-widget' ); ?>" class="widefat" style="width:100%;">
      <?php
        $selected_query_id = 0;
        foreach($widgets as $query_id => $name)
        {
          if ($instance['qw-widget'] == $query_id) {
            $query_selected = 'selected="selected"';
            $selected_query_id = $query_id;
          }
          else {
            $query_selected = '';
          }
          ?>
          <option <?php print $query_selected; ?> value="<?php print $query_id; ?>"><?php print $name; ?></option>
          <?php
        }
      ?>
     </select>
    </p>
    <p>
      <a href="<?php print get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$selected_query_id; ?>">Edit this Query</a>
    </p>
    <?php
  }
}
