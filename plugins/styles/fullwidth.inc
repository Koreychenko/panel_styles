<?php

// Plugin definition
$plugin = array(
  // Title and description of the plugin
  'title' => t('Fullwidth pane with background'),
  'description' => t('Allow to set fullwidth background.'),
  // Define a theme function for this plugin
  'render pane' => 'fullwidth_background_render_pane',
  // We'll be using a template for rendering
  'hook theme' => array(
    'fullwidth_background_picture_style' => array(
      'variables' => array(
        'content' => NULL,
        'style_attributes' => array(),
      ),
      'path' => drupal_get_path('module', 'sasta_panelst') . '/plugins/styles/theme',
      'template' => 'fullwidth-background',
    ),
  ),
  // This defines the settings form for the plugin
  'pane settings form' => 'fullwidth_background_style_settings_form',
);

/**
 * Settings form callback.
 */
function fullwidth_background_style_settings_form($form, $form_state) {

  $form['background'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t('Enter the background color in HEX format. You may use substitutions in this field.'),
    '#required' => FALSE,
    '#default_value' => (isset($form['background'])) ? $form['background'] : '',
  );

  $form['padding_top'] = array(
    '#type' => 'textfield',
    '#title' => t('Padding top'),
    '#description' => t('Padding top value'),
    '#required' => FALSE,
    '#default_value' => (isset($form['padding_top'])) ? $form['padding_top'] : '',
  );

  $form['padding_bottom'] = array(
    '#type' => 'textfield',
    '#title' => t('Padding bottom'),
    '#description' => t('Padding bottom value'),
    '#required' => FALSE,
    '#default_value' => (isset($form['padding_bottom'])) ? $form['padding_bottom'] : '',
  );

  $form['image'] = array(
    '#type' => 'textfield',
    '#title' => t('Image field'),
    '#description' => t('Enter the image field for the background image. You may use substitutions in this field. E.g. for the default image field attached to articles use "%node:field_image"'),
    '#required' => FALSE,
    '#default_value' => (isset($form['image'])) ? $form['image'] : '',
  );
  $form['image_style'] = array(
    '#type' => 'select',
    '#title' => t('Image Style'),
    '#description' => t('Choose the appropriate image style for the background image.'),
    '#options' => image_style_options(),
    '#default_value' => (isset($form['image_style'])) ? $form['image_style'] : '',
  );
  $form['class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS Classes'),
    '#description' => t('Enter CSS classes for this style. Separate multiple classes by spaces.'),
    '#default_value' => (isset($form['class'])) ? $form['class'] : '',
  );

  return $form;
}

/**
 * Render callback.
 */
function theme_fullwidth_background_render_pane($vars) {
  $image_url = NULL;

  // Get the absolute path of the original image from the context substitution
  $image_abs_path = ctools_context_keyword_substitute($vars['settings']['image'], array(), $vars['display']->context);

  $image_style = $vars['settings']['image_style'];

  $background_color = $vars['settings']['background'];

  $padding_top = isset($vars['settings']['padding_top'])?$vars['settings']['padding_top']:'';

  $padding_bottom = isset($vars['settings']['padding_bottom'])?$vars['settings']['padding_bottom']:'';

  if ($image_style == '') {
    // If no image style is selected, use the original image.
    $image_url = $image_abs_path;
  } else {
    // Image style is provided in the settings form.
    // We need to get the original image uri to return the URL for an image derivative.
    global $base_url;
    $files_rel_path = variable_get('file_public_path', conf_path() . '/files');
    $image_rel_path = str_replace($base_url . '/' . $files_rel_path, '', $image_abs_path);
    $image_uri = file_build_uri($image_rel_path);

    $image_style_url = image_style_url($image_style, $image_uri);

    $image_url = $image_style_url;
  }

  $style_attributes = array();
  $style_attributes['style'] = array();

  if ($image_url) {
    $style_attributes['style'][] = 'background-image: url(' . $image_url . ');';
  }

  if ($background_color) {
    $style_attributes['style'][] = 'background-color: '.$background_color .';';
  }

  if ($padding_top) {
    $style_attributes['style'][] = 'padding-top: '.$padding_top .';';
  }

  if ($padding_bottom) {
    $style_attributes['style'][] = 'padding-bottom: '.$padding_bottom;
  }

  // Add our classes to the attrubutes array, if any defined


  if ($vars['settings']['class']) {
    $style_attributes['class'] = explode(' ', $vars['settings']['class']);
  }

  $style_attributes['class'][] = 'fullwidth-background-pane';

  $style_attributes['style'] = implode(' ',$style_attributes['style']);

  $content = $vars['content']->content;

  return theme('fullwidth_background_picture_style', array(
      'content' => $content,
      'style_attributes' => $style_attributes,
    )
  );
}
