<?php
/*
 * Upgrade from 1.3 to 1.3.2beta
 */
function qw_upgrade_13_to_132(){
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