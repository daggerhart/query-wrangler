(function( $ ){


  // prevent queuing toggles
  var qw_ui_is_toggling = false;

  /**
   * Toggle an element if no other element is toggling
   *
   * @param element - the element to toggle
   */
  function _toggle( element ){
    if (!qw_ui_is_toggling) {
      qw_ui_is_toggling = true;

      $(element).toggle( 300, function(){
        qw_ui_is_toggling = false;
      });
    }
  }

  /**
   * Refresh the UI components on the page
   * @private
   */
  function _refresh_ui(){
    $('.qw-add-new-items').hide();
    $('textarea.qw-field-textarea').addClass('blurred');

    QueryWrangler.jsTitles.refresh();
    QueryWrangler.optionGroups.refresh();
    QueryWrangler.sortables.refresh();
  }


  // init
  $(document).ready(function(){
    // sortable item lists
    QueryWrangler.sortables.init('.qw-handler-items', '.qw-handler-item');

    // freshen the ui js
    _refresh_ui();

    // open and close the 'add new' checkbox groups
    $('.qw-add-item').click(function(){
      _toggle( $(this).parent().next('.qw-add-new-items') );
    });

    // open and close single handler items
    $('.qw-handler-items').on('click', '.qw-setting-header', function(){
      $(this).parent().find('.group').toggle();
    });

    // delegated events
    $('#qw-edit-query-form')
      // focus and blur for textareas
      .on('focus', 'textarea.qw-field-textarea', function(){
        $(this).removeClass('blurred');
      })
      .on('blur', 'textarea.qw-field-textarea', function(){
        $(this).addClass('blurred');
      })
      // remove buttons
      .on('click', '.qw-remove', function(){
        $(this).closest('.qw-handler-item').remove();
        QueryWrangler.sortables.updateItemWeights();
      });

    // get new handler items forms
    $('.qw-add-items-submit').click(function(){
      var handler = $(this).attr('title');
      var checkedboxes = $(this).closest('.qw-add-new-items').find('input[type=checkbox]:checked');

      $.each( checkedboxes, function( index, box ){
        var $box = $(box);
        var item_type = $box.val();
        var hook_key = $box.next('input[type=hidden]').val();

        QueryWrangler.ajax.getHandlerItemTemplate( handler, item_type, hook_key, function( results, original_handler ){
          // append the results
          $('#query-'+handler+'s').append( '<div class="qw-handler-item">' + results.template + '</div>');

          // uncheck the box
          $box.removeAttr('checked');

          if (handler == 'field'){
            // Update Field tokens
            QueryWrangler.generateFieldTokens();
          }
        });
      });

      _toggle( $(this).closest('.qw-add-new-items') );
      _refresh_ui();
    });
  });

})(jQuery);