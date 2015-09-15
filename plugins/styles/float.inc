<?php

// Plugin definition
$plugin = array(
  // Title and description of the plugin
  'title' => 'Врезка',
  'description' => t('Добавлять врезки слева или справа'),
  // Define a theme function for this plugin
  'render pane' => 'float_background_render_pane',
  // We'll be using a template for rendering
  'hook theme' => array(
    'float_background_picture_style' => array(
      'variables' => array(
        'content' => NULL,
        'style_attributes' => array(),
        'admin_links' => NULL,
        'pane_prefix' => NULL,
        'pane_suffix' => NULL,
        'title' => NULL,
        'position' => NULL,
      ),
      'path' => drupal_get_path('module', 'sasta_panelst') . '/plugins/styles/theme',
      'template' => 'float-background',
    ),
  ),
  // This defines the settings form for the plugin
  'pane settings form' => 'float_background_style_settings_form',
   'pane settings form submit' => 'float_background_style_settings_form_submit',

);

/**
 * Settings form callback.
 */
function float_background_style_settings_form($form, $form_state) {

  $styles = array();

  $styles = module_invoke_all('sasta_panelst', $styles);

  if (!empty($styles)) {
    $style_options = array('0' => 'Не выбрано');

    foreach ($styles as $id => $title) {
      $style_options[$id] = $title['title'];
    }
    $form['style'] = array(
      '#type' => 'select',
      '#title' => 'Выберите стиль',
      '#options' => $style_options,
      '#default_value' => ($form['style']) ? $form['style'] : array_pop(array_keys($style_options)),
      );
  }

  $form['position'] = array(
    '#type' => 'select',
    '#title' => 'Положение врезки',
    '#options' => array('left' => 'Слева', 'right' => 'Справа'),
    '#default_value' => ($form['position']) ? $form['position'] : 'left',
    );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => 'Дополнительные настройки',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'tree' => FALSE,
    );

  $form['advanced']['background'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#description' => t('Enter the background color in HEX format. You may use substitutions in this field.'),
    '#required' => FALSE,
    '#default_value' => (isset($form['advanced']['background'])) ? $form['advanced']['background'] : '',
  );

  $form['advanced']['padding_top'] = array(
    '#type' => 'textfield',
    '#title' => t('Padding top'),
    '#description' => t('Padding top value'),
    '#required' => FALSE,
    '#default_value' => (isset($form['advanced']['padding_top'])) ? $form['advanced']['padding_top'] : '15px',
  );

  $form['advanced']['padding_bottom'] = array(
    '#type' => 'textfield',
    '#title' => t('Padding bottom'),
    '#description' => t('Padding bottom value'),
    '#required' => FALSE,
    '#default_value' => (isset($form['advanced']['padding_bottom'])) ? $form['advanced']['padding_bottom'] : '15px',
  );



  if ((isset($form['advanced']['image'])) && ($form['advanced']['image'])) {
    $form['advanced']['image'] = array('#type' => 'hidden', '#value' => $form['advanced']['image']);
  } else {
    $form['advanced']['image'] = array(
      '#type' => 'managed_file',
      '#title' => t('Background Image'),
      '#required' => FALSE,
      '#default_value' => '',
      '#upload_location' => 'public://',
      '#upload_validators' => array(
        'file_validate_is_image' => array(),
        'file_validate_extensions' => array('png gif jpg jpeg'),
        'file_validate_size' => array(1 * 1024 * 1024),
      ),
    );
  }
  $form['advanced']['class'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS Classes'),
    '#description' => t('Enter CSS classes for this style. Separate multiple classes by spaces.'),
    '#default_value' => (isset($form['advanced']['class'])) ? $form['advanced']['class'] : '',
  );

  $form['#submit'][] = 'fullwidth_background_style_settings_form_submit';

  return $form;
}

function float_background_style_settings_form_submit(&$form, &$form_state) {
  if ($form_state['image']) {
    if (is_object($form_state['image'])) {
      $file = file_load($form_state['image']);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      $form_state['image'] = $file->fid;
    }
  }
}

/**
 * Render callback.
 */
function theme_float_background_render_pane($vars) {

  $background_color = $vars['settings']['advanced']['background'];

  $style = $vars['settings']['style'];

  $padding_top = isset($vars['settings']['advanced']['padding_top'])?$vars['settings']['advanced']['padding_top']:'';

  $padding_bottom = isset($vars['settings']['advanced']['padding_bottom'])?$vars['settings']['advanced']['padding_bottom']:'';

  $image_url = '';

  if ($vars['settings']['advanced']['image']) {
    $image_url = file_load($vars['settings']['advanced']['image']);
    if ($image_url) {
      $image_url = file_create_url($image_url->uri);
    }
  }

  $style_attributes = array();
  $style_attributes['style'] = array();

  if ($style) {
    $style_attributes['class'][] = 'sasta_panelst_style_'.$style;
  }

  if ($image_url) {
    $style_attributes['style'][] = 'background-image: url(' . $image_url . ');';
    $style_attributes['style'][] = 'background-repeat: repeat-x repeat-y;';
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

  $style_attributes['style'] = implode(' ',$style_attributes['style']);

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
      $vars['classes_array'][] = 'contextual-links-region';
      $vars['admin_links'] = drupal_render($build);
    }
  }

  if ($vars['settings']['advanced']['class']) {
    $style_attributes['class'] = explode(' ', $vars['settings']['advanced']['class']);
  }

  $style_attributes['class'][] = 'fullwidth-background-pane';

  if ($links) {
    $style_attributes['class'][] = 'contextual-links-region';
  }

  ctools_include('cleanstring');

  $type_class = $vars['content']->type ? 'pane-'. ctools_cleanstring($vars['content']->type, array('lower case' => TRUE)) : '';
  $subtype_class = $vars['content']->subtype ? 'pane-'. ctools_cleanstring($vars['content']->subtype, array('lower case' => TRUE)) : '';

  // Sometimes type and subtype are the same. Avoid redundant classes.
  $style_attributes['class'][] = $type_class;
  $style_attributes['class'][] = 'panel-pane';
  $style_attributes['class'][] = 'float-content-'.$vars['settings']['position'];
  if ($type_class != $subtype_class) {
    $style_attributes['class'][] = $subtype_class;
  }


  return theme('float_background_picture_style', array(
      'content' => $content,
      'style_attributes' => $style_attributes,
      'admin_links' => $vars['admin_links'],
      'title' => $vars['title'],
      'position' => $vars['settings']['position'],
      'pane_prefix' => $vars['pane_prefix'],
      'pane_suffix' => $vars['pane_suffix'],
    )
  );
}
