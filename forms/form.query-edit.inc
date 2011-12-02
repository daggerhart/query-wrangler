<form id="qw-edit-query-form" action='admin.php?page=query-wrangler&action=update&edit=<?php print $query_id; ?>&noheader=true' method='post'>
  <div id="qw-query-action-buttons">
    <input class='qw-button' type="submit" name="save" value="Save" />
    <input type="hidden" name="query-id" value="<?php print $query_id; ?>" />
  </div>
  
  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
  
  <div id="qw-query-admin-options-wrap">
    <div id="qw-query-args" class="qw-query-admin-options">
      <h4>Basic settings</h4>
      
      <span class="qw-query-title" title="qw-page-title">
        Title: <span><?php print ($query_page_title) ? $query_page_title : 'None'; ?></span>
      </span>
      <span class="qw-query-title" title="qw-display-type">
        Style: <span><?php print $qw_query_options['display']['type']; ?></span>
      </span>
      <span class="qw-query-title" title="qw-display-full-settings">
        Full Settings: <span><?php print $qw_query_options['display']['full_settings']['style']; ?></span>
      </span>
      <span class="qw-query-title" title="qw-display-fields-settings">
        Field Settings: <span><?php print $qw_query_options['display']['field_settings']['style']; ?></span>
      </span>          
      <span class="qw-query-title" title="qw-posts-per-page">
        Items per page: <span><?php print $qw_query_options['args']['posts_per_page']; ?></span>
      </span>
      <span class="qw-query-title" title="qw-offset">
        Offset: <span><?php print $qw_query_options['args']['offset']; ?></span>
      </span>
      <span class="qw-query-title" title="qw-sort-options">
        Sort Options: <span><?php print $qw_query_options['args']['orderby']; ?> - <?php print $qw_query_options['args']['order']; ?></span>
      </span>
      <span class="qw-query-title" title="qw-query-header">
        Header: <span><?php print ($qw_query_options['display']['header']) ? 'In Use' : 'None'; ?></span>
      </span>
      <span class="qw-query-title" title="qw-query-footer">
        Footer: <span><?php print ($qw_query_options['display']['footer']) ? 'In Use' : 'None'; ?></span>
      </span> 
      <span class="qw-query-title" title="qw-query-empty">
        Empty Text: <span><?php print ($qw_query_options['display']['empty']) ? 'In Use' : 'None'; ?></span>
      </span>      
      <span class="qw-query-title" title="qw-wrapper-classes">
        CSS Classes: <span><?php print ($qw_query_options['display']['wrapper-classes']) ? $qw_query_options['display']['wrapper-classes'] : 'None'; ?></span>
      </span>          
    </div> <!-- /qw-query-args -->
    
    <div id="qw-query-filters" class="qw-query-admin-options">
      <h4>Filters</h4>
      
      <span class="qw-query-title" title="qw-post-types">
        Post Types: <span><?php print (is_array($qw_query_options['args']['post_types'])) ? implode(",", $qw_query_options['args']['post_types']) : 'None'; ?></span>
      </span>
      <span class="qw-query-title" title="qw-post-status">
        Post Status: <span><?php print $qw_query_options['args']['post_status']; ?></span>
      </span>
      <?php
        // override queries can't choose categories or tags or page parents
        if ($query_type != "override")
        { ?>
          <span class="qw-query-title" title="qw-categories">
            Categories: <span><?php print (is_array($qw_query_options['args']['cat'])) ? implode(",", $qw_query_options['args']['cat']): 'None'; ?></span>
          </span>
          <span class="qw-query-title" title="qw-tags">
            Tags: <span><?php print (is_array($qw_query_options['args']['tag'])) ? implode(",", $qw_query_options['args']['tag']): 'None'; ?></span>
          </span>
          <span class="qw-query-title" title="qw-post-parent">
            Page Parent: <span><?php print ($qw_query_options['args']['post_parent']) ? $qw_query_options['args']['post_parent'] : 'None'; ?></span>
          </span>
          <?php
        }
      ?>
    </div>
    <!-- /filters -->
    
    <div id="qw-query-fields" class="qw-query-admin-options">
      <h4>Fields</h4>
      <span class="qw-query-fields-title" title="qw-display-add-fields">
        Add Fields
      </span>
      <span class="qw-query-fields-title" title="qw-sort-fields">
        Rearrange Fields
      </span>
      <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
  
      <div id="qw-query-fields-list">
        <?php
          if(is_array($qw_query_options['display']['field_settings']['fields']))
          {
            // loop through and display
            foreach($qw_query_options['display']['field_settings']['fields'] as $field_name => $field_settings)
            {
              $field_type_data = $fields[$field_settings['type']];
              ?>
                <div>
                  <span class="qw-query-fields-name" title="qw-field-<?php print $field_name; ?>">
                    <?php print $field_type_data['label']; ?>
                  </span>
                </div>
              <?php
            }
          }
        ?>
      </div>
    </div><!-- /fields -->
    
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
              <span class="qw-query-title" title="qw-page-path">
                Page Path: <span><?php print ($query_page_path) ? $query_page_path : 'None'; ?></span>
              </span>
              <?php
            }
          ?>
          <span class="qw-query-title" title="qw-page-template">
            Page Template: <span><?php print ($qw_query_options['display']['page']['template-name']) ? $qw_query_options['display']['page']['template-name'] : 'Default'; ?></span>
          </span>
          
          <span class="qw-query-title" title="qw-page-pager">
            Pager: <span><?php print ($qw_query_options['display']['page']['pager']['type']) ? $qw_query_options['display']['page']['pager']['type'] : 'Default'; ?></span>
          </span>
        </div>
        <!-- /page settings -->
        
        <?php
          if($query_type == 'override')
          { ?>
            <!-- override settings -->
            <div id="qw-override-settings" class="qw-query-admin-options">
              <h4>Override Settings</h4>
              <span class="qw-query-title" title="qw-override-categories">
                Categories: <span><?php print (is_array($qw_query_options['override']['cats'])) ? implode(",", $qw_query_options['override']['cats']): 'None'; ?></span>
              </span>
              <span class="qw-query-title" title="qw-override-tags">
                Tags: <span><?php print (is_array($qw_query_options['override']['tags'])) ? implode(",", $qw_query_options['override']['tags']): 'None'; ?></span>
              </span>
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
  
        <!-- header -->
        <div id="qw-query-header" class="qw-query-content qw-header-footer-empty">
          <p>
            <em>The content placed here will appear above the resulting query.</em>
          </p>
          <textarea name="qw-query-options[display][header]" class="qw-field-textarea"><?php print htmlentities($qw_query_options['display']['header']); ?></textarea>
        </div>

        <!-- footer -->
        <div id="qw-query-footer" class="qw-query-content qw-header-footer-empty">
          <p>
            <em>The content placed here will appear below the resulting query.</em>
          </p>
          <textarea name="qw-query-options[display][footer]" class="qw-field-textarea"><?php print htmlentities($qw_query_options['display']['footer']); ?></textarea>
        </div>
        
        <!-- empty text -->
        <div id="qw-query-empty" class="qw-query-content qw-header-footer-empty">
          <p>
            <em>The content placed here will appear if the query has no results.</em>
          </p>
          <textarea name="qw-query-options[display][empty]" class="qw-field-textarea"><?php print htmlentities($qw_query_options['display']['empty']); ?></textarea>
        </div>
        
        <!-- page title -->
        <div id="qw-page-title" class="qw-query-content qw-single-value">
          <p>
            <em>This page's or widget's title.</em>
          </p>
          <input class="qw-field-value" size="60" type="text" name="qw-query-options[display][title]" value="<?php print $query_page_title; ?>" />
        </div>
        
        <!-- Wrapper classes -->
        <div id="qw-wrapper-classes" class="qw-query-content qw-single-value">
          <p>
            <em>The CSS class names will be added to the query. This enables you to use specific CSS code for each query. You may define multiples classes separated by spaces.</em>
          </p>
          <input class="qw-field-value" size="60" type="text" name="qw-query-options[display][wrapper-classes]" value="<?php print $qw_query_options['display']['wrapper-classes']; ?>" />
        </div>        
        
        <?php // pages only
          if($query_type == 'page')
          { ?>
            <!-- page path -->
            <div id="qw-page-path" class="qw-query-content qw-single-value">
              <p>
                <em>The path or permalink you want this page to use. Avoid using spaces and capitalization for best results.</em>
              </p>
              <input class="qw-field-value" size="60" type="text" name="qw-query-options[display][page][path]" value="<?php print $query_page_path; ?>" />
            </div>
            <?php
          }
          
          // use template & pager on pages and overrides
          if($query_type == "page" || $query_type == "override")
          { ?>
            <!-- page template -->
            <div id="qw-page-template" class="qw-query-content qw-single-value">
              <p>
                <em>Select which page template should wrap this query page.</em>
              </p>
              <select name="qw-query-options[display][page][template-file]" id="qw-page-template" class="qw-field-value">
                <option value="index.php">Default</option>
                <?php
                  foreach($page_templates as $name => $file)
                  { ?>
                    <option value="<?php print $file; ?>" <?php if($file == $qw_query_options['display']['page']['template-file']) { print 'selected="selected"'; } ?>><?php print $name; ?></option>
                    <?php
                  }
                ?>
              </select>
            </div>
            
            <!-- pager -->
            <div id="qw-page-pager" class="qw-query-content qw-single-value">
              <label class='qw-field-checkbox'>
                <?php
                  $use_pager = ($qw_query_options['display']['page']['pager']['active']) ? 'checked="checked"': '';
                ?>
                <input type='checkbox' name="qw-query-options[display][page][pager][active]" <?php print $use_pager;?> /> Use Pagination
              </label>
              <p>
                <em>Select which type of pager to use.</em>
              </p>
              <?php $pager_type = $qw_query_options['display']['page']['pager']['type']; ?>
              <select name="qw-query-options[display][page][pager][type]" class="qw-field-value">
                <option value="default" <?php print ($pager_type == 'default') ? 'selected="selected"' : ''; ?>>Default</option>
                <option value="simple" <?php print ($pager_type == 'simple') ? 'selected="selected"' : ''; ?>>Simple</option>
                <option value="numbers" <?php print ($pager_type == 'numbers') ? 'selected="selected"' : ''; ?>>Page Numbers</option>
              </select>
              <p>
                <em>Use the following options to change the Default Pager labels.</em>
              </p>
              <strong>Previous Page Label:</strong><br />
              <input type="text" name="qw-query-options[display][page][pager][previous]" value="<?php print $qw_query_options['display']['page']['pager']['previous']; ?>" />
              <br /><br />
              <strong>Next Page Label:</strong><br />
              <input type="text" name="qw-query-options[display][page][pager][next]" value="<?php print $qw_query_options['display']['page']['pager']['next']; ?>" />
            </div>
            <?php
          }
        ?>
        
        <!-- posts per page -->
        <div id="qw-posts-per-page" class="qw-query-content qw-single-value">
          <p>Number of post to show per page </p>
          <input class="qw-field-value" type="text" name="qw-query-options[args][posts_per_page]" value="<?php print $qw_query_options['args']['posts_per_page']; ?>" />
        </div>
        
        <!-- offset -->
        <div id="qw-offset" class="qw-query-content qw-single-value">
          <p>Number of post to displace or pass over. </p>
          <input class="qw-field-value" type="text" name="qw-query-options[args][offset]" value="<?php print $qw_query_options['args']['offset']; ?>" />
        </div>
        
        <!-- display type -->
        <div id="qw-display-type" class="qw-query-content qw-single-value">
          <select name="qw-query-options[display][type]" class="qw-field-value">
            <option value="full" <?php if($qw_query_options['display']['type'] == "full") { print 'selected="selected"';} ?>>Full Posts </option>
            <option value="fields" <?php if($qw_query_options['display']['type'] == "fields") { print 'selected="selected"';} ?>>Fields </option>
          </select>
        </div>
        
        <!-- display full settings -->
        <div id="qw-display-full-settings" class="qw-query-content qw-single-value">
          <select name="qw-query-options[display][full_settings][style]" class="qw-field-value">
            <option value="complete" <?php if($qw_query_options['display']['full_settings']['style'] == "complete") { print 'selected="selected"';} ?>>Complete</option>
            <option value="excerpt" <?php if($qw_query_options['display']['full_settings']['style'] == "excerpt") { print 'selected="selected"';} ?>>Excerpt</option>
          </select>
        </div>
        
        <!-- display fields settings -->
        <div id="qw-display-fields-settings" class="qw-query-content qw-single-value">
          <strong>Field Style</strong><br />
          <select name="qw-query-options[display][field_settings][style]" class="qw-field-value">
            <?php
              // loop through field styles
              foreach($field_styles as $style_name => $style)
              {
                if($style['type'] == 'fields')
                { ?>
                  <option value="<?php print $style_name; ?>" <?php if($qw_query_options['display']['field_settings']['style'] == $style_name) { print 'selected="selected"';} ?>><?php print $style['label']; ?> </option>
                  <?php
                }
              }
            ?>
          </select>
        </div>
        
<!-- Multiple Values-->
        <!-- sort options -->
        <div id="qw-sort-options" class="qw-query-content qw-multiple-values">
          <p>Select how to sort the queried posts.</p>
          <strong>Order by: </strong><br />
          <select name="qw-query-options[args][orderby]" class="qw-field-value">
            <option value="none" <?php if($qw_query_options['args']['orderby'] == "none") { print 'selected="selected"';} ?>>None</option>
            <option value="ID" <?php if($qw_query_options['args']['orderby'] == "ID") { print 'selected="selected"';} ?>>Post ID</option>
            <option value="author" <?php if($qw_query_options['args']['orderby'] == "author") { print 'selected="selected"';} ?>>Author</option>
            <option value="title" <?php if($qw_query_options['args']['orderby'] == "title") { print 'selected="selected"';} ?>>Title</option>
            <option value="date" <?php if($qw_query_options['args']['orderby'] == "date") { print 'selected="selected"';} ?>>Date</option>
            <option value="modified" <?php if($qw_query_options['args']['orderby'] == "modified") { print 'selected="selected"';} ?>>Modified</option>
            <option value="parent" <?php if($qw_query_options['args']['orderby'] == "parent") { print 'selected="selected"';} ?>>Parent</option>
            <option value="rand" <?php if($qw_query_options['args']['orderby'] == "rand") { print 'selected="selected"';} ?>>Random</option>
            <option value="comment_count" <?php if($qw_query_options['args']['orderby'] == "comment_count") { print 'selected="selected"';} ?>>Comment Count (Popularity)</option>
            <option value="menu_order" <?php if($qw_query_options['args']['orderby'] == "menu_order") { print 'selected="selected"';} ?>>Menu Order (for Page types)</option>
          </select> 
          <br /><br />
          <strong>Order</strong><br />
          <select name="qw-query-options[args][order]" class="qw-field-value">
            <option value="ASC" <?php if($qw_query_options['args']['order'] == "ASC") { print 'selected="selected"';} ?>>Ascending</option>
            <option value="DESC" <?php if($qw_query_options['args']['order'] == "DESC") { print 'selected="selected"';} ?>>Descending</option>
          </select>
        </div>
        
<!-- Checkboxes -->
        <?php
          // override queries have different category and tag options
          if($query_type == "override")
          { ?>
            <!-- override categories -->
            <div id="qw-override-categories" class="qw-query-content qw-checkbox-values">
              <p>Select which categories apply this override to.</p>
              <div class="qw-checkboxes">
                <?php
                  // List all categories as checkboxes
                  foreach($category_ids as $cat_id)
                  {
                    $cat_name = get_cat_name($cat_id);
                    $cat_checked = (isset($qw_query_options['override']['cats'][$cat_id])) ? 'checked="checked"' : '';
                    ?>
                    <label class="qw-query-checkbox"><input type="checkbox" name="qw-query-options[override][cats][<?php print $cat_id; ?>]" value="<?php print $cat_name; ?>" <?php print $cat_checked; ?> /><?php print $cat_name; ?></label> 
                    <?php
                  }
                ?>
              </div>
            </div>
            <!-- override tags -->
            <div id="qw-override-tags" class="qw-query-content qw-checkbox-values">
              <p>Select which tags to apply this override to.</p>
              <div class="qw-checkboxes">
                <?php
                  foreach($tags as $tag)
                  {
                    $tag_checked = (isset($qw_query_options['override']['tags'][$tag->term_id])) ? 'checked="checked"' : '';
                    ?>
                    <label class="qw-query-checkbox"><input type="checkbox" name="qw-query-options[override][tags][<?php print $tag->term_id; ?>]" value="<?php print $tag->name; ?>" <?php print $tag_checked; ?> /><?php print $tag->name; ?></label> 
                    <?php
                  }
                ?>
              </div>
            </div>
            <?php
          }
        ?>
<!-- Filters -->
        <?php
          $args = array(
            'filters' => $qw_query_options['args'],
            'query_type' => $query_type,
            'post_types' => $post_types,
            'category_ids' => $category_ids,
            'tags' => $tags,
          );
          print theme('query_filters', $args);
        ?>

<!-- Fields -->        
        <!-- add fields -->
        <div id="qw-display-add-fields" class="qw-query-content">
          <div class="qw-checkboxes">
            <?php
              // loop through fields
              foreach($fields as $field_name => $field)
              {
                $field_checked = (is_array($qw_query_options['display']['fields']) && in_array($field_name, $qw_query_options['display']['fields'])) ? 'checked="checked"' : '';
                ?>
                <label class="qw-field-checkbox"><input type="checkbox" value="<?php print $field_name; ?>" <?php print $field_checked; ?> /><?php print $field['label']; ?></label>
                <?php
              }
            ?>
          </div>
          <div id="qw-add-selected-fields" class="qw-button">Add Selected Fields</div>
        </div>
        
        <!-- edit fields -->
        <?php
          if(is_array($qw_query_options['display']['field_settings']['fields']))
          {
            // loop through existing fields
            foreach($qw_query_options['display']['field_settings']['fields'] as $field_name => $field_settings)
            {
              $args = array(
                'image_sizes' => $image_sizes,
                'file_styles' => $file_styles,
                'field_name' => $field_name,
                'field_settings' => $field_settings,   
              );
              print theme('query_field', $args);
            }
          }
        ?>
        <!-- /edit fields -->
        
        <!-- sortable fields -->
        <div id="qw-sort-fields" class="qw-query-content qw-sort-fields-values">
          <strong>Fields</strong><br />
          <em>Click and Drag to sort</em><br />
          <ul id="qw-fields-sortable">
            <?php
              if(is_array($qw_query_options['display']['field_settings']['fields']))
              {
                // loop through existing fields
                foreach($qw_query_options['display']['field_settings']['fields'] as $field_name => $field_settings)
                {
                  $args = array(
                    'field_name' => $field_name,
                    'weight' => $field_settings['weight'],
                    'type' => $field_settings['type'],
                  );
                  print theme('query_field_sortable', $args);
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
    <h4 id="preview-title">Preview Query</h4>
    <p><em>This preview does not include your theme's CSS stylesheet.</em></p>
    <div id="query-preview-target">
      <?php
        print qw_execute_query($query_id);
      ?>
    </div>
    <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
    
    <div id="qw-show-arguments">
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox' />Show WP_Query Arguments
          </label>
        </div>
        <div class="qw-options-group-content qw-field-options-hidden">
          <?php
            $new_query_args = qw_generate_query_args($qw_query_options);
            print "<pre>".print_r($new_query_args, true)."</pre>";
            // */
          ?>
        </div>
      </div>
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox' />Show Display Settings
          </label>
        </div>
        <div class="qw-options-group-content qw-field-options-hidden">
          <?php
            print "<pre>".print_r($qw_query_options['display'], true)."</pre>";
          ?>
        </div>
      </div>
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox' />Show Resulting WP_Query Object
          </label>
        </div>
        <div class="qw-options-group-content qw-field-options-hidden">
          <?php
            global $wp_query;
            $old_query = $wp_query;
            $new_query = new WP_Query($new_query_args);
            print "<pre>".print_r($new_query, true)."</pre>";
            wp_reset_postdata();
            $wp_query = $old_query;
          ?>
        </div>
      </div>
    </div>
    
  </div>
</form>
