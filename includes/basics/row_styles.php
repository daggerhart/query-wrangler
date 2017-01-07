<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_row_style' );

// add default field styles to the filter
add_filter( 'qw_row_styles', 'qw_default_row_styles', 0 );

// add default field styles to the filter
add_filter( 'qw_row_fields_styles', 'qw_default_row_fields_styles', 0 );

// add default field styles to the filter
add_filter( 'qw_row_complete_styles', 'qw_default_row_complete_styles', 0 );

/*
 * Basic Settings
 */
function qw_basic_settings_row_style( $basics ) {
	$basics['display_row_style'] = array(
		'title'         => 'Row Style',
		'option_type'   => 'display',
		'description'   => 'How should each post in this query be presented?',
		'form_callback' => 'qw_basic_display_row_style_form',
		'weight'        => 0,
	);

	return $basics;
}

/*
 * Default Row Styles
 */
function qw_default_row_styles( $row_styles ) {
	$row_styles['posts']  = array(
		'title'             => 'Posts',
		'settings_callback' => 'qw_row_style_posts_settings',
		'settings_key'      => 'post',
	);
	$row_styles['fields'] = array(
		'title'             => 'Fields',
		'settings_callback' => 'qw_row_style_fields_settings',
		'settings_key'      => 'field',
	);
	$row_styles['template_part'] = array(
		'title'             => 'Template Part',
		'settings_callback' => 'qw_row_style_template_part_settings',
		'settings_key'      => 'template_part',
	);

	return $row_styles;
}


/*
 * Default Row 'Posts' Styles
 */
function qw_default_row_complete_styles( $row_complete_styles ) {
	$row_complete_styles['complete'] = array(
		'title' => 'Complete Post',
	);
	$row_complete_styles['excerpt']  = array(
		'title' => 'Excerpt',
	);

	return $row_complete_styles;
}

function qw_basic_display_row_style_form( $basic, $display ) {
	$row_styles = qw_all_row_styles();
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<select class='qw-js-title'
	        id="qw-display-type"
	        name="<?php print $basic['form_prefix']; ?>[row_style]">
		<?php
		foreach ( $row_styles as $type => $row_style ) { ?>
			<option value="<?php print $type; ?>"
				<?php if ( $display['row_style'] == $type ) {
					print 'selected="selected"';
				} ?>>
				<?php print $row_style['title']; ?>
			</option>
		<?php
		}
		?>
	</select>

	<!-- style settings -->
	<p class="description">Some Row Styles have additional settings.</p>
	<div id="row-style-settings">
		<?php
		foreach ( $row_styles as $type => $row_style ) {
			if ( isset( $row_style['settings_callback'] ) && function_exists( $row_style['settings_callback'] ) ) {
				$row_style['values'] = ( isset( $row_style['settings_key'] ) && isset( $display[ $row_style['settings_key'] . '_settings' ] ) ) ? $display[ $row_style['settings_key'] . '_settings' ] : array();
				?>
				<div
					id="tab-row-style-settings-<?php print $row_style['hook_key']; ?>"
					class="qw-query-content">
					<h3 class="qw-setting-header"><?php print $row_style['title']; ?>
						Settings</h3>

					<div class="qw-setting-group">
						<?php print $row_style['settings_callback']( $row_style,
							$display ); ?>
					</div>
				</div>
			<?php
			}
		}
		?>
	</div>
<?php
}

function qw_row_style_posts_settings( $row_style, $display ) {
	?>
	<p class="description">Select the amount of the post to be shown.</p>
	<select class="qw-js-title"
	        name="qw-query-options[display][post_settings][size]">
		<option value="complete"
			<?php if ( isset( $row_style['values']['size'] ) && $row_style['values']['size'] == "complete" ) {
				print 'selected="selected"';
			} ?>>
			Complete Post
		</option>
		<option value="excerpt"
			<?php if ( isset( $row_style['values']['size'] ) && $row_style['values']['size'] == "excerpt" ) {
				print 'selected="selected"';
			} ?>>
			Excerpt
		</option>
	</select>
<?php
}

/**
 * @param $row_style
 */
function qw_row_style_fields_settings( $row_style, $display ) {
	$query_fields = isset( $display['field_settings']['fields'] ) ? $display['field_settings']['fields'] : array();
	$group_by = !empty( $display['field_settings']['group_by_field'] ) ? $display['field_settings']['group_by_field'] : false;
	$strip = !empty( $display['field_settings']['strip_group_by_field'] ) ? true : false;
	$all_fields   = qw_all_fields();
	?>
	<p>
		<label>Group by field</label>
		<select class="qw-js-title"
		        name="qw-query-options[display][field_settings][group_by_field]">
			<option value="__none__"> - None -</option>
			<?php
			if ( ! empty( $query_fields ) ) {
				foreach ( $query_fields as $field_name => $field ) {
					?>
					<option
						value="<?php print esc_attr( $field_name ); ?>"
						<?php selected( $field_name, $group_by ); ?>><?php print $all_fields[ $field['hook_key'] ]['title']; ?> </option>
					<?php
				}
			}
			?>
		</select>
	</p>
	<p>
		<input type="hidden"
		       name="qw-query-options[display][field_settings][strip_group_by_field]"
		       value="">
		<label>
			<input type="checkbox"
			       name="qw-query-options[display][field_settings][strip_group_by_field]"
			       value="1"
			       <?php checked( $strip ); ?>
			> - Strip tags from Group by field
		</label>
	</p>
<?php
}

/**
 * @param $row_style
 */
function qw_row_style_template_part_settings( $row_style, $display ) {
	$path = isset( $row_style['values']['path'] ) ? $row_style['values']['path'] : '';
	$name = isset( $row_style['values']['name'] ) ? $row_style['values']['name'] : '';
	?>
	Path:
	<input type="text"
	       name="qw-query-options[display][template_part_settings][path]"
	       value="<?php echo esc_attr($path); ?>">

	Name:
	<input type="text"
	       name="qw-query-options[display][template_part_settings][name]"
	       value="<?php echo esc_attr($name); ?>">
	<?php
}
