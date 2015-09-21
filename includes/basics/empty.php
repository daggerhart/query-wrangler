<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_empty' );

/*
 * Basic Settings
 */
function qw_basic_settings_empty( $basics ) {
	$basics['empty'] = array(
		'title'         => 'Empty Text',
		'option_type'   => 'display',
		'description'   => 'The content placed here will appear if the query has no results.',
		'form_callback' => 'qw_basic_empty_form',
		'weight'        => 0,
	);

	return $basics;
}


function qw_basic_empty_form( $basic, $display ) {
	$empty = isset( $display['empty'] ) ? $display['empty'] : "";
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<textarea name="<?php print $basic['form_prefix']; ?>[empty]"
	          class="qw-field-textarea qw-js-title"><?php print qw_textarea( $empty ); ?></textarea>
<?php
}