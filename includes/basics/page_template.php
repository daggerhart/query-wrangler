<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_page_template' );

/*
 * Basic Settings
 */
function qw_basic_settings_page_template( $basics ) {
	$basics['page_template'] = array(
		'title'               => 'Page Template',
		'option_type'         => 'display',
		'description'         => 'Select which page template should wrap this query page.',
		'form_callback'       => 'qw_basic_page_template_form',
		'query_display_types' => array( 'page', 'override' ),
		'weight'              => 0,
	);

	return $basics;
}

function qw_basic_page_template_form( $basic, $display ) {
	$page_templates = get_page_templates();
	?>
	<select class='qw-js-title'
	        name="<?php print $basic['form_prefix']; ?>[page][template-file]"
	        id="qw-page-template">
		<option value="__none__">None - Allow theme to determine template
		</option>
		<option value="index.php">Default - index.php</option>
		<?php
		foreach ( $page_templates as $name => $file ) {
			$selected = ( $file == $display['page']['template-file'] ) ? 'selected="selected"' : '';
			?>
			<option value="<?php print $file; ?>"
				<?php print $selected; ?>>
				<?php print $name; ?>
			</option>
		<?php
		}
		?>
	</select>
<?php
}