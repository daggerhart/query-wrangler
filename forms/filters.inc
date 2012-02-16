<?php
/*
 * Providing theme functions for the default filters
 *
 * $filter - The current filter with filter type data
 */
/*
 * Post Types
 * - checkboxes
 */
function theme_qw_filter_post_types($filter)
{
  ob_start();
  ?>
  <div class="qw-checkboxes">
    <?php
      $post_types = qw_all_post_types();
      // loop through post types
      foreach ($post_types  as $post_type )
      {
        $post_type_checked = (isset($filter['values']['post_types'][$post_type])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[post_types][<?php print $post_type; ?>]"
                 value="<?php print $post_type; ?>"
                 <?php print $post_type_checked; ?> />
          <?php print ucfirst($post_type); ?>
        </label>
        <?php
      }
    ?>
  </div>
  <?php
  return ob_get_clean();
}

/*
 * Post Id
 * - text, select
 */
function theme_qw_filter_post_id($filter)
{
  ob_start();
  ?>
  <p>
    Comma separated list of post ids:<br />
    <input type='text' name="<?php print $filter['form_prefix']; ?>[post_ids]" value='<?php print $filter['values']['post_ids']; ?>' />
  </p>
  <p>
    Compare: <br />
    <select name="<?php print $filter['form_prefix']; ?>[compare]">
      <option value="post__in" <?php if($filter['values']['compare'] == "post__in") { print 'selected="selected"';} ?>>Only these posts</option>
      <option value="post__not_in" <?php if($filter['values']['compare'] == "post__not_in") { print 'selected="selected"';} ?>>Not these posts</option>
    </select>
  </p>
  <?php
  return ob_get_clean();
}

/*
 * Post Parent
 * - text
 */
function theme_qw_filter_post_parent($filter)
{
  ob_start();
  ?>
  <input class="qw-field-value"
         type="text"
         name="<?php print $filter['form_prefix']; ?>[post_parent]"
         value="<?php print $filter['values']['post_parent']; ?>" />
  <?php
  return ob_get_clean();
}

/*
 * Categories
 * - checkboxes, select
 */
function theme_qw_filter_categories($filter)
{
  ob_start();
  ?>
  <div class="qw-checkboxes">
    <?php
      $category_ids = get_all_category_ids();
      // List all categories as checkboxes
      foreach($category_ids as $cat_id)
      {
        $cat_name = get_cat_name($cat_id);
        $cat_checked = (isset($filter['values']['cats'][$cat_id])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[cats][<?php print $cat_id; ?>]"
                 value="<?php print $cat_name; ?>"
                 <?php print $cat_checked; ?> />
          <?php print $cat_name; ?>
        </label>
        <?php
      }
    ?>
  </div>
  <p><strong>Categories Options</strong> - show posts that are:</p>
  <select name="<?php print $filter['form_prefix']; ?>[cat_operator]" class="qw-field-value">
    <option value="cat" <?php if($filter['values']['cat_operator'] == "cat") {print 'selected="selected"';} ?>>Any category plus children categories</option>
    <option value="category__in" <?php if($filter['values']['cat_operator'] == "category__in") {print 'selected="selected"';} ?>>Any category without children categories</option>
    <option value="category__and" <?php if($filter['values']['cat_operator'] == "category__and") {print 'selected="selected"';} ?>>All categories selected</option>
    <option value="category__not_in" <?php if($filter['values']['cat_operator'] == "category__not_in") {print 'selected="selected"';} ?>>Not in the categories selected</option>
  </select>
  <?php
  return ob_get_clean();
}

/*
 * Tags
 * - checkboxes, select
 */
function theme_qw_filter_tags($filter)
{
  ob_start();
  ?>
  <div class="qw-checkboxes">
    <?php
      $tags = get_tags(array('hide_empty' => false));
      foreach($tags as $tag)
      {
        $tag_checked = (isset($filter['values']['tags'][$tag->term_id])) ? 'checked="checked"' : '';
        ?>
        <label class="qw-query-checkbox">
          <input type="checkbox"
                 name="<?php print $filter['form_prefix']; ?>[tags][<?php print $tag->term_id; ?>]"
                 value="<?php print $tag->name; ?>"
                 <?php print $tag_checked; ?> />
          <?php print $tag->name; ?>
        </label>
        <?php
      }
    ?>
  </div>
  <p><strong>Tag Options</strong> - show posts that have:</p>
  <select name="<?php print $filter['form_prefix']; ?>[tag_operator]" class="qw-field-value">
    <option value="tag__in">Any of the selected tags</option>
    <option value="tag__and">All of the selected tags</option>
    <option value="tag__not_in">None of the selected tags</option>
  </select>
  <?php
  return ob_get_clean();
}

/*
 * Meta Key / Value Pair
 * - text, select, text
 */
function theme_qw_filter_meta_key_value($filter)
{
  ob_start();
  ?>
  <p>
    Meta Key<br />
    <input type='text' name="<?php print $filter['form_prefix']; ?>[meta_key]" value='<?php print $filter['values']['meta_key']; ?>' />
  </p>
  <p>
    Compare<br />
    <select name="<?php print $filter['form_prefix']; ?>[meta_compare]">
      <option value="="  <?php if($filter['values']['meta_compare'] == "=")  {print 'selected="selected"';} ?>>Is equal to</option>
      <option value="!=" <?php if($filter['values']['meta_compare'] == "!=") {print 'selected="selected"';} ?>>Is not equal to</option>
      <option value="<"  <?php if($filter['values']['meta_compare'] == "<")  {print 'selected="selected"';} ?>>Is less than</option>
      <option value="<=" <?php if($filter['values']['meta_compare'] == "<=") {print 'selected="selected"';} ?>>Is less than or equal to</option>
      <option value=">"  <?php if($filter['values']['meta_compare'] == ">")  {print 'selected="selected"';} ?>>Is greater than</option>
      <option value=">=" <?php if($filter['values']['meta_compare'] == ">=") {print 'selected="selected"';} ?>>Is greater than or equal to</option>
    </select>
  </p>
  <p>
    Meta Value<br />
    <textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
              class="qw-meta-value"><?php print stripcslashes($filter['values']['meta_value']); ?></textarea>
  </p>
  <?php
  return ob_get_clean();
}

/*
 * Meta Key
 * - text
 */
function theme_qw_filter_meta_key($filter)
{
  ob_start();
  ?>
  <p>
    Meta Key<br />
    <input type='text'
           name="<?php print $filter['form_prefix']; ?>[meta_key]"
           value='<?php print $filter['values']['meta_key']; ?>' />
  </p>
  <?php
  return ob_get_clean();
}

/*
 * Meta Value
 * - text
 */
function theme_qw_filter_meta_value($filter)
{
  ob_start();
  ?>
  <p>
    Meta Value<br />
    <textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
              class="qw-meta-value"><?php print stripcslashes($filter['values']['meta_value']); ?></textarea>
  </p>
  <?php
  return ob_get_clean();
}
