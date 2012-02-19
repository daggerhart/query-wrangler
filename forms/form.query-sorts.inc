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

  <div class="qw-sort-form">
    <!-- sort options -->
    <p><strong>Order by <?php print $sort['title']; ?>: </strong></p>
    <p class="description"><?php print $sort['description']; ?></p>

    <p>Select how to order the results.</p>
    <select name="<?php print $sort['form_prefix']; ?>[order_value]">
      <?php
        foreach ($sort['order_options'] as $value => $label)
        { ?>
          <option value="<?php print $value; ?>"
                  <?php if($sort['values']['order_value'] == $value) { print 'selected="selected"';} ?>>
            <?php print $label; ?>
          </option>
          <?php
        }
      ?>
    </select>
  </div>
</div>
