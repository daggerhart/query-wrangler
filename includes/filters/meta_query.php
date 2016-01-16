<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_meta_query' );

function qw_filter_meta_query( $filters ) {

	$filters['meta_query'] = array(
		'title'               => 'Meta Query',
		'description'         => 'Filter for a single meta query',
		'form_callback'       => 'qw_filter_meta_query_form',
		'query_args_callback' => 'qw_generate_query_args_meta_query',
		'query_display_types' => array( 'page', 'widget', 'override' ),
	);

	return $filters;
}

function qw_filter_meta_query_form( $filter ) {
	if ( ! isset( $filter['values']['key'] ) ) {
		$filter['values']['key'] = '';
	}
	if ( ! isset( $filter['values']['type'] ) ) {
		$filter['values']['type'] = '';
	}
	if ( ! isset( $filter['values']['compare'] ) ) {
		$filter['values']['compare'] = '';
	}
	if ( ! isset( $filter['values']['value'] ) ) {
		$filter['values']['value'] = array( '', '' );
	}
	$compares = array(
		"=",
		"!=",
		"<",
		"<=",
		">",
		">=",
		"LIKE",
		"NOT LIKE",
		"IN",
		"NOT IN",
		"BETWEEN",
		"NOT BETWEEN",
		"EXISTS",
		"NOT EXISTS",
	);
	$types    = array(
		"CHAR",
		"NUMERIC",
		"BINARY",
		"DATE",
		"DATETIME",
		"DECIMAL",
		"SIGNED",
		"TIME",
		"UNSIGNED"
	);

	?>
    <p>
      <label class="qw-label">Meta Key:</label>
      <input class='qw-js-title' type='text' name="<?php print $filter['form_prefix']; ?>[key]" value='<?php print $filter['values']['key']; ?>' />
    </p>
    <p>
      <label class="qw-label">Meta Value 1:</label>
      <input class="qw-meta-value qw-js-title"
             type="text"
             size="40"
             name="<?php print $filter['form_prefix']; ?>[value][0]"
             value="<?php print stripcslashes( $filter['values']['value'][0] ); ?>" />
    </p>
    <p>
      <label class="qw-label">Meta Value 2:</label>
      <input class="qw-meta-value qw-js-title"
             type="text"
             size="40"
             name="<?php print $filter['form_prefix']; ?>[value][1]"
             value="<?php print stripcslashes( $filter['values']['value'][1] ); ?>" />
      <div>Only use Meta Value 2 when compare is 'IN', 'NOT IN', 'BETWEEN', or 'NOT BETWEEN'.</div>
    </p>
    <p>
      <label class="qw-label">Type:</label>
      <select class='qw-js-title' name="<?php print $filter['form_prefix']; ?>[type]">
        <?php
	foreach ( $types as $type ) { ?>
		<option value="<?php print $type; ?>" <?php selected( $type,
			$filter['values']['type'] ); ?>><?php print $type; ?></option>
	<?php
	}
	?>
      </select>
    </p>
    <p>
      <label class="qw-label">Compare:</label>
      <select class='qw-js-title' name="<?php print $filter['form_prefix']; ?>[compare]">
      <?php
	foreach ( $compares as $compare ) { ?>
		<option value="<?php print $compare; ?>" <?php selected( $compare,
			$filter['values']['compare'] ); ?>><?php print $compare; ?></option>
	<?php
	}
	?>
      </select>
    </p>
  <?php
}

function qw_generate_query_args_meta_query( &$args, $filter ) {
	if ( ! empty( $filter['values']['value'][1] ) ) {
		$value = $filter['values']['value'];
	} else {
		$value = $filter['values']['value'][0];
	}

	$args['meta_query'][] = array(
		'key'     => $filter['values']['key'],
		'value'   => $value,
		'compare' => $filter['values']['compare'],
		'type'    => $filter['values']['type'],
	);
}