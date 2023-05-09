<?php

// add default overrides to the filter
add_filter( 'qw_overrides', 'qw_override_post_type_archive' );
add_filter( 'qw_pre_save', 'qw_override_post_type_archive_pre_save', 10, 2 );
add_action( 'qw_delete_query', 'qw_override_post_type_delete_query' );

function qw_override_post_type_archive( $overrides ) {

	$overrides['post_type_archive'] = array(
		'title'              => 'Post Types',
		'description'        => 'Override archive output based on post type',
		'form_callback'      => 'qw_override_post_type_archive_form',
		'get_query_callback' => 'qw_override_post_type_archive_get_query',
	);

	return $overrides;
}

/**
 * Hook into qw_pre_save and add save additional data
 * Save term relationships to the query_override_terms table
 *
 * @param $options
 * @param $query_id
 *
 * @return mixed
 */
function qw_override_post_type_archive_pre_save( $options, $query_id ) {
	if ( ! isset( $options['override'] ) ) {
		return $options;
	}

	if ( ! is_array( $options['override'] ) ){
		return $options;
	}

	$post_type_archives = get_option( '_qw_override_post_type_archives', array() );

	foreach ( $options['override'] as $name => $override ) {
		if ( $override['type'] == 'post_type_archive' && is_array( $override['values'] ) ) {
			foreach ( $override['values'] as $type ){
				$post_type_archives[ $type ] = $query_id;
			}
		}
	}

	update_option( '_qw_override_post_type_archives', $post_type_archives );

	return $options;
}

/**
 * Clean up after a deleted query
 *
 * @param $query_id
 */
function qw_override_post_type_delete_query( $query_id ){
	$post_type_archives = get_option( '_qw_override_post_type_archives', array() );

	foreach ( $post_type_archives as $type => $_query_id ){
		if ( $query_id == $_query_id ){
			unset( $post_type_archives[ $type ] );
		}
	}

	update_option( '_qw_override_post_type_archives', $post_type_archives );
}

/**
 * Settings for this override
 *
 * @param $override
 */
function qw_override_post_type_archive_form( $override ) {
	$post_types = qw_all_post_types();
	?>
	<p>Select which post types to override.</p>
	<div class="qw-checkboxes">
		<?php
		// List all categories as checkboxes
		foreach ( $post_types as $post_type ) {
			?>
			<label class="qw-query-checkbox">
				<input class="qw-js-title"
				       type="checkbox"
				       name="<?php print $override['form_prefix']; ?>[values][<?php print $post_type; ?>]"
				       value="<?php print $post_type; ?>"
					<?php checked( isset( $override['values']['values'][ $post_type ] ) ); ?> />
				<?php print $post_type; ?>
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
 * @param \WP_Query $wp_query
 *
 * @return bool|QW_Query
 */
function qw_override_post_type_archive_get_query( $wp_query ) {
	if ( $wp_query->is_post_type_archive() ) {
		$post_types = get_query_var( 'post_type' );
		// Always deal with it as an array.
		$post_types = is_array( $post_types ) ? $post_types : [ $post_types ];
		$post_type_archives = get_option( '_qw_override_post_type_archives', array() );

		// Multiple post types can be overridden by a single query.
		$valid_post_types = [];
		$found_query = null;
		foreach ( $post_types as $post_type) {
			// Ensure the given post type is a QW override.
			if ( !isset( $post_type_archives[ $post_type ] ) ) {
				continue;
			}

			// If we haven't determined the query yet, set the first one.
			if ( !$found_query ) {
				$found_query = qw_get_query( (int) $post_type_archives[ $post_type ] );
			}

			// If we have a query, ensure that
			if ( $found_query && $found_query->id == $post_type_archives[ $post_type ]  ) {
				$valid_post_types[] = $post_type;
			}
		}

		if ( $found_query ) {
			// add the appropriate filter to the query
			$found_query->add_handler_item( 'filter',
				'post_type',
				$valid_post_types
			);

			return $found_query;
		}
	}

	return FALSE;
}
