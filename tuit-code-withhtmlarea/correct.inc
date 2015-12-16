<?

if(!am_i_teacher())
  return 123;

include('as_base.inc');
include('ex_classes.inc');

$page->set_top('Correct');

if(is_numeric($student) and is_numeric($assignment))
{
  $as = new ASSIGNMENT($assignment);
  $as->read_db_record();
  $show_menu=1;
  if(is_numeric($task))
  {
    $show_menu=0;
    if(isset($save_corrected))
    {
      if(is_array($qanda_cor))
      {
        @$res = mysql_query("select aq_id from an_qanda,answers where aq_an_id=an_id and an_ex_id=$task and an_who_id=$student and an_as_id=$assignment order by aq_id");
        while(@$row = mysql_fetch_array($res))
          $aq_ids[] = $row['aq_id'];

        $cor_cnt=0;
        $ls = '';
        while(list($k,$v) = each($qanda_cor))
        {
          if($v)
            $ls .= ($cor_cnt++?',':'').$aq_ids[$k];
        }
        $sql = "update an_qanda set aq_correct=1 where aq_id in($ls)";
        @mysql_query($sql);
      }
      elseif($mark_trans_cor)
      {
        @$res = mysql_query("select acr_id from an_crossword,answers where acr_an_id=an_id and an_ex_id=$task and an_who_id=$student and an_as_id=$assignment");
        @$row = mysql_fetch_array($res);
        $acr_id=$row['acr_id'];

        @mysql_query("update an_crossword set acr_translation_correct=1 where acr_id=$acr_id");
      }

      if(is_array($complete_cor) or is_array($radios_cor) or is_array($percents))
      {
        $cc_serial = serialize($complete_cor);
        $rc_serial = serialize($radios_cor);
        @$res = mysql_query("select aco_id,aco_type from an_complete, answers where aco_an_id=an_id and an_ex_id=$task and an_who_id=$student and an_as_id=$assignment");
        $aco=array();
        while(@$row = mysql_fetch_array($res))
          $aco[$row['aco_id']]=$row['aco_type'];

        $pt = array();
        if(is_array($points))
        {
          $cor_cnt = 0;
          while(list($k,$v) = each($points))
          {
            if(is_numeric($percents[$k]))
            {
              $cor_cnt += round($percents[$k]*$v/100,1);
              $pt[] = round($percents[$k]*$v/100,1);
            }
            else
            {
              $cor_cnt += $v;
              $pt[] = $v;
            }
          }
        }

        while(list($k,$v) = each($aco))
        {
          if($v == ANSWERS)
          {
            $aco_correct = $cc_serial;
            $aco_pt = serialize($pt);
          }
          elseif($v == RADIOS)
            $aco_correct = $rc_serial;

          @$res = mysql_query("update an_complete set aco_correct='".addslashes($aco_correct)."',aco_points='".addslashes($aco_pt)."' where aco_id=$k");
          if(!$res)
            echo "bla!";
        }
      }
      if(!$cor_cnt)
        $cor_cnt=0;
      if($res)
        @$res = mysql_query("update answers set an_corr_by=$udata[id],an_corr_when=now(),an_points=$cor_cnt,an_comment='".addslashes($corr_comment)."' where an_as_id=$assignment and an_who_id=$student and an_ex_id=$task");
      $show_menu=1;
      $task = 0;
    }
    else
    {
      $tab = new BOXEDTABLE;
      $tab->set_parameters(array('style'=>'subtable','cellpadding'=>5));
      $x = ex_inst($task);
      $tab->add_row(array($x->prepare_test(1,1,$student,$assignment)));
      $tab->add_row(array(array('attr'=>'align="center"','cont'=>"Enter any comment here:<br><textarea name=\"corr_comment\" class=\"largetext\" cols=60 rows=10></textarea>")));
      $tab->add_row(array(array('attr'=>'align="center"','cont'=>"<input type=\"submit\" name=\"save_corrected\" value=\"   OK   \" class=\"buttoninbox\">")));
      $page->add_to_middle("<center>
      <form action=\"index.php\" method=\"post\">
      <input type=\"hidden\" name=\"section\" value=\"correct\">
      <input type=\"hidden\" name=\"student\" value=\"$student\">
      <input type=\"hidden\" name=\"assignment\" value=\"$assignment\">
      <input type=\"hidden\" name=\"task\" value=\"$task\">
      ".$tab->prepare()."
      </form>
      </center>");
    }
  }

  $tr = $as->read_task_records($task?$task:0);
  if($show_menu)
  {
    $vm = new VMENU;
    reset($as->task_list);
    while(list($k,$v) = each($as->task_list))
    {
      if($tr[$v]['type']==EXPLANATION)
        continue;
      $ic = $as->is_complete($v,$student);
      $icor = $as->is_corrected($v,$student);
      $vm->add_item((($ic and !$icor)?'':'<li>')."<strong>".$tr[$v]['name']."</strong>".($icor?" | <a href=\"?section=results&student=$student&assignment=$assignment&task=$v\">Review</a>":''),($ic and !$icor)?"?section=correct&student=$student&assignment=$assignment&task=$v":'');
    }
    $page->add_to_middle("Choose exercises. Names without references are not yet complete.<br><br>".$vm->prepare()."</form>");
  }
}
else
{
  if(!is_numeric($limit) or $limit<0) $limit = 20;
  if(!is_numeric($offset) or $offset<0) $offset = 0;

  if($desc == 1 or $desc == 'desc') $desc = 'desc';
  else $desc = '';

  if(isset($find_student) and is_numeric($find_by) and isset($find_query))
  {
    $cond = 'and ';
    if($find_by)
      $cond .= 'login ';
    else
      $cond .= 'name ';

    $cond .= "like '%".addslashes($find_query)."'";
  }

  $table = new BOXEDTABLE;
  $table->set_parameters(array('style'=>'subtable'));
  $vmenu = new VMENU;
  reset($MY_CATEGORIES);
  $mc_ls = '';
  while(list($k,$v) = each($MY_CATEGORIES))
    $mc_ls .= ($mc_ls?',':'').$v;

  if(is_teacher($udata['admin']) and !empty($mc_ls))
    $cond .= ' and uc_cou_id in ('.$mc_ls.')';

  @$res = mysql_query("select count(distinct id) as cnt from users left join user_categories on id=uc_u_id where (admin=0 or admin) is null $cond");
  @$row = mysql_fetch_array($res);
  $users_count = $row['cnt'];

  @$res = mysql_query("select distinct id,login,name from users left join user_categories on id=uc_u_id where (admin=0 or admin is null) $cond order by name $desc limit $offset,$limit");
  $cnt = mysql_num_rows($res);
  while(@$row = mysql_fetch_array($res))
    $vmenu->add_item("<strong>$row[name]</strong> <small>($row[login])</small>","?section=assign&show_student=1&student=$row[id]");

  $table->add_row(array(array('attr'=>'align="left"','cont'=>"<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"correct\">
  <input type=\"submit\" name=\"show\" value=\" Show \" class=\"buttoninbox\">
  <input type=\"text\" size=5 maxlength=5 name=\"limit\" value=\"$limit\" class=\"boxedfield\">
  users sorted
  <input type=\"radio\" name=\"desc\" value=0".($desc?'':' checked')." class=\"radio\">ascending <input type=\"radio\" name=\"desc\" value=1".($desc?' checked':'')." class=\"radio\"> descending.<br><br>\n")));
  if($limit<$users_count)
    $table->add_row(array("<small>".pnav($users_count,$offset,$limit,"?section=users&desc=$desc&")."</small><br><br>\n"));
  if($cnt)
    $table->add_row(array($vmenu->prepare()));
  else
    $table->add_row(array("No students matching selected categories"));

  $table->add_row(array("&nbsp;"));
  $table->add_row(array("<input type=\"submit\" name=\"find_student\" value=\" Find student\" class=\"buttoninbox\"> by <input type=\"radio\" name=\"find_by\" value=0 class=\"radio\"".($find_by?'':' checked')."> name <input type=\"radio\" name=\"find_by\" value=1 class=\"radio\"".($find_by?' checked':'')."> login<br><input type=\"text\" name=\"find_query\" value=\"$find_query\" size=40 class=\"boxedfield\">"));
  $table->add_row(array("</form>"));

  $page->add_to_middle($table->prepare());
}

return 123;
?>
