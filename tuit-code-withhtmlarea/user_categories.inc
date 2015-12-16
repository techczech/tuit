<?

include('inc/forms.inc');
/*
   user categories processing script..
*/

#error_reporting(2047);

if(!am_i_acct_admin())
  return 123;

# by default show 20 first users sorted alphabeticaly ascending..

if(!is_numeric($limit) or $limit<0) $limit = 20;
if(!is_numeric($offset) or $offset<0) $offset = 0;

if($desc == 1 or $desc == 'desc') $desc = 'desc';
else $desc = '';

@$res = mysql_query("select count(*) as cnt from categories_of_users");
@$row = mysql_fetch_array($res);
$users_count = $row['cnt'];

if(isset($add_category))
{
  if(!$add_name)
    $err = 'Enter name of the category!';
  else
  {
    @$res2 = mysql_query("insert into categories_of_users (cou_name, cou_description) values ('".addslashes($add_name)."', '".addslashes($add_description)."')");

    if(!$res2)
      $err = "Creation of user category '".htmlspecialchars($add_name)."' failed!";
    else
    {
      $page->add_to_middle("<center><h3>User category '".htmlspecialchars($add_name)."' added.</h3></center><br><br>\n");
      $add_name='';
      $add_description='';
    }
  }
}
elseif(isset($change_category) and is_numeric($id))
{
  if(@mysql_query("update categories_of_users set cou_name='".addslashes($name)."', cou_description='".addslashes($description)."' where cou_id='".addslashes($id)."'"))
    $page->add_to_middle('<center><h3>Category settings saved.</h3></center>');
  else
    $err='Cannot save new settings!';
}
elseif(isset($delete))
{
  $q1 = 'delete from user_categories where uc_cou_id in (';
  $sql = 'delete from categories_of_users where cou_id in (';
  $cnt = 0;
  reset($users_list);
  while(list($k,$v) = each($users_list))
  {
    $sql .= ($cnt++?',':'').$v;
    $q1 .= ($cnt++?',':'').$v;
  }
  $sql .= ')';
  $q1 .= ')';

  @mysql_query($q1);
  if(@mysql_query($sql))
    $page->add_to_middle('<center><h3>Categories deleted.</h3></center>');
  else
    $err='Cannot delete account(s)!';
}

if(is_numeric($id))
{
  @$res = mysql_query("select * from categories_of_users where cou_id='".addslashes($id)."'");

  if(@$row = mysql_fetch_array($res))
  {
    $page->set_top("Change settings for category $row[cou_name]");
    $page->add_to_middle("<center>
    Change settings for selected category.
    <br><br>
    <form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"user_categories\">
    ".edit_user_category_form($row).
    "
    </form>
    </center>\n");
  }
  return 123;
}

if($err) error_f("$err<br><br>\n");

$page->set_top('User categories');
$page->add_to_middle("<center>
</center>\n");

$table = new TABLE;
$table->set_parameters(array('valign'=>'top'));
$ltable = new BOXEDTABLE;
$ltable->set_parameters(array('style'=>'subtable'));
$vmenu = new CHBOX;
$vmenu->set_name('users_list[]');

@$res = mysql_query("select cou_id, cou_name, cou_description from categories_of_users order by cou_name $desc limit $offset,$limit");

while(@$row = mysql_fetch_array($res))
  $vmenu->add_item("<a href=\"?section=user_categories&id=$row[cou_id]\" title=\"$row[cou_description]\"><strong>$row[cou_name]</strong></a>","$row[cou_id]");

$ltable->add_row(array(array('attr'=>'align="left"','cont'=>"<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"user_categories\">
  <input type=\"submit\" name=\"show\" value=\" Show \" class=\"buttoninbox\">
  <input type=\"text\" size=5 maxlength=5 name=\"limit\" value=\"$limit\" class=\"boxedfield\">
  records sorted
  <input type=\"radio\" name=\"desc\" value=0".($desc?'':' checked')." class=\"radio\">ascending <input type=\"radio\" name=\"desc\" value=1".($desc?' checked':'')." class=\"radio\">descending.<br><br>\n")));

if($limit<$users_count)
  $ltable->add_row(array("<small>".pnav($users_count,$offset,$limit,"?section=user_categories&desc=$desc&")."</small><br><br>\n"));

$ltable->add_row(array($vmenu->prepare()));

if($limit<$users_count and $limit>10)
  $ltable->add_row(array("<br><br>\n<small>".pnav($users_count,$offset,$limit,"?section=user_categories&desc=$desc&")."</small>\n"));

$ltable->add_row(array("<input type=\"submit\" value=\"   Delete selected   \" class=\"buttoninbox\" name=\"delete\">\n</form>\n"));

$right = add_user_category_form(array('add_name'=>$add_name,'add_description'=>$add_description));

$table->add_row(array(array('attr'=>'width="50%"','cont'=>"<center><h3>Manage user categories</h3>".$ltable->prepare()."</center>"),$right));

$page->add_to_middle($table->prepare());

return 123;
?>
