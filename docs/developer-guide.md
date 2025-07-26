# Query Wrangler Developer Guide

## Overview

Query Wrangler is a WordPress plugin that provides an intuitive interface for creating complex WP queries as shortcodes, widgets, and page overrides. This guide covers the plugin's architecture, extension points, and development patterns.

## Table of Contents

- [Plugin Architecture](#plugin-architecture)
- [Core Classes](#core-classes)
- [Handler System](#handler-system)
- [Database Schema](#database-schema)
- [Hooks and Filters](#hooks-and-filters)
- [Template System](#template-system)
- [Creating Custom Components](#creating-custom-components)

## Plugin Architecture

### File Structure

```
query-wrangler/
├── admin/                    # Admin interface files
│   ├── css/                 # Admin stylesheets
│   ├── js/                  # Admin JavaScript
│   ├── templates/           # Admin form templates
│   └── editors/             # Theme editors (Picnic, Views)
├── includes/                # Core functionality
│   ├── basics/              # Basic query settings
│   ├── fields/              # Field handlers
│   ├── filters/             # Filter handlers
│   ├── sorts/               # Sort handlers
│   ├── overrides/           # Override handlers
│   └── class-*.php          # Core classes
├── templates/               # Output templates
└── docs/                    # Documentation
```

### Initialization Flow

1. **Frontend Init** (`qw_init_frontend()` - line 49)
   - Loads core classes and handlers
   - Registers shortcodes
   - Includes template system
   - Sets up hooks

2. **Admin Init** (`qw_admin_init()` - line 134)
   - Loads admin interface
   - Sets up AJAX handlers
   - Registers admin scripts/styles

3. **Menu Setup** (`qw_menu()` - line 177)
   - Creates admin menu pages
   - Sets up capability checks

## Core Classes

### QW_Settings

**Location:** `includes/class-qw-settings.php`

Singleton class managing plugin settings with automatic migration from old option format.

```php
$settings = QW_Settings::get_instance();
$value = $settings->get('edit_theme', 'views');
$settings->set('custom_setting', 'value');
$settings->save();
```

**Key Methods:**
- `get($key, $default)` - Retrieve setting value
- `set($key, $value, $sanitize_callback)` - Set setting value
- `save()` - Persist settings to database

### QW_Query

**Location:** `includes/class-qw-query.php`

Core query class handling the complete query lifecycle from database retrieval to HTML output.

```php
$query = new QW_Query($id);
$query->execute(); // Process and execute query
echo $query->output; // Display results
$query->reset_postdata(); // Clean up
```

**Key Properties:**
- `$wp_query` - Generated WP_Query object
- `$options` - Processed query configuration
- `$args` - WP_Query arguments array
- `$output` - Final HTML output

**Key Methods:**
- `execute()` - Complete query processing pipeline
- `process_options()` - Convert data to options and args
- `execute_query()` - Create WP_Query instance
- `theme_query()` - Generate HTML output
- `add_handler_item($type, $item_type, $values)` - Dynamically add handlers

### Handler System

**Location:** `includes/handlers.php`

The handler system provides a unified interface for managing fields, filters, sorts, and overrides.

**Handler Types:**
- **Field** - Display components (title, content, meta fields, etc.)
- **Filter** - Query filters (category, author, meta queries, etc.)
- **Sort** - Sorting options (date, title, meta values, etc.)
- **Override** - Page/archive overrides

**Handler Structure:**
```php
$handlers['field'] = array(
    'title' => 'Field',
    'description' => 'Select Fields to add to this query output.',
    'data_callback' => 'qw_handler_field_data',
    'all_callback' => 'qw_all_fields',
    'form_prefix' => '[display][field_settings][fields]',
    'wrapper_template' => 'query_field',
);
```

## Database Schema

### query_wrangler Table

**Columns:**
- `id` (mediumint) - Primary key
- `name` (varchar 255) - Human readable title
- `slug` (varchar 255) - Machine-safe identifier
- `type` (varchar 16) - Query type: 'widget', 'page', 'override'
- `path` (varchar 255) - Page route (for page type)
- `data` (text) - Serialized query configuration

### query_override_terms Table

**Columns:**
- `query_id` (mediumint) - Reference to query_wrangler.id
- `term_id` (bigint) - WordPress term ID for overrides

## Hooks and Filters

### Core Filters

**`qw_pre_query`** - Modify WP_Query arguments before execution
```php
add_filter('qw_pre_query', function($args, $options) {
    // Modify $args before WP_Query
    return $args;
}, 10, 2);
```

**`qw_pre_render`** - Modify options before templating
```php
add_filter('qw_pre_render', function($options, $wp_query) {
    // Modify display options
    return $options;
}, 10, 2);
```

**`qw_handlers`** - Register custom handlers
```php
add_filter('qw_handlers', function($handlers) {
    $handlers['custom'] = array(
        'title' => 'Custom Handler',
        'data_callback' => 'custom_handler_data',
        'all_callback' => 'all_custom_handlers',
        // ...
    );
    return $handlers;
});
```

### Template System Integration

**`tw_templates`** - Register templates (Template Wrangler)
```php
add_filter('tw_templates', function($templates) {
    $templates['my_template'] = array(
        'files' => 'path/to/template.php',
        'default_path' => __DIR__,
    );
    return $templates;
});
```

## Template System

Query Wrangler uses Template Wrangler for flexible theming. Templates are organized hierarchically:

### Output Templates (`templates/`)
- `query-complete.php` - Full post display
- `query-excerpt.php` - Excerpt display
- `query-table.php` - Tabular format
- `query-unordered_list.php` - List format

### Admin Templates (`admin/templates/`)
- `handler-*.php` - Handler form wrappers
- `form-*.php` - Admin forms
- `page-*.php` - Admin pages

## Creating Custom Components

### Custom Field Handler

1. **Create field file:** `includes/fields/my_custom_field.php`
```php
add_filter('qw_fields', 'my_custom_field');

function my_custom_field($fields) {
    $fields['my_field'] = array(
        'title' => 'My Custom Field',
        'description' => 'Custom field description',
        'form_callback' => 'my_field_form',
        'query_callback' => 'my_field_query',
    );
    return $fields;
}

function my_field_form($field) {
    // Output form HTML
    echo '<input type="text" name="' . $field['form_prefix'] . '[custom_value]" />';
}

function my_field_query($field, $post) {
    // Return field output for display
    return get_post_meta($post->ID, $field['values']['custom_value'], true);
}
```

2. **Include in init:** Add to `qw_init_frontend()`
```php
include_once QW_PLUGIN_DIR . '/includes/fields/my_custom_field.php';
```

### Custom Filter Handler

1. **Create filter file:** `includes/filters/my_custom_filter.php`
```php
add_filter('qw_filters', 'my_custom_filter');

function my_custom_filter($filters) {
    $filters['my_filter'] = array(
        'title' => 'My Custom Filter',
        'description' => 'Custom filter description',
        'form_callback' => 'my_filter_form',
        'query_callback' => 'my_filter_query',
    );
    return $filters;
}

function my_filter_form($filter) {
    // Output form HTML for configuration
}

function my_filter_query($filter, $args) {
    // Modify $args based on filter values
    return $args;
}
```

### Custom Sort Handler

```php
add_filter('qw_sorts', 'my_custom_sort');

function my_custom_sort($sorts) {
    $sorts['my_sort'] = array(
        'title' => 'My Custom Sort',
        'query_callback' => 'my_sort_query',
    );
    return $sorts;
}

function my_sort_query($sort, $args) {
    $args['orderby'] = 'my_custom_field';
    $args['order'] = $sort['values']['direction'];
    return $args;
}
```

## Development Best Practices

### Code Standards
- Follow WordPress coding standards
- Use proper sanitization for user input
- Implement capability checks for admin functions
- Use nonces for form security

### Performance
- Cache expensive operations
- Use transients for temporary data
- Minimize database queries
- Optimize template loading

### Security
- Sanitize all user input
- Use WordPress nonces
- Implement proper capability checks
- Escape output appropriately

## Debugging

### Debug Mode
Enable debugging by setting:
```php
define('WP_DEBUG', true);
define('QW_DEBUG', true); // Custom debug flag
```

### Logging Query Arguments
```php
add_filter('qw_pre_query', function($args, $options) {
    error_log('QW Query Args: ' . print_r($args, true));
    return $args;
}, 10, 2);
```

### Template Debugging
```php
add_filter('qw_pre_render', function($options, $wp_query) {
    error_log('QW Options: ' . print_r($options, true));
    return $options;
}, 10, 2);
```

## Migration and Upgrades

The plugin includes automatic migration for settings and data structures. Version checking occurs in `qw_check_version()` with upgrade scripts in `upgrade.php`.

### Adding Migration Code
```php
function qw_upgrade_to_version_x() {
    // Migration logic
    update_option('qw_version', 'x.x.x');
}
```

## Contributing

When contributing to Query Wrangler:

1. Follow existing code patterns
2. Add documentation for new features
3. Include examples in `docs/examples/`
4. Update this developer guide
5. Test with multiple WordPress versions
6. Ensure backward compatibility