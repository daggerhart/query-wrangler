// https://github.com/cthielen/unserialize-to-form/blob/master/jQuery.unserializeForm.js
// Unserialize (to) form plugin
// Version 1.0.4
// Copyright (C) 2010-2011 Christopher Thielen, others (see ChangeLog below)
// Dual-licensed under GPLv2 and the MIT open source licenses

// Notes: Will recurse fieldsets and p tags as they are commonly used in forms.
//        Form elements must have a 'name' attribute

// Usage: var s = $("form").serialize(); // save form settings
//        $("form").unserializeForm(s);  // restore form settings

//        Alternatively, you can provide a second parameter to unserializeForm,
//        a callback which takes the element and the value, allowing you to build
//        dynamic forms via callback. If you return false, unserializeForm will
//        try to find and set the DOM element, otherwise, (on true) it assumes you've
//        handled that attribute and moves onto the next. E.g.:
//
//               var callback = function(el, val) { $(el).val(val); };

// See ChangeLog at end of file for history.

(function($) {
	// takes a GET-serialized string, e.g. first=5&second=3&a=b and sets input tags (e.g. input name="first") to their values (e.g. 5)
	$.fn.unserializeForm = function( _values, _callback ) {
		// this small bit of unserializing borrowed from James Campbell's "JQuery Unserialize v1.0"
		_values = _values.split("&");
		
		if(_callback && typeof(_callback) !== "function") {
			_callback = undefined; // whatever they gave us wasn't a function, act as though it wasn't given
		}
		
		var serialized_values = new Array();
		$.each(_values, function() {
			var properties = this.split("=");
			
			if((typeof properties[0] != 'undefined') && (typeof properties[1] != 'undefined')) {
				serialized_values[properties[0].replace(/\+/g, " ")] = properties[1].replace(/\+/g, " ");
			}
		});
		
		// _values is now a proper array with values[hash_index] = associated_value
		_values = serialized_values;
		
		// Start with all checkboxes and radios unchecked, since an unchecked box will not show up in the serialized form
		$(this).find(":checked").attr("checked", false);
		
		// Iterate through each saved element and set the corresponding element
		for(var key in _values) {
			var el = $(this).add("input,select,textarea").find("[name=\"" + unescape(key) + "\"]");
			var _value = unescape(_values[key]);
			
			if(_callback == undefined) {
				// No callback specified - assume DOM elements exist
				_unserializeFormSetValue(el, _value);
			} else {
				// Callback specified - don't assume DOM elements already exist
				var result = _callback.call(this, unescape(key), _value);
				
				// If they return true, it means they handled it. If not, we will handle it.
				// Returning false then allows for DOM building without setting values.
				if(result == false) {
					// Try and find the element again as it may have just been created by the callback
					var el = $(this).add("input,select,textarea").find("[name=\"" + unescape(key) + "\"]");
					_unserializeFormSetValue(el, _value);
				}
			}
		}
	}
})(jQuery);

// Modified to use jQuery noConflict: jonathan
function _unserializeFormSetValue(el, _value) {
	if(jQuery(el).length > 1) {
		// Assume multiple elements of the same name are radio buttons
		jQuery.each(el, function(i) {
			if(jQuery(this).attr("value") == _value) {
				// Check it
				jQuery(this).attr("checked", true);
			} else {
				// Uncheck it
				jQuery(this).attr("checked", false);
			}
		});
	} else {
		// Assume, if only a single element, it is not a radio button
		if(jQuery(el).attr("type") == "checkbox") {
			jQuery(el).attr("checked", true);
		} else {
			jQuery(el).val(_value);
		}
	}
}
