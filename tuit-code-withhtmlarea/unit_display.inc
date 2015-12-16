<?
include('un_classes.inc');

if(!is_numeric($unit))
  return 123;
if(!is_numeric($uns))
  $uns = 1;

if(isset($submit_section))
{
  #readable($answers);
  #readable($input_offset);
}

$un = new UNIT($unit);
$un->get_sections();
if(!is_array($un->sections[$uns]))
  return;
$un->read_data();
$page->set_top($un->name.": ".$un->sections[$uns]['name']);
$page->add_to_middle("<form action=\"index.php\" method=\"post\">
<input type=\"hidden\" name=\"section\" value=\"unit_display\">");
$page->add_to_middle($un->show_section($uns));
if(is_array($un->sections[$uns+1]))
{
  $page->add_to_middle("<br><br>
  <input type=\"hidden\" name=\"unit\" value=\"$unit\">
  <input type=\"hidden\" name=\"uns\" value=\"".($uns+1)."\">
  <input type=\"submit\" name=\"submit_section\" value=\"  Next part  \" class=\"buttoninbox\">");
}

if(is_array($MODULES))
  $href="tuit/";

$page->topnav->add_item("For printing",$href."?section=print_exercise&unit=$unit&uns=$uns");
$page->add_to_middle("</form>");

return 123;
?>
