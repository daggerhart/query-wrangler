<div id="qw-field-<?php print $field_name; ?>" class="qw-edit-field-settings qw-query-content">
  <input class='qw-field-type' type='hidden' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][type]' value='<?php print $field_settings['type']; ?>' />
  <input class='qw-field-name' type='hidden' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][name]' value='<?php print $field_name; ?>' />
  <span class='qw-field-title'><?php print str_replace("_", " ", $field_settings['type']); ?></span>
  <div class='qw-field-options'>
    <?php
      $link_selected = ($qw_query_options['display']['field_settings']['fields'][$field_name]['link']) ? 'checked="checked"' : '';
      $has_label = ($qw_query_options['display']['field_settings']['fields'][$field_name]['has_label']) ? 'checked="checked"' : '';
      $rewrite_output_selected = ($qw_query_options['display']['field_settings']['fields'][$field_name]['rewrite_output']) ? 'checked="checked"': '';
      $exclude_display_selected= ($qw_query_options['display']['field_settings']['fields'][$field_name]['exclude_display']) ? 'checked="checked"' : '';
    ?>
    <!-- exclude display -->
    <label class='qw-field-checkbox'>
      <input type='checkbox' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][exclude_display]' <?php print $exclude_display_selected; ?> />Exclude this field from display
    </label>
    
    <!-- link -->
    <label class='qw-field-checkbox'>
      <input type='checkbox' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][link]' <?php print $link_selected; ?> />Link this field to the post
    </label>
    
    <!-- label -->
    <div class="qw-options-group">
      <div class="qw-options-group-title">
        <label class='qw-field-checkbox'>
          <input type='checkbox' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][has_label]' <?php print $has_label; ?> />Create a Label for this field.
        </label>
      </div>
      <div class="qw-options-group-content qw-field-options-hidden">
        <strong>Label Text: </strong><input type="text" name="qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][label]" value="<?php print $qw_query_options['display']['field_settings']['fields'][$field_name]['label']; ?>"/>
      </div>
    </div>
    
    <!-- rewrite output -->
    <div class="qw-options-group">
      <div class="qw-options-group-title">
        <label class='qw-field-checkbox'>
          <input type='checkbox' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][rewrite_output]' <?php print $rewrite_output_selected; ?> />Rewrite the output of this field.
        </label>
      </div>
      <div class="qw-options-group-content qw-field-options-hidden">
        <textarea class="qw-field-textarea" name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][custom_output]'><?php print ($qw_query_options['display']['field_settings']['fields'][$field_name]['custom_output']) ? htmlentities($qw_query_options['display']['field_settings']['fields'][$field_name]['custom_output']): ''; ?></textarea>
        <div class="qw-field-tokens">
          <p>
            Available replacement tokens.  These tokens wll be replaced with the processed results of their fields.
          </p>
          <ul class="qw-field-tokens-list">
            <li>{{<?php print $field_name; ?>}}</li>
          </ul>
        </div>
      </div>
    </div>
    
    <?php
      /*
       * Images settings
       */ 
      if($field_settings['type'] == 'image_attachment')
      { ?>
        <!-- image display -->
        <div class="qw-options-group">
          <strong>Number of items to show:</strong> <br />
          <input type="text"
              name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][image_display_count]'
              value="<?php print ($qw_query_options['display']['field_settings']['fields'][$field_name]['image_display_count']) ? $qw_query_options['display']['field_settings']['fields'][$field_name]['image_display_count']: 0;?>" />
          <br />
          <strong>Image Display Style:</strong><br />
          <select name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][image_display_style]'>
            <?php
              foreach($image_sizes as $key => $size)
              {
                $size_selected = ($qw_query_options['display']['field_settings']['fields'][$field_name]['image_display_style'] == $size) ? 'selected="selected"' : '';
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
      if($field_settings['type'] == 'file_attachment')
      { ?>
        <!-- file display -->
        <div class="qw-options-group">
          <strong>Number of items to show:</strong> <br />
          <input type="text"
              name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][file_display_count]'
              value="<?php print ($qw_query_options['display']['field_settings']['fields'][$field_name]['file_display_count']) ? $qw_query_options['display']['field_settings']['fields'][$field_name]['file_display_count']: 0;?>" />
          <br />
          <strong>File Display Style:</strong><br />
          <select name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][file_display_style]'>
            <?php
              foreach($file_styles as $key => $file_style_details)
              {
                $style_selected = ($qw_query_options['display']['field_settings']['fields'][$field_name]['file_display_style'] == $key) ? 'selected="selected"' : '';
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