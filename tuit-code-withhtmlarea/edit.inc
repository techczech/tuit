<?

#error_reporting(2047);

if(!am_i_cont_admin())
  return 123;

if(!is_numeric($limit) or $limit<0) $limit = 20;
if(!is_numeric($offset) or $offset<0) $offset = 0;

if($desc == 1 or $desc == 'desc') $desc = 'desc';
else $desc = '';

$show_menu = 1;

if($action == 'make_assignment' and is_array($a_list))
{
  include('assign.inc');
  return 123;
}

if($action == 'compose_unit' and !$show)
{
  if(!is_numeric($set)) exit;

  if(isset($save_as_template) and isset($template_name) and is_numeric($set))
  {
    @$res = mysql_query("select * from set_sections where ss_set_id=$set order by ss_seq");
    $ins = 'insert into set_sections values ';
    $cnt = 1;
    while(@$row = mysql_fetch_array($res))
    {
      $ins .= ($cnt>1?',':'')."(NULL,*set_id*,'$row[ss_name]', $cnt)";
      $cnt++;
    }

    @$res = mysql_query("insert into ss_templates set sst_name='".addslashes($template_name)."',sst_insert='".addslashes($ins)."'");
  }

  if(isset($use_template) and isset($template))
  {
    @mysql_query("delete from set_sections where ss_set_id=$set");
    @mysql_query("update exercises_sets set se_ss_id=NULL where se_set_id=$set");
    @$res = mysql_query("select sst_insert from ss_templates where sst_id=$template");
    @$row = mysql_fetch_array($res);
    $ins = str_replace('*set_id*',$set,$row['sst_insert']);
    @mysql_query($ins);
  }

  if(is_array($a_list))
  {
    $sql = 'insert into exercises_sets values ';
    $cnt = 0;
    while(list($k,$v) = each($a_list))
      $sql .= ($cnt++?',':'')."(NULL,$v,$set,NULL,NULL)";
    
    @mysql_query($sql);
  }

  if(isset($rl) and is_array($ex_rl) and is_numeric($insert_exercises))
  {
    @$res = mysql_query("select max(se_seq) as max from exercises_sets where se_set_id=$set and se_ss_id=$insert_exercises");
    @$row = mysql_fetch_array($res);
    $id = $row['max']+1;
    while(list($k,$v) = each($ex_rl))
    {
      @mysql_query("update exercises_sets set se_ss_id=$insert_exercises,se_seq=$id where se_id=$k");
      $id++;
    }
  }

  if(isset($lr) and is_array($ex_lr))
  {
    $list = '';
    $cnt = 0;
    while(list($k,$v) = each($ex_lr))
      $list .= ($cnt++?',':'').$v;
    @mysql_query("update exercises_sets set se_ss_id=NULL,se_seq=NULL where se_id in($list) and se_set_id=$set");
    #@mysql_query("update exercises_sets set se_seq=0 where se_seq in($list)");
  }

  if(($down == 'ex' or $up == 'ex') and is_numeric($se) and is_numeric($set))
  {
    if($up == 'ex')
    {
      $desc = 'desc';
      $comp = '<';
    }
    elseif($down == 'ex')
    {
      $desc = '';
      $comp = '>';
    }

    @$res = mysql_query("select se_ss_id,se_seq from exercises_sets where se_id=$se");
    @$row = mysql_fetch_array($res);
    $seq = $row['se_seq'];
    $ss  = $row['se_ss_id'];
    @$res = mysql_query("select se_id,se_seq from exercises_sets where se_seq $comp $seq and se_ss_id=$ss and se_set_id=$set order by se_seq $desc limit 1");
    if(@mysql_num_rows($res) == 1)
    {
      @$row = mysql_fetch_array($res);
      $se2  = $row['se_id'];
      $seq2 = $row['se_seq'];

      @mysql_query("update exercises_sets set se_seq=$seq where se_id=$se2");
      @mysql_query("update exercises_sets set se_seq=$seq2 where se_id=$se");
    }
  }

  if($down == 'ss' and is_numeric($ss) and is_numeric($set))
  {
    @$res = mysql_query("select ss_seq from set_sections where ss_id=$ss");
    @$row = mysql_fetch_array($res);
    $seq = $row['ss_seq'];
    @$res = mysql_query("select ss_id,ss_seq from set_sections where ss_seq>$seq and ss_set_id=$set order by ss_seq limit 1");
    if(@mysql_num_rows($res)==1)
    {
      @$row = mysql_fetch_array($res);
      $ss2=$row['ss_id'];
      $seq2=$row['ss_seq'];

      @mysql_query("update set_sections set ss_seq=$seq where ss_id=$ss2");
      @mysql_query("update set_sections set ss_seq=$seq2 where ss_id=$ss");
    }
  }
  elseif($up == 'ss' and is_numeric($ss) and is_numeric($set))
  {
    @$res = mysql_query("select ss_seq from set_sections where ss_id=$ss");
    @$row = mysql_fetch_array($res);
    $seq = $row['ss_seq'];
    @$res = mysql_query("select ss_id,ss_seq from set_sections where ss_seq<$seq and ss_set_id=$set order by ss_seq desc limit 1");
    if(@mysql_num_rows($res)==1)
    {
      @$row = mysql_fetch_array($res);
      $ss2=$row['ss_id'];
      $seq2=$row['ss_seq'];

      @mysql_query("update set_sections set ss_seq=$seq where ss_id=$ss2");
      @mysql_query("update set_sections set ss_seq=$seq2 where ss_id=$ss");
    }
  }
  if(isset($make_new_section) and strlen($new_section)>0 and is_numeric($set))
  {
    @$res = mysql_query("select max(ss_seq) as max from set_sections where ss_set_id=$set");
    @$row = mysql_fetch_array($res);
    $max = $row['max']?$row['max']:0;
    @mysql_query("insert into set_sections set ss_name='".addslashes($new_section)."',ss_set_id=$set,ss_seq=$max+1");
  }
  elseif(isset($delete_selected_sections) and is_array($sections))
  {
    $ls = '';
    $cnt = 0;
    while(list($k,$v) = each($sections))
      $ls .= ($cnt++?',':'').$v;
    @mysql_query("delete from set_sections where ss_id in ($ls)");
    @mysql_query("update exercises_sets set se_ss_id=NULL,se_seq=NULL where se_ss_id in($ls)");
  }
  elseif(isset($finish_editing) and is_numeric($set))
    @mysql_query("delete from exercises_sets where se_set_id=$set and se_ss_id is NULL");

  $page->set_top('Alter unit contents');
  $table = new TABLE;
  $ltable = new BOXEDTABLE;
  $ltable->set_parameters(array('style'=>'subtable'));
  $rtable = new BOXEDTABLE;
  $rtable->set_parameters(array('style'=>'subtable'));

  $tempbox = new SELECTBOX;
  $tempbox->set_style('selectbox');
  $tempbox->set_name('template');
  $tempbox->set_size(1);
  $tempbox->set_multiple(0);
  @$res = mysql_query("select sst_id,sst_name from ss_templates order by sst_name");
  while(@$row = mysql_fetch_array($res))
    $tempbox->add_item($row['sst_name'],$row['sst_id']);

  $tempbox->how_many() and $ltable->add_row(array(array('attr'=>'colspan=3','cont'=>'<input type="submit" name="use_template" value="Use a predefined template" class="buttoninbox"> '.$tempbox->prepare())));
  $ltable->add_row(array(array('attr'=>'colspan=3','cont'=>'<input type="submit" name="make_new_section" value=" Add a section " class="buttoninbox"> <input type="text" name="new_section" size=30 maxlength=80 class="boxedfield">')));

  $n = 0;
  $cnt = 0;
  @$res = mysql_query("select ss_id,ss_name,ss_seq,se_id,se_ex_id,ex_name from set_sections left join exercises_sets on ss_set_id=se_set_id and ss_id=se_ss_id left join exercises on se_ex_id=ex_id where ss_set_id=$set order by ss_set_id,ss_seq,se_seq");
  $ex_n = mysql_num_rows($res);
  while(@$row = mysql_fetch_array($res))
  {
    if($row['ss_id'] != $n)
    {
      if($n)
        $ltable->add_row(array(array('attr'=>'colspan=2','cont'=>'&nbsp;')));
      $ltable->add_row(array("<input type=\"checkbox\" name=\"sections[]\" value=$row[ss_id] class=\"checkbox\"> <strong>$row[ss_name]</strong>","<small><a href=\"?section=edit&ed_cont=1&set=$set&action=$action&up=ss&ss=$row[ss_id]\">up</a>/<a href=\"?section=edit&ed_cont=1&set=$set&action=$action&down=ss&ss=$row[ss_id]\">down</a></small>",array('attr'=>'align="right"','cont'=>"<small>Add exercises <input type=\"radio\" name=\"insert_exercises\" value=$row[ss_id] class=\"radio\"></small>")));
      $n = $row['ss_id'];
    }

    if($row['se_ex_id'])
    {
      $ltable->add_row(array(array('attr'=>'colspan=2 align="left"','cont'=>"&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\"checkbox\" name=\"ex_lr[]\" value=$row[se_id] class=\"checkbox\">&nbsp;<strong><small>$row[ex_name]</small></strong>"),"<small><small><a href=\"?section=edit&ed_cont=1&set=$set&action=$action&up=ex&se=$row[se_id]\">up</a>/<a href=\"?section=edit&ed_cont=1&set=$set&action=$action&down=ex&se=$row[se_id]\">down</a></small></small>"));
      $cnt++;
    }
  }
  if($n)
  {
    $ltable->add_row(array(array('attr'=>'colspan=3','cont'=>'&nbsp;')));
    $ltable->add_row(array(array('attr'=>'colspan=3','cont'=>'<input type="submit" name="delete_selected_sections" value="Delete selected sections" class="buttoninbox"><input type="submit" name="finish_editing" value="Close unit contents" class="buttoninbox">')));
    $ltable->add_row(array(array('attr'=>'colspan=3','cont'=>'<input type="submit" name="save_as_template" value="Save as template" class="buttoninbox"> <input type="text" name="template_name" value="Template name" size=30 maxlength=80" class="boxedfield">')));
  }

  $ls = '';
  $cnt = 0;
  while(list($k,$v) = each($a_list))
    $ls .= ($cnt++?',':'').$v;
    
  @$res = mysql_query("select se_id,se_ex_id,ex_name,et_short from exercises_sets left join exercises on ex_id=se_ex_id left join exercise_types on ex_type=et_id where se_ss_id is NULL and se_set_id=$set order by ex_type,ex_name");
  while(@$row = mysql_fetch_array($res))
    $rtable->add_row(array("<input type=\"checkbox\" name=\"ex_rl[$row[se_id]]\" value=$row[se_ex_id] class=\"checkbox\"> <small><strong>$row[ex_name]</strong> <small>($row[et_short])</small></small>"));

  $mtable = new TABLE;
  $ex_n and $mtable->add_row(array(array('attr'=>'align="center"','cont'=>'<input type="submit" name="lr" value=">> Remove exercises >>" class="buttoninbox">')));
  $mtable->add_row(array('&nbsp'));
  $rtable->how_many() and $mtable->add_row(array(array('attr'=>'align="center"','cont'=>'<input type="submit" name="rl" value="<< Add exercises <<" class="buttoninbox">')));

  $table->add_row(array(array('attr'=>'width="50%" valign="top" align="right"','cont'=>$ltable->prepare()),'&nbsp;',array('attr'=>'valign="top" align="center"','cont'=>$mtable->prepare()),'&nbsp;',array('attr'=>'valign="top" align="center"','cont'=>$rtable->how_many()?$rtable->prepare():'')));
  $page->add_to_middle("<form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"$section\">
    <input type=\"hidden\" name=\"set\" value=$set>
    <input type=\"hidden\" name=\"ed_cont\" value=$ed_cont>
    <input type=\"hidden\" name=\"action\" value=\"compose_unit\">
    ".$table->prepare()."</form>");
  $show_menu = 0;
}

/*** show list of exercises with filters.. ***/

if($show_menu):

if($edit == 'exercises')
{
  $top = 'Exercises menu';
  $description = 'exercises';
  $_type = "ex_type";
  $_id = "ex_id";
  $_creator = "ex_creator";
  $_difficulty = "ex_difficulty";
  $_category = "ec_et_id";
  $_href = 'exercise';
  $_name = 'ex_name';
  $sql_delete[1] = 'delete from exercises where ex_id in ($list)';
  $sql_delete[2] = 'delete from exercise_categories where ec_ex_id in($list)';
  $sql_delete[3] = 'delete from ex_choose where ch_ex_id in($list)';
  $sql_delete[4] = 'delete from ex_match where ma_ex_id in($list)';
  $sql_delete[5] = 'delete from ex_qanda where qa_ex_id in($list)';
  $sql_count = 'select count(*) as cnt from exercises where $cond';
  $sql_select = "select ex_id as id,ex_type as type,ex_name as name,ex_description as description, ex_creator as creator,ec_et_id from exercises left join exercise_categories on ex_id=ec_ex_id where ".'$cond'." group by ex_type,ex_id order by ex_type,ex_name $desc limit $offset,$limit";
  $sql_select_cath = "select et_id as id,et_name as name,count(ec_et_id) as cnt from exercise_types left join exercise_categories on et_id=ec_et_id where et_id>100 group by et_id order by et_name";
}
elseif($edit == 'sets')
{
  $top = 'Units menu';
  $description = 'units';
  $_category = $_type = "set_type";
  $_id = "set_id";
  $_creator = "set_creator";
  $_difficulty = "set_difficulty";
  $_href = 'set';
  $_name = 'set_name';
  $sql_delete[1] = 'delete from sets where set_id in ($list)';
  $sql_count = 'select count(*) as cnt from sets where $cond';
  $sql_select = "select set_id as id, set_type as type, set_name as name,set_creator as creator from sets where ".'$cond'." order by set_type, set_name $desc limit $offset,$limit";
  $sql_select_cath = "select st_id as id, st_name as name, count(set_type) as cnt from set_types left join sets on st_id=set_type group by st_id order by st_name";
}

$cond = "$_type is not null";
$href_cond = '';
$h_conds = array();

if(is_numeric($type) and $type>0)
{
  $cond .= " and $_type=$type";
  $h_conds[] = "type=$type";
}

if($name_re)
{
  $cond .= " and $_name ".addslashes($not_name_re)." regexp '".addslashes($name_re)."'";
  $h_conds[] = "name_re=$name_re";
}

if($desc_re)
{
  $cond .= " and ex_description ".addslashes($not_desc_re)." regexp '".addslashes($desc_re)."'";
  $h_conds[] = "desc_re=$desc_re";
}

if(isset($set) and $ed_cont==1)
{
  @$res = mysql_query("select set_name,set_creator,set_difficulty from sets where set_id=$set");
  @$row = mysql_fetch_array($res);
  isset($difficulty) or $difficulty = $row['set_difficulty'];
  is_array($creators) or $creators[] = $row['set_creator'];
  $set_name = $row['set_name'];
  $h_conds[] = 'ed_cont=1';
  $h_conds[] = "set=$set";
}
      
if($action == 'delete')
{
  if(is_array($a_list))
  {
    $cnt = 0;
    while(list($k,$v) = each($a_list))
      $list .= ($cnt++?',':'').$v;

    while(list($k,$v) = each($sql_delete))
      @mysql_query(str_replace('$list',$list,$v));
  }
}

if($action == 'set_categories' and $edit=='exercises' and is_array($categories) and is_array($a_list))
{
  $ls = '';
  $cnt = 0;
  reset($a_list);
  while(list($k,$v) = each($a_list))
  {
    reset($categories);
    while(list($k2,$v2) = each($categories))
      $ls .= ($cnt++?',':'')."($v,$v2)";
  }
  $sql = "replace into exercise_categories values $ls";
  @$res = mysql_query($sql);
  if(!$res)
    $err = 'Error saving categories for selected exercises!';
}

if(isset($h_categories))
  $categories=split(',',$h_categories);

if(is_array($categories))
{
  $cond .= " and $_category in(";
  $cnt = 0;
  $list = '';
  reset($categories);
  while(list($k,$v) = each($categories))
    $list .= ($cnt++?',':'').$v;

  $cond .= $list.')';
  //$h_conds[] = "h_categories=$list";
}

if(isset($h_creators))
  $creators=split(',',$h_creators);

if(is_array($creators))
{
  $cond .= " and $_creator in(";
  $cnt = 0;
  $list = '';
  while(list($k,$v) = each($creators))
    $list .= ($cnt++?',':'').$v;
  $cond .= $list.')';
  $h_conds[] = "h_creators=$list";
}

if(is_numeric($difficulty) and $difficulty>0)
{
  $cond .= " and $_difficulty=$difficulty";
  $h_conds[] = "difficulty=$difficulty";
}

$c = 0;
while(list($k,$v) = each($h_conds))
  $href_cond .= ($c++?'&':'').$v;

if(isset($edit))
{
  @$res = mysql_query(str_replace('$cond',$cond,$sql_count));
  @$row = mysql_fetch_array($res);
  $count = $row['cnt'];

  $page->set_top($top);
  $ltable = new TABLE;

  if($limit<$count)
    $ltable->add_row(array(pnav($count,$offset,$limit,"?section=edit&edit=$edit&desc=$desc&".($href_cond?"$href_cond&":''))."<br><br>\n"));

  @$res = mysql_query(str_replace('$cond',$cond,$sql_select));
  if(mysql_num_rows($res))
  {
    $menu = new VMENU;
    $menu->set_itemized(0);

    $submenu = new CHBOX;
    $submenu->set_style('checkbox');
    $submenu->set_name('a_list[]');
    $t = 0;

    while($row = mysql_fetch_array($res))
    {
      if($row['type'] != $t)
      {
        $t = $row['type'];
        if($submenu->how_many())
          $menu->add_item($submenu->prepare());
        $menu->add_item("<strong>".$ex_strs[$t]."</strong>",'');
        $it = array();
        $submenu->recycle();
      }
      if($row['description'])
        $descr="($row[description])";
      else
        $descr='';
      $submenu->add_item("&nbsp; <a href=\"?section=create&create=$_href&$_href=$row[id]\"><strong>$row[name]</strong> $descr</a> <small>($row[id]) ".($edit=='sets'?" | <a href=\"?section=edit&edit=exercises&set=$row[id]&ed_cont=1\">Edit contents</a> | <a href=\"?section=unit_display&unit=$row[id]\">Preview</a>":" | <a href=\"?section=exercises&exercise=$row[id]&preview=1\">Preview</a>")."</small>",$row['id']);
    }
    if($submenu->how_many())
      $menu->add_item($submenu->prepare());

    $ltable->add_row(array($menu->prepare()));
  }
}
else
  $ltable = new TABLE;

if(isset($edit))
{
  if($edit == 'exercises')
  {
    $types = new SELECTBOX;
    $types->set_style('selectbox');
    $types->set_name('type');
    $types->set_size(1);
    $types->add_items(array('All'=>0,$ex_strs[COMPLETE]=>COMPLETE,$ex_strs[CHOOSE]=>CHOOSE,$ex_strs[CROSSWORD]=>CROSSWORD,$ex_strs[MATCH]=>MATCH,$ex_strs[QANDA]=>QANDA,$ex_strs[EXPLANATION]=>EXPLANATION));
    if($type)
      $types->set_selected(array($type));
  }

  $cbox = new SELECTBOX;
  $cbox->set_style('selectbox');
  $cbox->set_name('categories[]');
  $cbox->set_multiple(1);
  @$res = mysql_query($sql_select_cath);
  $cbox->set_size( ($cnt = mysql_num_rows($res))>10?10:$cnt );
  $sel = array();
  $cnt = 0;
  while(@$row = mysql_fetch_array($res))
  {
    $cbox->add_item($row['name'].' ('.$row['cnt'].')',$row['id']);
    if(in_array($row['id'],$categories))
      $sel[] = $cnt;

    $cnt++;
  }
  $cbox->set_selected($sel);

  $tbox = new SELECTBOX;
  $tbox->set_style('selectbox');
  $tbox->set_name('creators[]');
  $tbox->set_multiple(1);
  $sel = array();
  $cnt = 0;
  @$res = mysql_query("select id,login,name from users where admin&".CONT_ADMIN."=".CONT_ADMIN."");
  while(@$row = mysql_fetch_array($res))
  {
    $tbox->add_item($row['name']." (".$row['login'].")",$row['id']);
    if(in_array($row['id'],$creators))
      $sel[] = $cnt;

    $cnt++;
  }
  $tbox->set_selected($sel);

  $tbox->set_size( ($cnt = mysql_num_rows($res))>10?10:$cnt );

  $dbox=new SELECTBOX;
  $dbox->set_style('selectbox');
  #$dbox->add_items(array('All'=>0,$diff_strs[BASIC]=>BASIC,$diff_strs[INTERMEDIATE]=>INTERMEDIATE,$diff_strs[ADVANCED]=>ADVANCED));
  $dbox->add_item('All',0);
  while(list($k,$v) = each($diff_strs))
    $dbox->add_item($v,$k);
  $dbox->set_size(1);
  $dbox->set_name('difficulty');
  $dbox->set_selected(array($difficulty));

  $abox = new SELECTBOX;
  $abox->set_style('selectbox');
  $abox->set_size(1);
  $abox->set_name('action');
  $abox->add_items(array('Delete'=>'delete'));

  if($edit=='exercises')
    $abox->add_item('Set categories for','set_categories');

  if(($edit == 'exercises' or (isset($ed_cont)) and is_numeric($set)))
    $abox->add_item('Compose unit with','compose_unit');

  if(!empty($assign))
  {
    $abox->add_item('Make assignment with','make_assignment');
    $abox->set_selected(array($abox->how_many()-1));
  }

  if(isset($ed_cont) and is_numeric($set))
    $abox->set_selected(array(2));

  $rtable = new BOXEDTABLE;

  $rtable->set_parameters(array('style'=>'subtable','cellpadding'=>2,'valign'=>'top'));
  $rtable->add_row(array(array('attr'=>'colspan=4','cont'=>"
  <input type=\"submit\" value=\" Show \" name=\"show\" class=\"buttoninbox\"><input type=\"text\" name=\"limit\" value=\"$limit\" size=5 maxlength=5 class=\"boxedfield\">
  $description sorted
  <input type=\"radio\" value=0 name=\"desc\"".($desc=='desc'?'':' checked')." class=\"radio\">&nbsp;ascending
  <input type=\"radio\" name=\"desc\" value=1".($desc=='desc'?' checked':'')." class=\"radio\">&nbsp;descending
  ")));
  $rtable->add_row(array(array('attr'=>'colspan=4 align="center"','cont'=>'<strong>Filters</strong>')));
  $types and $rtable->add_row(array(array('attr'=>'colspan=1','cont'=>"Type"),array('attr'=>'colspan=3','cont'=>$types->prepare())));
  $rtable->add_row(array("Categories",$cbox->how_many()?$cbox->prepare():'<small>(none)</small>',"Creators",$tbox->how_many()?$tbox->prepare():'<small>(none)</small>'));
  $rtable->add_row(array("Difficulty",array('attr'=>'colspan=3','cont'=>$dbox->prepare())));
  $rtable->add_row(array('Name like',array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"name_re\" value=\"$name_re\" size=40 maxlength=40 class=\"boxedfield\"> (\"Not\" <input type=\"checkbox\" name=\"not_name_re\" value=\"not\"".($not_name_re?' checked':'')." class=\"checkbox\">)")));
  $rtable->add_row(array('Description like',array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"desc_re\" value=\"$desc_re\" size=40 maxlength=40 class=\"boxedfield\"> (\"Not\" <input type=\"checkbox\" name=\"not_desc_re\" value=\"not\"".($not_desc_re?' checked':'')." class=\"checkbox\">)")));
}
else
  $rtable = new TABLE;

/*
$table = new TABLE;
$table->set_parameters(array('valign'=>'top'));
$table->add_row(array($ltable->how_many()?(array('attr'=>'width="50%"','cont'=>
$ltable->prepare()."<br><br>".$abox->prepare()." selected $description<br>
<input type=\"submit\" value=\"   Do it now   \" class=\"buttoninbox\">")):'<strong>No entries in selected categories.</strong>',array('attr'=>'width="50%"','cont'=>$rtable->prepare())));
*/
$table = $rtable->prepare()."<br><br>".($ltable->how_many()?($ltable->prepare()."<br><br>".$abox->prepare()." selected $description<br><input type=\"submit\" value=\"   Do it now   \" class=\"buttoninbox\">"):'<strong>No entries in selected categories.</strong>');

if(is_numeric($set) and $ed_cont)
  $page->insert_to_middle("<center><h3>Choose exercises to add to the unit <i>$set_name</i></h3><a href=\"?section=edit&ed_cont=1&set=$set&action=compose_unit\">Continue without selecting more exercises</a></center><br>");

$page->add_to_middle("<form action=\"index.php\" method=\"post\">
<input type=\"hidden\" name=\"section\" value=\"edit\">
<input type=\"hidden\" name=\"edit\" value=\"$edit\">
<input type=\"hidden\" name=\"ed_cont\" value=\"$ed_cont\">
<input type=\"hidden\" name=\"set\" value=\"$set\">
<input type=\"hidden\" name=\"assign\" value=\"$assign\">
<input type=\"hidden\" name=\"user\" value=\"$user\">".
$table."</form>");

endif;

/*** end of show list of exercises and filters ***/

return 123;
?>
