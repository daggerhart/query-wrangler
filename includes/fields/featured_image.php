<?php
// add default fields to the hook filter
add_filter( 'qw_fields', 'qw_field_featured_image' );

/*
 * Add field to qw_fields
 */
function qw_field_featured_image( $fields ) {

	$fields['featured_image'] = array(
		'title'            => 'Featured Image',
		'description'      => 'The "post_thumbnail" of a given row.',
		'output_callback'  => 'qw_theme_featured_image',
		'output_arguments' => TRUE,
		'form_callback'    => 'qw_field_featured_image_form',
	);

	return $fields;
}

/*
 * Image attachment settings Form
 */
function qw_field_featured_image_form( $field ) {
	//$image_styles = _qw_get_image_styles();
	$image_styles = get_intermediate_image_sizes();
	?>
	<p>
		<label class="qw-label">Image Display Style:</label>
		<select class='qw-js-title'
		        name='<?php print $field['form_prefix']; ?>[image_display_style]'>
			<?php
			foreach ( $image_styles as $key => $style ) {
				$style_selected = ( $field['values']['image_display_style'] == $style ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $style; ?>" <?php print $style_selected; ?>><?php print $style; ?></option>
			<?php
			}
			?>
		</select>
	</p>
<?php
}

/*
 * Turn a list of images into html
 *
 * @param $post
 * @param $field
 */
function qw_theme_featured_image( $post, $field ) {
	$style = $field['image_display_style'];
	if ( has_post_thumbnail( $post->ID ) ) {
		$image_id = get_post_thumbnail_id( $post->ID, $style );

		return wp_get_attachment_image( $image_id, $style );
	}
}
