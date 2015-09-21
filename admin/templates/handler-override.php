<?php
/*
 * $override: array of default override data, and specific override data
 */
?>
<!-- <?php print $override['name']; ?> -->
<div id="qw-override-<?php print $override['name']; ?>"
     class="qw-override qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php print $override['title']; ?>
  </span>

	<div class="group">
		<input class='qw-handler-item-type'
		       type='hidden'
		       name='<?php print $override['form_prefix']; ?>[type]'
		       value='<?php print $override['type']; ?>'/>
		<input class='qw-handler-item-hook_key'
		       type='hidden'
		       name='<?php print $override['form_prefix']; ?>[hook_key]'
		       value='<?php print $override['hook_key']; ?>'/>
		<input class='qw-handler-item-name'
		       type='hidden'
		       name='<?php print $override['form_prefix']; ?>[name]'
		       value='<?php print $override['name']; ?>'/>

		<div class="qw-remove button">
			Remove
		</div>
		<div class="qw-weight-container">
			<input class='qw-weight'
			       name='<?php print $override['form_prefix']; ?>[weight]'
			       type='text' size='2'
			       value='<?php print $weight; ?>'/>
		</div>

		<p class="description"><?php print $override['description']; ?></p>

		<?php if ( $override['form'] ) { ?>
			<div class="qw-override-form">
				<?php print $override['form']; ?>
			</div>
		<?php } ?>
	</div>
</div>
