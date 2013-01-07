// array of available options for looping
QueryWrangler.handlers = ['field','filter','sort'];

/*
 * Sortable callback for field weights
 * @param {String} type field or filter
 */
QueryWrangler.update_weights = function()
{
  jQuery(QueryWrangler.handlers).each(function(i, handler){
    jQuery("#existing-"+handler+"s .qw-"+handler)
      .each(function(i){
        jQuery(this).find(".qw-weight").attr('value', i);
        jQuery(this).find(".qw-weight").val(i);
    });

    if (handler == 'field') {
      // Update Field tokens
      QueryWrangler.generate_field_tokens();
    }
  });
}
/*
 * Make tokens for fields
 */
QueryWrangler.generate_field_tokens = function() {
  var tokens = [];
  jQuery('#existing-fields div.qw-field').each(function(){
    // field name
    var field_name = jQuery(this).find('.qw-field-name').val();
    // add tokens
    tokens.push('<li>{{'+field_name+'}}</li>');
    // target the field and insert tokens
    jQuery('#qw-field-'+field_name+' ul.qw-field-tokens-list').html(tokens.join(""));
  });
}
/*
 * Get templates
 * @param {String} handler - field or filter
 * @param {String} item_type the field or filter type
 */
QueryWrangler.get_handler_templates = function(handler, handler_hook, item_type){
  var item_count = jQuery('#qw-options-forms input.qw-'+handler+'-type[value='+item_type+']').length;
  var next_name = (item_count > 0) ? item_type + "_" + item_count: item_type;
  var next_weight = jQuery('ul#qw-'+handler+'s-sortable li').length;

  // prepare post data for form and sortable form
  var post_data_form = {
    'action': 'qw_form_ajax',
    'form': handler+'_form',
    'name': next_name,
    'handler': handler,
    'hook_key': handler_hook,
    'type': item_type,
    'query_type': QueryWrangler.query.type,
    'next_weight': next_weight
  };

  // ajax call to get form
  jQuery.ajax({
    url: QueryWrangler.ajaxForm,
    type: 'POST',
    async: false,
    data: post_data_form,
    success: function(results){
      // append the results
      jQuery('#existing-'+handler+'s').append(results);
    }
  });

  QueryWrangler.toggle_empty_lists();

  return next_name;
}
/*
 * Add new handler
 */
QueryWrangler.add_item = function(){
  var handler = jQuery(this).children('input.add-handler-type').val();
  jQuery('#qw-display-add-'+handler+'s input[type=checkbox]')
    .each(function(index,element){
      if(jQuery(element).is(':checked')){
        // item type
        var item_type = jQuery(element).val();
        var handler_hook = jQuery(element).siblings('input.qw-hander-hook_key').val();
        // add a new field
        var next_name = QueryWrangler.get_handler_templates(handler, handler_hook, item_type);
        // remove check
        jQuery(element).removeAttr('checked');
      }
  });
  QueryWrangler.theme_accordions();
  //jQuery(this).dialog('close');
}

QueryWrangler.theme_accordions = function(){
  jQuery('#display-style-settings, #row-style-settings, .qw-sortable-list')
    .accordion('destroy');
  jQuery('#display-style-settings, #row-style-settings, .qw-sortable-list')
    .accordion({
      header: '> div > .qw-setting-header',
      collapsible: true,
      active: false,
      autoHeight: false
  });

  jQuery(".qw-sortable-list").sortable({
    axis: 'y',
    handle: 'span.qw-setting-header',
    autoHeight: false,
    update: function(event,ui){
      // update option weights
      QueryWrangler.update_weights();
      QueryWrangler.toggle_empty_lists();
    },
				stop: function( event, ui ) {
					// IE doesn't register the blur when sorting
					// so trigger focusout handlers to remove .ui-state-focus
					ui.item.children( "span.qw-setting-header" ).triggerHandler( "focusout" );
				}
  });
}

QueryWrangler.toggle_empty_lists = function()
{
  var lists = jQuery(".qw-sortable-list");
  jQuery.each(lists, function(){
    var num_items = jQuery(this).children('.qw-sortable-item');
    if(num_items.length > 0)
    {
      jQuery(this).children('.qw-empty-list').hide();
    }
    else
    {
      jQuery(this).children('.qw-empty-list').show();
    }
  });
}
jQuery(document).ready(function(){

  QueryWrangler.theme_accordions();
  QueryWrangler.toggle_empty_lists();
  QueryWrangler.update_weights();

  jQuery('#qw-query-admin-options-wrap').delegate('.qw-query-title', 'click', function(){
    var id = jQuery(this).attr('title');
    var dialog_title = jQuery(this).text();

    jQuery('#'+id).dialog({
      modal: true,
      width: '50%',
      height: 500,
      title: dialog_title,
      resizable: false,
      buttons: [{
        text: dialog_title.replace('Add', 'Add Selected'),
        click: QueryWrangler.add_item
      }]
    });
  });

  jQuery('#qw-query-admin-options-wrap').delegate('.qw-remove', 'click', function(){
    jQuery(this).parent().parent().remove();
    QueryWrangler.toggle_empty_lists();
    QueryWrangler.update_weights();
    QueryWrangler.theme_accordions();
  });

  // tab it all out
  jQuery('#qw-query-admin-options-wrap').tabs();
});