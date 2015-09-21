<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_offset' );

/*
 * Basic Settings
 */
function qw_basic_settings_offset( $basics ) {

	$basics['offset'] = array(
		'title'         => 'Offset',
		'option_type'   => 'args',
		'description'   => 'Number of post to skip, or pass over. For example, if this field is 3, the first 3 items will be skipped and not displayed.',
		'form_callback' => 'qw_basic_offset_form',
		'weight'        => 0,
	);

	return $basics;
}

function qw_basic_offset_form( $basic, $args ) {
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<input class="qw-text-short qw-js-title"
	       type="text"
	       name="<?php print $basic['form_prefix']; ?>[offset]"
	       value="<?php print $args['offset']; ?>"/>
<?php
}