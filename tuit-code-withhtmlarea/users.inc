<?

/*
   accounts processing script..
*/

#error_reporting(2047);

if(!am_i_acct_admin())
  return 123;

# by default show 20 first users sorted alphabeticaly ascending..

if(!is_numeric($limit) or $limit<0) $limit = 20;
if(!is_numeric($offset) or $offset<0) $offset = 0;

if($desc == 1 or $desc == 'desc') $desc = 'desc';
else $desc = '';

@$res = mysql_query("select count(*) as cnt from users");
@$row = mysql_fetch_array($res);
$users_count = $row['cnt'];

if(isset($add_account))
{
  if(!$add_login or !$add_passwd or !$add_passwd2 or !$add_name or !$add_email)
    $err = 'Incomplete input!';
  elseif($add_passwd != $add_passwd2)
    $err = 'Passwords do not match!';
  elseif(!ereg('^[[:alnum:]_]+$',$add_login) or ereg('[\'"\\]',$add_passwd))
    $err = 'Invalid characters in login or password!';
  elseif(is_user($add_login))
    $err = 'The desired userid is already taken. Please try another one.';
  elseif($add_expiry and !ereg('^([0-9]{1,2})\.[[:space:]]?([0-9]{1,2})\.[[:space:]]?([0-9]{4})$',$add_expiry,$regs))
    $err = 'Bad format of expiry date!';
  else
  {
    if($regs)
      $expiry = "'$regs[3]-$regs[2]-$regs[1]'";
    else
      $expiry = 'NULL';

    $ad = 0 + $add_teacher | $add_acct_admin | $add_cont_admin | $add_super_admin | $add_visitor;

    @$res2 = mysql_query("insert into users set login='$add_login',passwd=password('$add_passwd'),name='".addslashes($add_name)."', email='".addslashes($add_email)."', expiry=$expiry, admin='$ad'");
    if(!$res2)
      $err = "Creation of user account '$add_login' failed!";
    else
    {
      $page->add_to_middle("<center><h3>Account '$add_login' successfully added.</h3></center><br><br>\n");
      $add_login='';
      $add_name='';
      $add_email='';
      $add_expiry='';
      $add_teacher='';
      $add_acct_admin='';
      $add_cont_admin='';
    }
  }
}
elseif(isset($change_user) and is_numeric($user))
{
  $user = (int) $user;
  if(ereg('^([0-9]{1,2})\.[[:space:]]?([0-9]{1,2})\.[[:space:]]?([0-9]{4})$',$change_expiry,$regs))
    $expiry = "expiry='$regs[3]-$regs[2]-$regs[1]'";

  if(isset($delete_expiry))
    $expiry = "expiry=NULL";

  $perms = 0;
  if(is_array($change_permissions))
    while(list($k,$v) = each($change_permissions))
      $perms |= $v;

  $admin = "admin=$perms";
  if(@mysql_query("update users set ".($expiry?"$expiry, ":'').($admin?"$admin, ":'')."default_teacher=".($default_teacher?$default_teacher:'NULL')." where id=$user"))
    $page->add_to_middle('<center><h3>User settings saved.</h3></center>');
  else
    $err='Cannot save new settings!';

  $q1 = 'delete from user_categories where uc_u_id='.$user;
  $q2 = 'insert into user_categories values';
  reset($categories);
  $ls = '';
  while(list($k,$v) = each($categories))
    $ls .= ($ls?',':'')." ('".(int)$v."',$user)";

  $q2 .= $ls;

  if(@mysql_query($q1))
    @mysql_query($q2);
}
elseif(isset($delete) or $users_action=='delete')
{
  if(!is_array($users_list) and $user!=$udata['id'])
  {
    $users_list[]=$user;
    $user = '';
  }

  $sql = 'delete from users where id in (';
  $cnt = 0;
  while(list($k,$v) = each($users_list))
    if($v != $udata['id'])
      $sql .= ($cnt++?',':'').$v;
  $sql .= ')';

  if(@mysql_query($sql))
    $page->add_to_middle('<center><h3>Account(s) deleted.</h3></center>');
  else
    $err='Cannot delete account(s)!';
}
elseif($users_action=='expire' and is_array($users_list))
{
  $sql = 'update users set expiry=now() where id in (';
  $cnt = 0;
  while(list($k,$v) = each($users_list))
    $sql .= ($cnt++?',':'').$v;
  $sql .= ')';
  if(@mysql_query($sql))
    $page->add_to_middle('<center><h3>Expiry set on today for selected account(s).</h3></center>');
  else
    $err='Cannot set expiry for selected account(s)!';
}
elseif($users_action=='prolong')
{
  $page->set_top('Prolong account(s)');
  if(is_array($expiry_list))
  {
    while(list($k,$v) = each($expiry_list))
    {
      $expiry = '';
      if(ereg('^([0-9]{1,2})\.[[:space:]]?([0-9]{1,2})\.[[:space:]]?([0-9]{4})$',$v,$regs) and is_numeric($k))
        $expiry = "'$regs[3]-$regs[2]-$regs[1]'";
      if($delete_expiry[$k])
        $expiry = 'NULL';

      $expiry and @mysql_query("update users set expiry=$expiry where id=$k");
    }
  }
  elseif(is_array($users_list))
  {
    $btable = new BOXEDTABLE;
    $btable->set_parameters(array('style'=>'subtable','cellspacing'=>5,'cellpadding'=>2));
    $btable->add_row(array('<strong>Name</strong> (userid)','Expiry (DD. MM. YYYY)'));

    $sql = "select id,login,name,date_format(expiry,'%e. %c. %Y') as expiry from users where id in(";
    $cnt = 0;
    while(list($k,$v) = each($users_list))
      $sql .= ($cnt++?',':'').$v;
    $sql .= ') order by name';
    if(@$res=mysql_query($sql) and @mysql_num_rows($res))
      while(@$row = mysql_fetch_array($res))
        $btable->add_row(array("<a href=\"?section=users&user=$row[id]\"><strong>$row[name]</strong> ($row[login])</a>","<input type=\"text\" name=\"expiry_list[$row[id]]\" size=12 maxlength=12 value=\"$row[expiry]\" class=\"boxedfield\">".
        ($row['expiry']?"<input type=\"checkbox\" name=\"delete_expiry[$row[id]]\" value=1 class=\"checkbox\"><small>Delete expiry</small>":'')));

    $btable->add_row(array('&nbsp;','<input type="submit" value="   Save changes   " class="buttoninbox">'));
    $page->add_to_middle("<form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"users\">
    <input type=\"hidden\" name=\"users_action\" value=\"prolong\">
    <center>
    <h3>Set expiry date(s)</h3>".
    $btable->prepare().
    "
    </center>
    </form>\n");
    return 123;
  }
}
elseif($users_action=='set_teacher')
{
  $page->set_top('Set default teacher');
  if(is_array($teachers_list))
  {
    while(list($k,$v) = each($teachers_list))
    {
      if(is_numeric($k) and is_numeric($v))
        @mysql_query("update users set default_teacher=$v where id=$k");
    }
  }
  elseif(is_array($users_list))
  {
    $btable = new BOXEDTABLE;
    $btable->set_parameters(array('style'=>'subtable','cellspacing'=>5,'cellpadding'=>2));
    $btable->add_row(array('<strong>Name</strong> (userid)','Default teacher'));
    $sbox = new SELECTBOX;
    $sbox->set_style('selectbox');
    if(@$res = mysql_query('select id,login,name from users where (admin & '.TEACHER.')='.TEACHER))
    {
      $cnt = 0;
      while(@$row = mysql_fetch_array($res) and @$num=mysql_num_rows($res))
      {
        $sbox->add_item("$row[name] ($row[login])",$row['id']);
        $teachers[$row['id']]=$cnt;
        $cnt++;
      }
    }

    $sbox->set_size($num>3?'3':$num);
    
    $sql = 'select id,login,name,default_teacher from users where id in(';
    $cnt = 0;
    while(list($k,$v) = each($users_list))
      $sql .= ($cnt++?',':'').$v;
    $sql .= ') order by name';
    if(@$res = mysql_query($sql) and @mysql_num_rows($res))
      while(@$row = mysql_fetch_array($res))
      {
        $sbox->set_name("teachers_list[$row[id]]");
        $sbox->reset_selected();
        if($row['default_teacher'])
          $sbox->set_selected(array($teachers[$row['default_teacher']]));
        $btable->add_row(array("<strong>$row[name]</strong> ($row[login])",
          $sbox->prepare()));
      }

    $btable->add_row(array('&nbsp;','<input type="submit" value="   Save changes   " class="buttoninbox">'));
    $page->add_to_middle("<form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"users\">
    <input type=\"hidden\" name=\"users_action\" value=\"set_teacher\">
    <center>
    <h3>Set default teacher</h3>".
    $btable->prepare().
    "
    </center>
    </form>\n");
    return 123;
  }
}
       
if(is_numeric($user))
{
  if($user == $udata['id'])
  {
    include('change_udata.inc');
    return 123;
  }
  @$res = mysql_query("select id,login,name,email,admin,date_format(last_login,'%e. %c. %Y, %k:%i') as last_seen,date_format(expiry,'%e. %c. %Y') as expiry, default_teacher,logins from users where id=$user");

  if(@$row = mysql_fetch_array($res))
  {
    $sbox = new SELECTBOX;
    $sbox->set_multiple(1);
    $sbox->set_size(count($perm_strs));
    $sbox->set_name('change_permissions[]');
    $sbox->set_style('selectbox');
    while(list($k,$v) = each($perm_strs))
      $sbox->add_item($v,$k);
    $st = '';
    if(!$row['admin'])
      $sbox->set_selected(array(0));
    else
    {
      if(is_teacher($row['admin']))
        $sbox->set_selected(array(1));
      if(is_cont_admin($row['admin']))
        $sbox->set_selected(array(2));
      if(is_acct_admin($row['admin']))
        $sbox->set_selected(array(3));
      if(is_super_admin($row['admin']))
        $sbox->set_selected(array(4));
    }

    $toptable = new BOXEDTABLE;
    $toptable->set_parameters(array('style'=>'subtable','cellspacing'=>2));

    $toptable->add_row(array('Userid',$row['login']));
    $toptable->add_row(array('Name',$row['name']));
    $toptable->add_row(array('Permissions',$sbox->prepare()));
    $toptable->add_row(array('Last seen',ereg('[1-9]',$row['last_seen'])?$row['last_seen']:'Have not logged in yet'));
    $row['logins'] and $toptable->add_row(array('Logins',$row['logins']));
    $toptable->add_row(array('Expiry (DD. MM. YYYY)',"<input type=\"text\" name=\"change_expiry\" size=12 maxlength=12 value=\"$row[expiry]\" class=\"boxedfield\">".
    ($row['expiry']?"<input type=\"checkbox\" name=\"delete_expiry\" value=1 class=\"checkbox\"><small>Delete expiry</small>\n":'')));

    if(!$row['admin'])
    {
      $teachers = new SELECTBOX;
      $teachers->set_style('selectbox');
      $teachers->set_multiple(0);
      $teachers->set_name("default_teacher");

      @$res2 = mysql_query("select id,login,name from users where (admin & ".TEACHER.")=".TEACHER);
      $cnt=0;
      while(@$row2=mysql_fetch_array($res2))
      {
        if($row2['id'] == $row['default_teacher'])
          $teachers->set_selected(array($cnt));
        $teachers->add_item("$row2[name] ($row2[login])",$row2['id']);
        $cnt++;
      }
      if($cnt>10)
        $cnt = 10;
      $teachers->set_size($cnt);

      $toptable->add_row(array('Default teacher',$teachers->prepare()));
    }

    $my_categories = array();
    @$res3 = mysql_query("select uc_cou_id from user_categories where uc_u_id = '".((int)$user)."'");
    if($res3)
    {
      while(@$row3 = mysql_fetch_array($res3))
        $my_categories[$row3['uc_cou_id']] = $row3['uc_cou_id'];
    }

    @$res3 = mysql_query("select cou_id, cou_name from categories_of_users order by cou_name");
    $categories = new SELECTBOX;
    $categories->set_style('selectbox');
    $categories->set_multiple(1);
    $categories->set_name('categories[]');
    $cnt = 0;
    $selected = array();
    while(@$row3=mysql_fetch_array($res3))
    {
      if($my_categories[$row3['cou_id']])
        $selected[] = $cnt;
        
      $categories->add_item($row3['cou_name'],$row3['cou_id']);
      $cnt++;
    }
    $categories->set_selected($selected);
    if($cnt>10)
      $cnt = 10;
    $categories->set_size($cnt);
    $toptable->add_row(array('Categories',$categories->prepare()));


    $toptable->add_row(array('&nbsp;','<input type="submit" name="change_user" value="   Save   " class="buttoninbox"> <input type="submit" name="delete" value="   Delete   " class="buttoninbox">'));

    $page->set_top("Change user settings for $row[name]");
    $page->add_to_middle("<center>
    Change settings for selected user or delete user completely.
    Be careful when changing permissions. Since students cannot have
    other permissions, granting user more privileges automaticaly removes
    him or her from students. Other three permissions can be combined
    together without restrictions.<br><br>
    <form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"user\" value=\"$user\">
    <input type=\"hidden\" name=\"section\" value=\"users\">
    ".$toptable->prepare().
    "
    </form>
    </center>\n");
  }
  return 123;
}

include('inc/forms.inc');
if($err) error_f("$err<br><br>\n");

$page->set_top('Process user accounts');
$page->add_to_middle("<center>
On this page you can work with user accounts. The left table shows
existing accounts and actions that you can do with them in the select
box at the bottom. You can select multiple users using checkboxes
on the left. If you select <em>Expire</em> for multiple users
then expiry date will be set on today for all of them.
Selecting <em>Prolong</em> will bring up another dialog, where you will
be able to set expiry dates (in fact you can even <em>Expire</em> accounts
there by setting expiry in the past).
If you click on user's name, the information about the user will be
shown. You can then set default teacher or expiry for the specified user.
The right table is for adding new accounts. All items except
<em>Expiry</em> and <em>Teacher</em> must be filled.<br><br>
</center>\n");

$table = new TABLE;
$table->set_parameters(array('valign'=>'top'));
$ltable = new BOXEDTABLE;
$ltable->set_parameters(array('style'=>'subtable'));
$vmenu = new CHBOX;
$vmenu->set_name('users_list[]');

@$res = mysql_query("select id,login,name,to_days(expiry) as d_expiry, to_days(now()) as d_now from users order by name $desc limit $offset,$limit");

while(@$row = mysql_fetch_array($res))
  $vmenu->add_item("<a href=\"?section=users&user=$row[id]\"><strong>$row[name]</strong> ($row[login])</a>","$row[id]");

$ltable->add_row(array(array('attr'=>'align="left"','cont'=>"<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"users\">
  <input type=\"submit\" name=\"show\" value=\" Show \" class=\"buttoninbox\">
  <input type=\"text\" size=5 maxlength=5 name=\"limit\" value=\"$limit\" class=\"boxedfield\">
  users sorted
  <input type=\"radio\" name=\"desc\" value=0".($desc?'':' checked')." class=\"radio\">ascending <input type=\"radio\" name=\"desc\" value=1".($desc?' checked':'')." class=\"radio\">descending.<br><br>\n")));

if($limit<$users_count)
  $ltable->add_row(array("<small>".pnav($users_count,$offset,$limit,"?section=users&desc=$desc&")."</small><br><br>\n"));

$ltable->add_row(array($vmenu->prepare()));

if($limit<$users_count and $limit>10)
  $ltable->add_row(array("<br><br>\n<small>".pnav($users_count,$offset,$limit,"?section=users&desc=$desc&")."</small>\n"));

$sbox = new SELECTBOX(array(),'users_action',1,0);
$sbox->set_style('selectbox');
$sbox->add_items(array('Assign tasks to'=>'assign','Delete'=>'delete','Expire'=>'expire','Prolong account to'=>'prolong','Set default teacher for'=>'set_teacher'));
$sbox->set_selected(array(0));

$ltable->add_row(array("<br><br>\n".$sbox->prepare()." selected students.<br>\n"));
$ltable->add_row(array("<input type=\"submit\" value=\"   Do it now.   \" class=\"buttoninbox\">\n</form>\n"));

$right = add_user_form(array('add_login'=>$add_login,'add_name'=>$add_name,'add_email'=>$add_email,'add_expiry'=>$add_expiry,'add_teacher'=>$add_teacher));

$table->add_row(array(array('attr'=>'width="50%"','cont'=>"<center><h3>Manage existing accounts</h3>".$ltable->prepare()."</center>"),$right));

$page->add_to_middle($table->prepare());

return 123;
?>
