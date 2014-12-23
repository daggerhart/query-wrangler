<?php
/*
$basics
$filters
$sorts
$fields

=== Query ===

$basics['posts_per_page'], $basics['post_status'], $basics['offset']

$filters
 - $all_filters
$sorts
 - $all_sorts

=== Display / Output ===

$basics['display_title']
$basics['style']
$basics['display_row_style']
$basics['header']
$basics['footer']
$basics['empty']

$fields
 - $all_fields

=== Context ===

$overrides
*/

?>
<form id="qw-edit-query-form" action='admin.php?page=query-wrangler&action=update&edit=<?php print $query_id; ?>&noheader=true' method='post'>
  <div id="qw-query-action-buttons">
    <div id="query-actions">
      <a href="admin.php?page=query-wrangler&export=<?php print $query_id; ?>">Export</a>
    </div>
    <input class='button-primary' type="submit" name="save" value="Save" />
    <input type="hidden" name="query-id" value="<?php print $query_id; ?>" />
  </div>
  <div id="message" class="updated qw-changes">
    <p><a>*</a> Changes have been made that need to be saved.</p>
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

  <div id="qw-query-admin-options-wrap">

<!-- QUERY SETTINGS -->
    <div class="qw-query-admin-column">
      <h2>Query Settings</h2>
	    <p class="description"></p>

      <table class="form-table">
        <?php
          $tmp = array(
            $basics['posts_per_page'],
            $basics['post_status'],
            $basics['offset']
          );

          foreach($tmp as $basic)
          { ?>
            <tr>
              <th><label><?php print $basic['title']; ?></label></th>
              <td>
              <?php
                if ( isset( $basic['form_callback'] ) && function_exists( $basic['form_callback'] ) )
                { ?>
                  <div class="qw-basic-form">
                    <?php
                    ob_start();
                      $basic['form_callback']($basic, $options[$basic['option_type']]);
                    print ob_get_clean();
                    ?>
                  </div>
                <?php
                }
              ?>
              </td>
            </tr>
            <?php
          }
        ?>

        <?php // filters ?>
        <tr>
          <th>
            <label>Filters</label>
            <p class="description">Select filters to affect the query's results.</p>
          </th>
          <td>
            <div class="qw-add-items-wrapper">
              <div class="qw-add-items-controls">
                <span class="qw-add-item button">Add Filters</span>
              </div>
              <div id="qw-all-filters" class="qw-add-new-items">
                <?php
                  // loop through filters
                  foreach($all_filters as $hook_key => $filter)
                  {
                    // for now, this is how I'll prevent certain filters on overrides
                    if(isset($filter['query_display_types']) && is_array($filter['query_display_types']) && in_array($query_type, $filter['query_display_types']))
                    { ?>
                    <label>
                      <input type="checkbox" value="<?php print $filter['type']; ?>" />
                      <input type="hidden" value="<?php print $filter['hook_key']; ?>" />
                      <?php print $filter['title']; ?>
                    </label>
                    <p class="description qw-desc"><?php print $filter['description']; ?></p>
                    <?php
                    }
                  }
                ?>
                <div class="qw-add-items-submit-wrapper">
                  <span class="qw-add-items-submit button" title="filter">Submit</span>
                </div>
              </div>
            </div>
            <div id="query-filters" class="sortable qw-handler-items">
              <?php
                // loop through and display
                if ( is_array( $filters ) ) {
                  foreach($filters as $filter_name => $filter)
                  {
                    $args = array(
                      'filter' => $filter,
                      'weight' => $filter['weight'],
                    );
                    ?>
                    <div class="qw-handler-item">
                      <?php print theme('query_filter', $args); ?>
                    </div>
                  <?php
                  }
                }
              ?>
            </div>
          </td>
        </tr>

        <?php // sorts ?>
        <tr>
          <th>
            <label>Order By</label>
            <p class="description">Select options for sorting the query results.</p>
          </th>
          <td>
            <div class="qw-add-items-wrapper">
              <div class="qw-add-items-controls">
                <span class="qw-add-item button" title="sort">Add Sorts</span>
              </div>
              <div id="qw-all-sorts" class="qw-add-new-items">
                <?php
                  // list all sorts that can be chosen
                  foreach($all_sorts as $hook_key => $sort)
                  { ?>
                    <label>
                      <input type="checkbox"
                             value="<?php print $sort['type']; ?>" />
                      <input class="qw-handler-hook_key"
                             type="hidden"
                             value="<?php print $sort['hook_key']; ?>" />
                      <?php print $sort['title']; ?>
                    </label>
                    <p class="description"><?php print $sort['description']; ?></p>
                    <?php
                  }
                ?>
                <div class="qw-add-items-submit-wrapper">
                  <span class="qw-add-items-submit button" title="sort">Submit</span>
                </div>
              </div>
            </div>

            <div id="query-sorts" class="sortable qw-handler-items">
              <?php
                // list all existing sorts on the query
                if( is_array( $sorts ) ){
                  foreach($sorts as $sort)
                  {
                    $args = array(
                      'sort' => $sort,
                      'weight' => $sort['weight'],
                    );
                    ?>
                    <div class="qw-handler-item">
                      <?php  print theme('query_sort', $args); ?>
                    </div>
                    <?php
                  }
                }
              ?>
            </div>
          </td>
        </tr>
      </table>
    </div><!-- /query settings -->


  <!-- DISPLAY SETTINGS -->
  <div class="qw-query-admin-column">
    <h2>Display Settings</h2>
	  <p class="description"></p>

    <table class="form-table">
      <?php
        $tmp = array(
          $basics['display_title'],
          $basics['style'],
          $basics['display_row_style'],
          $basics['header'],
          $basics['footer'],
          $basics['empty']
        );

        foreach($tmp as $basic)
        { ?>
          <tr>
            <th>
              <label><?php print $basic['title']; ?></label>
            </th>
            <td>
              <?php
                if ( isset( $basic['form_callback'] ) && function_exists( $basic['form_callback'] ) )
                { ?>
                  <div class="qw-basic-form">
                    <?php
                    ob_start();
                      $basic['form_callback']( $basic, $options[$basic['option_type']] );
                    print ob_get_clean();
                    ?>
                  </div>
                  <?php
                }
              ?>
            </td>
          </tr>
        <?php
        }
      ?>
      <tr>
        <th>
          <label>Fields</label>
          <p class="description">Select Fields to add to this query's output.</p>
        </th>
        <td>
          <div class="qw-add-items-wrapper">
            <div class="qw-add-items-controls">
              <span class="qw-add-item button" title="field">Add Fields</span>
            </div>
            <div id="qw-all-fields" class="qw-add-new-items">
              <?php
                // loop through fields
                foreach($all_fields as $hook_key => $field)
                {
                  ?>
                  <label>
                    <input type="checkbox"
                           value="<?php print $field['type']; ?>" />
                    <input class="qw-handler-hook_key"
                           type="hidden"
                           value="<?php print $field['hook_key']; ?>" />
                    <?php print $field['title']; ?>
                  </label>
                  <p class="description"><?php print $field['description']; ?></p>
                <?php
                }
              ?>
              <div class="qw-add-items-submit-wrapper">
                <span class="qw-add-items-submit button" title="field">Submit</span>
              </div>
            </div>
          </div>

          <div id="query-fields" class="sortable qw-handler-items">
            <?php
              if( is_array( $fields ) ) {
                $tokens = array();
                // loop through existing fields
                foreach($fields as $field)
                {
                  $tokens[$field['name']] = '{{'.$field['name'].'}}';
                  $args = array(
                    'image_sizes' => $image_sizes,
                    'file_styles' => $file_styles,
                    'field' => $field,
                    'weight' => $field['weight'],
                    'options' => $options,
                    'display' => $display,
                    'args'  => $args,
                    'tokens' => $tokens,
                  );
                  ?>
                  <div class="qw-handler-item">
                    <?php print theme('query_field', $args); ?>
                  </div>
                  <?php
                }
              }
            ?>
          </div>
        </td>
      </tr>

    </table>
  </div><!-- /display settings -->

<?php
  // CONTEXTUAL SETTINGS
  // override queries have different category and tag options
  if( $query_type == "override" )
  { ?>
    <div class="qw-query-admin-column">
      <h2>Contextual Settings</h2>
	    <p class="description"></p>

      <table class="form-table">
        <tr>
          <th>
            <label>Overrides</label>
          </th>
          <td>
            <div id="query-overrides" class="qw-handler-items">
              <!-- Categories -->
              <div class="qw-handler-item">
                <span class="qw-setting-header">Categories</span>
                <div class="group">
                  <p class="description">Select which category archive pages will be replaced with this query.</p>
                  <?php
                    // List all categories as checkboxes
                    foreach($category_ids as $cat_id)
                    {
                      $cat_name = get_cat_name($cat_id);
                      $cat_checked = (isset($options['override']['cats'][$cat_id])) ? 'checked="checked"' : '';
                      ?>
                      <label class="qw-query-checkbox">
                        <input class="qw-js-title"
                               type="checkbox"
                               name="qw-query-options[override][cats][<?php print $cat_id; ?>]"
                               value="<?php print $cat_name; ?>"
                          <?php print $cat_checked; ?> />
                        <?php print $cat_name; ?>
                      </label>
                    <?php
                    }
                  ?>
                </div>
              </div>

              <!-- Tags -->
              <div class="qw-handler-item">
                <span class="qw-setting-header">Tags</span>
                <div class="group">
                  <p class="description">Select which tag archive pages will be replaced with this query.</p>
                  <?php
                    // List all categories as checkboxes
                    foreach($tags as $tag)
                    {
                      $tag_checked = (isset($options['override']['tags'][$tag->term_id])) ? 'checked="checked"' : '';
                      ?>
                      <label class="qw-query-checkbox">
                        <input class="qw-js-title"
                               type="checkbox"
                               name="qw-query-options[override][tags][<?php print $tag->term_id; ?>]"
                               value="<?php print $tag->name; ?>"
                          <?php print $tag_checked; ?> />
                        <?php print $tag->name; ?>
                      </label>
                    <?php
                    }
                  ?>
                </div>
              </div>

            </div>
          </td>
        </tr>

      </table>
    </div>
  <?php
  }
?>

<?php
  // editor-shared preview area html template
  include_once QW_PLUGIN_DIR . '/admin/editors/preview-area.php';
?>
