<?

error_reporting(0);

include('ex_classes.inc');
include('as_base.inc');

$max_comment_len = 200;

function can_see($as,$task=0)
{
  global $who_id;
  if(!$task)
    return -1;
  if($as->modifier==SELECT)
    return 1;
  if(!is_array($as->complete))
    $as->read_complete_list($who_id);

  if(!is_array($as->task_types))
    $as->read_task_types();

  $bla = $as->task_list;
  reset($bla);
  $pprev = $ret = $prev = 0;
  while(list($k,$v) = each($bla))
  {
    if($as->task_types[$prev]!=EXPLANATION)
      $pprev = in_array($prev,$as->complete);
    if($v == $task)
    {
      if(!$prev)
        $ret = 1;
      elseif(in_array($prev,$as->complete) or $pprev)
        $ret = 1;

      break;
    }
    $prev = $v;
  }
  return $ret;
}

/* Student section / now also teacher section..*/
if(is_teacher($udata['admin']) and $preview==1)
{
  if(is_numeric($exercise))
  {
    if(isset($submit))
    {
      if(is_array($answers))
      {
        print_r($answers);
        echo ": ";
        echo $ans_s=serialize($answers)."<br>\n";
        print_r(unserialize($ans_s));
        echo "<hr>\n";
      }
      if(is_array($radios))
      {
        echo $rad_s=serialize($radios)."<br>\n";
        print_r(unserialize($rad_s));
        echo "<br>\n";
      }
    }
    $tab = new BOXEDTABLE;
    $tab->set_parameters(array('style'=>'subtable_ex','width'=>''));
    $ex = ex_inst($exercise);
    if(is_array($MODULES))
      $href = "tuit/";
    $page->topnav->add_item("For printing",$href."?section=print_exercise&exercise=".$ex->ex_id);
    $tab->add_row(array($ex->prepare_test()));
//    $tab->add_row(array("<input type=\"submit\" name=\"submit\" class=\"buttoninbox\">"));
    $page->set_top($ex->parameters['ex_name']);
    $page->add_to_middle("<form action=\"index.php\" method=\"get\" name=\"ex_form\"><input type=\"hidden\" name=\"section\" value=\"exercises\"><input type=\"hidden\" name=\"exercise\" value=$exercise><input type=\"hidden\" name=\"preview\" value=1><center>\n".$tab->prepare()."</center></form>");
  }
}
else
{
  $h_ap='';
  if(!$udata['admin'])
    $who_id=$udata['id'];
  elseif($student)
  {
    $who_id=$student;
    $h_ap.="&student=$student";
  }
  else
    echo "choose student first!";


  if(!is_numeric($assignment))
  {
    if(!$show_all and !$show_complete)
      $show_incomplete=1;
    $cond = '';
    $page->topnav->add_items(array("Show incomplete"=>"?section=exercises$h_ap","Show complete"=>"?section=exercises&show_complete=1$h_ap","Show all"=>"?section=exercises&show_all=1$h_ap"));
    if($show_all)
      $top = "Assigned tasks (complete and incomplete)";
    elseif($show_complete)
      $top = "Complete tasks";
    else
      $top = "Incomplete assigned tasks";

    $page->set_top($top);

    $tab = new TABLE;
    $tab->set_parameters(array('cellspacing'=>5));
    @$res = mysql_query("select a1.as_id as as_id,a1.as_description as desc1,to_days(a1.as_limit)-to_days(now()) as limit_ex,date_format(a1.as_when,'%e. %c. %Y') as as_date,a1.as_comment as comment,a1.as_on as a_on,a1.as_on_after as on_after,aw_complete,to_days(now()) as days_now,name,a2.as_description as desc2 from assignments a1,as_who,users left join assignments a2 on a1.as_on_after=a2.as_id where a1.as_id=aw_as_id and a1.as_by=id and aw_who_id=$who_id and aw_active=1 $cond order by a1.as_id");
    $cnt = 0;
    while(@$row = mysql_fetch_array($res))
    {
      $ases[$row['as_id']]['desc'] = $row['desc2'];
      $ases[$row['as_id']]['complete'] = $row['aw_complete'];
      if($row['a_on']==LOGIN and $row['days_now']-$row['on_after']<0)
        continue;
      elseif($row['a_on']==COMPLETING and is_numeric($row['on_after']) and !$ases[$row['on_after']]['complete'])
        continue;
      elseif(isset($show_complete) and !$row['aw_complete'])
        continue;
      elseif(isset($show_incomplete) and $row['aw_complete'])
        continue;

      $cnt++;
      $ases[$row['as_id']]['accessible'] = 1;
      $comment = $row['comment'];
      if(strlen($comment) > $max_comment_len)
        #$comment = '"'.substr($comment,0,$max_comment_len)."...\"&nbsp;&nbsp;<a href=\"#\" onclick=\"alert('".preg_replace('/("|\'|\\n|\\r)/','',$comment)."')\"><strong><small>more</small></strong></a>";
        $comment = '"'.substr($comment,0,$max_comment_len)."...\"&nbsp;&nbsp;<a href=\"#\" onclick=\"mywin=window.open('','Comment','toolbar=no,location=no,directories=no,status=no,menubar=no,resizable=yes,width=300,height=300');mywin.document.open('');mywin.document.write('".preg_replace('/("|\'|\\r|\\n)/',' ',nl2br($comment))."<br><br><a href=javascript:window.close();>Close</a>');mywin.document.close();\"><strong><small>more</small></strong></a>";

      $subtabr = new BOXEDTABLE;
      $subtabr->set_parameters(array('bgcolor'=>"#FFFFFF"));
      if($row['limit_ex'])
        $subtabr->add_row(array("<font color=\"#FF0000\">Time limit exceeded $row[limit_ex] days ago!</font>"));
      $subtabr->add_row(array("Assigned by <strong>$row[name]</strong>"));
      if(($row['a_on'] == COMPLETING) and is_numeric($row['on_after']) and !$ases[$row['on_after']]['complete'])
      {
        $subtabr->add_row(array("Accessible after completing \"<strong><a href=\"?section=exercises&assignment=$row[on_after]\">$row[desc2]</a>\""));
        $acc = 0;
      }
      else
      {
        $subtabr->add_row(array($comment));
        $subtabr->add_row(array("<br><br><small><a href=\"?section=exercises&assignment=$row[as_id]$h_ap\"><!--See what's inside--><img src=\"img/glass.gif\" border=0 alt=\"Look inside\">See what's inside</a></small>"));
        $acc = 1;
      }

      $subtabl = new BOXEDTABLE;
      $subtabl->set_parameters(array('bgcolor'=>"#FFCC00"));
      $subtabl->add_row(array("<small>$row[as_date]</small>&nbsp;&nbsp;&nbsp;".($acc?"<a href=\"?section=exercises&assignment=$row[as_id]$h_ap\">":'')."<strong>$row[desc1]</strong>".($acc?"</a>":'')."
      <br><br><br>"));
      /*
      $subtabl->add_row(array(array('attr'=>'align="right"','cont'=>"<a href=\"#\" onclick=\"mywin=window.open('?section=notes&type=".N_ASSIGNMENT."&task=$row[as_id]','Notepad','toolbar=no,location=no,directories=no,status=no,menubar=no,resizable=yes,scrollbars=yes,width=400,height=400');\"><img src=\"img/notepad.gif\" border=0 alt=\"Notepad\"></a>")));
      */
      $tab->add_row(array(array('attr'=>'width="20%" bgcolor="#FFCC00" valign="top"','cont'=>$subtabl->prepare()),array('attr'=>'width="80%" valign="top"','cont'=>$subtabr->prepare())));
      $tab->add_row(array(array('attr'=>'colspan=2','cont'=>'&nbsp')));
    }
    $page->add_to_middle("<center><h3>$top</h3>".($cnt?$tab->prepare():'No tasks')."<br><br><a href=\"?section=exercises$h_ap\">Show incomplete</a> | <a href=\"?section=exercises&show_complete=1$h_ap\">Show complete</a> | <a href=\"?section=exercises&show_all=1$h_ap\">Show all</a></center>");
  }
  else
  {
    $as = new ASSIGNMENT($assignment);
    $as->read_db_record();
    $show_ok=1;
    if($as->on == COMPLETING and is_numeric($as->on_after))
    {
      @$res = mysql_query("select aw_complete,as_description from as_who,assignments where aw_as_id=as_id and aw_as_id=$as->on_after and aw_who_id=$who_id");
      if(@mysql_num_rows($res) != 1)
      {
        error_f('Error fetching info about assignment!');
        $show_ok=0;
      }
      else
      {
        @$row = mysql_fetch_array($res);
        if(!$row['aw_complete'])
        {
          error_f("You must first complete <a href=\"?section=exercises&assignment=$as->on_after$h_ap\">$row[as_description]</a>");
          $show_ok=0;
        }
      }
    }

    if(!$as->active)
    {
      error_f("Not a valid assignment!");
      $show_ok = 0;
    }

    if($show_ok)
    {
      $tpar = $as->read_task_records();
      $page->set_top($as->description);
      $cont2 = new TABLE;
      $cont2->set_parameters(array('width'=>'90%'));
      if(!is_numeric($task))
      {
        $cont2->add_row(array(nl2br($as->comment)));
        $cont  = '<h4>Choose exercise on the left panel</h4>Exercises without reference are accessible after completing exercises before them.';
      }
      elseif($cs = can_see($as,$task))
      {
        $ex = ex_inst($task);
        $ex->get_parameters();
        $ex->get_data();
        $co = $as->is_complete($task,$who_id);
        if((isset($submit_exercise) or isset($submit_to_teacher)) and is_array($answers))
        {
          if(isset($submit_to_teacher))
          {
            $res = mysql_query("select email from users where id=".$as->by['id']);
            $row = mysql_fetch_array($res);
            $email = $row['email'];

            $res = mysql_query("select name from users where id=$who_id");
            $row = mysql_fetch_array($res);
            $name = stripCzechChars(windows2iso($row['name']));

            mail($email,"Answers submitted by $name","
Student $name submitted answers for exercise\n     ".$ex->ex_name."

Correct on:
$server_addr/tuit/?section=correct&student=$who_id&assignment=".$as->id."&task=$task\n","From: $emails_from");
          }

          @$res = mysql_query("select an_id from answers where an_ex_id=$task and an_who_id=$who_id and an_as_id=$assignment");
          @$row = mysql_fetch_array($res);
          $old_an_id=$row['an_id'];
          ksort($answers);
          $ex->set_answers($answers);
          @$res = mysql_query("replace into answers set an_ex_id=$task, an_who_id=$who_id, an_as_id=$assignment");
          @$an_id = mysql_insert_id();
          if($an_id)
          {
            if(in_array($ex->parameters['ex_type'],array(QANDA,MATCH,CHOOSE)))
            {
              $dbn = array(QANDA=>'an_qanda',MATCH=>'an_match',CHOOSE=>'an_choose');
              $dbc = array(QANDA=>'aq_an_id',MATCH=>'am_an_id',CHOOSE=>'ac_an_id');
              $sql = "insert into ".$dbn[$ex->parameters['ex_type']]." values ";
              $cnt = 0;
              while(list($k,$v) = each($answers))
              {
                if($ex->parameters['ex_type']==MATCH and $v==$cnt)
                  $cor = 1;
                elseif($ex->parameters['ex_type']==CHOOSE and $v==$ex->correct[$cnt])
                  $cor = 1;
                elseif($ex->parameters['ex_type']==QANDA and $v==$ex->sample_answers[$cnt])
                  $cor = 1;
                else
                  $cor = 0;
                if($ex->parameters['ex_type']==QANDA)
                  $v = "'$v'";
                $sql .= ($cnt++?',':'')."(NULL,$an_id,$v,$cor)";
              }
              if($old_an_id)
                mysql_query("delete from ".$dbn[$ex->parameters['ex_type']]." where ".$dbc[$ex->parameters['ex_type']]."=$old_an_id");
            }
            elseif($ex->parameters['ex_type']==COMPLETE)
            {
              if($old_an_id)
                mysql_query("delete from an_complete where aco_an_id=$old_an_id");
              $ex->set_radios_ans($radios);
              $sql = "insert into an_complete values ";
              $c = '';
              if(is_array($answers))
              {
                $ans_s = serialize($answers);
                $sql .= "(NULL,$an_id,'".addslashes($ans_s)."',".ANSWERS.",'','')";
                $c = ',';
              }

              if(is_array($radios))
              {
                $rad_s = serialize($radios);
                $sql .= "$c(NULL,$an_id,'".addslashes($rad_s)."',".RADIOS.",'','')";
              }
            }
            elseif($ex->parameters['ex_type']==CROSSWORD)
            {
              if($old_an_id)
                mysql_query("delete from an_crossword where acr_an_id=$old_an_id");
              $answers['solution']=$translation;
              $ex->set_answers($answers);
              if(!$ex->ready or !is_array($ex->letters) or !$ex->m or !$ex->n)
                $ex->parse_story();

              $cnt = 0;
              $correct = array();
              for($i=0;$i<$ex->m;$i++)
              {
                for($j=0;$j<$ex->n;$j++)
                {
                  if($ex->letters[$i][$j]=='-')
                    continue;
                  if($ex->letters[$i][$j]==$ex->answers[$cnt])
                    $correct[$cnt]=1;
                  $cnt++;
                }
              }
              if($translation==$ex->translation)
                $translation_correct=1;
              else
                $translation_correct=0;
              $cor_cnt = count($correct);
              $sql = "insert into an_crossword values (NULL,$an_id,'".addslashes(serialize($answers))."','".addslashes(serialize($correct))."',$translation_correct)";
            }
            @$res2 = mysql_query($sql);
          }
          if(!$res or !$res2)
            echo "Error saving data!";
          else # check if all tasks are finished, then update aw_complete..
          {
            @$res = mysql_query("select count(at_task_id) as cnt from as_tasks,exercises where at_task_id=ex_id and at_as_id=$assignment and ex_type!=".EXPLANATION."");
            @$row = mysql_fetch_array($res);
            $cnt = $row['cnt'];
            @$res = mysql_query("select count(an_id) as cnt from answers where an_who_id=$who_id and an_as_id=$assignment");
            @$row = mysql_fetch_array($res);
            $cnt2 = $row['cnt'];
            if($cnt == $cnt2)
            {
              @mysql_query("update as_who set aw_complete=1 where aw_as_id=$assignment and aw_who_id=$who_id");
            }
            $as->read_complete_list($who_id);
          }
        }
        /*
         * get list of explanations to this exercise (of the same categories)
         * and add them to the top navigation..
         */
        $expls = $ex->get_explanations();
        $nav_expl = new HMENU;
        $shown = $cnt = 0;
        reset($expls);
        while(list($k,$v) = each($expls))
        {
          if($cnt++<MAX_EXPL_IN_LIST)
          {
            $page->topnav->add_item($v['ex_name'],"?section=exercise_display&exercise=$v[ex_id]");
            $shown++;
          }
        }

        if(count($expls) > $shown)
          $page->topnav->add_item("more..","?section=exercise_display&exercise=".$ex->ex_id."&show_more=1");

        $cont = "<h4>".$ex->parameters['ex_name']."</h4>";
//        if($nav_expl->how_many())
//          $cont .= "Explanations: ".$nav_expl->prepare();
//        while(list($k,$v) = each($expls))
        if($as->is_complete($task,$who_id))
        {
          if(!count($ex->answers))
            $ex->read_answers($who_id,$as->id);
        //  else
        //    $show_an = 1;
          $cont .= "Your answers";
        }

        if(isset($submit_to_teacher))
          $cont .= "<br>.. sent to teacher";

        $cont2->add_row(array("<span class=\"\"><form action=\"index.php\" method=\"post\">
        <input type=\"hidden\" name=\"section\" value=\"exercises\">
        <input type=\"hidden\" name=\"assignment\" value=\"$assignment\">
        <input type=\"hidden\" name=\"task\" value=\"$task\">".$ex->prepare_test($show_an)."<br><br>".(($ex->parameters['ex_type']!=EXPLANATION/* and !$as->is_complete($task,$who_id)*/)?"<center><input type=\"submit\" name=\"submit_exercise\" value=\"   Save answers   \" class=\"buttoninbox\">&nbsp;<input type=\"submit\" name=\"submit_to_teacher\" value=\"   Send to teacher   \" class=\"buttoninbox\"></center>":'')."</form></span>"));
        /* show name of the exercise in the top something */
        $page->set_top($ex->parameters['ex_name']);
      }
      $table = new TABLE;
      $table->set_parameters(array('width'=>'100%','cellpadding'=>10,'valign'=>'top','cellspacing'=>5));
      $table->add_row(array(array('attr'=>'width="20%" class="botbot"','cont'=>"<strong>$as->description</strong><br><br>".$as->by['name']."<br>$as->when"),array('attr'=>'width="80%" valign="top" class="exnav"','cont'=>"<center>$cont</center>")));
      $list = new VMENU;
      $list->set_itemized(0);
      reset($as->task_list);
      while(list($k,$v) = each($as->task_list))
      {
        $cs = can_see($as,$v);
        $ic = $as->is_corrected($v,$who_id);
        //if(can_see($as,$v))
          #$list->add_item(((can_see($as,$v) and !$as->is_complete($v,$udata['id']))?'':'<li>').$tpar[$v]['name'],(can_see($as,$v) and !$as->is_complete($v,$udata['id']))?"?section=exercises&assignment=$as->id&task=$v":'',($as->is_complete($v,$udata['id'])?"&nbsp;<font color=\"#FF0000\"><small>(Complete)</small></font>":''));
        $list->add_item((($cs and !$ic)?'':'').$tpar[$v]['name']."<br>",($cs and !$ic)?"?section=exercises&assignment=$as->id&task=$v$h_ap":'',$ic?"&nbsp;<font color=\"#FF0000\"><small><a href=\"?section=results&assignment=$as->id&task=$v$h_ap\" style=\"color: #FF0000;\">(Review)</a></small></font>":'');
      }
      $table->add_row(array(array('attr'=>'class="exnav"','cont'=>$list->prepare()),array('attr'=>'class="" align="center"','cont'=>$cont2->prepare())));
      $page->add_to_middle($table->prepare());
    }
  }
/* End of Student section */
}
/* Teacher (admin?) section */
/*
else
{
  if(is_numeric($exercise))
  {
    if(isset($submit))
    {
      if(is_array($answers))
      {
        print_r($answers);
        echo ": ";
        echo $ans_s=serialize($answers)."<br>\n";
        print_r(unserialize($ans_s));
        echo "<hr>\n";
      }
      if(is_array($radios))
      {
        echo $rad_s=serialize($radios)."<br>\n";
        print_r(unserialize($rad_s));
        echo "<br>\n";
      }
    }
    $tab = new BOXEDTABLE;
    $tab->set_parameters(array('style'=>'subtable','all_width'=>''));
    $ex = ex_inst($exercise);
    $tab->add_row(array($ex->prepare_test()));
    $tab->add_row(array("<input type=\"submit\" name=\"submit\" class=\"buttoninbox\">"));
    $page->set_top($ex->parameters['ex_name']);
    $page->add_to_middle("<form action=\"index.php\" method=\"get\" name=\"ex_form\"><input type=\"hidden\" name=\"section\" value=\"exercises\"><input type=\"hidden\" name=\"exercise\" value=$exercise><center>\n".$tab->prepare()."</center></form>");
  }
  else
  {
    @$res = mysql_query("select * from exercises order by ex_type,ex_name");
    while(@$row = mysql_fetch_array($res))
      $page->add_to_middle("<a href=\"?section=exercises&exercise=$row[ex_id]\">$row[ex_name] ($row[ex_type])</a><br>\n");

    $page->set_top('Exercises preview');
  }
}
*/

return 123;
?>
