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
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<textarea name="<?php print $basic['form_prefix']; ?>[header]"
	          class="qw-field-textarea qw-js-title"><?php print qw_textarea( $header ); ?></textarea>
<?php
}