<?php
// add default fields to the hook filter
add_filter( 'qw_fields', 'qw_field_taxonomy_terms' );

/**
 * Add field to qw_fields
 *
 * @param $fields
 *
 * @return array
 */
function qw_field_taxonomy_terms( $fields ) {

	$fields['taxonomy_terms'] = array(
		'title'            => 'Taxonomy Terms',
		'description'      => 'Information relating to the author of a post.',
		'form_callback'    => 'qw_field_taxonomy_terms_form',
		'output_callback'  => 'qw_field_taxonomy_terms_output',
		'output_arguments' => TRUE,
	);

	return $fields;
}

/**
 * Output callback
 *
 * @param $post
 * @param $field
 *
 * @return null|string
 */
function qw_field_taxonomy_terms_output( $post, $field ) {
	$output = array();

	$terms = get_the_terms( $post->ID, $field['taxonomy_name'] );

	foreach( $terms as $term ){
		if ( isset( $field['link_to_term'] ) ) {
			$output[] = '<a href="' . get_term_link( $term->term_id ) . '">' . $term->name . '</a>';
		}
		else {
			$output[] = $term->name;
		}
	}

	return "<span class='qw-taxonomy-term'>".
	       implode( "</span><span class='qw-taxonomy-term'>", $output ).
	       "</span>";
}

/**
 * Form callback
 *
 * @param $field
 */
function qw_field_taxonomy_terms_form( $field ) {
	$options = array();
	$taxes = get_taxonomies( array(
		'public' => true,
	), 'objects' );

	foreach( $taxes as $key => $tax ){
		$options[ $key ] = $tax->label;
	}

	$link_selected = ( isset( $field['values']['link_to_term'] ) ) ? 'checked="checked"' : '';
	?>
	<label class="qw-label">Taxonomy:</label>

	<select class='qw-js-title'
	        name="<?php print $field['form_prefix']; ?>[taxonomy_name]">
		<?php
		foreach ( $options as $value => $title ) { ?>
			<option value="<?php print $value; ?>"
				<?php print ( $field['values']['taxonomy_name'] == $value ) ? 'selected="selected"' : ''; ?>>
				<?php print $title; ?>
			</option>
		<?php
		}
		?>
	</select>

	<p>
		<label class='qw-field-checkbox'>
			<input type='checkbox'
			       name='<?php print $field['form_prefix']; ?>[link_to_term]'
				<?php print $link_selected; ?> />
			Link to the term page
		</label>
	</p>
<?php
}