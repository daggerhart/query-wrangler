
// Unserialize (to) form plugin - by Christopher Thielen
// adapted and desuckified (a little) by Paul Irish

// takes a GET-serialized string, e.g. first=5&second=3&a=b and sets input tags (e.g. input name="first") to their values (e.g. 5)


(function($) {
  $.fn.unserializeForm = function(values) {
    if (!values) {
      return this;
    }
    values = values.split("&");

    var serialized_values = [];
    $.each(values, function() {
      var properties = this.split("=");

      if ((typeof properties[0] != 'undefined') && (typeof properties[1] != 'undefined')) {
          serialized_values[properties[0].replace(/\+/g, " ")] = properties[1].replace(/\+/g, " ");
      }
    });

    values = serialized_values;

    $(this).find(":input").removeAttr('checked').each(function() {
      var tag_name = $(this).attr("name");
      if (values[tag_name] !== undefined) {
        if ($(this).attr("type") == "checkbox") {
          $(this).attr("checked", "checked");
        } else {
          $(this).val(values[tag_name]);
        }
      }
    });
  }
})(jQuery);