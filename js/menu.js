/*************************************************************************
 * For loading the menu.
 * The menu will not work if you open the file directly rather than on a web server.
 *************************************************************************/

(function () {
  "use strict";

  $(function () {
    $(".menu-container").load("menu.html", function () {
      // Highlight current page
      var path = window.location.pathname;
      var page = path.split("/").pop();

      // Normalize index page
      if (page === "" || page === "index.html") {
        page = "index.html";
      }

      var pageNameWithoutExtension = page.replace(".html", "");
      var specificPageClass = "menu-" + pageNameWithoutExtension;

      // Clear previous highlights
      $(".menu-items a").removeClass("menu-highlight");
      $(".menu-items .dropbtn").removeClass("menu-highlight");

      // Find the link that matches the current page directly (e.g., future-innovations.html)
      var $activeLink = $('.menu-items a[href^="' + page + '"]').not('.dropdown-content a');

      if ($activeLink.length > 0) {
        $activeLink.first().addClass('menu-highlight');
      } else {
        // If no direct link is found, check for a class match (for index page)
        $('.' + specificPageClass).addClass('menu-highlight');
      }

      // Check if an active link is inside a dropdown
      var $activeDropdownLink = $('.dropdown-content a[href^="' + page + '"]');
      if ($activeDropdownLink.length > 0) {
        // Highlight the dropdown item itself and the parent dropbtn
        $activeDropdownLink.first().addClass('menu-highlight');
        $activeDropdownLink.first().closest('.dropdown').find('.dropbtn').addClass('menu-highlight');
      }

      // Special handling for hash links on other pages
      var hash = window.location.hash;
      if (hash) {
        $('.dropdown-content a[href$="' + hash + '"]').each(function () {
          var $this = $(this);
          // If the link's page part matches the current page, highlight its parent
          if ($this.attr('href').startsWith(page)) {
            $this.closest('.dropdown').find('.dropbtn').addClass('menu-highlight');
          }
        });
      }

    });
  });
})();