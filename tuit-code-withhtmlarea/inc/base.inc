<?

include('inc/mi_functions.inc');

/* 
   this include adds basic functions that don't fit in any class..
   some of them are remnants of non-OOP-period in this project
   (error_f and error_fl for example) and should be revised..
*/

$res = mysql_query("select * from configuration");
while($row = mysql_fetch_array($res))
  $GLOBALS[$row['conf_key']] = $row['conf_value'];

function get_options($str)
{
  $opts = array();
  $tok = strtok($str,"\n");
  while($tok)
  {
    list($l,$r) = explode(':',$tok);
    $opts[$l] = $r;
    $tok = strtok("\n");
  }
  return $opts;
}

function check_permissions($perm,$value)
{
  return ($perm & $value)==$value;
}

function error_f($err)
{
  global $page;
  $page->set_top('Error online');
  $page->insert_to_middle("
<center><strong>Error: </strong>
<font color=\"#FF0000\">$err</font></center><br><br>\n");
}

function error_fl($err)
{
  global $page;
  error_f($err);
  $page->add_to_middle("<center><small>
Should the problem persist, please contact <a href=\"mailto:$GLOBALS[admin_email]\">the
administrator</a>.
</small></center><br><br>\n");
}

function is_super_admin($ad)
{
  return check_permissions($ad,SUPER_ADMIN);
}

function is_acct_admin($ad)
{
  if(check_permissions($ad,ACCT_ADMIN))
    return 1;
  else
    return 0;
}

function am_i_acct_admin()
{
  global $udata,$page;
  if(!is_acct_admin($udata['admin']) and !is_super_admin($udata['admin']))
  {
    error_f('Unauthorised access! This section is only available for accounts administrators.');
    include('main.inc');
    return 0;
  }
  else
    return 1;
}

function is_teacher($ad)
{
  if(check_permissions($ad,TEACHER))
    return 1;
  else
    return 0;
}

function am_i_teacher()
{
  global $udata,$page;
  if(!is_teacher($udata['admin']))
  {
    error_f('Unauthorised access! This section is only available for teachers.');
    include('main.inc');
    return 0;
  }
  else
    return 1;
}

function is_cont_admin($ad)
{
  if(check_permissions($ad,CONT_ADMIN))
    return 1;
  else
    return 0;
}

function am_i_cont_admin()
{
  global $udata,$page;
  if(!is_cont_admin($udata['admin']) and !is_super_admin($udata['admin']))
  {
    error_f('Unauthorised access! This section is only available for contents administrators.');
    include('main.inc');
    return 0;
  }
  else
    return 1;
}

function is_user($login)
{
  @$res = mysql_query("select id from users where login='$login'");
  @$row = mysql_fetch_array($res);
  if(!$row)
    return 0;
  else
    return $row['id'];
}

function send_email($to,$subject,$text)
{
  mail($to,$subject,$text,"From: $GLOBALS[emails_from]\n");
}

?>
