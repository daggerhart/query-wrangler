<?php
/*
 * Normal WP loop
 * $rows = the query
 */
  while($rows->have_posts())
  {
    $rows->the_post();
    ?>
    <div id="post-<?php the_ID(); ?>" class="query-post post">
      <h2 class="query-title">
        <a class="query-title-link" href="<?php print get_permalink(); ?>">
          <?php the_title(); ?>
        </a>
      </h2>
      <div class="query-content post-content">
        <?php the_content(); ?>
      </div>
    </div>
    <?php
  }
?>