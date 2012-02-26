<form method="POST" action="admin.php?page=query-wrangler&action=import&noheader=true">
  <label>
    <strong>Query name: </strong><br />
  </label>
  <input type="text" id="import-name" name="import-name" value="" />
  <p class="description">
    Enter the name to use for this query if it is different from the source query. Leave blank to use the name of the query.
  </p>

  <strong>Paste query code here:</strong><br />
  <textarea name="import-query" id="import-query"></textarea>
  <input type="hidden" name="action" value="import" />
  <br />
  <input type="submit" class="qw-button" value="Import" />
</form>