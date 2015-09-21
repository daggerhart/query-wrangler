<?php
// add default fields to the hook filter
add_filter( 'qw_fields', 'qw_field_image_attachment' );

/*
 * Add field to qw_fields
 */
function qw_field_image_attachment( $fields ) {

	$fields['image_attachment'] = array(
		'title'            => 'Image Attachment',
		'description'      => 'Image files that are attached to a post.',
		'output_callback'  => 'qw_theme_image',
		'output_arguments' => TRUE,
		'form_callback'    => 'qw_field_image_attachment_form',
	);

	return $fields;
}

/*
 * Image attachment settings Form
 */
function qw_field_image_attachment_form( $field ) {
	//$image_styles = _qw_get_image_styles();
	$image_styles   = get_intermediate_image_sizes();
	$featured_image = ( isset( $field['values']['featured_image'] ) ) ? 'checked="checked"' : "";
	?>
	<!-- image display -->
	<label class="qw-label">Number of items to show:</label>
	<input class="qw-text-short"
	       type="text"
	       name='<?php print $field['form_prefix']; ?>[image_display_count]'
	       value="<?php print ( isset( $field['values']['image_display_count'] ) ) ? $field['values']['image_display_count'] : 0;?>"/>
	<p>
		<label>
			<input
				type="checkbox"
				name="<?php print $field['form_prefix']; ?>[featured_image]"
				<?php print $featured_image; ?> /> Featured Image Only
		</label>
	</p>
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
 * TODO - allow images to use file styles
 */
function _qw_get_image_styles() {
	$image_styles = qw_all_file_styles();
	$image_sizes  = get_intermediate_image_sizes();

	foreach ( $image_sizes as $key => $size ) {
		$image_styles[ $size ] = array(
			'description' => $size,
			'callback'    => 'qw_theme_image',
		);
	}

	return $image_styles;
}

/*
 * Turn a list of images into html
 *
 * @param $post_id
 * @param $image_type
 * @param $count;
 */
function qw_theme_image( $post, $field ) {
	$style             = $field['image_display_style'];
	$count             = $field['image_display_count'];
	$featured_image_id = isset( $field['featured_image'] ) ? get_post_thumbnail_id( $post->ID ) : NULL;
	$images            = qw_get_post_images( $post->ID );

	if ( is_array( $images ) ) {
		$output = '';
		$i      = 0;
		foreach ( $images as $image ) {
			if ( $featured_image_id ) {
				if ( $image->ID == $featured_image_id ) {
					$output .= wp_get_attachment_image( $image->ID, $style );
				}
			} else {
				// ensure less than count
				if ( $count == 0 || ( $i < $count ) ) {
					$output .= wp_get_attachment_image( $image->ID, $style );
				}
			}
			$i ++;
		}

		return $output;
	}
}

/*
 * Get all images attached to a single post
 *
 * @param int $post_id The Wordpress ID for the post or page to get images from
 *
 * @return sorted array of images
 */
function qw_get_post_images( $post_id ) {
	$child_args = array(
		"post_type"      => "attachment",
		"post_mime_type" => "image",
		"post_parent"    => $post_id
	);
	// Get images for this post
	$images = &get_children( $child_args );

	// If images exist for this page
	if ( is_array( $images ) ) {
		// sort this so menu order matters
		$sorted   = array();
		$unsorted = array();
		foreach ( $images as $image ) {
			if ( $image->menu_order !== 0 ) {
				$sorted[ $image->menu_order ] = $image;
			} else {
				$unsorted[] = $image;
			}
		}
		// sort menu order
		ksort( $sorted );
		// reset array
		$sorted = array_values( $sorted );
		// add unsorted
		$sorted = array_merge( $sorted, $unsorted );

		return $sorted;
	}
}
