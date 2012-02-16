<?php
/*
 * $filter: array of default filter data, and specific filter data
 */
?>
<!-- <?php print $filter['name']; ?> -->
<div id="qw-filter-<?php print $filter['name']; ?>" class="qw-query-content qw-checkbox-values">
  <input class='qw-filter-type'
         type='hidden'
         name='qw-query-options[args][filters][<?php print $filter['name']; ?>][type]'
         value='<?php print $filter['type']; ?>' />
  <p class="description"><?php print $filter['description']; ?></p>

  <div class="qw-filter-form">
    <?php print $filter['form']; ?>
  </div>
</div>
