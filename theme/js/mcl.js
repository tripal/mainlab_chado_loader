(function($) {

  // Deals with functionality that integrates with Drupal.
  Drupal.behaviors.mcl = {
    attach: function (context, settings) {

      // Adds a confirmation dialog before perform an ajax action.
      $('.mcl-confirm').each(function() {
        Drupal.ajax[this.id].beforeSend = function (xmlhttprequest, options) {
          if(confirm('Please click "OK" to confirm this action?')) {
            return true;
          }
          xmlhttprequest.abort();
        }
      });

      // Loads the log file on the viewer.
      $(".mcl_log").click(function() {
        var id_viewer = '#' + $(this).attr("viewer-id");
        $.ajax({
          url : $(this).attr("log-file"),
          dataType: "text",
          success : function (data) {
            $(id_viewer).html(data.replace(/\n/g, '<br />'));
          }
        });
      });

    } // End of attach.
  }; // End of Drupal.behaviors.mcl
})(jQuery);