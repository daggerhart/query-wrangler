<?php

  $query_id = $_GET['edit'];
  
  // get the query
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT name,type,data,path FROM ".$table_name." WHERE id = ".$query_id;
  $rows = $wpdb->get_results($sql);
  
  // data
  $qw_query_options = unserialize($rows[0]->data);
  $query_name = $rows[0]->name;
  $query_type = $rows[0]->type;
  
  // Page Queries
  if($query_type == 'page'){
    $query_page_path = $rows[0]->path;
    $query_page_title = $qw_query_options['display']['title'];
    $page_templates = get_page_templates();
  }
  
  // Get all extra post types
  $args = array('public'   => true, '_builtin' => false); 
  $output = 'names'; // names or objects, note names is the default
  $operator = 'and'; // 'and' or 'or'
  $post_types = get_post_types($args,$output,$operator);
  // Add standard types
  $post_types['post'] = 'post';
  $post_types['page'] = 'page';
  $post_types['revision'] = 'revision';
  ksort($post_types);
  
  // categories
  $category_ids = get_all_category_ids();
  
  // tags
  $tags = get_tags(array('hide_empty' => false));

  // image sizes
  $image_sizes = get_intermediate_image_sizes();
  // file styles
  $file_styles = qw_file_styles();
  
  // fields and styles
  $fields = qw_all_fields();
  $field_styles = qw_all_field_styles();
  
  // sort fields according to weight  
  if(is_array($qw_query_options['display']['field_settings']['fields'])){  
    uasort($qw_query_options['display']['field_settings']['fields'],'qw_cmp');
  }
  ?>
  <div class='wrap'>
    <h2>Edit query <em><?php print $query_name; ?></em></h2> 
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
        </div> <!-- /qw-query-args -->
        
        <div id="qw-query-filters" class="qw-query-admin-options">
          <h4>Filters</h4>
          <span class="qw-query-title" title="qw-categories">
            Categories: <span><?php print (is_array($qw_query_options['args']['cat'])) ? implode(",", $qw_query_options['args']['cat']): 'None'; ?></span>
          </span>
          <span class="qw-query-title" title="qw-tags">
            Tags: <span><?php print (is_array($qw_query_options['args']['tag'])) ? implode(",", $qw_query_options['args']['tag']): 'None'; ?></span>
          </span>
          <span class="qw-query-title" title="qw-post-types">
            Post Types: <span><?php print (is_array($qw_query_options['args']['post_types'])) ? implode(",", $qw_query_options['args']['post_types']) : 'None'; ?></span>
          </span>
          <span class="qw-query-title" title="qw-post-parent">
            Page Parent: <span><?php print ($qw_query_options['args']['post_parent']) ? $qw_query_options['args']['post_parent'] : 'None'; ?></span>
          </span>
          <span class="qw-query-title" title="qw-post-status">
            Post Status: <span><?php print $qw_query_options['args']['post_status']; ?></span>
          </span>
        </div> <!-- /filters -->
        
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
                { ?>
                    <div>
                      <span class="qw-query-fields-name" title="qw-field-<?php print $field_name; ?>">
                        <?php print str_replace("_", " ",$field_settings['type']); ?>
                      </span>
                    </div>
                  <?php
                }
              }
            ?>
          </div>
        </div><!-- /fields -->
        
        <?php
          if($query_type == 'page')
          { ?>
            <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
            <div id="qw-page-settings" class="qw-query-admin-options">
              <h4>Page Settings</h4>
              
              <span class="qw-query-title" title="qw-page-path">
                Page Path: <span><?php print ($query_page_path) ? $query_page_path : 'None'; ?></span>
              </span>
              <span class="qw-query-title" title="qw-page-template">
                Page Template: <span><?php print ($qw_query_options['display']['page']['template-name']) ? $qw_query_options['display']['page']['template-name'] : 'Default'; ?></span>
              </span>
              <span class="qw-query-title" title="qw-page-pager">
                Pager: <span><?php print ($qw_query_options['display']['page']['pager']['type']) ? $qw_query_options['display']['page']['pager']['type'] : 'Default'; ?></span>
              </span>
              
            </div>
            <?php
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
                  <select name="qw-query-options[display][page][pager][type]" class="qw-field-value">
                    <option value="default">Default</option>
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
            
            <!-- post status -->
            <div id="qw-post-status" class="qw-query-content qw-single-value">
              <p>Select the status of the posts to show.</p>
              <select name="qw-query-options[args][post_status]" class="qw-field-value">
                <option value="publish" <?php if($qw_query_options['args']['post_status'] == "published") { print 'selected="selected"';} ?>>Published</option>
                <option value="pending" <?php if($qw_query_options['args']['post_status'] == "pending") { print 'selected="selected"';} ?>>Pending</option>
                <option value="draft" <?php if($qw_query_options['args']['post_status'] == "draft") { print 'selected="selected"';} ?>>Draft</option>
                <option value="future" <?php if($qw_query_options['args']['post_status'] == "future") { print 'selected="selected"';} ?>>Future (Scheduled)</option>
                <option value="trash" <?php if($qw_query_options['args']['post_status'] == "trash") { print 'selected="selected"';} ?>>Trashed</option>
              </select>
            </div>
            
            <!-- page parents -->
            <div id="qw-post-parent" class="qw-query-content qw-single-value">
              <p>Use only with post type 'Page' to show results with the chosen parent ID.</p>
              <input class="qw-field-value" type="text" name="qw-query-options[args][post_parent]" value="<?php print $qw_query_options['args']['post_parent']; ?>" />
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
                  { ?>
                    <option value="<?php print $style_name; ?>" <?php if($qw_query_options['display']['field_settings']['style'] == $style_name) { print 'selected="selected"';} ?>><?php print str_replace("_"," ",$style_name); ?> </option>
                    <?php
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
              <strong>Order </strong><br />
              <select name="qw-query-options[args][order]" class="qw-field-value">
                <option value="ASC" <?php if($qw_query_options['args']['order'] == "ASC") { print 'selected="selected"';} ?>>Ascending</option>
                <option value="DESC" <?php if($qw_query_options['args']['order'] == "DESC") { print 'selected="selected"';} ?>>Descending</option>
              </select>
            </div>
            
    <!-- Checkboxes -->
    
            <!-- categories -->
            <div id="qw-categories" class="qw-query-content qw-checkbox-values">
              <p>Select which categories to pull posts from, and how to treat those categories.</p>
              <div class="qw-checkboxes">
                <?php
                  // List all categories as checkboxes
                  foreach($category_ids as $cat_id)
                  {
                    $cat_name = get_cat_name($cat_id);
                    $cat_checked = (isset($qw_query_options['args']['cat'][$cat_id])) ? 'checked="checked"' : '';
                    ?>
                    <label class="qw-query-checkbox"><input type="checkbox" name="qw-query-options[args][cat][<?php print $cat_id; ?>]" value="<?php print $cat_name; ?>" <?php print $cat_checked; ?> /><?php print $cat_name; ?></label> 
                    <?php
                  }
                ?>
              </div>
              <p><strong>Categories Options</strong> - show posts that are:</p>
              <select name="qw-query-options[args][cat_operator]" class="qw-field-value">
                <option value="cat" <?php if($qw_query_options['args']['cat_operator'] == "cat") {print 'selected="selected"';} ?>>Any category plus children categories</option>
                <option value="category__in" <?php if($qw_query_options['args']['cat_operator'] == "category__in") {print 'selected="selected"';} ?>>Any category without children categories</option>
                <option value="category__and" <?php if($qw_query_options['args']['cat_operator'] == "category__and") {print 'selected="selected"';} ?>>All categories selected</option>
                <option value="category__not_in" <?php if($qw_query_options['args']['cat_operator'] == "category__not_in") {print 'selected="selected"';} ?>>Not in the categories selected</option>
              </select>
            </div>
            
            <!-- tags -->
            <div id="qw-tags" class="qw-query-content qw-checkbox-values">
              <p>Select which tags to use.</p>
              <div class="qw-checkboxes">
                <?php
                  foreach($tags as $tag)
                  {
                    $tag_checked = (isset($qw_query_options['args']['tag'][$tag->term_id])) ? 'checked="checked"' : '';
                    ?>
                    <label class="qw-query-checkbox"><input type="checkbox" name="qw-query-options[args][tag][<?php print $tag->term_id; ?>]" value="<?php print $tag->name; ?>" <?php print $tag_checked; ?> /><?php print $tag->name; ?></label> 
                    <?php
                  }
                ?>
              </div>
              <p><strong>Tag Options</strong> - show posts that have:</p>
              <select name="qw-query-options[args][tag_operator]" class="qw-field-value">
                <option value="tag__in">Any of the selected tags</option>
                <option value="tag__and">All of the selected tags</option>
                <option value="tag__not_in">None of the selected tags</option>
              </select>
            </div>
            
            <!-- post types -->
            <div id="qw-post-types" class="qw-query-content qw-checkbox-values">
              <p>Select which post types should be shown.</p>
              <div class="qw-checkboxes">
                <?php
                  // loop through post types
                  foreach ($post_types  as $post_type )
                  {
                    $post_type_checked = (isset($qw_query_options['args']['post_types'][$post_type])) ? 'checked="checked"' : '';
                    ?>
                    <label class="qw-query-checkbox"><input type="checkbox" name="qw-query-options[args][post_types][<?php print $post_type; ?>]" value="<?php print $post_type; ?>" <?php print $post_type_checked; ?> /><?php print ucfirst($post_type); ?> </label>
                    <?php
                  }
                ?>
              </div>
            </div>
    
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
                    include QW_PLUGIN_DIR.'/forms/form.query-field.inc';
                  }
                }
              ?>
            <!-- /edit fields -->
            
            <!-- sort fields -->
            <div id="qw-sort-fields" class="qw-query-content qw-sort-fields-values">
              <strong>Fields</strong><br />
              <em>Click and Drag to sort</em><br />
              <ul id="qw-fields-sortable">
                <?php
                  if(is_array($qw_query_options['display']['field_settings']['fields']))
                  {
                    // loop through existing fields
                    foreach($qw_query_options['display']['field_settings']['fields'] as $field_name => $field_settings)
                    { ?>
                      <li class="qw-item qw-field-item">
                        <div class='sort-handle'></div>                      
                        <span class='qw-field-remove qw-button'>Remove this field</span>
                        <span class='qw-sort-field-name'><?php print $field_name; ?></span>
                        <input class='qw-field-weight' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][weight]' type='text' size='2' value='<?php print $field_settings['weight']; ?>' />
                        <span class="qw-field-title"><?php print str_replace("_", " ", $field_settings['type']); ?></span>
                      </li>
                      <?php
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
                // */
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
                // */
                wp_reset_postdata();
                $wp_query = $old_query;
              ?>
            </div>
          </div>
        </div>
        
      </div>
    </form>
  </div>
