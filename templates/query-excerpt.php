<?php
/*
 * Normal item in WP loop
 *
 * This template appears within the style templates on output
 * eg. output structure
 * - query-unformatted
 * - - query-unformatted-row
 * - - - query-excerpt
 */
?>
<div id="post-<?php the_ID(); ?>" class="query-post post">
	<h2 class="query-title">
		<a class="query-title-link" href="<?php print get_permalink(); ?>">
			<?php the_title(); ?>
		</a>
	</h2>

	<div class="query-content post-excerpt">
		<?php the_excerpt(); ?>
	</div>
</div>