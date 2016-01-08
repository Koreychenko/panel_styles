<div<?php print drupal_attributes($style_attributes); ?>>
<?php if ($pane_prefix): ?>
  <?php print $pane_prefix; ?>
<?php endif; ?>
<?php if ($admin_links): ?>
    <?php print $admin_links; ?>
  <?php endif; ?>
    <?php if ($type == 'container'): ?>
      <div class="container">
    <?php endif; ?>
    <?php print render($content); ?>
    <?php if ($type == 'container'): ?>
      </div>
    <?php endif; ?>
  <?php if ($pane_suffix): ?>
  <?php print $pane_suffix; ?>
<?php endif; ?>
</div>

