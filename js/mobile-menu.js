/*************************************************************************
 * Mobile Menu Functionality
 * This script handles mobile menu toggling and dropdown functionality
 *************************************************************************/

(function () {
    "use strict";

    // Wait for DOM to load and menu to be loaded
    function initMobileMenu() {
        const menuButton = document.querySelector('.menu-button');
        const menuItems = document.querySelector('.menu-items');
        const dropdowns = document.querySelectorAll('.dropdown');

        // Toggle mobile menu
        if (menuButton && menuItems) {
            menuButton.addEventListener('click', function (e) {
                e.preventDefault();
                e.stopPropagation();

                // Close any open dropdowns first
                dropdowns.forEach(dropdown => {
                    dropdown.classList.remove('active');
                });

                menuItems.classList.toggle('show');

                // Add animation class
                if (menuItems.classList.contains('show')) {
                    menuItems.style.display = 'flex';
                    // Force reflow
                    menuItems.offsetHeight;
                } else {
                    setTimeout(() => {
                        if (!menuItems.classList.contains('show')) {
                            menuItems.style.display = 'none';
                        }
                    }, 300);
                }
            });
        }

        // Enhanced dropdown functionality for both desktop and mobile
        dropdowns.forEach(dropdown => {
            const dropbtn = dropdown.querySelector('.dropbtn');
            const dropdownContent = dropdown.querySelector('.dropdown-content');

            if (dropbtn && dropdownContent) {
                // Desktop hover behavior
                if (window.innerWidth > 768) {
                    dropdown.addEventListener('mouseenter', function () {
                        dropdown.classList.add('active');
                        dropdownContent.style.display = 'block';
                    });

                    dropdown.addEventListener('mouseleave', function () {
                        dropdown.classList.remove('active');
                        dropdownContent.style.display = 'none';
                    });

                    // Also handle click for desktop
                    dropbtn.addEventListener('click', function (e) {
                        e.preventDefault();
                        e.stopPropagation();

                        // Close other dropdowns
                        dropdowns.forEach(otherDropdown => {
                            if (otherDropdown !== dropdown) {
                                otherDropdown.classList.remove('active');
                                const otherContent = otherDropdown.querySelector('.dropdown-content');
                                if (otherContent) {
                                    otherContent.style.display = 'none';
                                }
                            }
                        });

                        // Toggle current dropdown
                        dropdown.classList.toggle('active');
                        if (dropdown.classList.contains('active')) {
                            dropdownContent.style.display = 'block';
                        } else {
                            dropdownContent.style.display = 'none';
                        }
                    });
                } else {
                    // Mobile dropdown behavior
                    dropbtn.addEventListener('click', function (e) {
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

        // Close mobile menu when clicking outside
        document.addEventListener('click', function (e) {
            if (menuItems && !menuItems.contains(e.target) && !menuButton.contains(e.target)) {
                menuItems.classList.remove('show');
                setTimeout(() => {
                    if (!menuItems.classList.contains('show')) {
                        menuItems.style.display = 'none';
                    }
                }, 300);

                // Close all dropdowns
                dropdowns.forEach(dropdown => {
                    dropdown.classList.remove('active');
                });
            }
        });

        // Handle window resize
        window.addEventListener('resize', function () {
            if (window.innerWidth > 768) {
                if (menuItems) {
                    menuItems.classList.remove('show');
                    menuItems.style.display = '';
                }
                // Reset dropdown styles for desktop
                dropdowns.forEach(dropdown => {
                    dropdown.classList.remove('active');
                    const dropdownContent = dropdown.querySelector('.dropdown-content');
                    if (dropdownContent) {
                        dropdownContent.style.display = 'none';
                    }
                });
            }
        });
    }

    // Initialize when DOM is ready
    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', function () {
            // Wait a bit for menu.html to load
            setTimeout(initMobileMenu, 100);
        });
    } else {
        // DOM is already ready
        setTimeout(initMobileMenu, 100);
    }

    // Also initialize after a longer delay to ensure menu is loaded
    setTimeout(initMobileMenu, 500);
})();
