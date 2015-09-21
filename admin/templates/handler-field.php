<?php
$link_selected               = ( isset( $field['values']['link'] ) ) ? 'checked="checked"' : '';
$has_label                   = ( isset( $field['values']['has_label'] ) ) ? 'checked="checked"' : '';
$label                       = ( isset( $field['values']['label'] ) ) ? $field['values']['label'] : "";
$rewrite_output_selected     = ( isset( $field['values']['rewrite_output'] ) ) ? 'checked="checked"' : '';
$exclude_display_selected    = ( isset( $field['values']['exclude_display'] ) ) ? 'checked="checked"' : '';
$apply_the_content           = ( isset( $field['values']['apply_the_content'] ) ) ? 'checked="checked"' : '';
$hide_if_empty               = ( isset( $field['values']['hide_if_empty'] ) ) ? 'checked="checked"' : '';
$classes                     = ( isset( $field['values']['classes'] ) ) ? $field['values']['classes'] : '';
$empty_field_content         = ( isset( $field['values']['empty_field_content'] ) ) ? $field['values']['empty_field_content'] : '';
$empty_field_content_enabled = ( isset( $field['values']['empty_field_content_enabled'] ) ) ? $field['values']['empty_field_content_enabled'] : '';
?>
<div id="qw-field-<?php print $field['name']; ?>"
     class="qw-field qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php
    print $field['title'];
    if ( $has_label ) {
	    print ': ' . $field['values']['label'];
    }
    ?>
  </span>

	<div class="group">
		<input class='qw-field-type '
		       type='hidden'
		       name='<?php print $field['form_prefix']; ?>[type]'
		       value='<?php print $field['type']; ?>'/>
		<input class='qw-field-hook_key'
		       type='hidden'
		       name='<?php print $field['form_prefix']; ?>[hook_key]'
		       value='<?php print $field['hook_key']; ?>'/>
		<input class='qw-field-name qw-js-title'
		       type='hidden'
		       name='<?php print $field['form_prefix']; ?>[name]'
		       value='<?php print $field['name']; ?>'/>


		<div class="qw-remove button">
			Remove
		</div>
		<div class="qw-weight-container">
			Weight:
			<input class='qw-weight'
			       name='qw-query-options[display][field_settings][fields][<?php print $field['name']; ?>][weight]'
			       type='text' size='2'
			       value='<?php print $weight; ?>'/>
		</div>

		<p class="description"><?php print $field['description']; ?></p>

		<?php if ( isset( $field['form'] ) ) { ?>
			<div class="qw-field-form qw-setting">
				<?php print $field['form']; ?>
			</div>
		<?php } ?>

		<div class='qw-field-options'>
			<!-- exclude display -->
			<label class='qw-field-checkbox qw-field-row'>
				<input type='checkbox'
				       name='<?php print $field['form_prefix']; ?>[exclude_display]'
					<?php print $exclude_display_selected; ?> />
				Exclude this field from display
			</label>

			<!-- link -->
			<label class='qw-field-checkbox qw-field-row'>
				<input type='checkbox'
				       name='<?php print $field['form_prefix']; ?>[link]'
					<?php print $link_selected; ?> />
				Link this field to the post
			</label>

			<!-- label -->
			<div class="qw-options-group qw-field-row">
				<div class="qw-options-group-title">
					<label class='qw-field-checkbox'>
						<input type='checkbox'
						       name='<?php print $field['form_prefix']; ?>[has_label]'
							<?php print $has_label; ?> />
						Create a Label for this field.
					</label>
				</div>
				<div class="qw-options-group-content qw-field-options-hidden">
					<strong>Label Text: </strong>
					<input class='qw-js-title'
					       type="text"
					       name="<?php print $field['form_prefix']; ?>[label]"
					       value="<?php print $label; ?>"/>
				</div>
			</div>

			<?php
			if ( isset( $field['content_options'] ) && $field['content_options'] ) {
				?>
				<label class='qw-field-checkbox qw-field-row'>
					<input type='checkbox'
					       name='<?php print $field['form_prefix']; ?>[apply_the_content]'
						<?php print $apply_the_content; ?> />
					Apply "the_content" filter to this field
				</label>
			<?php
			}
			?>

			<!-- hide_if_empty -->
			<label class='qw-field-checkbox qw-field-row'>
				<input type='checkbox'
				       name='<?php print $field['form_prefix']; ?>[hide_if_empty]'
					<?php print $hide_if_empty; ?> />
				Hide field if empty
			</label>

			<!-- rewrite output -->
			<div class="qw-options-group qw-field-row">
				<div class="qw-options-group-title">
					<label class='qw-field-checkbox'>
						<input type='checkbox'
						       name='<?php print $field['form_prefix']; ?>[rewrite_output]'
							<?php print $rewrite_output_selected; ?> />
						Rewrite the output of this field
					</label>
				</div>
				<div class="qw-options-group-content qw-field-options-hidden">
          <textarea name='<?php print $field['form_prefix']; ?>[custom_output]'
                    class="qw-field-textarea"><?php print ( isset( $field['values']['custom_output'] ) ) ? qw_textarea( $field['values']['custom_output'] ) : ''; ?></textarea>

					<div class="qw-field-tokens">
						<p>
							Available replacement tokens. These tokens will be
							replaced with the processed results of their fields.
						</p>
						<ul class="qw-field-tokens-list">
							<?php
							if ( isset( $tokens ) && is_array( $tokens ) ) {
								foreach ( $tokens as $token ) { ?>
									<li><?php print $token; ?></li>
								<?php
								}
							}
							?>
						</ul>
					</div>
				</div>
			</div>

			<!-- additional field classes -->
			<div class="qw-field-wrapper qw-field-row">
				<label class='qw-field'>
					<strong>Additional Classes</strong><br/>
					<input type='text'
					       name='<?php print $field['form_prefix']; ?>[classes]'
					       value='<?php print $classes; ?>'/>
				</label>

				<p class="description">Additional CSS classes to add to the
					field during output. Separate multiple classes with
					spaces.</p>
			</div>
		</div>

		<!-- enable empty field content -->
		<div class="qw-options-group qw-field-row">
			<div class="qw-options-group-title">
				<label class='qw-field-checkbox'>
					<input type='checkbox'
					       name='<?php print $field['form_prefix']; ?>[empty_field_content_enabled]'
						<?php print $empty_field_content_enabled; ?> />
					Rewrite empty result text
				</label>
			</div>
			<div class="qw-options-group-content qw-field-options-hidden">
				<!-- empty field content -->
				<label class='qw-field'>
          <textarea
	          name='<?php print $field['form_prefix']; ?>[empty_field_content]'
	          class="qw-field-textarea"><?php print qw_textarea( $empty_field_content ); ?></textarea>
				</label>

				<p class="description">Field settings will apply to this
					content.</p>

				<div class="qw-field-tokens">
					<p>
						Available replacement tokens. These tokens will be
						replaced with the processed results of their fields.
					</p>
					<ul class="qw-field-tokens-list">
						<?php
						if ( isset( $tokens ) && is_array( $tokens ) ) {
							foreach ( $tokens as $token ) { ?>
								<li><?php print $token; ?></li>
							<?php
							}
						}
						?>
					</ul>
				</div>
			</div>
		</div>

	</div>
</div>