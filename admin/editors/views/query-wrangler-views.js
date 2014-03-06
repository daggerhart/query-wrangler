QueryWrangler.set_modified = true;

/*
 * Move dialog boxes to the form
 */
QueryWrangler.restore_form = function(dialog, handler){
  jQuery(dialog).dialog('destroy');
  if (handler){
    jQuery(dialog).appendTo('#existing-'+handler+'s').hide();
  }
  else {
    jQuery(dialog).appendTo('#qw-options-forms').hide();
  }
}

/*
 * Get templates
 * @param {String} handler - field or filter
 * @param {String} item_type the field or filter type
 */
QueryWrangler.get_handler_templates = function(handler, handler_hook, item_type){
  var item_count = jQuery('#qw-options-forms input.qw-'+handler+'-type[value='+item_type+']').length;
  var next_name = (item_count > 0) ? item_type + "_" + item_count: item_type;
  var next_weight = jQuery('#qw-query-'+handler+'s-list .qw-query-title').length;

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
      QueryWrangler.add_list_item(post_data_form);
      
      if (handler == 'field'){
          // Update Field tokens
        QueryWrangler.generate_field_tokens();
      }
    }
  });

  QueryWrangler.toggle_empty_lists();
  
  return next_name;
}

/*
 * Add new qw-query-title
 */
QueryWrangler.add_list_item = function(post_data_form){
  var title ;
  switch(post_data_form.handler){
    case 'field':
      title = QueryWrangler.allFields[post_data_form.hook_key].title;
      break;
    case 'filter':
      title = QueryWrangler.allFilters[post_data_form.hook_key].title;
      break;
    case 'sort':
      title = QueryWrangler.allSortOptions[post_data_form.hook_key].title;
      break;
  }


  var output = "<div class='qw-query-title' title='qw-"+post_data_form.handler+"-"+post_data_form.name+"'>";
  output    +=   "<span class='qw-setting-title'>"+title+"</span> : ";
  output    +=   "<span class='qw-setting-value'>"+post_data_form.name+"</span>";
  output    += "</div>";

  jQuery('#qw-query-'+post_data_form.handler+'s-list').append(output);
}

/*
 * Dynamically set the setting title for updated fields
 */
QueryWrangler.set_setting_title = function(){
  //gather all info
  var form = jQuery('#'+QueryWrangler.current_form_id);
  var settings = jQuery('div[title='+QueryWrangler.current_form_id+']');
  var title_target = settings.children('.qw-setting-value');
  var fields = jQuery(form).find('.qw-js-title');
  var new_title = [];
  
  jQuery.each(fields, function(i, element){
    var element_tag = jQuery(element).prop('tagName');
    var element_value = jQuery(element).val();
    
    switch (element_tag){
      case 'INPUT':
        var tag_type = jQuery(element).attr('type');
        // checkboxes are special!
        if (tag_type == 'checkbox'){
          var is_checked = jQuery(element).prop('checked');
          if (!element_value) {
            element_value = (is_checked) ? "on" : "off";
          }
          else if (!is_checked){
            element_value = '';
          }
        }
        
        if (element_value){
          new_title.push(element_value);
        }
        break;
      
      case 'TEXTAREA':
        if (element_value){
          new_title.push('In Use');
        }
        break;
      
      case 'SELECT':
        new_title.push(element_value);
        break;
    }
  });
  
  // no items found
  if (new_title.length == 0){
    new_title.push('None');
  }
  // title array to string
  new_title = new_title.join(', ');
  // set new title
  title_target.text(new_title);
  
  if (QueryWrangler.set_modified){
    title_target.parent().addClass('qw-modified');
  }
}

/*
 * Add new handler
 */
QueryWrangler.add_item = function(dialog){
  var handler = jQuery(dialog).children('input.add-handler-type').val();
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
}

/*
 * Activate accordions
 */
QueryWrangler.theme_accordions = function(){
  if (jQuery('#display-style-settings, #row-style-settings, .qw-sortable-list').hasClass('is-accordion')){
    jQuery('#display-style-settings, #row-style-settings, .qw-sortable-list')
      .removeClass('is-accordion')
      .accordion('destroy');
  }
  jQuery('#display-style-settings, #row-style-settings, .qw-sortable-list')
    .accordion({
      header: '> div > .qw-setting-header',
			heightStyle: "content",
      collapsible: true,
      active: false,
      autoHeight: false
  }).addClass('is-accordion');
}

/*
 * Handle sortable lists that are empty
 */
QueryWrangler.toggle_empty_lists = function(){
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

/*
 * Update button
 */
QueryWrangler.button_update = function(dialog){
  var is_handler = false;
  var form = jQuery('#'+QueryWrangler.current_form_id);

  // handlers have special needs
  jQuery.each(QueryWrangler.handlers, function(i,handler){
    if (form.attr('id') == 'qw-display-add-'+handler+'s'){
      is_handler = true;
      return;
    }
  });

  // sortable handlers
  if (is_handler){
    QueryWrangler.add_item(dialog);
  }
  // normal titles
  else {
    // set the title for the updated field
    QueryWrangler.set_setting_title();
  }
  // clear the current form id
  QueryWrangler.current_form_id = '';

  // preview
  if(jQuery('#live-preview').is(':checked')){
    QueryWrangler.get_preview();
  }
  // changes
  if (QueryWrangler.changes == false){
    QueryWrangler.changes = true;
    jQuery('.qw-changes').show();
  }
  //jQuery(dialog).dialog('close');
}

/*
 * Cancel button
 */
QueryWrangler.button_cancel = function(dialog){
  if(QueryWrangler.current_form_id != ''){
    // set backup_form
    jQuery('form#qw-edit-query-form').unserializeForm(QueryWrangler.form_backup);
  }
}

/*
 * Build a sortable list from the qw-query-titles
 */
QueryWrangler.sortable_list_build = function(element){
  QueryWrangler.current_form_id = jQuery(element).closest('.qw-query-admin-options').attr('id');

	//http://stackoverflow.com/questions/8873946/jquery-ui-dialog-height
	var wWidth = jQuery(window).width();
	var wHeight = jQuery(window).height();
  
  var output = '<ul id="'+QueryWrangler.current_form_id+'-sortable" class="qw-hidden">';
  jQuery('#'+QueryWrangler.current_form_id+'-list .qw-query-title')
    .each(function(i, element){
      html = jQuery(element).wrap('<div>').parent().html();
      output+= '<li class="qw-sortable ui-helper-reset ui-state-default ui-corner-all">'+html+'</li>';
    });
  output+= '</ul>';

  jQuery(output).appendTo('#qw-options-forms');
  jQuery('#'+QueryWrangler.current_form_id+'-sortable')
    .sortable()
    .dialog({
      modal: true,
      width: (wWidth * 0.7),
      height: (wHeight * 0.8),
      title: jQuery(element).text(),
			open: function() {
				jQuery(this).dialog("option", "position", "center");
			},
      close: function() {
        QueryWrangler.sortable_list_destroy(this);
      },
      buttons: [{
        text: 'Update',
        click: function() {
          QueryWrangler.sortable_list_update(this);
          QueryWrangler.sortable_list_destroy(this);
        }
      },{
        text: 'Cancel',
        click: function() {
          QueryWrangler.sortable_list_destroy(this);
        }
      }]
    });
}

/*
 * Destroy sortable list
 */
QueryWrangler.sortable_list_destroy = function(dialog){
  jQuery(dialog).dialog('destroy');
}

/*
 * Rebuild the list of handler items
 */
QueryWrangler.sortable_list_update = function(dialog){
  list_id = QueryWrangler.current_form_id+"-list";

  // empty list
  jQuery('#'+list_id).html('');
  // loop through to repopulate list and update weights
  var items = jQuery(dialog).children('.qw-sortable');
  jQuery(items).each(function(i, item){
    // repopulate list
    jQuery('#'+list_id).append('<div class="qw-modified">'+jQuery(item).html()+'</div>');

    // update weight
    var form_id = jQuery(item).children('.qw-query-title').attr('title');
    // kitchen sink
    jQuery('#'+form_id).find('.qw-weight').val(i).attr('value', i);
  });
	jQuery('.qw-changes').show();
  //QueryWrangler.sortable_list_update_weights(list_id);
}

/*
 * Update handler item weights
 */
QueryWrangler.sortable_list_update_weights = function(list_id){
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
 * Init()
 */
jQuery(document).ready(function(){

  QueryWrangler.theme_accordions();
  QueryWrangler.toggle_empty_lists();
  QueryWrangler.sortable_list_update_weights();

  jQuery('.qw-rearrange-title').click(function(){
    QueryWrangler.sortable_list_build(this);
  });

  jQuery('#qw-query-admin-options-wrap').delegate('.qw-query-title', 'click', function(event){
    // backup the form
    QueryWrangler.form_backup = jQuery('form#qw-edit-query-form').serialize();    
    QueryWrangler.current_form_id = jQuery(this).attr('title');
    
    var dialog_title = jQuery(this).text().split(':');
		var wWidth = jQuery(window).width();
		var wHeight = jQuery(window).height();
		var clicked_element_title = jQuery(event.currentTarget).attr('title');
    var handler = '';
    
    // find the handler involved
    if (clicked_element_title.indexOf('-field-') > 0){
      handler = 'field';
    }
    else if (clicked_element_title.indexOf('-filter-') > 0){
      handler = 'filter';
    }
    else if (clicked_element_title.indexOf('-sort-') > 0){
      handler = 'sort';
    }
    else if (clicked_element_title.indexOf('-basic-') > 0){
      handler = 'basic';
    }
    else if (clicked_element_title.indexOf('-override-') > 0){
      handler = 'override';
    }
    
    jQuery('#'+QueryWrangler.current_form_id).dialog({
      modal: true,
      width: (wWidth * 0.7),
      height: (wHeight * 0.8),
      title: dialog_title[0],
			open: function() {
				jQuery(this).dialog("option", "position", "center");
			},			
      resizable: false,
      close: function() {
        QueryWrangler.restore_form(this, handler);
        QueryWrangler.button_cancel(this);
      },
      buttons: [{
        text: 'Update',
        click: function() {
          QueryWrangler.restore_form(this, handler);
          QueryWrangler.button_update(this);
        }
      },{
        text: 'Cancel',
        click: function() {
          QueryWrangler.restore_form(this, handler);
          QueryWrangler.button_cancel(this);
        }
      }]
    }).dialog('open');
  });

  jQuery('body.wp-admin').delegate('.qw-remove', 'click', function(){
    var title = jQuery(this).closest('.ui-dialog-content').attr('id');
    // remove dialog
    jQuery(this).closest('.ui-dialog').remove();
    // remove form item
    jQuery('#'+title).remove();
    // remove list item
    jQuery('.qw-query-title[title='+title+']').remove();
    QueryWrangler.toggle_empty_lists();
  });

  // fix settings titles
  QueryWrangler.set_modified = false;
  jQuery('.qw-query-title').each(function(i,element){
    QueryWrangler.current_form_id = jQuery(this).attr('title');
    QueryWrangler.set_setting_title();
  });
  QueryWrangler.set_modified = true;

});
