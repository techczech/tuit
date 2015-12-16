<?

include('ex_classes.inc');
#error_reporting(2047);

function show_expls($nav_expl,$expls,$ex)
{
  $cont = '';
  reset($expls);
  while(list($k,$v) = each($expls))
    $nav_expl->add_item($v['ex_name'],"?section=exercise_display&exercise=$v[ex_id]");

  //$cont = "<h4>".$ex->parameters['ex_name']."</h4>";
  if($nav_expl->how_many())
  {
    if($ex->parameters['ex_type']==EXPLANATION)
      $cont .= "Exercises: ";
    else
      $cont .= "Explanations: ";
    $cont .= $nav_expl->prepare();
  }
  return $cont;
}

function save_answers($ex,$ans)
{
  global $udata;
  if(!$ex->ex_id or !$udata['id'])
    return -1;
  $who_id  = $udata['id'];
  $answers = $ans['answers'];
  $radios  = $ans['radios'];

  ksort($answers);
  $ex->set_answers($answers);
  @$res = mysql_query("replace into answers set an_ex_id=$ex->ex_id, an_who_id=$who_id, an_as_id=0");
  @$an_id = mysql_insert_id();
  if($an_id)
  {
    if(in_array($ex->parameters['ex_type'],array(QANDA,MATCH,CHOOSE)))
    {
      $dbn = array(QANDA=>'an_qanda',MATCH=>'an_match',CHOOSE=>'an_choose');
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
    }
    elseif($ex->parameters['ex_type']==COMPLETE)
    {
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
      $answers['solution']=$ans['translation'];
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
}

if(!is_numeric($exercise))
  return 123;

$ex = ex_inst($exercise);
$ex->get_parameters();
$expls = $ex->get_explanations();
$page->set_top($ex->parameters['ex_name']);
$show_an = 0;

if($show_more)
{
  $nav_expl = new VMENU;
  $cont = show_expls($nav_expl,$expls,$ex);
  $page->add_to_middle($cont);
}
else
{
  $ex->get_data();

  $ex->read_answers($udata['id'],0);
  if(count($ex->answers))
  {
    $page->add_to_middle("<h3>Your answers</h3>\n");
    $show_an = 1;
  }
  
  if(isset($submit_answers))
  {
    ksort($answers);
    $ex->set_answers($answers);
    $ex->set_radios_ans($radios);
    save_answers($ex,array('answers'=>$answers,'radios'=>$radios,'translation'=>$translation));
    $page->insert_to_middle("<h3>Your answers</h3>\n");
    $show_an = 1;
  }

  if(is_array($MODULES))
    $href="tuit/";
  $page->topnav->add_item("For printing",$href."?section=print_exercise&exercise=".$ex->ex_id);

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

  if($ex->parameters['ex_type']==EXPLANATION)
    $page->add_to_middle($ex->prepare_test(0));
  else
  {
    $page->add_to_middle("<form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"exercise_display\">
    <input type=\"hidden\" name=\"exercise\" value=\"$ex->ex_id\">".
    $ex->prepare_test($show_an,1)."
    <br><br>");
    if(!$show_an)
      $page->add_to_middle("
    <center>
    <input type=\"submit\" name=\"submit_answers\" value=\"   Submit answers   \" class=\"buttoninbox\">
    </center>\n");

    $page->add_to_middle("</form>");
  }
}

return 123;
?>
