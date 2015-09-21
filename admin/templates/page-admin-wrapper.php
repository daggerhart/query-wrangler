<div class="wrap">
	<h2><?php print $title; ?></h2>

	<?php if ( isset( $description ) ) : ?>
		<div class="description"><?php print $description; ?></div>
	<?php endif; ?>

	<div class="admin-content">
		<?php print $content; ?>
	</div>
</div>