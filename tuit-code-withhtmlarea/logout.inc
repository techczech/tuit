<?

/*
   each time user log out using the 'Logout' item in the menu,
   cookie is set to contain '***' (tristar) string in the browser
   and in the database as well..
*/

include('inc/forms.inc');
setcookie('bisquit','***',0,'/');
@mysql_query("update users set cookie='***' where login='$udata[login]'");

$page->set_logged(0);
$page->set_top('Logged out');
$page->add_to_middle('<center>You have just logged out.<br><br>'.login_form()."</center>\n");

return 123;
?>
