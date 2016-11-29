<?php
// add default fields to the hook filter
add_filter( 'qw_fields', 'qw_field_file_attachment' );

// add default file styles to the filter
add_filter( 'qw_file_styles', 'qw_default_file_styles', 0 );

/*
 * Add field to qw_fields
 */
function qw_field_file_attachment( $fields ) {

	$fields['file_attachment'] = array(
		'title'            => 'File Attachment',
		'description'      => 'Files that are attached to a post.',
		'output_callback'  => 'qw_theme_file',
		'output_arguments' => TRUE,
		'form_callback'    => 'qw_field_file_attachment_form',
	);

	return $fields;
}

/*
 * File Styles
 *
 * @return array of file styles
 */
function qw_default_file_styles( $file_styles ) {
	$file_styles['link']     = array(
		'description' => 'Filename Link to File',
		//'callback' => 'qw_theme_file',
	);
	$file_styles['link_url'] = array(
		'description' => 'URL Link to File',
		//'callback' => 'qw_theme_file',
	);
	$file_styles['url']      = array(
		'description' => 'URL of File',
		//'callback' => 'qw_theme_file',
	);

	return $file_styles;
}

/*
 * File attachment settings Form
 */
function qw_field_file_attachment_form( $field ) {
	$file_styles = qw_all_file_styles();
	?>
	<!-- file display -->
	<label class="qw-label">Number of items to show:</label>
	<input class="qw-text-short"
	       type="text"
	       name='<?php print $field['form_prefix']; ?>[file_display_count]'
	       value="<?php print ( isset( $field['values']['file_display_count'] ) ) ? $field['values']['file_display_count'] : 0;?>"/>

	<p>
		<label class="qw-label">File Display Style:</label>
		<select class='qw-js-title'
		        name='<?php print $field['form_prefix']; ?>[file_display_style]'>
			<?php
			foreach ( $file_styles as $key => $file_style_details ) {
				$style_selected = ( $field['values']['file_display_style'] == $key ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $key; ?>" <?php print $style_selected; ?>><?php print $file_style_details['description']; ?></option>
			<?php
			}
			?>
		</select>
	</p>
<?php
}

/*
 * Get and theme attached post files
 *
 * @param int $post_id The post->ID
 * $param int $count Number of files to get
 */
function qw_theme_file( $post, $field ) {
	$style = ( $field['file_display_style'] ) ? $field['file_display_style'] : 'link';
	$count = ( $field['file_display_count'] ) ? $field['file_display_count'] : 0;

	$files = qw_get_post_files( $post->ID );
	if ( is_array( $files ) ) {
		$output = array();
		$i      = 0;
		foreach ( $files as $file ) {
			if ( ( $count == 0 || ( $i < $count ) ) && substr( $file->post_mime_type,
					0,
					5 ) != "image"
			) {
				switch ( $style ) {
					case 'url':
						$output[] = wp_get_attachment_url( $file->ID );
						break;

					case 'link':
						// complete file name
						$file_name = explode( "/", $file->guid );
						$file_name = $file_name[ count( $file_name ) - 1 ];
						$output[] = '<a href="' . wp_get_attachment_url( $file->ID ) . '" class="query-file-link">' . $file_name . '</a>';
						break;

					case 'link_url':
						$output[] = '<a href="' . wp_get_attachment_url( $file->ID ) . '" class="query-file-link">' . $file->guid . '</a>';
						break;
				}
			}
			$i ++;
		}

		return "<span class='qw-file-attachment'>".implode( "</span><span class='qw-file-attachment'>", $output ) ."</span>";
	}
}

/*
 * Get files attached to a post
 *
 * @param int $post_id The WP post id
 * @return Array of file posts
 */
function qw_get_post_files( $post_id ) {
	$child_args = array(
		"post_type"   => "attachment",
		"post_parent" => $post_id,
	);
	// Get images for this post
	$files = get_posts( $child_args );

	if ( is_array( $files ) ) {
		return $files;
	}

	return FALSE;
}

