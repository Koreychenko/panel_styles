<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if (!$label_hidden): ?>
    <div class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</div>
  <?php endif; ?>
  <div class="field-items row"<?php print $content_attributes; ?>>
    <?php $count = count($items); ?>
    <?php $row_size = floor(12 / $count); ?>
    <?php foreach ($items as $delta => $item): ?>
      <?php if ($delta == ($count-1)) {
        $row_size = 12 - $row_size * $delta;
      }

      $class = 'col-md-'.$row_size;

      ?>

      <div class="<?php print $class; ?> field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"<?php print $item_attributes[$delta]; ?>><?php print render($item); ?></div>
    <?php endforeach; ?>
  </div>
</div>
