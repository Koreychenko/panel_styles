(function($) {
  Drupal.behaviors.sasta_panelst_admin = {
    attach: function (context, settings) {
      var opener = $('<a href="#">Показать все категории</a>');
      $('.panels-add-content-modal').once('opener',function(){
        opener.appendTo($('.panels-add-content-modal'));
        opener.click(function(){
          if ($('.panels-section-column-categories').hasClass('open')) {
            $('.panels-section-column-categories').removeClass('open');
            opener.html('Показать все категории');
          } else {
            $('.panels-section-column-categories').addClass('open');
            opener.html('Скрыть категории');
          }

        });
      })
    }
  };
})(jQuery);
