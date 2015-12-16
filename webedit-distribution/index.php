<?

error_reporting(0);

$inc = 'include.inc';
$mod = 'modules.inc';

$MODULES['argv'] = array_merge($_GET,$_POST);
$m = $MODULES['argv']['m'];

include("$inc/database.inc.php");
include("$inc/configuration.inc.php");

if(!$m or !ereg('^[[:alnum:]_]+$',$m))
  $MODULES['argv']['m'] = $m = 'catalog';

if(!empty($GLOBALS['section']) or !empty($GLOBALS['exercise']))
  $m = 'tuit';

include("$mod/$m.inc.php");

include("$inc/header.inc.php");
include("$inc/layout.inc.php");
include("$inc/footer.inc.php");

?>
