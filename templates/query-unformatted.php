<?php
/*
 * Note: For post queries, the post content (compete or excerpt) will appear
 * as the only field within a row.
 *
 * $style - field style
 * $rows - a processed array of rows fields and classes
 * $query_details - other query details
 */
?>
<div class="query-unformatted query-<?php print $slug; ?>">
  <?php foreach($rows as $row): ?>

    <div class="<?php print $row['row_classes']; ?>">
      <?php if ($row['fields']) : ?>

        <?php foreach($row['fields'] as $field): ?>
          <?php if(isset($field['output'])): ?>
            <div class="<?php print $field['classes']; ?>">
              <?php print $field['output']; ?>
            </div>
          <?php endif; ?>
        <?php endforeach; ?>

      <?php endif; ?>
    </div>

  <?php endforeach; ?>
</div>