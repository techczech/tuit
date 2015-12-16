<?

/*
   if this system shall work, users must have browser that allows cookies
   which are sent and also saved into the database and compared each time
   cookie is received.. the cookie is baked as md5 sum of 'login-client_IP'
   string..
*/

$page->set_logged(0);
$page->set_top('Login form');

if( !empty($PHP_AUTH_USER) and !empty($PHP_AUTH_PW) )
{
  @$res = mysql_query("select login, name, admin, date_format(last_login,'%e. %c. %Y, %k:%i') as last_seen,to_days(expiry) as d_expiry, to_days(now()) as d_now from users where login regexp '".addslashes($PHP_AUTH_USER)."' and passwd=password('".addslashes($PHP_AUTH_PW)."')");
  if( mysql_num_rows($res) == 1 )
  {
    $udata = mysql_fetch_array($res);
    if(!$udata["d_expiry"] or $udata["d_expiry"] > $udata["d_now"])
    {
      if(!$section or !ereg('^[[:alnum:]_]+$',$section))
        $section='main';
      $page->set_logged(1);
      @$res = mysql_query("update users set ip='$REMOTE_ADDR', cookie=md5('$udata[login]-$REMOTE_ADDR'), logins=logins+1,last_login=now() where login='$udata[login]'");
      $page->set_top("Welcome $udata[name], this is the main menu");
      include("$section.inc");
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
  Header( "WWW-Authenticate: Basic realm=\"Czech Online Autentication\"");
  Header( "HTTP/1.0 401 Unauthorized");
  echo("This section is available to registered users only.");
}

return 123;
?>
