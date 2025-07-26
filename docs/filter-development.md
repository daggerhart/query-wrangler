# Filter Development Guide

## Overview

Filters in Query Wrangler control which posts are included in query results by modifying WP_Query arguments. This guide covers how to create custom filter types, understand the filter system architecture, and implement complex filtering logic.

## Filter System Architecture

### Filter Processing Flow

1. **Registration** - Filters are registered via the `qw_filters` filter hook
2. **Form Generation** - Admin forms are created for filter configuration
3. **Query Modification** - Filter values are processed into WP_Query arguments
4. **Query Execution** - Modified arguments are used in WP_Query

### Filter Structure

```php
$filters['filter_name'] = array(
    // Required: Display name in admin
    'title' => 'Filter Display Name',
    
    // Required: Description shown to users
    'description' => 'What this filter does',
    
    // Required: Function to generate admin form
    'form_callback' => 'filter_form_function',
    
    // Required: Function to modify query arguments
    'query_args_callback' => 'filter_query_function',
    
    // Optional: Where this filter can be used
    'query_display_types' => array('page', 'widget', 'override'),
    
    // Optional: Template for admin form
    'form_template' => 'filter_form_template',
    
    // Optional: Exposed form for frontend filtering
    'exposed_form_callback' => 'filter_exposed_form',
);
```

## Built-in Filter Types

### Taxonomy Filters

**Categories Filter:**
```php
$filters['categories'] = array(
    'title' => 'Categories',
    'description' => 'Select which categories to pull posts from',
    'form_callback' => 'qw_filter_categories_form',
    'query_args_callback' => 'qw_generate_query_args_categories',
    'query_display_types' => array('page', 'widget'),
);
```

**Tags and Custom Taxonomies:**
- Similar structure to categories
- Support for include/exclude operations
- Hierarchical taxonomy handling

### Meta Field Filters

**Meta Query Filter:**
```php
$filters['meta_query'] = array(
    'title' => 'Meta Query',
    'description' => 'Filter for a single meta query',
    'form_callback' => 'qw_filter_meta_query_form',
    'query_args_callback' => 'qw_generate_query_args_meta_query',
);
```

**Meta Key/Value Filters:**
- Simple meta key existence checks
- Key/value pair matching
- Support for different comparison operators

### Content Filters

**Search Filter:**
- Text search in post content and titles
- Configurable search fields
- Custom search algorithms

**Author Filter:**
- Filter by post author
- Support for multiple authors
- User role-based filtering

**Post Type Filter:**
- Restrict results to specific post types
- Multiple post type selection
- Custom post type support

## Creating Custom Filters

### Simple Filter Example

```php
add_filter('qw_filters', 'my_custom_filters');

function my_custom_filters($filters) {
    $filters['featured_posts'] = array(
        'title' => 'Featured Posts',
        'description' => 'Show only posts marked as featured',
        'form_callback' => 'featured_posts_form',
        'query_args_callback' => 'featured_posts_query',
        'query_display_types' => array('page', 'widget'),
    );
    
    return $filters;
}

function featured_posts_form($filter) {
    $show_featured = isset($filter['values']['show_featured']) ? $filter['values']['show_featured'] : 'yes';
    
    ?>
    <p>
        <label>Show Featured Posts:</label>
        <select name="<?php echo $filter['form_prefix']; ?>[show_featured]">
            <option value="yes" <?php selected($show_featured, 'yes'); ?>>Yes</option>
            <option value="no" <?php selected($show_featured, 'no'); ?>>No</option>
            <option value="only" <?php selected($show_featured, 'only'); ?>>Only Featured</option>
        </select>
    </p>
    <?php
}

function featured_posts_query(&$args, $filter) {
    $show_featured = isset($filter['values']['show_featured']) ? $filter['values']['show_featured'] : 'yes';
    
    if (!isset($args['meta_query'])) {
        $args['meta_query'] = array();
    }
    
    switch ($show_featured) {
        case 'only':
            $args['meta_query'][] = array(
                'key' => 'featured_post',
                'value' => '1',
                'compare' => '='
            );
            break;
            
        case 'no':
            $args['meta_query'][] = array(
                'key' => 'featured_post',
                'compare' => 'NOT EXISTS'
            );
            break;
            
        // 'yes' - no filtering needed, show all posts
    }
}
```

### Advanced Filter with Multiple Options

```php
add_filter('qw_filters', 'date_range_filter');

function date_range_filter($filters) {
    $filters['date_range'] = array(
        'title' => 'Date Range',
        'description' => 'Filter posts by publication date range',
        'form_callback' => 'date_range_form',
        'query_args_callback' => 'date_range_query',
    );
    
    return $filters;
}

function date_range_form($filter) {
    $date_from = isset($filter['values']['date_from']) ? $filter['values']['date_from'] : '';
    $date_to = isset($filter['values']['date_to']) ? $filter['values']['date_to'] : '';
    $date_field = isset($filter['values']['date_field']) ? $filter['values']['date_field'] : 'post_date';
    $relative_dates = isset($filter['values']['relative_dates']) ? $filter['values']['relative_dates'] : '';
    
    ?>
    <div>
        <label>Date Field:</label>
        <select name="<?php echo $filter['form_prefix']; ?>[date_field]">
            <option value="post_date" <?php selected($date_field, 'post_date'); ?>>Publication Date</option>
            <option value="post_modified" <?php selected($date_field, 'post_modified'); ?>>Modified Date</option>
        </select>
    </div>
    
    <div>
        <label>From Date:</label>
        <input type="date" 
               name="<?php echo $filter['form_prefix']; ?>[date_from]" 
               value="<?php echo esc_attr($date_from); ?>" />
    </div>
    
    <div>
        <label>To Date:</label>
        <input type="date" 
               name="<?php echo $filter['form_prefix']; ?>[date_to]" 
               value="<?php echo esc_attr($date_to); ?>" />
    </div>
    
    <div>
        <label>Or Use Relative Dates:</label>
        <select name="<?php echo $filter['form_prefix']; ?>[relative_dates]">
            <option value="" <?php selected($relative_dates, ''); ?>>Custom Dates</option>
            <option value="last_week" <?php selected($relative_dates, 'last_week'); ?>>Last Week</option>
            <option value="last_month" <?php selected($relative_dates, 'last_month'); ?>>Last Month</option>
            <option value="last_year" <?php selected($relative_dates, 'last_year'); ?>>Last Year</option>
            <option value="this_month" <?php selected($relative_dates, 'this_month'); ?>>This Month</option>
            <option value="this_year" <?php selected($relative_dates, 'this_year'); ?>>This Year</option>
        </select>
        <p class="description">Relative dates override custom date fields</p>
    </div>
    <?php
}

function date_range_query(&$args, $filter) {
    $date_from = isset($filter['values']['date_from']) ? $filter['values']['date_from'] : '';
    $date_to = isset($filter['values']['date_to']) ? $filter['values']['date_to'] : '';
    $date_field = isset($filter['values']['date_field']) ? $filter['values']['date_field'] : 'post_date';
    $relative_dates = isset($filter['values']['relative_dates']) ? $filter['values']['relative_dates'] : '';
    
    // Handle relative dates first
    if (!empty($relative_dates)) {
        list($date_from, $date_to) = get_relative_date_range($relative_dates);
    }
    
    // Build date query if we have dates
    if (!empty($date_from) || !empty($date_to)) {
        $date_query = array();
        
        if (!empty($date_from)) {
            $date_query['after'] = $date_from;
        }
        
        if (!empty($date_to)) {
            $date_query['before'] = $date_to;
        }
        
        // Set the date field to query
        if ($date_field === 'post_modified') {
            $date_query['column'] = 'post_modified';
        }
        
        $args['date_query'] = array($date_query);
    }
}

function get_relative_date_range($relative_period) {
    $current_date = current_time('Y-m-d');
    
    switch ($relative_period) {
        case 'last_week':
            $date_from = date('Y-m-d', strtotime('-1 week', strtotime($current_date)));
            $date_to = $current_date;
            break;
            
        case 'last_month':
            $date_from = date('Y-m-d', strtotime('-1 month', strtotime($current_date)));
            $date_to = $current_date;
            break;
            
        case 'last_year':
            $date_from = date('Y-m-d', strtotime('-1 year', strtotime($current_date)));
            $date_to = $current_date;
            break;
            
        case 'this_month':
            $date_from = date('Y-m-01');
            $date_to = date('Y-m-t');
            break;
            
        case 'this_year':
            $date_from = date('Y-01-01');
            $date_to = date('Y-12-31');
            break;
            
        default:
            return array('', '');
    }
    
    return array($date_from, $date_to);
}
```

### Complex Meta Query Filter

```php
add_filter('qw_filters', 'advanced_meta_filter');

function advanced_meta_filter($filters) {
    $filters['advanced_meta'] = array(
        'title' => 'Advanced Meta Filter',
        'description' => 'Multiple meta field filtering with complex logic',
        'form_callback' => 'advanced_meta_form',
        'query_args_callback' => 'advanced_meta_query',
    );
    
    return $filters;
}

function advanced_meta_form($filter) {
    $conditions = isset($filter['values']['conditions']) ? $filter['values']['conditions'] : array();
    $relation = isset($filter['values']['relation']) ? $filter['values']['relation'] : 'AND';
    
    // Ensure we have at least one condition
    if (empty($conditions)) {
        $conditions = array(
            array('key' => '', 'value' => '', 'compare' => '=', 'type' => 'CHAR')
        );
    }
    
    ?>
    <div id="advanced-meta-conditions">
        <div>
            <label>Condition Relation:</label>
            <select name="<?php echo $filter['form_prefix']; ?>[relation]">
                <option value="AND" <?php selected($relation, 'AND'); ?>>AND (all conditions must match)</option>
                <option value="OR" <?php selected($relation, 'OR'); ?>>OR (any condition must match)</option>
            </select>
        </div>
        
        <div id="meta-conditions-list">
            <?php foreach ($conditions as $index => $condition): ?>
                <div class="meta-condition" data-index="<?php echo $index; ?>">
                    <h4>Condition <?php echo $index + 1; ?></h4>
                    
                    <div>
                        <label>Meta Key:</label>
                        <input type="text" 
                               name="<?php echo $filter['form_prefix']; ?>[conditions][<?php echo $index; ?>][key]" 
                               value="<?php echo esc_attr($condition['key']); ?>" />
                    </div>
                    
                    <div>
                        <label>Value:</label>
                        <input type="text" 
                               name="<?php echo $filter['form_prefix']; ?>[conditions][<?php echo $index; ?>][value]" 
                               value="<?php echo esc_attr($condition['value']); ?>" />
                    </div>
                    
                    <div>
                        <label>Compare:</label>
                        <select name="<?php echo $filter['form_prefix']; ?>[conditions][<?php echo $index; ?>][compare]">
                            <?php
                            $compares = array('=', '!=', '>', '>=', '<', '<=', 'LIKE', 'NOT LIKE', 'IN', 'NOT IN', 'EXISTS', 'NOT EXISTS');
                            foreach ($compares as $compare):
                                ?>
                                <option value="<?php echo $compare; ?>" <?php selected($condition['compare'], $compare); ?>><?php echo $compare; ?></option>
                            <?php endforeach; ?>
                        </select>
                    </div>
                    
                    <div>
                        <label>Type:</label>
                        <select name="<?php echo $filter['form_prefix']; ?>[conditions][<?php echo $index; ?>][type]">
                            <?php
                            $types = array('CHAR', 'NUMERIC', 'BINARY', 'DATE', 'DATETIME', 'DECIMAL', 'SIGNED', 'TIME', 'UNSIGNED');
                            foreach ($types as $type):
                                ?>
                                <option value="<?php echo $type; ?>" <?php selected($condition['type'], $type); ?>><?php echo $type; ?></option>
                            <?php endforeach; ?>
                        </select>
                    </div>
                    
                    <button type="button" class="remove-condition">Remove Condition</button>
                </div>
            <?php endforeach; ?>
        </div>
        
        <button type="button" id="add-condition">Add Condition</button>
    </div>
    
    <script>
        jQuery(document).ready(function($) {
            var conditionIndex = <?php echo count($conditions); ?>;
            
            $('#add-condition').click(function() {
                // Add new condition HTML
                var newCondition = '<div class="meta-condition" data-index="' + conditionIndex + '">';
                // ... HTML for new condition ...
                newCondition += '</div>';
                
                $('#meta-conditions-list').append(newCondition);
                conditionIndex++;
            });
            
            $(document).on('click', '.remove-condition', function() {
                $(this).closest('.meta-condition').remove();
            });
        });
    </script>
    <?php
}

function advanced_meta_query(&$args, $filter) {
    $conditions = isset($filter['values']['conditions']) ? $filter['values']['conditions'] : array();
    $relation = isset($filter['values']['relation']) ? $filter['values']['relation'] : 'AND';
    
    if (empty($conditions)) {
        return;
    }
    
    $meta_query = array('relation' => $relation);
    
    foreach ($conditions as $condition) {
        if (empty($condition['key'])) {
            continue;
        }
        
        $meta_condition = array(
            'key' => $condition['key'],
            'compare' => $condition['compare'],
            'type' => $condition['type'],
        );
        
        // Handle value based on compare type
        if (!in_array($condition['compare'], array('EXISTS', 'NOT EXISTS'))) {
            if (in_array($condition['compare'], array('IN', 'NOT IN'))) {
                // Convert comma-separated values to array
                $meta_condition['value'] = array_map('trim', explode(',', $condition['value']));
            } else {
                $meta_condition['value'] = $condition['value'];
            }
        }
        
        $meta_query[] = $meta_condition;
    }
    
    if (count($meta_query) > 1) { // More than just the relation
        if (!isset($args['meta_query'])) {
            $args['meta_query'] = array();
        }
        
        $args['meta_query'][] = $meta_query;
    }
}
```

### Taxonomy-Based Filter with AJAX

```php
add_filter('qw_filters', 'dynamic_taxonomy_filter');

function dynamic_taxonomy_filter($filters) {
    $filters['dynamic_taxonomy'] = array(
        'title' => 'Dynamic Taxonomy Filter',
        'description' => 'Taxonomy filter with dynamic term loading',
        'form_callback' => 'dynamic_taxonomy_form',
        'query_args_callback' => 'dynamic_taxonomy_query',
    );
    
    return $filters;
}

function dynamic_taxonomy_form($filter) {
    $taxonomy = isset($filter['values']['taxonomy']) ? $filter['values']['taxonomy'] : '';
    $terms = isset($filter['values']['terms']) ? $filter['values']['terms'] : array();
    $operator = isset($filter['values']['operator']) ? $filter['values']['operator'] : 'IN';
    
    $taxonomies = get_taxonomies(array('public' => true), 'objects');
    
    ?>
    <div>
        <label>Taxonomy:</label>
        <select name="<?php echo $filter['form_prefix']; ?>[taxonomy]" id="taxonomy-select" data-filter-prefix="<?php echo $filter['form_prefix']; ?>">
            <option value="">Select Taxonomy</option>
            <?php foreach ($taxonomies as $tax_name => $tax_object): ?>
                <option value="<?php echo $tax_name; ?>" <?php selected($taxonomy, $tax_name); ?>><?php echo $tax_object->label; ?></option>
            <?php endforeach; ?>
        </select>
    </div>
    
    <div id="terms-container">
        <?php if ($taxonomy): ?>
            <div>
                <label>Terms:</label>
                <div id="terms-checkboxes">
                    <?php
                    $available_terms = get_terms(array(
                        'taxonomy' => $taxonomy,
                        'hide_empty' => false,
                    ));
                    
                    foreach ($available_terms as $term):
                        $checked = in_array($term->term_id, $terms) ? 'checked' : '';
                        ?>
                        <label>
                            <input type="checkbox" 
                                   name="<?php echo $filter['form_prefix']; ?>[terms][]" 
                                   value="<?php echo $term->term_id; ?>" 
                                   <?php echo $checked; ?> />
                            <?php echo $term->name; ?>
                        </label><br>
                    <?php endforeach; ?>
                </div>
            </div>
        <?php endif; ?>
    </div>
    
    <div>
        <label>Operator:</label>
        <select name="<?php echo $filter['form_prefix']; ?>[operator]">
            <option value="IN" <?php selected($operator, 'IN'); ?>>IN (any selected)</option>
            <option value="AND" <?php selected($operator, 'AND'); ?>>AND (all selected)</option>
            <option value="NOT IN" <?php selected($operator, 'NOT IN'); ?>>NOT IN (exclude selected)</option>
        </select>
    </div>
    
    <script>
        jQuery(document).ready(function($) {
            $('#taxonomy-select').change(function() {
                var taxonomy = $(this).val();
                var filterPrefix = $(this).data('filter-prefix');
                
                if (taxonomy) {
                    // AJAX call to load terms
                    $.post(ajaxurl, {
                        action: 'load_taxonomy_terms',
                        taxonomy: taxonomy,
                        filter_prefix: filterPrefix,
                        _ajax_nonce: '<?php echo wp_create_nonce("load_taxonomy_terms"); ?>'
                    }, function(response) {
                        $('#terms-container').html(response);
                    });
                } else {
                    $('#terms-container').empty();
                }
            });
        });
    </script>
    <?php
}

// AJAX handler for loading taxonomy terms
add_action('wp_ajax_load_taxonomy_terms', 'handle_load_taxonomy_terms');
function handle_load_taxonomy_terms() {
    check_ajax_referer('load_taxonomy_terms');
    
    $taxonomy = sanitize_text_field($_POST['taxonomy']);
    $filter_prefix = sanitize_text_field($_POST['filter_prefix']);
    
    if (!taxonomy_exists($taxonomy)) {
        wp_die('Invalid taxonomy');
    }
    
    $terms = get_terms(array(
        'taxonomy' => $taxonomy,
        'hide_empty' => false,
    ));
    
    if (is_wp_error($terms) || empty($terms)) {
        echo '<p>No terms found</p>';
        wp_die();
    }
    
    echo '<div><label>Terms:</label><div id="terms-checkboxes">';
    foreach ($terms as $term) {
        echo '<label><input type="checkbox" name="' . esc_attr($filter_prefix) . '[terms][]" value="' . $term->term_id . '" /> ' . esc_html($term->name) . '</label><br>';
    }
    echo '</div></div>';
    
    wp_die();
}

function dynamic_taxonomy_query(&$args, $filter) {
    $taxonomy = isset($filter['values']['taxonomy']) ? $filter['values']['taxonomy'] : '';
    $terms = isset($filter['values']['terms']) ? $filter['values']['terms'] : array();
    $operator = isset($filter['values']['operator']) ? $filter['values']['operator'] : 'IN';
    
    if (empty($taxonomy) || empty($terms)) {
        return;
    }
    
    if (!isset($args['tax_query'])) {
        $args['tax_query'] = array();
    }
    
    $args['tax_query'][] = array(
        'taxonomy' => $taxonomy,
        'field'    => 'term_id',
        'terms'    => array_map('intval', $terms),
        'operator' => $operator,
    );
}
```

## Exposed Filters (Frontend)

### Creating Frontend Filter Forms

```php
add_filter('qw_filters', 'exposed_search_filter');

function exposed_search_filter($filters) {
    $filters['exposed_search'] = array(
        'title' => 'Exposed Search',
        'description' => 'Search filter that can be exposed to frontend users',
        'form_callback' => 'exposed_search_admin_form',
        'query_args_callback' => 'exposed_search_query',
        'exposed_form_callback' => 'exposed_search_frontend_form',
    );
    
    return $filters;
}

function exposed_search_admin_form($filter) {
    $placeholder = isset($filter['values']['placeholder']) ? $filter['values']['placeholder'] : 'Search...';
    $search_fields = isset($filter['values']['search_fields']) ? $filter['values']['search_fields'] : array('title', 'content');
    
    ?>
    <div>
        <label>Placeholder Text:</label>
        <input type="text" 
               name="<?php echo $filter['form_prefix']; ?>[placeholder]" 
               value="<?php echo esc_attr($placeholder); ?>" />
    </div>
    
    <div>
        <label>Search Fields:</label>
        <label><input type="checkbox" name="<?php echo $filter['form_prefix']; ?>[search_fields][]" value="title" <?php echo in_array('title', $search_fields) ? 'checked' : ''; ?> /> Title</label>
        <label><input type="checkbox" name="<?php echo $filter['form_prefix']; ?>[search_fields][]" value="content" <?php echo in_array('content', $search_fields) ? 'checked' : ''; ?> /> Content</label>
        <label><input type="checkbox" name="<?php echo $filter['form_prefix']; ?>[search_fields][]" value="excerpt" <?php echo in_array('excerpt', $search_fields) ? 'checked' : ''; ?> /> Excerpt</label>
    </div>
    <?php
}

function exposed_search_frontend_form($filter, $current_values = array()) {
    $placeholder = isset($filter['placeholder']) ? $filter['placeholder'] : 'Search...';
    $current_search = isset($current_values['search']) ? $current_values['search'] : '';
    
    ?>
    <div class="qw-exposed-filter exposed-search">
        <label for="qw-search">Search:</label>
        <input type="text" 
               id="qw-search" 
               name="qw_filters[search]" 
               value="<?php echo esc_attr($current_search); ?>" 
               placeholder="<?php echo esc_attr($placeholder); ?>" />
        <button type="submit">Search</button>
    </div>
    <?php
}

function exposed_search_query(&$args, $filter) {
    // Check for exposed filter values
    $search_term = '';
    
    if (isset($_GET['qw_filters']['search'])) {
        $search_term = sanitize_text_field($_GET['qw_filters']['search']);
    } elseif (isset($filter['values']['default_search'])) {
        $search_term = $filter['values']['default_search'];
    }
    
    if (!empty($search_term)) {
        $search_fields = isset($filter['search_fields']) ? $filter['search_fields'] : array('title', 'content');
        
        if (in_array('title', $search_fields) && in_array('content', $search_fields)) {
            // Use WordPress built-in search
            $args['s'] = $search_term;
        } else {
            // Custom search implementation
            add_filter('posts_where', function($where) use ($search_term, $search_fields) {
                global $wpdb;
                
                $search_conditions = array();
                
                if (in_array('title', $search_fields)) {
                    $search_conditions[] = "{$wpdb->posts}.post_title LIKE '%" . esc_sql($search_term) . "%'";
                }
                
                if (in_array('content', $search_fields)) {
                    $search_conditions[] = "{$wpdb->posts}.post_content LIKE '%" . esc_sql($search_term) . "%'";
                }
                
                if (in_array('excerpt', $search_fields)) {
                    $search_conditions[] = "{$wpdb->posts}.post_excerpt LIKE '%" . esc_sql($search_term) . "%'";
                }
                
                if (!empty($search_conditions)) {
                    $where .= " AND (" . implode(' OR ', $search_conditions) . ")";
                }
                
                return $where;
            });
        }
    }
}
```

## Filter Performance Optimization

### Caching Filter Options

```php
function optimized_filter_form($filter) {
    // Cache expensive operations
    $cache_key = 'filter_options_' . md5(serialize($filter));
    $options = get_transient($cache_key);
    
    if ($options === false) {
        $options = generate_expensive_filter_options($filter);
        set_transient($cache_key, $options, HOUR_IN_SECONDS);
    }
    
    // Render form with cached options
    render_filter_form($filter, $options);
}
```

### Efficient Query Modifications

```php
function efficient_filter_query(&$args, $filter) {
    // Avoid redundant query modifications
    if (isset($args['meta_query'])) {
        // Check if similar meta query already exists
        foreach ($args['meta_query'] as $existing_query) {
            if (isset($existing_query['key']) && $existing_query['key'] === $filter['values']['key']) {
                // Modify existing query instead of adding new one
                return;
            }
        }
    }
    
    // Add new meta query
    $args['meta_query'][] = array(
        'key' => $filter['values']['key'],
        'value' => $filter['values']['value'],
        'compare' => $filter['values']['compare'],
    );
}
```

## Testing Custom Filters

### Unit Testing Filter Logic

```php
class CustomFilterTest extends WP_UnitTestCase {
    
    public function test_featured_posts_filter() {
        // Create test posts
        $featured_post = $this->factory->post->create();
        $regular_post = $this->factory->post->create();
        
        // Mark one as featured
        update_post_meta($featured_post, 'featured_post', '1');
        
        // Test filter configuration
        $filter = array(
            'values' => array(
                'show_featured' => 'only'
            )
        );
        
        $args = array();
        featured_posts_query($args, $filter);
        
        // Verify meta query was added
        $this->assertArrayHasKey('meta_query', $args);
        $this->assertEquals('featured_post', $args['meta_query'][0]['key']);
        $this->assertEquals('1', $args['meta_query'][0]['value']);
    }
    
    public function test_date_range_filter() {
        $filter = array(
            'values' => array(
                'date_from' => '2023-01-01',
                'date_to' => '2023-12-31',
                'date_field' => 'post_date'
            )
        );
        
        $args = array();
        date_range_query($args, $filter);
        
        // Verify date query was added
        $this->assertArrayHasKey('date_query', $args);
        $this->assertEquals('2023-01-01', $args['date_query'][0]['after']);
        $this->assertEquals('2023-12-31', $args['date_query'][0]['before']);
    }
}
```

## Security Considerations

### Input Sanitization

```php
function secure_filter_query(&$args, $filter) {
    // Sanitize all input values
    $meta_key = isset($filter['values']['meta_key']) ? sanitize_key($filter['values']['meta_key']) : '';
    $meta_value = isset($filter['values']['meta_value']) ? sanitize_text_field($filter['values']['meta_value']) : '';
    $compare = isset($filter['values']['compare']) ? sanitize_text_field($filter['values']['compare']) : '=';
    
    // Validate compare operator
    $allowed_compares = array('=', '!=', '>', '>=', '<', '<=', 'LIKE', 'NOT LIKE', 'IN', 'NOT IN', 'EXISTS', 'NOT EXISTS');
    if (!in_array($compare, $allowed_compares)) {
        $compare = '=';
    }
    
    // Build secure meta query
    if (!empty($meta_key)) {
        $args['meta_query'][] = array(
            'key' => $meta_key,
            'value' => $meta_value,
            'compare' => $compare,
        );
    }
}
```

### Capability Checks

```php
function admin_only_filter_form($filter) {
    // Check user capabilities for sensitive filters
    if (!current_user_can('manage_options')) {
        echo '<p>Insufficient permissions to configure this filter.</p>';
        return;
    }
    
    // Render form for authorized users
    render_sensitive_filter_form($filter);
}
```

This comprehensive guide covers all aspects of filter development in Query Wrangler, from simple boolean filters to complex multi-condition filters with AJAX functionality and frontend exposure.