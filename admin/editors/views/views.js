(function($){

  QWViews = {

    // current open dialog
    current_dialog: {},

    // HTML id of current form in dialog
    current_dialog_id: '',

    // html clone of the form to be displayed
    current_dialog_backup: '',

    /**
     * Initialize the views editor theme
     */
    init: function(){
      $('#qw-edit-query-form')
        // item title dialogs
        .on('click','.qw-handler-item-title', QWViews.handlerItemTitleDialog );

      // need a broad content because dialog is near top of html
      $('body')
        // remove buttons
        .on('click', '.qw-remove.button', function(){
          /// get the dialog element and execute the remove
          QWViews.removeHandlerItem( QWViews.current_dialog );
        });

      // rearrange sortable handler items
      $('.qw-rearrange-title').click( QWViews.rearrangeHandlerItemsDialog );

      // add new handler items
      $('.qw-add-title').click( QWViews.addHandlerItemsDialog );

      QueryWrangler.jsTitles.init('.qw-handler-item', '.qw-handler-item-title');
      QueryWrangler.optionGroups.init();
    },

    /**
     * Open a dialog with the setting title as the dialog title, and the item form as the dialog content
     */
    handlerItemTitleDialog: function(){
      var $item = $(this);
      var dialog_title = $item.html().split('<span ')[0];

      QWViews.openDialog(dialog_title, $item.next('.qw-handler-item-form') );
    },

    /**
     * Dialog box for adding new handler items
     */
    addHandlerItemsDialog: function(){
      var handler = $(this).data('handler-type');
      var title   = $(this).closest('.qw-query-admin-options').find('h4:first').text();
      var id      = '#' + $(this).data('form-id');

      QWViews.openDialog( title , $(id), QWViews.addHandlerItems );
    },

    /**
     * Loop through handler item checkboxes,
     *  get the template for the item, and append to the list
     *
     * @param element - dom element where the checkboxes can be found
     */
    addHandlerItems: function( element ){
      var handler = $(element).data('handler-type');
      var checkedboxes = $(element).find('input[type=checkbox]:checked');

      $.each( checkedboxes, function( index, box ) {
        var $box = $(box);
        var item_type = $box.val();
        var hook_key = $box.next('input[type=hidden]').val();

        QueryWrangler.ajax.getHandlerItemTemplate( handler, item_type, hook_key, function( results, original_handler ) {
          var item =
            '<div class="qw-handler-item">' +
            '<div class="qw-handler-item-title">'+ original_handler.title + '</div>' +
            '<div class="qw-handler-item-form">' +
            results.template +
            '</div>' +
            '</div>';

          // append the results
          $('#query-'+handler+'s').append( item );

          // uncheck the box
          $box.removeAttr('checked');

          if (handler == 'field'){
            // Update Field tokens
            QueryWrangler.generateFieldTokens();
          }

          QueryWrangler.optionGroups.refresh();
        });
      });
    },

    /**
     * Dialog box for rearranging a set of handler items
     */
    rearrangeHandlerItemsDialog: function(){
      var $wrapper = $(this).closest('.qw-query-admin-options').find('.qw-handler-items');
      var wrapper_selector = '#' + $wrapper.attr('id');
      QWViews.openDialog( $(this).text(), $wrapper );

      // similar to sortables.init, but with some differences to the sortable setup
      QueryWrangler.sortables.wrapper_selector = wrapper_selector;
      QueryWrangler.sortables.item_selector = '.qw-handler-item';

      $( wrapper_selector )
        .sortable({
          update: QueryWrangler.sortables.updateItemWeights
        })
        // avoid potential conflicts with other complicated ui elements and events
        .disableSelection();
      QueryWrangler.sortables.refresh();
    },

    /**
     * Open a dialog box
     *
     * @param dialog_title - title for the dialog box
     * @param element - element to load into the dialog
     * @param update_callback - callback to execute after dialog Update button pressed
     */
    openDialog: function( dialog_title, element, update_callback ){
      var $element = $(element);

      QWViews.current_dialog_backup = $element.html();
      QWViews.current_dialog_id = $element.find('.qw-item-form:first').attr('id');

      var args = {
        modal: true,
        width: ($(window).width() * 0.7),
        height: ($(window).height() * 0.8),
        title: dialog_title,
        resizable: false,

        open: function() {
          $(this).dialog("option", "position", "center");
        },
        close: function() {
          QWViews.cancelDialog(this);
        },
        buttons: [{
          text: 'Update',
          click: function() {
            if ( update_callback ){
              update_callback( this );
            }
            QWViews.keepDialogChanges(this);
          }
        },{
          text: 'Cancel',
          click: function() {
            QWViews.cancelDialog(this);
          }
        }]
      };

      // handler items can be removed
      if ( $element.hasClass('can-remove') ) {
        args.buttons.push({
          text: 'Remove',
          click: function(){
            QWViews.removeHandlerItem( this );
          }
        });
      }

      $element.dialog( args ).dialog('open');

      QWViews.current_dialog = $element;
    },

    /**
     * Do not save the changes to the content in the dialog box
     *
     * @param element - the element that was presented in the dialog
     */
    cancelDialog: function( element ) {
      $(element).dialog('destroy');

      if ( QWViews.current_dialog_id ){
        // replace the dialog contents with its backup
        $('form#qw-edit-query-form')
          .find('#' + QWViews.current_dialog_id)
            .html( QWViews.current_dialog_backup );

        // clear the current dialog info
        QWViews.current_dialog = {};
        QWViews.current_dialog_id = '';
        QWViews.current_dialog_backup = '';
      }
    },

    /**
     * Keep the changes made to the contents of the dialog
     *
     * @param element - the element that was presented in the dialog
     */
    keepDialogChanges: function( element ) {
      $(element).dialog('destroy');

      QueryWrangler.jsTitles.refresh();

      // preview
      if ($('#live-preview').is(':checked')) {
        QueryWrangler.ajax.getPreview();
      }
      // changes
      if (QueryWrangler.changes == false) {
        QueryWrangler.changes = true;
        $('.qw-changes').show();
      }
    },

    /**
     * Destroy the dialog and remove the item from the dom
     *
     * @param element - the element that was presented in the dialog
     */
    removeHandlerItem: function( element ){
      $(element).dialog('destroy')
        .closest('.qw-handler-item').remove();
    }
  };

  $(document).ready(function(){
    QWViews.init();
  });
})(jQuery);


