<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_meta_key_value' );

function qw_filter_meta_key_value( $filters ) {

	$filters['meta_key_value'] = array(
		'title'               => 'Meta Key/Value Compare',
		'description'         => 'Filter for a specific meta_key / meta_value pair.',
		'form_callback'       => 'qw_filter_meta_key_value_form',
		'query_args_callback' => 'qw_generate_query_args_meta_key_value',
		'query_display_types' => array( 'page', 'widget', 'override' ),
	);

	return $filters;
}

function qw_filter_meta_key_value_form( $filter ) {
	if ( ! isset( $filter['values']['meta_key'] ) ) {
		$filter['values']['meta_key'] = '';
	}
	if ( ! isset( $filter['values']['meta_compare'] ) ) {
		$filter['values']['meta_compare'] = '';
	}
	if ( ! isset( $filter['values']['meta_value'] ) ) {
		$filter['values']['meta_value'] = '';
	}

	$meta_compare = array(
		"="  => "Is equal to",
		"!=" => "Is not equal to",
		"<"  => "Is less than",
		"<=" => "Is less than or equal to",
		">"  => "Is greater than",
		">=" => "Is greater than or equal to",
	);
	?>
	<p>
		<label class="qw-label">Meta Key:</label>
		<input class='qw-js-title' type='text'
		       name="<?php print $filter['form_prefix']; ?>[meta_key]"
		       value='<?php print $filter['values']['meta_key']; ?>'/>
	</p>
	<p>
		<label class="qw-label">Compare:</label>
		<select class='qw-js-title'
		        name="<?php print $filter['form_prefix']; ?>[meta_compare]">
			<?php
			foreach ( $meta_compare as $op => $title ) {
				$selected = ( $filter['values']['meta_compare'] == $op ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $op; ?>"  <?php print $selected; ?>><?php print $title; ?></option>
			<?php
			}
			?>
		</select>
	</p>
	<p>
		<label class="qw-label">Meta Value:</label>
      <textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
                class="qw-meta-value qw-js-title"><?php print stripcslashes( $filter['values']['meta_value'] ); ?></textarea>
	</p>
<?php
}

function qw_generate_query_args_meta_key_value( &$args, $filter ) {
	$args['meta_key']     = $filter['values']['meta_key'];
	$args['meta_value']   = stripslashes( $filter['values']['meta_value'] );
	$args['meta_compare'] = $filter['values']['meta_compare'];
}