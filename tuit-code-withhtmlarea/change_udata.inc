<?

/*
   script for changing users own settings
   finished, but a little testing still needed
*/

include('inc/forms.inc');

$err='';
$page->set_top('Change user settings');

if($change_udata)
{
  if(empty($change_name))
    $change_name = $udata['name'];
  if(empty($change_email))
    $change_email = $udata['email'];
  if($new_passwd)
  {
    if($new_passwd!=$new_passwd2)
      $err = 'New passwords do not match!';
    elseif(ereg('[\'"\\]',$curr_passwd) or ereg('[\'"\\]',$new_passwd))
      $err = 'Invalid characters in password!';
    else
    {
      @$res = mysql_query("select id from users where passwd=password('$curr_passwd')");
      @$row = mysql_fetch_array($res);
      if($row['id']!=$udata['id'])
        $err = 'Bad password!';

      $query = "update users set passwd=password('$new_passwd'), name='$change_name', email='$change_email', send_note='$send_note' where id=$udata[id] and passwd=password('$curr_passwd')";
    }
  }
  else
    $query="update users set name='$change_name', email='$change_email', send_note='$send_note' where id=$udata[id]";

  if(!$err)
  {
    if(!(@$res = mysql_query($query)))
      error_fl('Error updating settings!');

    @$res = mysql_query("select id,login,name,email,send_note,admin from users where id=$udata[id]");
    @$udata = mysql_fetch_array($res);
  }

  if(is_acct_admin($udata['admin']) and !$err)
  {
    @mysql_query("update users set default_teacher=0 where default_teacher=$udata[id]");
    $sql = "update users set default_teacher=$udata[id] where id in(";

    $cnt = 0;
    while(list($k,$v) = each($default_teacher))
    {
      if($cnt > 0)
        $sql .= ',';

      $sql .= $v;
      $cnt++;
    }
    $sql .= ')';

    @$res = mysql_query($sql);
  }
}

if($err)
  error_f("<strong><font color=\"#FF0000\">$err</font></strong><br><br>\n");
$page->add_to_middle(change_udata_form());

return 123;
?>
