/*
 * Globals
 */
QueryWrangler.current_form_id = '';
QueryWrangler.new_form_id = '';
QueryWrangler.form_backup = '';
// array of available options for looping
QueryWrangler.handlers = ['field','filter'];
// changes have been made
QueryWrangler.changes = false;

/*
 * Ajax preview
 */
QueryWrangler.get_preview = function() {
  // show throbber
  jQuery('#query-preview-controls').removeClass('query-preview-inactive').addClass('query-preview-active');
  // serialize form data
  QueryWrangler.form_backup = jQuery('form#qw-edit-query-form').serialize();
  // prepare post data
  var post_data_form = {
    'action': 'qw_form_ajax',
    'form': 'preview',
    'options': QueryWrangler.form_backup
  };
  jQuery.ajax({
    url: QueryWrangler.ajaxForm,
    type: 'POST',
    async: false,
    data: post_data_form,
    dataType: 'json',
    success: function(results){
      jQuery('#query-preview-target').html(results.preview);
      jQuery('#qw-show-arguments-target').html(results.args);
      jQuery('#qw-show-display-target').html(results.display);
      jQuery('#qw-show-wpquery-target').html(results.wpquery);
    }
  });
  // hide throbber
  jQuery('#query-preview-controls').removeClass('query-preview-active').addClass('query-preview-inactive');
}

/*
 * Simple show/hide toggle for field options
 */
QueryWrangler.options_group_toggle  = function(element) {
  if (jQuery(element).is(':checked')){
    jQuery(element).parent().parent().siblings('.qw-options-group-content').show();
  }
  else {
    jQuery(element).parent().parent().siblings('.qw-options-group-content').hide();
  }
}

/*
 * Simple hide functions for forms
 */
QueryWrangler.hide_forms = function(){
  // hide all forms
  jQuery('#qw-options-forms .qw-query-content').hide();
  // empty the title
  jQuery('#qw-options-target-title').html('&nbsp;');
}

/*
 * Sortable callback for field weights
 * @param {String} type field or filter
 */
QueryWrangler.update_weights = function(type)
{
  jQuery("#qw-sort-"+type+"s  ul#qw-"+type+"s-sortable li.qw-"+type+"-item").each(function(i){
    jQuery(this).find(".qw-"+type+"-weight").attr('value', i);
    jQuery(this).find(".qw-"+type+"-weight").val(i);
  });

  if (type == 'field') {
    // Update Field tokens
    QueryWrangler.generate_field_tokens();
  }
}
/*
 * Make tokens for fields
 */
QueryWrangler.generate_field_tokens = function() {
  var tokens = [];
  jQuery('#qw-fields-sortable .qw-field-item').each(function(){
    // field name
    var field_name = jQuery(this).find('.qw-sort-field-name').text();
    // add tokens
    tokens.push('<li>{{'+field_name+'}}</li>');
    // target the field and insert tokens
    jQuery('#qw-options-forms #qw-field-'+field_name+' ul.qw-field-tokens-list').html(tokens.join(""));
  });
}
/*
 * toggle settings for display type
 */
QueryWrangler.style_settings_toggle = function()
{
  if(jQuery('#query-display-type').val() == 'posts'){
    jQuery('div[title=qw-display-post-settings]').show();
  }
  else{
    jQuery('div[title=qw-display-post-settings]').hide();
  }
}

/*
 * Dynamically set the setting title for updated fields
 */
QueryWrangler.set_setting_title = function(){
  //gather all info
  var form = jQuery('#'+QueryWrangler.current_form_id);
  var settings = jQuery('div[title='+QueryWrangler.current_form_id+']');
  var fields = form.find('input[type=text],input[type=checkbox],select,textarea,input[type=hidden]');
  var new_title = [];
  var title_target = settings.children('.qw-setting-value');


  // fields
  if(settings.parent().attr('id') == 'qw-query-fields-list'){
    new_title.push(form.find('input.qw-field-name').val());
  }

  // loop through the fields
  jQuery.each(fields, function(i, field){
    // select
    if (jQuery(field).is('select')){
      new_title.push( jQuery(field).children('option[value='+jQuery(field).val()+']').text() );
    }
    // text field
    if (jQuery(field).is('input[type=text]') &&
        jQuery(field).val() != ''){
      new_title.push(jQuery(field).val());
    }
    // checkbox with value set
    if (jQuery(field).is('input[type=checkbox]') &&
        jQuery(field).is(':checked') &&
        jQuery(field).val() != 'on')
    {
      new_title.push(jQuery(field).val());
    }
  });

  // single textarea fields, like header, footer, empty
  if(fields.length == 1 &&
     jQuery(fields[0]).is('textarea'))
  {
    // in use
    if (jQuery(fields[0]).val().trim() != ''){
      new_title.push('In Use');
    }
  }

  // no items found
  if (new_title.length == 0){
    new_title.push('None');
  }

  // title array to string
  new_title = new_title.join(', ');

  // set new title
  title_target.text(new_title)
    .parent()
      .addClass('qw-modified');
}
/*
 * Get templates
 * @param {String} handler - field or filter
 * @param {String} handler_type the field or filter type
 */
QueryWrangler.get_handler_templates = function(handler, handler_type){
  var item_count = jQuery('#qw-options-forms input.qw-'+handler+'-type[value='+handler_type+']').length;
  var next_name = (item_count > 0) ? handler_type + "_" + item_count: handler_type;
  var next_weight = jQuery('ul#qw-'+handler+'s-sortable li').length;

  // prepare post data for form and sortable form
  var post_data_form = {
    'action': 'qw_form_ajax',
    'form': handler+'_form',
    'name': next_name,
    'type': handler_type,
    'query_type': QueryWrangler.query.type
  };
  var post_data_sortable = {
    'action': 'qw_form_ajax',
    'form': handler+'_sortable',
    'name': next_name,
    'type': handler_type,
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
      jQuery('#qw-options-forms').append(results);
    }
  });

  // new sortable item
  jQuery.ajax({
    url: QueryWrangler.ajaxForm,
    type: 'POST',
    async: false,
    data: post_data_sortable,
    success: function(results){
      // add new sortable item
      jQuery('#qw-options-forms ul#qw-'+handler+'s-sortable').append(results);
    }
  });

  return next_name;
}
/*
 * Add selected fields
 * @param {String} handler field or filter
 */
QueryWrangler.add_new_handler = function(handler){
  jQuery('#qw-options-forms .add-selected-wrapper').addClass('add-selected-wrapper-active');
  var title_array = [];
  jQuery('#qw-options-form-target #qw-display-add-'+handler+'s input[type=checkbox]').each(function(index,element){
    if(jQuery(this).is(':checked')){
      // handler type
      var handler_type = jQuery(this).val();
      // add a new field
      var next_name = QueryWrangler.get_handler_templates(handler, handler_type);
      // remove check
      jQuery(this).removeAttr('checked');
      var field_title;
      if (handler == 'field'){
        field_title = QueryWrangler.allFields[handler_type].title;
      } else if (handler == 'filter') {
        field_title = QueryWrangler.allFilters[handler_type].title
      }
      title_array.push('<div class="qw-query-title" title="qw-'+handler+'-'+next_name+'"><span class="qw-setting-title">'+field_title+'</span> : <span class="qw-setting-value">'+next_name+'</span></div>');
    }
  });

  // Update the 'titles' for the given handler
  // Titles are fields or filter names that are clickable
  jQuery('#qw-query-'+handler+'s-list').append(title_array.join(''));

  // empty form title & hide form
  QueryWrangler.hide_forms();

  if(handler == 'field'){
    // Update Field tokens
    QueryWrangler.generate_field_tokens();
  }

  // refresh sortable items
  jQuery('ul#qw-'+handler+'s-sortable').sortable("refreshItems");
  jQuery('.add-selected-wrapper').removeClass('add-selected-wrapper-active');
}

/*
 * Sort field and filter titles
 */
QueryWrangler.sort_handlers = function(form){
  var handler_type, item_name, item_title, setting_title;
  var items = form.find('li.qw-item');

  // get handler type
  if (form.hasClass('qw-sort-filters-values')){
    handler_type = 'filter';
  } else if (form.hasClass('qw-sort-fields-values')) {
    handler_type = 'field';
  }

  // empty the handler's list of titles
  var target = '#qw-query-'+handler_type+'s-list';
  jQuery(target).html('');

  // loop through the new items
  jQuery.each(items, function(i, item){
    // get its name
    item_name = jQuery(item).children('.qw-sort-'+handler_type+'-name').text().trim();
    item_title = 'qw-'+handler_type+'-'+item_name;
    setting_title = jQuery(item).children('.qw-'+handler_type+'-title').text().trim();
    jQuery(target).append('<div class="qw-query-title qw-modified" title="'+item_title+'"><span class="qw-setting-title">'+setting_title+'</span> : <span class="qw-setting-value"></span></div>');

    // set the title values
    QueryWrangler.current_form_id = 'qw-'+handler_type+'-'+item_name;
    QueryWrangler.set_setting_title();
  });
}

/*
 * Execution
 */
jQuery(document).ready(function(){
  // display style settings
  QueryWrangler.style_settings_toggle();

  /*
   * Form handling
   */
  /*
   * Basic forms click link = show form functionality
   */
  jQuery('#qw-query-admin-options-wrap')
    .delegate('div.qw-query-title span, div.qw-query-add-titles span', 'click', function(){
      // set the title
      var form_title;
      var show_buttons = true;

      // hide forms
      QueryWrangler.hide_forms();
      // hide buttons
      jQuery('#qw-options-actions').hide();

      // trigger cancel button incase another form is open but not 'updated'
      if(QueryWrangler.current_form_id !== undefined && QueryWrangler.current_form_id != ''){
        jQuery('#qw-options-actions-cancel').trigger('click');
      }

      // handle special titles (add/rearrange)
      if (jQuery(this).parent().hasClass('qw-query-add-titles')){
        // get new form info
        QueryWrangler.new_form_id = jQuery(this).attr('title');
        // show field name as part of the form title
        form_title = jQuery(this).text();
        // hide butons on the 'add' title buttons
        if (QueryWrangler.new_form_id == 'qw-display-add-filters' ||
            QueryWrangler.new_form_id == 'qw-display-add-fields')
        {
          show_buttons = false;
        }
      }
      // standard titles
      else {
        // get new form info
        QueryWrangler.new_form_id = jQuery(this).parent('.qw-query-title').attr('title');
        // build the title
        form_title = jQuery(this).parent('.qw-query-title').text().split(':');
        form_title = form_title[0];
      }

      // show buttons
      if (show_buttons){
        jQuery('#qw-options-actions').show();
      }
      // show form
      jQuery('#'+QueryWrangler.new_form_id).show();
      // backup the form
      QueryWrangler.form_backup = jQuery('form#qw-edit-query-form').serialize();
      // make the new form id the current form id
      QueryWrangler.current_form_id = QueryWrangler.new_form_id;

      // set title
      jQuery('#qw-options-target-title').text(form_title);
  });

  /*
   * Update button
   */
  jQuery('#qw-options-actions-update').click(function(){
    var form = jQuery('#'+QueryWrangler.current_form_id);
    // empty form title & hide form
    QueryWrangler.hide_forms();
    // hide buttons
    jQuery('#qw-options-actions').hide();

    // sortables have special needs
    if (form.hasClass('qw-sort-fields-values') ||
        form.hasClass('qw-sort-filters-values'))
    {
      QueryWrangler.sort_handlers(form);
    }
    // normal titles
    else {
      // set the title for the updated field
      QueryWrangler.set_setting_title();
    }
    // clear the current form id
    QueryWrangler.current_form_id = '';
    // display style settings
    QueryWrangler.style_settings_toggle();
    // preview
    if(jQuery('#live-preview').is(':checked')){
      QueryWrangler.get_preview();
    }
    // changes
    if (QueryWrangler.changes == false){
      QueryWrangler.changes = true;
      jQuery('.qw-changes').show();
    }
  });

  /*
   * Cancel button
   */
  jQuery('#qw-options-actions-cancel').click(function(){
    if(QueryWrangler.current_form_id != ''){
      // set backup_form
      jQuery('form#qw-edit-query-form').unserializeForm(QueryWrangler.form_backup);
    }
    // empty form title & hide form
    QueryWrangler.hide_forms();
  });

  /*
   * Preview
   */
  jQuery('#get-preview').click(QueryWrangler.get_preview);

  /*
   * Field options Checkboxes
   */
  jQuery('#qw-options-forms, #query-preview')
    .delegate('.qw-options-group-title input[type=checkbox]','click', function(){
    QueryWrangler.options_group_toggle(jQuery(this));
  });

  /*
   * Toggle selected fields
   */
  jQuery('.qw-field-options-hidden').each(function(index,element){
    if(jQuery(element).parent().find('input[type=checkbox]').is(':checked')){
      jQuery(element).removeClass('qw-field-options-hidden');
    }
  });

  /*****************************************************************************
   * handler init functions/delegates
   */

  /*
   * Adding new buttons for all QueryWrangler.handlers
   */
  jQuery.each(QueryWrangler.handlers, function(index, handler){
    jQuery('#qw-add-selected-'+handler+'s').click(function(){
      QueryWrangler.add_new_handler(handler);
    });
  });

  /*
   * Sortable lists for all QueryWrangler.handlers
   */
  jQuery.each(QueryWrangler.handlers, function(index, handler){
    jQuery('ul#qw-'+handler+'s-sortable').sortable({
      handle: '.sort-handle',
      update: function(event,ui){
        // update option weights
        QueryWrangler.update_weights(handler);
      }
    }).disableSelection();
  });

  // Remove buttons for all QueryWrangler.handlers
  jQuery.each(QueryWrangler.handlers, function(index, handler){
    // delegate the remove click function to all remove buttons
    jQuery('li.qw-'+handler+'-item').delegate('span.qw-'+handler+'-remove', 'click', function(){
      // remove the field's form
      var form_name = jQuery(this).siblings('.qw-sort-'+handler+'-name').text();
      jQuery('#qw-options-forms #qw-'+handler+'-'+form_name).remove();
      // remove this sortable item
      jQuery(this).parent('li.qw-'+handler+'-item').remove();
      QueryWrangler.update_weights(handler);
    });
  });

  /********* end handlers functions ********************************************/

  QueryWrangler.get_preview();
  /*
   * Query List page
   */
  // delete confirm
  jQuery('.qw-delete-query, .tablenav #doaction').click(function(){
    var ask = confirm('Are you sure you want to delete?');
    if (ask) {
      return true;
    }
    else{
      return false;
    }
  });
});
