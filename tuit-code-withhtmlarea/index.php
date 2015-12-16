<?

/************************* Language Course Online ****************************/
/*                                                                           */
/*                      version 0.2.1, september 2001                        */
/*                                                                           */
/* written from a scratch, resembles previous system now called version 0.1, */
/* which was perl-based with no external database support. version 0.2 is    */
/* entirely based on PHP and MySQL, except for a few soon-to-be-written      */
/* scripts for transplatation of v0.1 files..                                */
/*                                                                           */
/*****************************************************************************/


error_reporting(0);
#error_reporting(2047);

$inc_path = "inc";

include("$inc_path/pre.inc");
include("$inc_path/classes.inc");
include("$inc_path/base.inc");

$page = new PAGE('TuiT Demo');

include("$inc_path/lookandfeel.inc");

$page->add_to_head($style_in_head);

/*
   compare the contents of received cookie with database and if both
   are the same, then treat user as already logged. the IP is also compared
   for 'safety'..
   if we don't get a cookie or get '***' (which we send after logout),
   user can only see login and acct_request sections..
*/

if(isset($udata))
  $udata = array();

if(isset($bisquit) and strcmp($bisquit,"***"))
{
  @$res = mysql_query("select id,login,name,email,send_note,admin from users where ip='$REMOTE_ADDR' and cookie='$bisquit'");
  if( !(@$udata = mysql_fetch_array($res)) )
    $section = 'login';
  else
  {
    $page->set_logged(1);
    @$res2 = mysql_query("select uc_cou_id from user_categories where uc_u_id='".$udata['id']."'");
    while(@$row2 = mysql_fetch_array($res2))
      $MY_CATEGORIES[$row2['uc_cou_id']] = $row2['uc_cou_id'];
  }

  if(empty($section) and is_numeric($exercise))
    $section = 'exercise_display';
  elseif(empty($section) and is_numeric($unit))
    $section = 'unit_display';
  if( empty($section) )
    $section = 'main';
}
elseif(!isset($section) and is_numeric($exercise) or is_numeric($unit))
  $section = 'public_access';
elseif(strcmp($section,'acct_request') and strcmp($section,'mb') and strcmp($section,'public_access'))
  $section = 'login';


/*
   each section has its own include file, which must return 123, this
   tells this main script that everything went fine. otherwise displays
   error. section is a parameter set as section=name_of_section parameter
   in url or as hidden field in forms..
*/

if(ereg('^[[:alnum:]_]+$',$section))
{
  $r = include("$section.inc");
  if($r != 123)
    error_fl('The page you requested does not exist.');
}
else
  error_fl('The page you requested does not exist.');

if($GLOBALS['m']=='tuit')
{
}
else
{
  $out = $page->display($section=='notepad');
  if(preg_match('/--INSERTEWCONTROLHERE--/',$out))
  {
    list($pre,$post) = explode('<!--INSERTEWCONTROLHERE-->',$out);
    echo $pre;
    eval("$".$INSERTEWCONTROL."->ShowControl('800','400');");
    echo $post;
  }
  else
    echo $out;
}
?>
