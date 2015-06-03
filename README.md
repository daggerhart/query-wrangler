# Query Wrangler

Query Wrangler is a WordPress plugin that provides an intuitive interface for creating complex WP queries as shortcodes and widgets. UI based on Drupal Views.

This plugin lets you create new WP queries as widgets, and use shortcodes for to show queries in your content.  Additionally, it allows you to override the way category and tag pages display without editing tempalte files.

Supports:

* Most post data, including meta fields
* Taxonomy data
* Advanced Custom Fields
* Custom Content Type Manager
* Some exposed filters

Some examples of how you would use this plugin include:

* Create a list posts with featured images
* Create a list of pages or posts within a specific category or tag
* Create an image gallery
* Modify the way your category pages look


## Frequently Asked Questions

### How do I use query shortcodes?

Easy, the code you're looking for is like this. `[query id=2]` , where the number 2 is the query id. When viewing the list of queries each one will have 2 options for shortcode. One that uses the query's id, and the other using the query's slug.  I recommend using the query's slug for future maintainability.

* By id: `[query id=2]`
* By slug: `[query slug="my-query"]`
* Customize WP_Query arguments: `[query id=2 args="posts_per_page=1&post_type=page"]`
* Customize WP_Query arguments with contextual data: `[query id=1 args="author={{post:post_author}}&post_type={{post:post_type}}"]`

### What are overrides and how do I use them?

Overrides allow you to alter the display and information given on category and tag archive pages.

For a simple example, add a new query and chose the type `override`.  Choose how you want the content to display, then examine the `Overrides` box in the center column. From there you can add taxonomies and terms this query will override.


### Other

* [Meta Field Display Handler: Advanced Custom Fields](https://wordpress.org/support/topic/how-to-get-he-image-url-instead-of-image-id?replies=5#post-5411991 "Meta Field Display Handler: Advanced Custom Fields")
* [Sort by Meta value](https://wordpress.org/support/topic/orderby-meta_value-1#post-6719953 "Sort a query by meta values")
* [Meta Field Display Handler: Custom Content Type Manager](http://wordpress.org/support/topic/cant-put-php-code-into-rewrite-results-field?replies=4#post-5411970 "Meta Field Display Handler: Custom Content Type Manager")
* [Custom field example](https://gist.github.com/daggerhart/10417309 "Custom field example")
* [Field callback usage: wp_get_attachment_url](http://wordpress.org/support/topic/add-php-in-rewrite-output-of-this-field?replies=3#post-5480638 "Callback field usage")
* [Field callback usage: the_tags](http://wordpress.org/support/topic/callback-field-plugin-version-1524?replies=1#post-5487515 "Callback field usage 2")
* [Filter callback usage: ](https://wordpress.org/support/topic/filter-by-subquery?replies=3#post-6719945 "Filter Callback usage")
* [Query Slideshow](http://wordpress.org/extend/plugins/query-slideshow/ "Query Slideshow") - Example of creating a custom style. Turn your queries into slideshows using jquery.cycle

