/*
 * Simple show/hide toggle for field options
 */
function qw_options_group_toggle(element) {
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
function qw_empty_form(){
  // hide all forms
  jQuery('#qw-options-forms .qw-query-content').hide();
  // empty the title
  jQuery('#qw-options-target-title').html('&nbsp;');
}
/*
 * Sortable callback for field weights
 * @param {String} type field or filter
 */
function qw_update_weights(type)
{
  jQuery("#qw-sort-"+type+"s  ul#qw-"+type+"s-sortable li.qw-"+type+"-item").each(function(i){
    jQuery(this).find(".qw-"+type+"-weight").attr('value', i);
    jQuery(this).find(".qw-"+type+"-weight").val(i);
  });
  
  if (type == 'field') {
    // Update Field tokens
    qw_generate_field_tokens();
  }
}
/*
 * make tokens for fields
 */
function qw_generate_field_tokens() {
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
function qw_style_settings_toggle()
{
  if(jQuery('#qw-query-admin-options-wrap span[title=qw-display-type] span').text() == 'full'){
    jQuery('#qw-query-admin-options-wrap span[title=qw-display-full-settings]').show();
    jQuery('#qw-query-admin-options-wrap span[title=qw-display-fields-settings]').hide();
  }
  else if(jQuery('#qw-query-admin-options-wrap span[title=qw-display-type] span').text() == 'fields'){
    jQuery('#qw-query-admin-options-wrap span[title=qw-display-fields-settings]').show();
    jQuery('#qw-query-admin-options-wrap span[title=qw-display-full-settings]').hide();
  }
}
/*
 * Dynamically set the setting title for updated fields
 */
function qw_set_setting_title(){
  // change title values
  var new_title = '';

  // single value
  if(jQuery('#'+qw_current_form_id).hasClass('qw-single-value')){
    new_title = jQuery('#'+qw_current_form_id).children('.qw-field-value').val();
  }
  // multiple values
  else if (jQuery('#'+qw_current_form_id).hasClass('qw-multiple-values')){
    // use an array and join it
    var title_array = [];
    jQuery('#'+qw_current_form_id).children('.qw-field-value').each(function(index, element){
      title_array.push(jQuery(element).val());
    });
    
    // handle empty
    if(title_array.length > 0){
      new_title = title_array.join(" - ");
    } else {
      new_title = 'None';
    }
  }
  // checkboxes
  else if (jQuery('#'+qw_current_form_id).hasClass('qw-checkbox-values')){
    // use an array and join it
    var title_array = [];
    jQuery('#'+qw_current_form_id).find('input[type=checkbox]:checked').each(function(index, element){
      title_array.push(jQuery(element).val());
    });
    
    // handle empty
    if(title_array.length > 0){
      new_title = title_array.join(",");
    } else {
      new_title = 'None';
    }
  }
  // fields
  else if (jQuery('#'+qw_current_form_id).hasClass('qw-sort-fields-values')) {
    var title_array = [];
    jQuery('#'+qw_current_form_id).find('.qw-field-title').each(function(index, element){
      var field_name = jQuery(element).siblings('.qw-sort-field-name').text();
      title_array.push('<div><span class="qw-query-fields-name" title="qw-field-'+field_name+'">'+field_name.replace("_", " ")+'</span></div>'); 
    });
    jQuery('#qw-query-fields-list').html(title_array.join(''));
  }
  // filters
  else if (jQuery('#'+qw_current_form_id).hasClass('qw-sort-filters-values')) {
    var title_array = [];
    jQuery('#'+qw_current_form_id).find('.qw-filter-title').each(function(index, element){
      var filter_name = jQuery(element).siblings('.qw-sort-filter-name').text();
      title_array.push('<div><span class="qw-query-filters-name" title="qw-filter-'+filter_name+'">'+filter_name.replace("_", " ")+'</span></div>'); 
    });
    jQuery('#qw-query-filters-list').html(title_array.join(''));
  }  
  // header - footer - empty
  else if (jQuery('#'+qw_current_form_id).hasClass('qw-header-footer-empty')){
    if(jQuery('#'+qw_current_form_id).find('.qw-field-textarea').val().trim() == ""){
      new_title = 'None';
    } else {
      new_title = 'In Use';
    }
  }
  // set new title
  jQuery('span[title='+qw_current_form_id+'] span').text(new_title);
}
/*
 * Get templates
 * @param {String} option field or filter
 * @param {String} option_type the field or filter type
 */
function qw_get_option_templates(option, option_type){
  var item_count = jQuery('#qw-options-forms input.qw-field-type[value='+option_type+']').length;
  var next_name = (item_count > 0) ? option_type + "_" + item_count: option_type;
  var next_weight = jQuery('ul#qw-'+option+'s-sortable li').length;

  // prepare post data for form and sortable form
  var post_data_form = {
    'action': 'qw_form_ajax',
    'form': option+'_form',
    'name': next_name,
    'type': option_type,
    'query_type': QueryWrangler.query.type
  };
  var post_data_sortable = {
    'action': 'qw_form_ajax',
    'form': option+'_sortable',
    'name': next_name,
    'type': option_type,
    'query_type': QueryWrangler.query.type,
    'next_weight': next_weight
  };
  
  // TODO:  since this is synchronous, show a throbber
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
      jQuery('#qw-options-forms ul#qw-'+option+'s-sortable').append(results);
    }
  });
  
  return next_name;
}
/*
 * Add selected fields
 * @param {String} option field or filter
 */
function qw_add_new_option(option){
  var title_array = [];
  jQuery('#qw-options-form-target #qw-display-add-'+option+'s input[type=checkbox]').each(function(index,element){
    if(jQuery(this).is(':checked')){
      // option type
      var option_type = jQuery(this).val();
      // add a new field
      var next_name = qw_get_option_templates(option, option_type);
      // remove check
      jQuery(this).removeAttr('checked');
      title_array.push('<div><span class="qw-query-'+option+'s-name" title="qw-'+option+'-'+next_name+'">'+next_name.replace("_", " ")+'</span></div>'); 
    }
  });
  
  // Update the 'titles' for the given option
  // Titles are fields or filter names that are clickable
  jQuery('#qw-query-'+option+'s-list').append(title_array.join(''));
  
  // empty form title & hide form
  qw_empty_form();
  
  if(option == 'field'){
    // Update Field tokens
    qw_generate_field_tokens();
  }

  // refresh sortable items
  jQuery('#qw-options-form-target ul#qw-'+option+'s-sortable').sortable("refreshItems");
}
/*
 * Standard operations of a title click action
 */
function qw_title_click_action()
{
  // trigger cancel button
  if(qw_current_form_id !== undefined && qw_current_form_id != ''){
    jQuery('#qw-options-actions-cancel').trigger('click');
  }
  // hide forms
  jQuery('#qw-options-forms .qw-query-content').hide();
  // show form
  jQuery('#qw-options-forms #'+qw_new_form_id).show();
  // backup the form
  qw_form_backup = jQuery('form#qw-edit-query-form').serialize();
  // make the new form id the current form id
  qw_current_form_id = qw_new_form_id;
}

/*
 * Globals
 */
var qw_current_form_id;
var qw_new_form_id;
var qw_form_backup;
// array of available options for looping
var qw_options = ['field','filter'];
/*
 * Execution
 */
jQuery(document).ready(function(){
  // display style settings 
  qw_style_settings_toggle();
  
  /*
   * Form handling
   */
  
  /*
   * Basic forms click link = show form functionality
   */ 
  jQuery('span.qw-query-title span').click(function(){
    // get new form info
    qw_new_form_id = jQuery(this).parent('.qw-query-title').attr('title');
    // standard click actions
    qw_title_click_action();
    // set the title
    var qw_form_title = jQuery(this).parent('.qw-query-title').text().split(':');
    qw_form_title = qw_form_title[0];
    // set title
    jQuery('#qw-options-target-title').text(qw_form_title);
    // show buttons
    jQuery('#qw-options-actions').show();
  });  
  
  /*
   * Update button
   */ 
  jQuery('#qw-options-actions-update').click(function(){
    // empty form title & hide form
    qw_empty_form();
    // hide buttons
    jQuery('#qw-options-actions').hide();
    // set the title for the updated field
    qw_set_setting_title();
    // clear the current form id
    qw_current_form_id = '';
    // display style settings 
    qw_style_settings_toggle();
  });
  
  /*
   * Cancel button
   */ 
  jQuery('#qw-options-actions-cancel').click(function(){
    if(qw_current_form_id != ''){
      // set backup_form
      jQuery('form#qw-edit-query-form').unserializeForm(qw_form_backup);
    }
    // empty form title & hide form
    qw_empty_form();
  });
  
  /*
   * Field Options Checkboxes
   */ 
  jQuery('#qw-options-forms, #query-preview').delegate('.qw-options-group-title input[type=checkbox]','click', function(){
    qw_options_group_toggle(jQuery(this));
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
   * Option init functions/delegates
   */
  
  /*
   * Adding new buttons for all qw_options
   */
  jQuery.each(qw_options, function(index, option){
    jQuery('#qw-add-selected-'+option+'s').click(function(){
      qw_add_new_option(option);
    });  
  });
  
  /*
   * Sortable lists for all qw_options
   */ 
  jQuery.each(qw_options, function(index, option){
    jQuery('#qw-options-form-target ul#qw-'+option+'s-sortable').sortable({
      handle: '.sort-handle',
      update: function(event,ui){
        // update option weights
        qw_update_weights(option);
      }
    }).disableSelection();  
  });  
  
  // Title click actions for all qw_options
  // Titles for options are Add Field/Filter & Rearrange Field/Filter, etc
  jQuery.each(qw_options, function(index, option){
    jQuery('#qw-query-'+option+'s').delegate('.qw-query-'+option+'s-title', 'click', function(){
      // get new form info
      qw_new_form_id = jQuery(this).attr('title');
      // standard click actions
      qw_title_click_action();
      // set the title
      jQuery('#qw-options-target-title').text(jQuery(this).text());
      // show buttons
      if(qw_current_form_id == 'qw-sort-'+option+'s'){
        jQuery('#qw-options-actions').show();      
      }
    });
  });
  
  // Remove buttons for all qw_options
  jQuery.each(qw_options, function(index, option){
    jQuery('li.qw-'+option+'-item').delegate('span.qw-'+option+'-remove', 'click', function(){
      // remove the field's form
      var form_name = jQuery(this).siblings('.qw-sort-'+option+'-name').text();
      jQuery('#qw-options-forms #qw-'+option+'-'+form_name).remove();
      // remove this sortable item
      jQuery(this).parent('li.qw-'+option+'-item').remove();
      qw_update_weights(option);
    });
  });

  
  // Options Name click, like field and filter name clicking
  jQuery.each(qw_options, function(index,option){
    jQuery('#qw-query-'+option+'s-list').delegate('.qw-query-'+option+'s-name','click', function(){
      // get form id
      qw_new_form_id = jQuery(this).attr('title');
      // standard click actions
      qw_title_click_action();
      // set the title
      var qw_form_title = jQuery(this).text();
      // set title
      jQuery('#qw-options-target-title').text(qw_form_title);    
      // show action buttons
      jQuery('#qw-options-actions').show();  
    });
  });
  /********* end options functions ********************************************/
  
  /*
   * Query List page
   */
  // delete confirm
  jQuery('.qw-delete-query').click(function(){
    var ask = confirm('Are you sure you want to delete this Query?');
    if (ask) {
      return true;
    }
    else{
      return false;
    }
  });
});
