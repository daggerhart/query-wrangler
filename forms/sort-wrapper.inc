<?php
/*
 * $sort: array of default sort data, and specific sort data
 */
?>
<!-- <?php print $sort['name']; ?> -->
<div id="qw-sort-<?php print $sort['name']; ?>" class="qw-sort qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php print $sort['title']; ?>
  </span>
  <div class="group">
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

    <div class="qw-remove button">
      Remove
    </div>
    <input class='qw-weight'
           name='qw-query-options[args][sorts][<?php print $sort['name']; ?>][weight]'
           type='text' size='2'
           value='<?php print $weight; ?>' />

    <p class="description"><?php print $sort['description']; ?></p>

    <?php if ($sort['form']) { ?>
      <div class="qw-sort-form">
        <?php print $sort['form']; ?>
      </div>
    <?php } ?>
  </div>
</div>
