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
$display = array_map( 'stripslashes_deep', $display );
?>
<form id="qw-edit-query-form" action='admin.php?page=query-wrangler&action=update&edit=<?php print $query_id; ?>&noheader=true' method='post'>
  <div id="qw-query-action-buttons">
    <input class='qw-button' type="submit" name="save" value="Save" />
    <input type="hidden" name="query-id" value="<?php print $query_id; ?>" />
    <div id="query-actions">
      <a href="admin.php?page=query-wrangler&export=<?php print $query_id; ?>">Export</a>
    </div>
  </div>
  <div id="message" class="updated qw-changes">
    <p><strong>*</strong> Changes have been made that need to be saved.</p>
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

  <div id="qw-query-admin-options-wrap">
    <div id="qw-query-args" class="qw-query-admin-options">
      <h4>Basic settings</h4>

      <div class="qw-query-title" title="qw-page-title">
        Title:
        <span class="qw-setting-value">
          <?php print ($query_page_title) ? $query_page_title : 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-display-style">
        Template Style:
        <span class="qw-setting-value">
          <?php print ($styles[$display['style']]['title']) ? $styles[$display['style']]['title']: 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-display-row-style">
        Row Style:
        <span class="qw-setting-value">
          <?php print ($row_styles[$display['row_style']]['title']) ? $row_styles[$display['row_style']]['title']: 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-display-post-settings">
        Posts Settings:
        <span class="qw-setting-value">
          <?php print ($row_complete_styles[$display['post_settings']['size']]['title']) ? $row_complete_styles[$display['post_settings']['size']]['title'] : 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-post-status">
        Post Status:
        <span class="qw-setting-value">
          <?php print $post_statuses[$options['args']['post_status']]['title']; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-posts-per-page">
        Posts per page:
        <span class="qw-setting-value">
          <?php print $args['posts_per_page']; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-offset">
        Offset:
        <span class="qw-setting-value">
          <?php print $args['offset']; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-sort-options">
        Sort Options:
        <span class="qw-setting-value">
          <?php print ($sort_options[$args['orderby']]['title']) ? $sort_options[$args['orderby']]['title'] : 'None'; ?>,
          <?php print ($sort_orders[$args['order']]['title']) ? $sort_orders[$args['order']]['title'] : 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-query-header">
        Header:
        <span class="qw-setting-value">
          <?php print ($display['header']) ? 'In Use' : 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-query-footer">
        Footer:
        <span class="qw-setting-value">
          <?php print ($display['footer']) ? 'In Use' : 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-query-empty">
        Empty Text:
        <span class="qw-setting-value">
          <?php print ($display['empty']) ? 'In Use' : 'None'; ?>
        </span>
      </div>

      <div class="qw-query-title" title="qw-wrapper-classes">
        CSS Classes:
        <span class="qw-setting-value">
          <?php print ($display['wrapper-classes']) ? $display['wrapper-classes'] : 'None'; ?>
        </span>
      </div>
    </div> <!-- /qw-query-args -->

    <div id="qw-query-filters" class="qw-query-admin-options">
      <h4>Filters</h4>
      <div class="qw-query-add-titles">
        <span class="qw-query-title" title="qw-display-add-filters">
          Add Filters
        </span>
        <span class="qw-query-title" title="qw-sort-filters">
          Rearrange Filters
        </span>
      </div>
      <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

      <div id="qw-query-filters-list">
        <?php
          if(is_array($filters))
          {
            // loop through and display
            foreach($filters as $filter_name => $filter)
            {
              $values = array();
              foreach($filter['values'] as $value){
                if (is_array($value)){
                  $values[] = implode(",", $value);
                } else {
                  $values[] = $value;
                }
              }
              $values = implode(' | ', $values);
              ?>
                <div class="qw-query-title" title="qw-filter-<?php print $filter_name; ?>">
                  <span class="qw-setting-title"><?php print $filter['title']; ?></span>
                  :
                  <span class="qw-setting-value"><?php print $values; ?></span>
                </div>
              <?php
            }
          }
        ?>
      </div>
    </div>
    <!-- /filters -->

    <div id="qw-query-fields" class="qw-query-admin-options">
      <h4>Fields</h4>
      <div class="qw-query-add-titles">
        <span class="qw-query-title" title="qw-display-add-fields">
          Add Fields
        </span>
        <span class="qw-query-title" title="qw-sort-fields">
          Rearrange Fields
        </span>
      </div>
      <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

      <div id="qw-query-fields-list">
        <?php
          if(is_array($fields))
          {
            // loop through and display
            foreach($fields as $field)
            { ?>
                <div class="qw-query-title" title="qw-field-<?php print $field['name']; ?>">
                  <span class="qw-setting-title"><?php print $field['title'];  ?></span>
                  :
                  <span class="qw-setting-value"><?php print $field['name']; ?></span>
                </div>
              <?php
            }
          }
        ?>
      </div>
    </div>
    <!-- /fields -->

    <?php
      // use pager on pages and overrides
      if($query_type == 'page' || $query_type == 'override')
      { ?>
        <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
        <div id="qw-page-settings" class="qw-query-admin-options">
          <h4>Page Settings</h4>

          <?php
            // path for pages only
            if($query_type == 'page')
            { ?>
              <div class="qw-query-title" title="qw-page-path">
                Page Path:
                <span class="qw-setting-value">
                  <?php print ($query_page_path) ? $query_page_path : 'None'; ?>
                </span>
              </div>
              <?php
            }
          ?>
          <div class="qw-query-title" title="qw-page-template">
            Page Template:
            <span class="qw-setting-value">
              <?php print ($display['page']['template-name']) ? $display['page']['template-name'] : 'Default'; ?>
            </span>
          </div>

          <div class="qw-query-title" title="qw-page-pager">
            Pager:
            <span class="qw-setting-value">
              <?php print ($pager_types[$display['page']['pager']['type']]['title']) ? $pager_types[$display['page']['pager']['type']]['title'] : 'Default'; ?>
            </span>
          </div>
        </div>
        <!-- /page settings -->

        <?php
          if($query_type == 'override')
          { ?>
            <!-- override settings -->
            <div id="qw-override-settings" class="qw-query-admin-options">
              <h4>Override Settings</h4>

              <div class="qw-query-title" title="qw-override-categories">
                Categories:
                <span class="qw-setting-value">
                  <?php print (is_array($options['override']['cats'])) ? implode(",", $options['override']['cats']): 'None'; ?>
                </span>
              </div>

              <div class="qw-query-title" title="qw-override-tags">
                Tags:
                <span class="qw-setting-value">
                  <?php print (is_array($options['override']['tags'])) ? implode(",", $options['override']['tags']): 'None'; ?>
                </span>
              </div>
            </div>
            <!-- /override settings -->
            <?php
          }
      }
    ?>
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

<!-- ------- Update Area --------- -->
  <div id="qw-options-target">
    <h4 id="qw-options-target-title">&nbsp;</h4>
    <div id="qw-options-form-target">

<!-- ------- FORMS --------- -->
      <div id="qw-options-forms">

<!-- Single Values -->

        <!-- posts per page -->
        <div id="qw-posts-per-page" class="qw-query-content">
          <p>
            Number of posts to show per page.
          </p>
          <input type="text"
                 name="qw-query-options[args][posts_per_page]"
                 value="<?php print $args['posts_per_page']; ?>" />
        </div>

        <div id="qw-post-status" class="qw-query-content">
          <p>
            Select the status of the posts to show.
            <?php print $options['args']['post_status']; ?>
          </p>
          <select name="qw-query-options[args][post_status]" class="qw-field-value">
            <?php
              foreach($post_statuses as $type => $post_status)
              { ?>
                <option value="<?php print $type; ?>"
                        <?php if($options['args']['post_status'] == $type) { print 'selected="selected"';} ?>>
                  <?php print $post_status['title']; ?>
                </option>
                <?php
              }
            ?>
           </select>
        </div>

        <!-- offset -->
        <div id="qw-offset" class="qw-query-content">
          <p>
            Number of post to skip, or pass over. For example, if this field is 3, the first 3 items will be skipped and not displayed.
          </p>
          <input type="text"
                 name="qw-query-options[args][offset]"
                 value="<?php print $args['offset']; ?>" />
        </div>

        <!-- Style -->
        <div id="qw-display-style" class="qw-query-content">
          <p>
            How should this query be styled?
          </p>
          <select name="qw-query-options[display][style]">
            <?php
              // loop through field styles
              foreach($styles as $type => $style)
              { ?>
                  <option value="<?php print $type; ?>"
                          <?php if($display['style'] == $type) { print 'selected="selected"';} ?>>
                    <?php print $style['title']; ?>
                  </option>
                <?php
              }
            ?>
          </select>
        </div>

        <!-- Row Style: display row style -->
        <div id="qw-display-row-style" class="qw-query-content">
          <p>
            How should each post in this query be presented?
          </p>
          <select id="query-display-type"
                  name="qw-query-options[display][row_style]">
            <?php
              foreach($row_styles as $type => $row_style)
              { ?>
                  <option value="<?php print $type; ?>"
                          <?php if($display['row_style'] == $type) { print 'selected="selected"';} ?>>
                    <?php print $row_style['title']; ?>
                  </option>
                <?php
              }
            ?>
          </select>
        </div>

        <!-- display post settings -->
        <div id="qw-display-post-settings" class="qw-query-content">
          <p>
            Select the amount of the post to be shown.
          </p>
          <select name="qw-query-options[display][post_settings][size]">
            <option value="complete"
                    <?php if($display['post_settings']['size'] == "complete") { print 'selected="selected"';} ?>>
              Complete Post
            </option>
            <option value="excerpt"
                    <?php if($display['post_settings']['size'] == "excerpt") { print 'selected="selected"';} ?>>
              Excerpt
            </option>
          </select>
        </div>

        <!-- TODO: display fields settings -->
        <div id="qw-display-fields-settings-TODO" class="qw-query-content">
          <!-- to be done -->
        </div>

        <!-- header -->
        <div id="qw-query-header" class="qw-query-content qw-header-footer-empty">
          <p>
            The content placed here will appear above the resulting query.
          </p>
          <textarea name="qw-query-options[display][header]"
                    class="qw-field-textarea"><?php print qw_textarea($display['header']); ?></textarea>
        </div>

        <!-- footer -->
        <div id="qw-query-footer" class="qw-query-content qw-header-footer-empty">
          <p>
            The content placed here will appear below the resulting query.
          </p>
          <textarea name="qw-query-options[display][footer]"
                    class="qw-field-textarea"><?php print qw_textarea($display['footer']); ?></textarea>
        </div>

        <!-- empty text -->
        <div id="qw-query-empty" class="qw-query-content qw-header-footer-empty">
          <p>
            The content placed here will appear if the query has no results.
          </p>
          <textarea name="qw-query-options[display][empty]"
                    class="qw-field-textarea"><?php print qw_textarea($display['empty']); ?></textarea>
        </div>

        <!-- Wrapper classes -->
        <div id="qw-wrapper-classes" class="qw-query-content">
          <p>
            The CSS class names will be added to the query. This enables you to use specific CSS code for each query. You may define multiples classes separated by spaces.
          </p>
          <input size="60"
                 type="text"
                 name="qw-query-options[display][wrapper-classes]"
                 value="<?php print $display['wrapper-classes']; ?>" />
        </div>

        <!-- page title -->
        <div id="qw-page-title" class="qw-query-content">
          <p>
            This page's or widget's title.
          </p>
          <input size="60"
                 type="text"
                 name="qw-query-options[display][title]"
                 value="<?php print $query_page_title; ?>" />
        </div>

        <?php // pages only
          if($query_type == 'page')
          { ?>
            <!-- page path -->
            <div id="qw-page-path" class="qw-query-content">
              <p>
                The path or permalink you want this page to use. Avoid using spaces and capitalization for best results.
              </p>
              <input size="60"
                     type="text"
                     name="qw-query-options[display][page][path]"
                     value="<?php print $query_page_path; ?>" />
            </div>
            <?php
          }

          // use template & pager on pages and overrides
          if($query_type == "page" || $query_type == "override")
          { ?>
            <!-- page template -->
            <div id="qw-page-template" class="qw-query-content">
              <p>
                Select which page template should wrap this query page.
              </p>
              <select name="qw-query-options[display][page][template-file]" id="qw-page-template">
                <option value="index.php">Default</option>
                <?php
                  foreach($page_templates as $name => $file)
                  { ?>
                    <option value="<?php print $file; ?>"
                            <?php if($file == $display['page']['template-file']) { print 'selected="selected"'; } ?>>
                      <?php print $name; ?>
                    </option>
                    <?php
                  }
                ?>
              </select>
            </div>

            <!-- pager -->
            <div id="qw-page-pager" class="qw-query-content">
              <p>
                Select which type of pager to use.
              </p>
              <label class='qw-field-checkbox'>
                <?php
                  $use_pager = ($display['page']['pager']['active']) ? 'checked="checked"': '';
                ?>
                <input type='checkbox'
                       name="qw-query-options[display][page][pager][active]"
                       <?php print $use_pager;?> />
                Use Pagination
              </label>

              <select name="qw-query-options[display][page][pager][type]">
                <?php
                  foreach($pager_types as $pager_name => $pager_options)
                  {
                    $selected = ($display['page']['pager']['type'] == $pager_name) ? 'selected="selected"' : '';
                    ?>
                    <option value="<?php echo $pager_name; ?>"
                            <?php echo $selected; ?>>
                      <?php echo $pager_options['title']; ?>
                    </option>
                    <?php
                  }
                ?>
              </select>
              <p>
                Use the following options to change the Default Pager labels.
              </p>
              <strong>Previous Page Label:</strong>
              <p>
                <input type="text"
                       name="qw-query-options[display][page][pager][previous]"
                       value="<?php print $display['page']['pager']['previous']; ?>" />
              </p>
              <strong>Next Page Label:</strong>
              <p>
                <input type="text"
                       name="qw-query-options[display][page][pager][next]"
                       value="<?php print $display['page']['pager']['next']; ?>" />
              </p>
            </div>
            <?php
          }
        ?>

<!-- Multiple Values-->
        <!-- sort options -->
        <div id="qw-sort-options" class="qw-query-content qw-multiple-values">
          <p>
            <strong>Order by: </strong>
          </p>
          <p>
            Select how to sort the queried posts.
          </p>
          <select name="qw-query-options[args][orderby]">
            <?php
              foreach($sort_options as $type => $sort_option)
              { ?>
                <option value="<?php print $type; ?>" <?php if($args['orderby'] == $type) { print 'selected="selected"';} ?>><?php print $sort_option['title']; ?></option>
                <?php
              }
            ?>
          </select>
          <p>
            <strong>Order:</strong>
          </p>
          <p>
            Select if you want the sorting to be Ascending or Descending.
          </p>
          <select name="qw-query-options[args][order]">
            <option value="ASC"
                    <?php if($args['order'] == "ASC") { print 'selected="selected"';} ?>>
              Ascending
            </option>
            <option value="DESC"
                    <?php if($args['order'] == "DESC") { print 'selected="selected"';} ?>>
              Descending
            </option>
          </select>
        </div>

<!-- Checkboxes -->
        <?php
          // override queries have different category and tag options
          if($query_type == "override")
          { ?>
            <!-- override categories -->
            <div id="qw-override-categories" class="qw-query-content">
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
                      <input type="checkbox"
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
            <div id="qw-override-tags" class="qw-query-content">
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
                      <input type="checkbox"
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
            <?php
          }
        ?>
      <!-- edit Filters -->
        <?php
          if(is_array($filters))
          {
            // loop through existing filters
            foreach($filters as $filter_name => $filter)
            {
              $args = array(
                'filter' => $filter,
              );
              print theme('query_filter', $args);
            }
          }
        ?>
      <!-- /edit filters -->

      <!-- edit fields -->
        <?php
          if(is_array($fields))
          {
            $tokens = array();
            // loop through existing fields
            foreach($fields as $field)
            {
              $tokens[$field['name']] = '{{'.$field['name'].'}}';
              $args = array(
                'image_sizes' => $image_sizes,
                'file_styles' => $file_styles,
                'field' => $field,
                'options' => $options,
                'display' => $display,
                'args'  => $args,
                'tokens' => $tokens,
              );
              print theme('query_field', $args);
            }
          }
        ?>
        <!-- /edit fields -->

<!-- Fields -->
        <!-- add fields -->
        <div id="qw-display-add-fields" class="qw-query-content">
          <div class="qw-checkboxes">
            <?php
              // loop through fields
              foreach($all_fields as $field_name => $field)
              {
                ?>
                <label class="qw-field-checkbox">
                  <input type="checkbox"
                         value="<?php print $field_name; ?>"
                         <?php print $field_checked; ?> />
                  <?php print $field['title']; ?>
                </label>
                <?php
              }
            ?>
          </div>
          <div class="add-selected-wrapper">
            <div id="qw-add-selected-fields" class="qw-button">Add Selected Fields</div>
          </div>
        </div>

        <!-- sortable fields -->
        <div id="qw-sort-fields" class="qw-query-content qw-sort-fields-values">
          <p>
            Click and Drag to sort
          </p>
          <ul id="qw-fields-sortable">
            <?php
              if(is_array($fields))
              {
                // loop through existing fields
                foreach($fields as $field_name => $field)
                {
                  $args = array(
                    'field' => $field,
                    'weight' => $field['weight'],
                  );
                  print theme('query_field_sortable', $args);
                }
              }
            ?>
          </ul>
        </div>

<!-- Filters -->
        <!-- add filters -->
        <div id="qw-display-add-filters" class="qw-query-content">
          <div class="qw-checkboxes">
            <?php
              // loop through filters
              foreach($all_filters as $filter_name => $filter)
              {
                // for now, this is how I'll prevent certain filters on overrides
                if(in_array($query_type, $filter['query_display_types']))
                { ?>
                  <label class="qw-filter-checkbox">
                    <input type="checkbox"
                           value="<?php print $filter_name; ?>"
                           <?php print $filter_checked; ?> />
                    <?php print $filter['title']; ?>
                  </label>
                  <?php
                }
              }
            ?>
          </div>
          <div class="add-selected-wrapper">
            <div id="qw-add-selected-filters" class="qw-button">Add Selected Filters</div>
          </div>
        </div>

        <!-- sortable filters -->
        <div id="qw-sort-filters" class="qw-query-content qw-sort-filters-values">
          <p>
            Click and Drag to sort fitlers.
          </p>
          <ul id="qw-filters-sortable">
            <?php
              if(is_array($filters))
              {
                // loop through existing filters
                foreach($filters as $filter_name => $filter)
                {
                  $args = array(
                    'filter' => $filter,
                    'weight' => $filter['weight'],
                  );
                  print theme('query_filter_sortable', $args);
                }
              }
            ?>
          </ul>
        </div>

        <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

    </div><!-- options forms -->
  </div><!-- options form target -->
    <div id="qw-options-actions">
      <div id="qw-options-actions-update" class="qw-button">Update</div>
      <div id="qw-options-actions-cancel" class="qw-button">Cancel</div>
    </div>
    <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
  </div><!-- /qw-options-target-->

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
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox' />Show WP_Query Arguments
          </label>
        </div>
        <div id="qw-show-arguments-target" class="qw-options-group-content qw-field-options-hidden">
          <!-- args -->
        </div>
      </div>
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox' />Show Display Settings
          </label>
        </div>
        <div id="qw-show-display-target" class="qw-options-group-content qw-field-options-hidden">
          <!-- display -->
        </div>
      </div>
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox' />Show Resulting WP_Query Object
          </label>
        </div>
        <div id="qw-show-wpquery-target" class="qw-options-group-content qw-field-options-hidden">
          <!-- WP_Query -->
        </div>
      </div>
    </div>

  </div>
</form>
