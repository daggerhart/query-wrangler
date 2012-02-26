<form id="qw-edit-settings" action='admin.php?page=query-wrangler&action=save_settings&noheader=true' method='post'>
<input class="button-primary" type="submit" value="Save Settings" />

<div id="qw-settings-page" class="ui-corner-all">
  <div class="qw-setting">

    <label class="qw-label">Editor Theme:</label>
    <select name="qw-theme">
      <?php
        foreach($edit_themes as $key => $edit_theme)
        {
          $selected = ($theme == $key) ? 'selected="selected"': '';
          ?>
          <option value="<?php print $key; ?>" <?php print $selected; ?>>
            <?php print $edit_theme['title']; ?>
          </option>
          <?php
        }
      ?>
    </select>
    <p class="description">Choose the Query Wrangler editor theme.</p>
  </div>
</div>
</form>