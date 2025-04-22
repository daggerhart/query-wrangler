<?php
/**
 * @var string $edit_theme
 * @var bool $widget_theme_compat
 * @var bool $live_preview
 * @var bool $show_silent_meta
 * @var bool $shortcode_compat
 * @var string $meta_value_field_handler
 * @var string $meta_key_cache_life
 * @var array[] $edit_themes
 * @var array $meta_value_field_options
 */
?>
<form id="qw-edit-settings"
      action="<?php print admin_url( 'admin.php?page=query-wrangler&action=save_settings&noheader=true' ); ?>"
      method='post'>

	<?php wp_nonce_field( 'qw-settings' ); ?>

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
					foreach ( $edit_themes as $key => $theme ) { ?>
						<option
							value="<?php print $key; ?>" <?php selected( $key, $edit_theme ); ?>>
							<?php print $theme['title']; ?>
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
						<option value="<?php print $value; ?>" <?php selected( $value, $meta_value_field_handler ); ?>>
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
		<tr>
			<th>
				<label>Meta Keys Cache Life</label>
			</th>
			<td>
				<p class="description">
					Discovering meta keys can be very expensive and time-consuming query for large sites.
					It's recommended to enable <em>some</em> amount of caching on this.
				</p>
				<?php
				$meta_key_cache_options = [
					'none' => '- ' . __('No Caching') . ' -',
					3600 => __('1 Hour'),
					DAY_IN_SECONDS => __('1 Day'),
					WEEK_IN_SECONDS => __('1 Week'),
					0 => __('Forever'),
				];
				?>
				<select name="qw-meta-keys-cache-life">
					<?php foreach ( $meta_key_cache_options as $value => $label ) { ?>
						<option value="<?= esc_attr($value) ?>" <?php selected($value, $meta_key_cache_life) ?>>
							<?= esc_html( $label ) ?>
						</option>
					<?php } ?>
				</select>
				<ul>
					<li><b>No Caching</b> - Never cache meta keys. This could cause performance issues.</li>
					<li><b>Forever</b> - Cache meta keys until a Query Wrangler Query is edited and saved.</li>
				</ul>
			</td>
		</tr>

		<tr>
			<th>
				<label>Shortcode compatibility</label>
			</th>
			<td>
				<input type="checkbox"
				       name="qw-shortcode-compat" <?php checked( $shortcode_compat, "on" ); ?> />

				<p class="description">Change the shortcode keyword from <code>query</code> to <code>qw_query</code>, to avoid conflicts with other plugins.</p>
				<p>
					Example usage:
					<br>
					<b>Compatibility Disabled</b>- <code>[query slug="my-test"]</code>
					<br>
					<b>Compatibility Enabled</b>- <code>[qw_query slug="my-test"]</code>
				</p>
			</td>
		</tr>
	</table>
</form>
