var QueryWrangler = {};

(
    function ( $ ) {

      QueryWrangler = {
        // this query specific data
        query: {},

        // results from qw_all hooks. All handlers, styles, etc
        data: {},

        // array of available options for looping
        handlers: ['field', 'filter', 'sort', 'override'],

        // changes have been made
        changes: false,

        /**
         * Init core QW UI components
         */
        init: function ( query_id ) {
          QueryWrangler.query.id = query_id;

          // hide message that says this query needs saving
          $( '.qw-changes' ).hide();

          // bind preview button click
          $( '#get-preview' ).click( QueryWrangler.ajax.getPreview );

          // show initial preview
          if ( $( '#live-preview' ).prop( 'checked' ) ) {
            QueryWrangler.ajax.getPreview();
          }

          // provide meta_value_field key suggestions
          //$('body').on( 'keyup', '.qw-meta-value-key-autocomplete', QueryWrangler.ajax.meta_value_key_autocomplete );
          $( '.qw-meta-value-key-autocomplete' ).autocomplete( {
            source: QueryWrangler.ajax.metaValueKeySearch
          } );

          // preview data accordions
          $( '#query-details' ).accordion( {
            header: '> div > .qw-setting-header',
            heightStyle: "content",
            collapsible: true,
            active: false,
            autoHeight: false
          } );

          // get our core hook data
          QueryWrangler.ajax.getQwData();
          QueryWrangler.generateFieldTokens();
        },

        /**
         * Make and output tokens for fields
         */
        generateFieldTokens: function () {
          var tokens = [];

          $( '#query-fields' ).find( '.qw-field-name' ).each( function (
              i,
              element
          ) {
            // field name
            var field_name = $( element ).val();

            // add tokens
            tokens.push( '<li>{{' + field_name + '}}</li>' );

            // target the field and insert tokens
            $( '#qw-field-' + field_name + ' ul.qw-field-tokens-list' ).html( tokens.join( "" ) );
          } );
        }
      };


      /**
       * AJAX methods & helpers
       */
      QueryWrangler.ajax = {
        /**
         * Submit an ajax call via POST
         * @param post_data_form
         * @param callback
         */
        post: function ( post_data_form, callback ) {
          // ajax call to get form
          jQuery.ajax( {
            url: $( 'form#qw-edit-query-form' ).data( 'ajax-url' ),
            type: 'POST',
            async: false,
            data: post_data_form,
            dataType: 'json',
            success: callback
          } );
        },

        /**
         * Get QW hook and query data
         */
        getQwData: function () {
          var post_data = {
            action: 'qw_data_ajax',
            data: 'all_hooks',
            queryId: $( 'form#qw-edit-query-form' ).data( 'query-id' )
          };
          QueryWrangler.ajax.post( post_data, function ( results ) {
            QueryWrangler.data = JSON.parse( results );
          } );
        },

        /**
         * jQuery ui autocomplete{ source: } callback for meta_keys
         */
        metaValueKeySearch: function ( request, response ) {
          QueryWrangler.ajax.post( {
                action: 'qw_meta_key_autocomplete',
                qw_meta_key_autocomplete: request.term
              },
              // success callback
              function ( result ) {
                if ( typeof result.values !== 'undefined' ) {
                  response( result.values );
                }
              } );
        },

        /**
         * Preview
         */
        getPreview: function () {
          var $preview_controls = $( '#query-preview-controls' );

          // show throbber
          $preview_controls.removeClass( 'query-preview-inactive' ).addClass( 'query-preview-active' );

          // prepare post data
          var post_data_form = {
            'action': 'qw_form_ajax',
            'form': 'preview',
            'options': $( 'form#qw-edit-query-form' ).serialize(),
            'query_id': QueryWrangler.query.id
          };

          // make ajax call
          QueryWrangler.ajax.post( post_data_form, function ( results ) {
            $( '#query-preview-target' ).html( results.preview );
            $( '#qw-show-arguments-target' ).html( results.args );
            $( '#qw-show-php_wpquery-target' ).html( results.php_wpquery );
            $( '#qw-show-display-target' ).html( results.display );
            $( '#qw-show-options-target' ).html( results.options );
            $( '#qw-show-wpquery-target' ).html( results.wpquery );
            $( '#qw-show-templates-target' ).html( results.templates );
          } );

          // hide throbber
          $preview_controls.removeClass( 'query-preview-active' ).addClass( 'query-preview-inactive' );
        },

        /**
         *
         * @param handler - text name of handler: field, filter, sort
         * @param item_type - handler item type.  a specific name for the handler
         * @param hook_key - handler item's key
         * @param callback - success callback
         */
        getHandlerItemTemplate: function (
            handler,
            item_type,
            hook_key,
            callback
        ) {
          var item_count = $( '#query-' + handler + 's input.qw-' + handler + '-type[value=' + item_type + ']' ).length;

          // prepare post data for form and sortable form
          var post_data_form = {
            'action': 'qw_form_ajax',
            'form': handler + '_form',
            'name': (
            item_count > 0
            ) ? item_type + "_" + item_count : item_type,
            'handler': handler,
            'hook_key': hook_key,
            'type': item_type,
            'query_type': QueryWrangler.query.type,
            'next_weight': $( '#query-' + handler + 's .qw-handler-item' ).length
          };

          var original_handler;

          // get original object from QW
          switch ( post_data_form.handler ) {
            case 'filter':
              if ( QueryWrangler.data.allFilters[post_data_form.hook_key] ) {
                original_handler = QueryWrangler.data.allFilters[post_data_form.hook_key];
              }
              break;

            case 'sort':
              if ( QueryWrangler.data.allSortOptions[post_data_form.hook_key] ) {
                original_handler = QueryWrangler.data.allSortOptions[post_data_form.hook_key];
              }
              break;

            case 'field':
              if ( QueryWrangler.data.allFields[post_data_form.hook_key] ) {
                original_handler = QueryWrangler.data.allFields[post_data_form.hook_key];
              }
              break;

            case 'override':
              if ( QueryWrangler.data.allOverrides[post_data_form.hook_key] ) {
                original_handler = QueryWrangler.data.allOverrides[post_data_form.hook_key];
              }
              break;
          }

          // get template and handle results
          QueryWrangler.ajax.post( post_data_form, function ( results ) {
            callback( results, original_handler );
          } );
        }
      };
      // ajax

      /**
       * Methods for Sortable handler items (fields, filters, sorts)
       */
      QueryWrangler.sortables = {

        // css selector where sortables can be found
        wrapper_selector: '',
        // individual item that is sortable
        item_selector: '',

        /**
         * Init sortables and create handles
         *
         * @params selector - common css selector for all sortable areas
         */
        init: function ( wrapper_selector, item_selector ) {
          QueryWrangler.sortables.wrapper_selector = wrapper_selector;
          QueryWrangler.sortables.item_selector = item_selector;

          $( wrapper_selector )
              .sortable( {
                handle: '.sortable-handle',
                update: QueryWrangler.sortables.updateItemWeights
              } );
          QueryWrangler.sortables.refresh();
        },

        /**
         * Refresh the jQuery UI sortable()
         */
        refresh: function () {
          QueryWrangler.sortables.makeSortableHandles();
          $( QueryWrangler.sortables.wrapper_selector ).sortable( 'refresh' );
        },

        /**
         * Create handles for sortable items
         *
         * @params selector - common css selector where the handles should be created
         */
        makeSortableHandles: function () {
          var $handler_items = $( QueryWrangler.sortables.wrapper_selector ).find( QueryWrangler.sortables.item_selector );
          $handler_items.find( '.sortable-handle' ).remove();

          $handler_items.each( function ( i, element ) {
            $( element ).prepend( '<div class="sortable-handle"></div>' );
          } );
        },

        /**
         * jQuery UI sortable.update callback
         * - Update all item weights
         */
        updateItemWeights: function () {
          // loop through sortable handlers
          $( QueryWrangler.sortables.wrapper_selector ).each( function (
              handlers_i,
              handler_items
          ) {
            // loop through handler items and set new weights
            $( handler_items ).find( QueryWrangler.sortables.item_selector ).each( function (
                i,
                handler_item
            ) {
              $( handler_item ).find( '.qw-weight' ).attr( 'value',
                  i ).val( i );
            } );
          } );

        }
      };
      // sortables

      /**
       * Option Group methods
       *  - option groups are extra settings for handler item that require enabling to work.
       *  - eg, rewrite output, create label, etc
       */
      QueryWrangler.optionGroups = {
        /**
         * Init option groups by delegating checkbox change
         */
        init: function () {
          // delegate toggling of option group
          $( 'body' ).on( 'change',
              '.qw-options-group-title input[type=checkbox]',
              function ( event ) {
                $box = $( this );

                if ( $box.is( ':checked' ) ) {
                  // show options
                  $box.closest( '.qw-options-group' ).find( '.qw-options-group-content' ).show();
                }
                else {
                  // hide options
                  $box.closest( '.qw-options-group' ).find( '.qw-options-group-content' ).hide();
                }
              } );

          // set load-states
          QueryWrangler.optionGroups.refresh();
        },

        /**
         * Loop through options groups, determine state, and toggle as necessary
         */
        refresh: function () {
          $( '.qw-options-group' ).each( function ( i, group ) {
            // get the checkbox
            var $box = $( group ).find( '.qw-options-group-title' ).find( 'input[type=checkbox]' );

            if ( ! $box.is( ':checked' ) ) {
              // hide options if not checked initially
              $box.closest( '.qw-options-group' ).find( '.qw-options-group-content' ).hide();
            }
          } );
        }
      };
      // option groups

      /**
       * JS Titles
       *
       * @type {Object}
       */
      QueryWrangler.jsTitles = {

        // generic selector for all handler items
        handler_items_selector: '',

        // selector for where the js-title details should be placed
        details_container_selector: '',

        /**
         * Initialize the js-titles for handlers
         *
         * @param handler_items_selector
         * @param details_container_selector
         */
        init: function ( handler_items_selector, details_container_selector ) {
          QueryWrangler.jsTitles.handler_items_selector = handler_items_selector;
          QueryWrangler.jsTitles.details_container_selector = details_container_selector;

          // initial titles
          QueryWrangler.jsTitles.refresh();
        },

        /**
         * Update handler item titles based on values selected
         * - gets values from fields with .qw-js-title class
         */
        refresh: function () {
          // remove and recreate any details spans
          $( QueryWrangler.jsTitles.details_container_selector + ' .details' ).remove();
          $( QueryWrangler.jsTitles.details_container_selector ).append( '<span class="details"></span>' );

          var handler_items = $( QueryWrangler.jsTitles.handler_items_selector );

          // each handler_item may have multiple qw-js-title fields
          $.each( handler_items, function ( i, handler_item ) {
            var $handler_item = $( handler_item );
            var $title_target = $handler_item.find( QueryWrangler.jsTitles.details_container_selector + ' .details' );
            var new_title = [];

            // loop through all js-title elements and make a new tile
            $handler_item.find( '.qw-js-title' ).each( function ( i, element ) {
              $element = $( element );
              var element_value = $element.val();

              switch ( $element.prop( 'tagName' ) ) {
                case 'INPUT':
                  var tag_type = $element.attr( 'type' );
                  // checkboxes are special!
                  if ( tag_type == 'checkbox' ) {
                    var is_checked = $element.prop( 'checked' );
                    if ( ! element_value ) {
                      element_value = (
                          is_checked
                      ) ? "on" : "off";
                    }
                    else if ( ! is_checked ) {
                      element_value = '';
                    }
                  }

                  if ( element_value ) {
                    new_title.push( element_value );
                  }
                  break;

                case 'TEXTAREA':
                  if ( element_value ) {
                    new_title.push( 'In Use' );
                  }
                  break;

                case 'SELECT':
                  new_title.push( element_value );
                  break;
              }
            } );

            // no items found
            if ( new_title.length == 0 ) {
              new_title.push( 'None' );
            }
            // title array to string
            new_title = new_title.join( ', ' );

            // set new title
            $title_target.text( new_title );
          } );
        }
      };

    }
)( jQuery );

/*
 * Init QW
 */
(
    function ( $ ) {

      $( document ).ready( function () {
        var $form = $( 'form#qw-edit-query-form' );

        // some data is required from PHP
        if ( $form.data( 'query-id' ) && $form.data( 'ajax-url' ) ) {
          QueryWrangler.init( $form.data( 'query-id' ) );
        }
        else {
          alert( 'no query id' );
        }
      } );

    }
)( jQuery );
