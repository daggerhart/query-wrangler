<li class="qw-item qw-sort-item">
  <div class='sort-handle'></div>
  <span class='qw-sort-remove qw-button'>Remove this sort</span>
  <span class='qw-sort-sort-name'><?php print $sort['name']; ?></span>
  <input class='qw-sort-weight'
         name='qw-query-options[args][sorts][<?php print $sort['name']; ?>][weight]'
         type='text' size='2'
         value='<?php print $weight; ?>' />
  <span class="qw-sort-title"><?php print $sort['title']; ?></span>
</li>