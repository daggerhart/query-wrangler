<?php
// hook into qw_basics
add_filter( 'qw_basics', 'qw_basic_settings_pager' );

// add default pager types
add_filter( 'qw_pager_types', 'qw_default_pager_types', 0 );

/*
 * Basic Settings
 */
function qw_basic_settings_pager( $basics ) {
	$basics['pager'] = array(
		'title'         => 'Pager',
		'option_type'   => 'display',
		'description'   => 'Select which type of pager to use.',
		'form_callback' => 'qw_basic_pager_form',
		'weight'        => 0,
	);

	return $basics;
}

/*
 * Setup pager types
 */
function qw_default_pager_types( $pagers ) {
	$pagers['default'] = array(
		'title'    => 'Default',
		'callback' => 'qw_theme_pager_default'
	);
	$pagers['numbers'] = array(
		'title'    => 'Page Numbers',
		'callback' => 'qw_theme_pager_numbers'
	);

	// WP PageNavi Plugin
	if ( function_exists( 'wp_pagenavi' ) ) {
		$pagers['pagenavi'] = array(
			'title'    => 'PageNavi',
			'callback' => 'wp_pagenavi'
		);
	}

	return $pagers;
}

function qw_basic_pager_form( $basic, $display ) {
	$pager_types    = qw_all_pager_types();
	$use_pager      = isset( $display['page']['pager']['active'] ) ? 'checked="checked"' : '';
	$pager_previous = isset( $display['page']['pager']['previous'] ) ? $display['page']['pager']['previous'] : "";
	$pager_next     = isset( $display['page']['pager']['next'] ) ? $display['page']['pager']['next'] : "";
	//$use_pager_key  = isset($display['page']['pager']['use_pager_key']) ? 'checked="checked"': '';
	//$pager_key     = isset($display['page']['pager']['pager_key']) ? $display['page']['pager']['pager_key']: "";
	?>
	<label class='qw-field-checkbox'>
		<input class='qw-js-title'
		       type='checkbox'
		       name="qw-query-options[display][page][pager][active]"
			<?php print $use_pager;?> />
		Use Pagination
	</label>

	<select class='qw-js-title'
	        name="<?php print $basic['form_prefix']; ?>[page][pager][type]">
		<?php
		foreach ( $pager_types as $pager_name => $pager_options ) {
			$selected = ( $display['page']['pager']['type'] == $pager_name ) ? 'selected="selected"' : '';
			?>
			<option value="<?php echo $pager_name; ?>"
				<?php echo $selected; ?>>
				<?php echo $pager_options['title']; ?>
			</option>
		<?php
		}
		?>
	</select>
	<p>
		Use the following options to change the Default Pager labels.
	</p>
	<strong>Previous Page Label:</strong>
	<p>
		<input class='qw-js-title'
		       type="text"
		       name="<?php print $basic['form_prefix']; ?>[page][pager][previous]"
		       value="<?php print $pager_previous; ?>"/>
	</p>
	<strong>Next Page Label:</strong>
	<p>
		<input class='qw-js-title'
		       type="text"
		       name="<?php print $basic['form_prefix']; ?>[page][pager][next]"
		       value="<?php print $pager_next; ?>"/>
	</p>
	<?php /*
    <strong>Pager Key:</strong>
    <p class="description">Use this if you need multiple paginating querys on a single wordpress page.</p>
    <p>
      <label>
        <input type="checkbox"
               name="<?php print $basic['form_prefix']; ?>[page][pager][use_pager_key]"
               <?php print $use_pager_key; ?> />
        Use pager key
      </label>
    </p>
    <p class="description">Pager key should a unique string of lowercase characters with underscores. No spaces.</p>
    <p>
      <input type="text"
             name="<?php print $basic['form_prefix']; ?>[page][pager][pager_key]"
             value="<?php print $pager_key; ?>" />
    </p>
    */ ?>
<?php
}

/*
 * Custom Pager function
 *
 * @param array $pager Query pager details
 * @param object $qw_query Object
 * @return HTML processed pager
 */
function qw_make_pager( $pager, &$qw_query ) {
	$pager_themed = '';
	$pagers       = qw_all_pager_types();

	//set callback if function exists
	if ( function_exists( $pagers[ $pager['type'] ]['callback'] ) ) {
		$callback = $pagers[ $pager['type'] ]['callback'];
	} else {
		$callback = $pagers['default']['callback'];
	}

	// execute callback
	$pager_themed = call_user_func_array( $callback,
		array( $pager, $qw_query ) );

	return $pager_themed;
}

/*
 * Custom Default Pager
 *
 * @param array $pager Query options for pager
 * @param object $qw_query Object
 */
function qw_theme_pager_default( $pager, &$qw_query ) {
	// help figure out the current page
	$exposed_path_array = explode( '?', $_SERVER['REQUEST_URI'] );
	$path_array         = explode( '/page/', $exposed_path_array[0] );

	$exposed_path = NULL;
	if ( isset( $exposed_path_array[1] ) ) {
		$exposed_path = $exposed_path_array[1];
	}

	$pager_themed      = '';
	$pager['next']     = ( $pager['next'] ) ? $pager['next'] : 'Next Page &raquo;';
	$pager['previous'] = ( $pager['previous'] ) ? $pager['previous'] : '&laquo; Previous Page';

	if ( $page = qw_get_page_number( $qw_query ) ) {
		$path = rtrim( $path_array[0], '/' );

		$wpurl = get_bloginfo( 'wpurl' );

		// previous link with page number
		if ( $page >= 3 ) {
			$url = $wpurl . $path . '/page/' . ( $page - 1 );
			if ( $exposed_path ) {
				$url .= '?' . $exposed_path;
			}
			$pager_themed .= '<div class="query-prevpage">
                        <a href="' . $url . '">' . $pager['previous'] . '</a>
                      </div>';
		} // previous link with no page number
		else if ( $page == 2 ) {
			$url = $wpurl . $path;
			if ( $exposed_path ) {
				$url .= '?' . $exposed_path;
			}
			$pager_themed .= '<div class="query-prevpage">
                        <a href="' . $url . '">' . $pager['previous'] . '</a>
                      </div>';
		}

		// next link
		if ( ( $page + 1 ) <= $qw_query->max_num_pages ) {
			$url = $wpurl . $path . '/page/' . ( $page + 1 );
			if ( $exposed_path ) {
				$url .= '?' . $exposed_path;
			}
			$pager_themed .= '<div class="query-nextpage">
                        <a href="' . $url . '">' . $pager['next'] . '</a>
                      </div>';
		}

		return $pager_themed;
	}
}

/*
 * Default Pager with page numbers
 *
 * @param array $pager Query options for pager
 * @param object $qw_query Object
 *
 * @return string HTML for pager
 */
function qw_theme_pager_numbers( $pager, $qw_query ) {
	$big          = intval( $qw_query->found_posts . '000' );
	$args         = array(
		'base'    => str_replace( $big, '%#%', get_pagenum_link( $big ) ),
		'format'  => '?paged=%#%',
		'current' => max( 1, qw_get_page_number( $qw_query ) ),
		'total'   => $qw_query->max_num_pages
	);
	$pager_themed = paginate_links( $args );

	return $pager_themed;
}
