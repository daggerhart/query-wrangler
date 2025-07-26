# Query Wrangler Hooks and Filters Reference

## Overview

Query Wrangler provides an extensive hook system for customizing and extending functionality. This document covers all available hooks, filters, and their usage patterns.

## Core Filter Hooks

### Handler Registration Hooks

#### `qw_handlers`
Register custom handler types (fields, filters, sorts, overrides).

**Parameters:**
- `$handlers` (array) - Array of handler definitions

**Example:**
```php
add_filter('qw_handlers', function($handlers) {
    $handlers['custom_handler'] = array(
        'title' => 'Custom Handler',
        'description' => 'Custom handler description',
        'data_callback' => 'custom_handler_data',
        'all_callback' => 'all_custom_handlers',
        'form_prefix' => '[custom]',
        'wrapper_template' => 'custom_handler',
    );
    return $handlers;
});
```

#### `qw_fields`
Register field handlers for displaying post data.

**Parameters:**
- `$fields` (array) - Array of field definitions

**Example:**
```php
add_filter('qw_fields', function($fields) {
    $fields['custom_field'] = array(
        'title' => 'Custom Field',
        'description' => 'Display custom post data',
        'form_callback' => 'custom_field_form',
        'query_callback' => 'custom_field_output',
    );
    return $fields;
});
```

#### `qw_filters`
Register filter handlers for modifying query arguments.

**Parameters:**
- `$filters` (array) - Array of filter definitions

**Example:**
```php
add_filter('qw_filters', function($filters) {
    $filters['custom_filter'] = array(
        'title' => 'Custom Filter',
        'description' => 'Filter posts by custom criteria',
        'form_callback' => 'custom_filter_form',
        'query_callback' => 'custom_filter_args',
    );
    return $filters;
});
```

#### `qw_sort_options`
Register sorting options for query results.

**Parameters:**
- `$sort_options` (array) - Array of sort definitions

**Example:**
```php
add_filter('qw_sort_options', function($sorts) {
    $sorts['custom_sort'] = array(
        'title' => 'Custom Sort',
        'orderby_key' => 'orderby',
        'order_key' => 'order',
        'query_callback' => 'custom_sort_args',
    );
    return $sorts;
});
```

#### `qw_overrides`
Register override handlers for replacing WordPress default pages.

**Parameters:**
- `$overrides` (array) - Array of override definitions

**Example:**
```php
add_filter('qw_overrides', function($overrides) {
    $overrides['custom_override'] = array(
        'title' => 'Custom Override',
        'description' => 'Override specific page types',
        'form_callback' => 'custom_override_form',
        'process_callback' => 'custom_override_process',
    );
    return $overrides;
});
```

### Query Processing Hooks

#### `qw_pre_query`
Modify WP_Query arguments before query execution.

**Parameters:**
- `$args` (array) - WP_Query arguments
- `$options` (array) - Query Wrangler options

**Example:**
```php
add_filter('qw_pre_query', function($args, $options) {
    // Modify query arguments
    if (isset($options['custom_setting'])) {
        $args['meta_query'][] = array(
            'key' => 'custom_meta',
            'value' => $options['custom_setting'],
        );
    }
    return $args;
}, 10, 2);
```

#### `qw_pre_render`
Modify options before template rendering.

**Parameters:**
- `$options` (array) - Query options
- `$wp_query` (WP_Query) - Query results

**Example:**
```php
add_filter('qw_pre_render', function($options, $wp_query) {
    // Add custom data for templates
    $options['custom_data'] = calculate_custom_data($wp_query);
    return $options;
}, 10, 2);
```

### Template and Display Hooks

#### `qw_styles`
Register display styles for query output.

**Parameters:**
- `$styles` (array) - Array of style definitions

**Example:**
```php
add_filter('qw_styles', function($styles) {
    $styles['custom_style'] = array(
        'title' => 'Custom Style',
        'description' => 'Custom display format',
        'template' => 'custom-template',
    );
    return $styles;
});
```

#### `qw_row_styles`
Register row display styles.

**Parameters:**
- `$row_styles` (array) - Array of row style definitions

#### `qw_post_types`
Register available post types for queries.

**Parameters:**
- `$post_types` (array) - Array of post type names

### Meta Value Display Handlers

#### `qw_meta_value_display_handlers`
Register handlers for displaying meta field values.

**Parameters:**
- `$handlers` (array) - Array of display handler definitions

**Built-in Handlers:**
- `none` - Basic `get_post_meta()` output
- `acf_default` - Advanced Custom Fields integration
- `cctm_default` - Custom Content Type Manager integration

**Example:**
```php
add_filter('qw_meta_value_display_handlers', function($handlers) {
    $handlers['custom_meta'] = array(
        'title' => 'Custom Meta Handler',
        'callback' => 'custom_meta_display',
    );
    return $handlers;
});

function custom_meta_display($post, $field) {
    $value = get_post_meta($post->ID, $field['meta_key'], true);
    return custom_format_value($value);
}
```

## Built-in Field Types

### Standard Fields
- `post_title` - Post title
- `post_content` - Post content
- `post_excerpt` - Post excerpt  
- `post_date` - Publication date
- `post_author` - Author name
- `post_author_avatar` - Author avatar image
- `featured_image` - Featured/thumbnail image
- `file_attachment` - File attachment links
- `image_attachment` - Image attachment display
- `taxonomy_terms` - Category/tag/taxonomy terms
- `meta_value` - Custom field values
- `callback_field` - Custom PHP callback output

### Field Handler Structure
```php
$fields['field_name'] = array(
    'title' => 'Field Display Name',
    'description' => 'Field description',
    'form_callback' => 'field_form_function',    // Admin form
    'query_callback' => 'field_output_function', // Display output
    'form_template' => 'template_name',          // Alternative to form_callback
);
```

## Built-in Filter Types

### Standard Filters
- `author` - Filter by post author
- `categories` - Filter by categories
- `tags` - Filter by tags
- `taxonomies` - Filter by custom taxonomies
- `post_types` - Filter by post type
- `post_id` - Filter by specific post IDs
- `post_parent` - Filter by parent post
- `meta_key` - Filter by meta key existence
- `meta_key_value` - Filter by meta key/value pairs
- `meta_query` - Complex meta queries
- `meta_value` - Filter by meta value
- `search` - Text search filter
- `callback` - Custom callback filter

### Filter Handler Structure
```php
$filters['filter_name'] = array(
    'title' => 'Filter Display Name',
    'description' => 'Filter description',
    'form_callback' => 'filter_form_function',    // Admin form
    'query_callback' => 'filter_query_function',  // Query modification
    'exposed_form_callback' => 'exposed_form',    // Frontend form (optional)
);
```

## Template Wrangler Integration

#### `tw_templates`
Register templates with Template Wrangler system.

**Example:**
```php
add_filter('tw_templates', function($templates) {
    $templates['custom_query_template'] = array(
        'files' => 'templates/custom-template.php',
        'default_path' => plugin_dir_path(__FILE__),
        'arguments' => array(
            'posts' => array(),
            'options' => array(),
        ),
    );
    return $templates;
});
```

## Action Hooks

### AJAX Actions
- `wp_ajax_qw_form_ajax` - Admin form AJAX handler
- `wp_ajax_qw_data_ajax` - Data retrieval AJAX handler

### WordPress Integration
- `admin_menu` - Menu registration (`qw_menu`)
- `admin_init` - Admin initialization (`qw_admin_init`)
- `init` - Frontend initialization (`qw_init_frontend`)

## Callback Function Patterns

### Field Callback Pattern
```php
function custom_field_output($field, $post) {
    // $field contains field configuration
    // $post is the current WP_Post object
    
    $output = get_post_meta($post->ID, $field['values']['meta_key'], true);
    return apply_filters('custom_field_output', $output, $field, $post);
}
```

### Filter Callback Pattern
```php
function custom_filter_query($filter, $args) {
    // $filter contains filter configuration
    // $args is the WP_Query arguments array
    
    if (!empty($filter['values']['custom_value'])) {
        $args['meta_query'][] = array(
            'key' => 'custom_key',
            'value' => $filter['values']['custom_value'],
            'compare' => '='
        );
    }
    
    return $args;
}
```

### Form Callback Pattern
```php
function custom_handler_form($handler) {
    // $handler contains handler configuration
    // Output form HTML directly
    
    $prefix = $handler['form_prefix'];
    $values = isset($handler['values']) ? $handler['values'] : array();
    
    echo '<input type="text" name="' . $prefix . '[custom_field]" value="' . 
         esc_attr($values['custom_field'] ?? '') . '" />';
}
```

## Advanced Hook Usage

### Conditional Handler Registration
```php
add_action('init', function() {
    // Only register if certain conditions are met
    if (class_exists('CustomPlugin')) {
        add_filter('qw_fields', 'register_custom_plugin_fields');
    }
});
```

### Handler Dependencies
```php
add_filter('qw_fields', function($fields) {
    // Check for required functions/classes
    if (function_exists('custom_function')) {
        $fields['dependent_field'] = array(
            'title' => 'Dependent Field',
            'form_callback' => 'dependent_field_form',
            'query_callback' => 'dependent_field_output',
        );
    }
    return $fields;
});
```

### Dynamic Handler Configuration
```php
add_filter('qw_filters', function($filters) {
    // Get available taxonomies
    $taxonomies = get_taxonomies(array('public' => true), 'objects');
    
    foreach ($taxonomies as $taxonomy) {
        $filters['tax_' . $taxonomy->name] = array(
            'title' => $taxonomy->label . ' Filter',
            'form_callback' => 'taxonomy_filter_form',
            'query_callback' => 'taxonomy_filter_query',
            'taxonomy' => $taxonomy->name,
        );
    }
    
    return $filters;
});
```

## Error Handling and Validation

### Handler Validation
```php
add_filter('qw_fields', function($fields) {
    $fields['validated_field'] = array(
        'title' => 'Validated Field',
        'form_callback' => 'validated_field_form',
        'query_callback' => 'validated_field_output',
        'validate_callback' => 'validated_field_validate',
    );
    return $fields;
});

function validated_field_validate($values) {
    $errors = array();
    
    if (empty($values['required_field'])) {
        $errors[] = 'Required field is missing';
    }
    
    return $errors;
}
```

## Performance Considerations

### Caching Handler Results
```php
function expensive_field_output($field, $post) {
    $cache_key = 'custom_field_' . $post->ID;
    $cached = get_transient($cache_key);
    
    if ($cached !== false) {
        return $cached;
    }
    
    $result = expensive_calculation($post);
    set_transient($cache_key, $result, HOUR_IN_SECONDS);
    
    return $result;
}
```

### Lazy Loading
```php
add_filter('qw_fields', function($fields) {
    // Only load expensive fields when needed
    if (is_admin() || is_query_wrangler_context()) {
        $fields = array_merge($fields, load_expensive_fields());
    }
    return $fields;
});
```

This comprehensive hook system allows for extensive customization while maintaining clean separation of concerns and consistent patterns across all handler types.