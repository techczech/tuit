<?

$page->set_top('Comments');

if(isset($mainmenu) and isset($comment))
{
  mail($admin_email,"Comment from user $udata[name]",$comment,"From: $emails_from");
  $page->insert_to_middle('<center>Comments sent</center>');
  include('main.inc');
  return 123;
}
elseif(!am_i_teacher())
  return 123;
else
{
  if(is_array($users) and isset($subject) and isset($comment) and isset($from))
  {
    $ls = '';
    $c = 0;
    while(list($k,$v) = each($users))
      $ls = ($c++?',':'').$v;

    @$res = mysql_query("select id,email from users where id in($ls)");
    while(@$row = mysql_fetch_array($res))
      mail($row['email'],$subject,$comment,"From: $from");

    $page->insert_to_middle('<h3><center>Comments sent</center></h3>');
    $subject = '';
    $comment = '';
    $users = array();
  }
  else
    $page->insert_to_middle('<h3><center>Please fill in all items in the form.</center></h3>');

  $sbox = new SELECTBOX;
  $sbox->set_multiple(1);
  $sbox->set_style('selectbox');
  $sbox->set_name('users[]');
  @$res = mysql_query("select id,login,name,email from users");
  @$cnt = mysql_num_rows($res);
  while(@$row = mysql_fetch_array($res))
    $sbox->add_item("$row[name] ($row[login], $row[email])",$row['id']);

  $sbox->set_size($cnt>15?15:$cnt);

  $ltable = new BOXEDTABLE;
  $ltable->set_parameters(array('style'=>'subtable'));
  $ltable->add_row(array("<strong>Subject:</strong><br><input type=\"text\" name=\"subject\" size=50 maxlength=160 value=\"$subject\" class=\"boxedfield\">"));
  $ltable->add_row(array("<strong>Comment:</strong><br><textarea rows=15 cols=50 name=\"comment\" class=\"largetext\">$comment</textarea>"));

  $rtable = new BOXEDTABLE;
  $rtable->set_parameters(array('style'=>'subtable'));
  $rtable->add_row(array(array('attr'=>'align="center"','cont'=>'<strong>Select users</strong><br><br>')));
  $rtable->add_row(array($sbox->prepare()));

  $table = new TABLE;
//  $table->add_row(array(array('attr'=>'width="50%" valign="top"','cont'=>$ltable->prepare()),array('attr'=>'width="50%" valign="top"','cont'=>$rtable->prepare())));
  $table->add_row(array(array('attr'=>'colspan=2 align="center"','cont'=>$rtable->prepare()."<br>".$ltable->prepare())));
  $table->add_row(array(array('attr'=>'colspan=2 align="center"','cont'=>'<input type="submit" value="   Send   " class="buttoninbox">')));
  $page->add_to_middle("<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"comments\">
  <input type=\"hidden\" name=\"from\" value=\"$udata[email]\">".$table->prepare()."</form>");

}
  

return 123;
?>
