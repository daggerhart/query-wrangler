<?php

// add default overrides to the filter
add_filter('qw_overrides', 'qw_override___NAME__');

function qw_override___NAME__($overrides){

  $overrides['__NAME__'] = array(
    'title' => '__NAME__',
    'description' => 'Override output based on __NAME__',
    'form_callback' => 'qw_override___NAME___form',
  );
  return $overrides;
}

function qw_override___NAME___form($filter){

}
