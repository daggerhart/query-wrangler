=== Query Wrangler ===
Contributors: daggerhart, forrest.livengood
Donate link: http://www.widgetwrangler.com/
Tags: query, pages, widget, admin, widgets, administration, manage, views
Requires at least: 3
Tested up to: 3.3.1
Stable tag: trunk

Query Wrangler provides an intuitive interface for creating complex WP queries as pages or widgets. Based on Drupal Views.

== Description ==

This plugin lets you create new WP queries as pages or widgets.  It also allows you to override the way category and tag pages display.

Query Wrangler's interface is highly intuitive way to create queries and will be second nature for any Drupal Views user.

This plugin will bring extreme flexibility to WordPress users with its ability to create custom queries using the WP_Query class with a user interface.

Additional Plugins

* [Query Slideshow](http://wordpress.org/extend/plugins/query-slideshow/ "Query Slideshow") - Turn your queries into slideshows using jquery.cycle

Some examples of how you would use this plugin include:

* Create a list of pages or posts within a specific category or tag
* Create an image gallery
* Modify the way your category pages look
* Create a category list widget with custom sorting

[Introduction to Query Wrangler](http://www.widgetwrangler.com/forum/query-wrangler/general/intro-query-wrangler "Learn to setup Queries")

== Installation ==

1. Upload `query-wrangler` to the `/wp-content/plugins/` directory
1. Activate the plugin
1. Visit the Query Wrangler Menu to being Creating your custom queries

== Frequently Asked Questions ==

= How do I add Query Pages to my menu? =

The easiest way is to add it as a custom link in the Menus section of your site.

= How do I use query shortcodes? =

Easy, the code you're looking for is like this.   [query id=2] , where the number 2 is the query id.

= What are overrides and how do I use them? =

Overrides allow you to alter the display and information given on category and tag pages.
For a simple example, add a new query and chose the type `override`.  Choose how you want the content to display, then examine the `Override Settings` options.
Select a category or multiple categories to override.   Save the query, then visit that category page.

== Screenshots ==

1. Drupal Views Editor Theme
1. Tabs Editor Theme

== Changelog ==

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

1.5rc8 Fixing sortable
