<?

include('as_base.inc');
#error_reporting(2047);

if(!am_i_teacher())
  return 123;

$show_menu = 0;
$page->set_top('Assignments');

$page->insert_to_middle("<form action=\"index.php\" method=\"post\">
<input type=\"hidden\" name=\"section\" value=\"assign\">
\n");


if(isset($use_template) and is_numeric($template))
{
  @$res = mysql_query("select atm_query from as_templates where atm_id=$template");
  if(mysql_num_rows($res) != 1)
  {
    $page->add_to_middle('<center><h3>Cannot read template!</h3></center>');
    $show_menu = 1;
  }
  else
  {
    @$row = mysql_fetch_array($res);
    $as = new ASSIGNMENT;
    $as->parse_query($row['atm_query']);
    $a_list = $as->task_list;
    $cnt = 1;
    $seq = array();
    reset($a_list);
    while(list($k,$v) = each($a_list))
      $seq[$v] = $cnt++;
    if($as->task_type == EXERCISES)
      $edit = 'exercises';
    elseif($as->task_type == UNITS)
      $edit = 'sets';
    $after_edit=1;
    $as_modifier = $as->modifier;
    $as_limit = $as->limit;
  }
}

if($show_assignment==1 and is_numeric($assignment))
{
  $as = new ASSIGNMENT($assignment);
  $as->read_db_record();
  $ls = '';
  $cnt = 0;
  while(list($k,$v) = each($as->task_list))
    $ls .= ($cnt++?',':'').$v;

  $sql = 'select ';
  if($as->task_type==EXERCISES)
    $sql .= "ex_name as name, ex_type as type, ex_difficulty as difficulty from as_tasks,exercises where at_task_id=ex_id and at_task_id in ($ls) and at_as_id=$assignment";
  elseif($as->task_type==UNITS)
    $sql .= "set_name as name,set_difficulty as difficulty,st_name as type from as_tasks,sets,set_types where at_task_id=set_id and set_type=st_id and at_task_id in ($ls) and at_as_id=$assignment";

  @$res = mysql_query($sql);
  $ltable = new BOXEDTABLE;
  $ltable->set_parameters(array('style'=>'subtable','cellpadding'=>2));
  $ltable->add_row(array("<strong>Name</strong>","Type","Difficulty"));
  $ltable->add_row(array(array('attr'=>'colspan=3','cont'=>'<hr size=1 noshade>')));
  while(@$row = mysql_fetch_array($res))
  {
    $ltable->add_row(array("<strong>$row[name]&nbsp;&nbsp;</strong>",($as->task_type==EXERCISES?substr($ex_strs[$row['type']],0,40):$row['type'])."&nbsp;&nbsp;",$diff_strs[$row['difficulty']]));
  }

  $ls = '';
  $cnt = 0;
  while(list($k,$v) = each($as->who_list))
    $ls .= ($cnt++?',':'').$v['id'];

  $sql = "select name, aw_complete from users,as_who,assignments where aw_who_id=id and aw_as_id=as_id and aw_who_id in($ls) and aw_as_id=$assignment";
  @$res = mysql_query($sql);
  $rtable = new BOXEDTABLE;
  $rtable->set_parameters(array('style'=>'subtable','cellpadding'=>2));
  $rtable->add_row(array("<strong>Name</strong>","Complete"));
  $rtable->add_row(array(array('attr'=>'colspan=2','cont'=>'<hr size=1 noshade>')));
  while(@$row = mysql_fetch_array($res))
  {
    $rtable->add_row(array("<strong>$row[name]</strong>&nbsp;&nbsp;",$row['aw_complete']?'Complete':'Incomplete'));
  }

  $table = new TABLE;
  $page->insert_to_middle("<center><h3>$as->description</h3></center>");
  $table->add_row(array(array('attr'=>'width="50%" align="center" valign="top"','cont'=>"<h4>Tasks (".$task_str[$as->task_type].")</h4>".$ltable->prepare()),array('attr'=>'width="50%" align="center" valign="top"','cont'=>"<h4>Students</h4>".$rtable->prepare())));
  $page->add_to_middle("<center>".$table->prepare()."</center>");
}
elseif(isset($show_student) or isset($show_all) or isset($delete_as) or isset($remove_student))
{
  if(is_array($s_list))
  {
    $cnt = 0;
    $ls = '';
    while(list($k,$v) = each($s_list))
      $ls .= ($cnt++?',':'').$v;

    if(isset($remove_student))
    {
      @mysql_query("delete from as_who where aw_as_id in($ls)");
    }
    elseif(isset($delete_as))
    {
      @mysql_query("delete from assignments where as_id in($ls)");
      @mysql_query("delete from as_who where aw_as_id in($ls)");
      @mysql_query("delete from as_tasks where at_as_id in($ls)");
    }
  }

  if(isset($h_teachers))
    $teachers=split(',',$h_teachers);

  if(!$teachers)
    $teachers[] = $udata['id'];

  if(!is_numeric($limit) or $limit<0) $limit = 20;
  if(!is_numeric($offset) or $offset<0) $offset = 0;
  if(!isset($sort_by)) $sort_by = 'date';

  if($desc == 1 or $desc == 'desc') $desc = 'desc';
  else $desc = '';

  if($sort_by == 'description')
    $sort = 'as_description';
  elseif($sort_by == 'date')
    $sort = 'as_when';

  $cond = '';

  if($complete == 'complete')
    $cond .= ' and aw_complete=1';
  elseif($complete == 'incomplete')
    $cond .= ' and aw_complete=0';
  elseif($complete != 'all')
    $complete = 'all';

  if($active == 'active')
    $cond .= ' and as_active=1';
  elseif($active == 'inactive')
    $cond .= ' and as_active=0';
  elseif($active != 'all')
    $active = 'all';

  if(isset($show_student) and is_numeric($student))
  {
    $cond .= " and aw_who_id=$student";
    $h_conds[] = "student=$student&show_student=1";
  }
  elseif(isset($show_all))
  {
    $h_conds[] = "show_all=1";
    $student = '';
  }

  if(is_array($teachers))
  {
    $cnt = 0;
    $list = '';
    while(list($k,$v) = each($teachers))
      $list .= ($cnt++?',':'').$v;
    $cond .= " and as_by in($list)";
    $h_conds[] = "h_teachers=$list";
  }
  
  $c = 0;
  while(list($k,$v) = each($h_conds))
    $href_cond .= ($c++?'&':'').$v;

  $as_list = array();
  $table = new TABLE;
  @$res = mysql_query("select name,login from users where id=$student");
  @$row = mysql_fetch_array($res);
  $st_name = $row['name'];
  $st_login = $row['login'];
  $page->insert_to_middle("<center>Assignments".((isset($show_student) or isset($remove_student))?" for <strong>$st_name</strong> <small>($st_login)</small></center>":'')."<br><br>");
  @$res = mysql_query("select count(*) as cnt from as_who,assignments where aw_as_id=as_id $cond");
  @$row = mysql_fetch_array($res);
  $count = $row['cnt'];
  @$res = mysql_query("select as_id,as_description,date_format(as_when,'%e. %c. %Y') as date,as_active,aw_complete,name,login from assignments left join as_who on as_id=aw_as_id,users where as_by=id $cond order by $sort $desc limit $offset,$limit");
  $cnt = 0;
  while(@$row = mysql_fetch_array($res))
  {
    $as_list[$cnt]['id'] = $row['as_id'];
    $as_list[$cnt]['description'] = $row['as_description']?$row['as_description']:'(none)';
    $as_list[$cnt]['date'] = $row['date'];
    $as_list[$cnt]['active'] = $row['as_active'];
    $as_list[$cnt]['complete'] = $row['aw_complete'];
    $as_list[$cnt]['t_name'] = $row['name'];
    $as_list[$cnt]['t_login'] = $row['login'];
    $cnt++;
  }
  $ltable = new TABLE;
  $ltable->set_parameters(array('width'=>'','cellpadding'=>2));

  if($limit<$count)
    $ltable->add_row(array(array('attr'=>'colspan=2','cont'=>pnav($count,$offset,$limit,"?section=assign&active=$active&complete=$complete&desc=$desc&".($href_cond?"$href_cond&":''))."<br><br>\n")));
    
  while(list($k,$v) = each($as_list))
  {
    $ltable->add_row(array("<em><small>$v[date]</small></em>",array('attr'=>'align="left"','cont'=>(is_teacher($udata['admin'])?"<input type=\"checkbox\" name=\"s_list[]\" value=\"$v[id]\" class=\"checkbox\">":'')."<strong><a href=\"?section=assign&student=$student&show_assignment=1&assignment=$v[id]\">$v[description]</a></strong>"),array('cont'=>"<a href=\"?section=correct&student=$student&assignment=$v[id]\">Correct</a>&nbsp;".((isset($show_student) or isset($remove_student))?($v['complete']?'<font color="#FF0000">(Complete)</font>':''):''))));
  }
  $ltable->add_row(array(array('attr'=>'colspan=2','cont'=>'&nbsp;')));
  $ltable->add_row(array(array('attr'=>'colspan=2','cont'=>(isset($show_student) or isset($remove_student))?"<input type=\"submit\" name=\"remove_student\" value=\"Remove student from selected assignments\" class=\"buttoninbox\">":"<input type=\"submit\" name=\"delete_as\" value=\"Delete selected assignments\" class=\"buttoninbox\">")));

  $tbox = new SELECTBOX;
  $tbox->set_multiple(1);
  $tbox->set_name('teachers[]');
  @$res = mysql_query("select id,name,login from users where admin&".TEACHER."=".TEACHER);
  $cnt = mysql_num_rows($res);
  $tbox->set_size($cnt>5?5:$cnt);
  $sel = array();
  $c = 0;
  while(@$row = mysql_fetch_array($res))
  {
    $tbox->add_item("$row[name] ($row[login])",$row['id']);
    if(in_array($row['id'],$teachers))
      $sel[] = $c;

    $c++;
  }
  $tbox->set_selected($sel);

  $rtable = new BOXEDTABLE;
  $rtable->set_parameters(array('style'=>'subtable','cellpadding'=>2));
  $rtable->add_row(array(array('attr'=>'colspan=3','cont'=>"
  <input type=\"hidden\" name=\"student\" value=$student>
  <input type=\"submit\" name=\"".(isset($show_student)?'show_student':'show_all')."\" value=\"   Show   \" class=\"buttoninbox\"><input type=\"text\" name=\"limit\" value=\"$limit\" size=5 maxlength=5 class=\"boxedfield\">
  assignments sorted
  <input type=\"radio\" value=0 name=\"desc\"".($desc?'':' checked')." class=\"radio\">&nbsp;ascending
  <input type=\"radio\" name=\"desc\" value=1".($desc?' checked':'')." class=\"radio\">&nbsp;descending")));
  $rtable->add_row(array("&nbsp;",array('attr'=>'colspan=2','cont'=>"by <input type=\"radio\" name=\"sort_by\" value=\"description\"".($sort_by=='description'?' checked':'')." class=\"radio\"> Description <input type=\"radio\" name=\"sort_by\" value=\"date\"".($sort_by=='date'?' checked':'')." class=\"radio\"> Date")));
  $rtable->add_row(array(array('attr'=>'align="center" colspan=3','cont'=>'<strong>Filters</strong>')));
  $rtable->add_row(array(array('attr'=>'valign="top"','cont'=>"Teachers"),array('attr'=>'colspan=2','cont'=>$tbox->prepare())));
  $rtable->add_row(array(array('attr'=>'valign="top"','cont'=>'<input type="radio" name="complete" value="complete"'.($complete=='complete'?' checked':'').' class="radio"> Complete'), array('attr'=>'valign="top"','cont'=>'<input type="radio" name="complete" value="incomplete"'.($complete=='incomplete'?' checked':'').' class="radio"> Incomplete'),array('attr'=>'valign="top"','cont'=>'<input type="radio" name="complete" value="all"'.($complete=='all'?' checked':'').' class="radio"> All')));
  $rtable->add_row(array(array('attr'=>'valign="top"','cont'=>'<input type="radio" name="active" value="active"'.($active=='active'?' checked':'').' class="radio"> Active'),array('attr'=>'valign="top"','cont'=>'<input type="radio" name="active" value="inactive"'.($active=='inactive'?' checked':'').' class="radio"> Inactive'),array('attr'=>'valign="top"','cont'=>'<input type="radio" name="active" value="all"'.($active=='all'?' checked':'').' class="radio"> All')));

  $table->add_row(array(array('attr'=>'width="50%" valign="top"','cont'=>$ltable->prepare()),array('attr'=>'width="50%" valign="top"','cont'=>$rtable->prepare())));
  $page->add_to_middle($table->prepare());

}
elseif( (isset($next_students) or isset($next_template)) and is_array($a_list) and is_array($seq) and isset($edit))
{
  $as = new ASSIGNMENT;
  if(is_numeric($asssignment_id))
    $as->id=$assignment_id;

  if(isset($description))
    $as->set_description($description);

  if($edit == 'exercises')
    $ttype = EXERCISES;
  elseif($edit == 'sets')
    $ttype = UNITS;

  if(is_numeric($as_modifier))
    $as->modifier=$as_modifier;
  if(ereg('^[[:digit:]]{4}-[[:digit:]]{1,2}-[[:digit:]]{1,2}$',$as_limit))
    $as->limit=$as_limit;

  $seq2 = array();
  while(list($k,$v) = each($seq))
    $seq2[] = $k;
  $as->set_tasks($ttype,$seq2);

  $q = $as->prepare_query();
  if(isset($next_template))
  {
    $res = mysql_query("insert into as_templates set atm_name='".addslashes($template_name)."',atm_query='".addslashes($q)."'");
    $page->add_to_middle('Template saved.');
    $show_menu=1;
  }
  elseif(isset($next_students))
  {
    $sbox = new SELECTBOX;
    $sbox->set_style('selectbox');
    $sbox->set_multiple(1);
    $sbox->set_name('students[]');
    @$res = mysql_query('select id,name,login from users where admin=0 or admin is null');
    $cnt = 0;
    while(@$row = mysql_fetch_array($res))
    {
      if(!is_numeric($row['id']))
        continue;

      if(is_super_admin($udata['admin']))
      {
        $sbox->add_item("$row[name] ($row[login])",$row['id']);
        $cnt++;
        continue;
      }

      @$res2 = mysql_query('select uc_cou_id from user_categories where uc_u_id='.$row['id']);
      $student_categories = array();
      while(@$row2 = mysql_fetch_array($res2))
        $student_categories[] = $row2['uc_cou_id'];

      if(count($student_categories) and count(array_intersect($student_categories,$MY_CATEGORIES)))
      {
        $sbox->add_item("$row[name] ($row[login])",$row['id']);
        $cnt++;
      }
    }

    $sbox->set_size($cnt>20?20:$cnt);

    $tab = new BOXEDTABLE;
    $tab->set_parameters(array('style'=>'subtable'));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"<h3>Check assignment in the text field and choose students from the list</h3>")));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    $tab->add_row(array("<strong>Assignment</strong>","<input type=\"text\" name=\"query\" size=60 value=\"$q\" class=\"boxedfield\">"));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    $tab->add_row(array("<strong>Choose students</strong> ",$sbox->prepare()));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    $tab->add_row(array(array('attr'=>'colspan=2 align="center"','cont'=>'<input type="submit" name="next_modifiers" value="   Continue   " class="buttoninbox">')));
    $page->add_to_middle("<center>\n".$tab->prepare()."</center>");
    if(isset($description))
      $page->add_to_middle("<input type=\"hidden\" name=\"description\" value=\"".$description."\">");
  }
}
elseif(isset($next_modifiers) and isset($query))
{
  if(!is_array($students))
    error_f('You must choose students from the list.');
  else
  {
    $qu = new ASSIGNMENT;
    $qu->parse_query($query);
    $qu->who_type=STUDENTS;
    $ls = '';
    $cnt=0;
    while(list($k,$v) = each($students))
    {
      $qu->who_list[$cnt]['id']=$v;
      $ls .= ($cnt?',':'').$v;
      $cnt++;
    }
    @$res = mysql_query("select aw_as_id,as_description from as_who,assignments where aw_as_id=as_id and aw_who_id in($ls) group by aw_as_id");
    if(@$cnt = mysql_num_rows($res))
    {
      $abox = new SELECTBOX;
      $abox->set_style('selectbox');
      $abox->set_multiple(0);
      $abox->set_name('previous');
      $abox->set_size($cnt>10?10:$cnt);
      while(@$row = mysql_fetch_array($res))
        $abox->add_item($row['as_description']?$row['as_description']:$row['aw_as_id'].' (no description)',$row['aw_as_id']);
    }
    
    $tab = new BOXEDTABLE;
    $tab->set_parameters(array('style'=>'subtable'));
    $tab->add_row(array("<h3>Choose action</h3>"));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    $tab->add_row(array("<strong>Assignment</strong>","<input type=\"text\" name=\"query\" size=60 value=\"".$qu->prepare_query()."\" class=\"boxedfield\">"));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    $tab->add_row(array("<strong>On</strong>","<table><tr><td><input type=\"radio\" name=\"on\" value=".LOGIN." checked class=\"radio\"></td><td><b>Login (<small>after YYYY-MM-DD</small> <input type=\"text\" name=\"after\" size=10 maxlength=10 class=\"boxedfield\"> )</b></td></tr></table>"));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    if($abox and $abox->how_many())
    {
      $tab->add_row(array("&nbsp;","<table><tr><td><input type=\"radio\" name=\"on\" value=".COMPLETING." class=\"radio\"></td><td><b>Completing</b></td></tr><tr><td>&nbsp;</td><td>".$abox->prepare()."</td></tr></table>"));
      $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    }
    $tab->add_row(array("<strong>Comment</strong>","<textarea rows=10 cols=60 name=\"comment\" class=\"largetext\"></textarea>"));
    $tab->add_row(array(array('attr'=>'colspan=2','cont'=>"&nbsp;")));
    
    $tab->add_row(array(array('attr'=>'colspan=2 align="center"','cont'=>'<input type="submit" value="   Finish   " name="finish" class="buttoninbox">')));
    $page->add_to_middle("<center>\n".$tab->prepare()."</center>");
    if(isset($description))
      $page->add_to_middle("<input type=\"hidden\" name=\"description\" value=\"$description\">");
  }
}
elseif(isset($finish) and isset($query) and is_numeric($on))
{
  $qu = new ASSIGNMENT;
  $qu->parse_query($query);

  if(isset($description))
    $qu->set_description($description);
  if(isset($comment))
    $qu->set_comment($comment);
  if($on == COMPLETING and isset($previous))
  {
    $qu->on = $on;
    $qu->on_after = $previous;
  }
  elseif(ereg('^[[:digit:]]{4}-[[:digit:]]{1,2}-[[:digit:]]{1,2}$',$after))
    $qu->on_after = $after;

  if($qu->prepare_query()!=-1)
  {
    if($qu->write_db_record()!=-1)
    {
      $out = "Task(s) assigned.";
      $show_menu = 1;
    }
    else
      $out = "Error: cannot save assignment!";
  }
  else
    $out = "Error preparing assignment!";

  if($out)
    $page->add_to_middle("<center><h3>$out</h3></center><br><br>");
  
}
elseif(($section == 'edit' or $after_edit==1) and is_array($a_list))
{
  if(!is_numeric($as_modifier))
    $as_modifier=SEQUENTIAL;
  if(is_array($del_a_list) and isset($remove))
  {
    $tmp = array();
    reset($a_list);
    while(list($k,$v) = each($a_list))
    {
      if(!$del_a_list[$v])
        $tmp[] = $v;
    }
    $a_list = $tmp;
  }
    
  $page->add_to_middle("<input type=\"hidden\" name=\"after_edit\" value=1>\n");
  $show_menu = 0;
  $ls = '';
  $cnt = 0;
  reset($a_list);
  while(list($k,$v) = each($a_list))
  {
    if($v)
      $ls .= ($cnt++?',':'').$v;
  }

  if($edit == 'exercises')
    $sql = "select ex_id as id,ex_name as name from exercises where ex_id in($ls)";
  elseif($edit == 'sets')
    $sql = "select set_id as id, set_name as name from sets where set_id in($ls)";

  $a_box = new TABLE;

  @$res = mysql_query($sql);
  $cnt = 1;
  while(@$row = mysql_fetch_array($res))
  {
    $mod_a_list[$cnt]['id']   = $row['id'];
    $mod_a_list[$cnt]['name'] = $row['name'];
    $mod_a_list[$cnt]['seq']  = $cnt;
    $cnt++;
  }

  $a_list = array();
  if(is_array($seq) or isset($reorder))
  {
    reset($mod_a_list);
    while(list($k,$v) = each($mod_a_list))
    {
      $a_list[$seq[$v['id']]]['id'] = $v['id'];
      $a_list[$seq[$v['id']]]['name'] = $v['name'];
      $a_list[$seq[$v['id']]]['seq']  = $seq[$v['id']];
    }
  }
  else
    $a_list = $mod_a_list;

  ksort($a_list);
  reset($a_list);
  while(list($k,$v) = each($a_list))
  {
    $a_box->add_row(array("<input type=\"hidden\" name=\"a_list[]\" value=$v[id]><input type=\"checkbox\" name=\"del_a_list[$v[id]]\" value=1 class=\"checkbox\">&nbsp;<input type=\"text\" name=\"seq[$v[id]]\" value=\"$v[seq]\" size=2 maxlength=3 class=\"boxedfield\">&nbsp;<strong>$v[name]</strong>"));
  }

  $ltable = new TABLE;
  $ltable->add_row(array(array('attr'=>'align="center"','cont'=>'<h3>Make assignment</h3>Description (optional):<br><input type="text" size=40 maxlength=200 name="description" class="boxedfield"><br><br>')));
  $ltable->add_row(array($a_box->prepare()));

  $rtable = new TABLE;
  $rtable->add_row(array(array('attr'=>'align="center"','cont'=>'<h3>Additional parameters</h3>
  Task order
  <select name="as_modifier" class="selectbox">
  <option value='.SELECT.($as_modifier==SELECT?' selected':'').'>Select any exercise
  <option value='.SEQUENTIAL.($as_modifier==SEQUENTIAL?' selected':'').'>Sequential
  </select><br><br>
  Limit&nbsp;(YYYY-MM-DD) <input type="text" name="as_limit" size=10 maxlength=10 value="'.$as_limit.'" class="boxedfield"><br><br>
  ')));

  $table = new BOXEDTABLE;
  $table->set_parameters(array('style'=>'subtable'));
  $table->add_row(array(array('attr'=>'width="45%" valign="top" align="center"','cont'=>$ltable->prepare()),array('attr'=>'width="10%"','cont'=>'&nbsp;&nbsp;&nbsp;'),array('attr'=>'width="45%" valign="top" align="center"','cont'=>$rtable->prepare())));
  $table->add_row(array(array('attr'=>'align="center" colspan=3','cont'=>"<br><input type=\"hidden\" name=\"edit\" value=\"$edit\"><input type=\"submit\" name=\"reorder\" value=\"Reorder sequence\" class=\"buttoninbox\"> <input type=\"submit\" name=\"remove\" value=\"Remove selected\" class=\"buttoninbox\"> <input type=\"submit\" name=\"next_students\" value=\"   Continue   \" class=\"buttoninbox\"><br><br>
  <input type=\"submit\" name=\"next_template\" value=\"Save as a template\" class=\"buttoninbox\"> <input type=\"text\" name=\"template_name\" size=30 maxlength=80 value=\"Template name\" class=\"boxedfield\">")));

  $page->add_to_middle("<center>\n".$table->prepare()."</center>\n");
}
else
  $show_menu = 1;

if($show_menu)
{
  $tlist = new SELECTBOX;
  $tlist->set_multiple(0);
  $tlist->set_name('template');
  $tlist->set_style('selectbox');
  @$res = mysql_query("select atm_id,atm_name from as_templates");
  $cnt = mysql_num_rows($res);
  $tlist->set_size($cnt>10?10:$cnt);
  while(@$row = mysql_fetch_array($res))
    $tlist->add_item($row['atm_name'],$row['atm_id']);

  $menu = new VMENU;
  $menu->add_items(array('Make assignment of units'=>'?section=edit&edit=sets&assign=sets','Make assignment of single exercises'=>'?section=edit&edit=exercises&assign=exercises'/*,'Make template teaching plan'=>'?section=assign&assign=template'*/,''=>''));
  @$res = mysql_query("select id,name,login from users where admin=0 or admin is null");
  $cnt = mysql_num_rows($res);
  $slist = new SELECTBOX;
  $slist->set_name('student');
  $slist->set_style('selectbox');
  $slist->set_multiple(0);
  while(@$row = mysql_fetch_array($res))
    $slist->add_item("$row[name] ($row[login])",$row['id']);
  $slist->set_size($cnt>20?20:$cnt);
  if($tlist->how_many())
    $menu->add_item("
  <li><input type=\"submit\" name=\"use_template\" value=\"Use a template\" class=\"buttoninbox\"><br><br>\n".$tlist->prepare(),'');

  $rtable = new BOXEDTABLE;
  $rtable->set_parameters(array('style'=>'subtable'));
  $rtable->add_row(array('<center><h3>Show plan for student:</h3>'.$slist->prepare().'<br><br><input type="submit" name="show_student" value="   Show   " class="buttoninbox"> <input type="submit" name="show_all" value="   Show all   " class="buttoninbox"></center>'));

  $table = new TABLE;
  $table->add_row(array(array('attr'=>'valign="top" width="50%"','cont'=>$menu->prepare()),array('attr'=>'valign="top" width="50%"','cont'=>$rtable->prepare())));
  $page->add_to_middle($table->prepare());
}

$page->add_to_middle("</form>\n");

return 123;
?>
