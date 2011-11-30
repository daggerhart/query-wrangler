=== Query Wrangler ===
Contributors: daggerhart, forrest.livengood
Tags: query, pages, widget, admin, widgets, administration, manage, views
Requires at least: 3
Tested up to: 3.2.1
Stable tag: trunk

This plugin lets you create new WP queries as pages or widgets. It's basically Drupal Views for Wordpress.

== Description ==

This plugin lets you create new WP queries as pages or widgets.  It also allows you to override the way category and tag pages display.

Query Wrangler's interface is highly intuitive way to create queries and will be second nature for any Drupal Views user.

This plugin will bring extreme flexibility to WordPress users with its ability to create custom queries using the WP_Query class with a user interface.

Some examples of how you would use this plugin include:

* Create a list of pages or posts with a specific category or tag
* Create an image gallery
* Modify the way your category pages look

== Installation ==

1. Upload `query-wrangler` to the `/wp-content/plugins/` directory
1. Activate the plugin
1. Visit the Query Wrangler Menu to being Creating your custom queries

== Frequently Asked Questions ==

= How do I add Query Pages to my menu? =

The easiest way is to add it as a custom link in the Menus section of your site.

= How do I use query shortcodes? =

Easy, the code you're lookind for looks like this.   [query id=2] , where the number 2 is the query id.

= What are overrides and how do I use them? =

Overrides allow you to alter the display and information given on category and tag pages.
For a simple example, add a new query and chose the type `override`.  Choose what you want to display then examine the `Override Settings` options.
Select a category or multiple categories to override.   Save the query, then visit that category page.

== Screenshots ==

1. Query Wrangler edit screen

== Changelog ==

= 1.3beta1 =

 * Added Wordpress hooks for fields and field styles.
 * Fixed some bugs with replacement tokens and rewriting output
 * Fixed some templates from displaying excluded fields
 * Determined methodology for field callbacks and arguments

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

1.3beta1 Added wordpress hooks, improved fields, lots of other fixes and improvements
