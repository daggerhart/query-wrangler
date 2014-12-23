<form id="qw-edit-settings" action="<?php print admin_url('admin.php?page=query-wrangler&action=save_settings&noheader=true'); ?>" method='post'>
  <?php submit_button('Save Settings'); ?>
  <table class="form-table">
    <tr>
      <th>
        <label>Editor Theme</label>
      </th>
      <td>
        <p class="description">Choose the Query Wrangler editor theme.</p>
        <select name="qw-theme">
          <?php
            foreach($edit_themes as $key => $edit_theme)
            { ?>
              <option value="<?php print $key; ?>" <?php selected($key, $theme); ?>>
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
        <p class="description">If you're having trouble with the way Query Wrangler Widgets appear in your sidebar, select this option.</p>
        <input type="checkbox" name="qw-widget-theme-compat" <?php checked($widget_theme_compat, "on"); ?> />
      </td>
    </tr>

    <tr>
      <th>
        <label>Live Preview</label>
      </th>
      <td>
        <p class="description">Default setting for live preview during query editing.</p>
        <input type="checkbox" name="qw-live-preview" <?php checked($live_preview, "on"); ?> />
      </td>
    </tr>
  </table>
</form>