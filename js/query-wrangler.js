/*
 * Globals
 */
QueryWrangler.current_form_id = '';
QueryWrangler.new_form_id = '';
QueryWrangler.form_backup = '';
// array of available options for looping
QueryWrangler.handlers = ['field','filter','sort'];
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
    'options': QueryWrangler.form_backup,
    'query_id': QueryWrangler.query.id
  };
  // make ajax call
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
 * Init()
 */
jQuery(document).ready(function(){
  // preview
  jQuery('#get-preview').click(QueryWrangler.get_preview);
  QueryWrangler.get_preview();

  // accordions
  jQuery('#query-details').accordion({
      header: '> div > .qw-setting-header',
      collapsible: true,
      active: false,
      autoHeight: false
  });
});