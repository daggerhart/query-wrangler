<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_style' );
// add default template styles to the hook
add_filter( 'qw_styles', 'qw_template_styles_default' );

/*
 * Styles with settings
 */
function qw_basic_settings_style( $basics ) {
	$basics['style'] = array(
		'title'         => 'Template Style',
		'option_type'   => 'display',
		'description'   => 'How should this query be styled?',
		'form_callback' => 'qw_basic_display_style_form',
		'weight'        => 0,
	);

	return $basics;
}

/*
 * All Field Styles and settings
 *
 * @return array Field Styles
 */
function qw_template_styles_default( $styles ) {
	$styles['unformatted']    = array(
		'title'        => 'Unformatted',
		'template'     => 'query-unformatted',
		'default_path' => QW_PLUGIN_DIR, // do not include last slash
	);
	$styles['unordered_list'] = array(
		'title'        => 'Unordered List',
		'template'     => 'query-unordered_list',
		'default_path' => QW_PLUGIN_DIR, // do not include last slash
	);
	$styles['ordered_list']   = array(
		'title'        => 'Ordered List',
		'template'     => 'query-ordered_list',
		'default_path' => QW_PLUGIN_DIR, // do not include last slash
	);
	$styles['table']          = array(
		'title'        => 'Table',
		'template'     => 'query-table',
		'default_path' => QW_PLUGIN_DIR, // do not include last slash
	);

	return $styles;
}

function qw_basic_display_style_form( $basic, $display ) {
	$styles = qw_all_styles();
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<select class='qw-js-title'
	        name="<?php print $basic['form_prefix']; ?>[style]"
	        id="query-display-style">
		<?php
		// loop through field styles
		foreach ( $styles as $type => $style ) {
			?>
			<option value="<?php print $type; ?>"
				<?php if ( $display['style'] == $type ) {
					print 'selected="selected"';
				} ?>>
				<?php print $style['title']; ?>
			</option>
		<?php
		}
		?>
	</select>

	<!-- style settings -->
	<div id="display-style-settings">
		<?php
		foreach ( $styles as $type => $style ) {
			if ( isset( $style['settings_callback'] ) && function_exists( $style['settings_callback'] ) ) {
				$style['values'] = $display[ $style['settings_key'] ];
				?>
				<div id="tab-style-settings-<?php print $style['hook_key']; ?>"
				     class="qw-query-content">
					<span
						class="qw-setting-header"><?php print $style['title']; ?>
						Settings</span>

					<div class="qw-setting-group">
						<?php print $style['settings_callback']( $style ); ?>
					</div>
				</div>
			<?php
			}
		}
		?>
	</div>
<?php
}