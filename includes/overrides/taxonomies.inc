<?php

// add default overrides to the filter
add_filter('qw_overrides', 'qw_override_taxonomies');
add_filter('qw_pre_save', 'qw_override_taxonomies_pre_save', 10, 2);

function qw_override_taxonomies($overrides){

  $overrides['taxonomies'] = array(
    'title' => 'Taxonomies',
    'description' => 'Override term archive pages for an entire taxonomy.',
    'form_callback' => 'qw_override_taxonomies_form',
    'get_query_callback' => 'qw_override_taxonomies_get_query'
  );

  return $overrides;
}

/**
 * @param $options
 * @param $query_id
 *
 * @return mixed
 */
function qw_override_taxonomies_pre_save( $options, $query_id ) {

  if ( !empty( $options['override'] ) ) {
    // find overrides for this item
    foreach ( $options['override'] as $name => $override ){
      if ( $override['type'] == 'taxonomies' ) {
        $taxonomies = get_taxonomies(array('public'   => true,), 'objects');

        foreach( $taxonomies as $taxonomy ){
          $wp_option_name ='_qw_override_taxonomies__' . $taxonomy->name;

          // see if this taxonomy is meant to be overridden
          if ( isset( $override[ $taxonomy->name ] ) ) {
            // need to save overrides somewhere quickly accessible
            // eg, _qw_override_taxonomies__category = query_id
            update_option( $wp_option_name, $query_id);
          }
          else {
            delete_option( $wp_option_name );
          }
        }
      }
    }
  }
  return $options;
}

/**
 * @param $override
 */
function qw_override_taxonomies_form( $override ) {
  $taxonomies = get_taxonomies(array('public'   => true,), 'objects');
  ?>
  <p>Select which tags to override.</p>
  <div class="qw-checkboxes">
    <?php
    foreach ($taxonomies as $taxonomy) { ?>
      <label class="qw-query-checkbox">
        <input class="qw-js-title"
               type="checkbox"
               name="<?php print $override['form_prefix']; ?>[<?php print $taxonomy->name; ?>]"
               value="<?php print $taxonomy->name; ?>"
          <?php checked( isset( $override['values'][ $taxonomy->name ] ) ); ?> />
        <?php print $taxonomy->labels->name; ?>
      </label>
    <?php
    }
    ?>
  </div>
<?php
}

/**
 *
 */
function qw_override_taxonomies_get_query(){
  if ( is_archive() && ( is_tag() || is_category() || is_tag() )) {
    $term = get_queried_object();
    $query_id = get_option( '_qw_override_taxonomies__' . $term->taxonomy, FALSE );

    if ( $query_id ) {
      $type = 'taxonomy_' . $term->taxonomy;

      $options_override = array(
        // override the post title
        'display' => array(
          'title' => single_term_title( '', false ),
        ),
        // add a filter to the query for this term
        'args' => array(
          'filters' => array(
            $type  => array(
              'type'     => $type,
              'hook_key' => $type,
              'name'     => $type,
              'weight'   => 10,
              'terms'    => array( $term->term_id => $term->name ),
              'operator' => 'IN',
              'include_children' => true,
            ),
          )
        )
      );

      return new QW_Query( $query_id, $options_override );
    }
  }
}