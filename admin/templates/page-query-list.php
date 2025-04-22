<?php

/*
 * Taken From Custom List Table Exmaple
 *
 * http://wordpress.org/extend/plugins/custom-list-table-example/
 */

/*************************** LOAD THE BASE CLASS *******************************
 *******************************************************************************
 * The WP_List_Table class isn't automatically available to plugins, so we need
 * to check if it's available and load it if necessary.
 */
if ( ! class_exists( 'WP_List_Table' ) ) {
	require_once( ABSPATH . 'wp-admin/includes/class-wp-list-table.php' );
}


/************************** CREATE A PACKAGE CLASS *****************************
 *******************************************************************************
 * Create a new list table package that extends the core WP_List_Table class.
 * WP_List_Table contains most of the framework for generating the table, but
 * we
 * need to define and override some methods so that our data can be displayed
 * exactly the way we need it to be.
 *
 * To display this example on a page, you will first need to instantiate the
 * class, then call $yourInstance->prepare_items() to handle any data
 * manipulation, then finally call $yourInstance->display() to render the table
 * to the page.
 *
 * Our theme for this list table is going to be movies.
 */
class Query_Wrangler_List_Table extends WP_List_Table {
	/** ************************************************************************
	 * REQUIRED. Set up a constructor that references the parent constructor.
	 * We
	 * use the parent reference to set some default configs.
	 ***************************************************************************/
	function __construct() {
		global $status, $page;

		//Set parent defaults
		parent::__construct( array(
			'singular' => 'query',     //singular name of the listed records
			'plural'   => 'queries',    //plural name of the listed records
			'ajax'     => FALSE        //does this table support ajax?
		) );

	}


	/** ************************************************************************
	 * Recommended. This method is called when the parent class can't find a
	 * method specifically build for a given column. Generally, it's
	 * recommended to include one method for each column you want to render,
	 * keeping your package class neat and organized. For example, if the class
	 * needs to process a column named 'title', it would first see if a method
	 * named $this->column_title() exists - if it does, that method will be
	 * used. If it doesn't, this one will be used. Generally, you should try to
	 * use custom column methods as much as possible.
	 *
	 * Since we have defined a column_title() method later on, this method
	 * doesn't need to concern itself with any column with a name of 'title'.
	 * Instead, it needs to handle everything else.
	 *
	 * For more detailed insight into how columns are handled, take a look at
	 * WP_List_Table::single_row_columns()
	 *
	 * @param array $item A singular item (one full row's worth of data)
	 * @param array $column_name The name/slug of the column to be processed
	 *
	 * @return string Text or HTML to be placed inside the column <td>
	 **************************************************************************/
	function column_default( $item, $column_name ) {
		switch ( $column_name ) {
			case 'name':
				return '<a href="?page=query-wrangler&edit=' . $item['ID'] . '">' . $item['name'] . '</a>';
			case 'type':
				return ucfirst( $item[ $column_name ] );
			case 'details':
				$details = '';
				$settings = QW_Settings::get_instance();

				if ( $item['type'] != 'override' ) {
					if ( $settings->get('shortcode_compat') ){
						$details .= 'Shortcode options:<br />[qw_query id=' . $item['ID'] . ']<br />[qw_query slug="' . $item['slug'] . '"]';
					}
					else {
						$details .= 'Shortcode options:<br />[query id=' . $item['ID'] . ']<br />[query slug="' . $item['slug'] . '"]';
					}
				}

				if ( $item['type'] == 'override' ) {
					$details = 'Overriding: ';

					$row = qw_get_query_by_id( $item['ID'] );
					if ( isset( $row->data['override'] ) ) {
						$all_overrides = qw_all_overrides();
						foreach ( $row->data['override'] as $type => $values ) {

							if ( isset( $all_overrides[ $type ] ) ) {
								$override = $all_overrides[ $type ];

								$details .= '<br>'.$override['title'] . ': ';

								if ( is_array( $values['values'] ) ) {
									$details.= implode( ", ", $values['values'] );
								}
								else {
									$details.= ", ". $values['values'];
								}
							}
						}
					}

				}

				return $details;
			default:
				return print_r( $item,
					TRUE ); //Show the whole array for troubleshooting purposes
		}
	}


	/** ************************************************************************
	 * Recommended. This is a custom column method and is responsible for what
	 * is rendered in any column with a name/slug of 'title'. Every time the
	 * class needs to render a column, it first looks for a method named
	 * column_{$column_title} - if it exists, that method is run. If it doesn't
	 * exist, column_default() is called instead.
	 *
	 * This example also illustrates how to implement rollover actions. Actions
	 * should be an associative array formatted as 'slug'=>'link html' - and
	 * you
	 * will need to generate the URLs yourself. You could even ensure the links
	 *
	 *
	 * @see WP_List_Table::::single_row_columns()
	 *
	 * @param array $item A singular item (one full row's worth of data)
	 *
	 * @return string Text to be placed inside the column <td> (movie title
	 *     only)
	 **************************************************************************/
	function column_name( $item ) {
		$edit_url = esc_url( strtr( '?page=%page&edit=%id', [
			'%page' => $_REQUEST['page'],
			'%id' => $item['ID'],
		] ) );
		$export_url = esc_url( wp_nonce_url( strtr( '?page=%page&export=%id', [
			'%page' => $_REQUEST['page'],
			'%id' => $item['ID'],
		] ), 'qw-export_' . $item['ID'] ) );
		$delete_url = esc_url( wp_nonce_url( strtr( '?page=%page&noheader=true&action=delete&edit=%id', [
			'%page' => $_REQUEST['page'],
			'%id' => $item['ID'],
		] ), 'qw-delete_' . $item['ID'] ) );

		//Build row actions
		$actions = array(
			'edit'   => '<a href="' . $edit_url. '">Edit</a>',
			'export' => '<a href="' . $export_url . '">Export</a>',
			'delete' => '<a class="qw-delete-query" href="' . $delete_url . '">Delete</a>',
		);
		// pages
		if ( $item['type'] == 'page' ) {
			$view_url =  esc_url(get_bloginfo( 'wpurl' ) . '/' . $item['path']);
			$actions['view'] = '<a href="' . $view_url . '">View</a>';
		}

		//Return the title contents
		return '<a href="' . $edit_url . '">' . $item['name'] . '</a>
			<span style="color:silver">(ID: ' . $item['ID'] . ')</span> ' .
			$this->row_actions( $actions );
	}

	/** ************************************************************************
	 * REQUIRED if displaying checkboxes or using bulk actions! The 'cb' column
	 * is given special treatment when columns are processed. It ALWAYS needs
	 * to
	 * have it's own method.
	 *
	 * @see WP_List_Table::::single_row_columns()
	 *
	 * @param array $item A singular item (one full row's worth of data)
	 *
	 * @return string Text to be placed inside the column <td> (movie title
	 *     only)
	 **************************************************************************/
	function column_cb( $item ) {
		return '<input type="checkbox" name="' . esc_attr( $this->_args['singular'] . '[]' ) . '" value="' . esc_attr( $item['ID'] ) . '" />';
	}


	/** ************************************************************************
	 * REQUIRED! This method dictates the table's columns and titles. This
	 * should return an array where the key is the column slug (and class) and
	 * the value is the column's title text. If you need a checkbox for bulk
	 * actions, refer to the $columns array below.
	 *
	 * The 'cb' column is treated differently than the rest. If including a
	 * checkbox column in your table you must create a column_cb() method. If
	 * you don't need bulk actions or checkboxes, simply leave the 'cb' entry
	 * out of your array.
	 *
	 * @see WP_List_Table::::single_row_columns()
	 * @return array An associative array containing column information:
	 *     'slugs'=>'Visible Titles'
	 **************************************************************************/
	function get_columns() {
		$columns = array(
			'cb'      => '<input type="checkbox" />',
			//Render a checkbox instead of text
			'name'    => 'Name',
			'type'    => 'Type',
			'details' => 'Details'
		);

		return $columns;
	}

	/** ************************************************************************
	 * Optional. If you want one or more columns to be sortable (ASC/DESC
	 * toggle), you will need to register it here. This should return an array
	 * where the key is the column that needs to be sortable, and the value is
	 * db column to sort by. Often, the key and value will be the same, but
	 * this is not always the case (as the value is a column name from the
	 * database, not the list table).
	 *
	 * This method merely defines which columns should be sortable and makes
	 * them clickable - it does not handle the actual sorting. You still need
	 * to detect the ORDERBY and ORDER querystring variables within
	 * prepare_items() and sort your data accordingly (usually by modifying
	 * your query).
	 *
	 * @return array An associative array containing all the columns that
	 *     should be sortable: 'slugs'=>array('data_values',bool)
	 **************************************************************************/
	function get_sortable_columns() {
		$sortable_columns = array(
			'name' => array( 'name', TRUE ),     //true means its already sorted
			'type' => array( 'type', FALSE ),
		);

		return $sortable_columns;
	}


	/** ************************************************************************
	 * Optional. If you need to include bulk actions in your list table, this
	 * is
	 * the place to define them. Bulk actions are an associative array in the
	 * format
	 * 'slug'=>'Visible Title'
	 *
	 * If this method returns an empty value, no bulk action will be rendered.
	 * If you specify any bulk actions, the bulk actions box will be rendered
	 * with the table automatically on display().
	 *
	 * Also note that list tables are not automatically wrapped in <form>
	 * elements, so you will need to create those manually in order for bulk
	 * actions to function.
	 *
	 * @return array An associative array containing all the bulk actions:
	 *     'slugs'=>'Visible Titles'
	 **************************************************************************/
	function get_bulk_actions() {
		$actions = array(
			'delete' => 'Delete'
		);

		return $actions;
	}


	/** ************************************************************************
	 * Optional. You can handle your bulk actions anywhere or anyhow you prefer.
	 * For this example package, we will handle it in the class to keep things
	 * clean and organized.
	 *
	 * @see $this->prepare_items()
	 **************************************************************************/
	function process_bulk_action() {

		//Detect when a bulk action is being triggered...
		if ( 'delete' === $this->current_action() ) {
			if ( is_array( $_REQUEST['query'] ) ) {
				foreach ( $_REQUEST['query'] as $query_id ) {
					if ( is_numeric( $query_id ) ) {
						qw_delete_query( $query_id );
					}
				}
			}
			//wp_die('Items deleted (or they would be if we had items to delete)!');
			wp_redirect( get_bloginfo( 'wpurl' ) . '/wp-admin/admin.php?page=query-wrangler' );
		}

	}


	/** ************************************************************************
	 * REQUIRED! This is where you prepare your data for display. This method
	 * will usually be used to query the database, sort and filter the data,
	 * and generally get it ready to be displayed. At a minimum, we should set
	 * $this->items and
	 * $this->set_pagination_args(), although the following properties and
	 * methods are frequently interacted with here...
	 *
	 * @uses $this->_column_headers
	 * @uses $this->items
	 * @uses $this->get_columns()
	 * @uses $this->get_sortable_columns()
	 * @uses $this->get_pagenum()
	 * @uses $this->set_pagination_args()
	 **************************************************************************/
	function prepare_items() {

		/**
		 * First, lets decide how many records per page to show
		 */
		$per_page = 12;


		/**
		 * REQUIRED. Now we need to define our column headers. This includes a complete
		 * array of columns to be displayed (slugs & titles), a list of columns
		 * to keep hidden, and a list of columns that are sortable. Each of these
		 * can be defined in another method (as we've done here) before being
		 * used to build the value for our _column_headers property.
		 */
		$columns  = $this->get_columns();
		$hidden   = array();
		$sortable = $this->get_sortable_columns();


		/**
		 * REQUIRED. Finally, we build an array to be used by the class for column
		 * headers. The $this->_column_headers property takes an array which contains
		 * 3 other arrays. One for all columns, one for hidden columns, and one
		 * for sortable columns.
		 */
		$this->_column_headers = array( $columns, $hidden, $sortable );


		/**
		 * Optional. You can handle your bulk actions however you see fit. In this
		 * case, we'll handle them within our package just to keep things clean.
		 */
		$this->process_bulk_action();


		/**
		 * Instead of querying a database, we're going to fetch the example data
		 * property we created for use in this plugin. This makes this example
		 * package slightly different than one you might build on your own. In
		 * this example, we'll be using array manipulation to sort and paginate
		 * our data. In a real-world implementation, you will probably want to
		 * use sort and pagination data to build a custom query instead, as you'll
		 * be able to use your precisely-queried data immediately.
		 */
		//$data = $this->example_data;


		/**
		 * This checks for sorting input and sorts the data in our array accordingly.
		 *
		 * In a real-world situation involving a database, you would probably want
		 * to handle sorting by passing the 'orderby' and 'order' values directly
		 * to a custom query. The returned data will be pre-sorted, and this array
		 * sorting technique would be unnecessary.
		 */

		/***********************************************************************
		 * ---------------------------------------------------------------------
		 * vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
		 *
		 * In a real-world situation, this is where you would place your query.
		 *
		 * ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		 * ---------------------------------------------------------------------
		 **********************************************************************/
		global $wpdb;

		$sql = "SELECT `id` as `ID`, `type`, `name`, `slug`, `path` FROM {$wpdb->prefix}query_wrangler";
		$args = array();

		if ( !empty( $_REQUEST['s'] ) ){
			$sql.= " WHERE `name` LIKE %s";
			$args[] = '%' . $wpdb->esc_like( trim( $_REQUEST['s'] ) ) . '%';
		}

		// whitelist orderby
		$orderby = 'name';
		if ( ! empty( $_REQUEST['orderby'] ) &&
		     in_array( $_REQUEST['orderby'], array('name', 'type') ) ){
			$orderby = $_REQUEST['orderby'];
		}

		// whitelist order
		$order = 'ASC';
		if ( ! empty( $_REQUEST['order'] ) && strtolower( $_REQUEST['order'] ) == 'desc' ){
			$order = 'DESC';
		}

        $sql.= " ORDER BY %s {$order}";
		$args[] = $orderby;

		$sql = $wpdb->prepare( $sql, $args );

		$data = $wpdb->get_results( $sql, ARRAY_A );


		/**
		 * REQUIRED for pagination. Let's figure out what page the user is currently
		 * looking at. We'll need this later, so you should always include it in
		 * your own package classes.
		 */
		$current_page = $this->get_pagenum();

		/**
		 * REQUIRED for pagination. Let's check how many items are in our data array.
		 * In real-world use, this would be the total number of items in your database,
		 * without filtering. We'll need this later, so you should always include it
		 * in your own package classes.
		 */
		$total_items = count( $data );


		/**
		 * The WP_List_Table class does not handle pagination for us, so we need
		 * to ensure that the data is trimmed to only the current page. We can use
		 * array_slice() to
		 */
		$data = array_slice( $data,
			( ( $current_page - 1 ) * $per_page ),
			$per_page );


		/**
		 * REQUIRED. Now we can add our *sorted* data to the items property, where
		 * it can be used by the rest of the class.
		 */
		$this->items = $data;


		/**
		 * REQUIRED. We also have to register our pagination options & calculations.
		 */
		$this->set_pagination_args( array(
			'total_items' => $total_items,
			//WE have to calculate the total number of items
			'per_page'    => $per_page,
			//WE have to determine how many items to show on a page
			'total_pages' => ceil( $total_items / $per_page )
			//WE have to calculate the total number of pages
		) );
	}

}

/***************************** RENDER TEST PAGE ********************************
 *******************************************************************************
 * This function renders the admin page and the example list table. Although
 * it's possible to call prepare_items() and display() from the constructor,
 * there are often times where you may need to include logic here between those
 * steps, so we've instead called those methods explicitly. It keeps things
 * flexible, and it's the way the list tables are used in the WordPress core.
 */
function qw_list_queries_form() {

	//Create an instance of our package class...
	$ListTable = new Query_Wrangler_List_Table();
	//Fetch, prepare, sort, and filter our data...
	$ListTable->prepare_items();

	// if noheader is set, then we're bulk operating
	if ( ! isset( $_REQUEST['noheader'] ) ) {
		?>
		<div class="wrap">

			<div id="icon-tools" class="icon32"><br/></div>
			<h2>Query Wrangler <a class="add-new-h2"
			                      href="<?= esc_url('admin.php?page=qw-create' ) ?>">Add New</a>
			</h2>

			<form id="search-queries-filter" method="get">
				<input type="hidden" name="page" value="<?= esc_attr($_REQUEST['page']) ?>"/>
				<?php $ListTable->search_box( 'Search', 'post' ); ?>
			</form>

			<!-- Forms are NOT created automatically, so you need to wrap the table in one to use features like bulk actions -->
			<form id="queries-filter" method="get">
				<!-- For plugins, we also need to ensure that the form posts back to our current page -->
				<input type="hidden" name="page"
				       value="<?php echo esc_attr($_REQUEST['page']) ?>"/>
				<input type="hidden" name="noheader" value="true"/>
				<!-- Now we can render the completed list table -->
				<?php $ListTable->display() ?>
			</form>

		</div>
	<?php
	}
}
