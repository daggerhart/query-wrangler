<?php

/*
 * Process and theme exposed handlers
 */
function qw_generate_exposed_handlers( $options ) {
	if ( $exposed = qw_process_exposed_handlers( $options ) ) {
		$output = array( 'sorts' => '', 'filters' => '' );
		// loop through sorts and filters
		if ( isset( $exposed['sorts'] ) && is_array( $exposed['sorts'] ) ) {
			// loop through each exposed item
			foreach ( $exposed['sorts'] as $name => $item ) {
				// show the exposed form
				if ( function_exists( $item['exposed_form'] ) ) {
					$item['name'] = $name;
					$output['sorts'] .= qw_theme_single_exposed_handler( $item );
				}
			}
		}

		// loop through sorts and filters
		if ( isset( $exposed['filters'] ) && is_array( $exposed['filters'] ) ) {
			// loop through each exposed item
			foreach ( $exposed['filters'] as $name => $item ) {
				// show the exposed form
				if ( function_exists( $item['exposed_form'] ) ) {
					$item['name'] = $name;
					$output['filters'] .= qw_theme_single_exposed_handler( $item );
				}
			}
		}

		return $output;
	}
}

/*
 * Look for and prepare exposed handlers
 */
function qw_process_exposed_handlers( $options ) {
	// look for exposed filters or sorts
	if ( isset( $options['args']['sorts'] ) && is_array( $options['args']['sorts'] ) ) {
		$all_sorts     = qw_all_sort_options();
		$exposed_sorts = array();
		foreach ( $options['args']['sorts'] as $name => $sort ) {
			if ( isset( $sort['is_exposed'] ) ) {
				$exposed_sorts[ $name ] = $all_sorts[ $sort['hook_key'] ];
				// override exposed_key
				if ( ! empty( $sort['exposed_key'] ) ) {
					$exposed_sorts[ $name ]['exposed_key'] = $sort['exposed_key'];
				}
				$exposed_sorts[ $name ]['values'] = $sort;
			}
		}
	}
	if ( isset( $options['args']['filters'] ) && is_array( $options['args']['filters'] ) ) {
		$exposed_filters = array();
		$all_filters     = qw_all_filters();
		foreach ( $options['args']['filters'] as $name => $filter ) {
			if ( isset( $filter['is_exposed'] ) ) {
				$exposed_filters[ $name ] = $all_filters[ $filter['hook_key'] ];
				// override exposed_key
				if ( ! empty( $filter['exposed_key'] ) ) {
					$exposed_filters[ $name ]['exposed_key'] = $filter['exposed_key'];
				}
				$exposed_filters[ $name ]['values'] = $filter;
			}
		}
	}

	$exposed = array();
	if ( isset( $exposed_filters ) && count( $exposed_filters ) > 0 ) {
		$exposed_filters    = apply_filters( 'qw_process_exposed_filters',
			$exposed_filters );
		$exposed['filters'] = $exposed_filters;
	}
	/*
	  if (isset($exposed_sorts) && count($exposed_sorts) > 0){
		do_action_ref_array('qw_process_exposed_sorts', array(&$exposed_sorts));
		$exposed['sorts'] = $exposed_sorts;
	  }
	  */
	if ( count( $exposed ) > 0 ) {
		return $exposed;
	}

	return FALSE;
}

/*
 * Make getting the subitted exposed data easy
 */
function qw_exposed_submitted_data() {
	$data = array();
	if ( ! empty( $_GET ) ) {
		$data = $_GET;
	} else if ( ! empty( $_POST ) ) {
		$data = $_POST;
	}
	foreach ( $data as $k => $v ) {
		if ( is_null( $data[ $k ] ) ) {
			unset( $data[ $k ] );
			unset( $_GET[ $k ] );
			unset( $_POST[ $k ] );
		} else {
			if ( is_array( $v ) ) {
				array_walk_recursive( $v, 'sanitize_text_field' );
				$data[ $k ] = $v;
			} else {
				$data[ $k ] = sanitize_text_field( urldecode( $v ) );
			}
		}
	}

	if ( count( $data ) > 0 ) {
		return $data;
	}

	return FALSE;
}

/*
 * Single exposed handler wrapper html
 */
function qw_theme_single_exposed_handler( $item ) {
	if ( empty( $item['exposed_key'] ) ) {
		$item['exposed_key'] = 'exposed_' . $item['name'];
	}
	// gather submitted values
	$submitted = qw_exposed_submitted_data();
	$values    = '';
	if ( isset( $submitted[ $item['exposed_key'] ] ) ) {
		$values = $submitted[ $item['exposed_key'] ];
	}
	ob_start();
	?>
	<div class="query-exposed-<?php print $item['name']; ?>">
		<?php if ( ! empty( $item['values']['exposed_label'] ) ) { ?>
			<label
				class="query-exposed-label query-exposed-label-<?php print $item['name']; ?>">
				<?php print $item['values']['exposed_label']; ?>
			</label>
		<?php } ?>
		<!-- exposed-<?php print $item['name']; ?> -->
		<?php $item['exposed_form']( $item, $values ); ?>

		<?php if ( ! empty( $item['values']['exposed_desc'] ) ) { ?>
			<div
				class="query-exposed-description query-exposed-description-<?php print $item['name']; ?>">
				<?php print $item['values']['exposed_desc']; ?>
			</div>
		<?php } ?>
	</div>
	<?php
	return ob_get_clean();
}

/*
 * Generic exposed setting of 'type'
 */
function qw_exposed_setting_type( $filter ) {
	$opts = array(
		'select'     => 'Select',
		'checkboxes' => 'Checkboxes',
	);
	?>
	<div>
		<label class="qw-label">Exposed Values:</label>
		<select
			name="<?php print $filter['form_prefix']; ?>[exposed_settings][type]">
			<?php
			foreach ( $opts as $value => $title ) {
				$selected = ( isset( $filter['values']['exposed_settings']['type'] ) && $value == $filter['values']['exposed_settings']['type'] ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $value; ?>" <?php print $selected; ?> ><?php print $title; ?></option>
			<?php
			}
			?>
		</select>

		<p class="description">Provide an exposed filter that accepts the
			selected number of values.</p>
	</div>
<?php
}