<?php
/*
 * Template the entire query
 * 
 * @param array $wp_query Wordpress query object
 * @param array $options the query options
 * @return string HTML for themed/templated query
 */
function qw_template_query($wp_query, $options)
{
  $results_count = count($wp_query->posts);
  $content = '';
  
  // look for empty results
  if ($results_count > 0)
  {
    // see if page needs pagination
    //if ($results_count < $options['args']['posts_per_page']){
    //  $options['meta']['pagination'] = false;
    //}
    
    // the content of the widget is the result of the query
    if($options['display']['type'] == "full"){
      $content = qw_template_query_rows($options, $wp_query);
    }
    else if ($options['display']['type'] == "fields")
    {
      // run style function
      $field_rows = qw_make_fields_rows($wp_query, $options['display']);
      $content = qw_template_query_rows($options, $field_rows);
    }
  }
  // empty results  
  else {
    // no pages
    $options['meta']['pagination'] = false;
    $content = '<div class="query-empty">'.$options['meta']['empty'].'</div>';
  }
  
  $wrapper_classes = array();
  $wrapper_classes[] = 'query';
  $wrapper_classes[] = 'query-'.$options['meta']['slug'].'-wrapper';
  $wrapper_classes = implode(" ", $wrapper_classes);
  
  // header
  if($options['meta']['header'] != '') {
    $header = $options['meta']['header'];
  }
  // footer
  if($options['meta']['footer'] != '') {
    $footer = $options['meta']['footer'];
  }
    
  // pagination
  if($options['meta']['pagination'] && isset($options['display']['page']['pager']['active'])){
    $pager = qw_make_pager($options['display']['page']['pager'], $wp_query);
    $pager_classes = array();
    $pager_classes[] = 'query-pager';
    $pager_classes[] = 'pager-'.$options['display']['page']['pager']['type'];
    $pager_classes = implode(" ", $pager_classes);
  }
  
  // template the widget
  ob_start();
    // look for slug specific template in theme folder
    // eg.  query-test_query-wrapper.php
    if (file_exists(TEMPLATEPATH . "/query-".$options['meta']['slug']."-wrapper.php")){
      include TEMPLATEPATH . "/query-".$options['meta']['slug']."-wrapper.php";
    }
    // look for generic  template in theme folder
    // eg.  query-unformatted.php
    else if (file_exists(TEMPLATEPATH . "/query-wrapper.php")){
      include TEMPLATEPATH . "/query-wrapper.php";
    }
    // fallback on default template
    else{
      include QW_PLUGIN_DIR. '/templates/query-wrapper.php';
    }
    $themed = ob_get_clean();
  
  return $themed;
  
}
/*
 * Build array of fields and rows for templating
 * 
 * @param object $new_query WP_Query objecte generated
 * @param array $display Query display data
 * @return array Executed query rows
 */
function qw_make_fields_rows($new_query, $display)
{
  $all_fields = qw_all_fields();
  $rows = array();
  $tokens = array();
  
  // loop through each post
  $i = 0;
  while($new_query->have_posts())
  {
    $new_query->the_post();
    //  
    $this_post = $new_query->post;
    $rows[$i] = array();
    
    // make row classes
    $row_classes   = array('qw-query-row');
    $row_classes[] = ($i%2) ? 'qw-odd' : 'qw-even';
    $row_classes[] = 'qw-row-'.$i;
    
    $rows[$i]['row_classes'] = implode(" ", $row_classes);
    
    // sort according to weights
    uasort($display['field_settings']['fields'],'qw_cmp');
    
    // loop through each field
    foreach($display['field_settings']['fields'] as $field_name => $field_settings)
    {
      if(!$field_settings['exclude_display']){
        // field open
        $field_classes = array('qw-field');
        $field_classes[] = 'qw-field-'.$field_settings['name'];
        
        $rows[$i]['fields'][$field_name]['classes'] = implode(" ",$field_classes);
          
        // get field details from all fields list
        $field_defaults = $all_fields[$field_settings['type']];

        // look for callback
        if(isset($field_defaults['callback'])){
          // meta keys
          if($field_defaults['callback'] == 'get_post_meta'){
            $rows[$i]['fields'][$field_name]['output'] = $field_defaults['callback']($this_post->ID, $field_defaults['arguments']['key'], true);
          }
          // image fields
          else if($field_defaults['callback'] == 'qw_theme_image'){
            $rows[$i]['fields'][$field_name]['output'] = $field_defaults['callback']($this_post->ID, $field_settings['image_display_style'], $field_settings['image_display_count']);
          }
          // file fields
          else if($field_defaults['callback'] == 'qw_theme_file'){
            $rows[$i]['fields'][$field_name]['output'] = $field_defaults['callback']($this_post->ID, $field_settings['file_display_style'], $field_settings['file_display_count']);
          }          
          // TODO: a generic method for using an array to execute a funtion
          // other callbacks
          //else if (is_array($field['args'])){
          //  $rows[$i]['fields'][$field_name]['output'] = call_user_func_array($field['callback'](), array_values($fields['args']));
          //}        
          // normal callback
          else { 
            $rows[$i]['fields'][$field_name]['output'].= $field_defaults['callback']();
          }
        }
        // use field itself
        else {
          $rows[$i]['fields'][$field_name]['output'].= $this_post->{$field_settings['type']};
        }
        
        // add token for replace
        $tokens['{{'.$field_name.'}}'] = $rows[$i]['fields'][$field_name]['output'];
        
        // look for rewrite output
        if(isset($field_settings['rewrite_output'])){
          // replace tokens with results
          $field_settings['custom_output'] = str_replace(array_keys($tokens), array_values($tokens), $field_settings['custom_output']);
          // update output
          /*
          ob_start();
            eval('?>'.$field_settings['custom_output']);
          $rows[$i]['fields'][$field_name]['output'] = ob_get_clean();
          */
          $rows[$i]['fields'][$field_name]['output'] = $field_settings['custom_output'];
        }
        
        // apply link to field
        if(isset($field_settings['link'])){
          $rows[$i]['fields'][$field_name]['output'] = '<a class="query-field-link" href="'.get_permalink().'">'.$rows[$i]['fields'][$field_name]['output'].'</a>';
        }
        
        // get default field label for tables
        $rows[$i]['fields'][$field_name]['label'] = ($field_settings['has_label']) ? $field_settings['label'] : '';
        
        // apply labels to full style fields
        if(isset($field_settings['has_label']) &&
           $display['type'] != 'full' &&
           $display['field_settings']['style'] != 'table')
        {
          $rows[$i]['fields'][$field_name]['output'] = '<label class="query-label">'.$field_settings['label'].'</label>'.$rows[$i]['fields'][$field_name]['output'];
        }
      }
    }
    // increment row
    $i++;
  }
  
  return $rows;
}
/*
 * Look for possible custom templates, then default to query-style.php
 * 
 * @param string $style Query style
 * @param array $rows Generated rows
 * @return string Templated query
 */ 
function qw_template_query_rows($options, $rows = array())
{
  if($options['display']['type'] == 'full'){
    $style = $options['display']['full_settings']['style'];    
  } else {
    $style = $options['display']['field_settings']['style'];      
  }
  $slug = $options['meta']['slug'];
  
  ob_start();
  
  // look for slug specific template in theme folder
  // eg.  query-test_query-unformatted.php
  if (file_exists(TEMPLATEPATH . "/query-".$slug."-".$style.".php")){
    include TEMPLATEPATH . "/query-".$slug."-".$style.".php";
  }
  // look for generic  template in theme folder
  // eg.  query-unformatted.php
  else if (file_exists(TEMPLATEPATH . "/query-".$style.".php")){
    include TEMPLATEPATH . "/query-".$style.".php";
  }
  // fallback on default template
  else{
    include QW_PLUGIN_DIR. '/templates/query-'.$style.'.php';
  }
  $templated = ob_get_clean();
  
  return $templated;
}

/*
 * Custom Pager function
 * 
 * @param array $pager Query pager details
 * @param object $wp_query Object
 * @return HTML processed pager
 */
function qw_make_pager($pager, $wp_query)
{
  $pager_themed = '';
  switch($pager['type'])
  {
    case 'default':
      $pager_themed = qw_theme_pager_default($pager, $wp_query);
      break;
  }
  return $pager_themed;
}
/*
 * Custom Default Pager
 *
 * @param array $pager Query options for pager
 * @param object $wp_query Object
 */
function qw_theme_pager_default($pager, $wp_query)
{
  $pager_themed = '';
  $pager['next'] = ($pager['next']) ? $pager['next'] : 'Next Page &raquo;';
  $pager['previous'] = ($pager['previous']) ? $pager['previous'] : '&laquo; Previous Page';

  // figure out the current page
  $path_array = explode('/page/', $_SERVER['REQUEST_URI']);

  // look for WP paging first
  if($wp_query->query_vars['paged']){
    $page = $wp_query->query_vars['paged'];
  }
  // paging with slashes
  else if(isset($path_array[1])) {
    $page = explode('/', $path_array[1]);
    $page = $page[0];
  }
  // paging with get variable
  else if (isset($_GET['page'])) {
    $page = $_GET['page'];
  }
  
  if(isset($page))
  {
    // next link
    if(($page+1) <= $wp_query->max_num_pages){
      $pager_themed.= '<div class="query-nextpage">
                      <a href="'.get_bloginfo('wpurl').$path_array[0].'/page/'.($page+1).'">'.$pager['next'].'</a>
                    </div>';
    }
    
    // previous link with page number
    if($page >= 3){
      $pager_themed.= '<div class="query-prevpage">
                    <a href="'.get_bloginfo('wpurl').$path_array[0].'/page/'.($page-1).'">'.$pager['previous'].'</a>
                  </div>';
    }
    // previous link with no page number
    else if ($page == 2)
    {
      $pager_themed.= '<div class="query-prevpage">
                    <a href="'.get_bloginfo('wpurl').$path_array[0].'">'.$pager['previous'].'</a>
                  </div>';
    }
    
    return $pager_themed;
  }
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
function qw_theme_image($post_id, $image_size, $count = 0)
{
  $images = qw_get_post_images($post_id);
  if(is_array($images)){
    $output = '';
    $i = 0;
    foreach($images as $image){
      if($count == 0 || ($i < $count)){
        $output.= wp_get_attachment_image($image->ID, $image_size);
      }
      $i++;
    }
    return $output;
  }
}
/*
 * Get all images attached to a single post
 * 
 * @param $post_id
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