<?

require("Catalog.class.php");
define('MIN_TO_COUNT',11);

$meMod = 'catalog';

$MODULES[$meMod]['head'] .= "
<script language=\"Javascript\">
<!--

function confirmDelete(url)
{
  var yes = confirm('Are you sure you want to delete this element?');
  if(yes)
    return url;
  else
    return '#';
}

-->
</script>
";

$catSecNum = $MODULES['argv']['s'];
$artToShow = $MODULES['argv']['a'];

if(!is_numeric($catSecNum) or $catSecNum<1)
  $catSecNum = 1;

if(!is_numeric($artToShow))
  unset($artToShow);
else
{
  $artToShow = new Article($artToShow);
  findLinkedArticle($artToShow);
}

$catSection = new Section($catSecNum);
findLinkedSection($catSection);

$MODULES[$meMod]['title'] = $catSection->title;
$MODULES[$meMod]['topright'] = $MODULES[$meMod]['title'];
$MODULES[$meMod]['logo'] = $catSection->logo;

$myMain = &$MODULES[$meMod]['mainright'];

$mySubs = &$catSection->readSubsDataDB();
$myArts = &$catSection->readArtsDataDB();

if(!$MODULES['argv']['do_admin'])
{
  $catSection->readTuitContentsDB();
  $catSection->readTextBoxesDB();
}

if( (count($mySubs)+count($myArts) )>=MIN_TO_COUNT)
  $doCnt = 1;

if(!$artToShow and !$MODULES['argv']['do_admin'])
{
  $myMain .= "
  <table>
  <tr valign=\"top\">
  <td>
    <table cellspacing=2 cellpadding=5>
    ".$catSection->allElementsHTML($doCnt);

  $myMain .= "
    <tr>
    <td colspan=2>
      <a href=\"?m=$meMod&do_admin=1&act=section&s=1\">Edit main section</a><br>
      <a href=\"?m=$meMod&do_admin=1&act=section&pre=$catSecNum\">Add subsection</a><br>
      <a href=\"?m=$meMod&do_admin=1&act=article&s=$catSecNum\">Add article</a><br>
      <a href=\"?m=$meMod&do_admin=1&act=textbox&s=$catSecNum\">Add textbox</a><br>
      <a href=\"?m=$meMod&do_admin=1&act=xoftheday&s=$catSecNum\">Add X of the day</a><br>
      <a href=\"?m=$meMod&do_admin=1&act=linx&s=$catSecNum\">Add link</a><br>
      <a href=\"?m=$meMod&do_admin=1&act=tuit&add=exercise&s=$catSecNum\">Add exercise</a><br>
      <a href=\"?m=$meMod&do_admin=1&act=tuit&add=unit&s=$catSecNum\">Add unit</a>
    </td>
    </tr>\n";
  
  $myMain .= "
  <tr>
  <td colspan=2>\n";

//  $myMain .= $catSection->textBoxesHTML();
  $myLinx = new LinkList($catSection->id);
  $myMain .= $myLinx->linxHTML();

  $myMain .= "
  </td>
  </tr>
  </table>
  </td>\n";

  $catSection->readXOfTheDaysDataDB();
  if(count($catSection->xOfTheDays) or $catSection->desc)
  {
    $myMain .= "
    <td class=\"xoftheday\">";
    if($catSection->desc)
      $myMain .= $catSection->sectionDescriptionHTML();
    if(count($catSection->xOfTheDays))
      $myMain .= $catSection->xOfTheDaysHTML();
    $myMain .= "</td>\n";
  };
  $myMain .= "
  </tr>
  </table>\n";
}
elseif($artToShow and !$MODULES['argv']['do_admin'])
{
  $artToShow->increaseReaders();
  $myMain .= $artToShow->articleHTML();
}

if($artToShow)
  $up = $artToShow->section;
else
  $up = $catSection->previous;
$myLeft = &$MODULES[$meMod]['mainleft'];
$myLeft .= "<div class=\"mainleft\">\n";
$myLeft .= $catSection->leftMenuHTML();
$myLeft .= "<tr><td>&nbsp;</td></tr>
".($catSection->previous?("
<tr><td></td></tr>
<tr>
  <td><img src=\"images/bluebulletsquare.gif\" border=0 width=13 height=13></td>
  <td class=\"sectinfohead\"><span class=\"textsmall\"><a href=\"?m=catalog&s=$up\">Up</a></span></td>
</tr>
<tr><td></td></tr>"):'')."
</table>\n";
$myLeft .= "</div>\n";

$myMain .= "<a href=\"?m=configuration\">Configuration</a>";

?>
