<?

include('Catalog.class.php');
$catSection = new Section();
$catSection->findByName('Czech Course');
$myLeft = &$MODULES['tuit']['mainleft'];
$myLeft .= "<div class=\"mainleft\">\n";
$myLeft .= $catSection->leftMenuHTML();
$myLeft .= "<tr><td></td></tr></table>\n</div>\n";
//if($MODULES['argv']['section']=='login' or $MODULES['argv']['section']=='acct_request' or !empty($_COOKIE['bisquit']) and $_COOKIE['bisquit']!='***')
  include("tuit/index.php");
//else
//  include("tuit/public_access.php");

$page->disp_bohemica=0;
$MODULES['tuit']['mainright'] = $page->display($section=='notepad');
$MODULES['tuit']['topright'] = $page->top_text;
$MODULES['tuit']['topmenu'] = $page->topnav->prepare();
$MODULES['tuit']['bottommenu'] = $page->botnavleft->prepare();

?>
