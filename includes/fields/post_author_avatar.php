<?php
// add default fields to the hook filter
add_filter( 'qw_fields', 'qw_field_author_avatar' );

/*
 * Add field to qw_fields
 */
function qw_field_author_avatar( $fields ) {

	$fields['post_author_avatar'] = array(
		'title'            => 'Post Author Avatar',
		'description'      => 'Avatar for the author of a post.',
		'form_callback'    => 'qw_field_author_avatar_form',
		'output_callback'  => 'qw_get_avatar',
		'output_arguments' => TRUE,
	);

	return $fields;
}

/*
 * Avatar output callback
 *
 * get_avatar( $id_or_email, $size, $default, $alt );
 */
function qw_get_avatar( $post, $field ) {
	if ( isset( $field['link_to_author'] ) ) {
		$output = '<a href="' . get_author_posts_url( $post->post_author ) . '">' . get_avatar( $post->post_author,
				$field['size'] ) . '</a>';
	} else {
		$output = get_avatar( $post->post_author, $field['size'] );
	}

	return $output;
}

/*
 * Avatar form callback
 */
function qw_field_author_avatar_form( $field ) {
	$link_selected = ( isset( $field['values']['link_to_author'] ) ) ? 'checked="checked"' : '';
	$size          = isset( $field['values']['size'] ) ? $field['values']['size'] : '';
	?>
	<label class="qw-label">Avatar Size: </label>
	<input class='qw-js-title'
	       type="text"
	       name="<?php print $field['form_prefix']; ?>[size]"
	       value="<?php print $size; ?>"/>
	(pixel width)
	<p>
		<label class='qw-field-checkbox'>
			<input type='checkbox'
			       name='<?php print $field['form_prefix']; ?>[link_to_author]'
				<?php print $link_selected; ?> />
			Link to author page (list of author posts)
		</label>
	</p>
<?php
}