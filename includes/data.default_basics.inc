<?php
/*
 * Basic Settings
 */
function qw_default_basic_settings($basics){
  $basics['display_title'] = array(
    'title' => 'Display Title',
    'option_type' => 'display',
    'description' => 'The title above the query page or widget',
    'form_callback' => 'qw_basic_display_title',
    'weight' => 0,
  );
  $basics['style'] = array(
    'title' => 'Template Style',
    'option_type' => 'display',
    'description' => 'How should this query be styled?',
    'form_callback' => 'qw_basic_display_style',
    'weight' => 0,
  );
  $basics['display_row_style'] = array(
    'title' => 'Row Style',
    'option_type' => 'display',
    'description' => 'How should each post in this query be presented?',
    'form_callback' => 'qw_basic_display_row_style',
    'weight' => 0,
  );
  $basics['posts_per_page'] = array(
    'title' => 'Posts Per Page',
    'option_type' => 'args',
    'description' => 'Number of posts to show per page. Use -1 to display all results.',
    'form_callback' => 'qw_basic_posts_per_page',
    'weight' => 0,
  );
  $basics['offset'] = array(
    'title' => 'Offset',
    'option_type' => 'args',
    'description' => 'Number of post to skip, or pass over. For example, if this field is 3, the first 3 items will be skipped and not displayed.',
    'form_callback' => 'qw_basic_offset',
    'weight' => 0,
  );
  $basics['post_status'] = array(
    'title' => 'Posts Status',
    'option_type' => 'args',
    'description' => 'Select the post status of the items displayed.',
    'form_callback' => 'qw_basic_post_status',
    'weight' => 0,
  );
  $basics['header'] = array(
    'title' => 'Header',
    'option_type' => 'display',
    'description' => 'The content placed here will appear above the resulting query.',
    'form_callback' => 'qw_basic_header',
    'weight' => 0,
  );
  $basics['footer'] = array(
    'title' => 'Footer',
    'option_type' => 'display',
    'description' => 'The content placed here will appear below the resulting query.',
    'form_callback' => 'qw_basic_footer',
    'weight' => 0,
  );
  $basics['empty'] = array(
    'title' => 'Empty Text',
    'option_type' => 'display',
    'description' => 'The content placed here will appear if the query has no results.',
    'form_callback' => 'qw_basic_empty',
    'weight' => 0,
  );
  $basics['wrapper_classes'] = array(
    'title' => 'Wrapper Classes',
    'option_type' => 'display',
    'description' => 'The CSS class names will be added to the query. This enables you to use specific CSS code for each query. You may define multiples classes separated by spaces.',
    'form_callback' => 'qw_basic_wrapper_classes',
    'weight' => 0,
  );
  return $basics;
}
add_filter('qw_basics', 'qw_default_basic_settings', 0);

function qw_basic_posts_per_page($basic, $args)
{
  ob_start();
  ?>
    <input class="qw-text-short"
           type="text"
           name="<?php print $basic['form_prefix']; ?>[posts_per_page]"
           value="<?php print $args['posts_per_page']; ?>" />
    <p class="description"><?php print $basic['description']; ?></p>
  <?php
  return ob_get_clean();
}
function qw_basic_offset($basic, $args)
{
  ob_start();
  ?>
    <input class="qw-text-short"
           type="text"
           name="<?php print $basic['form_prefix']; ?>[offset]"
           value="<?php print $args['offset']; ?>" />
    <p class="description"><?php print $basic['description']; ?></p>
  <?php
  return ob_get_clean();
}
function qw_basic_display_style($basic, $display)
{
  $styles = qw_all_styles();
  ob_start();
  ?>
    <select name="<?php print $basic['form_prefix']; ?>[style]"
            id="query-display-style">
      <?php
        // loop through field styles
        foreach($styles as $type => $style)
        {
          ?>
            <option value="<?php print $type; ?>"
                    <?php if($display['style'] == $type) { print 'selected="selected"';} ?>>
              <?php print $style['title']; ?>
            </option>
          <?php
        }
      ?>
    </select>
    <p class="description"><?php print $basic['description']; ?></p>

    <!-- style settings -->
    <div id="display-style-settings">
      <?php
        foreach($styles as $type => $style){
          if (function_exists($style['settings_callback']))
          {
            $style['values'] = $display[$style['settings_key']];
            ?>
            <div id="tab-style-settings-<?php print $style['hook_key']; ?>" class="qw-query-content">
              <span class="qw-setting-header"><?php print $style['title']; ?> Settings</span>
              <div class="qw-setting-group">
                <?php print $style['settings_callback']($style); ?>
              </div>
            </div>
            <?php
          }
        }
      ?>
    </div>
  <?php
  return ob_get_clean();
}
function qw_basic_display_row_style($basic, $display)
{
  $row_styles = qw_all_row_styles();
  ob_start();
  ?>
    <select id="qw-display-type"
            name="<?php print $basic['form_prefix']; ?>[row_style]">
      <?php
        foreach($row_styles as $type => $row_style)
        { ?>
            <option value="<?php print $type; ?>"
                    <?php if($display['row_style'] == $type) { print 'selected="selected"';} ?>>
              <?php print $row_style['title']; ?>
            </option>
          <?php
        }
      ?>
    </select>
    <p class="description"><?php print $basic['description']; ?></p>

    <!-- style settings -->
    <div id="row-style-settings">
      <?php
        foreach($row_styles as $type => $row_style){
          if (function_exists($row_style['settings_callback']))
          {
            $row_style['values'] = $display[$row_style['settings_key']];
            ?>
            <div id="tab-row-style-settings-<?php print $row_style['hook_key']; ?>" class="qw-query-content">
              <span class="qw-setting-header"><?php print $row_style['title']; ?> Settings</span>
              <div class="qw-setting-group">
                <?php print $row_style['settings_callback']($row_style); ?>
              </div>
            </div>
            <?php
          }
        }
      ?>
    </div>
  <?php
  return ob_get_clean();
}
function qw_basic_post_status($basic, $args)
{
  $post_statuses = qw_all_post_statuses();
  ob_start();
  ?>
    <select id="qw-post-status"
            name="<?php print $basic['form_prefix']; ?>[post_status]">
      <?php
        foreach($post_statuses as $key => $post_status)
        { ?>
            <option value="<?php print $key; ?>"
                    <?php if($args['post_status'] == $key) { print 'selected="selected"';} ?>>
              <?php print $post_status['title']; ?>
            </option>
          <?php
        }
      ?>
    </select>
    <p class="description"><?php print $basic['description']; ?></p>
  <?php
  return ob_get_clean();
}
function qw_basic_header($basic, $display)
{
  ob_start();
  ?>
    <p class="description"><?php print $basic['description']; ?></p>
    <textarea name="<?php print $basic['form_prefix']; ?>[header]"
            class="qw-field-textarea"><?php print qw_textarea($display['header']); ?></textarea>
  <?php
  return ob_get_clean();
}
function qw_basic_footer($basic, $display)
{
  ob_start();
  ?>
    <p class="description"><?php print $basic['description']; ?></p>
    <textarea name="<?php print $basic['form_prefix']; ?>[footer]"
              class="qw-field-textarea"><?php print qw_textarea($display['footer']); ?></textarea>
  <?php
  return ob_get_clean();
}
function qw_basic_empty($basic, $display)
{
  ob_start();
  ?>
    <p class="description"><?php print $basic['description']; ?></p>
    <textarea name="<?php print $basic['form_prefix']; ?>[empty]"
              class="qw-field-textarea"><?php print qw_textarea($display['empty']); ?></textarea>
  <?php
  return ob_get_clean();
}
function qw_basic_wrapper_classes($basic, $display)
{
  ob_start();
  ?>
    <input class="qw-text-long"
           type="text"
           name="<?php print $basic['form_prefix']; ?>[wrapper-classes]"
           value="<?php print $display['wrapper-classes']; ?>" />
    <p class="description"><?php print $basic['description']; ?></p>
  <?php
  return ob_get_clean();
}
function qw_basic_display_title($basic, $display)
{
  ob_start();
  ?>
    <input class="qw-text-long"
           type="text"
           name="<?php print $basic['form_prefix']; ?>[title]"
           value="<?php print $display['title']; ?>" />
    <p class="description"><?php print $basic['description']; ?></p>
  <?php
  return ob_get_clean();
}
/*
function qw_basic_($basic, $options)
{
  ob_start();
  ?>

  <?php
  return ob_get_clean();
}
// */
