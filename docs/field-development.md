# Field Development Guide

## Overview

Fields in Query Wrangler control what data is displayed for each post in query results. This guide covers how to create custom field types, extend existing fields, and understand the field system architecture.

## Field System Architecture

### Field Registration Flow

1. **Registration** - Fields are registered via the `qw_fields` filter hook
2. **Processing** - Field configurations are processed during query setup
3. **Form Generation** - Admin forms are generated for field configuration
4. **Output Generation** - Field values are generated during template rendering

### Field Structure

```php
$fields['field_name'] = array(
    // Required: Display name in admin
    'title' => 'Field Display Name',
    
    // Required: Description shown to users
    'description' => 'What this field displays',
    
    // Optional: Function to generate output
    'output_callback' => 'field_output_function',
    
    // Optional: Pass post and field data to output callback
    'output_arguments' => true,
    
    // Optional: Function to generate admin form
    'form_callback' => 'field_form_function',
    
    // Optional: Template for admin form (alternative to form_callback)
    'form_template' => 'field_form_template',
    
    // Optional: Enable content formatting options
    'content_options' => true,
    
    // Optional: Additional metadata
    'meta_key' => 'custom_meta_key',
);
```

## Built-in Field Types

### Standard WordPress Fields

**Basic Post Fields:**
```php
$fields['post_title'] = array(
    'title' => 'Post Title',
    'description' => 'The title of a post.',
    'output_callback' => 'get_the_title',
);

$fields['post_content'] = array(
    'title' => 'Post Content',
    'description' => 'The full content body of a post.',
    'output_callback' => 'get_the_content',
    'content_options' => true,
);
```

**Date and Status Fields:**
```php
$fields['post_date'] = array(
    'title' => 'Post Date',
    'description' => 'Published date of a post.',
    'output_callback' => 'get_the_date',
);

$fields['post_status'] = array(
    'title' => 'Post Status',
    'description' => 'Status of a post.',
);
```

### Advanced Field Types

**Meta Value Fields:**
- Automatically generated from available post meta keys
- Support for display handlers (ACF, CCTM integration)
- Configurable count and separators
- Image ID support with size selection

**Attachment Fields:**
- Featured images with size options
- File attachments with download links
- Image galleries

**Taxonomy Fields:**
- Category and tag displays
- Custom taxonomy support
- Link generation options

## Creating Custom Fields

### Simple Field Example

```php
add_filter('qw_fields', 'my_custom_fields');

function my_custom_fields($fields) {
    $fields['reading_time'] = array(
        'title' => 'Reading Time',
        'description' => 'Estimated reading time for the post',
        'output_callback' => 'calculate_reading_time',
        'output_arguments' => true,
    );
    
    return $fields;
}

function calculate_reading_time($post, $field) {
    $word_count = str_word_count(strip_tags($post->post_content));
    $reading_time = ceil($word_count / 200); // 200 words per minute
    
    return $reading_time . ' min read';
}
```

### Field with Configuration Form

```php
add_filter('qw_fields', 'my_configurable_fields');

function my_configurable_fields($fields) {
    $fields['custom_excerpt'] = array(
        'title' => 'Custom Excerpt',
        'description' => 'Customizable excerpt with length control',
        'form_callback' => 'custom_excerpt_form',
        'output_callback' => 'custom_excerpt_output',
        'output_arguments' => true,
    );
    
    return $fields;
}

function custom_excerpt_form($field) {
    $length = isset($field['values']['excerpt_length']) ? $field['values']['excerpt_length'] : 100;
    $suffix = isset($field['values']['excerpt_suffix']) ? $field['values']['excerpt_suffix'] : '...';
    
    ?>
    <div>
        <label>Excerpt Length:</label>
        <input type="number" 
               name="<?php echo $field['form_prefix']; ?>[excerpt_length]" 
               value="<?php echo esc_attr($length); ?>" 
               min="1" max="500" />
        <p class="description">Number of words to display</p>
    </div>
    
    <div>
        <label>Suffix:</label>
        <input type="text" 
               name="<?php echo $field['form_prefix']; ?>[excerpt_suffix]" 
               value="<?php echo esc_attr($suffix); ?>" />
        <p class="description">Text to append to truncated excerpts</p>
    </div>
    <?php
}

function custom_excerpt_output($post, $field) {
    $length = isset($field['excerpt_length']) ? (int)$field['excerpt_length'] : 100;
    $suffix = isset($field['excerpt_suffix']) ? $field['excerpt_suffix'] : '...';
    
    $content = strip_tags($post->post_content);
    $words = explode(' ', $content);
    
    if (count($words) > $length) {
        $excerpt = implode(' ', array_slice($words, 0, $length)) . $suffix;
    } else {
        $excerpt = $content;
    }
    
    return $excerpt;
}
```

### Complex Field with External Data

```php
add_filter('qw_fields', 'weather_field');

function weather_field($fields) {
    // Only add if API key is configured
    if (get_option('weather_api_key')) {
        $fields['current_weather'] = array(
            'title' => 'Current Weather',
            'description' => 'Display current weather information',
            'form_callback' => 'weather_field_form',
            'output_callback' => 'weather_field_output',
            'output_arguments' => true,
        );
    }
    
    return $fields;
}

function weather_field_form($field) {
    $location = isset($field['values']['location']) ? $field['values']['location'] : '';
    $format = isset($field['values']['format']) ? $field['values']['format'] : 'simple';
    
    ?>
    <div>
        <label>Location:</label>
        <input type="text" 
               name="<?php echo $field['form_prefix']; ?>[location]" 
               value="<?php echo esc_attr($location); ?>" 
               placeholder="City, State or ZIP" />
    </div>
    
    <div>
        <label>Display Format:</label>
        <select name="<?php echo $field['form_prefix']; ?>[format]">
            <option value="simple" <?php selected($format, 'simple'); ?>>Simple</option>
            <option value="detailed" <?php selected($format, 'detailed'); ?>>Detailed</option>
        </select>
    </div>
    <?php
}

function weather_field_output($post, $field) {
    $location = isset($field['location']) ? $field['location'] : '';
    $format = isset($field['format']) ? $field['format'] : 'simple';
    
    if (empty($location)) {
        return '';
    }
    
    // Use transients for caching
    $cache_key = 'weather_' . md5($location);
    $weather_data = get_transient($cache_key);
    
    if ($weather_data === false) {
        $weather_data = fetch_weather_data($location);
        set_transient($cache_key, $weather_data, 30 * MINUTE_IN_SECONDS);
    }
    
    if (!$weather_data) {
        return 'Weather data unavailable';
    }
    
    if ($format === 'detailed') {
        return format_detailed_weather($weather_data);
    } else {
        return $weather_data['temperature'] . '°F, ' . $weather_data['condition'];
    }
}

function fetch_weather_data($location) {
    $api_key = get_option('weather_api_key');
    $url = "https://api.weather.com/v1/current?key={$api_key}&location=" . urlencode($location);
    
    $response = wp_remote_get($url);
    
    if (is_wp_error($response)) {
        return false;
    }
    
    $body = wp_remote_retrieve_body($response);
    $data = json_decode($body, true);
    
    return array(
        'temperature' => $data['current']['temp_f'],
        'condition' => $data['current']['condition']['text'],
        'humidity' => $data['current']['humidity'],
        'wind_speed' => $data['current']['wind_mph'],
    );
}
```

## Meta Value Field System

### Meta Field Auto-Generation

Query Wrangler automatically creates fields for all available post meta keys:

```php
function qw_field_meta_value($fields) {
    $show_silent_meta = QW_Settings::get_instance()->get('show_silent_meta', false);
    
    // Cache meta fields for performance
    $cache_name = md5(json_encode($fields));
    $meta_fields = get_transient($cache_name);
    
    if (!$meta_fields) {
        $meta_fields = array();
        $meta_keys = qw_get_meta_keys();
        
        foreach ($meta_keys as $key) {
            $field_key = 'meta_' . str_replace(' ', '_', $key);
            
            // Skip "silent" meta keys (starting with _) unless enabled
            $key_is_not_silent = (substr($key, 0, 1) != '_' && 
                                 substr($key, 0, 3) != 'ww-' && 
                                 substr($key, 0, 3) != 'ww_');
            
            if ($show_silent_meta || $key_is_not_silent) {
                $meta_fields[$field_key] = array(
                    'title' => 'Custom Field: ' . $key,
                    'description' => 'Custom Field data with key: ' . $key,
                    'output_callback' => 'qw_display_post_meta_value',
                    'output_arguments' => true,
                    'meta_key' => $key,
                    'form_callback' => 'qw_meta_value_form_callback',
                    'content_options' => true,
                );
            }
        }
        
        set_transient($cache_name, $meta_fields, 15 * MINUTE_IN_SECONDS);
    }
    
    return array_merge($fields, $meta_fields);
}
```

### Meta Display Handlers

Custom display handlers for different meta field types:

```php
add_filter('qw_meta_value_display_handlers', 'custom_meta_handlers');

function custom_meta_handlers($handlers) {
    // JSON field handler
    $handlers['json_decode'] = array(
        'title' => 'JSON Decode',
        'callback' => 'display_json_meta',
    );
    
    // Serialized data handler
    $handlers['unserialize'] = array(
        'title' => 'Unserialize Data',
        'callback' => 'display_serialized_meta',
    );
    
    // URL field handler
    $handlers['make_link'] = array(
        'title' => 'Make Clickable Link',
        'callback' => 'display_url_meta',
    );
    
    return $handlers;
}

function display_json_meta($post, $field) {
    $value = get_post_meta($post->ID, $field['meta_key'], true);
    $data = json_decode($value, true);
    
    if (json_last_error() === JSON_ERROR_NONE) {
        return '<pre>' . esc_html(print_r($data, true)) . '</pre>';
    }
    
    return $value;
}

function display_url_meta($post, $field) {
    $url = get_post_meta($post->ID, $field['meta_key'], true);
    
    if (filter_var($url, FILTER_VALIDATE_URL)) {
        return '<a href="' . esc_url($url) . '" target="_blank">' . esc_html($url) . '</a>';
    }
    
    return $url;
}
```

## Advanced Field Techniques

### Conditional Field Display

```php
add_filter('qw_fields', 'conditional_fields');

function conditional_fields($fields) {
    $fields['video_embed'] = array(
        'title' => 'Video Embed',
        'description' => 'Display video embed if available',
        'output_callback' => 'conditional_video_output',
        'output_arguments' => true,
    );
    
    return $fields;
}

function conditional_video_output($post, $field) {
    $video_url = get_post_meta($post->ID, 'video_url', true);
    
    if (empty($video_url)) {
        return ''; // No output if no video
    }
    
    // Check if it's a YouTube URL
    if (strpos($video_url, 'youtube.com') !== false || strpos($video_url, 'youtu.be') !== false) {
        return wp_oembed_get($video_url);
    }
    
    // For other video URLs, create a simple video element
    $video_formats = array('mp4', 'webm', 'ogg');
    $extension = pathinfo($video_url, PATHINFO_EXTENSION);
    
    if (in_array($extension, $video_formats)) {
        return '<video controls><source src="' . esc_url($video_url) . '" type="video/' . $extension . '"></video>';
    }
    
    return '<a href="' . esc_url($video_url) . '">View Video</a>';
}
```

### Multi-Value Field Processing

```php
add_filter('qw_fields', 'gallery_field');

function gallery_field($fields) {
    $fields['image_gallery'] = array(
        'title' => 'Image Gallery',
        'description' => 'Display a gallery of images from post meta',
        'form_callback' => 'gallery_field_form',
        'output_callback' => 'gallery_field_output',
        'output_arguments' => true,
    );
    
    return $fields;
}

function gallery_field_form($field) {
    $meta_key = isset($field['values']['gallery_meta_key']) ? $field['values']['gallery_meta_key'] : 'gallery_images';
    $columns = isset($field['values']['gallery_columns']) ? $field['values']['gallery_columns'] : 3;
    $size = isset($field['values']['image_size']) ? $field['values']['image_size'] : 'medium';
    
    ?>
    <div>
        <label>Meta Key:</label>
        <input type="text" 
               name="<?php echo $field['form_prefix']; ?>[gallery_meta_key]" 
               value="<?php echo esc_attr($meta_key); ?>" />
        <p class="description">Meta key containing image IDs (comma-separated or array)</p>
    </div>
    
    <div>
        <label>Columns:</label>
        <select name="<?php echo $field['form_prefix']; ?>[gallery_columns]">
            <?php for ($i = 1; $i <= 6; $i++): ?>
                <option value="<?php echo $i; ?>" <?php selected($columns, $i); ?>><?php echo $i; ?></option>
            <?php endfor; ?>
        </select>
    </div>
    
    <div>
        <label>Image Size:</label>
        <select name="<?php echo $field['form_prefix']; ?>[image_size]">
            <?php
            $sizes = get_intermediate_image_sizes();
            array_unshift($sizes, 'full');
            foreach ($sizes as $size_option):
                ?>
                <option value="<?php echo $size_option; ?>" <?php selected($size, $size_option); ?>><?php echo $size_option; ?></option>
            <?php endforeach; ?>
        </select>
    </div>
    <?php
}

function gallery_field_output($post, $field) {
    $meta_key = isset($field['gallery_meta_key']) ? $field['gallery_meta_key'] : 'gallery_images';
    $columns = isset($field['gallery_columns']) ? (int)$field['gallery_columns'] : 3;
    $size = isset($field['image_size']) ? $field['image_size'] : 'medium';
    
    $image_ids = get_post_meta($post->ID, $meta_key, true);
    
    if (empty($image_ids)) {
        return '';
    }
    
    // Handle different data formats
    if (is_string($image_ids)) {
        $image_ids = explode(',', $image_ids);
    } elseif (!is_array($image_ids)) {
        $image_ids = array($image_ids);
    }
    
    $image_ids = array_filter(array_map('intval', $image_ids));
    
    if (empty($image_ids)) {
        return '';
    }
    
    $output = '<div class="qw-gallery qw-gallery-columns-' . $columns . '">';
    
    foreach ($image_ids as $image_id) {
        $image = wp_get_attachment_image($image_id, $size);
        if ($image) {
            $output .= '<div class="qw-gallery-item">' . $image . '</div>';
        }
    }
    
    $output .= '</div>';
    
    return $output;
}
```

## Field Validation and Security

### Input Sanitization

```php
function secure_field_form($field) {
    $api_key = isset($field['values']['api_key']) ? $field['values']['api_key'] : '';
    $max_items = isset($field['values']['max_items']) ? (int)$field['values']['max_items'] : 5;
    
    ?>
    <div>
        <label>API Key:</label>
        <input type="password" 
               name="<?php echo $field['form_prefix']; ?>[api_key]" 
               value="<?php echo esc_attr($api_key); ?>" />
        <p class="description">Sensitive data - stored securely</p>
    </div>
    
    <div>
        <label>Max Items:</label>
        <input type="number" 
               name="<?php echo $field['form_prefix']; ?>[max_items]" 
               value="<?php echo esc_attr($max_items); ?>" 
               min="1" max="20" />
    </div>
    <?php
}

function secure_field_output($post, $field) {
    // Validate and sanitize all inputs
    $api_key = isset($field['api_key']) ? sanitize_text_field($field['api_key']) : '';
    $max_items = isset($field['max_items']) ? absint($field['max_items']) : 5;
    
    // Limit max items for security
    $max_items = min($max_items, 20);
    
    if (empty($api_key)) {
        return 'API key required';
    }
    
    // Use nonces for any AJAX requests
    $nonce = wp_create_nonce('field_ajax_' . $post->ID);
    
    // Escape all output
    return '<div data-nonce="' . esc_attr($nonce) . '" data-max="' . esc_attr($max_items) . '">Content</div>';
}
```

## Performance Optimization

### Caching Strategies

```php
function cached_field_output($post, $field) {
    $cache_key = 'field_data_' . $post->ID . '_' . md5(serialize($field));
    $cached_data = get_transient($cache_key);
    
    if ($cached_data !== false) {
        return $cached_data;
    }
    
    // Expensive operation
    $data = expensive_data_processing($post, $field);
    
    // Cache for 1 hour
    set_transient($cache_key, $data, HOUR_IN_SECONDS);
    
    return $data;
}
```

### Batch Processing

```php
function batch_processed_field_output($post, $field) {
    static $batch_data = array();
    static $processed_posts = array();
    
    // If we haven't processed this post's data yet
    if (!isset($processed_posts[$post->ID])) {
        // Batch process multiple posts at once
        if (empty($batch_data)) {
            $post_ids = get_batch_post_ids(); // Get current query post IDs
            $batch_data = get_batch_field_data($post_ids, $field);
        }
        
        $processed_posts[$post->ID] = true;
    }
    
    return isset($batch_data[$post->ID]) ? $batch_data[$post->ID] : '';
}
```

## Testing Custom Fields

### Unit Testing

```php
class CustomFieldTest extends WP_UnitTestCase {
    
    public function test_reading_time_calculation() {
        $post_id = $this->factory->post->create(array(
            'post_content' => str_repeat('word ', 200), // 200 words
        ));
        
        $post = get_post($post_id);
        $field = array(); // Empty field config
        
        $result = calculate_reading_time($post, $field);
        
        $this->assertEquals('1 min read', $result);
    }
    
    public function test_gallery_field_with_valid_ids() {
        $post_id = $this->factory->post->create();
        $attachment_id = $this->factory->attachment->create_object('image.jpg', $post_id);
        
        update_post_meta($post_id, 'gallery_images', array($attachment_id));
        
        $post = get_post($post_id);
        $field = array(
            'gallery_meta_key' => 'gallery_images',
            'gallery_columns' => 2,
            'image_size' => 'thumbnail',
        );
        
        $result = gallery_field_output($post, $field);
        
        $this->assertStringContains('qw-gallery-columns-2', $result);
        $this->assertStringContains('qw-gallery-item', $result);
    }
}
```

## Migration and Backwards Compatibility

When updating field structures, ensure backwards compatibility:

```php
function backwards_compatible_field_output($post, $field) {
    // Handle old field format
    if (isset($field['old_format_key'])) {
        $field['new_format_key'] = migrate_old_format($field['old_format_key']);
        unset($field['old_format_key']);
    }
    
    // Provide defaults for new options
    $new_option = isset($field['new_option']) ? $field['new_option'] : 'default_value';
    
    return generate_field_output($post, $field, $new_option);
}
```

This comprehensive guide covers all aspects of field development in Query Wrangler, from simple custom fields to complex multi-value processing with caching and security considerations.