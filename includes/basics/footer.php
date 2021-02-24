<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_footer' );

/*
 * Basic Settings
 */
function qw_basic_settings_footer( $basics ) {

	$basics['footer'] = array(
		'title'         => 'Footer',
		'option_type'   => 'display',
		'description'   => 'The content placed here will appear below the resulting query.',
		'form_callback' => 'qw_basic_footer_form',
		'weight'        => 0,
	);

	return $basics;
}

function qw_basic_footer_form( $basic, $display ) {
	$footer = isset( $display['footer'] ) ? $display['footer'] : "";
	$settings = !empty( $display['footer_settings'] ) ? $display['footer_settings'] : [];
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<textarea name="<?php print $basic['form_prefix']; ?>[footer]"
	          class="qw-field-textarea qw-js-title"><?php print qw_textarea( $footer ); ?></textarea>
	<p>
		<label for="hide-footer-empty">
			<input
				type="checkbox"
				id="hide-footer-empty"
				name="<?php print $basic['form_prefix'] ?>[footer_settings][hide_if_empty]"
				value="1"
				<?php checked( !empty($settings['hide_if_empty'] ) ) ?>
			>
			<?php _e('Hide footer when query has no results') ?>
		</label>
	</p>
<?php
}

/*
 * Hide the footer if the query result is empty.
 */
add_filter( 'qw_pre_render', function( $options, $wp_query ) {
	if ( !empty( $options['display']['footer_settings']['hide_if_empty'] ) && !count($wp_query->posts) ) {
		$options['meta']['footer'] = '';
	}

	return $options;
}, 0, 2 );
