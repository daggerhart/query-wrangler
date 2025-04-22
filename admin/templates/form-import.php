<form method="POST"
      action="admin.php?page=query-wrangler&action=import&noheader=true">

	<?php wp_nonce_field( 'qw-import' ); ?>

	<div class="qw-setting">
		<label class="qw-label">Query Name:</label>
		<input type="text" id="import-name" name="import-name" value=""/>

		<p class="description">Enter the name to use for this query if it is
			different from the source query. Leave blank to use the name of the
			query.</p>
	</div>

	<strong>Paste query code here:</strong><br/>
	<textarea name="import-query" id="import-query"></textarea>

	<div>
		<input type="hidden" name="action" value="import"/>
		<input type="submit" class="button-primary" value="Import"/>
	</div>
</form>
