<?php
// hook into qw_all_filters()
add_filter( 'qw_filters', 'qw_filter_taxonomies' );

/*
 * Add filter to qw_filters
 */
function qw_filter_taxonomies( $filters ) {
	$ts = get_taxonomies( array(), 'objects' );

	if ( count( $ts ) > 0 ) {
		foreach ( $ts as $t ) {
			$filters[ 'taxonomy_' . $t->name ] = array(
				'title'                 => 'Taxonomy: ' . $t->label,
				'taxonomy'              => $t,
				'description'           => 'Creates a taxonomy filter based on terms selected.',
				'form_callback'         => 'qw_filter_taxonomies_form',
				'query_args_callback'   => 'qw_filter_taxonomies_args',
				'query_display_types'   => array( 'page', 'widget' ),
				// exposed
				'exposed_form'          => 'qw_filter_taxonomies_exposed_form',
				'exposed_process'       => 'qw_filter_taxonomies_exposed_process',
				'exposed_settings_form' => 'qw_filter_taxonomies_exposed_settings_form',
			);

			// update titles for built in taxonomies
			if ( $t->_builtin ) {
				$filters[ 'taxonomy_' . $t->name ]['title'] = 'Taxonomy: ' . $t->label;
			}
		}
	}

	return $filters;
}


/*
 * Convert values into query args
 */
function qw_filter_taxonomies_args( &$args, $filter ) {
	if ( isset( $args['tax_query'] ) && ! is_array( $args['tax_query'] ) ) {
		$args['tax_query'] = array();
	}

	if ( isset( $filter['values']['terms'] ) && is_array( $filter['values']['terms'] ) ) {
		$include_children = ( isset( $filter['values']['include_children'] ) ) ? TRUE : FALSE;

		$args['tax_query'][ $filter['name'] ] = array(
			'taxonomy'         => $filter['taxonomy']->name,
			'field'            => 'id',
			'terms'            => array_keys( $filter['values']['terms'] ),
			'operator'         => $filter['values']['operator'],
			'include_children' => $include_children,
		);
	}
}

/******************************** FORMS ***************************************/
/*
 * Filter form
 */
function qw_filter_taxonomies_form( $filter ) {
	$terms = get_terms( $filter['taxonomy']->name, array( 'hide_empty' => FALSE ) );
	qw_filter_taxonomies_form_terms_checkboxes( $filter, $terms );
	qw_filter_taxonomies_form_operator( $filter );
	qw_filter_taxonomies_form_include_children( $filter );
}

/*
 * Term checkboxes for admin form
 */
function qw_filter_taxonomies_form_terms_checkboxes( $filter, $terms ) { ?>
	<div class="qw-checkboxes">
		<?php
		// List all categories as checkboxes
		foreach ( $terms as $term ) {
			$term_checked = ( isset( $filter['values']['terms'][ $term->term_id ] ) ) ? 'checked="checked"' : '';
			?>
			<label class="qw-query-checkbox">
				<input class="qw-js-title"
				       type="checkbox"
				       name="<?php print $filter['form_prefix']; ?>[terms][<?php print $term->term_id; ?>]"
				       value="<?php print $term->name; ?>"
					<?php print $term_checked; ?> />
				<?php print $term->name; ?>
			</label>
		<?php
		}
		?>
	</div>
<?php
}

/*
 * Admin operator form
 */
function qw_filter_taxonomies_form_operator( $filter ) { ?>
	<div>
		<p>
			<label class="qw-label"><?php print $filter['taxonomy']->label; ?>
				Operator:</label>
			<select name="<?php print $filter['form_prefix']; ?>[operator]"
			        class="qw-field-value qw-js-title">
				<option
					value="IN" <?php if ( $filter['values']['operator'] == "IN" ) {
					print 'selected="selected"';
				} ?>>
					(In) Posts with term
				</option>
				<option
					value="NOT IN" <?php if ( $filter['values']['operator'] == "NOT IN" ) {
					print 'selected="selected"';
				} ?>>
					(NOT IN) Posts without term
				</option>
				<option
					value="AND" <?php if ( $filter['values']['operator'] == "AND" ) {
					print 'selected="selected"';
				} ?>>
					(ALL) Posts with All terms
				</option>
			</select>
		</p>
		<p class="description">Test results against the chosen operator.</p>
	</div>
<?php
}

/*
 * Admin Include Children form
 */
function qw_filter_taxonomies_form_include_children( $filter ) {
	$include_children = ( isset( $filter['values']['include_children'] ) ) ? 'checked="checked"' : '';
	?>
	<div>
		<p>
			<label class="qw-label">Include children:</label>
			<input type="checkbox"
			       name="<?php print $filter['form_prefix']; ?>[include_children]"
				<?php print $include_children; ?> />
		</p>

		<p class="description clear-left">Include the term's children.</p>
	</div>
<?php
}

/*
 * Exposed settings form
 */
function qw_filter_taxonomies_exposed_settings_form( $filter ) {
	// use the default provided single/multiple exposed values
	// saves values to [exposed_settings][type]
	print qw_exposed_setting_type( $filter );
}

/*
 * Process submitted exposed form values
 */
function qw_filter_taxonomies_exposed_process( &$args, $filter, $values ) {
	$alter_args = FALSE;

	switch ( $filter['values']['exposed_settings']['type'] ) {
		case 'select':
			$alter_args                = TRUE;
			$filter['values']['terms'] = array( $values => 'on' );
			break;

		case 'checkboxes':
			if ( is_array( $values ) ) {
				$alter_args = TRUE;
				// gather the terms into the array expected by qw_filter_taxonomies_args()
				$terms = array();
				foreach ( $values as $v ) {
					$terms[ $v ] = 'on';
				}
				$filter['values']['terms'] = $terms;
			}
			break;
	}

	if ( $alter_args ) {
		qw_filter_taxonomies_args( $args, $filter );
	}
}

/*
 * Exposed forms
 */
function qw_filter_taxonomies_exposed_form( $filter, $values ) {
	$filter['values']['submitted'] = $values;
	$terms = array();
	$t = get_terms( $filter['taxonomy']->name, array( 'hide_empty' => FALSE ) );
	// handle limited options
	$t = qw_filter_taxonomies_exposed_limit_values( $filter, $t );
	qw_sort_terms_hierarchically( $t, $terms );

	switch ( $filter['values']['exposed_settings']['type'] ) {
		case 'select':
			qw_filter_taxonomies_exposed_form_terms_select( $filter, $terms );
			break;

		case 'checkboxes':
			qw_filter_taxonomies_exposed_form_terms_checkboxes( $filter, $terms );
			break;
	}
}

/*
 * Exposed terms as select box
 */
function qw_filter_taxonomies_exposed_form_terms_select( $filter, $terms ) {
	// handle submitted values
	if ( isset( $filter['values']['submitted'] ) ) {
		$filter['values']['terms'] = $filter['values']['submitted'];

		// select boxes submit as single values
		if ( !is_array( $filter['values']['terms'] ) ){
			$filter['values']['terms'] = array( $filter['values']['terms'] );
		}

	}

	?>
	<div class="query-select">
		<select name="<?php print $filter['exposed_key']; ?>">
			<?php
			qw_filter_taxonomies_exposed_form_terms_options($filter,$terms)
			?>
		</select>
	</div>
<?php
}

/*
 * Select box options recursion
 */
function qw_filter_taxonomies_exposed_form_terms_options($filter,$terms,$level = 0){
	// handle submitted values
	if ( isset( $filter['values']['submitted'] ) ) {
		$filter['values']['terms'] = $filter['values']['submitted'];

		// select boxes submit as single values
		if ( !is_array( $filter['values']['terms'] ) ){
			$filter['values']['terms'] = array( $filter['values']['terms'] );
		}

	}

	foreach ( $terms as $term ) {
		$term_selected = ( in_array( $term->term_id, $filter['values']['terms'] ) ) ? 'selected="selected"' : '';
		?>
		<option
			value="<?php print $term->term_id; ?>"<?php print $term_selected; ?> >
			<?php print $term->name; ?>
		</option>
	<?php
		if ( ! empty( $term->children ) ) {
			qw_filter_taxonomies_exposed_form_terms_options( $filter, $term->children, ($level + 1) );
		}
	}
}

/*
 * Exposed terms as checkboxes
 */
function qw_filter_taxonomies_exposed_form_terms_checkboxes(
	$filter,
	$terms,
	$wrapper_class = ""
) {

	?>
	<div class="query-checkboxes <?php print $wrapper_class;?>">
		<?php
		// List all categories as checkboxes
		foreach ( $terms as $term ) {
			$checked = ( is_array( $filter['values']['submitted'] ) && in_array( $term->term_id, $filter['values']['submitted'] ) );
			?>
			<label class="query-checkbox">
				<input type="checkbox"
				       name="<?php print $filter['exposed_key']; ?>[]"
				       value="<?php print $term->term_id; ?>"
					<?php checked( $checked, true ); ?> />
				<?php print $term->name; ?>
			</label>
			<?php
			if ( ! empty( $term->children ) ) {
				qw_filter_taxonomies_exposed_form_terms_checkboxes( $filter, $term->children, "children" );
			}
		}
		?>
	</div>
<?php
}

/*
 * Simple helper function to determine values with consideration for defaults
 */
function qw_filter_taxonomies_exposed_limit_values( $filter, $terms ) {
	$limited = array();
	if ( isset( $filter['values']['exposed_limit_values'] ) && is_array( $filter['values']['terms'] ) ) {
		foreach ( $terms as $k => $term ) {
			if ( isset( $filter['values']['terms'][ $term->term_id ] ) ) {
				$limited[ $term->term_id ] = $term;
			}
		}
	}
	else {
		foreach($terms as $k => $term){
			$limited[ $term->term_id ] = $term;
		}
	}

	return $limited;
}

/**
 * Recursively sort an array of taxonomy terms hierarchically. Child categories
 * will be placed under a 'children' member of their parent term.
 *
 * @param Array $cats taxonomy term objects to sort
 * @param Array $into result array to put them in
 * @param integer $parentId the current parent ID to put them in
 */
function qw_sort_terms_hierarchically(
	Array &$cats,
	Array &$into,
	$parentId = 0
) {
	if(0 == $parentId){
		//used to limit the number of expensive loops for orphans
		$cats_copy = $cats;
	}

	foreach ( $cats as $i => $cat ) {
		if ( $cat->parent == $parentId ) {
			$into[ $cat->term_id ] = $cat;
			unset( $cats[ $i ] );
		}
	}

	foreach ( $into as $topCat ) {
		$topCat->children = array();
		qw_sort_terms_hierarchically( $cats, $topCat->children, $topCat->term_id );
	}

	//handle terms that are orphans based on limited values
	if(0 == $parentId && count($cats) > 0){
		while(count($cats) > 0){
			foreach($cats as $term_id => $cat){
				if(isset($cats_copy[$cat->parent])){
					continue;
				}

				$cat->children = array();
				$into[$cat->term_id] = $cat;
				unset($cats[$cat->term_id]);
				qw_sort_terms_hierarchically( $cats, $cat->children, $cat->term_id );
			}
		}
	}
}
