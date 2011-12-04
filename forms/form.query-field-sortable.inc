<li class="qw-item qw-field-item">
  <div class='sort-handle'></div>                      
  <span class='qw-field-remove qw-button'>Remove this field</span>
  <span class='qw-sort-field-name'><?php print $field_name; ?></span>
  <input class='qw-field-weight' name='qw-query-options[display][field_settings][fields][<?php print $field_name; ?>][weight]' type='text' size='2' value='<?php print $weight; ?>' />
  <span class="qw-field-title"><?php print str_replace("_", " ", $field_name); ?></span>
</li>