=== Query Wrangler ===
Contributors: daggerhart
Donate link: http://www.widgetwrangler.com/
Tags: query, pages, widget, admin, widgets, administration, manage, views, loop
Requires at least: 3
Tested up to: 4.1
Stable tag: trunk

Query Wrangler provides an intuitive interface for creating complex WP queries as shortcodes and widgets. UI based on Drupal Views.

== Description ==

This plugin lets you create new WP queries as widgets and use shortcodes for queries on your pages.  It also allows you to override the way category and tag pages display.

Query Wrangler's interface is highly intuitive way to create queries and will be second nature for any Drupal Views user.

This plugin will bring extreme flexibility to WordPress users with its ability to create custom queries using the WP_Query class with a user interface.

Supports:

* Most post data, including meta fields
* Taxonomy data
* Advanced Custom Fields
* Custom Content Type Manager
* Some exposed filters

Some examples of how you would use this plugin include:

* Create a list of pages or posts within a specific category or tag
* Create an image gallery
* Modify the way your category pages look
* Create a category list widget with custom sorting

[Introduction to Query Wrangler](http://www.widgetwrangler.com/forum/query-wrangler/general/intro-query-wrangler "Learn to setup Queries")


Additional Plugins

* [Query Slideshow](http://wordpress.org/extend/plugins/query-slideshow/ "Query Slideshow") - Turn your queries into slideshows using jquery.cycle


== Installation ==

1. Upload `query-wrangler` to the `/wp-content/plugins/` directory
1. Activate the plugin
1. Visit the Query Wrangler Menu to begin creating your custom queries

== Frequently Asked Questions ==

= How do I use query shortcodes? =

Easy, the code you're looking for is like this.   [query id=2] , where the number 2 is the query id. This can be found on the Query Wrangler page eside each query.

* By slug: [query slug="my-query"]
* Customize WP_Query arguments: [query id=2 args="posts_per_page=1&post_type=page"]
* Customize WP_Query arguments with contextual data: [query id=1 args="author={{post:post_author}}&post_type={{post:post_type}}"]


= Developer & Usage =

* [Custom field example](https://gist.github.com/daggerhart/10417309 "Custom field example")
* [Callback field usage: wp_get_attachment_url](http://wordpress.org/support/topic/add-php-in-rewrite-output-of-this-field?replies=3#post-5480638 "Callback field usage")
* [Callback field usage: the_tags](http://wordpress.org/support/topic/callback-field-plugin-version-1524?replies=1#post-5487515 "Callback field usage 2")
* [Meta Field Display Handler: Custom Content Type Manager](http://wordpress.org/support/topic/cant-put-php-code-into-rewrite-results-field?replies=4#post-5411970 "Meta Field Display Handler: Custom Content Type Manager")
* [Meta Field Display Handler: Advanced Custom Fields](https://wordpress.org/support/topic/how-to-get-he-image-url-instead-of-image-id?replies=5#post-5411991 "Meta Field Display Handler: Advanced Custom Fields")


= What are overrides and how do I use them? =

Overrides allow you to alter the display and information given on category and tag pages.
For a simple example, add a new query and chose the type `override`.  Choose how you want the content to display, then examine the `Override Settings` options.
Select a category or multiple categories to override.   Save the query, then visit that category page.


== Screenshots ==

1. Drupal Views Editor Theme

== Changelog ==

= 1.5.3 =

* Feature: post content and post excerpt fields have new option to apply the_content filter
* Feature: new sort option: post__in to sort by order of post__in filter array
* New shortcode hooks for custom attributes and query options
* Bug fix: is_array check for missing override value
* Bug fix: category__not_in needs term ids as array

= 1.5.26 =

* Bug fix: Admin UI fix on WP 4.1

= 1.5.25 =

* Bug fix: qw_pre_query & qw_pre_render hooks
* Bug fix: Plugin activation warnings
* Bug fix: Import textarea CSS
* Bug fix: Live Preview error on new query
* Feature: Show Display Title on widget with or without theme compatibility
* Feature: Arguments field on widgets


= 1.5.24 =

* New: Callback field - Use any function to provide a field value.
* New: Shortcode argument contextual tokens.  eg, [query id=1 args="author={{post:post_author}}&post_type={{post:post_type}}"]  - returns the query showing only posts by the author of the current page being viewed, and of the post type of the current post_type being viewed.
* Fix: index on query_override_terms table

= 1.5.23 =

* New: Shortcode arguments.  eg, [query id=1 args="posts_per_page=1&post_type=page"]
* New: Support for "Advanced Custom Fields" fields as settings for meta_values
* New: Support for "Custom Content Type Manager" fields as settings for meta_values

= 1.5rc22 =

* Bug fix: Empty request_uri causing issues.

= 1.5rc21 =

* New: Field - Featured Image
* New: Exposed filter - Post IDs
* New: Exposed filter - Post Parent
* New: Exposed filter - Post Types
* New: Exposed filter - Taxonomies
* New: Setting - Live Preview default
* Fix: Image field style setting

= 1.5rc20 =

* Updating template wrangler to use apply_filters instead of do_action_ref_array

= 1.5rc19 =

* Feature: New author filter

= 1.5rc18 =

* Fix: Pagination bug when using Page numbers.

= 1.5rc17 =

* Fix: post_parent filter

= 1.5rc16 =

* Feature: New taxonomies filter
* Feature: Featured Image setting for "Image Attachment" field
* Fix: Making js better
* Fix: New handler item weight bug
* Fix: New field tokens bug
* Remove: tabs editor
* Refactoring a lot

= 1.5rc15 =

* Compatibility issues with Widget Wrangler
* Feature: Template suggestions in preview.

= 1.5rc14 =

* Fix: Child theme page templating
* Fix: Comment disabling works on pages with shortcodes
* Feature/Fix: Theme compatibility for widgets - enable in QW settings.

= 1.5rc13 =

* Fix: Shortcode & Widget paging - but can't have 2 independent pagers on 1 page yet.
* Fix: Unserialize bug - http://wordpress.org/support/topic/bug-that-wipes-all-settings-of-query-after-excluding-fields-from-display?replies=2
* Fix: 3.7 problem - according to - http://wordpress.org/support/topic/update-has-broken-site#post-4868397
* Fix: Row style settings form bug 
* More complete default query according to WP_Query defaults
* Feature: PHP WP_Query in preview

= 1.5rc12 =
 
 * New page routing with hook parse_request

= 1.5rc11 =
 
 * Chasing WP 3.7 related bugs

= 1.5rc10 =

 * UI fixes and improvements.
 
= 1.5rc9 =

 * Fix: Wordpress 3.7 update redirecting from custom pages

= 1.5rc8 =

 * Fix: Fixing sortable jquery ui issue.  QW UI working w/ WP 3.5
 
= 1.5rc7 =

 * Fix: No longer relying on external jquery sources, working to fix views ui
 
= 1.5rc6 =

 * Fix: WP update broke jquery and jquery ui. now relying on external sources
 
= 1.5rc5 =

 * Fix: bug, javascript sometimes enqueuing in wrong order (Google Libraries)

= 1.5rc4 =

 * Fix: bug, custom label and rewrite output fields not showing up.

= 1.5rc3 =

 * Fix: bug in looking for a query page's path

= 1.5rc2 =

 * Fix: meta field returning array
 * Fix: page links missing starting slash

= 1.5rc1 =

 * Major API enhancements
 * New simpler editor theme
 * Lots of Refactoring

= 1.4.1beta =

 * Template preprocess fix

= 1.4beta =

 * Fixed override pagination
 * UI Improvements
 * Template hierarchy solidification
 * Live Preview
 * Meta Key / Value Filters
 * Filter Api

= 1.3.2beta =

 * Fixed template-wrangler comments bug
 * Fixed display style bug
 * Refactored javascript some

= 1.3beta1 =

 * Added Wordpress hooks for fields, field styles, filters, and pagers
 * Fixed some bugs with replacement tokens and rewriting output
 * Fixed some templates from displaying excluded fields
 * Determined methodology for field callbacks and arguments
 * Shortcode support for fields

= 1.2beta3 =

 * Bug fix for empty category and tag pages
 * Bug fix for query shortcodes

= 1.2beta2 =

 * Added shortcode support

= 1.2beta1 =

 * Added Wordpress page overrides for categories and tags.
 * Fixed query edit page js bug.

= 1.1beta =

 * Bug with canceling forms.  Changed use of jQuery unserializeForm

= 1.0beta =

 * Initial Release

== Upgrade Notice ==

1.5.3 bug fixes, and some new features.
