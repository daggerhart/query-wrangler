<?php
/*
 * Where do all these variables come from?
 * They are coming from the arguments sent along with the theme('query_edit', $args) function in query-wrangler.php
 *
 * All keys in the argument array become variables in the template file
 *
 * See the following link for more details on how that works:
 * https://github.com/daggerhart/Query-Wrangler/wiki/Template-Wrangler
 */

?>
<form id="qw-edit-query-form" action='admin.php?page=query-wrangler&action=update&edit=<?php print $query_id; ?>&noheader=true' method='post'>
  <div id="qw-query-action-buttons">
    <div id="query-actions">
      <a href="admin.php?page=query-wrangler&export=<?php print $query_id; ?>">Export</a>
    </div>
    <input class='button-primary' type="submit" name="save" value="Save" />
    <input type="hidden" name="query-id" value="<?php print $query_id; ?>" />
  </div>
  <div id="message" class="updated qw-changes">
    <p><strong>*</strong> Changes have been made that need to be saved.</p>
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

  <div id="qw-query-admin-options-wrap">

<!-- left column -->
    <div class="qw-query-admin-column">
      <div id="qw-query-args" class="qw-query-admin-options">
        <h4>Basic Settings</h4>
        <?php
          foreach($basics as $basic) {
            // no display types set means all querys  || query type is in display types
            if (!isset($basic['query_display_types']) || (in_array($query_type, $basic['query_display_types'])))
            {
              ?>
              <div class="qw-query-title" title="qw-basic-<?php print $basic['hook_key']; ?>">
                <span class="qw-setting-title"><?php print $basic['title']; ?></span>:
                <span class="qw-setting-value">&nbsp;</span>
              </div>
              <?php
            }
          }
        ?>
      </div> <!-- /qw-query-args -->
      <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
    </div>
    <!-- /column -->

<!-- middle column -->
    <div class="qw-query-admin-column">
      <?php
        if($query_type == 'override')
        { ?>
          <!-- override settings -->
          <div id="qw-override-settings" class="qw-query-admin-options">
            <h4>Override Settings</h4>

            <div class="qw-query-title" title="qw-override-categories">
              <span class="qw-setting-title">Categories</span>:
              <span class="qw-setting-value">
                <?php print (is_array($options['override']['cats'])) ? implode(",", $options['override']['cats']): 'None'; ?>
              </span>
            </div>

            <div class="qw-query-title" title="qw-override-tags">
              <span class="qw-setting-title">Tags</span>:
              <span class="qw-setting-value">
                <?php print (is_array($options['override']['tags'])) ? implode(",", $options['override']['tags']): 'None'; ?>
              </span>
            </div>
          </div>
          <!-- /override settings -->
          <?php
        }
      ?>

      <div id="qw-query-fields" class="qw-query-admin-options">
        <div class="qw-query-add-titles">
          <span class="qw-rearrange-title" title="qw-sort-fields">
            Rearrange
          </span>
          <span class="qw-query-title" title="qw-display-add-fields">
            Add
          </span>
        </div>
				
				<h4>Fields</h4>
        <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
        <div id="qw-query-fields-list">
          <?php
            if(is_array($fields)){
              foreach($fields as $field)
              { ?>
                  <div class="qw-query-title" title="qw-field-<?php print $field['name']; ?>">
                    <span class="qw-setting-title"><?php print $field['title'];  ?></span>:
                    <span class="qw-setting-value"><?php print $field['name']; ?></span>
                  </div>
                <?php
              }
            }
          ?>
        </div>
      </div>
      <!-- /fields -->
    </div>
    <!-- /column -->

<!-- right column -->
    <div class="qw-query-admin-column">
    <!-- sorts -->
      <div id="qw-query-sorts" class="qw-query-admin-options">
        <div class="qw-query-add-titles">
          <span class="qw-rearrange-title" title="qw-sort-sorts">
            Rearrange
          </span>
          <span class="qw-query-title" title="qw-display-add-sorts">
            Add
          </span>
        </div>
        <h4>Sort Options</h4>
        <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
        <div id="qw-query-sorts-list">
          <?php
            if(is_array($sorts)){
              foreach($sorts as $sort)
              { ?>
                  <div class="qw-query-title" title="qw-sort-<?php print $sort['name']; ?>">
                    <span class="qw-setting-title"><?php print $sort['title'];  ?></span>:
                    <span class="qw-setting-value">&nbsp;</span>
                  </div>
                <?php
              }
            }
          ?>
        </div>
      </div>

    <!-- filters -->
      <div id="qw-query-filters" class="qw-query-admin-options">
        <div class="qw-query-add-titles">
          <span class="qw-rearrange-title" title="qw-sort-filters">
            Rearrange
          </span>
          <span class="qw-query-title" title="qw-display-add-filters">
            Add
          </span>
        </div>
        <h4>Filters</h4>
        <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

        <div id="qw-query-filters-list">
          <?php
            // loop through and display
            if(is_array($filters)){
              foreach($filters as $filter_name => $filter)
              {
                ?>
                  <div class="qw-query-title" title="qw-filter-<?php print $filter_name; ?>">
                    <span class="qw-setting-title"><?php print $filter['title']; ?></span>:
                    <span class="qw-setting-value">&nbsp;</span>
                  </div>
                <?php
              }
            }
          ?>
        </div>
      </div>
    </div>
    <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
  </div>

<!-- ------- FORMS --------- -->
      <div id="qw-options-forms">
<!-- Basic Settings -->
      <div id="existing-basics">
        <?php
          foreach($basics as $basic)
          {
            if(isset($basic['form_callback']) && function_exists($basic['form_callback']))
            { ?>
              <div id="qw-basic-<?php print $basic['hook_key']; ?>" class="qw-item-form">
                <?php
                  ob_start();
                    $basic['form_callback']($basic, $options[$basic['option_type']]);
                  print ob_get_clean();
                ?>
              </div>
              <?php
            }
          }
        ?>
      </div>

<!-- Page Settings -->

<!-- Edit Existing handlers -->
      <!-- edit sorts -->
      <div id="existing-sorts">
        <?php
          if(is_array($sorts)) {
            // loop through existing sorts
            foreach($sorts as $sort_name => $sort)
            {
              $args = array(
                'sort' => $sort,
                'weight' => $sort['weight'],
              );
              print theme('query_sort', $args);
            }
          }
        ?>
      </div>
      <!-- /edit sorts -->

      <!-- edit Filters -->
      <div id="existing-filters">
        <?php
          if(is_array($filters)){
            // loop through existing filters
            foreach($filters as $filter_name => $filter)
            {
              $args = array(
                'filter' => $filter,
                'weight' => $filter['weight'],
              );
              print theme('query_filter', $args);
            }
          }
        ?>
      </div>
      <!-- /edit filters -->

      <!-- edit fields -->
      <div id="existing-fields">
        <?php
          if(is_array($fields)) {
            $tokens = array();
            // loop through existing fields
            foreach($fields as $field)
            {
              $tokens[$field['name']] = '{{'.$field['name'].'}}';
              $args = array(
                'image_sizes' => $image_sizes,
                'file_styles' => $file_styles,
                'field' => $field,
                'weight' => $field['weight'],
                'options' => $options,
                'display' => $display,
                'args'  => $args,
                'tokens' => $tokens,
              );
              print theme('query_field', $args);
            }
          }
        ?>
      </div>
      <!-- /edit fields -->

<!-- Add Handlers -->
        <!-- add sorts -->
        <div id="qw-display-add-sorts" class="qw-hidden">
          <input class="add-handler-type" type="hidden" value="sort">
          <p class="description">Select options for sorting the query results.</p>
          <div class="qw-checkboxes">
            <?php
              // loop through sorts
              foreach($all_sorts as $hook_key => $sort)
              {
                ?>
                <label class="qw-sort-checkbox">
                  <input type="checkbox"
                         value="<?php print $sort['type']; ?>" />
                  <input class="qw-hander-hook_key"
                         type="hidden"
                         value="<?php print $sort['hook_key']; ?>" />
                  <?php print $sort['title']; ?>
                </label>
                <p class="description qw-desc"><?php print $sort['description']; ?></p>
                <?php
              }
            ?>
          </div>
        </div>

        <!-- add fields -->
        <div id="qw-display-add-fields" class="qw-hidden">
          <input class="add-handler-type" type="hidden" value="field">
          <p class="description">Select Fields to add to this query's output.</p>
          <div class="qw-checkboxes">
            <?php
              // loop through fields
              foreach($all_fields as $hook_key => $field)
              {
                ?>
                <label class="qw-field-checkbox">
                  <input type="checkbox"
                         value="<?php print $field['type']; ?>" />
                  <input class="qw-hander-hook_key"
                         type="hidden"
                         value="<?php print $field['hook_key']; ?>" />
                  <?php print $field['title']; ?>
                </label>
                <p class="description qw-desc"><?php print $field['description']; ?></p>
                <?php
              }
            ?>
          </div>
        </div>

        <!-- add filters -->
        <div id="qw-display-add-filters" class="qw-hidden">
          <input class="add-handler-type" type="hidden" value="filter">
          <p class="description">Select filters to affect the query's results.</p>
          <div class="qw-checkboxes">
            <?php
              // loop through filters
              foreach($all_filters as $hook_key => $filter)
              {
                // for now, this is how I'll prevent certain filters on overrides
                if(isset($filter['query_display_types']) && is_array($filter['query_display_types']) && in_array($query_type, $filter['query_display_types']))
                { ?>
                  <label class="qw-filter-checkbox">
                    <input type="checkbox"
                           value="<?php print $filter['type']; ?>" />
                  <input class="qw-hander-hook_key"
                         type="hidden"
                         value="<?php print $filter['hook_key']; ?>" />
                  <?php print $filter['title']; ?>
                  </label>
                  <p class="description qw-desc"><?php print $filter['description']; ?></p>
                  <?php
                }
              }
            ?>
          </div>
        </div>

<!-- Overrides -->
        <?php
          // override queries have different category and tag options
          if($query_type == "override")
          { ?>
            <div id="existing-overrides">
              <!-- override categories -->
              <div id="qw-override-categories" class="qw-item-form">
                <p>
                  Select which categories to override.
                </p>
                <div class="qw-checkboxes">
                  <?php
                    // List all categories as checkboxes
                    foreach($category_ids as $cat_id)
                    {
                      $cat_name = get_cat_name($cat_id);
                      $cat_checked = (isset($options['override']['cats'][$cat_id])) ? 'checked="checked"' : '';
                      ?>
                      <label class="qw-query-checkbox">
                        <input class="qw-js-title"
                               type="checkbox"
                               name="qw-query-options[override][cats][<?php print $cat_id; ?>]"
                               value="<?php print $cat_name; ?>"
                               <?php print $cat_checked; ?> />
                        <?php print $cat_name; ?>
                      </label>
                      <?php
                    }
                  ?>
                </div>
              </div>
              <!-- override tags -->
              <div id="qw-override-tags" class="qw-item-form">
                <p>
                  Select which tags to override.
                </p>
                <div class="qw-checkboxes">
                  <?php
                    foreach($tags as $tag)
                    {
                      $tag_checked = (isset($options['override']['tags'][$tag->term_id])) ? 'checked="checked"' : '';
                      ?>
                      <label class="qw-query-checkbox">
                        <input class="qw-js-title"
                               type="checkbox"
                               name="qw-query-options[override][tags][<?php print $tag->term_id; ?>]"
                               value="<?php print $tag->name; ?>"
                               <?php print $tag_checked; ?> />
                        <?php print $tag->name; ?>
                      </label>
                      <?php
                    }
                  ?>
                </div>
              </div>
            </div>
            <?php
          }
        ?>
    </div><!-- options forms -->

<!-- Preview -->
  <div id="query-preview" class="qw-query-option">
    <div id="query-preview-controls" class="query-preview-inactive">
      <label>
        <input id="live-preview"
               type="checkbox"
               checked="checked" />
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