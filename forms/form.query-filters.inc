  <!-- post types -->
  <div id="qw-post-types" class="qw-query-content qw-checkbox-values">
    <p>Select which post types should be shown.</p>
    <div class="qw-checkboxes">
      <?php
        // loop through post types
        foreach ($post_types  as $post_type )
        {
          $post_type_checked = (isset($filters['post_types'][$post_type])) ? 'checked="checked"' : '';
          ?>
          <label class="qw-query-checkbox"><input type="checkbox" name="qw-query-options[args][post_types][<?php print $post_type; ?>]" value="<?php print $post_type; ?>" <?php print $post_type_checked; ?> /><?php print ucfirst($post_type); ?> </label>
          <?php
        }
      ?>
    </div>
  </div>
  
  <!-- post status -->
  <div id="qw-post-status" class="qw-query-content qw-single-value">
    <p>Select the status of the posts to show.</p>
    <select name="qw-query-options[args][post_status]" class="qw-field-value">
      <option value="publish" <?php if($filters['post_status'] == "published") { print 'selected="selected"';} ?>>Published</option>
      <option value="pending" <?php if($filters['post_status'] == "pending") { print 'selected="selected"';} ?>>Pending</option>
      <option value="draft" <?php if($filters['post_status'] == "draft") { print 'selected="selected"';} ?>>Draft</option>
      <option value="future" <?php if($filters['post_status'] == "future") { print 'selected="selected"';} ?>>Future (Scheduled)</option>
      <option value="trash" <?php if($filters['post_status'] == "trash") { print 'selected="selected"';} ?>>Trashed</option>
    </select>
  </div>
  
  <?php
    // overrides don't have page parents, categories or tags
    if($query_type != 'override')
    { ?>
      <!-- page parents -->
      <div id="qw-post-parent" class="qw-query-content qw-single-value">
        <p>Use only with post type 'Page' to show results with the chosen parent ID.</p>
        <input class="qw-field-value" type="text" name="qw-query-options[args][post_parent]" value="<?php print $filters['post_parent']; ?>" />
      </div>
      
      <!-- categories -->
      <div id="qw-categories" class="qw-query-content qw-checkbox-values">
        <p>Select which categories to pull posts from, and how to treat those categories.</p>
        <div class="qw-checkboxes">
          <?php
            // List all categories as checkboxes
            foreach($category_ids as $cat_id)
            {
              $cat_name = get_cat_name($cat_id);
              $cat_checked = (isset($filters['cat'][$cat_id])) ? 'checked="checked"' : '';
              ?>
              <label class="qw-query-checkbox"><input type="checkbox" name="qw-query-options[args][cat][<?php print $cat_id; ?>]" value="<?php print $cat_name; ?>" <?php print $cat_checked; ?> /><?php print $cat_name; ?></label> 
              <?php
            }
          ?>
        </div>
        <p><strong>Categories Options</strong> - show posts that are:</p>
        <select name="qw-query-options[args][cat_operator]" class="qw-field-value">
          <option value="cat" <?php if($filters['cat_operator'] == "cat") {print 'selected="selected"';} ?>>Any category plus children categories</option>
          <option value="category__in" <?php if($filters['cat_operator'] == "category__in") {print 'selected="selected"';} ?>>Any category without children categories</option>
          <option value="category__and" <?php if($filters['cat_operator'] == "category__and") {print 'selected="selected"';} ?>>All categories selected</option>
          <option value="category__not_in" <?php if($filters['cat_operator'] == "category__not_in") {print 'selected="selected"';} ?>>Not in the categories selected</option>
        </select>
      </div>
      
      <!-- tags -->
      <div id="qw-tags" class="qw-query-content qw-checkbox-values">
        <p>Select which tags to use.</p>
        <div class="qw-checkboxes">
          <?php
            foreach($tags as $tag)
            {
              $tag_checked = (isset($filters['tag'][$tag->term_id])) ? 'checked="checked"' : '';
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
      
      <?php
    }
  ?>        