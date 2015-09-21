<?php
// add default fields to the hook filter
add_filter( 'qw_fields', 'qw_field_author' );

/*
 * Add field to qw_fields
 */
function qw_field_author( $fields ) {

	$fields['post_author'] = array(
		'title'            => 'Post Author',
		'description'      => 'Information relating to the author of a post.',
		'form_callback'    => 'qw_field_author_form',
		'output_callback'  => 'qw_get_the_author',
		'output_arguments' => TRUE,
	);

	return $fields;
}

/*
 * Author output callback
 */
function qw_get_the_author( $post, $field ) {
	switch ( $field['output_type'] ) {
		case 'ID':
			$author = $post->post_author;
			break;

		case 'name':
		default:
			$author = get_the_author();
			break;
	}

	if ( isset( $field['link_to_author'] ) ) {
		$author = '<a href="' . get_author_posts_url( $post->post_author ) . '">' . $author . '</a>';
	}

	return $author;
}

/*
 * Author form callback
 */
function qw_field_author_form( $field ) {
	$options       = array(
		'name' => 'Author Name',
		'ID'   => 'Author ID',
	);
	$link_selected = ( isset( $field['values']['link_to_author'] ) ) ? 'checked="checked"' : '';
	?>
	<label class="qw-label">Author Field Settings:</label>

	<select class='qw-js-title'
	        name="<?php print $field['form_prefix']; ?>[output_type]">
		<?php
		foreach ( $options as $value => $title ) { ?>
			<option value="<?php print $value; ?>"
				<?php print ( $field['values']['output_type'] == $value ) ? 'selected="selected"' : ''; ?>>
				<?php print $title; ?>
			</option>
		<?php
		}
		?>
	</select>

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