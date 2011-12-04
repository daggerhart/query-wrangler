<li class="qw-item qw-filter-item">
  <div class='sort-handle'></div>                      
  <span class='qw-filter-remove qw-button'>Remove this filter</span>
  <span class='qw-sort-filter-name'><?php print $filter_name; ?></span>
  <input class='qw-filter-weight' name='qw-query-options[args][filters][<?php print $filter_name; ?>][weight]' type='text' size='2' value='<?php print $weight; ?>' />
  <span class="qw-filter-title"><?php print str_replace("_", " ", $filter_name); ?></span>
</li>