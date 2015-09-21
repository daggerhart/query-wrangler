<form id="qw-edit-settings"
      action="<?php print admin_url( 'admin.php?page=query-wrangler&action=save_settings&noheader=true' ); ?>"
      method='post'>
	<?php submit_button( 'Save Settings' ); ?>
	<table class="form-table">
		<tr>
			<th>
				<label>Editor Theme</label>
			</th>
			<td>
				<p class="description">Choose the Query Wrangler editor
					theme.</p>
				<select name="qw-theme">
					<?php
					foreach ( $edit_themes as $key => $edit_theme ) { ?>
						<option
							value="<?php print $key; ?>" <?php selected( $key,
							$theme ); ?>>
							<?php print $edit_theme['title']; ?>
						</option>
					<?php
					}
					?>
				</select>
			</td>
		</tr>

		<tr>
			<th>
				<label>Widget Theme Compatibility</label>
			</th>
			<td>
				<input type="checkbox"
				       name="qw-widget-theme-compat" <?php checked( $widget_theme_compat,
					"on" ); ?> />

				<p class="description">If you're having trouble with the way
					Query Wrangler Widgets appear in your sidebar, select this
					option.</p>
			</td>
		</tr>

		<tr>
			<th>
				<label>Live Preview</label>
			</th>
			<td>
				<input type="checkbox"
				       name="qw-live-preview" <?php checked( $live_preview,
					"on" ); ?> />

				<p class="description">Default setting for live preview during
					query editing.</p>
			</td>
		</tr>

		<tr>
			<th>
				<label>Show Silent Meta fields</label>
			</th>
			<td>
				<input type="checkbox"
				       name="qw-show-silent-meta" <?php checked( $show_silent_meta,
					"on" ); ?> />

				<p class="description">Show custom meta fields that are normally
					hidden.</p>
			</td>
		</tr>

		<tr>
			<th>
				<label>Meta Value field handler</label>
			</th>
			<td>
				<p class="description">Choose the way meta_value fields are
					handled.</p>
				<select name="qw-meta-value-field-handler">
					<?php
					foreach ( $meta_value_field_options as $value => $text ) { ?>
						<option
							value="<?php print $value; ?>" <?php selected( $value,
							$meta_value_field_handler ); ?>>
							<?php print $text; ?>
						</option>
					<?php
					}
					?>
				</select>
				<ul>
					<li><b>Default</b> - each meta_key is treated as a unique
						field in the UI.
					</li>
					<li><b>New</b> - a generic "Custom field" is available in
						the UI, and you must provide it the meta key.
					</li>
				</ul>
			</td>
		</tr>
	</table>
</form>