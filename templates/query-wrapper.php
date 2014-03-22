<?php
/*
 * $header - header content
 * $content - processed html of the content
 * $pager - content pager
 * $footer - footer content
 * $wrapper_classes - generated query wrapper classes
 * $pager_classes - generated query pager classes
 */
?>
<div class="<?php print $wrapper_classes; ?>">
  <div class="query-wrapper-content">
    <?php if (isset($header)) { ?>
      <div class="query-header">
        <?php print $header; ?>
      </div>
    <?php } ?>

    <div class="query-content">
      <?php print $content; ?>
      <?php if (isset($pager)) { ?>
        <div class="<?php print $pager_classes; ?>">
          <?php print $pager; ?>
        </div>
      <?php } ?>
    </div>

    <?php if (isset($footer)) { ?>
      <div class="query-footer">
        <?php print $footer; ?>
      </div>
    <?php } ?>
  </div>
</div>