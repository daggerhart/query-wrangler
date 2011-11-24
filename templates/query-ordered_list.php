<?php
/*
 * $style - field style
 * $rows - a processed array of rows fields and classes
 * $query_details - other query details
 */
?>
<ol class="qw-query-unformatted query-<?php print $slug; ?>">
  <?php foreach($rows as $row): ?>
    <li class="<?php print $row['row_classes']; ?>">
      
      <?php foreach($row['fields'] as $field): ?>
        <div class="<?php print $field['classes']; ?>">
          <?php print $field['output']; ?>
        </div>
      <?php endforeach; ?>
    
    </li>
  <?php endforeach; ?>
</ol>