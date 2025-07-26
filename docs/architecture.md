# Query Wrangler Architecture

## Overview

Query Wrangler follows a modular architecture with clear separation between data processing, query generation, and output rendering. The plugin uses a handler-based system for extensibility and Template Wrangler for flexible theming.

## Core Architecture Components

### 1. Query Lifecycle

```
Database Query → Data Processing → WP_Query Generation → Template Rendering → HTML Output
```

**Detailed Flow:**
1. **Load Query Data** - Retrieve stored query configuration from database
2. **Process Options** - Transform raw data into structured options array
3. **Generate Arguments** - Convert options to WP_Query compatible arguments
4. **Execute Query** - Create and run WP_Query instance
5. **Template Output** - Render results using template system
6. **Cleanup** - Reset WordPress global query state

### 2. Handler System Architecture

The handler system provides a unified interface for different query components:

```
Handler Registry → Handler Processing → Form Generation → Query Integration
```

**Handler Types:**
- **Fields** - Control what data is displayed for each post
- **Filters** - Determine which posts are included in results
- **Sorts** - Define result ordering
- **Overrides** - Replace default WordPress archive pages

### 3. Data Flow

```php
// 1. Raw Database Data (serialized)
$raw_data = '{"args":{"filters":{"category":{"type":"categories","values":{"categories":["1","2"]}}}}}'

// 2. Processed Options Array
$options = array(
    'args' => array(
        'filters' => array(
            'category' => array(
                'type' => 'categories',
                'values' => array('categories' => array('1', '2'))
            )
        )
    )
);

// 3. WP_Query Arguments
$args = array(
    'post_type' => 'post',
    'category__in' => array(1, 2),
    'posts_per_page' => 10
);

// 4. WP_Query Instance
$wp_query = new WP_Query($args);

// 5. Template Variables
$template_vars = array(
    'posts' => $wp_query->posts,
    'fields' => $processed_fields,
    'query_meta' => $query_info
);
```

## Core Classes Architecture

### QW_Settings (Singleton)

**Responsibilities:**
- Centralized configuration management
- Automatic migration from legacy options
- Type-safe setting access with defaults

**Pattern:**
```php
class QW_Settings {
    private static $instance = null;
    private $values = array();
    
    public static function get_instance() {
        if (null === self::$instance) {
            self::$instance = new self();
        }
        return self::$instance;
    }
}
```

### QW_Query (Query Builder)

**Responsibilities:**
- Query data management and processing
- WP_Query argument generation
- Template rendering coordination
- Handler integration

**Key Design Patterns:**
- **Fluent Interface** - Method chaining for query building
- **Command Pattern** - Encapsulated query operations
- **Template Method** - Consistent processing pipeline

```php
$query = new QW_Query($id)
    ->override_options($custom_options)
    ->process_options()
    ->execute_query()
    ->theme_query();
```

### Handler Registry System

**Architecture:**
```php
// Handler Definition
$handlers['field'] = array(
    'title' => 'Field',
    'data_callback' => 'qw_handler_field_data',    // Extract data from query
    'all_callback' => 'qw_all_fields',             // Get available items
    'form_prefix' => '[display][field_settings][fields]',
    'wrapper_template' => 'query_field',
);

// Handler Item Definition
$fields['post_title'] = array(
    'title' => 'Post Title',
    'form_callback' => 'field_post_title_form',    // Admin form
    'query_callback' => 'field_post_title_query',  // Output generation
);
```

## Database Architecture

### Query Storage Strategy

**Table: `wp_query_wrangler`**
- Uses JSON-like serialized data storage for flexibility
- Supports complex nested configurations
- Maintains backward compatibility through data migration

**Table: `wp_query_override_terms`**
- Junction table for query-term relationships
- Enables efficient override matching
- Supports multiple terms per override query

### Data Serialization

```php
// Stored Data Structure
$data = array(
    'display' => array(
        'type' => 'unformatted',
        'field_settings' => array(
            'fields' => array(
                'title' => array('type' => 'post_title', 'weight' => 0),
                'content' => array('type' => 'post_content', 'weight' => 1)
            )
        )
    ),
    'args' => array(
        'filters' => array(
            'category' => array('type' => 'categories', 'values' => array(...))
        ),
        'sorts' => array(
            'date' => array('type' => 'post_date', 'direction' => 'DESC')
        )
    )
);
```

## Template System Architecture

### Template Wrangler Integration

Query Wrangler extends Template Wrangler with query-specific templates:

```php
// Template Registration
add_filter('tw_templates', function($templates) {
    $templates['query_complete'] = array(
        'files' => 'templates/query-complete.php',
        'default_path' => QW_PLUGIN_DIR,
    );
    return $templates;
});
```

### Template Hierarchy

1. **Theme Override** - `theme/query-wrangler/template-name.php`
2. **Plugin Template** - `plugin/templates/template-name.php`
3. **Fallback** - Default template or empty output

### Template Variables

Templates receive standardized variables:
- `$posts` - Array of WP_Post objects
- `$fields` - Processed field handlers
- `$query` - QW_Query instance
- `$options` - Query configuration options

## Hook System Architecture

### Filter Chain Processing

```php
// Pre-query filtering
$args = apply_filters('qw_pre_query', $args, $options);

// Query execution
$wp_query = new WP_Query($args);

// Pre-render processing
$options = apply_filters('qw_pre_render', $options, $wp_query);

// Template rendering
$output = qw_template_query($wp_query, $options);
```

### Handler Integration Points

**Field Handlers:**
```php
add_filter('qw_fields', 'my_custom_fields');
function my_custom_fields($fields) {
    $fields['custom'] = array(
        'title' => 'Custom Field',
        'form_callback' => 'custom_field_form',
        'query_callback' => 'custom_field_output',
    );
    return $fields;
}
```

**Filter Handlers:**
```php
add_filter('qw_filters', 'my_custom_filters');
function my_custom_filters($filters) {
    $filters['custom'] = array(
        'title' => 'Custom Filter',
        'form_callback' => 'custom_filter_form',
        'query_callback' => 'custom_filter_args',
    );
    return $filters;
}
```

## Security Architecture

### Input Sanitization

```php
// Settings sanitization
function set($key, $value, $sanitize_callback = 'sanitize_text_field') {
    if (is_callable($sanitize_callback)) {
        $value = call_user_func($sanitize_callback, $value);
    }
    $this->values[$key] = $value;
}
```

### Capability Checks

```php
// Menu registration with capability requirements
add_menu_page(
    'Query Wrangler',
    'Query Wrangler', 
    'manage_options',    // Required capability
    'query-wrangler',
    'qw_page_handler'
);
```

### Nonce Verification

```php
// AJAX security
add_action('wp_ajax_qw_form_ajax', 'qw_form_ajax');
function qw_form_ajax() {
    check_ajax_referer('qw_ajax_nonce');
    // Process request
}
```

## Performance Architecture

### Caching Strategy

- **Transient API** - Temporary data caching
- **Object Caching** - WordPress object cache integration
- **Query Result Caching** - Optional result set caching

### Optimization Patterns

**Lazy Loading:**
```php
// Load handlers only when needed
if (!$this->options) {
    $this->options = $this->data;
}
```

**Efficient Querying:**
```php
// Single query for multiple posts
$wp_query = new WP_Query($optimized_args);
```

## Extension Architecture

### Plugin Extension Points

1. **Handler Registration** - Add new field/filter/sort types
2. **Template Override** - Custom output formatting
3. **Hook Integration** - Modify query processing
4. **Settings Extension** - Additional configuration options

### Theme Integration

```php
// Theme can override any template
// Location: theme/query-wrangler/template-name.php

// Theme can register custom handlers
add_action('after_setup_theme', function() {
    include get_template_directory() . '/query-wrangler-extensions.php';
});
```

## Debugging Architecture

### Error Handling

```php
// Graceful degradation
try {
    $query = new QW_Query($id);
    $output = $query->execute()->output;
} catch (Exception $e) {
    $output = qw_error_template($e->getMessage());
}
```

### Debug Information

```php
// Debug mode output
if (defined('WP_DEBUG') && WP_DEBUG) {
    error_log('QW Debug: ' . print_r($debug_data, true));
}
```

This architecture ensures Query Wrangler remains flexible, extensible, and maintainable while providing robust functionality for complex WordPress queries.