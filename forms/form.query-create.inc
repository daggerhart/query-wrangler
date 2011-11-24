<div class="wrap">
  <h2>Create Query</h2>
  <div>
    <p>
      Choose the name and the type of your query.
    </p>
  </div>

  <div id="qw-create">
    <form action='admin.php?page=query-wrangler&action=create&noheader=true' method='post'>
      <div class="qw-create">
        <strong>Query Name:</strong>
        <p>
          Query name is a way for you, the admin, to identify the query easily.
        </p>
        <input class="qw-create-input" type="text" name="qw-name" value="" />
      </div>
      
      <div class="qw-create">
        <strong>Query Type:</strong>
        <p>
          Query type determines how the query works within Wordpress.  View desriptions on the right.
        </p>
        <select name="qw-type" class="qw-create-select">
          <option value="widget">Widget</option>
          <option value="page">Page</option>
          <option value="override">Override</option>
        </select>
      </div>
      
      <div class="qw-create">
        <input type="submit" value="Create" class="qw-button" />
      </div>
    </form>
  </div>
  
  <div id="qw-create-description">
    <div>
      <h3>Widget Queries</h3>
      <p>
        The Query Wrangler comes with a reusable Wordpress Widget that an be places in sidebars.
        When you create a query of the this type, that query becomes selectable in the Widget settings.
      </p>
    </div>
    <div>
      <h3>Page Queries</h3>
      <p>
        When you create a Page Query, you give that query a path (URI) to display on.
        After creating the query, you can visit that URI on your website to view the results.
        This is a great way to create new, complex pages on your Wordpress site.
      </p>
    </div>
    <div>
      <h3>Override Queries</h3>
      <p>
        An Override Query allows you to alter the way existing Wordpress pages such as Category and Tag pages display.
        <br /><br />
        <strong><em>This feature is still in beta development.  It has only been tested with permalinks set to /%category%/%postname%/</em></strong>
      </p>
    </div>
  </div>
</div>