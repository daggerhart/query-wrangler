<li class="qw-item qw-field-item">
  <div class='sort-handle'></div>
  <span class='qw-field-remove qw-button'>Remove this field</span>
  <span class="qw-field-title"><?php print $field['title']; ?></span>
  <input class='qw-field-weight' name='qw-query-options[display][field_settings][fields][<?php print $field['name']; ?>][weight]' type='text' size='2' value='<?php print $weight; ?>' />
  <span class='qw-sort-field-name'><?php print $field['name']; ?></span>
</li>