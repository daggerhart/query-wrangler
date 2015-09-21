<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_post_status' );

// add default fields to the hook filter
add_filter( 'qw_post_statuses', 'qw_default_post_statuses', 0 );

/*
 * Basic Settings
 */
function qw_basic_settings_post_status( $basics ) {

	$basics['post_status'] = array(
		'title'         => 'Posts Status',
		'option_type'   => 'args',
		'description'   => 'Select the post status of the items displayed.',
		'form_callback' => 'qw_basic_post_status_form',
		'weight'        => 0,
	);

	return $basics;
}

/*
 * Post statuses as a hook for contributions
 */
function qw_default_post_statuses( $post_statuses ) {
	$post_statuses['publish'] = array(
		'title' => 'Published',
	);
	$post_statuses['pending'] = array(
		'title' => 'Pending',
	);
	$post_statuses['draft']   = array(
		'title' => 'Draft',
	);
	$post_statuses['future']  = array(
		'title' => 'Future (Scheduled)',
	);
	$post_statuses['trash']   = array(
		'title' => 'Trashed',
	);
	$post_statuses['private'] = array(
		'title' => 'Private',
	);
	$post_statuses['any']     = array(
		'title' => 'Any',
	);

	return $post_statuses;
}

function qw_basic_post_status_form( $basic, $args ) {
	$post_statuses = qw_all_post_statuses();
	?>
	<p class="description"><?php print $basic['description']; ?></p>
	<select id="qw-post-status"
	        class='qw-js-title'
	        name="<?php print $basic['form_prefix']; ?>[post_status]">
		<?php
		foreach ( $post_statuses as $key => $post_status ) { ?>
			<option value="<?php print $key; ?>"
				<?php if ( $args['post_status'] == $key ) {
					print 'selected="selected"';
				} ?>>
				<?php print $post_status['title']; ?>
			</option>
		<?php
		}
		?>
	</select>
<?php
}
