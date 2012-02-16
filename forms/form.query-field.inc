<?php
  $link_selected = ($field['values']['link']) ? 'checked="checked"' : '';
  $has_label = ($field['values']['has_label']) ? 'checked="checked"' : '';
  $rewrite_output_selected = ($field['values']['rewrite_output']) ? 'checked="checked"': '';
  $exclude_display_selected= ($field['values']['exclude_display']) ? 'checked="checked"' : '';
?>
<div id="qw-field-<?php print $field['name']; ?>" class="qw-edit-field-settings qw-query-content">
  <input class='qw-field-type'
         type='hidden'
         name='<?php print $field['form_prefix']; ?>[type]'
         value='<?php print $field['type']; ?>' />
  <input class='qw-field-name'
         type='hidden'
         name='<?php print $field['form_prefix']; ?>[name]'
         value='<?php print $field['name']; ?>' />

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
               value="<?php print $field['label']; ?>"/>
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
            Available replacement tokens.  These tokens wll be replaced with the processed results of their fields.
          </p>
          <ul class="qw-field-tokens-list">
            <?php
              foreach($tokens as $token)
              { ?>
                <li><?php print $token; ?></li>
                <?php
              }
            ?>
          </ul>
        </div>
      </div>
    </div>

    <?php
      /*
       * Images settings
       */
      if($field['type'] == 'image_attachment')
      { ?>
        <!-- image display -->
        <div class="qw-options-group">
          <strong>Number of items to show:</strong> <br />
          <input type="text"
              name='<?php print $field['form_prefix']; ?>[image_display_count]'
              value="<?php print ($field['values']['image_display_count']) ? $field['values']['image_display_count']: 0;?>" />
          <br />
          <strong>Image Display Style:</strong><br />
          <select name='<?php print $field['form_prefix']; ?>[image_display_style]'>
            <?php
              foreach($image_sizes as $key => $size)
              {
                $size_selected = ($field['values']['image_display_style'] == $size) ? 'selected="selected"' : '';
                ?>
                  <option value="<?php print $size; ?>" <?php print $size_selected; ?>><?php print $size; ?></option>
                <?php
              }
            ?>
          </select>
        </div>
        <?php
      }

      /*
       * Files settings
       */
      if($field['type'] == 'file_attachment')
      { ?>
        <!-- file display -->
        <div class="qw-options-group">
          <strong>Number of items to show:</strong> <br />
          <input type="text"
              name='<?php print $field['form_prefix']; ?>[file_display_count]'
              value="<?php print ($field['values']['file_display_count']) ? $field['values']['file_display_count']: 0;?>" />
          <br />
          <strong>File Display Style:</strong><br />
          <select name='<?php print $field['form_prefix']; ?>[file_display_style]'>
            <?php
              foreach($file_styles as $key => $file_style_details)
              {
                $style_selected = ($field['values']['file_display_style'] == $key) ? 'selected="selected"' : '';
                ?>
                  <option value="<?php print $key; ?>" <?php print $style_selected; ?>><?php print $file_style_details['description']; ?></option>
                <?php
              }
            ?>
          </select>
        </div>
        <?php
      }
    ?>
  </div>
</div>