<?
if(is_cont_admin($udata['admin']) or is_super_admin($udata['admin']))
  $admin = 1;

if($udata)
  $logged_in=1;

$author = $udata['name'];
$email = $udata['email'];

$wrapped = 1;
$inc_path_wrap='mb';

include('mb/index.php');
$page->set_title($mb_page_title);
$page->set_top("<strong>$mb_page_top</strong>");
$page->add_to_middle("<center>$mb_page</center>");

return 123;
?>
