<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_author' );

function qw_filter_author( $filters ) {

	$filters['author'] = array(
		'title'               => 'Author',
		'description'         => 'Filter posts by author',
		'form_callback'       => 'qw_filter_author_form',
		'query_args_callback' => 'qw_generate_query_args_author',
		'query_display_types' => array( 'page', 'widget' ),
	);

	return $filters;
}

function qw_filter_author_form( $filter ) {
	$aut_ops = array(
		"author"         => "Author ids",
		"author_name"    => "Author nice name",
		"author__in"     => "Authors in list of ids",
		"author__not_in" => "Authors Not in list of author ids",
	);
	if ( ! isset( $filter['values']['author_operator'] ) ) {
		$filter['values']['author_operator'] = '';
	}
	if ( ! isset( $filter['values']['author_values'] ) ) {
		$filter['values']['author_values'] = '';
	}
	?>
  <p><strong>Author Options</strong> - show posts that are from:</p>
  <p>
    <select class="qw-field-value qw-js-title" name="<?php print $filter['form_prefix']; ?>[author_operator]">
      <?php
	foreach ( $aut_ops as $op => $title ) {
		$selected = ( $filter['values']['author_operator'] == $op ) ? 'selected="selected"' : '';
		?>
		<option
			value="<?php print $op;?>" <?php print $selected; ?>><?php print $title; ?></option>
	<?php
	}
	?>
    </select>
  </p>
  <p>
    <strong>Values</strong>
    <input type="text" size="48" name="<?php print $filter['form_prefix']; ?>[author_values]" class="qw-js-title" value="<?php print $filter['values']['author_values']; ?>" />
    <p clas="description">Provide the values appropriate for the author option.  Ids should be comma separated.</p>
  </p>
  <?php
}


function qw_generate_query_args_author( &$args, $filter ) {
	if ( ! isset( $filter['values']['author_operator'] ) ) {
		$filter['values']['author_operator'] = '';
	}
	if ( ! isset( $filter['values']['author_values'] ) ) {
		$filter['values']['author_values'] = '';
	}

	$op = $filter['values']['author_operator'];
	if ( $op == "author" || $op == "author_name" ) {
		$args[ $op ] = $filter['values']['author_values'];
	} else {
		// turn values into array
		$args[ $op ] = array_map( 'trim',
			explode( ",", $filter['values']['author_values'] ) );
	}
}