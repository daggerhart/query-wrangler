<?php
/*
 * $filter: array of default filter data, and specific filter data
 */
?>
<!-- <?php print $filter['name']; ?> -->
<div id="qw-filter-<?php print $filter['name']; ?>" class="qw-query-content qw-checkbox-values">
  <input class='qw-filter-type'
         type='hidden'
         name='<?php print $filter['form_prefix']; ?>[type]'
         value='<?php print $filter['type']; ?>' />
  <input class='qw-filter-hook_key'
         type='hidden'
         name='<?php print $filter['form_prefix']; ?>[hook_key]'
         value='<?php print $filter['hook_key']; ?>' />
  <input class='qw-filter-name'
         type='hidden'
         name='<?php print $filter['form_prefix']; ?>[name]'
         value='<?php print $filter['name']; ?>' />

  <p class="description"><?php print $filter['description']; ?></p>

  <?php if ($filter['form']) { ?>
    <div class="qw-filter-form">
      <?php print $filter['form']; ?>
    </div>
  <?php } ?>
</div>
