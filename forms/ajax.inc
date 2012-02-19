<?php

/*
 * Ajax form templates
 */
function qw_form_ajax(){
  // sort
  if($_POST['form'] == 'sort_form'){
    $sorts = qw_all_sort_options();

    if (isset($_POST['hook_key'])){
      $sort = $sorts[$_POST['hook_key']];
    } else if (isset($_POST['type'])){
      $sort = $sorts[qw_get_hook_key($sorts, $_POST['type'])];
    }

    $sort['name'] = $_POST['name'];
    // set new form_prefix
    $sort['form_prefix'] = 'qw-query-options[args][sorts]['.$sort['name'].']';

    $args = array(
      'sort' => $sort,
    );
    print theme('query_sort', $args);
  }
  // sortable sort
  else if($_POST['form'] == 'sort_sortable'){
    $sorts = qw_all_sort_options();

    if (isset($_POST['hook_key'])){
      $sort = $sorts[$_POST['hook_key']];
    } else if (isset($_POST['type'])){
      $sort = $sorts[qw_get_hook_key($sorts, $_POST['type'])];
    }

    $sort['name'] = $_POST['name'];
    // set new form_prefix
    $sort['form_prefix'] = 'qw-query-options[args][sorts]['.$sort['name'].']';

    $args = array(
      'sort' => $sort,
      'weight' => $_POST['next_weight'],
    );
    print theme('query_sort_sortable', $args);
  }

  // field
  if($_POST['form'] == 'field_form'){
    $fields = qw_all_fields();

    if (isset($_POST['hook_key'])){
      $field = $fields[$_POST['hook_key']];
    } else if (isset($_POST['type'])){
      $field = $fields[qw_get_hook_key($fields, $_POST['type'])];
    }

    $field['name'] = $_POST['name'];
    // set new form_prefix
    $field['form_prefix'] = 'qw-query-options[display][field_settings][fields]['.$field['name'].']';

    $args = array(
      'image_sizes' => get_intermediate_image_sizes(),
      'file_styles' => qw_all_file_styles(),
      'field' => $field,
    );
    print theme('query_field', $args);
  }
  // sortable field
  else if($_POST['form'] == 'field_sortable'){
    $fields = qw_all_fields();

    if (isset($_POST['hook_key'])){
      $field = $fields[$_POST['hook_key']];
    } else if (isset($_POST['type'])){
      $field = $fields[qw_get_hook_key($fields, $_POST['type'])];
    }

    $field['name'] = $_POST['name'];
    // set new form_prefix
    $field['form_prefix'] = 'qw-query-options[display][field_settings][fields]['.$field['name'].']';

    $args = array(
      'field' => $field,
      'weight' => $_POST['next_weight'],
    );
    print theme('query_field_sortable', $args);
  }
  // filter
  else if($_POST['form'] == 'filter_form')
  {
    $filters = qw_all_filters();

    if (isset($_POST['hook_key'])){
      $filter = $filters[$_POST['hook_key']];
    } else if (isset($_POST['type'])){
      $filter = $filters[qw_get_hook_key($filters, $_POST['type'])];
    }

    $filter['name'] = $_POST['name'];
    // set new form_prefix
    $filter['form_prefix'] = "qw-query-options[args][filters][".$filter['name']."]";

    // filter's form
    // expect returned output
    if (function_exists($filter['form_callback'])){
      $filter['form'] = $filter['form_callback']($filter);
    }
    // provide template wrangler support
    else if (isset($filter['template'])){
      $filter['form'] = theme($filter['template'], array('filter' => $filter));
    }

    $args = array(
      'filter' => $filter,
      'query_type' => $_POST['query_type'],
    );

    print theme('query_filter', $args);
  }
  // sortable filter
  else if($_POST['form'] == 'filter_sortable') {
    $all_filters = qw_all_filters();

    if (isset($_POST['hook_key'])){
      $filter = $filters[$_POST['hook_key']];
    } else if (isset($_POST['type'])){
      $filter = $filters[qw_get_hook_key($filters, $_POST['type'])];
    }

    $filter['name'] = $_POST['name'];
    // set new form_prefix
    $filter['form_prefix'] = "qw-query-options[args][filters][".$filter['name']."]";

    $args = array(
      'filter' => $filter,
      'weight' => $_POST['next_weight'],
    );
    print theme('query_filter_sortable', $args);
  }
  // preview
  else if($_POST['form'] == 'preview') {
    $decode = urldecode($_POST['options']);
    $options = array();
    parse_str($decode, $options);
    $options['qw-query-options']['args']['paged'] = 1;
    $args = array(
      'options' => $options['qw-query-options']
    );
    print theme('query_preview', $args);
  }

  exit;
}