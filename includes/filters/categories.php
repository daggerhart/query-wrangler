<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_categories' );

function qw_filter_categories( $filters ) {

	$filters['categories'] = array(
		'title'               => 'Categories',
		'description'         => 'Select which categories to pull posts from, and how to treat those categories.',
		'form_callback'       => 'qw_filter_categories_form',
		'query_args_callback' => 'qw_generate_query_args_categories',
		'query_display_types' => array( 'page', 'widget' ),
	);

	return $filters;
}

/**
 * Options for "categories" filter
 *
 * @param $filter
 */
function qw_filter_categories_form( $filter ) {
	$cat_ops = array(
		"cat"              => "Any category plus children categories",
		"category__in"     => "Any category without children categories",
		"category__and"    => "All categories selected",
		"category__not_in" => "Not in the categories selected",
	);
	?>
	<div class="qw-checkboxes">
		<?php
		$categories = get_terms( 'category',
			array( 'fields' => 'id=>name', 'hide_empty' => 0 ) );
		// List all categories as checkboxes
		foreach ( $categories as $cat_id => $cat_name ) {
			$cat_checked = ( isset( $filter['values']['cats'][ $cat_id ] ) ) ? 'checked="checked"' : '';
			?>
			<label class="qw-query-checkbox">
				<input class=" qw-js-title"
				       type="checkbox"
				       name="<?php print $filter['form_prefix']; ?>[cats][<?php print $cat_id; ?>]"
				       value="<?php print $cat_name; ?>"
					<?php print $cat_checked; ?> />
				<?php print $cat_name; ?>
			</label>
		<?php
		}
		?>
	</div>
	<p><strong>Categories Options</strong> - show posts that are:</p>
	<p>
		<select class="qw-field-value qw-js-title"
		        name="<?php print $filter['form_prefix']; ?>[cat_operator]">
			<?php
			foreach ( $cat_ops as $op => $title ) {
				$selected = ( $filter['values']['cat_operator'] == $op ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $op;?>" <?php print $selected; ?>><?php print $title; ?></option>
			<?php
			}
			?>
		</select>
	</p>
<?php
}

/**
 * Alter the values for the given category operator
 *
 * @param $args
 * @param $filter
 */
function qw_generate_query_args_categories( &$args, $filter ) {
	// category__not_in wants and array of term ids
	if ( isset( $filter['values']['cat_operator'] ) && isset( $filter['values']['cats'] ) ) {
		if ( $filter['values']['cat_operator'] == 'category__not_in' && is_array( $filter['values']['cats'] ) ) {
			$args[ $filter['values']['cat_operator'] ] = array_keys( $filter['values']['cats'] );
		} // cats wants a comma separated string
		else if ( $filter['values']['cat_operator'] == 'cat' && is_array( $filter['values']['cats'] ) ) {
			$args[ $filter['values']['cat_operator'] ] = implode( ",",
				array_keys( $filter['values']['cats'] ) );
		}
	} //
	else if ( isset( $filter['values']['cats'] ) ) {
		$args[ $filter['values']['cat_operator'] ] = $filter['values']['cats'];
	}
}