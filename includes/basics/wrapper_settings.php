<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_wrapper_settings' );

/*
 * Basic Settings
 */
function qw_basic_settings_wrapper_settings( $basics ) {
	$basics['wrapper_classes'] = array(
		'title'         => 'Wrapper Classes',
		'option_type'   => 'display',
		'description'   => 'The CSS class names will be added to the query. This enables you to use specific CSS code for each query. You may define multiples classes separated by spaces.',
		'form_callback' => 'qw_basic_wrapper_classes_form',
		'weight'        => 0,
	);

	return $basics;
}

function qw_basic_wrapper_classes_form( $basic, $display ) {
	$wrapper_classes = isset( $display['wrapper-classes'] ) ? $display['wrapper-classes'] : "";
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<input class="qw-text-long qw-js-title"
	       type="text"
	       name="<?php print $basic['form_prefix']; ?>[wrapper-classes]"
	       value="<?php print $wrapper_classes; ?>"/>
<?php
}