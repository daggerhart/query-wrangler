<?php
  $link_selected            = ($field['values']['link']) ? 'checked="checked"' : '';
  $has_label                = ($field['values']['has_label']) ? 'checked="checked"' : '';
  $rewrite_output_selected  = ($field['values']['rewrite_output']) ? 'checked="checked"': '';
  $exclude_display_selected = ($field['values']['exclude_display']) ? 'checked="checked"' : '';
?>
<div id="qw-field-<?php print $field['name']; ?>" class="qw-field qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php
      print $field['title'];
      if($has_label){
        print ': '.$field['values']['label'];
      }
    ?>
  </span>

  <div class="group">
    <input class='qw-field-type'
           type='hidden'
           name='<?php print $field['form_prefix']; ?>[type]'
           value='<?php print $field['type']; ?>' />
    <input class='qw-field-hook_key'
           type='hidden'
           name='<?php print $field['form_prefix']; ?>[hook_key]'
           value='<?php print $field['hook_key']; ?>' />
    <input class='qw-field-name'
           type='hidden'
           name='<?php print $field['form_prefix']; ?>[name]'
           value='<?php print $field['name']; ?>' />


    <div class="qw-remove button">
      Remove
    </div>
    <input class='qw-weight'
           name='qw-query-options[display][field_settings][fields][<?php print $field['name']; ?>][weight]'
           type='text' size='2'
           value='<?php print $weight; ?>' />

    <p class="description"><?php print $field['description']; ?></p>

    <?php if ($field['form']) { ?>
      <div class="qw-field-form qw-setting">
        <?php print $field['form']; ?>
      </div>
    <?php } ?>

    <div class='qw-field-options'>
      <!-- exclude display -->
      <label class='qw-field-checkbox'>
        <input type='checkbox'
               name='<?php print $field['form_prefix']; ?>[exclude_display]'
               <?php print $exclude_display_selected; ?> />
        Exclude this field from display
      </label>

      <!-- link -->
      <label class='qw-field-checkbox'>
        <input type='checkbox'
               name='<?php print $field['form_prefix']; ?>[link]'
               <?php print $link_selected; ?> />
        Link this field to the post
      </label>

      <!-- label -->
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox'
                   name='<?php print $field['form_prefix']; ?>[has_label]'
                   <?php print $has_label; ?> />
            Create a Label for this field.
          </label>
        </div>
        <div class="qw-options-group-content qw-field-options-hidden">
          <strong>Label Text: </strong>
          <input type="text"
                 name="<?php print $field['form_prefix']; ?>[label]"
                 value="<?php print $field['values']['label']; ?>"/>
        </div>
      </div>

      <!-- rewrite output -->
      <div class="qw-options-group">
        <div class="qw-options-group-title">
          <label class='qw-field-checkbox'>
            <input type='checkbox'
                   name='<?php print $field['form_prefix']; ?>[rewrite_output]'
                   <?php print $rewrite_output_selected; ?> />
            Rewrite the output of this field.
          </label>
        </div>
        <div class="qw-options-group-content qw-field-options-hidden">
          <textarea name='<?php print $field['form_prefix']; ?>[custom_output]'
                    class="qw-field-textarea"><?php print ($field['values']['custom_output']) ? qw_textarea($field['values']['custom_output']): ''; ?></textarea>
          <div class="qw-field-tokens">
            <p>
              Available replacement tokens.  These tokens will be replaced with the processed results of their fields.
            </p>
            <?php
              if(is_array($tokens))
              { ?>
                <ul class="qw-field-tokens-list">
                  <?php
                    foreach($tokens as $token)
                    { ?>
                      <li><?php print $token; ?></li>
                      <?php
                    }
                  ?>
                </ul>
                <?php
              }
            ?>
          </div>
        </div>
      </div>
    </div>

  </div>
</div>