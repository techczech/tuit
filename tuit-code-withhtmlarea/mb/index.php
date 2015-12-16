<?

#error_reporting(2047);

$inc_path = "../inc";
include("$inc_path/mi_classes.inc");
include("$inc_path/mi_functions.inc");
include("$inc_path/pre.inc");

$mb_page = '';

if(!$wrapped)
{
  include("$inc_path/lookandfeel.inc");
  $inc_path='.';
}
else
  $inc_path=$inc_path_wrap;

if(!$admin or $HTTP_GET_VARS['admin'] or $HTTP_POST_VARS['admin'])
  $admin = 0;

if(empty($mb_section))
  $mb_section='boards';

if( ereg('^[[:alnum:]_]+$',$mb_section) )
{
  @$r = include("$inc_path/$mb_section.inc");
  if($r != 123)
    $mb_page_body='The page you requested does not exist.';
}
else
  $mb_page_body='The page you requested does not exist.';

if(!$wrapped)
{
  $mb_page .= "<html>
  <head>
  <meta http-equiv=\"Content-language\" content=\"cs\">
  <meta http-equiv=\"Content-type\" content=\"text/html; charset=windows-1250\">
  <title>$mb_page_title $mb_page_top</title>
  $style_in_head
  </head>
  <body>
  $mb_page_body
  </body>\n</html>\n";
  echo $mb_page;
}
else
  $mb_page=$mb_page_body;

?>
