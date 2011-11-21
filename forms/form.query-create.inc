<div class="wrap">
  <h2>Create Query</h2>
  <div>
    <p>
      Choose the name and the type of your query.
    </p>
  </div>
  
  <form action='admin.php?page=query-wrangler&action=create&noheader=true' method='post'>
    <div class="qw-create">
      <strong>Query Name:</strong><br />
      <input class="qw-create-input" type="text" name="qw-name" value="" />
    </div>
    
    <div class="qw-create">
      <strong>Query Type:</strong><br />
      <select name="qw-type" class="qw-create-select">
        <option value="widget">Widget</option>
        <option value="page">Page</option>
      </select>
    </div>
    
    <div class="qw-create">
      <input type="submit" value="Create" class="qw-button" />
    </div>
  </form>
</div>