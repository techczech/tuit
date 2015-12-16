<?
$db_server   = 'localhost';
$db_user     = 'yourDBuser';
$db_password = 'yourDBpassword';
$db_name     = 'yourDBname';

mysql_connect($db_server,$db_user,$db_password);
mysql_select_db($db_name);

include("$inc_path/constants.inc");
?>
