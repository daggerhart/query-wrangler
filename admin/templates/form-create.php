<div>
	<p>
		Choose the name and the type of your query.
	</p>
</div>

<div id="qw-create">
	<form action='admin.php?page=query-wrangler&action=create&noheader=true'
	      method='post'>
		<div class="qw-setting">
			<label class="qw-label">Query Name:</label>
			<input class="qw-create-input" type="text" name="qw-name" value=""/>

			<p class="description">Query name is a way for you, the admin, to
				identify the query easily.</p>
		</div>

		<div class="qw-setting">
			<label class="qw-label">Query Type:</label>
			<select name="qw-type" class="qw-create-select">
				<option value="widget">Widget</option>
				<!-- option value="page">Page</option -->
				<option value="override">Override</option>
			</select>

			<p class="description">Query type determines how the query works
				within Wordpress. View desriptions on the right.</p>
		</div>

		<div class="qw-create">
			<input type="submit" value="Create" class="button-primary"/>
		</div>
	</form>
</div>

<div id="qw-create-description">
	<div>
		<h3>Widget Queries</h3>

		<p>
			The Query Wrangler comes with a reusable Wordpress Widget that an be
			places in sidebars.
			When you create a query of the this type, that query becomes
			selectable in the Widget settings.
		</p>
	</div>
	<div>
		<h3>Page Queries</h3>

		<p>
			Currently disabled. For using Queries as a page, create a normal WP
			Page and place the query shortcode on it.
			<!-- When you create a Page Query, you give that query a path (URI) to display on.
      After creating the query, you can visit that URI on your website to view the results.
      This is a great way to create new, complex pages on your Wordpress site.
      <br /><br />
      <strong><em>Pages do not work with the Default permalink structure found <a href="<?php print get_bloginfo( 'wpurl' ); ?>/wp-admin/options-permalink.php">here</a>.</em></strong>
    -->
		</p>
	</div>
	<div>
		<h3>Override Queries</h3>

		<p>
			An Override Query allows you to alter the way existing Wordpress
			pages such as Category and Tag pages display.
			<br/><br/>
			<strong><em>This feature is still in beta development. It has only
					been tested with permalinks set to
					/%category%/%postname%/</em></strong>
		</p>
	</div>
</div>