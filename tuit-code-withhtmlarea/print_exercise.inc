<?

$page->disp_bohemica=0;

if(is_numeric($exercise))
{
  include('ex_classes.inc');
  $ex = ex_inst($exercise);
  $ex->get_parameters();
  $ex->get_data();
  $page->add_to_middle("<h2>".$ex->parameters['ex_name']."</h2>\n<br>".$ex->prepare_test(0));
}
elseif(is_numeric($unit))
{
  include('un_classes.inc');
  $un = new UNIT($unit);
  $un->get_sections();
  if(!is_numeric($uns))
    $uns = 1;
  if(!is_array($un->sections[$uns]))
    return;

  $un->read_data();
  $page->add_to_middle("<h2>".$un->name.": ".$un->sections[$uns]['name']."</h2>\n<br>".$un->show_section($uns));
}


return 123;
?>
