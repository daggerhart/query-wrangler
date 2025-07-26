# Database Schema Documentation

## Overview

Query Wrangler uses two custom database tables to store query configurations and override relationships. This document details the schema structure, data formats, and migration patterns.

## Database Tables

### wp_query_wrangler

The main table storing query configurations and metadata.

**Table Structure:**
```sql
CREATE TABLE wp_query_wrangler (
    id mediumint(9) NOT NULL AUTO_INCREMENT,
    name varchar(255) NOT NULL,
    slug varchar(255) NOT NULL,
    type varchar(16) NOT NULL,
    path varchar(255) DEFAULT NULL,
    data text NOT NULL,
    UNIQUE KEY id (id)
);
```

**Column Definitions:**

| Column | Type | Description | Examples |
|--------|------|-------------|----------|
| `id` | mediumint(9) | Primary key, auto-increment | 1, 2, 3... |
| `name` | varchar(255) | Human-readable query name | "Featured Posts", "Recent Articles" |
| `slug` | varchar(255) | Machine-safe identifier | "featured-posts", "recent-articles" |
| `type` | varchar(16) | Query type | "widget", "page", "override" |
| `path` | varchar(255) | Page route (for page type) | "/custom-page", "/products" |
| `data` | text | Serialized query configuration | PHP serialized array |

**Query Types:**
- **widget** - Can be used as WordPress widget
- **page** - Creates a standalone page 
- **override** - Overrides default WordPress pages (categories, tags, archives)

**Example Records:**
```sql
INSERT INTO wp_query_wrangler VALUES 
(1, 'Featured Posts', 'featured-posts', 'widget', NULL, 'a:3:{s:7:"display";a:2:{s:4:"type";s:11:"unformatted";s:14:"field_settings";a:1:{s:6:"fields";a:2:{s:5:"title";a:3:{s:4:"type";s:10:"post_title";s:6:"weight";i:0;s:8:"hook_key";s:10:"post_title";}s:7:"content";a:3:{s:4:"type";s:12:"post_content";s:6:"weight";i:1;s:8:"hook_key";s:12:"post_content";}}}}s:4:"args";a:2:{s:7:"filters";a:1:{s:8:"featured";a:3:{s:4:"type";s:8:"featured";s:6:"weight";i:0;s:8:"hook_key";s:8:"featured";}}s:5:"sorts";a:1:{s:4:"date";a:4:{s:4:"type";s:9:"post_date";s:6:"weight";i:0;s:8:"hook_key";s:9:"post_date";s:9:"direction";s:4:"DESC";}}}s:8:"override";a:0:{}}'),

(2, 'Category Archive Override', 'category-override', 'override', NULL, 'a:3:{s:7:"display";a:2:{s:4:"type";s:7:"excerpt";s:14:"field_settings";a:1:{s:6:"fields";a:3:{s:5:"title";a:3:{s:4:"type";s:10:"post_title";s:6:"weight";i:0;s:8:"hook_key";s:10:"post_title";}s:7:"excerpt";a:3:{s:4:"type";s:12:"post_excerpt";s:6:"weight";i:1;s:8:"hook_key";s:12:"post_excerpt";}s:4:"date";a:3:{s:4:"type";s:9:"post_date";s:6:"weight";i:2;s:8:"hook_key";s:9:"post_date";}}}}s:4:"args";a:1:{s:5:"sorts";a:1:{s:4:"date";a:4:{s:4:"type";s:9:"post_date";s:6:"weight";i:0;s:8:"hook_key";s:9:"post_date";s:9:"direction";s:4:"DESC";}}}s:8:"override";a:1:{s:10:"categories";a:3:{s:4:"type";s:10:"categories";s:6:"weight";i:0;s:8:"hook_key";s:10:"categories";}}}');
```

### wp_query_override_terms

Junction table linking override queries to specific taxonomy terms.

**Table Structure:**
```sql
CREATE TABLE wp_query_override_terms (
    query_id mediumint(9) NOT NULL,
    term_id bigint(20) NOT NULL,
    UNIQUE KEY query_term (query_id, term_id)
);
```

**Column Definitions:**

| Column | Type | Description |
|--------|------|-------------|
| `query_id` | mediumint(9) | References wp_query_wrangler.id |
| `term_id` | bigint(20) | References wp_terms.term_id |

**Example Records:**
```sql
-- Query ID 2 overrides categories with term IDs 5, 12, and 18
INSERT INTO wp_query_override_terms VALUES 
(2, 5),
(2, 12),
(2, 18);
```

## Data Structure Format

### Query Configuration Array

The `data` column contains a PHP serialized array with the following structure:

```php
$data = array(
    'display' => array(
        'type' => 'unformatted',              // Display template
        'field_settings' => array(
            'fields' => array(
                'title' => array(
                    'type' => 'post_title',
                    'weight' => 0,
                    'hook_key' => 'post_title',
                    // Additional field-specific settings
                ),
                'content' => array(
                    'type' => 'post_content',
                    'weight' => 1,
                    'hook_key' => 'post_content',
                    'content_options' => array(
                        'trim_length' => 200,
                        'more_text' => 'Read more...'
                    )
                )
            )
        ),
        // Display-specific settings
        'header' => 'Query Results',
        'footer' => '',
        'empty' => 'No results found.',
        'template_styles' => array(),
        'row_styles' => array(),
    ),
    
    'args' => array(
        'filters' => array(
            'category' => array(
                'type' => 'categories',
                'weight' => 0,
                'hook_key' => 'categories',
                'values' => array(
                    'cats' => array(5 => 'Technology', 12 => 'Programming'),
                    'cat_operator' => 'category__in'
                )
            ),
            'meta_filter' => array(
                'type' => 'meta_query',
                'weight' => 1,
                'hook_key' => 'meta_query',
                'values' => array(
                    'key' => 'featured',
                    'value' => '1',
                    'compare' => '='
                )
            )
        ),
        
        'sorts' => array(
            'date_sort' => array(
                'type' => 'post_date',
                'weight' => 0,
                'hook_key' => 'post_date',
                'values' => array(
                    'direction' => 'DESC'
                )
            )
        )
    ),
    
    'override' => array(
        'categories' => array(
            'type' => 'categories',
            'weight' => 0,
            'hook_key' => 'categories',
            'values' => array(
                'terms' => array(5, 12, 18)
            )
        )
    )
);
```

### Field Configuration Structure

```php
$field_config = array(
    'type' => 'post_title',           // Field type identifier
    'weight' => 0,                    // Display order
    'hook_key' => 'post_title',       // Handler hook key
    'values' => array(                // Field-specific configuration
        'link_to_post' => true,
        'html_tag' => 'h2',
        'css_class' => 'entry-title'
    )
);
```

### Filter Configuration Structure

```php
$filter_config = array(
    'type' => 'categories',           // Filter type identifier
    'weight' => 0,                    // Processing order
    'hook_key' => 'categories',       // Handler hook key
    'values' => array(                // Filter-specific values
        'cats' => array(              // Selected categories
            5 => 'Technology',
            12 => 'Programming'
        ),
        'cat_operator' => 'category__in'  // Filter operator
    )
);
```

### Meta Field Examples

**Basic Meta Field:**
```php
$meta_field = array(
    'type' => 'meta_value',
    'weight' => 2,
    'hook_key' => 'meta_value',
    'meta_key' => 'custom_field_name',
    'values' => array(
        'meta_value_count' => 1,
        'meta_value_separator' => ', ',
        'display_handler' => 'none'
    )
);
```

**Advanced Custom Fields Integration:**
```php
$acf_field = array(
    'type' => 'meta_value',
    'weight' => 1,
    'hook_key' => 'meta_value',
    'meta_key' => 'gallery_images',
    'values' => array(
        'meta_value_count' => 0,        // 0 = all values
        'display_handler' => 'acf_default',
        'are_image_ids' => true,
        'image_display_style' => 'medium'
    )
);
```

## Database Operations

### Query Retrieval

```php
// Get query by ID
function qw_get_query_by_id($id) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    $query = $wpdb->get_row($wpdb->prepare(
        "SELECT * FROM $table_name WHERE id = %d",
        $id
    ));
    
    if ($query) {
        $query->data = unserialize($query->data);
    }
    
    return $query;
}

// Get query by slug
function qw_get_query_by_slug($slug) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    $query = $wpdb->get_row($wpdb->prepare(
        "SELECT * FROM $table_name WHERE slug = %s",
        $slug
    ));
    
    if ($query) {
        $query->data = unserialize($query->data);
    }
    
    return $query;
}

// Get all queries of a specific type
function qw_get_queries_by_type($type) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    $queries = $wpdb->get_results($wpdb->prepare(
        "SELECT * FROM $table_name WHERE type = %s ORDER BY name ASC",
        $type
    ));
    
    foreach ($queries as $query) {
        $query->data = unserialize($query->data);
    }
    
    return $queries;
}
```

### Query Storage

```php
// Save new query
function qw_save_query($name, $slug, $type, $path, $data) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    $result = $wpdb->insert(
        $table_name,
        array(
            'name' => $name,
            'slug' => $slug,
            'type' => $type,
            'path' => $path,
            'data' => serialize($data)
        ),
        array('%s', '%s', '%s', '%s', '%s')
    );
    
    return $result ? $wpdb->insert_id : false;
}

// Update existing query
function qw_update_query($id, $name, $slug, $type, $path, $data) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    return $wpdb->update(
        $table_name,
        array(
            'name' => $name,
            'slug' => $slug,
            'type' => $type,
            'path' => $path,
            'data' => serialize($data)
        ),
        array('id' => $id),
        array('%s', '%s', '%s', '%s', '%s'),
        array('%d')
    );
}

// Delete query
function qw_delete_query($id) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    $override_table = $wpdb->prefix . 'query_override_terms';
    
    // Delete from both tables
    $wpdb->delete($table_name, array('id' => $id), array('%d'));
    $wpdb->delete($override_table, array('query_id' => $id), array('%d'));
    
    return true;
}
```

### Override Term Management

```php
// Save override terms
function qw_save_override_terms($query_id, $term_ids) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_override_terms';
    
    // First, delete existing terms for this query
    $wpdb->delete($table_name, array('query_id' => $query_id), array('%d'));
    
    // Insert new terms
    foreach ($term_ids as $term_id) {
        $wpdb->insert(
            $table_name,
            array(
                'query_id' => $query_id,
                'term_id' => $term_id
            ),
            array('%d', '%d')
        );
    }
    
    return true;
}

// Get override terms for query
function qw_get_override_terms($query_id) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_override_terms';
    
    return $wpdb->get_col($wpdb->prepare(
        "SELECT term_id FROM $table_name WHERE query_id = %d",
        $query_id
    ));
}

// Find queries that override a specific term
function qw_get_override_queries_for_term($term_id) {
    global $wpdb;
    $query_table = $wpdb->prefix . 'query_wrangler';
    $override_table = $wpdb->prefix . 'query_override_terms';
    
    return $wpdb->get_results($wpdb->prepare(
        "SELECT q.* FROM $query_table q 
         INNER JOIN $override_table ot ON q.id = ot.query_id 
         WHERE ot.term_id = %d AND q.type = 'override'",
        $term_id
    ));
}
```

## Data Migration and Versioning

### Version Tracking

Query Wrangler tracks database version for migrations:

```php
// Check current version
function qw_get_db_version() {
    return get_option('qw_db_version', '1.0');
}

// Update version after migration
function qw_update_db_version($version) {
    update_option('qw_db_version', $version);
}
```

### Migration Examples

**Adding New Column:**
```php
function qw_migrate_to_version_150() {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    // Check if column exists
    $columns = $wpdb->get_col("DESCRIBE $table_name");
    
    if (!in_array('created_date', $columns)) {
        $wpdb->query("ALTER TABLE $table_name ADD COLUMN created_date datetime DEFAULT CURRENT_TIMESTAMP");
    }
    
    qw_update_db_version('1.5.0');
}
```

**Data Structure Migration:**
```php
function qw_migrate_data_structure_160() {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    // Get all queries
    $queries = $wpdb->get_results("SELECT id, data FROM $table_name");
    
    foreach ($queries as $query) {
        $data = unserialize($query->data);
        
        // Migrate old structure to new structure
        if (isset($data['old_field_name'])) {
            $data['new_field_name'] = $data['old_field_name'];
            unset($data['old_field_name']);
            
            // Update database
            $wpdb->update(
                $table_name,
                array('data' => serialize($data)),
                array('id' => $query->id),
                array('%s'),
                array('%d')
            );
        }
    }
    
    qw_update_db_version('1.6.0');
}
```

## Database Optimization

### Indexing Strategies

```sql
-- Add index for common queries
ALTER TABLE wp_query_wrangler ADD INDEX idx_type_slug (type, slug);
ALTER TABLE wp_query_wrangler ADD INDEX idx_name (name);

-- Override terms table is already optimized with unique key
-- UNIQUE KEY query_term (query_id, term_id)
```

### Query Optimization Tips

1. **Use Prepared Statements** - Always use `$wpdb->prepare()` for dynamic queries
2. **Limit Results** - Use LIMIT clauses for large datasets
3. **Cache Results** - Use transients for expensive queries
4. **Selective Fields** - Only select needed columns

**Example Optimized Query:**
```php
function qw_get_query_list_optimized($type = null, $limit = 50) {
    global $wpdb;
    $table_name = $wpdb->prefix . 'query_wrangler';
    
    $where = $type ? $wpdb->prepare("WHERE type = %s", $type) : "";
    $limit_clause = $limit ? "LIMIT " . intval($limit) : "";
    
    return $wpdb->get_results(
        "SELECT id, name, slug, type FROM $table_name $where ORDER BY name ASC $limit_clause"
    );
}
```

## Backup and Recovery

### Export Query Data

```php
function qw_export_queries($query_ids = null) {
    global $wpdb;
    $query_table = $wpdb->prefix . 'query_wrangler';
    $override_table = $wpdb->prefix . 'query_override_terms';
    
    $where = $query_ids ? 
        "WHERE id IN (" . implode(',', array_map('intval', $query_ids)) . ")" : "";
    
    $queries = $wpdb->get_results(
        "SELECT * FROM $query_table $where ORDER BY id ASC"
    );
    
    $export_data = array();
    
    foreach ($queries as $query) {
        $query_data = array(
            'query' => $query,
            'override_terms' => array()
        );
        
        if ($query->type === 'override') {
            $query_data['override_terms'] = $wpdb->get_col($wpdb->prepare(
                "SELECT term_id FROM $override_table WHERE query_id = %d",
                $query->id
            ));
        }
        
        $export_data[] = $query_data;
    }
    
    return json_encode($export_data, JSON_PRETTY_PRINT);
}
```

### Import Query Data

```php
function qw_import_queries($json_data, $overwrite = false) {
    $import_data = json_decode($json_data, true);
    
    if (!$import_data) {
        return false;
    }
    
    $imported_count = 0;
    
    foreach ($import_data as $item) {
        $query_data = $item['query'];
        
        // Check if query exists by slug
        $existing = qw_get_query_by_slug($query_data['slug']);
        
        if ($existing && !$overwrite) {
            continue; // Skip existing queries unless overwrite is enabled
        }
        
        if ($existing && $overwrite) {
            // Update existing query
            qw_update_query(
                $existing->id,
                $query_data['name'],
                $query_data['slug'],
                $query_data['type'],
                $query_data['path'],
                unserialize($query_data['data'])
            );
            $query_id = $existing->id;
        } else {
            // Create new query
            $query_id = qw_save_query(
                $query_data['name'],
                $query_data['slug'],
                $query_data['type'],
                $query_data['path'],
                unserialize($query_data['data'])
            );
        }
        
        // Import override terms if present
        if (!empty($item['override_terms']) && $query_id) {
            qw_save_override_terms($query_id, $item['override_terms']);
        }
        
        $imported_count++;
    }
    
    return $imported_count;
}
```

## Performance Monitoring

### Query Analysis

```php
function qw_analyze_query_performance() {
    global $wpdb;
    
    // Get table sizes
    $query_table_size = $wpdb->get_var(
        "SELECT ROUND(((data_length + index_length) / 1024 / 1024), 2) 
         FROM information_schema.TABLES 
         WHERE table_schema = DATABASE() 
         AND table_name = '{$wpdb->prefix}query_wrangler'"
    );
    
    $override_table_size = $wpdb->get_var(
        "SELECT ROUND(((data_length + index_length) / 1024 / 1024), 2) 
         FROM information_schema.TABLES 
         WHERE table_schema = DATABASE() 
         AND table_name = '{$wpdb->prefix}query_override_terms'"
    );
    
    // Get record counts
    $total_queries = $wpdb->get_var("SELECT COUNT(*) FROM {$wpdb->prefix}query_wrangler");
    $total_overrides = $wpdb->get_var("SELECT COUNT(*) FROM {$wpdb->prefix}query_override_terms");
    
    return array(
        'query_table_size_mb' => $query_table_size,
        'override_table_size_mb' => $override_table_size,
        'total_queries' => $total_queries,
        'total_override_terms' => $total_overrides,
        'avg_data_size' => $wpdb->get_var(
            "SELECT AVG(LENGTH(data)) FROM {$wpdb->prefix}query_wrangler"
        )
    );
}
```

This comprehensive database documentation covers all aspects of Query Wrangler's data storage, from basic table structures to advanced migration and optimization strategies.