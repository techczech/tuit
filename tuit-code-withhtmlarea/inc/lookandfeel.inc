<?
if(ereg('Gecko',$HTTP_USER_AGENT))
  $stylef = 'lookandfeel-mozilla.css';
elseif(ereg('MSIE',$HTTP_USER_AGENT))
  $stylef = 'lookandfeel-ie.css';
else
  $stylef = 'lookandfeel-ie.css';

if($section=='print_exercise')
  $stylef = "print-$stylef";

$style_in_head="
<link rel=STYLESHEET type=\"text/css\" href=\"styles/$stylef\" title=\"muj styl\">
\n";
?>
