<?

#error_reporting(2047);

function boards_list()
{
  global $admin;
  global $desc,$offset,$limit,$boards_count;

  @$res=mysql_query("select id,name,description,count,date_format(modified,'%e. %c. %Y, %k:%i') as last from parameters order by name $desc limit $offset,$limit");
  if(@$cnt = mysql_num_rows($res))
  {
    $ex_table = new BOXEDTABLE;
    $ex_table->set_parameters(array('style'=>'subtable'));

    if($admin)
    {
      $vmenu = new CHBOX;
      $vmenu->set_name('boards_list[]');
    }
    else
    {
      $vmenu = new VMENU;
      $vmenu->set_itemized(1);
    }
    
    while(@$row = mysql_fetch_array($res))
      $vmenu->add_item("<a href=\"?section=mb&mb_section=board&board=$row[id]\"><strong>$row[name]</strong></a> <small>($row[description], ".($row['count']?$row['count']:'no')." message".($row['count']>1?'s':'').($row['count']?", last $row[last]":'').")</small>",$row['id']);

    $ex_table->add_row(array(array('attr'=>'align="left"','cont'=>"<form action=\"index.php\" method=\"post\">
      <input type=\"hidden\" name=\"section\" value=\"mb\">
      <input type=\"hidden\" name=\"mb_section\" value=\"boards\">
      <input type=\"submit\" name=\"show\" value=\" Show \">
      <input type=\"text\" size=5 maxlength=5 name=\"limit\" value=\"$limit\" class=\"boxedfield\">
      boards sorted
      <input type=\"radio\" name=\"desc\" value=0".($desc?'':' checked')." class=\"radio\">ascending
      <input type=\"radio\" name=\"desc\" value=1".($desc?' checked':'')." class=\"radio\">descending.<br><br>\n")));

    if($limit<$boards_count)
      $ex_table->add_row(array("<br><br>\n<small>".pnav($boards_count,$offset,$limit,"?section=mb&mb_section=boards&desc=$desc&")."</small>\n"));

    $ex_table->add_row(array($vmenu->prepare()));

    if($limit<$boards_count and $boards_count>20)
      $ex_table->add_row(array("<br><br>\n<small>".pnav($boards_count,$offset,$limit,"?section=mb&mb_section=boards&desc=$desc&")."</small>\n"));

    if($admin)
    {
      $sbox = new SELECTBOX(array(),'action',1,0);
      $sbox->add_items(array('Delete'=>'delete'));
      $sbox->set_selected(array(0));

      $ex_table->add_row(array("<br><br>\n".$sbox->prepare()." selected boards.<br>\n"));
      $ex_table->add_row(array("<input type=\"submit\" value=\"   Do it now.   \" class=\"buttoninbox\">"));
    }
    $ex_table->add_row(array('</form>'));
  }
  return $ex_table;
}

if(!is_numeric($limit) or $limit<0) $limit = 20;
if(!is_numeric($offset) or $offset<0) $offset = 0;

if($desc == 1 or $desc == 'desc') $desc = 'desc';
else $desc = '';

@$res = mysql_query("select count(*) as cnt from parameters");
@$row = mysql_fetch_array($res);
$boards_count = $row['cnt'];

if($admin)
{
  if($action == 'create' and isset($name))
  {
    if($anonymous != 1)
      $anonymous = 0;

    if($admin_notify != 1)
      $admin_notify = 0;

    if(!@mysql_query("insert into parameters (name,description,anonymous,admin_notify) values ('".addslashes($name)."','".addslashes($description)."',$anonymous,$admin_notify)") or !@mysql_insert_id())
      $err = 'Cannot create new board! Perhaps duplicate entry.';
  }
  elseif($action == 'delete' and is_array($boards_list) and !$show)
  {
    $cnt = 0;
    while(list($k,$v) = each($boards_list))
      $ids .= ($cnt++?',':'').(is_numeric($v)?$v:'NULL');
    $sql = "delete from parameters where id in ($ids)";
    @mysql_query($sql);
    $sql = "delete from messages where board_id in($ids)";
    @mysql_query($sql);

  }

  $mb_table = new TABLE;
  $mb_table->set_parameters(array('valign'=>'top'));
  
  $new_table = new BOXEDTABLE;
  $new_table->set_parameters(array('style'=>'subtable'));
  $new_table->add_row(array(array('attr'=>'colspan=2','cont'=>"
  <form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"mb\">
  <input type=\"hidden\" name=\"mb_section\" value=\"boards\">
  <input type=\"hidden\" name=\"action\" value=\"create\">")));
  $new_table->add_row(array('Name','<input type="text" name="name" maxlength=80'));
  $new_table->add_row(array('Description','<input type="text" name="description" maxlength=160>'));
  $new_table->add_row(array('Allow anonymous?','<input type="checkbox" name="anonymous" value=1 checked>'));
  $new_table->add_row(array('Send notification to admin on a new post?','<input type="checkbox" name="admin_notify" value=1>'));
  $new_table->add_row(array('',"<input type=\"submit\" value=\"   Create   \">\n</form>"));

  $ex_table = boards_list();

  $mb_table->add_row(array(array('attr'=>'width="50%"','cont'=>"<center><h3>Manage existing boards</h3>\n".($ex_table?$ex_table->prepare():'(none)')."</center>"),"<center><h3>Create new board</h3>\n".$new_table->prepare()."</center>\n"));

  if($err)
    $mb_page_body .= "<center>$err</center>\n";

  $mb_page_body .= "
  <form action=\"$PHP_SELF\" method=\"post\">
  $local_form_parameters
  ".
  $mb_table->prepare();
}
else
{
  $ex_table = boards_list();
  $mb_page_body .= $ex_table->prepare();
}

$mb_page_title='Discussion boards';
$mb_page_top='Available boards';

return 123;
?>
