<?

define('LOCAL_INCLUDE_PATH',"$inc/ClasSQL");
require ("$inc/ClasSQL/ClasSQL.inc.php");
$db_host = 'localhost';
$db_name = 'yourDBname';
$db_user = 'yourDBuser';
$db_pwd  = 'yourDBpassword';

$SQL = &newSQL('mysql');
$SQL->connect($db_host,$db_user,$db_pwd);
$SQL->selectDB($db_name);

?>
