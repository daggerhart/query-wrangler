<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_ignore_sticky_posts' );

/*
 * Basic Settings
 */
function qw_basic_ignore_sticky_posts( $basics ) {

	$basics['ignore_sticky_posts'] = array(
		'title'         => 'Ignore Sticky Posts',
		'option_type'   => 'args',
		'description'   => 'Do not enforce stickiness in the resulting query.',
		'form_callback' => 'qw_basic_ignore_sticky_posts_form',
	);

	return $basics;
}

function qw_basic_ignore_sticky_posts_form( $basic, $args ) {
	$value = isset( $args['ignore_sticky_posts'] ) ? $args['ignore_sticky_posts'] : 0;
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<label><input class="qw-text-short qw-js-title"
	              type="checkbox"
	              name="<?php print $basic['form_prefix']; ?>[ignore_sticky_posts]"
			<?php checked( $value, 1 ); ?>
	              value="1"/>
		<?php print $basic['title']; ?>
	</label>
<?php
}