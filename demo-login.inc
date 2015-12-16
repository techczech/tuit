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
  @$res = mysql_query("select login, name, admin, date_format(last_login,'%e. %c. %Y, %k:%i') as last_seen,to_days(expiry) as d_expiry, to_days(now()) as d_now from users where login regexp '".addslashes($form_login)."' and passwd=password('".addslashes($form_passwd)."')");
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

 $page->add_to_middle("
 <table>
 <tr>
 <td valign=\"top\">
 <p>Welcome to the demo of TuiT (Teaching using internet Technologies). <br />
You can login as a teacher using <b>login:</b> demoteacher <b>password:</b> demo or a student using <b>login:</b> demostudent <b>password:</b> demo. More details on use in <a href=\"manual/index.htm\">http://tuit.glottalstart.com/manual</a>. Feel free to experiment, delete and create. TuiT is used to power parts of <a href=\"http://www.bohemica.com\">Bohemica.com</a>. You can listen to <a href=\"http://www.dominiklukes.net/files/audio/brussels06/BarCampBrussels06-LanguageLearningCommunitiesOnline.mp3\">a presentation of TuiT at Barcamp Brussels</a> by <a href=\"http://www.dominiklukes.net\">Dominik Lukeš</a> - a <a href=\"http://video.google.co.uk/videoplay?docid=-7345310516516023872&hl=en-GB\">screencast is available on Google video</a>.<p>
If you'd like to try TuiT on your system (PHP/MySQL), you can <a href=tuit.distribution.zip>download it here</a> as a zipped file. TuiT is GPL - more developers are sought. Further details in the <a href=\"README&INSTALL.TXT\">README&INSTALL.TXT</a> file. (For those interested, the accompanying <a href=\"webedit-distribution.zip\">WebEDIT CMS</a> is also available.)
</p>

<embed style=\"width:400px; height:326px;\" id=\"VideoPlayback\" type=\"application/x-shockwave-flash\" src=\"http://video.google.com/googleplayer.swf?docId=-7345310516516023872&hl=en-GB\" flashvars=\"\"> </embed>

 </td>
 <td valign=\"top\">
  <center><p><b>Standard intro text:</b> This section is available to registered users only. If you are not a registered
student, you can fill in <a href=\"?section=acct_request\">this form</a>
to apply for a trial registration.<br>
(Note: your browser must allow cookies for the course to work.)<br><br>".
    login_form()."\n</center>\n</p>
 </td>
 </tr></table>
"); 
}

return 123;
?>
