<?php

// add default overrides to the filter
add_filter('qw_overrides', 'qw_override_categories');

function qw_override_categories($overrides){

  $overrides['cats'] = array(
    'title' => 'Categories',
    'description' => 'Override output based on categories',
    'form_callback' => 'qw_override_categories_form',
  );
  return $overrides;
}

function qw_override_categories_form( $override ){
  $category_ids = get_terms('category', array('fields' => 'ids', 'hide_empty' => 0));
  ?>
  <p>
    Select which categories to override.
  </p>
  <div class="qw-checkboxes">
    <?php
    // List all categories as checkboxes
    foreach($category_ids as $cat_id)
    {
      $cat_name = get_cat_name($cat_id);
      //$cat_checked = (isset($options['override']['cats'][$cat_id])) ? 'checked="checked"' : '';
      ?>
      <label class="qw-query-checkbox">
        <input class="qw-js-title"
               type="checkbox"
               name="qw-query-options[override][cats][<?php print $cat_id; ?>]"
               value="<?php print $cat_name; ?>"
          <?php //print $cat_checked; ?> />
        <?php print $cat_name; ?>
      </label>
    <?php
    }
    ?>
  </div>
  <?php
}
