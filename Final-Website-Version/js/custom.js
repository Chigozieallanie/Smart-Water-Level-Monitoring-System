/*************************************************************************
 * Your custom JS file
 *************************************************************************/

(function () {
  "use strict";

  var widgets; // Keep if edaplotjs.Widgets is used for some basic things like dialogs

  // Helper function to check if an element is in viewport for scroll animations
  function isInViewport(element) {
    if (!element) return false;
    const rect = element.getBoundingClientRect();
    return (
      rect.top >= 0 &&
      rect.left >= 0 &&
      rect.bottom <= (window.innerHeight || document.documentElement.clientHeight) &&
      rect.right <= (window.innerWidth || document.documentElement.clientWidth)
    );
  }

  function isPartiallyInViewport(element) {
    if (!element) return false;
    const rect = element.getBoundingClientRect();
    const windowHeight = (window.innerHeight || document.documentElement.clientHeight);
    const windowWidth = (window.innerWidth || document.documentElement.clientWidth);
    // Check if any part of the element is in the viewport
    const vertInView = (rect.top <= windowHeight) && ((rect.top + rect.height) >= 0);
    const horInView = (rect.left <= windowWidth) && ((rect.left + rect.width) >= 0);
    return vertInView && horInView;
  }


  // Scroll-triggered animations
  function setupScrollAnimations() {
    const sections = document.querySelectorAll('.scroll-reveal-section, .timeline-item');
    if (!sections.length) return;

    const observer = new IntersectionObserver(entries => {
      entries.forEach(entry => {
        if (entry.isIntersecting) {
          entry.target.classList.add('revealed');
          //   observer.unobserve(entry.target); // Optional: stop observing once revealed
        }
      });
    }, { threshold: 0.1 }); // Trigger when 10% visible

    sections.forEach(section => {
      observer.observe(section);
    });
  }

  // Requirements Carousel (Simple Click-Based Navigation)
  function setupRequirementsCarousel() {
    const carousel = document.querySelector('.requirements-carousel');
    if (!carousel) return;
    // For now, it's a scrollable flex container, no JS needed for basic functionality.
    // Future enhancements could add prev/next buttons here.
  }

  // Enhanced Target Users Tabs with Animations
  function setupTargetUsersTabs() {
    const tabContainer = document.getElementById('target-users-tab-container');
    if (!tabContainer) {
      console.log('Tab container not found');
      return;
    }

    const tabMenuItems = tabContainer.querySelectorAll('.custom-tab-menu-item');
    const tabContents = tabContainer.querySelectorAll('.custom-tab-content');

    console.log('Found menu items:', tabMenuItems.length);
    console.log('Found content items:', tabContents.length);

    // Hide all content initially except the first one
    tabContents.forEach((content, index) => {
      if (index === 0) {
        content.style.display = 'block';
        setTimeout(() => {
          content.classList.add('show');
        }, 100);
      } else {
        content.style.display = 'none';
        content.classList.remove('show');
      }
    });

    tabMenuItems.forEach((menuItem, index) => {
      menuItem.addEventListener('click', function (e) {
        e.preventDefault();

        const targetContent = this.getAttribute('data-content');
        console.log('Tab clicked:', targetContent);

        // Remove active from all menu items
        tabMenuItems.forEach(item => item.classList.remove('active'));

        // Hide all content immediately
        tabContents.forEach(content => {
          content.classList.remove('show');
          content.style.display = 'none';
        });

        // Add active to clicked item
        this.classList.add('active');

        // Find and show corresponding content
        const targetContentElement = tabContainer.querySelector(`.custom-tab-content[data-content="${targetContent}"]`);
        console.log('Target content element:', targetContentElement);

        if (targetContentElement) {
          // Show the element immediately
          targetContentElement.style.display = 'block';

          // Add animation class after a short delay
          setTimeout(() => {
            targetContentElement.classList.add('show');
            console.log('Added show class to:', targetContent);

            // Add avatar animation
            const avatar = this.querySelector('.tab-avatar');
            if (avatar) {
              avatar.style.animation = 'none';
              setTimeout(() => {
                avatar.style.animation = 'pulse-avatar 2s ease-in-out infinite';
              }, 100);
            }
          }, 50);
        } else {
          console.error('Could not find content element for:', targetContent);
        }
      });

      // Add hover effects
      menuItem.addEventListener('mouseenter', function () {
        const avatar = this.querySelector('.tab-avatar');
        if (avatar && !this.classList.contains('active')) {
          avatar.style.transform = 'scale(1.2) rotate(3deg)';
          avatar.style.transition = 'all 0.3s cubic-bezier(0.4, 0, 0.2, 1)';
        }
      });

      menuItem.addEventListener('mouseleave', function () {
        const avatar = this.querySelector('.tab-avatar');
        if (avatar && !this.classList.contains('active')) {
          avatar.style.transform = 'scale(1)';
        }
      });
    });

    console.log('Target Users Tabs initialized successfully');
  }

  // Project Design Interactive SVG
  function setupInteractiveSVG() {
    const svg = document.getElementById('project-design-svg');
    const popup = document.getElementById('component-popup');
    const popupTitle = document.getElementById('popup-title');
    const popupDescription = document.getElementById('popup-description');
    const closePopupBtn = document.getElementById('close-popup-btn');

    if (!svg || !popup || !closePopupBtn) return;

    const componentDetails = {
      esp32: "The ESP32 is a powerful microcontroller with integrated Wi-Fi and Bluetooth, serving as the brain of the system.",
      ultrasonic: "The Ultrasonic Sensor (HC-SR04) measures the distance to the water surface, determining the water level.",
      lcd: "The LCD 1602 Display shows real-time water level information and system status locally.",
      // Add more details for other components
    };

    $('map[name="systemmap"] area').on('click', function (e) {
      e.preventDefault();
      const componentKey = $(this).data('component');
      const componentTitle = $(this).attr('title') || "Component Detail";

      if (componentDetails[componentKey]) {
        popupTitle.textContent = componentTitle;
        popupDescription.textContent = componentDetails[componentKey];

        // Position popup near the clicked area (simplified)
        // For accurate positioning relative to area, more complex calculations are needed
        const svgRect = svg.getBoundingClientRect();
        const areaCoords = $(this).attr('coords').split(',').map(Number);
        const areaX = areaCoords[0];
        const areaY = areaCoords[1];

        popup.style.left = Math.min(svgRect.width - popup.offsetWidth - 10, areaX + 20) + 'px';
        popup.style.top = Math.min(svgRect.height - popup.offsetHeight - 10, areaY) + 'px';

        popup.style.display = 'block';
      }
    });

    closePopupBtn.addEventListener('click', () => {
      popup.style.display = 'none';
    });

    $(document).on('click', function (event) {
      if (!$(event.target).closest('map[name="systemmap"] area, .component-popup').length) {
        if ($(popup).is(':visible')) {
          $(popup).hide();
        }
      }
    });
  }

  // Results Bar Chart Animation
  function animateBarChart() {
    const bars = document.querySelectorAll('.animated-bar-chart .bar');
    if (!bars.length) return;
    bars.forEach(bar => {
      const targetHeight = bar.style.height; // Already set by inline style
      bar.style.height = '0%'; // Start from 0
      // Trigger reflow
      void bar.offsetWidth;
      // Animate to target height (CSS transition will handle this)
      setTimeout(() => { bar.style.height = targetHeight; }, 100);
    });
  }

  // Conclusion Accordion
  function setupConclusionAccordion() {
    if ($("#conclusionAccordion").length > 0 && widgets) {
      widgets.setCustomLegend($("#conclusionAccordion"), { animate: 200 }); // or use default
    }
  }

  // References Flip Cards
  function setupFlipCards() {
    // CSS handles the flip on hover. No JS needed unless more complex interaction is desired.
  }

  // Contact Form Animation
  function setupContactForm() {
    const form = document.getElementById('contact-form');
    const feedbackDiv = document.getElementById('form-feedback');
    if (!form || !feedbackDiv) return;

    form.addEventListener('submit', function (e) {
      e.preventDefault();
      const submitButton = form.querySelector('.submit-button');
      submitButton.classList.add('submitting');
      submitButton.disabled = true;

      // Simulate form submission
      setTimeout(() => {
        submitButton.classList.remove('submitting');
        submitButton.disabled = false;
        // Display success message (in a real app, this depends on server response)
        feedbackDiv.textContent = 'Thank you! Your message has been "sent". (Demo)';
        feedbackDiv.className = 'form-feedback success';
        feedbackDiv.style.display = 'block';
        form.reset();

        // Hide feedback after a few seconds
        setTimeout(() => { feedbackDiv.style.display = 'none'; }, 5000);

      }, 1500); // Simulate network delay
    });
  }

  // Smooth scroll for menu links (Anchor links)
  function setupSmoothScroll() {
    $(".menu-items a[href^='#'], .menu-items a[href*='#']").not('[href="#"]').not('[href="#0"]').on('click', function (event) {
      var targetPath = this.pathname;
      var currentPagePath = window.location.pathname;
      var targetHash = this.hash;

      // Check if the link is to a section on the CURRENT page
      if (targetPath === currentPagePath || targetPath === currentPagePath.substring(currentPagePath.lastIndexOf('/') + 1) || (targetPath === "" && this.hostname === window.location.hostname)) {
        if (targetHash.length) {
          event.preventDefault();
          var $targetElement = $(targetHash);
          if ($targetElement.length) {
            var menuHeight = $('.menu-container .menu.fixed-header').outerHeight() || 60;
            $('html, body').stop().animate({
              scrollTop: $targetElement.offset().top - menuHeight - 10 // Extra 10px offset
            }, 800, 'easeInOutExpo', function () {
              // Optional: Update URL hash without jumping
              // window.location.hash = targetHash;
            });
          }
        }
      }
      // If it's a link to another page with a hash, the browser will handle the jump after page load.
      // A separate function on page load could scroll to hash if present.
    });

    // Scroll to hash if present on page load (after menu is fixed)
    if (window.location.hash) {
      var hash = window.location.hash;
      var $targetElementOnLoad = $(hash);
      if ($targetElementOnLoad.length) {
        setTimeout(function () { // Ensure fixed menu height is calculated
          var menuHeight = $('.menu-container .menu.fixed-header').outerHeight() || 60;
          $('html, body').stop().animate({
            scrollTop: $targetElementOnLoad.offset().top - menuHeight - 10
          }, 800, 'easeInOutExpo');
        }, 100); // Small delay
      }
    }
  }


  function init() {
    if (typeof edaplotjs !== 'undefined' && typeof edaplotjs.Widgets !== 'undefined') {
      widgets = new edaplotjs.Widgets();
    } else {
      console.warn("edaplotjs.Widgets not found. Some interactive elements might not work.");
      // Provide a dummy widgets object if necessary to prevent errors,
      // or ensure that parts of the code relying on it are conditionally executed.
      widgets = {
        createCustomTab: function (settings) { console.warn("Dummy createCustomTab called for: ", settings.selector); },
        setCustomLegend: function (settings) { console.warn("Dummy setCustomLegend called for: ", settings.selector); },
        // Add other methods if needed by the template parts you are still using.
      };
    }

    setupScrollAnimations();
    setupRequirementsCarousel(); // Currently CSS-driven scroll
    setupTargetUsersTabs();
    setupInteractiveSVG();
    animateBarChart(); // Call on page load or when section is visible
    setupConclusionAccordion();
    // setupFlipCards(); // Currently CSS-driven hover
    setupContactForm();
    setupSmoothScroll();

    // Specific initializations for pages if needed
    if (document.body.classList.contains('overview-page')) {
      // Overview page specific JS
      $('.overview-banner').addClass('ken-burns-effect');
    }
    if (document.body.classList.contains('system-insights-page')) {
      // System Insights page specific JS
    }
    if (document.body.classList.contains('future-innovations-page')) {
      // Future Innovations page specific JS
    }
  }

  $(document).ready(function () {
    init(); // Initialize your custom code

    // [REINVENTION] Scroll reveal animation for the future-innovations page
    if ($('body.future-innovations-page').length) {
      const revealElements = document.querySelectorAll('.scroll-reveal');

      const revealObserver = new IntersectionObserver((entries) => {
        entries.forEach(entry => {
          if (entry.isIntersecting) {
            entry.target.classList.add('visible');
            // Optional: stop observing once it's visible so it doesn't re-animate
            revealObserver.unobserve(entry.target);
          }
        });
      }, {
        threshold: 0.1 // Trigger when 10% of the element is visible
      });

      revealElements.forEach(el => {
        revealObserver.observe(el);
      });
    }

    // [REINVENTION] Smooth scroll for the scroll-down prompt
    $('.scroll-down-prompt').on('click', function (e) {
      e.preventDefault();
      const targetId = $(this).attr('href');
      const targetElement = $(targetId);
      if (targetElement.length) {
        $('html, body').animate({
          scrollTop: targetElement.offset().top - 60 // Adjust 60 for your fixed menu height
        }, 800);
      }
    });

  });
})();
