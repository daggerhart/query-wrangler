<?php

// add default fields to the hook filter
add_filter( 'qw_fields', 'qw_field_meta_value_new' );

/*
 * Add field to qw_fields
 */
function qw_field_meta_value_new( $fields ) {
	$fields['custom_field'] = array(
		'title'            => 'Custom Field',
		'description'      => 'Select a custom field (meta value)',
		'output_callback'  => 'qw_display_post_meta_value_new',
		'output_arguments' => TRUE,
		'form_callback'    => 'qw_meta_value_new_form_callback',
		'content_options'  => TRUE,
		// need to dynamicall fix this on the the form and during output
		'meta_key'         => '',
	);

	return $fields;
}

/*
 * Post Meta form settings
 */
function qw_meta_value_new_form_callback( $field ) {
	$meta_key = isset( $field['values']['meta_key'] ) ? $field['values']['meta_key'] : '';

	// pick up after the old meta_value field
	// try to find the old meta_key
	if ( empty( $meta_key ) && strpos( $field['type'], 'meta_' ) === 0 ) {
		$meta_key = substr( $field['type'], 5 );
	}

	$separator        = ( isset( $field['values']['meta_value_separator'] ) ) ? $field['values']['meta_value_separator'] : '';
	$count            = ( isset( $field['values']['meta_value_count'] ) ) ? (int) $field['values']['meta_value_count'] : 1;
	$display_handlers = apply_filters( 'qw_meta_value_display_handlers',
		array() );
	$are_image_ids    = isset( $field['values']['are_image_ids'] ) ? 'checked="checked"' : '';
	$image_styles     = get_intermediate_image_sizes();
	?>
	<div>
		<label for="<?php print $field['form_prefix']; ?>[meta_key]">Meta
			Key:</label>
		<input type="text"
		       class="qw-meta-value-key-autocomplete"
		       name="<?php print $field['form_prefix']; ?>[meta_key]"
		       value="<?php print $meta_key; ?>"/>

		<p class="description">The custom field name / meta key that identifies
			the field.</p>
	</div>
	<div>
		<label>Count:</label> <input type="text"
		                             name="<?php print $field['form_prefix']; ?>[meta_value_count]"
		                             value="<?php print $count; ?>"/>

		<p class="description">Number of the meta values to show. User '0' for
			all values.</p>
	</div>
	<div>
		<label>Separator:</label> <input type="text"
		                                 name="<?php print $field['form_prefix']; ?>[meta_value_separator]"
		                                 value="<?php print $separator;?>"/>

		<p class="description">How to separate the meta values (if more than
			1).</p>
	</div>
	<div>
		<label class="qw-label">Display Handler:</label>
		<select class='qw-js-title'
		        name='<?php print $field['form_prefix']; ?>[display_handler]'>
			<?php
			foreach ( $display_handlers as $handler => $details ) {
				$handler_selected = ( $field['values']['display_handler'] == $handler ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $handler; ?>" <?php print $handler_selected; ?>><?php print $details['title']; ?></option>
			<?php
			}
			?>
		</select>

		<p class="description">Select the method fo displaying the meta value.
			To display the raw value, choose '-none-'.</p>
	</div>
	<div>
		<label><input type="checkbox"
		              name="<?php print $field['form_prefix']; ?>[are_image_ids]" <?php print $are_image_ids; ?> />Load
			Image IDs as Images</label>

		<p class="description">If the meta value returned from the display
			handler as an image ID, display the image HTML.</p>

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

		<p class="description">If the meta value is an Image ID, select the
			style for display.</p>
	</div>
	<?php

	if ( function_exists( 'get_custom_field' ) ) {
		$chaining = ( isset( $field['values']['cctm_chaining'] ) ) ? $field['values']['cctm_chaining'] : '';
		?>
		<div>
			<label>CCTM Output Filters:</label> <input type="text"
			                                           name="<?php print $field['form_prefix']; ?>[cctm_chaining]"
			                                           value="<?php print $chaining; ?>"
			                                           ;/>

			<p class="description">Include first colon. ex, ":filter1:filter2".
				Or to get image IDs from an image field, ":raw". <a
					target="_blank"
					href="http://code.google.com/p/wordpress-custom-content-type-manager/wiki/OutputFilters#Chaining">CCTM
					Filter Chaining</a></p>
		</div>
	<?php
	}
}

/*
 * Display the post meta field based on field settings
 */
function qw_display_post_meta_value_new( $post, $field ) {
	// pick up after the old meta_value field
	// try to find the old meta_key
	if ( empty( $field['meta_key'] ) && strpos( $field['type'],
			'meta_' ) === 0
	) {
		$field['meta_key'] = substr( $field['type'], 5 );
	}

	$display_handlers    = apply_filters( 'qw_meta_value_display_handlers',
		array() );
	$display_handler_key = isset( $field['display_handler'] ) ? $field['display_handler'] : 'none';
	$handler             = isset( $display_handlers[ $display_handler_key ] ) ? $display_handlers[ $display_handler_key ] : $display_handlers['none'];

	$count       = isset( $field['meta_value_count'] ) ? $field['meta_value_count'] : 1;
	$separator   = isset( $field['meta_value_separator'] ) ? $field['meta_value_separator'] : '';
	$meta_values = array();
	$values      = array();

	if ( function_exists( $handler['callback'] ) ) {
		$meta_values = $handler['callback']( $post, $field );

		// ensure we're working with an array
		if ( ! is_array( $meta_values ) ) {
			$meta_values = array( $meta_values );
		}
	}

	// handle count limit
	if ( $count <= 0 || count( $meta_values ) <= $count ) {
		$values = $meta_values;
	} else {
		$i = 0;
		foreach ( $meta_values as $k => $v ) {
			if ( $i < $count ) {
				$values[] = $v;
			}
			$i ++;
		}
	}

	// image ids
	if ( isset( $field['are_image_ids'] ) ) {
		$image_ids = $values;
		$values    = array();
		foreach ( $image_ids as $image_id ) {
			$values[] = wp_get_attachment_image( $image_id,
				$field['image_display_style'] );
		}
	}

	return implode( $separator, $values );
}
