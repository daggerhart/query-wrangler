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
      <span class="qw-query-title" title="qw-display-style">
        Style: <span><?php print ($qw_query_options['display']['style']) ? $qw_query_options['display']['style']: 'None'; ?></span>
      </span>          
      <span class="qw-query-title" title="qw-display-type">
        Row Style: <span><?php print $qw_query_options['display']['type']; ?></span>
      </span>
      <span class="qw-query-title" title="qw-display-full-settings">
        Posts Settings: <span><?php print $qw_query_options['display']['full_settings']['style']; ?></span>
      </span>
      <!-- TODO
      <span class="qw-query-title" title="qw-display-fields-settings">
        Field Settings: <span><?php print $qw_query_options['display']['field_settings']['style']; ?></span>
      </span>
      -->       
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
      <span class="qw-query-filters-title" title="qw-display-add-filters">
        Add Filters
      </span>
      <span class="qw-query-filters-title" title="qw-sort-filters">
        Rearrange Filters
      </span>
      <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
  
      <div id="qw-query-filters-list">
        <?php
          if(is_array($qw_query_options['args']['filters']))
          {
            // loop through and display
            foreach($qw_query_options['args']['filters'] as $filter_name => $filter_settings)
            {
              $filter_type_data = $filters[$filter_settings['type']];
              ?>
                <div>
                  <span class="qw-query-filters-name" title="qw-filter-<?php print $filter_name; ?>">
                    <?php print str_replace("_", " ", $filter_name); ?>
                  </span>
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
                    <?php print str_replace("_", " ", $field_name);; ?>
                  </span>
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
        
        <!-- posts per page -->
        <div id="qw-posts-per-page" class="qw-query-content qw-single-value">
          <p>
            Number of posts to show per page.
          </p>
          <input class="qw-field-value" type="text" name="qw-query-options[args][posts_per_page]" value="<?php print $qw_query_options['args']['posts_per_page']; ?>" />
        </div>
        
        <!-- offset -->
        <div id="qw-offset" class="qw-query-content qw-single-value">
          <p>
            Number of post to skip, or pass over. For example, if this field is 3, the first 3 items will be skipped and not displayed.
          </p>
          <input class="qw-field-value" type="text" name="qw-query-options[args][offset]" value="<?php print $qw_query_options['args']['offset']; ?>" />
        </div>
        
        <!-- Style -->
        <div id="qw-display-style" class="qw-query-content qw-single-value">
          <p>
            How should this query be styled?
          </p>
          <select name="qw-query-options[display][style]" class="qw-field-value">
            <?php
              // loop through field styles
              foreach($styles as $style_name => $style)
              { ?>
                  <option value="<?php print $style_name; ?>" <?php if($qw_query_options['display']['style'] == $style_name) { print 'selected="selected"';} ?>><?php print $style['label']; ?> </option>
                <?php
              }
            ?>
          </select>
        </div>
        
        <!-- Row Style: display type -->
        <div id="qw-display-type" class="qw-query-content qw-single-value">
          <p>
            How should each post in this query be styled?
          </p>
          <select name="qw-query-options[display][type]" class="qw-field-value">
            <?php
              foreach($row_styles as $row_style_name => $row_style)
              { ?>
                  <option value="<?php print $row_style_name; ?>" <?php if($qw_query_options['display']['type'] == $row_style_name) { print 'selected="selected"';} ?>><?php print $row_style['label']; ?> </option>
                <?php
              }
            ?>
          </select>
        </div>
        
        <!-- display full settings -->
        <div id="qw-display-full-settings" class="qw-query-content qw-single-value">
          <p>
            Select the amount of the post to be shown.
          </p>
          <select name="qw-query-options[display][full_settings][style]" class="qw-field-value">
            <option value="complete" <?php if($qw_query_options['display']['full_settings']['style'] == "complete") { print 'selected="selected"';} ?>>Complete Post</option>
            <option value="excerpt" <?php if($qw_query_options['display']['full_settings']['style'] == "excerpt") { print 'selected="selected"';} ?>>Excerpt</option>
          </select>
        </div>
        
        <!-- TODO: display fields settings -->
        <div id="qw-display-fields-settings-TODO" class="qw-query-content qw-single-value">
          <!-- to be done -->
        </div>
        
        <!-- header -->
        <div id="qw-query-header" class="qw-query-content qw-header-footer-empty">
          <p>
            The content placed here will appear above the resulting query.
          </p>
          <textarea name="qw-query-options[display][header]" class="qw-field-textarea"><?php print htmlentities($qw_query_options['display']['header']); ?></textarea>
        </div>

        <!-- footer -->
        <div id="qw-query-footer" class="qw-query-content qw-header-footer-empty">
          <p>
            The content placed here will appear below the resulting query.
          </p>
          <textarea name="qw-query-options[display][footer]" class="qw-field-textarea"><?php print htmlentities($qw_query_options['display']['footer']); ?></textarea>
        </div>
        
        <!-- empty text -->
        <div id="qw-query-empty" class="qw-query-content qw-header-footer-empty">
          <p>
            The content placed here will appear if the query has no results.
          </p>
          <textarea name="qw-query-options[display][empty]" class="qw-field-textarea"><?php print htmlentities($qw_query_options['display']['empty']); ?></textarea>
        </div>
        
        <!-- Wrapper classes -->
        <div id="qw-wrapper-classes" class="qw-query-content qw-single-value">
          <p>
            The CSS class names will be added to the query. This enables you to use specific CSS code for each query. You may define multiples classes separated by spaces.
          </p>
          <input class="qw-field-value" size="60" type="text" name="qw-query-options[display][wrapper-classes]" value="<?php print $qw_query_options['display']['wrapper-classes']; ?>" />
        </div>
        
        <!-- page title -->
        <div id="qw-page-title" class="qw-query-content qw-single-value">
          <p>
            This page's or widget's title.
          </p>
          <input class="qw-field-value" size="60" type="text" name="qw-query-options[display][title]" value="<?php print $query_page_title; ?>" />
        </div>
        
        <?php // pages only
          if($query_type == 'page')
          { ?>
            <!-- page path -->
            <div id="qw-page-path" class="qw-query-content qw-single-value">
              <p>
                The path or permalink you want this page to use. Avoid using spaces and capitalization for best results.
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
                Select which page template should wrap this query page.
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
              <p>
                Select which type of pager to use.
              </p>
              <label class='qw-field-checkbox'>
                <?php
                  $use_pager = ($qw_query_options['display']['page']['pager']['active']) ? 'checked="checked"': '';
                ?>
                <input type='checkbox' name="qw-query-options[display][page][pager][active]" <?php print $use_pager;?> /> Use Pagination
              </label>
              
              <select name="qw-query-options[display][page][pager][type]" class="qw-field-value">
                <?php
                  foreach($pager_types as $pager_name => $pager_options)
                  {
                    $selected = ($qw_query_options['display']['page']['pager']['type'] == $pager_name) ? 'selected="selected"' : '';
                    ?>
                    <option value="<?php echo $pager_name; ?>" <?php echo $selected; ?>><?php echo $pager_options['label']; ?></option> 	
                    <?php 
                  }
                ?>
              </select>
              <p>
                Use the following options to change the Default Pager labels.
              </p>
              <strong>Previous Page Label:</strong>
              <p>
                <input type="text" name="qw-query-options[display][page][pager][previous]" value="<?php print $qw_query_options['display']['page']['pager']['previous']; ?>" />
              </p>
              <strong>Next Page Label:</strong>
              <p>
                <input type="text" name="qw-query-options[display][page][pager][next]" value="<?php print $qw_query_options['display']['page']['pager']['next']; ?>" />
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
          <p>
            <strong>Order:</strong>
          </p>
          <p>
            Select if you want the sorting to be Ascending or Descending.
          </p>
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
              <p>
                Select which categories to override.
              </p>
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
              <p>
                Select which tags to override.
              </p>
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
      <!-- edit Filters -->
        <?php
          if(is_array($qw_query_options['args']['filters']))
          {
            // loop through existing filters
            foreach($qw_query_options['args']['filters'] as $filter_name => $filter_settings)
            {
              $filter = $filters[$filter_settings['type']];
              $args = array(
                'filter_name' => $filter_name,
                'filter_type' => $filter['type'],
                'filters' => $qw_query_options['args']['filters'],
                'filter_settings' => $filter_settings,   
                'query_type' => $query_type,
                'post_types' => qw_all_post_types(),
                'category_ids' => get_all_category_ids(),
                'tags' => get_tags(array('hide_empty' => false)),
              );
              print theme('query_filter', $args);
            }
          }
        ?>
      <!-- /edit filters -->  
        
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
                'qw_query_options' => $qw_query_options,
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
              foreach($fields as $field_name => $field)
              { ?>
                <label class="qw-field-checkbox"><input type="checkbox" value="<?php print $field_name; ?>" <?php print $field_checked; ?> /><?php print $field['label']; ?></label>
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

<!-- Filters -->
        <!-- add filters -->
        <div id="qw-display-add-filters" class="qw-query-content">
          <div class="qw-checkboxes">
            <?php
              // loop through filters
              foreach($filters as $filter_name => $filter)
              {
                // for now, this is how I'll prevent certain filters on overrides
                if($query_type == 'override' &&
                   ($filter_name == 'categories' ||
                    $filter_name == 'tags' ||
                    $filter_name == 'post_parent')
                  )
                {
                  $hide = true;
                } else {
                  $hide = false;
                }
                
                if(!$hide)
                {  ?>
                  <label class="qw-filter-checkbox"><input type="checkbox" value="<?php print $filter_name; ?>" <?php print $filter_checked; ?> /><?php print $filter['label']; ?></label>
                  <?php
                }
              }
            ?>
          </div>
          <div class="add-selected-wrapper">
            <div id="qw-add-selected-filters" class="qw-button">Add Selected filters</div>
          </div>
        </div>
        
        <!-- sortable filters -->
        <div id="qw-sort-filters" class="qw-query-content qw-sort-filters-values">
          <p>
            Click and Drag to sort fitlers.
          </p>
          <ul id="qw-filters-sortable">
            <?php
              if(is_array($qw_query_options['args']['filters']))
              {
                // loop through existing filters
                foreach($qw_query_options['args']['filters'] as $filter_name => $filter_settings)
                {
                  $args = array(
                    'filter_name' => $filter_name,
                    'filter' => $filter_settings,
                    'weight' => $filter_settings['weight'],
                    'type' => $filter_settings['type'],
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
