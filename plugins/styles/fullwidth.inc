<?php

// Plugin definition
$plugin = array(
  // Title and description of the plugin
  'title' => t('Блок с фоном на всю ширину'),
  'description' => t('Allow to set fullwidth background.'),
  // Define a theme function for this plugin
  'render pane' => 'fullwidth_background_render_pane',
  // We'll be using a template for rendering
  'hook theme' => array(
    'fullwidth_background_picture_style' => array(
      'variables' => array(
        'content' => NULL,
        'type' => NULL,
        'style_attributes' => array(),
        'admin_links' => NULL,
        'pane_prefix' => NULL,
        'pane_suffix' => NULL,
        'title' => NULL,
        'inner_classes_array' => NULL,
      ),
      'path' => drupal_get_path('module', 'sasta_panelst') . '/plugins/styles/theme',
      'template' => 'fullwidth-background',
    ),
  ),
  // This defines the settings form for the plugin
  'pane settings form' => 'fullwidth_background_style_settings_form',
   'pane settings form submit' => 'fullwidth_background_style_settings_form_submit',

);

/**
 * Settings form callback.
 */
function fullwidth_background_style_settings_form(&$form, &$form_state) {

  $settings = array();

  $settings['row_type'] = isset($form['row_type'])?$form['row_type']:'';

  $settings['padding_left'] = $form['paddings']['padding_left'];
  $settings['padding_right'] = $form['paddings']['padding_right'];
  $settings['padding_top'] = $form['paddings']['padding_top'];
  $settings['padding_bottom'] = $form['paddings']['padding_bottom'];
  $settings['height'] = $form['height'];
  $settings['background_color'] = $form['colors']['background_color'];
  $settings['text_color'] = $form['colors']['text_color'];
  $settings['background_image'] = $form['colors']['background_image'];
  $settings['background_repeat'] = $form['colors']['background_repeat'];
  $settings['background_position'] = $form['colors']['background_position'];

  ipe_layout_base_settings_form($form, $form_state,$settings);

  $form['header_position'] = array(
    '#type' => 'select',
    '#options' => array(
      'left' => t('Left'),
      'center' => t('Center'),
      'right' => t('Right'),
      ),
    '#title' => t('Header position'),
    '#default_value' => isset($form['header_position'])?$form['header_position']:'left',
    );

  $form['class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS Classes'),
    '#description' => t('Enter CSS classes for this style. Separate multiple classes by spaces.'),
    '#default_value' => (isset($form['class'])) ? $form['class'] : '',
  );

  $form['#submit'][] = 'fullwidth_background_style_settings_form_submit';

  return $form;
}

function fullwidth_background_style_settings_form_submit(&$form, &$form_state) {


}

/**
 * Render callback.
 */
function theme_fullwidth_background_render_pane($vars) {

  $inner_classes_array = array();

  $background_color = '';

  $style = '';

  $styles = array('class'=>array(),'style'=>array());

  if (isset($vars['settings']['paddings']['padding_top'])) {
    $styles['style'][] = 'padding-top:'.$vars['settings']['paddings']['padding_top'].'px!important;';
  }

  if (isset($vars['settings']['paddings']['padding_bottom'])) {
    $styles['style'][] = 'padding-bottom:'.$vars['settings']['paddings']['padding_bottom'].'px!important;';
  }

  if (isset($vars['settings']['paddings']['padding_left'])) {
    $styles['style'][] = 'padding-left:'.$vars['settings']['paddings']['padding_left'].'px!important;';
  }

  if (isset($vars['settings']['paddings']['padding_right'])) {
    $styles['style'][] = 'padding-right:'.$vars['settings']['paddings']['padding_right'].'px!important;';
  }

  if (isset($vars['settings']['colors']['background_color'])) {
    $styles['style'][] = 'background-color:'.$vars['settings']['colors']['background_color'].'!important;';
  }

  if (isset($vars['settings']['height'])) {
    $styles['style'][] = 'height:'.$vars['settings']['height'].'px!important;';
    $styles['style'][] = 'overflow: hidden!important;';
  }

  if (isset($vars['settings']['colors']['text_color'])) {
    $styles['style'][] = 'color:'.$vars['settings']['colors']['text_color'].'!important;';
  }

  if (isset($vars['settings']['colors']['background_image'])) {
    $file = file_load($vars['settings']['colors']['background_image']);
    if ($file) {
      $image = file_create_url($file->uri);
      $styles['style'][] = 'background-image: url('.$image.')!important;';
    }
  }

  if (isset($vars['settings']['colors']['background_repeat'])) {
    switch ($vars['settings']['colors']['background_repeat']) {
      case 'no-repeat':
        $styles['style'][] = 'background-repeat: no-repeat!important;';
      break;
      case 'fullsize':
        $styles['style'][] = 'background-repeat: no-repeat!important;';
        $styles['style'][] = 'background-size: 100% auto!important;';
      break;
    }
  }

  if (isset($vars['settings']['colors']['background_position'])) {
    switch ($vars['settings']['colors']['background_position']) {
      case 'top-center':
        $styles['style'][] = 'background-position: 50% 0!important;';
      break;
      case 'middle-center':
        $styles['style'][] = 'background-position: 50% 50%!important;';
      break;
      case 'bottom-center':
        $styles['style'][] = 'background-position: 50% 100%!important;';
      break;
      case 'left-top':
        $styles['style'][] = 'background-position: 0% 0%!important;';
      break;
      case 'left-bottom':
        $styles['style'][] = 'background-position: 0% 100%!important;';
      break;
      case 'right-top':
        $styles['style'][] = 'background-position: 100% 0%!important;';
      break;
      case 'right-bottom':
        $styles['style'][] = 'background-position: 100% 100%!important;';
      break;
    }
  }

  // Add our classes to the attrubutes array, if any defined

  $content = $vars['content']->content;

  $vars['pane_prefix'] = !empty($content->pane_prefix) ? $content->pane_prefix : '';
  $vars['pane_suffix'] = !empty($content->pane_suffix) ? $content->pane_suffix : '';

  $vars['title'] = !empty($vars['content']->title) ? $vars['content']->title : '';

  $vars['contextual_links'] = array();

  $vars['classes_array'] = array();
  $vars['admin_links'] = '';
  $links = array();
  if (module_exists('contextual') && user_access('access contextual links')) {
    $links = array();
    // These are specified by the content.
    if (!empty($vars['content']->admin_links)) {
      $links += $vars['content']->admin_links;
    }

    // Take any that may have been in the render array we were given and
    // move them up so they appear outside the pane properly.
    if (is_array($vars['content']->content) && isset($vars['content']->content['#contextual_links'])) {
      $element = array(
        '#type' => 'contextual_links',
        '#contextual_links' => $vars['content']->content['#contextual_links'],
      );
      unset($vars['content']->content['#contextual_links']);

      // Add content to $element array
      if (is_array($vars['content']->content)) {
        $element['#element'] = $vars['content']->content;
      }

      $element = contextual_pre_render_links($element);
      if(!empty($element['#links'])) {
        $links += $element['#links'];
      }
    }

    if ($links) {
      $build = array(
        '#prefix' => '<div class="contextual-links-wrapper">',
        '#suffix' => '</div>',
        '#theme' => 'links__contextual',
        '#links' => $links,
        '#attributes' => array('class' => array('contextual-links')),
        '#attached' => array(
          'library' => array(array('contextual', 'contextual-links')),
        ),
      );

      $inner_classes_array[] = 'contextual-links-region';
      $vars['admin_links'] = drupal_render($build);
    }
  }

  if (isset($vars['settings']['advanced'])) {
    if (isset($vars['settings']['class'])) {
    if ($vars['settings']['class']) {
      $styles['class'] = explode(' ', $vars['settings']['class']);
    }
  }
}


  $styles['class'][] = 'fullwidth-background-pane';

  $inner_classes_array = implode(' ', $inner_classes_array);

  ctools_include('cleanstring');

  $type_class = $vars['content']->type ? 'pane-'. ctools_cleanstring($vars['content']->type, array('lower case' => TRUE)) : '';
  $subtype_class = $vars['content']->subtype ? 'pane-'. ctools_cleanstring($vars['content']->subtype, array('lower case' => TRUE)) : '';

  // Sometimes type and subtype are the same. Avoid redundant classes.
  $styles['class'][] = $type_class;
  $styles['class'][] = 'panel-pane';
  if ($type_class != $subtype_class) {
    $styles['class'][] = $subtype_class;
  }

  if (isset($vars['settings']['header_position'])) {
    $styles['class'][] = 'header-position-'.$vars['settings']['header_position'];
  }


  return theme('fullwidth_background_picture_style', array(
      'type' => isset($vars['settings']['row_type'])?$vars['settings']['row_type']:'fullwidth',
      'content' => $content,
      'style_attributes' => $styles,
      'inner_classes_array' => $inner_classes_array,
      'admin_links' => $vars['admin_links'],
      'title' => $vars['title'],
      'pane_prefix' => $vars['pane_prefix'],
      'pane_suffix' => $vars['pane_suffix'],
    )
  );
}
