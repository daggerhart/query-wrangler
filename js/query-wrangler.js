/*
 * Simple show/hide toggle for field options
 */
function qw_field_options_toggle(element) {
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
function qw_hide_options(){
  jQuery('.qw-query-content').hide();
}
/*
 * Sortable callback for field weights
 */
function qw_update_field_weights()
{
  jQuery("#qw-sort-fields  ul#qw-fields-sortable li.qw-field-item").each(function(i){
    jQuery(this).find(".qw-field-weight").attr('value', i);
    jQuery(this).find(".qw-field-weight").val(i);
  });
  
  // Update Field tokens
  qw_generate_field_tokens();
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
      title_array.push('<div><span class="qw-query-fields-name" title="qw-field-'+field_name+'">'+jQuery(element).text()+'</span></div>'); 
    });
    jQuery('#qw-query-fields-list').html(title_array.join(''));
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
  qw_bind_events();
}
/*
 * Get Field Template and add new sortable items
 */
function qw_new_field_template(field_type){
  var item_count = jQuery('#qw-options-forms input.qw-field-type[value='+field_type+']').length;
  var field_name = (item_count > 0) ? field_type + "_" + item_count: field_type;
  var next_weight = jQuery('ul#qw-fields-sortable li').length;

  // new sortable item
  var output = '';
  output+= "<li class='qw-item qw-field-item'>";
  output+= "<div class='sort-handle'></div>";
  output+= "<span class='qw-field-remove qw-button'>Remove this field</span>";
  output+= "<span class='qw-sort-field-name'>"+field_name+"</span>";
  output+= "<input class='qw-field-weight' name='qw-query-options[display][field_settings][fields]["+field_name+"][weight]' type='text' size='2' value='"+(next_weight)+"' />";
  output+= "<span class='qw-field-title'>"+field_type.replace("_", " ")+"</span>";
  output+= "</li>";
  
  // TODO:  since this is synchronous, show a throbber
  // ajax call to get field form
  jQuery.ajax({
    url: QueryWrangler.formField,
    type: 'POST',
    async: false,
    data: {'field_name': field_name, 'field_type': field_type, 'action': 'qw_form_field_ajax'},
    success: function(results){
      // append the results
      jQuery('#qw-options-forms').append(results);
      // add new sortable item
      jQuery('#qw-options-forms ul#qw-fields-sortable').append(output);
    }
  });
  
  
  return field_name;
}
/*
 * Add selected fields
 */
function qw_add_new_fields(){
  var title_array = [];
  jQuery('#qw-options-form-target #qw-display-add-fields input[type=checkbox]').each(function(index,element){
    if(jQuery(this).is(':checked')){
      // field type
      var field_type = jQuery(this).val();
      // add a new field
      var true_name = qw_new_field_template(field_type);
      // remove check
      jQuery(this).removeAttr('checked');
      title_array.push('<div><span class="qw-query-fields-name" title="qw-field-'+true_name+'">'+jQuery(element).val().replace("_", " ")+'</span></div>'); 
    }
  });
  
  jQuery('#qw-query-fields-list').append(title_array.join(''));
  // rebind events
  qw_bind_events();
}
/*
 * Unbind then rebind events to items that get updated or replaced
 */
function qw_bind_events(){
  // unbind first
  jQuery('#qw-add-selected-fields').unbind('click');
  jQuery('span.qw-field-remove').unbind('click');
  jQuery('#qw-options-form-target ul#qw-fields-sortable').unbind('sortable');
  jQuery('.qw-query-fields-name').unbind('click');
  jQuery('.qw-options-group-title input[type=checkbox]').unbind('click');
  
  // adding new fields
  jQuery('#qw-add-selected-fields').click(function(){
    qw_add_new_fields();
    jQuery('#qw-options-forms .qw-query-content').hide();
    jQuery('#qw-options-target-title').html('&nbsp;');
  });
  
  // removing fields
  jQuery('span.qw-field-remove').click(function(){
    // remove the field's form
    var form_name = jQuery(this).siblings('.qw-sort-field-name').text();
    jQuery('#qw-options-forms #qw-field-'+form_name).remove();
    // remove this sortable item
    jQuery(this).parent('li.qw-field-item').remove();
    qw_update_field_weights();
  });
  
  // sortable fields
  jQuery('#qw-options-form-target ul#qw-fields-sortable').sortable({
    handle: '.sort-handle',
    update: function(event,ui){
      qw_update_field_weights();
    }
  }).disableSelection();
  
  // fields forms functionality
  jQuery('.qw-query-fields-name').click(function(){
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
  
  // Field Options
  jQuery('.qw-options-group-title input[type=checkbox]').click(function(){
    qw_field_options_toggle(jQuery(this));
  });
  
  // Update Field tokens
  qw_generate_field_tokens();
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
  //console.log(qw_form_backup);
  // make the new form id the current form id
  qw_current_form_id = qw_new_form_id;
}

/*
 * Globals
 */
var qw_current_form_id;
var qw_new_form_id;
var qw_form_backup;
/*
 * Execution
 */
jQuery(document).ready(function(){
  // bind click and stuff
  qw_bind_events();
  // display style settings 
  qw_style_settings_toggle();
  /*
   * Form handling
   */
  // basic forms functionality
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
  
  // fields actions functionality
  jQuery('.qw-query-fields-title').click(function(){
    // get new form info
    qw_new_form_id = jQuery(this).attr('title');
    // standard click actions
    qw_title_click_action();
    // set the title
    jQuery('#qw-options-target-title').text(jQuery(this).text());
    // show buttons
    if(qw_current_form_id == "qw-sort-fields"){
      jQuery('#qw-options-actions').show();      
    }
  });
  
  /*
   * Update button
   */ 
  jQuery('#qw-options-actions-update').click(function(){
    // hide forms
    jQuery('#qw-options-forms .qw-query-content').hide();
    // empty title
    jQuery('#qw-options-target-title').html('&nbsp;');
    // hide buttons
    jQuery('#qw-options-actions').hide();
    
    qw_set_setting_title();  
    qw_current_form_id = '';
    // display style settings 
    qw_style_settings_toggle();
  });
  
  /*
   * Cancel button
   */ 
  jQuery('#qw-options-actions-cancel').click(function(){
    // hide forms
    jQuery('#qw-options-forms .qw-query-content').hide();
    if(qw_current_form_id != ''){
      // set backup_form
      jQuery('form#qw-edit-query-form').unserializeForm(qw_form_backup);
    }
    // rebind events
    qw_bind_events();
    // empty title
    jQuery('#qw-options-target-title').html('&nbsp;');
    // hide buttons
    jQuery('#qw-options-actions').hide();
  });
  
  // toggle selected fields
  jQuery('.qw-field-options-hidden').each(function(index,element){
    if(jQuery(element).parent().find('input[type=checkbox]').is(':checked')){
      jQuery(element).removeClass('qw-field-options-hidden');
    }
  });
  
  /*
   * List page
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
