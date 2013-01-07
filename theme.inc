<?php
/*
 * Template Wrangler templates
 *
 * @param $templates array Passed from the filter hook from WP
 *
 * @return array All template arrays filtered so far by Wordpress' filter hook
 */
function qw_templates($templates){
  // preview query
  $templates['query_preview'] = array(
    'files' => 'templates/admin-preview.php',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array('options' => NULL),
  );
  // edit query template
  $templates['query_edit'] = array(
    'files' => 'forms/form.query-edit.inc',
    'default_path' => QW_PLUGIN_DIR,
  );
  // create query template
  $templates['query_create'] = array(
    'files' => 'forms/form.query-create.inc',
    'default_path' => QW_PLUGIN_DIR,
  );
  // import query template
  $templates['query_import'] = array(
    'files' => 'forms/form.query-import.inc',
    'default_path' => QW_PLUGIN_DIR,
  );
  // export query template
  $templates['query_export'] = array(
    'files' => 'forms/form.query-export.inc',
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'query_id' => 0,
    ),
  );
  // field template
  $templates['query_field'] = array(
    'files' => 'forms/form.query-field.inc',
    'default_path' => QW_PLUGIN_DIR,
  );
  // sortable fields template
  $templates['query_field_sortable'] = array(
    'files' => 'forms/form.query-field-sortable.inc',
    'default_path' => QW_PLUGIN_DIR,
  );
  // filters edit form
  $templates['query_filter'] = array(
    'files' => 'forms/form.query-filters.inc',
    'default_path' => QW_PLUGIN_DIR,
  );
  // sortable filters template
  $templates['query_filter_sortable'] = array(
    'files' => 'forms/form.query-filter-sortable.inc',
    'default_path' => QW_PLUGIN_DIR,
  );

  // display queries style wrapper
  $templates['query_display_wrapper'] = array(
    'files' => array(
      'query-wrapper-[slug].php',
      'query-wrapper.php',
      'templates/query-wrapper.php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'slug' => '',
    )
  );
  // full and field styles
  $templates['query_display_rows'] = array(
    'files' => array(
      '[template]-[slug].php',
      '[template].php',
      'templates/[template].php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
      'template' => 'query-unformatted',
      'slug' => 'not-found',
      'style' => 'unformatted',
      'rows' => array(),
    )
  );
  // useful for plugin admin pages
  $templates['admin_wrapper'] = array(
    'files' => array(
        'templates/admin-wrapper.php',
        'admin-wrapper.php',
    ),
    'default_path' => QW_PLUGIN_DIR,
    'arguments' => array(
        'title' => 'Admin Page',
        'content' => 'content goes here.',
    ),
  );

  // filters
  $filters = qw_all_filters();
  foreach($filters as $type => $filter){
    if(isset($filter['template'])){
      $templates[$filter['template']] = array(
        'arguments' => array(
          'filter' => array()
        )
      );
    }
  }
  return $templates;
}
// tw hook
add_filter('tw_templates', 'qw_templates');
/*
 * Preprocess query_display_rows to allow field styles to define their own default path
 */
function theme_query_display_rows_preprocess($template)
{
  // make sure we know what style to use
  if(isset($template['arguments']['style']))
  {
    // get the specific style
    $all_styles = qw_all_styles();

    // set this template's default path to the style's default path
    if(isset($all_styles[$template['arguments']['style']])){
      $style = $all_styles[$template['arguments']['style']];
      $template['default_path'] = $style['default_path'];
    }
  }
  return $template;
}
/*
 * Template the entire query
 *
 * @param object
 *   $wp_query Wordpress query object
 * @param array
 *   $options the query options
 *
 * @return string HTML for themed/templated query
 */
function qw_template_query(&$wp_query, $options)
{
  $results_count = count($wp_query->posts);
  $content = '';

  // start building theme arguments
  $wrapper_args = array(
    'slug' => $options['meta']['slug'],
  );

  // look for empty results
  if ($results_count > 0)
  {
    $all_styles = qw_all_styles();

    // the content of the widget is the result of the query
    if($options['display']['row_style'] == "posts")
    {
      // setup row template arguments
      $template_args = array(
        'template' => 'query-'.$options['display']['style'],
        'slug' => $options['meta']['slug'],
        'style' => $options['display']['style'],
        'rows' => qw_make_posts_rows($wp_query, $options),
      );
    }
    else if ($options['display']['row_style'] == "fields")
    {
      // get style type from display style
      $style = $all_styles[$options['display']['style']];

      // setup row template arguments
      $template_args = array(
        'template' => 'query-'.$options['display']['style'],
        'slug' => $options['meta']['slug'],
        'style' => $options['display']['style'],
        // make rows
        'rows' => qw_make_fields_rows($wp_query, $options),
      );
    }

    // template the query rows
    $wrapper_args['content'] = theme('query_display_rows', $template_args);
  }
  // empty results
  else {
    // no pagination
    $options['meta']['pagination'] = false;
    // show empty text
    $wrapper_args['content'] = '<div class="query-empty">'.$options['meta']['empty'].'</div>';
  }

  $wrapper_classes = array();
  $wrapper_classes[] = 'query';
  $wrapper_classes[] = 'query-'.$options['meta']['slug'].'-wrapper';
  $wrapper_classes[] = $options['display']['wrapper-classes'];
  $wrapper_args['wrapper_classes'] = implode(" ", $wrapper_classes);

  // header
  if($options['meta']['header'] != '') {
    $wrapper_args['header'] = $options['meta']['header'];
  }
  // footer
  if($options['meta']['footer'] != '') {
    $wrapper_args['footer'] = $options['meta']['footer'];
  }

  // pagination
  if($options['meta']['pagination'] && isset($options['display']['page']['pager']['active'])){
    $pager_classes = array();
    $pager_classes[] = 'query-pager';
    $pager_classes[] = 'pager-'.$options['display']['page']['pager']['type'];
    $wrapper_args['pager_classes'] = implode(" ", $pager_classes);
    // pager
    $wrapper_args['pager'] = qw_make_pager($options['display']['page']['pager'], $wp_query);
  }

  // template with wrapper
  return theme('query_display_wrapper', $wrapper_args);
}
/*
 *
 */
function qw_make_posts_rows(&$wp_query, $options){
  $rows = array();
  $i = 0;
  while($wp_query->have_posts())
  {
    $wp_query->the_post();
    $template_args = array(
      'template' => 'query-'.$options['display']['post_settings']['size'],
      'slug' => $options['meta']['slug'],
      'style' => $options['display']['post_settings']['size'],
    );
    $rows[$i]['row_classes'] = qw_row_classes($i);
    $field_classes = array('query-post-wrapper');
    $rows[$i]['fields'][$i]['classes'] = implode(" ",$field_classes);
    $rows[$i]['fields'][$i]['output'] = theme('query_display_rows' , $template_args);
    $i++;
  }
  return $rows;
}
/*
 * Make theme row classes
 */
function qw_row_classes($i){
  $row_classes   = array('query-row');
  $row_classes[] = ($i%2) ? 'query-row-odd' : 'query-row-even';
  $row_classes[] = 'query-row-'.$i;

  return implode(" ", $row_classes);
}
/*
 * Build array of fields and rows for templating
 *
 * @param object $new_query WP_Query objecte generated
 * @param array $display Query display data
 * @return array Executed query rows
 */
function qw_make_fields_rows(&$wp_query, $options)
{
  $display = $options['display'];
  $all_fields = qw_all_fields();
  $rows = array();
  $tokens = array();

  // loop through each post
  $i = 0;
  while($wp_query->have_posts())
  {
    $wp_query->the_post();
    //
    $this_post = $wp_query->post;
    $rows[$i] = array();

    // make row classes
    $rows[$i]['row_classes'] = qw_row_classes($i);

    if(is_array($display['field_settings']['fields']))
    {
      // sort according to weights
      uasort($display['field_settings']['fields'],'qw_cmp');

      // loop through each field
      foreach($display['field_settings']['fields'] as $field_name => $field_settings)
      {
        // field open
        $field_classes = array('query-field');
        $field_classes[] = 'query-field-'.$field_settings['name'];

        $rows[$i]['fields'][$field_name]['classes'] = implode(" ",$field_classes);

        // get field details from all fields list
        $field_defaults = $all_fields[$field_settings['type']];

        // look for callback
        if(isset($field_defaults['callback']))
        {
          // callbacks with arguments
          if (is_array($field_defaults['arguments']))
          {
            $args = array();
            foreach($field_defaults['arguments'] as $key => $argument_token)
            {
              // split the argument into op & value
              $op = explode(':', $argument_token);

              // see if we have a value
              if(isset($op[1]))
              {
                // post object variable
                if($op[0] == 'this'){
                  $args[$key] = $this_post->{$op[1]};
                }

                // feild setting  variable
                if($op[0] == 'setting'){
                  $args[$key] = $field_settings[$op[1]];
                }
              }
              // if not, use the value given in the field definition
              else
              {
                // acts as a default value
                $args[$key] = $argument_token;
              }
            }

            // run the callback function with the arguments list
            $rows[$i]['fields'][$field_name]['output'] = call_user_func_array($field_defaults['callback'], array_values($args));
          }
          // normal callback w/o arguments
          else {
            $rows[$i]['fields'][$field_name]['output'].= $field_defaults['callback']();
          }
        }
        // use field itself
        else {
          $rows[$i]['fields'][$field_name]['output'].= $this_post->{$field_settings['type']};
        }

        // apply link to field
        if(isset($field_settings['link'])){
          $rows[$i]['fields'][$field_name]['output'] = '<a class="query-field-link" href="'.get_permalink().'">'.$rows[$i]['fields'][$field_name]['output'].'</a>';
        }

        // add token for replace
        $tokens['{{'.$field_name.'}}'] = $rows[$i]['fields'][$field_name]['output'];

        // look for rewrite output
        if(isset($field_settings['rewrite_output'])){
          // replace tokens with results
          $field_settings['custom_output'] = str_replace(array_keys($tokens), array_values($tokens), $field_settings['custom_output']);
          $rows[$i]['fields'][$field_name]['output'] = $field_settings['custom_output'];
        }

        // get default field label for tables
        $rows[$i]['fields'][$field_name]['label'] = ($field_settings['has_label']) ? $field_settings['label'] : '';

        // apply labels to full style fields
        if(isset($field_settings['has_label']) &&
           $display['type'] != 'full' &&
           $display['field_settings']['style'] != 'table')
        {
          $rows[$i]['fields'][$field_name]['output'] = '<label class="query-label">'.$field_settings['label'].'</label> '.$rows[$i]['fields'][$field_name]['output'];
        }

        // apply shortcodes to field output
        $rows[$i]['fields'][$field_name]['output'] = do_shortcode($rows[$i]['fields'][$field_name]['output']);

        // after all operations, remove if excluded
        if($field_settings['exclude_display']){
          unset($rows[$i]['fields'][$field_name]['output']);
        }
      }
    }
    // increment row
    $i++;
  }

  return $rows;
}

/*
 * Custom Pager function
 *
 * @param array $pager Query pager details
 * @param object $wp_query Object
 * @return HTML processed pager
 */
function qw_make_pager($pager, &$wp_query)
{
  $pager_themed = '';
  $pagers = qw_all_pager_types();

  //set callback if function exists
  if(function_exists($pagers[$pager['type']]['callback'])) {
    $callback = $pagers[$pager['type']]['callback'];
  } else {
    $callback = $pagers['default']['callback'];
  }

  // execute callback
  $pager_themed = call_user_func_array($callback, array($pager, $wp_query));

  return $pager_themed;
}
/*
 * Custom Default Pager
 *
 * @param array $pager Query options for pager
 * @param object $wp_query Object
 */
function qw_theme_pager_default($pager, &$wp_query)
{
  // help figure out the current page
  $path_array = explode('/page/', $_SERVER['REQUEST_URI']);

  $pager_themed = '';
  $pager['next'] = ($pager['next']) ? $pager['next'] : 'Next Page &raquo;';
  $pager['previous'] = ($pager['previous']) ? $pager['previous'] : '&laquo; Previous Page';

  if($page = qw_get_page_number($wp_query))
  {
    $wpurl = get_bloginfo('wpurl');

    // previous link with page number
    if($page >= 3){
      $pager_themed.= '<div class="query-prevpage">
                        <a href="'.$wpurl.$path_array[0].'/page/'.($page-1).'">'.$pager['previous'].'</a>
                      </div>';
    }
    // previous link with no page number
    else if ($page == 2)
    {
      $pager_themed.= '<div class="query-prevpage">
                        <a href="'.$wpurl.$path_array[0].'">'.$pager['previous'].'</a>
                      </div>';
    }

    // next link
    if(($page+1) <= $wp_query->max_num_pages){
      $pager_themed.= '<div class="query-nextpage">
                        <a href="'.$wpurl.$path_array[0].'/page/'.($page+1).'">'.$pager['next'].'</a>
                      </div>';
    }

    return $pager_themed;
  }
}
/*
 * Default Pager with page numbers
 *
 * @param array $pager Query options for pager
 * @param object $wp_query Object
 *
 * @return string HTML for pager
 */
function qw_theme_pager_numbers($pager, $wp_query)
{
		$big = intval($wp_query->found_posts.'000');
		$args = array(
    'base' => str_replace( $big, '%#%', get_pagenum_link( $big ) ),
    'format' => '?paged=%#%',
    'current' => max( 1, get_query_var('paged') ),
    'total' => $wp_query->max_num_pages
		);
  $pager_themed = paginate_links($args);
  return $pager_themed;
}

/*
 * Get and theme attached post files
 *
 * @param int $post_id The post->ID
 * $param int $count Number of files to get
 */
function qw_theme_file($post_id, $style = 'link', $count = 0){
  $files = qw_get_post_files($post_id);
  if(is_array($files))
  {
    $output = '';
    $i=0;
    foreach($files as $file){
      if(($count == 0 || ($i < $count)) && substr($file->post_mime_type,0,5) != "image")
      {
        switch($style){
          case 'url':
          $output .= wp_get_attachment_url($file->ID);
          break;

          case 'link':
          // complete file name
          $file_name = explode("/", $file->guid);
          $file_name = $file_name[count($file_name)-1];
          $output.= '<a href="'.wp_get_attachment_url($file->ID).'" class="query-file-link">'.$file_name.'</a>';
          break;

          case 'link_url':
            $output.= '<a href="'.wp_get_attachment_url($file->ID).'" class="query-file-link">'.$file->guid.'</a>';
            break;
        }
      }
      $i++;
    }
    return $output;
  }
}
/*
 * Get files attached to a post
 *
 * @param int $post_id The WP post id
 * @return Array of file posts
 */
function qw_get_post_files($post_id)
{
  $child_args = array(
    "post_type" => "attachment",
    "post_parent" => $post_id,
  );
		// Get images for this post
  $files = get_posts($child_args);

  if(is_array($files))
		{
    return $files;
  }
  return false;
}
/*
 * Turn a list of images into html
 *
 * @param $post_id
 * @param $image_type
 * @param $count;
 */
function qw_theme_image($post_id, $style, $count = 0)
{
  $images = qw_get_post_images($post_id);
  if(is_array($images)){
    $output = '';
    $i = 0;
    foreach($images as $image){
      if($count == 0 || ($i < $count)){
        $output.= wp_get_attachment_image($image->ID, $style);
      }
      $i++;
    }
    return $output;
  }
}
/*
 * Get all images attached to a single post
 *
 * @param int $post_id The Wordpress ID for the post or page to get images from
 *
 * @return sorted array of images
 */
function qw_get_post_images($post_id)
{
		$child_args = array(
    "post_type" => "attachment",
    "post_mime_type" => "image",
    "post_parent" => $post_id
  );
		// Get images for this post
  $images = &get_children($child_args);

		// If images exist for this page
		if(is_array($images))
		{
				// sort this so menu order matters
				$sorted = array();
    $unsorted = array();
				foreach ($images as $image)
				{
      if($image->menu_order !== 0){
        $sorted[$image->menu_order] = $image;
      }
      else {
        $unsorted[] = $image;
      }
				}
    // sort menu order
    ksort($sorted);
    // reset array
    $sorted = array_values($sorted);
    // add unsorted
    $sorted = array_merge($sorted, $unsorted);

				return $sorted;
		}
}
