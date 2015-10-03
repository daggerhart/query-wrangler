<?php

// add default overrides to the filter
add_filter( 'qw_overrides', 'qw_override_taxonomies' );
add_filter( 'qw_pre_save', 'qw_override_taxonomies_pre_save', 10, 2 );
add_action( 'qw_delete_query', 'qw_override_taxonomies_delete_query' );

function qw_override_taxonomies( $overrides ) {

	$overrides['taxonomies'] = array(
		'title'              => 'Taxonomies',
		'description'        => 'Override term archive pages for an entire taxonomy.',
		'form_callback'      => 'qw_override_taxonomies_form',
		'get_query_callback' => 'qw_override_taxonomies_get_query'
	);

	return $overrides;
}

/**
 * Hook into qw_pre_save and add save additional data
 *
 * @param $options
 * @param $query_id
 *
 * @return mixed
 */
function qw_override_taxonomies_pre_save( $options, $query_id ) {

	if ( isset( $options['override']['taxonomies'] ) &&
	     is_array( $options['override']['taxonomies'] )
	) {
		$override                = $options['override']['taxonomies'];
		$taxonomies              = get_taxonomies( array( 'public' => TRUE, ),
			'objects' );
		$_qw_override_taxonomies = get_option( '_qw_override_taxonomies',
			array() );

		/*
		 * expecting
		 * array(
		 *   '{$taxonomy->name}__{$query_id}' => array(
		 *     'query_id' =>  $query_id,
		 *     'taxonomy' => $taxonomy->name,
		 *   )
		 * )
		 */
		// loop through all taxonomies so we can know what was not submitted
		foreach ( $taxonomies as $taxonomy ) {
			$key = "{$taxonomy->name}__{$query_id}";

			// see if this taxonomy checkbox was submitted
			if ( isset( $override['values'][ $taxonomy->name ] ) ) {
				$_qw_override_taxonomies[ $key ] = array(
					'query_id' => $query_id,
					'taxonomy' => $taxonomy->name,
				);
			} // otherwise, remove any existing instances of this query
			else if ( isset( $_qw_override_taxonomies[ $key ] ) ) {
				unset( $_qw_override_taxonomies[ $key ] );
			}
		}

		// need to save overrides somewhere quickly accessible
		// eg, _qw_override_taxonomies__category = query_id
		update_option( '_qw_override_taxonomies', $_qw_override_taxonomies );

		// clean up soem redundant data from the form
		$options['override']['taxonomies'] = $options['override']['taxonomies']['values'];
	}

	return $options;
}

/**
 * QW hook 'qw_delete_query'
 *
 * @param $query_id
 */
function qw_override_taxonomies_delete_query( $query_id ){
	$_qw_override_taxonomies = get_option( '_qw_override_taxonomies', array() );
	foreach ( $_qw_override_taxonomies as $key => $values ) {
		if ( $values['query_id'] == $query_id ) {
			unset( $_qw_override_taxonomies[ $key ] );
		}
	}

	update_option( '_qw_override_taxonomies', $_qw_override_taxonomies );
}

/**
 * Settings for this override
 *
 * @param $override
 */
function qw_override_taxonomies_form( $override ) {
	$taxonomies = get_taxonomies( array( 'public' => TRUE, ), 'objects' );
	?>
	<p>Select which tags to override.</p>
	<div class="qw-checkboxes">
		<?php
		foreach ( $taxonomies as $taxonomy ) { ?>
			<label class="qw-query-checkbox">
				<input class="qw-js-title"
				       type="checkbox"
				       name="<?php print $override['form_prefix']; ?>[values][<?php print $taxonomy->name; ?>]"
				       value="<?php print $taxonomy->name; ?>"
					<?php checked( isset( $override['values'][ $taxonomy->name ] ) ); ?> />
				<?php print $taxonomy->labels->name; ?>
			</label>
		<?php
		}
		?>
	</div>
<?php
}

/**
 * Determine if this override should be executed
 * return a QW_Query object if so, otherwise return false;
 *
 * @return bool|QW_Query
 */
function qw_override_taxonomies_get_query( $wp_query ) {
	if ( $wp_query->is_archive() && ( $wp_query->is_tag() || $wp_query->is_category() || $wp_query->is_tag() ) ) {
		$term     = $wp_query->get_queried_object();
		$query_id = FALSE;

		// look for a taxonomies override on this term->taxonomy
		$_qw_override_taxonomies = get_option( '_qw_override_taxonomies',
			array() );

		foreach ( $_qw_override_taxonomies as $key => $values ) {
			if ( $values['taxonomy'] == $term->taxonomy ) {
				$query_id = $values['query_id'];
				break;
			}
		}

		if ( $query_id && $qw_query = qw_get_query( $query_id ) ) {

			// add the appropriate filter to the query
			$qw_query->add_handler_item( 'filter',
				'taxonomy_' . $term->taxonomy,
				array(
					'terms'            => array( $term->term_id => $term->name ),
					'operator'         => 'IN',
					'include_children' => TRUE,
				) )
				// override the post title
				     ->override_options( array(
					'display' => array(
						'title' => single_term_title( '', FALSE ),
					),
				) );

			return $qw_query;
		}
	}

	return FALSE;
}