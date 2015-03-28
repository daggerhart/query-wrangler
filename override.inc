<?php

class QW_Override {

  private $override_executed = FALSE;

  function __construct() {
    add_action( 'template_redirect', array( $this, 'action_template_redirect' ) );
  }

  function action_template_redirect(){
    if ( $this->is_override_page() ){

    }
  }

  function is_override_page(){
    if ( is_category() ) {

    }
  }
}