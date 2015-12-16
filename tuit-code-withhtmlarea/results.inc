<?

include('ex_classes.inc');
include('as_base.inc');

/* Student section */
if(!$udata['admin'])
{
  if(is_numeric($assignment))
  {
    $as = new ASSIGNMENT($assignment);
    if(is_numeric($task))
    {
      $ex = ex_inst($task);
      $page->set_top("Answers: ".$ex->parameters['ex_name']);
      $tabl = new TABLE;
      $ex->read_answers($udata['id'],$assignment);
      $tabl->add_row(array($ex->prepare_test(1,0,$udata['id'],$assignment,$ex->is_corrected)));
      if($ex->is_corrected)
      {
        $tabr = new TABLE;
        $tabr->set_parameters(array('cellspacing'=>5));
        $tabr->add_row(array("Corrected by: <strong>".$ex->corrected['name'].", ".$ex->corrected['when']."</strong><br><br>Comments:<br><strong>".nl2br($ex->corrected['comment'])."</strong><br><br>Points:<br><strong>".$ex->corrected['points']."</strong>"));
        $tab = new TABLE;
        $tab->set_parameters(array('cellpadding'=>5));
        $tab->add_row(array(array('attr'=>'valign="top"','cont'=>$tabl->prepare()),array('attr'=>'valign="top" class="subtable"','cont'=>$tabr->prepare())));
        $page->add_to_middle($tab->prepare()."<br><br><a href=\"javascript:history.go(-1);\">Back</a>");
      }
      else
      {
        $page->add_to_middle("<center>".$tabl->prepare()."<br><br><a href=\"javascript:history.go(-1);\">Back</a></center>");
      }
    }
  }
}
/* end of student section */
/* Teacher section */
elseif(is_teacher($udata['admin']))
{
  if(is_numeric($assignment) and is_numeric($student))
  {
    $as = new ASSIGNMENT($assignment);
    if(is_numeric($task))
    {
      $ex = ex_inst($task);
      $page->set_top("Answers: ".$ex->parameters['ex_name']);
      $tabl = new TABLE;
      $ex->read_answers($student,$assignment);
      $tabl->add_row(array($ex->prepare_test(1,0,$student,$assignment,$ex->is_corrected)));
      if($ex->is_corrected)
      {
        $tabr = new TABLE;
        $tabr->set_parameters(array('cellspacing'=>5));
        $tabr->add_row(array("Corrected by: <strong>".$ex->corrected['name'].", ".$ex->corrected['when']."</strong><br><br>Comments:<br><strong>".nl2br($ex->corrected['comment'])."</strong><br><br>Points:<br><strong>".$ex->corrected['points']."</strong>"));
        $tab = new TABLE;
        $tab->set_parameters(array('cellpadding'=>5));
        $tab->add_row(array(array('attr'=>'valign="top"','cont'=>$tabl->prepare()),array('attr'=>'valign="top" class="subtable"','cont'=>$tabr->prepare())));
        $page->add_to_middle($tab->prepare()."<br><br><a href=\"javascript:history.go(-1);\">Back</a>");
      }
      else
      {
        $page->add_to_middle("<center>".$tabl->prepare()."<br><br><a href=\"javascript:history.go(-1);\">Back</a></center>");
      }
      
    }
  }


}

return 123;
?>
