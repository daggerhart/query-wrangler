<?php

// add default overrides to the filter
add_filter('qw_overrides', 'qw_override_tags');
add_filter('qw_pre_save', 'qw_override_tags_pre_save', 10, 2);

function qw_override_tags($overrides){

  $overrides['tags'] = array(
    'title' => 'Tags',
    'description' => 'Override output based on tags',
    'form_callback' => 'qw_override_tags_form',
    'get_query_callback' => 'qw_override_tags_get_query',
  );
  return $overrides;
}

/**
 * Save term relationships to the query_override_terms table
 *
 * @param $options
 * @param $query_id
 *
 * @return mixed
 */
function qw_override_tags_pre_save( $options, $query_id ){

  // no matter what, we delete all previous relationships
  global $wpdb;
  $table = $wpdb->prefix."query_override_terms";

  // get a list of term_ids from this taxonomy for pre-save deletion
  $terms = get_terms( 'category', array( 'hide_empty' => false ) );

  // delete all existing relationships
  foreach ( $terms as $term ){
    $wpdb->delete( $table, array(
      'query_id' => $query_id,
      'term_id' => $term->term_id,
    ));
  }

  // merge tags
  if( isset( $options['override']['tags'] ) &&
      is_array( $options['override']['tags'] ) )
  {
    // new terms to save
    $insert_terms = array_keys( $options['override']['tags'] );

    // loop through all terms and insert them
    foreach($insert_terms as $term_id){
      $wpdb->insert( $table, array(
        'query_id' => $query_id,
        'term_id' => $term_id,
      ));
    }
  }
  return $options;
}

/**
 * @param $override
 */
function qw_override_tags_form( $override ){
  $tags = get_terms( 'post_tag', array( 'hide_empty' => false ) );
  ?>
  <p>Select which tags to override.</p>
  <div class="qw-checkboxes">
    <?php
    foreach($tags as $tag)
    { ?>
      <label class="qw-query-checkbox">
        <input class="qw-js-title"
               type="checkbox"
               name="<?php print $override['form_prefix']; ?>[<?php print $tag->term_id; ?>]"
               value="<?php print $tag->name; ?>"
              <?php checked( isset( $override['values'][ $tag->term_id ] ) ); ?> />
        <?php print $tag->name; ?>
      </label>
    <?php
    }
    ?>
  </div>
  <?php
}

/**
 * @return bool|QW_Query
 */
function qw_override_tags_get_query(){
  if ( is_tag() && is_archive() ) {
    $term = get_queried_object();
    $query_id = qw_get_query_by_override_term( $term->term_id );

    if ( $query_id ) {

      $options_override = array(
        // override the post title
        'display' => array(
          'title' => single_term_title( '', false ),
        ),
        // add a filter to the query for this term
        'args' => array(
          'filters' => array(
            'tags' => array(
              'type' => 'tags',
              'tag_operator' => 'tag__in',
              'tags' => array( $term->term_id => $term->name ),
            )
          )
        )
      );

      return new QW_Query( $query_id, $options_override );
    }
  }

  return false;
}
