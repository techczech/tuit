<?

require("Catalog.class.php");
define('MIN_TO_COUNT',13);

$meMod = 'catalog';
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
$catSection->readTextBoxesDB();
$catSection->readTuitContents();
if( (count($mySubs)+count($myArts) )>=MIN_TO_COUNT)
  $doCnt = 1;

if(!$artToShow)
{
  $myMain .= "
  <table>
  <tr valign=\"top\">
  <td>
    <table cellspacing=2 cellpadding=5>
    ".$catSection->allElementsHTML($doCnt);

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
else
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

?>
