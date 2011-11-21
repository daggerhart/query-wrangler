<?php
/*
 * $style - field style
 * $rows - a processed array of rows fields and classes
 * $query_details - other query details
 */
?>
<div class="qw-query-unformatted query-<?php print $query_details['slug']; ?>">
  <?php foreach($rows as $row): ?>
    <div class="<?php print $row['row_classes']; ?>">
      
      <?php foreach($row['fields'] as $field): ?>
        <div class="<?php print $field['classes']; ?>">
          <?php print $field['output']; ?>
        </div>
      <?php endforeach; ?>
    
    </div>
  <?php endforeach; ?>
</div>