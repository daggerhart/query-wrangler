<?php
/*
 * Multi-version updates
 */
function qw_upgrade_13_to_current(){
  qw_upgrade_13_to_132();
}
function qw_upgrade_132_to_current(){
  qw_upgrade_132_to_14();
}
function qw_upgrade_14_to_current(){
  qw_upgrade_14_to_15();
}
function qu_upgrade_12_to_current(){
  qw_upgrade_12_to_13();
}

/*
 * Upgrade from 1.4 to 1.5
 */
function qw_upgrade_14_to_15()
{
  // set the edit theme
  update_option('qw_edit_theme', 'views');

  // get all queries
  global $wpdb;
  $table = $wpdb->prefix."query_wrangler";
  $sql = "SELECT * FROM ".$table;

  $rows = $wpdb->get_results($sql);

  // loop through queries
  foreach($rows as $query){
    $data = qw_unserialize($query->data);

    // create new sort based on  args[orderby], args[order]
    if (isset($data['args']['orderby']) &&
        isset($data['args']['order']))
    {
      $orderby = $data['args']['orderby']; unset($data['args']['orderby']);
      $order = $data['args']['order']; unset($data['args']['order']);

      $data['args']['sorts'] = array();
      $all_sorts = qw_all_sort_options();

      // loop and find right of all_sorts to use for data
      foreach($all_sorts as $hook_key => $sort){
        if ($orderby == $sort['type']){
          // set old sorting options as a new sort
          $data['args']['sorts'][$sort['type']] = array(
            'type' => $sort['type'],
            'hook_key' => $hook_key,
            'name' => $sort['type'],
            'weight' => 0,
            'order_value' => $order,
          );
          break;
        }
      }
    }

    $path = $query->path;

    // remove page path slashes
    if (substr($path, 0, 1) == "/"){
      $path = ltrim($path, '/');
    }
    // update old page path
    $data['display']['page']['path'] = $path;

    // save query
    $update = array(
      'path' => $path,
      'data' => qw_serialize($data),
    );
    $where = array(
      'id' => $query->id,
    );
    $wpdb->update($table, $update, $where);
  }
}

/*
 * Upgrade from 1.3.2 to 1.4
 */
function qw_upgrade_132_to_14()
{
  // get all queries
  global $wpdb;
  $table = $wpdb->prefix."query_wrangler";
  $sql = "SELECT * FROM ".$table;

  $rows = $wpdb->get_results($sql);

  // loop through queries
  foreach($rows as $query){
    $data = qw_unserialize($query->data);

    // display[type] = display[row_style]
    if (isset($data['type'])){
      $data['row_style'] = $data['type'];
      unset($data['type']);
    }

    // display[full_settings][style] = display[post_settings][size]
    if (isset($data['full_settings']['style'])){
      $data['post_settings']['size'] = $data['full_settings']['style'];
      unset($data['full_settings']['style']);
    }

    // filter post_status = args[post_status]
    if (empty($data['args']['post_status'])){
      if (isset($data['args']['filters']['post_status']['post_status'])){
        $data['args']['post_status'] = $data['args']['filters']['post_status']['post_status'];
      } else {
        $data['args']['post_status'] = 'publish';
      }
    }

    if (isset($data['args']['filters']['post_status'])){
      unset($data['args']['filters']['post_status']);
    }

    // save query
    $update = array(
      'data' => qw_serialize($data),
    );
    $where = array(
      'id' => $query->id,
    );
    $wpdb->update($table, $update, $where);
  }

  // continue upgrade chain
  qw_upgrade_14_to_15();
}
/*
 * Upgrade from 1.3 to 1.3.2beta
 */
function qw_upgrade_13_to_132()
{
  // get all queries
  global $wpdb;
  $table = $wpdb->prefix."query_wrangler";
  $sql = "SELECT * FROM ".$table;

  $rows = $wpdb->get_results($sql);

  //adjust arguments to filter values
  foreach($rows as $query){
    $data = qw_unserialize($query->data);

    // convert field settings style to style
    if(isset($data['display']['field_settings']['style'])){
      $data['display']['style'] = $data['display']['field_settings']['style'];
    }
    // convert 'full' type to 'posts'
    if($data['display']['type'] == 'full'){
      $data['display']['type'] = 'posts';
    }

    $update = array(
      'data' => qw_serialize($data),
    );
    $where = array(
      'id' => $query->id,
    );
    $wpdb->update($table, $update, $where);
  }

  // next upgrade
  qw_upgrade_132_to_14();
}

/*
 * Upgrade from 1.2 (pre-plugin version option) to 1.3 (first version option)
 */
function qw_upgrade_12_to_13(){
  // get all queries
  global $wpdb;
  $table = $wpdb->prefix."query_wrangler";
  $sql = "SELECT * FROM ".$table;

  $rows = $wpdb->get_results($sql);

  //adjust arguments to filter values
  foreach($rows as $query){
    $data = qw_unserialize($query->data);
    $args = $data['args'];

    $filter_weight = 0;

    // post status
    if(isset($args['post_status'])){
      $args['filters']['post_status'] = array(
        'type' => 'post_status',
        'post_status' => $args['post_status'],
        'weight' => $filter_weight,
      );
      $filter_weight++;
      unset($args['post_status']);
    }

    // post parent
    if(isset($args['post_parent'])){
      if($args['post_parent'] != ''){
        $args['filters']['post_status'] = array(
          'type' => 'post_status',
          'post_status' => $args['post_status'],
          'weight' => $filter_weight,
        );
        $filter_weight++;
      }
      unset($args['post_parent']);
    }

    // categories
    if(isset($args['cat'])){
      $args['filters']['categories'] = array(
        'type' => 'categories',
        'cats' => $args['cat'],
        'weight' => $filter_weight,
        'cat_operator' => $args['cat_operator'],
      );
      $filter_weight++;
      unset($args['cat']);
    }

    // tags
    if(isset($args['tag'])){
      $args['filters']['tags'] = array(
        'type' => 'tags',
        'tags' => $args['tag'],
        'weight' => $filter_weight,
        'tag_operator' => $args['tag_operator'],
      );
      $filter_weight++;
      unset($args['tag']);
    }

    // post_types
    if(isset($args['post_types'])){
      $args['filters']['post_types'] = array(
        'type' => 'post_types',
        'post_types' => $args['post_types'],
        'weight' => $filter_weight,
      );
      $filter_weight++;
      unset($args['post_types']);
    }
    unset($args['cat_operator']);
    unset($args['tag_operator']);

    $data['args'] = $args;

    $update = array(
      'data' => serialize($data),
    );
    $where = array(
      'id' => $query->id,
    );
    $wpdb->update($table, $update, $where);
  }

  // continue upgrading
  qw_upgrade_13_to_132();

}