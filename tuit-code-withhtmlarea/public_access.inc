<?
error_reporting(0);
include('un_classes.inc');

$page->add_to_head($style_in_head);
$page->set_top('Language exercises');
$page->add_to_middle("<form action=\"$PHP_SELF\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"$section\">\n");

function show_expls($nav_expl,$expls,$ex)
{
  $cont = '';
  reset($expls);
  while(list($k,$v) = each($expls))
    $nav_expl->add_item($v['ex_name'],"?exercise=$v[ex_id]");

  //$cont = "<h4>".$ex->parameters['ex_name']."</h4>";
  if($nav_expl->how_many())
  { 
    IF($Ex->parameters['ex_type']==EXPLANATION)
      $cont .= "Exercises: ";
    else
      $cont .= "Explanations: ";
    $cont .= $nav_expl->prepare();
  }
  return $cont;
}

if(is_numeric($exercise))
{
  $ex = ex_inst($exercise);
  $ex->get_parameters();
  $expls = $ex->get_explanations();
  $page->set_top($ex->parameters['ex_name']);

  if($show_more)
  {
    $nav_expl = new VMENU;
    $cont = show_expls($nav_expl,$expls,$ex);
    $page->add_to_middle($cont);
  }
  else
  {
    $ex->get_data();
    if(isset($submit_exercise))
    {
      ksort($answers);
      $answers['solution']=$translation;
      $ex->set_answers($answers);
      $ex->set_radios_ans($radios);
    }

    if($submit_exercise)
      $cont .= "<h3>Your answers</h3>";
    elseif(count($expls)>0)
    {
      $nav_expl = new HMENU;

      $shown = $cnt = 0;
      reset($expls);
      while(list($k,$v) = each($expls))
      {
        if($cnt++<MAX_EXPL_IN_LIST)
        {
          $page->topnav->add_item($v['ex_name'],"?exercise=$v[ex_id]");
          $shown++;
        }
      }

      if(count($expls)>$shown)
        $page->topnav->add_item("more..","?exercise=".$ex->ex_id."&show_more=1");
    }

    $page->add_to_middle("<center>$cont</center>");

    if($submit_exercise)
      $page->add_to_middle($ex->prepare_test(1,0,0,0,1));
    else
      $page->add_to_middle($ex->prepare_test(0));
    if(!$submit_exercise and $ex->parameters['ex_type']!=EXPLANATION)
      $page->add_to_middle("<br><br>
    <input type=\"hidden\" name=\"exercise\" value=\"$exercise\">
    <input type=\"submit\" name=\"submit_exercise\" value=\"   Submit answers   \" class=\"buttoninbox\">");
  }
}
elseif(is_numeric($unit))
{
  if(!is_numeric($uns))
    $uns = 1;

  $un = new UNIT($unit);
  $un->get_sections();
  if(!is_array($un->sections[$uns]))
    return;
  $un->read_data();
  $page->set_top($un->name.": ".$un->sections[$uns]['name']);
  $page->add_to_middle($un->show_section($uns));
  if(is_array($un->sections[$uns+1]))
  {
    $page->add_to_middle("<br><br>
    <input type=\"hidden\" name=\"unit\" value=\"$unit\">
    <input type=\"hidden\" name=\"uns\" value=\"".($uns+1)."\">
    <input type=\"submit\" name=\"submit_section\" value=\" &gt;&gt; Continue &gt;&gt; \" class=\"buttoninbox\">");
  }
}
else
{
  if(!is_numeric($limit) or $limit<1) $limit = 20;
  if(!is_numeric($offset) or $offset<0) $offset = 0;

  $cond = "ex_type is not null";
  $href_cond = '';
  $h_conds = array();

  if(is_numeric($type) and $type>0)
  {
    $cond .= " and ex_type=$type";
    $h_conds[] = "type=$type";
  }

  if($name_re)
  {
    $cond .= " and ex_name ".addslashes($not_name_re)." regexp '".addslashes($name_re)."'";
    $h_conds[] = "name_re=$name_re";
  }

  if($desc_re)
  {
    $cond .=" and ex_description ".addslashes($not_desc_re)." regexp '".addslashes($desc_re)."'";
    $h_conds[] = "desc_re=$desc_re";
  }

  if(isset($h_categories))
    $categories = split(',',$h_categories);

  if(is_array($categories))
  {
    $cond .= " and ec_et_id in(";
    $cnt = 0;
    $list = '';
    reset($categories);
    while(list($k,$v) = each($categories))
      $list .= ($cnt++?',':'').$v;

    $cond .= "$list)";
  }

  $c = 0;
  while(list($k,$v) = each($h_conds))
    $href_cond .= ($c++?'&':'').$v;

  $types = new SELECTBOX;
  $types->set_style('selectbox');
  $types->set_name('type');
  $types->set_size(1);
  $types->add_items(array('All'=>0,$ex_strs[COMPLETE]=>COMPLETE,$ex_strs[CHOOSE]=>CHOOSE,$ex_strs[CROSSWORD]=>CROSSWORD,$ex_strs[MATCH]=>MATCH,$ex_strs[QANDA]=>QANDA,$ex_strs[EXPLANATION]=>EXPLANATION));
  if($type)
    $types->set_selected(array($type));
 
  $cbox = new SELECTBOX;
  $cbox->set_style('selectbox');
  $cbox->set_name('categories[]');
  $cbox->set_multiple(1);
  @$res = mysql_query("select et_id as id,et_name as name,count(ec_et_id) as cnt from exercise_types left join exercise_categories on et_id=ec_et_id where et_id>100 group by et_id order by et_name");
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

  @$res = mysql_query("select count(ex_id) as cnt from exercises where $cond");
  @$row = mysql_fetch_array($res);
  $cnt = $row['cnt'];

  $rtab = new BOXEDTABLE;
  $rtab->set_parameters(array('style'=>'subtable'));
  $rtab->add_row(array(array('attr'=>'colspan=4','cont'=>"
    <input type=\"submit\" value=\" Show \" name=\"show\" class=\"buttoninbox\"><input type=\"text\" name=\"limit\" value=\"$limit\" size=5 maxlength=5 class=\"boxedfield\"> exercises.")));
  $rtab->add_row(array(array('attr'=>'colspan=4 align="center"','cont'=>'<strong>Search filters</strong>')));
  $rtab->add_row(array(array('attr'=>'colspan=1','cont'=>"Type"),array('attr'=>'colspan=3','cont'=>$types->prepare())));
  $rtab->add_row(array("Categories",array('attr'=>'colspan=3','cont'=>$cbox->how_many()?$cbox->prepare():'<small>(none)</small>')));
  $rtab->add_row(array('Name contains',array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"name_re\" value=\"$name_re\" size=40 maxlength=40 class=\"boxedfield\"> (\"Not\" <input type=\"checkbox\" name=\"not_name_re\" value=\"not\"".($not_name_re?' checked':'')." class=\"checkbox\">)")));
  $rtab->add_row(array('Description contains',array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"desc_re\" value=\"$desc_re\" size=40 maxlength=40 class=\"boxedfield\"> (\"Not\" <input type=\"checkbox\" name=\"not_desc_re\" value=\"not\"".($not_desc_re?' checked':'')." class=\"checkbox\">)")));

  $ltab = new BOXEDTABLE;
  $ltab->set_parameters(array('style'=>'subtable'));
  $ltab->add_row(array(array('attr'=>'align="center"','cont'=>"<h3>Exercises</h3>")));
  if($limit<$cnt)
    $ltab->add_row(array(array('attr'=>'align="center"','cont'=>(pnav($cnt,$offset,$limit,"?section=public_access&$href_cond&")."<br><br>\n"))));

  @$res = mysql_query("select ex_id as id,ex_type as type,ex_name as name,ex_description as description, ex_creator as creator,ec_et_id from exercises left join exercise_categories on ex_id=ec_ex_id where $cond group by ex_type,ex_id order by ex_type,ex_name $desc limit $offset,$limit");

  $t = 0;

  if(@mysql_num_rows($res))
  {
    $menu = new VMENU;
    $menu->set_itemized(0);
    while(@$row = mysql_fetch_array($res))
    {
      if($row['description'])
        $descr = "($row[description])";
      else
        $descr='';
      $menu->add_item("<a href=\"?exercise=$row[id]\"><strong>$row[name]</strong></a> <small>$descr</small>",$row['id']);
    }
    $ltab->add_row(array($menu->prepare()."<br>"));
  }
  else
    $ltab->add_row(array("No exercises in selected categories."));

  $tab = new TABLE;
//  $tab->add_row(array(array('attr'=>'valign="top"','cont'=>$ltab->prepare()),array('attr'=>'valign="top"','cont'=>$rtab->prepare())));
  $tab->add_row(array($rtab->prepare()."<br><br>".$ltab->prepare()));
  
  $page->add_to_middle($tab->prepare());
}

$page->add_to_middle("</form>\n");
return 123;
?>
