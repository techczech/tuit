TuiT 
(stands for Teaching using internet Technologies)

ABOUT
TuiT is the creation of Dominik Lukes and Michal Belda distributed under the GPL licence understood as following the share alike principle. Please contact Dominik Lukes for details at me@dominiklukes.net.

TUIT HISTORY AND PHILOSOPHY
TuiT was developed to support the online course on Bohemica.com. Bohemica.com is a comprehensive resource for learners of Czech language and culture and TuiT was used to encode over 600 hundred exercises (accessible for free on http://www.bohemica.com/publicaccess) and to serve as a way of communication between teachers and students. Although it was developed for a Czech course, it can be used for any language or even for teaching other subjects. TuiT was originally developed in Perl and in 2001 rewritten in PHP 3. TuiT was used for teaching Czech at Jerome of Prague College (http://www.chp.cz) and at the University of Glasgow in 2002-2003. TuiT's development slowed down to a trickle around 2004 when a sponsorship deal fell through (until then Dominik Lukes sponsored part of the development out of private funds and a lot of time was donated by Michal Belda who did all the programming). Developers for further development are sought. The current plan is to convert TuiT into a Drupal module corresponding with the move of Bohemica.com to Drupal.

TuiT's philosophy is that learning happens primarily during interactions between students and teachers. The role of teachnology should be to provide a channel for this interaction rather than replace it (or even supplement it). Most other tools (e.g. Hot Potatoes) and online services (e.g. http://www.rosettastone.com) focus on providing the student with self-study tools. The problem with these is that if the student is motivated enough to use them on their own, they will learn the language using pretty much anything. Similar to the other tools, TuiT helps teachers create execises using guided forms or a simple mark-up language (HTML can provide the rich multimedia part) but the exercises can also be combined into units, assigned to students and corrected.

KEY FEATURES
* Creation of exercises using a simple markup language
* Creation of exercises using guided forms (multiple choice, question and answer, matching sentences)
* Creation of simple crossword puzzles
* Combining exercises into units
* Categorizing exercises
* Linking exercises to grammar explanations using categories
* Maintaining a dictionary (each students can set up simple vocabularies for practice)
* Personal notebook for teachers and students
* Discussion forums
* Assingment of exercises to students or groups of students
* Private achievement portfolio maintained by students
* Different levels of users
* Correction of exercises (including comments)
* Integration into a simple CMS (see Bohemica.com)

* Mark-up language features (see manual/tagdescriptions.htm for details)
- fill in the blank (cloze)
- multiple choice (one or multiple options using checkboxes or radio buttons)
- multiple choice through a pull-down menu
- essay-type questions (it is possible to enter corrections directly into essays - this feature is not yet fully implemented)
- pre-correcting items and teacher corrected items in a single exercises
- pop-up hints
- pre-filled form fields (for sample items)
- linking individual words to items in a dictionary
- assigning test values to all items in the exercises
- javascript self-correcting exercises
- redirecting hints into a separate textarea

PLANNED or PARTIALLY IMPLEMENTED FEATURES
* combining of students into classes that can be easily managed
* book-like display of units
* ability of teachers to mark up student essays directly in the text
* convert language markup into XML compatible with mark up test standards
* conversion script to distribute exercises on a CD-ROM
* better management of notes and private vocabularies by students

DREAM FEATURES
* improve usability through AJAX
* automatic creation of JavaScript language games
* recording of sounds directly into exercises by teachers or by students when they submit exercises (Odeo style)
* provide plugin for textarea HTML editor to edit mark up language
* implement Web2.0 social features (such as a social dictionary, sharing of exercises by students, creation of exercises by students for other students, buddy lists, personal private/public portfolios My-Space-style, word blogs, sharing of learning strategies by students, etc.)

INSTRUCTIONS FOR USE
Please refer to the files in the 'manual' folder.

SYSTEM REQUIREMENTS
TuiT works under MySQL 3, 4, 5 and PHP 3 to 4 (not tested under PHP 5, but should work)

With PHP version 4 and up, you must set PHP to the following values (either in .htaccess or php.ini):
php_flag magic_quotes_gpc off
php_flag register_globals on

INSTALL
1. Set up your MySQL database and import the tuit-database-structure.sql file. (You can also use the tuit-sample-Czech-data-NOTfree.sql.gz file but remember that the data is not free to use - this sample version is also miscoded - by accident).
1a. Use PhpMyAdmin to navigate to the 'users' table to set up a user. (Use the following SQL to set a password for the user: update users set passwd=password('newpasswd') where login='userid'; - this may vary depending on your version of MySQL)
2. Copy all the files in tuit-code-withhtmlarea into an appropriate folder on your server (it is possible to extract HTMLarea but no separate code exists at the moment)
3. Edit inc/pre.inc file with your database data

You're ready to go. Just point your browser to the TuiT directory on your server.

CONFIGURATION
* You can change the site name, page title and main site links in the 'configuration' table in your database (some of these settings only have an effect you if you're also using the WebEdit CMS. 
* You can change the site's colors in lookandfeel-ie.css and lookandfeel-mozilla.css
* To change the site's title, edit index.php on line 24 $page = new PAGE('TuiT Demo');
(You may also have to make the same change in edit public_access.php and specdict-indep.php)
* To change the login/intro page, edit the login.inc file (line 42 and following) - any other 'interface' changes have to be made through other .inc files.
