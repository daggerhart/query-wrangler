
<!-- Preview -->
<div id="query-preview" class="qw-query-option">
  <div id="query-preview-controls" class="query-preview-inactive">
    <label>
      <?php $live_preview_checked = (get_option('qw_live_preview', true)) ? 'checked="checked"' : ""; ?>
      <input id="live-preview"
             type="checkbox"
        <?php print $live_preview_checked; ?> />
      Live Preview
    </label>
    <div id="get-preview" class="qw-button">Preview</div>
  </div>

  <h4 id="preview-title">Preview Query</h4>
  <p><em>This preview does not include your theme's CSS stylesheet.</em></p>
  <div id="query-preview-target">
    <!-- preview -->
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

  <div id="query-details">
    <div class="group">
      <div class="qw-setting-header">WP_Query Arguments</div>
      <div id="qw-show-arguments-target">
        <!-- args -->
      </div>
    </div>
    <div class="group">
      <div class="qw-setting-header">PHP WP_Query</div>
      <div id="qw-show-php_wpquery-target">
        <!-- php wp_query -->
      </div>
    </div>
    <div class="group">
      <div class="qw-setting-header">Display Settings</div>
      <div id="qw-show-display-target">
        <!-- display -->
      </div>
    </div>
    <div class="group">
      <div class="qw-setting-header">Resulting WP_Query Object</div>
      <div id="qw-show-wpquery-target">
        <!-- WP_Query -->
      </div>
    </div>
    <div class="group">
      <div class="qw-setting-header">Template Suggestions</div>
      <div id="qw-show-templates-target">
        <!-- WP_Query -->
      </div>
    </div>
  </div>

</div>
</form>
