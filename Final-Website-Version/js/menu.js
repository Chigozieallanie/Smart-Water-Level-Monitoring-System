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

      // Initialize dropdown functionality after menu is loaded
      setTimeout(function () {
        initDropdownFunctionality();
        initMobileMenuFunctionality();
      }, 100);

      // Dropdown initialization function
      function initDropdownFunctionality() {
        const dropdowns = document.querySelectorAll('.dropdown');

        dropdowns.forEach(dropdown => {
          const dropbtn = dropdown.querySelector('.dropbtn');
          const dropdownContent = dropdown.querySelector('.dropdown-content');

          if (dropbtn && dropdownContent) {
            // Remove existing event listeners by cloning
            const newDropbtn = dropbtn.cloneNode(true);
            dropbtn.parentNode.replaceChild(newDropbtn, dropbtn);

            let closeTimeout;
            // For desktop: hover functionality
            if (window.innerWidth > 768) {
              // Hover events for desktop
              newDropbtn.addEventListener('mouseenter', function () {
                clearTimeout(closeTimeout);
                dropdown.classList.add('active');
              });
              newDropbtn.addEventListener('mouseleave', function () {
                closeTimeout = setTimeout(() => dropdown.classList.remove('active'), 150);
              });
              dropdownContent.addEventListener('mouseenter', function () {
                clearTimeout(closeTimeout);
                dropdown.classList.add('active');
              });
              dropdownContent.addEventListener('mouseleave', function () {
                closeTimeout = setTimeout(() => dropdown.classList.remove('active'), 150);
              });
              // Click functionality for accessibility
              newDropbtn.addEventListener('click', function (e) {
                e.preventDefault();
                e.stopPropagation();
                // Close other dropdowns
                dropdowns.forEach(otherDropdown => {
                  if (otherDropdown !== dropdown) {
                    otherDropdown.classList.remove('active');
                  }
                });
                // Toggle current dropdown
                dropdown.classList.toggle('active');
              });
            } else {
              // For mobile: click functionality
              newDropbtn.addEventListener('click', function (e) {
                e.preventDefault();
                e.stopPropagation();
                // Close other dropdowns
                dropdowns.forEach(otherDropdown => {
                  if (otherDropdown !== dropdown) {
                    otherDropdown.classList.remove('active');
                  }
                });
                // Toggle current dropdown
                dropdown.classList.toggle('active');
              });
            }
          }
        });

        // Close dropdowns when clicking outside
        document.addEventListener('click', function (e) {
          if (!e.target.closest('.dropdown')) {
            dropdowns.forEach(dropdown => {
              dropdown.classList.remove('active');
            });
          }
        });

        // Handle window resize
        window.addEventListener('resize', function () {
          dropdowns.forEach(dropdown => {
            dropdown.classList.remove('active');
          });
        });
      }

      // Mobile menu functionality
      function initMobileMenuFunctionality() {
        const menuButton = document.querySelector('.menu-button');
        const menuItems = document.querySelector('.menu-items');

        if (menuButton && menuItems) {
          // Remove existing event listeners by cloning
          const newMenuButton = menuButton.cloneNode(true);
          menuButton.parentNode.replaceChild(newMenuButton, menuButton);

          // Add click event to menu button
          newMenuButton.addEventListener('click', function (e) {
            e.preventDefault();
            e.stopPropagation();

            // Toggle menu visibility
            menuItems.classList.toggle('show');
          });

          // Close menu when clicking outside
          document.addEventListener('click', function (e) {
            if (!e.target.closest('.menu-table')) {
              menuItems.classList.remove('show');
            }
          });

          // Close menu when window is resized to desktop
          window.addEventListener('resize', function () {
            if (window.innerWidth > 768) {
              menuItems.classList.remove('show');
            }
          });

          // Handle menu item clicks on mobile
          const menuLinks = menuItems.querySelectorAll('a:not(.dropbtn)');
          menuLinks.forEach(link => {
            link.addEventListener('click', function () {
              // Close menu when a link is clicked
              menuItems.classList.remove('show');
            });
          });
        }
      }
    });
  });
})();