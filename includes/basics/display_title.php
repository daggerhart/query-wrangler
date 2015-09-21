<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_display_title' );

/*
 * Basic Settings
 */
function qw_basic_settings_display_title( $basics ) {
	$basics['display_title'] = array(
		'title'         => 'Display Title',
		'option_type'   => 'display',
		'description'   => 'The title above the query page or widget',
		'form_callback' => 'qw_basic_display_title_form',
		'weight'        => 0,
	);

	return $basics;
}

function qw_basic_display_title_form( $basic, $display ) {
	$title = isset( $display['title'] ) ? $display['title'] : "";
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<input class="qw-text-long qw-js-title"
	       type="text"
	       name="<?php print $basic['form_prefix']; ?>[title]"
	       value="<?php print $title; ?>"/>
<?php
}