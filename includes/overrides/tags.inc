<?php

// add default overrides to the filter
add_filter('qw_overrides', 'qw_override_tags');

function qw_override_tags($overrides){

  $overrides['tags'] = array(
    'title' => 'Tags',
    'description' => 'Override output based on tags',
    'form_callback' => 'qw_override_tags_form',
  );
  return $overrides;
}

function qw_override_tags_form( $override ){
  $tags = get_tags(array('hide_empty' => false));
  ?>
  <p>
    Select which tags to override.
  </p>
  <div class="qw-checkboxes">
    <?php
    foreach($tags as $tag)
    { ?>
      <label class="qw-query-checkbox">
        <input class="qw-js-title"
               type="checkbox"
               name="<?php print $override['form_prefix']; ?>[<?php print $tag->term_id; ?>]"
               value="<?php print $tag->name; ?>"
              <?php checked( isset( $override['values'][ $tag->term_id ] ) ); ?> />
        <?php print $tag->name; ?>
      </label>
    <?php
    }
    ?>
  </div>
  <?php
}
