<?

/*
   if this system shall work, users must have browser that allows cookies
   which are sent and also saved into the database and compared each time
   cookie is received.. the cookie is baked as md5 sum of 'login-client_IP'
   string..
*/

$page->set_logged(0);
$page->set_top('Login form');

if( !empty($form_login) and !empty($form_passwd) )
{
  @$res = mysql_query("select login, name, admin, date_format(last_login,'%e. %c. %Y, %k:%i') as last_seen,to_days(expiry) as d_expiry, to_days(now()) as d_now from users where login = '".addslashes($form_login)."' and passwd=password('".addslashes($form_passwd)."')");
  if( mysql_num_rows($res) == 1 )
  {
    $udata = mysql_fetch_array($res);
    if(!$udata["d_expiry"] or $udata["d_expiry"] > $udata["d_now"])
    {
      $page->set_logged(1);
      @$res = mysql_query("update users set ip='$REMOTE_ADDR', cookie=md5('$udata[login]-$REMOTE_ADDR'), logins=logins+1,last_login=now() where login='$udata[login]'");
      setcookie('bisquit',md5("$udata[login]-$REMOTE_ADDR"),/*time()+86400*/0,"/");
      $page->set_top("Welcome $udata[name], this is the main menu");
      include('main.inc');
    }
    else
    {
      $page->set_top('Account expired');
      $page->add_to_middle('<center><font color="#FF0000">Sorry, your account has expired.</font></center><br><br>');
    }
  }
  else
    error_f('Bad password or userid!');
    
}

if(!$page->is_logged())
{
  include('inc/forms.inc');

  $page->add_to_middle("<center>
This section is available to registered users only. If you are not a registered
student, you can fill in <a href=\"?section=acct_request\">this form</a>
to apply for a trial registration.<br>
(Note: your browser must allow cookies for the course to work.)<br><br>".
    login_form()."\n</center>\n");
}

return 123;
?>
