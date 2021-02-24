<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_header' );

/*
 * Basic Settings
 */
function qw_basic_settings_header( $basics ) {
	$basics['header'] = array(
		'title'         => 'Header',
		'option_type'   => 'display',
		'description'   => 'The content placed here will appear above the resulting query.',
		'form_callback' => 'qw_basic_header_form',
		'weight'        => 0,
	);

	return $basics;
}

function qw_basic_header_form( $basic, $display ) {
	$header = isset( $display['header'] ) ? $display['header'] : "";
	$settings = !empty( $display['header_settings'] ) ? $display['header_settings'] : [];
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<textarea name="<?php print $basic['form_prefix']; ?>[header]"
	          class="qw-field-textarea qw-js-title"><?php print qw_textarea( $header ); ?></textarea>
	<p>
		<label for="hide-header-empty">
			<input
				type="checkbox"
				id="hide-header-empty"
				name="<?php print $basic['form_prefix'] ?>[header_settings][hide_if_empty]"
				value="1"
				<?php checked( !empty($settings['hide_if_empty'] ) ) ?>
			>
			<?php _e('Hide header when query has no results') ?>
		</label>
	</p>
<?php
}

/*
 * Hide the header if the query result is empty.
 */
add_filter( 'qw_pre_render', function( $options, $wp_query ) {
	if ( !empty( $options['display']['header_settings']['hide_if_empty'] ) && !count($wp_query->posts) ) {
		$options['meta']['header'] = '';
	}

	return $options;
}, 0, 2 );
