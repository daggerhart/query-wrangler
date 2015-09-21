<?php
add_filter( 'qw_handlers', 'qw_handlers_default' );
add_filter( 'tw_templates', 'qw_handlers_templates' );

/*
 * Default handlers
 */
function qw_handlers_default( $handlers ) {
	$handlers['field']    = array(
		'title'            => 'Field',
		'description'      => 'Select Fields to add to this query output.',
		'data_callback'    => 'qw_handler_field_data',
		'all_callback'     => 'qw_all_fields',
		// relative to base
		'form_prefix'      => '[display][field_settings][fields]',
		'wrapper_template' => 'query_field',
	);
	$handlers['sort']     = array(
		'title'            => 'Sort Option',
		'description'      => 'Select options for sorting the query results.',
		'data_callback'    => 'qw_handler_sort_data',
		'all_callback'     => 'qw_all_sort_options',
		// relative to base
		'form_prefix'      => '[args][sorts]',
		'wrapper_template' => 'query_sort',
	);
	$handlers['filter']   = array(
		'title'            => 'Filter',
		'description'      => 'Select filters to affect the query results.',
		'data_callback'    => 'qw_handler_filter_data',
		'all_callback'     => 'qw_all_filters',
		// relative to base
		'form_prefix'      => '[args][filters]',
		'wrapper_template' => 'query_filter',
	);
	$handlers['override'] = array(
		'title'            => 'Override',
		'description'      => 'Select overrides to affect the query results based on the context of where the query is displayed.',
		'data_callback'    => 'qw_handler_override_data',
		'all_callback'     => 'qw_all_overrides',
		// relative to base
		'form_prefix'      => '[override]',
		'wrapper_template' => 'query_override',
	);

	return $handlers;
}

/**
 * Filter to add handler wrapper templates to tw
 *
 * @param $templates
 *
 * @return mixed
 */
function qw_handlers_templates( $templates ) {
	$handlers = qw_all_handlers();

	foreach ( $handlers as $type => $handler ) {
		// wrapper edit form
		$templates[ $handler['wrapper_template'] ] = array(
			'files'        => 'admin/templates/handler-' . $type . '.php',
			'default_path' => QW_PLUGIN_DIR,
		);

		// all handler items
		$all = $handler['all_callback']();

		// look for templates within all items
		foreach ( $all as $type => $item ) {
			// form template
			if ( isset( $item['form_template'] ) ) {
				$templates[ $item['form_template'] ] = array(
					'arguments' => array(
						$type => array()
					)
				);
			}
		}
	}

	return $templates;
}

/*
 * Simple functions for getting handler data from the query options
 */
function qw_handler_field_data( $options ) {
	if ( isset( $options['display']['field_settings']['fields'] ) ) {
		return $options['display']['field_settings']['fields'];
	}
}

function qw_handler_sort_data( $options ) {
	if ( isset( $options['args']['sorts'] ) ) {
		return $options['args']['sorts'];
	}
}

function qw_handler_filter_data( $options ) {
	if ( isset( $options['args']['filters'] ) ) {
		return $options['args']['filters'];
	}
}

function qw_handler_override_data( $options ) {
	if ( isset( $options['override'] ) ) {
		return $options['override'];
	}
}

/*
 * Organize an existing filters and give it all the data they needs
 *
 * @param $type
 * handler-type = 'filter', 'field', 'sort'
 */
function qw_preprocess_handlers( $options ) {
	// build handlers data
	$handlers = qw_all_handlers();
	// Retrieve the handler items from the query array
	foreach ( $handlers as $k => $handler ) {
		if ( function_exists( $handler['data_callback'] ) ) {
			$handlers[ $k ]['items'] = $handler['data_callback']( $options );
		}
	}

	foreach ( $handlers as $type => $handler ) {
		// load all our default handlers
		if ( is_array( $handler['items'] ) ) {
			$all = $handler['all_items'];

			// generate the form name prefixes
			foreach ( $handler['items'] as $name => $values ) {
				// load sort type data
				$hook_key = qw_get_hook_key( $all, $values );

				if ( empty( $hook_key ) ) {
					$hook_key = ! empty( $values['hook_key'] ) ? $values['hook_key'] : $name;
				}

				$this_item = $all[ $hook_key ];

				// copy type, hook_key, and weight to top level of array
				$this_item['name']        = $name;
				$this_item['type']        = ! empty( $values['type'] ) ? $values['type'] : $name;
				$this_item['weight']      = ! empty( $values['weight'] ) ? $values['weight'] : 0;
				$this_item['hook_key']    = $hook_key;
				$this_item['form_prefix'] = QW_FORM_PREFIX . $handler['form_prefix'] . '[' . $name . ']';

				// values are own array
				$this_item['values'] = $values;

				qw_handler_make_form( $this_item );

				// set new item
				$handlers[ $type ]['items'][ $name ] = $this_item;
			}
		} else {
			// empty array by default
			$handlers[ $type ]['items'] = array();
		}

		// sort according to weight
		if ( is_array( $handlers[ $type ]['items'] ) ) {
			uasort( $handlers[ $type ]['items'], 'qw_cmp' );
		}
	}

	return $handlers;
}

/*
 * Look for handler forms and settings forms and execute the callbacks
 */
function qw_handler_make_form( &$handler ) {
	// this handler's form
	if ( isset( $handler['form_callback'] ) && function_exists( $handler['form_callback'] ) ) {
		ob_start();
		$handler['form_callback']( $handler );
		$handler['form'] = ob_get_clean();
	} // provide template wrangler support
	else if ( isset( $handler['form_template'] ) ) {
		$handler['form'] = theme( $handler['form_template'],
			array( 'this' => $handler ) );
	}
	/*
	  // see if item has an exposed settings form
	  if (isset($handler['exposed_settings_form_callback']) && function_exists($handler['exposed_settings_form_callback'])) {
		ob_start();
		  $handler['exposed_settings_form_callback']($handler);
		$handler['exposed_settings_form'] = ob_get_clean();
	  }
	  // provide template wrangler support
	  else if (isset($handler['exposed_settings_form_template'])){
		$handler['exposed_settings_form'] = theme($handler['exposed_settings_form_template'], array('this' => $handler));
	  }
	*/
	// Contextual Filter override form
	// see if item has an exposed settings form
	if ( isset( $handler['override_form_callback'] ) && function_exists( $handler['override_form_callback'] ) ) {
		ob_start();
		$handler['override_form_callback']( $handler );
		$handler['override_form'] = ob_get_clean();
	} // provide template wrangler support
	else if ( isset( $handler['override_form_template'] ) ) {
		$handler['override_form'] = theme( $handler['override_form_template'],
			array( 'this' => $handler ) );
	}
}