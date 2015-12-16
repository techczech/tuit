WebEDIT

ABOUT
WebEDIT is the creation of Dominik Lukes and Michal Belda distributed under the GPL licence understood as following the share alike principle. Please contact Dominik Lukes for details at me@dominiklukes.net.

WebEDIT is a very simple section-based (as opposed to category-based) Content Management System which was designed specifically to support Bohemica.com. It was written in 2002 and further development was halted in 2003. It is a very straightforward and simple system that may not meet the needs of more complex multi-user sites but could still be useful for simple sites with basic hierarchies. It's main shortcoming is that many of its features are hardcoded and cannot be customized through a user interface.

FEATURES
- sections
- articles
- boxes (small and full page)
- links
- side boxes
- automatic side navigation
- ordering of sections
- integration of language exercises and units from TuiT

SYSTEM REQUITEMENTS
PHP 4.2 and later
MySQL 4 and 5

INSTALL AND SETUP
1. Copy all files into a directory on your server and load configuration.sql into your database
2. Enter your database login and password into include.inc/database.inc.php
3. Run index.php
4. To edit run adminht/index.php - you must use .htaccess
5. You have to edit the interface and look and feel in individual .php and .css files (it is not hard to figure out which - simply search for the text you want to modify) - much of the information describes Bohemica.com
6. If you want to integrate language exercises place the 'tuit' directory in both the main and the admin directories (alternatively, you can set up a symlink in the adminht directory)

