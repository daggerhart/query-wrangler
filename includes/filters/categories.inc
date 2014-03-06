<?php

// add default filters to the filter
add_filter('qw_filters', 'qw_filter_categories');

function qw_filter_categories($filters){

  $filters['categories'] = array(
    'title' => 'Categories',
    'description' => 'Select which categories to pull posts from, and how to treat those categories.',
    'form_callback' => 'qw_filter_categories_form',
    'query_args_callback' => 'qw_generate_query_args_categories',
    'query_display_types' => array('page','widget'),
  );
  return $filters;
}

function qw_filter_categories_form($filter){
  $cat_ops = array(
    "cat" => "Any category plus children categories",
    "category__in" => "Any category without children categories",
    "category__and" => "All categories selected",
    "category__not_in" => "Not in the categories selected",
  );
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
          <input class=" qw-js-title"
                 type="checkbox"
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
  <p>
    <select class="qw-field-value qw-js-title" name="<?php print $filter['form_prefix']; ?>[cat_operator]">
      <?php
        foreach ($cat_ops as $op => $title)
        {
          $selected = ($filter['values']['cat_operator'] == $op) ? 'selected="selected"' : '';
          ?>
          <option value="<?php print $op;?>" <?php print $selected; ?>><?php print $title; ?></option>
          <?php
        }
      ?>
    </select>
  </p>
  <?php
}


function qw_generate_query_args_categories(&$args, $filter){
  if($filter['values']['cat_operator'] == 'cat' && is_array($filter['values']['cats'])){
    $args[$filter['values']['cat_operator']] = implode(",", array_keys($filter['values']['cats']));
  }
  else {
    $args[$filter['values']['cat_operator']] = $filter['values']['cats'];
  }
}