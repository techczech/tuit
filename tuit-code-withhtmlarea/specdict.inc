<?

if (am_i_cont_admin()):

$page->set_top('Specialist dictionaries');
#@mysql_select_db('dict');
/*
if(!is_numeric($dict) and $dict!='all')
{ 
  $res = mysql_query('select * from dicts order by d_name');
  while($row = mysql_fetch_array($res))
    $menu.="<a href=\"?section=specdict&dict=$row[d_id]\" alt=\"$row[d_desc]\">$row[d_name]</a><br>";
  $menu.="<a href=\"?section=specdict&dict=all\"><b>All</b></a><br>";
  $page->add_to_middle($menu);
}
else
{
*/
  $dicts=array();
  @$res = mysql_query("select * from dicts");
  while(@$row = mysql_fetch_array($res))
    $dicts[$row['d_id']]=$row['d_name'];
  asort($dicts);

  if(!$find_in)
    $find_in='all';
  $tab = new TABLE;
  $ltab = new BOXEDTABLE;
  $ltab->set_parameters(array('style'=>'subtable'));

  if(isset($fw) and !isset($find))
    $find_word = $fw;

  $ltab->add_row(array("Search for: <input type=\"text\" name=\"find_word\" value=\"$find_word\" size=40 maxlength=100 class=\"boxedfield\">"));
  $ltab->add_row(array("in <input type=\"radio\" name=\"find_in\" value=\"all\"".($find_in=='all'?' checked':'')." class=\"radio\">All
  <input type=\"radio\" name=\"find_in\" value=\"v_czech\"".($find_in=='v_czech'?' checked':'')." class=\"radio\">Czech
  <input type=\"radio\" name=\"find_in\" value=\"v_english\"".($find_in=='v_english'?' checked':'')." class=\"radio\">English
  <input type=\"radio\" name=\"find_in\" value=\"v_latin\"".($find_in=='v_latin'?' checked':'')." class=\"radio\">Latin
  <input type=\"radio\" name=\"find_in\" value=\"v_slovak\"".($find_in=='v_slovak'?' checked':'')." class=\"radio\">Slovak
  <input type=\"radio\" name=\"find_in\" value=\"v_german\"".($find_in=='v_german'?' checked':'')." class=\"radio\">German"));

  $sbox = new SELECTBOX;
  $sbox->set_size(1);
  $sbox->set_style('selectbox');
  $sbox->set_name("find_dict");
  $cnt = 0;
  reset($dicts);
  while(list($k,$v) = each($dicts))
  {
    $sbox->add_item($v,$k);
    if($find_dict==$k)
      $sbox->set_selected(array($cnt));
    $cnt++;
  }
  if(!is_numeric($find_dict))
    $sbox->set_selected(array($cnt));
  $sbox->add_item('All','all');

  $ltab->add_row(array($sbox->prepare()));
  
  $ltab->add_row(array("<input type=\"submit\" name=\"find\" value=\"   Find   \" class=\"buttoninbox\">"));

  if(isset($find) or isset($find_word))
  {
    $sql = "select * from vocab where 1";
    if(is_numeric($find_dict))
      $sql .= " and v_d_id=$find_dict";
    if($find_in=='all')
    {
      $w = " regexp '".addslashes($find_word)."'";
      $sql .= " and (v_czech$w or v_english$w or v_latin$w or v_slovak$w or v_german$w)";
    }
    else
      $sql .=" and ".addslashes($find_in)." regexp '".addslashes($find_word)."'";

    @$res = mysql_query($sql);
    while(@$row = mysql_fetch_array($res))
      $ltab->add_row(array("<a href=\"?section=specdict&dict=$dict&word=$row[v_id]&fw=$find_word\"><b>$row[v_czech]</b>, $row[v_english], $row[v_latin], $row[v_slovak], $row[v_german]</a>"));
  }

  if(isset($save) and isset($v_czech) and isset($v_english) and isset($v_latin) and is_numeric($dict))
  {
    $sql = "replace into vocab set v_czech='".addslashes($v_czech)."',v_english='".addslashes($v_english)."',v_latin='".addslashes($v_latin)."',v_slovak='".addslashes($v_slovak)."',v_german='".addslashes($v_german)."',v_note='".addslashes($v_note)."'";
    if(is_numeric($word))
      $sql .= ",v_id=$word";

    $sql .= ",v_d_id=$dict";
    @$res = mysql_query($sql);
    if($res)
    {
      $page->insert_to_middle("<center><h3>Input saved..</h3></center>");
      $word = '';
    }
  }

  if(isset($delete) and is_numeric($word))
  {
    @mysql_query("delete from vocab where v_id=$word");
    $word='';
  }

  if(isset($start))
    $word='';

  if(is_numeric($word))
  {
    @$res = mysql_query("select * from vocab where v_id=$word");
    @$voc = mysql_fetch_array($res);
  }

  $rtab = new BOXEDTABLE;
  $rtab->set_parameters(array('style'=>'subtable'));

  $rtab->add_row(array("Czech","<input type=\"text\" size=40 maxlength=100 name=\"v_czech\" value=\"$voc[v_czech]\" class=\"boxedfield\">"));
  $rtab->add_row(array("English","<input type=\"text\" size=40 maxlength=100 name=\"v_english\" value=\"$voc[v_english]\" class=\"boxedfield\">"));
  $rtab->add_row(array("Latin","<input type=\"text\" size=40 maxlength=100 name=\"v_latin\" value=\"$voc[v_latin]\" class=\"boxedfield\">"));
  $rtab->add_row(array("Slovak","<input type=\"text\" size=40 maxlength=100 name=\"v_slovak\" value=\"$voc[v_slovak]\" class=\"boxedfield\">"));
  $rtab->add_row(array("German","<input type=\"text\" size=40 maxlength=100 name=\"v_german\" value=\"$voc[v_german]\" class=\"boxedfield\">"));
  $rtab->add_row(array("Note","<input type=\"text\" size=40 maxlength=255 value=\"$voc[v_note]\" name=\"v_note\" class=\"boxedfield\">"));

  $sbox = new SELECTBOX;
  $sbox->set_size(1);
  $sbox->set_name("dict");
  $cnt = 0;
  reset($dicts);
  while(list($k,$v) = each($dicts))
  {
    $sbox->add_item($v,$k);
    if(is_numeric($voc['v_d_id']))
    {
      if($voc['v_d_id']==$k)
        $sbox->set_selected(array($cnt));
    }
    elseif($dict==$k)
      $sbox->set_selected(array($cnt));
    $cnt++;
  }

  $sbox->add_item('No dictionary selected','all');
  $rtab->add_row(array("Dictionary",$sbox->prepare()));

  $rtab->add_row(array("",array('attr'=>'align=center','cont'=>"<input type=\"submit\" name=\"save\" value=\"   Save changes  \" class=\"buttoninbox\"><input type=\"submit\" name=\"delete\" value=\"   Delete   \" class=\"buttoninbox\"><br><input type=\"submit\" name=\"start\" value=\"   Start new   \" class=\"buttoninbox\">")));

  $tab->add_row(array(array('attr'=>'valign=top','cont'=>$ltab->prepare()),array('attr'=>'valign=top','cont'=>$rtab->prepare())));

  $out = "<form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"specdict\">
    <input type=\"hidden\" name=\"dict\" value=\"$dict\">
    <input type=\"hidden\" name=\"word\" value=\"$word\">
    <input type=\"hidden\" name=\"fw\" value=\"$find_word\">".
    $tab->prepare().
    "</form>";

  $page->add_to_middle($out);
/*
}
*/

endif;

return 123;
?>
