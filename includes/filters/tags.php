<?php

// add default filters to the filter
add_filter( 'qw_filters', 'qw_filter_tags' );

function qw_filter_tags( $filters ) {

	$filters['tags'] = array(
		'title'               => 'Tags',
		'description'         => 'Select which tags to use.',
		'form_callback'       => 'qw_filter_tags_form',
		'query_args_callback' => 'qw_generate_query_args_tags',
		'query_display_types' => array( 'page', 'widget' ),
	);

	return $filters;
}

function qw_filter_tags_form( $filter ) {
	$tag_ops = array(
		"tag__in"     => "Any of the selected tags",
		"tag__and"    => "All of the selected tags",
		"tag__not_in" => "None of the selected tags",
	);
	?>
	<div class="qw-checkboxes">
		<?php
		$tags = get_tags( array( 'hide_empty' => FALSE ) );
		foreach ( $tags as $tag ) {
			$tag_checked = ( isset( $filter['values']['tags'][ $tag->term_id ] ) ) ? 'checked="checked"' : '';
			?>
			<label class="qw-query-checkbox">
				<input class="qw-js-title"
				       type="checkbox"
				       name="<?php print $filter['form_prefix']; ?>[tags][<?php print $tag->term_id; ?>]"
				       value="<?php print $tag->name; ?>"
					<?php print $tag_checked; ?> />
				<?php print $tag->name; ?>
			</label>
		<?php
		}
		?>
	</div>
	<p><strong>Tag Options</strong> - show posts that have:</p>
	<p>
		<select class="qw-field-value qw-js-title"
		        name="<?php print $filter['form_prefix']; ?>[tag_operator]">
			<?php
			foreach ( $tag_ops as $op => $title ) {
				$selected = ( $filter['values']['tag_operator'] == $op ) ? 'selected="selected"' : '';
				?>
				<option
					value="<?php print $op; ?>" <?php print $selected; ?>><?php print $title; ?></option>
			<?php
			}
			?>
		</select>
	</p>
<?php
}

function qw_generate_query_args_tags( &$args, $filter ) {
	if ( isset( $filter['values']['tags'] ) && is_array( $filter['values']['tags'] ) ) {
		$args[ $filter['values']['tag_operator'] ] = array_keys( $filter['values']['tags'] );
	}
}