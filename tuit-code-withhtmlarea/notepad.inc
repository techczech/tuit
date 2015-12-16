<?
if(isset($URL))
{
  list(,$my_URL) = explode('?',$URL,2);
  if(!$my_URL)
    $my_URL='section=main';
}

$out = '';

if(!is_numeric($note) and $action!='make_new' and $action!='save')
{
  $sql = "select n_id,n_description,n_my_URL from notes where n_who_id=$udata[id]";

  if($show_url)
    $sql .= " and n_my_URL is not null";
  else
    $sql .= " and n_my_URL is null";

  $sql .= " order by n_my_URL";
  @$res = mysql_query($sql);
  @$cnt = mysql_num_rows($res);
  if($cnt)
  {
    $list = new VMENU;
    while(@$row = mysql_fetch_array($res))
    {
      if(empty($row['n_description']))
        $row['n_description'] = '[empty]';

      $list->add_item($row['n_description'],"?section=notepad&note=$row[n_id]&wrapped=$wrapped&show_url=$show_url");
    }

    $out .= $list->prepare();
  }
  else
    $out .= "No notes here. <a href=\"?section=notepad&action=make_new&wrapped=$wrapped\">Create new.</a>";
}
else
{
  if($action == 'save' and isset($text))
  {
    if(isset($do_save))
    {
      $sql = "replace into notes set n_who_id=".$udata['id'].",n_text='".addslashes($text)."',n_description='".addslashes($description)."'";

      if(isset($my_URL))
        $sql .= ",n_my_URL='".addslashes($my_URL)."'";
      if(is_numeric($note))
        $sql .= ", n_id=$note";

      @$res = mysql_query($sql);
      if($res)
      {
        $note = mysql_insert_id();
        $row['n_description']=$description;
        $row['n_text']=$text;
      }
    }
    elseif(isset($do_delete) and is_numeric($note))
    {
      @mysql_query("delete from notes where n_id=$note");
    }
  }
  elseif(is_numeric($note))
  {
    @$res = mysql_query("select n_text,n_description,n_my_URL from notes where n_id=$note");
    @$row = mysql_fetch_array($res);
    @$my_URL=$row['n_my_URL'];
  }

  $out .= "<input type=\"hidden\" name=\"action\" value=\"save\">
  <input type=\"hidden\" name=\"note\" value=\"$note\">
  Description (optional)<br>
  <input type=\"text\" name=\"description\" value=\"$row[n_description]\" class=\"boxedfield\" size=40 maxlength=100>
  <br>
  Text<br>
  <textarea cols=40 rows=13 class=\"largetext\" name=\"text\">$row[n_text]</textarea><br>\n
  <input type=\"submit\" value=\"   Save   \" class=\"buttoninbox\" name=\"do_save\">
  <input type=\"submit\" value=\"   Delete   \" class=\"buttoninbox\" name=\"do_delete\">";
  if(isset($my_URL))
    $out .= "<br><br>\n<input type=\"hidden\" name=\"URL\" value=\"$URL\">Bound to <a href=\"?$my_URL\">?$my_URL</a>";
}

$menu = new HMENU;
$menu->set_style('topnav');
if($show_url or isset($my_URL))
  $show_url=1;
else
  $show_url=0;
$menu_array = array((!$show_url?"<strong>Miscellaneous</strong>":"Miscellaneous")=>"?section=notepad&wrapped=$wrapped&show_url=0",(!$show_url?"Site specific":"<strong>Site specific</strong>")=>"?section=notepad&wrapped=$wrapped&show_url=1&URL=".urlencode($URL),($action=='make_new'?"<strong>New note</strong>":"New note")=>"?section=notepad&wrapped=$wrapped&show_url=$show_url&action=make_new".(isset($URL)?("&URL=".urlencode($URL)):''),"<a href=\"javascript:window.close();\">Close</a>"=>'');
if($wrapped)
{
  $page->topnav->add_items($menu_array);
  $page->set_top("Notepad");
}
else
{
  $menu->add_items($menu_array);
#  $page->top=$menu->prepare();
}

$tab = new BOXEDTABLE;
if($wrapped)
  //$tab->set_parameters(array('bgcolor'=>'#FFFFFF','box_width'=>0));
  $tab->set_parameters(array('style'=>'subtable','box_width'=>1));
else
{
  $tab->set_parameters(array('style'=>'subtable'/*,'all_width'=>'100%'*/));
  $menu->set_style('');
  $tab->add_row(array($menu->prepare()."<br><br>"));
}
$tab->add_row(array($out));
$page->disp_bohemica=$wrapped;
$page->add_to_middle("<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"notepad\">
  <input type=\"hidden\" name=\"wrapped\" value=\"$wrapped\">
  <input type=\"hidden\" name=\"note\" value=\"$note\">".
  $tab->prepare());
return 123;
?>
