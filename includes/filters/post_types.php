<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_post_types' );

function qw_filter_post_types( $filters ) {
	$filters['post_types'] = array(
		'title'                 => 'Post Types',
		'description'           => 'Select which post types should be shown.',
		'form_callback'         => 'qw_filter_post_types_form',
		'query_args_callback'   => 'qw_filter_post_types_args',
		'query_display_types'   => array( 'page', 'widget', 'override' ),
		// exposed
		'exposed_form'          => 'qw_filter_post_types_exposed_form',
		'exposed_process'       => 'qw_filter_post_types_exposed_process',
		'exposed_settings_form' => 'qw_filter_post_types_exposed_settings_form',
	);

	return $filters;
}

/*
 * Admin form for filter
 */
function qw_filter_post_types_form( $filter ) {
	if ( ! isset( $filter['values']['post_types'] ) ) {
		$filter['values']['post_types'] = array();
	}

	?>
	<div class="qw-checkboxes">
		<?php
		$post_types = qw_all_post_types();
		// loop through post types
		foreach ( $post_types as $post_type ) {
			$post_type_checked = ( isset( $filter['values']['post_types'][ $post_type ] ) ) ? 'checked="checked"' : '';
			?>
			<label class="qw-query-checkbox">
				<input class="qw-js-title"
				       type="checkbox"
				       name="<?php print $filter['form_prefix']; ?>[post_types][<?php print $post_type; ?>]"
				       value="<?php print $post_type; ?>"
					<?php print $post_type_checked; ?> />
				<?php print ucfirst( $post_type ); ?>
			</label>
		<?php
		}
		?>
	</div>
<?php
}

/*
 * Add query args as necessary
 */
function qw_filter_post_types_args( &$args, $filter ) {
	if ( isset( $filter['values']['post_types'] ) ) {
		$args['post_type'] = $filter['values']['post_types'];
	}
}

/*
 * Exposed settings form
 */
function qw_filter_post_types_exposed_settings_form( $filter ) {
	// use the default provided single/multiple exposed values
	// saves values to [exposed_settings][type]
	print qw_exposed_setting_type( $filter );
}

/*
 * Process submitted exposed form values
 */
function qw_filter_post_types_exposed_process( &$args, $filter, &$values ) {
	$alter_args = FALSE;
	qw_filter_post_types_exposed_default_values( $filter, $values );

	switch ( $filter['values']['exposed_settings']['type'] ) {
		case 'select':
			$alter_args                     = TRUE;
			$filter['values']['post_types'] = $values;
			break;

		case 'checkboxes':
			if ( is_array( $values ) ) {
				$alter_args = TRUE;
				// gather the post types into the array expected by qw_filter_post_types_args()
				$post_types = array();
				foreach ( $values as $v ) {
					$post_types[ $v ] = $v;
				}
				$filter['values']['post_types'] = $post_types;
			}
			break;
	}

	if ( $alter_args ) {
		qw_filter_post_types_args( $args, $filter );
	}
}

/*
 * Exposed forms
 */
function qw_filter_post_types_exposed_form( $filter, &$values ) {
	// adjust for default values
	qw_filter_post_types_exposed_default_values( $filter, $values );

	switch ( $filter['values']['exposed_settings']['type'] ) {
		case 'select':
			qw_filter_post_types_exposed_form_select( $filter, $values );
			break;

		case 'checkboxes':
			qw_filter_post_types_exposed_form_checkboxes( $filter, $values );
			break;
	}
}

/*
 * Simple helper funtion to handle default values
 */
function qw_filter_post_types_exposed_default_values( $filter, &$values ) {
	if ( isset( $filter['values']['exposed_default_values'] ) ) {
		if ( is_null( $values ) ) {
			$values = $filter['values']['post_types'];
		}
	}
}

/*
 * Exposed post types as select box
 */
function qw_filter_post_types_exposed_form_select( $filter, &$values ) {
	$post_types = qw_all_post_types();
	// adjust for allowed values
	qw_filter_post_types_exposed_limit_values( $filter, $post_types );
	qw_filter_post_types_adjust_for_submitted_values( $filter, $values );

	?>
	<div class="query-select">
		<select name="<?php print $filter['exposed_key']; ?>">
			<?php
			foreach ( $post_types as $type ) {
				$type_selected = ( in_array( $type,
					$filter['values']['post_types'] ) ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $type; ?>" <?php print $type_selected; ?>>
					<?php print ucfirst( $type ); ?>
				</option>
			<?php
			}
			?>
		</select>
	</div>
<?php
}

/*
 * Exposed post types as checkboxes
 */
function qw_filter_post_types_exposed_form_checkboxes( $filter, &$values ) {
	$post_types = qw_all_post_types();
	// adjust for allowed values
	qw_filter_post_types_exposed_limit_values( $filter, $post_types );
	qw_filter_post_types_adjust_for_submitted_values( $filter, $values );

	?>
	<div class="query-checkboxes">
		<?php
		// List all categories as checkboxes
		foreach ( $post_types as $type ) {
			if ( is_array( $filter['values']['post_types'] ) ) {
				// see if our submitted value is
				if ( in_array( $type, $filter['values']['post_types'] ) ) {
					$type_checked = 'checked="checked"';
				} else {
					$type_checked = '';
				}
			}
			?>
			<label class="query-checkbox">
				<input type="checkbox"
				       name="<?php print $filter['exposed_key']; ?>[]"
				       value="<?php print $type; ?>"
					<?php print $type_checked; ?> />
				<?php print ucfirst( $type ); ?>
			</label>
		<?php
		}
		?>
	</div>
<?php
}

/*
 * Simple helper function to determine values with consideration for defaults
 */
function qw_filter_post_types_exposed_limit_values( $filter, &$post_types ) {
	if ( isset( $filter['values']['exposed_limit_values'] ) && is_array( $filter['values']['post_types'] ) ) {
		foreach ( $post_types as $k => $type ) {
			if ( ! in_array( $type, $filter['values']['post_types'] ) ) {
				unset( $post_types[ $k ] );
			}
		}
	}
}

/*
 * Adjusted the selected values of the exposed filter based on submitted values
 */
function qw_filter_post_types_adjust_for_submitted_values( &$filter, $values ) {
	// adjust for submitted values
	if ( ! empty( $values ) ) {
		if ( is_array( $values ) ) {
			$filter['values']['post_types'] = array();
			foreach ( $values as $value ) {
				$filter['values']['post_types'][ $value ] = $value;
			}
		} else {
			$filter['values']['post_types'] = array( $values => $values );
		}
	}
}