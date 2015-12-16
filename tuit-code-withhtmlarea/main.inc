<?

/*
   main menu..
   items in the menu are displayed according to user's permissions..
   permission checks must be also included in each section..
*/

#error_reporting(2047);

if(!$udata["d_now"])
  $page->set_top('Main menu');

$menu = new VMENU;
$menu->set_style('middle');
$menu->set_itemized(0);

$table = new TABLE;
$table->set_parameters(array('style'=>'middle'));

if(is_cont_admin($udata['admin']))
{
  $cmenu = new VMENU;
  $cmenu->set_itemized(2);
  $cmenu->set_style('middle');
  $cmenu->add_items(array(
  /*
  ''=>'',*/
  'Create new exercise or unit'=>'?section=create',
  'View/Edit exercises'=>'?section=edit&edit=exercises',
  'View/Edit units'=>'?section=edit&edit=sets',
  'Dictionary'=>'?section=dictionary',
  'Specialist dictionaries'=>'?section=specdict'));
  if(is_super_admin($udata['admin']))
    $cmenu->add_item('Dbedit','?section=dbedit');
  $menu->add_items(array(
  '<strong>Contents manager</strong>'=>'',
  #'View or edit prepared exercises'=>'?section=edit&show_menu=exercises',
  #'View or edit prepared units'=>'?section=edit&show_menu=units',
  $cmenu->prepare()=>'',
  ''=>''));
}
elseif(is_teacher($udata['admin']))
  $menu->add_items(array('View prepared exercises/units'=>'?section=exercises',''=>''));

if(is_teacher($udata['admin']))
{
  $tmenu = new VMENU();
  $tmenu->set_itemized(2);
  $tmenu->set_style('middle');
  $tmenu->add_items(array(
  'Define teaching plans<br>Assign tasks to students'=>'?section=assign',
  'Correct/Review students\' exercises'=>'?section=correct',
//  'Review students\' results'=>'?section=results',
  'Send comments by email'=>'?section=comments',
  ''=>''));
  $menu->add_items(array(
  '<strong>Students manager</strong>'=>'',
  $tmenu->prepare()=>'',
  ''=>''));
}
/*
if(is_cont_admin($udata['admin']))
{
  $menu->add_items(array(
  'Contents manager'=>'?section=contents',
  'Layout manager'=>'?section=layout',
  ''=>''));
}
*/
if(is_acct_admin($udata['admin']))
{
  $umenu = new VMENU;
  $umenu->set_itemized(2);
  $umenu->set_style('middle');
  $umenu->add_items(array(
  'Process user accounts'=>'?section=users',
  'User categories' => '?section=user_categories',
  'Process account requests'=>'?section=acct_approve'));
  $menu->add_items(array('<strong>Users administration</strong>'=>'',/*''=>'',*/$umenu->prepare()=>''));
}

if(!$udata['admin'])
{
  $menu->add_items(array('Assigned exercises'=>'?section=exercises',
  'See your results'=>'?section=exercises&show_complete=1',
  ''=>'',
  'Vocabulary'=>'?section=dictionary',
  'Notepad'=>'?section=notepad&wrapped=1'));
  $menu->add_item('','');
}

$menu->add_items(array('Change your settings'=>'?section=change_udata',
''=>''));

$menu->add_items(array('Message boards'=>'?section=mb',''=>''));
$menu->add_items(array('<strong>Logout</strong>'=>'?section=logout'));

$table->add_row(array(array('attr'=>'width="50%"','cont'=>$menu->prepare()),
  array('attr'=>'width="50%"','cont'=>"<center>
  Enter any comments here, they will be sent to the <a href=\"mailto:$admin_email\">administrator</a>.<br><br>
  <form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"comments\">
  <textarea rows=10 cols=40 name=\"comment\" class=\"largetext\"></textarea><br><br>
  <input type=\"submit\" value=\"   Send   \" name=\"mainmenu\" class=\"buttoninbox\">
  </form>
  </center>\n")));

$page->add_to_middle($table->prepare());

return 123;
?>
