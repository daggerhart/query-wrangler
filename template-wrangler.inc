<?php
/*
 * Template Wrangler
 *
 * Functions
 * theme() - template some arguments
 *
 */

/*
 * Main Templating function
 *
 * @param string $template_name The array key given when defining a new template
 * @param array  $arguments Array of variables to pass using key -> value pairs.  In the template file the key will be the variable name, eg $key
 *
 * @return string Template HTML
 */
function theme($template_name = '', $arguments = array())
{
  if(is_null($arguments)){
    $arguments = array();
  }

  // get all the hooks
  $templates = tw_templates();

  // make sure given templates key exists
  if(is_array($templates[$template_name]))
  {
    // template we're using
    $template = $templates[$template_name];

    // get theme folder name
    $theme_folder = explode("/", TEMPLATEPATH);
    $theme_folder = array_pop($theme_folder);

    // if not arguments array given, make empty array for future actions
    if(!is_array($template['arguments'])){
      $template['arguments'] = array();
    }

    // merge passed arguments into array overriding defaults
    $arguments = array_merge($template['arguments'], $arguments);
    // update the template arguments for preprocess functions
    $template['arguments'] = $arguments;

    // if there is a theme based preprocess function, run it on the arguments
    if(function_exists($theme_folder.'_'.$template_name.'_preprocess')){
      $preprocess = $theme_folder.'_'.$template_name.'_preprocess';
      $template = call_user_func($preprocess, $template);
    }
    // else if there is a default preprocess function, run it on the arguments
    else if(function_exists('theme_'.$template_name.'_preprocess')){
      $preprocess = 'theme_'.$template_name.'_preprocess';
      $template = call_user_func($preprocess, $template);
    }

    // if no default_path, set to function execution path
    if(!isset($template['default_path']))
    {
      // get function caller backtrace
      $bt = debug_backtrace();
      $caller = array_shift($bt);

      // set the function caller's file
      // @TODO: instead, find a way to set the default path to where the hook was set
      $template['default_path'] = dirname($caller['file']);
    }

    // if no templates were given, look for theme functions
    if(!isset($template['files']))
    {
      // look for theme folder name name based function
      if(function_exists($theme_folder.'_'.$template_name)){
        // function found, execute it and return the results
        return call_user_func_array($theme_folder.'_'.$template_name, $arguments);
      }
      // see if a function using the template name exists
      else if(function_exists('theme_'.$template_name)){
        // function found, execute it and return the results
        return call_user_func_array('theme_'.$template_name, $arguments);
      }
    }
    // else if given templates is not an array, make it one
    else if(!is_array($template['files']))
    {
      $template['files'] = array($template['files']);
    }

    // loop through and find an existing suggested template
    $found_path = '';
    
    foreach($template['files'] as $suggestion)
    {
      // look for an argument in template suggestions
      if (preg_match('/\[.*\]/',$suggestion))
      {
        // we have arguments, lets build the possibilities
        $search = array();
        $replace = array();
        foreach($arguments as $key => $value)
        {
          // only apply to strings and numerics
          if(is_string($arguments[$key]) || is_numeric($arguments[$key])){
            $search[] = '['.$key.']';
            $replace[] = $value;
          }
        }

        // do the replacement
        $suggestion = str_replace($search, $replace, $suggestion);
      }

      // easier to read this way
      $theme_path = TEMPLATEPATH.'/'.$suggestion;
      $default_path = rtrim($template['default_path']).'/'.$suggestion;

      // look in the theme folder
      if(file_exists($theme_path)){
        $found_path = $theme_path;
        break;
      }
      // look for file in default path
      else if(file_exists($default_path)){
        $found_path = $default_path;
        break;
      }
    }

    // proceed if file found
    if($found_path != '')
    {
      // start output buffer
      ob_start();

        // turn arguments into variables
        extract($arguments);

        // include the file
        include $found_path;

      // return output buffer
      return ob_get_clean();
    }
  }
}

/*
 * Hook for adding templates to the system
 *
 * @return array All templates
 */
function tw_templates(){
  $templates = apply_filters('tw_templates', array());
  return $templates;
}

/* very simple link function */
function theme_link($href, $text){
  return "<a href='$href'>$text</a>";
}

/*
 * Example template architecture
 *
 * @param $templates array Passed from the filter hook from WP
 *        $templates['function_name'] = array(
 *          'files' => array(
 *            'first-template-to-look-for.php',
 *            'second-template-to-look-for.php'
 *            ),
 *          'default_path' => '/folder/location/of/default/template',
 *          'arguments' => array(
 *            'argument-1' => 'argument-1's default value',
 *            'argument-2' => 'argument-2's default value',
 *          )
 *        )
 *
 *  @return array All template arrays filtered so far by Wordpress' filter hook
 */
/*
 * EXAMPLES
 *
// how to include template-wrangler in a plugin
// include Template Wrangler
if(!function_exists('theme')){
  include_once QW_PLUGIN_DIR.'/template-wrangler.inc';
}

// example template hook
function example_template($templates)
{
  // template applied by files
  $templates['example_template'] = array(
    // string of single template suggestion
    // or array of multiple template suggestions
    'files' => array(
        // use argument keys as replacable patterns with the argument's value
        // order is important as the order here is the order inwhich templates are searched for
        'mytemplate-[arg1]-[arg2].php',
        'mytemplate-[arg1].php',
        // a default fall back template name
        'mytemplate.php'
    ),

    // location of the default template if nothing is found
    'default_path' => dirname(__FILE__),

    // optional arguments to be passed to the themeing function
    'arguments' => array(
        // must be key => value pairs
        'arg1' => 'arg1_default_value',
        'arg2' => 'arg2_default_value',
    ),
  );

  // theme function example
  // since no template files are specified, it will attempt to run a function named theme_link()
  // it will look first for a funtion name THEME_FOLDER_NAME_link()
  // example:  if your active theme is in a folder name my_theme, it will look for my_theme_link()
  $templates['link'] = array(
    'default_path' => dirname(__FILE__),
    'arguments' => array(
      'href' => 'http://google.com',
      'text' => 'your link text',
    )
  );

  return $templates;
}
// hook your function into the wordpress filter
add_filter('tw_templates', 'example_template');

// example preprocess function
function example_template_preprocess($template)
{
  // change argument values
  $template['arguments']['arg2'] = '1';
  // add a new argument
  $template['arguments']['new_arg'] = 'brand new argument';

  // add a new template suggestions as the first suggestion
  array_unshift($template['files'], 'mytemplate-new-first-suggestion.php');
  // add a new template suggestion as the last suggestion
  $template['files'][] = 'mytemplate-new-final-suggestion.php';

  // return the modified template
  return $template;
}

// example theme function
function theme_link($href, $text){
  return "<a href='$href'>$text</a>";
}
*/