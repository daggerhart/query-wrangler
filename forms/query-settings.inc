<form id="qw-edit-settings" action='admin.php?page=query-wrangler&action=save_settings&noheader=true' method='post'>
<input class="button" type="submit" value="Save Settings" />

<hr />
<h3>Editor Theme</h3>
<p>
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
</p>
</form>