<?php
/*
 * $filter: array of default filter data, and specific filter data
 */
?>
<!-- <?php print $filter['name']; ?> -->
<div id="qw-filter-<?php print $filter['name']; ?>" class="qw-filter qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php print $filter['title']; ?>
  </span>
  <div class="group">
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

    <div class="qw-remove button">
      Remove
    </div>
    <input class='qw-weight'
           name='qw-query-options[args][filters][<?php print $filter['name']; ?>][weight]'
           type='text' size='2'
           value='<?php print $weight; ?>' />

    <p class="description"><?php print $filter['description']; ?></p>

    <?php if ($filter['form']) { ?>
      <div class="qw-filter-form">
        <?php print $filter['form']; ?>
      </div>
    <?php } ?>
  </div>
</div>
