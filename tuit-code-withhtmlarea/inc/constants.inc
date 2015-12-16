<?

define('STUDENT',0);
define('TEACHER',1);
define('CONT_ADMIN',2);
define('ACCT_ADMIN',4);
define('SUPER_ADMIN',8);
define('VISITOR',16);
define('VIS_TEACHER',32);
define('VIS_CONT_ADMIN',64);
$perm_strs = array(STUDENT=>'Student',TEACHER=>'Teacher',CONT_ADMIN=>'Contents administrator',ACCT_ADMIN=>'Accounts administrator',SUPER_ADMIN=>'Super administrator',VISITOR=>'Visitor');

/*
   the same for exercise difficulty
*/

define('ORIENTATION',1);
define('ELEMENTARY',2);
define('BEGINNER',3);
define('INTERMEDIATE',4);
define('ADVANCED',5);

$diff_strs = array(ORIENTATION=>'Orientation',ELEMENTARY=>'Elementary',BEGINNER=>'Beginner',INTERMEDIATE=>'Intermediate',ADVANCED=>'Advanced');
ksort($diff_strs);

define('QANDA',5);
define('CHOOSE',2);
define('MATCH',4);
define('COMPLETE',1);
define('CROSSWORD',3);
define('EXPLANATION',6);

$ex_strs = array(QANDA=>'Questions and answers',CHOOSE=>'Choosing alternatives',CROSSWORD=>'Crossword puzzles',MATCH=>'Matching sentences',COMPLETE=>'Universal exercise (Cloze, tables, etc.)',EXPLANATION=>'Information/Content Presentation (Explanations, hints, etc.)');

#define('N_ASSIGNMENT',1);
define('N_EXERCISE',2);
define('N_MISCELLANEOUS',3);

?>
