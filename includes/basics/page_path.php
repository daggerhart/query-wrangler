<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_page_path' );

/*
 * Basic Settings
 */
function qw_basic_settings_page_path( $basics ) {
	$basics['page_path'] = array(
		'title'               => 'Page path',
		'option_type'         => 'display',
		'description'         => 'The path or permalink you want this page to use. Avoid using spaces and capitalization for best results.',
		'form_callback'       => 'qw_basic_page_path_form',
		'query_display_types' => array( 'page', ),
		'weight'              => 0,
	);

	return $basics;
}

function qw_basic_page_path_form( $basic, $display ) {
	$query_page_path = isset( $display['page']['path'] ) ? $display['page']['path'] : "";
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<input class='qw-js-title'
	       size="60"
	       type="text"
	       name="<?php print $basic['form_prefix']; ?>[page][path]"
	       value="<?php print $query_page_path; ?>"/>
<?php
}