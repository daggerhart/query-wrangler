<?php
/*
 * $style - field style
 * $rows - a processed array of rows fields and classes
 * $query_details - other query details
 */
?>
<ul class="qw-query-unformatted query-<?php print $slug; ?>">
  <?php foreach($rows as $row): ?>
    <li class="<?php print $row['row_classes']; ?>">
      
      <?php foreach($row['fields'] as $field): ?>
        <?php if(isset($field['output'])): ?>
          <div class="<?php print $field['classes']; ?>">
            <?php print $field['output']; ?>
          </div>
        <?php endif; ?>
      <?php endforeach; ?>
    
    </li>
  <?php endforeach; ?>
</ul>