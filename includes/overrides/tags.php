<?php

// add default overrides to the filter
add_filter( 'qw_overrides', 'qw_override_tags' );
add_filter( 'qw_pre_save', 'qw_override_tags_pre_save', 10, 2 );

function qw_override_tags( $overrides ) {

	$overrides['tags'] = array(
		'title'              => 'Tags',
		'description'        => 'Override output based on tags',
		'form_callback'      => 'qw_override_tags_form',
		'get_query_callback' => 'qw_override_tags_get_query',
	);

	return $overrides;
}

/**
 * Hook into qw_pre_save and add save additional data*
 * Save term relationships to the query_override_terms table
 *
 * @param $options
 * @param $query_id
 *
 * @return mixed
 */
function qw_override_tags_pre_save( $options, $query_id ) {

	// no matter what, we delete all previous relationships
	global $wpdb;
	$table = $wpdb->prefix . "query_override_terms";

	// get a list of term_ids from this taxonomy for pre-save deletion
	$terms = get_terms( 'post_tag', array( 'hide_empty' => FALSE ) );

	// delete all existing relationships
	foreach ( $terms as $term ) {
		$wpdb->delete( $table,
			array(
				'query_id' => $query_id,
				'term_id'  => $term->term_id,
			) );
	}

	// merge tags
	if ( isset( $options['override']['tags']['values'] ) &&
	     is_array( $options['override']['tags']['values'] )
	) {
		// new relationships to save
		$insert_terms = array();
		foreach ( $options['override']['tags']['values'] as $term_id => $name ) {
			if ( term_exists( $term_id, 'post_tag' ) ) {
				$insert_terms[] = $term_id;
			}
		}

		// loop through all terms and insert them
		foreach ( $insert_terms as $term_id ) {
			$wpdb->insert( $table,
				array(
					'query_id' => $query_id,
					'term_id'  => $term_id,
				) );
		}
	}

	return $options;
}

/**
 * Settings for this override
 *
 * @param $override
 */
function qw_override_tags_form( $override ) {
	$tags = get_terms( 'post_tag', array( 'hide_empty' => FALSE ) );
	?>
	<p>Select which tags to override.</p>
	<div class="qw-checkboxes">
		<?php
		foreach ( $tags as $tag ) { ?>
			<label class="qw-query-checkbox">
				<input class="qw-js-title"
				       type="checkbox"
				       name="<?php print $override['form_prefix']; ?>[values][<?php print $tag->term_id; ?>]"
				       value="<?php print $tag->name; ?>"
					<?php checked( isset( $override['values']['values'][ $tag->term_id ] ) ); ?> />
				<?php print $tag->name; ?>
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
function qw_override_tags_get_query( $wp_query ) {
	if ( $wp_query->is_tag() && $wp_query->is_archive() ) {
		$term     = $wp_query->get_queried_object();
		$query_id = qw_get_query_by_override_term( $term->term_id );

		if ( $query_id && $qw_query = qw_get_query( $query_id ) ) {

			// add the appropriate filter to the query
			$qw_query->add_handler_item( 'filter',
				'categories',
				array(
					'tag_operator' => 'tag__in',
					'tags'         => array( $term->term_id => $term->name ),
				) )
				// override the post title
				     ->override_options( array(
					'display' => array(
						'title' => single_term_title( '', FALSE ),
					)
				) );

			return $qw_query;
		}
	}

	return FALSE;
}
