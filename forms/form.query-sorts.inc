<?php
/*
 * $sort: array of default sort data, and specific sort data
 */
?>
<!-- <?php print $sort['name']; ?> -->
<div id="qw-sort-<?php print $sort['name']; ?>" class="qw-query-content">
  <input class='qw-sort-type'
         type='hidden'
         name='<?php print $sort['form_prefix']; ?>[type]'
         value='<?php print $sort['type']; ?>' />
  <input class='qw-sort-hook_key'
         type='hidden'
         name='<?php print $sort['form_prefix']; ?>[hook_key]'
         value='<?php print $sort['hook_key']; ?>' />
  <input class='qw-sort-name'
         type='hidden'
         name='<?php print $sort['form_prefix']; ?>[name]'
         value='<?php print $sort['name']; ?>' />

  <p class="description"><?php print $sort['description']; ?></p>

  <?php if ($sort['form']) { ?>
    <div class="qw-sort-form">
      <?php print $sort['form']; ?>
    </div>
  <?php } ?>
</div>
