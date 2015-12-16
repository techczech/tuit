<?

//require("$mod/Catalog.class.php");

function readXOfTheDayIDs()
{
  global $SQL;
  $res = $SQL->query("select ".DB_XOTD_ID.",".DB_XOTD_NAME." from ".DB_XOTD_TABLE." order by ".DB_XOTD_NAME);
  while($row = $SQL->fetchArray($res))
    $xs[$row[DB_XOTD_NAME]] = $row[DB_XOTD_ID];

  return $xs;
}

$actions = array
(
  'article'  =>'Articles',
  'section'  =>'Sections',
  'textbox'  =>'Text box',
  'xoftheday'=>'X of the day'
);

$action = $MODULES['argv']['act'];
$catSecNum = $MODULES['argv']['s'];
$artNum = $MODULES['argv']['a'];
$miscId = $MODULES['argv']['i'];

$MODULES[$meMod]['title'] = 'Admin';
$MODULES[$meMod]['topright'] = $MODULES[$meMod]['title'];

$myMain = &$MODULES[$meMod]['mainright'];

switch($action)
{
  case 'section':
    if(!is_numeric($catSecNum) or $catSecNum<1)
    {
      unset($catSecNum);
      $catSection = new Section();
      $catSection->previous = $MODULES['argv']['pre'];
    }
    else
      $catSection = new Section($catSecNum);

    if($MODULES['argv']['do_delete']==1)
    {
      $catSection->deleteFromDB();
      header("Location: index.php?s=".$catSection->previous."\n");
      break;
    }

    if(!empty($MODULES['argv']['do_save']))
    {
      if(!$MODULES['argv']['S_PRIORITY'])
        $MODULES['argv']['S_PRIORITY'] = DEFAULT_PRIORITY;

      if(!$MODULES['argv']['S_SUBPRIORITY'])
        $MODULES['argv']['S_SUBPRIORITY'] = DEFAULT_PRIORITY;
      $catSection->name = $MODULES['argv']['S_NAME'];
      $catSection->title = $MODULES['argv']['S_TITLE'];
      $catSection->short_desc = $MODULES['argv']['S_SHORT_DESC'];
      $catSection->desc = $MODULES['argv']['S_DESCRIPTION'];
      $catSection->previous = $MODULES['argv']['S_PREVIOUS'];
      $catSection->priority = $MODULES['argv']['S_PRIORITY'];
      $catSection->subpriority = $MODULES['argv']['S_SUBPRIORITY'];
      $catSection->logo = $MODULES['argv']['S_LOGO'];
      $catSection->url = $MODULES['argv']['S_URL'];
      $opt = 0;
      while(list($k,$v) = each($MODULES['argv']['S_OPTIONS']))
        $opt |= $v;
      $catSection->options = $opt;
      $catSection->writeDataDB();
      header("Location: index.php?s=".$catSection->previous."\n");
    }
    elseif(!$catSection->ready)
      $catSection->readDataDB();
    $myMain .= $catSection->editForm();
    break;
  case 'article':
    if(!is_numeric($artNum))
    {
      unset($article);
      $article = new Article();
      $article->section = $catSecNum;
    }
    else
      $article = new Article($artNum);

    if($MODULES['argv']['do_delete']==1)
    {
      $article->deleteFromDB();
      header("Location: index.php?s=".$article->section."\n");
      break;
    }

    if(!empty($MODULES['argv']['do_save']))
    {
      if(!$MODULES['argv']['A_PRIORITY'])
        $MODULES['argv']['A_PRIORITY'] = DEFAULT_PRIORITY;
      $article->name   = $MODULES['argv']['A_NAME'];
      $article->author = $MODULES['argv']['A_AUTHOR'];
      $article->perex  = $MODULES['argv']['A_PEREX'];
      $article->text   = $MODULES['argv']['A_TEXT'];
      $article->priority = $MODULES['argv']['A_PRIORITY'];
      $article->section = $MODULES['argv']['A_SECTION'];
      $article->writeDataDB();
      header("Location: index.php?s=".$article->section."\n");
    }
    elseif(!$article->ready)
      $article->readDataDB();
    $myMain .= $article->editForm();
    break;
  case 'textbox':
    if(!is_numeric($miscId))
    {
      $textbox = new TextBox();
      $textbox->section = $catSecNum;
    }
    else
      $textbox = new TextBox($miscId);

    if($MODULES['argv']['do_delete']==1)
    {
      $textbox->deleteFromDB();
      header("Location: index.php?s=".$textbox->section."\n");
      break;
    }

    if(!empty($MODULES['argv']['do_save']))
    {
      if(!$MODULES['argv']['T_PRIORITY'])
        $MODULES['argv']['T_PRIORITY'] = DEFAULT_PRIORITY;
      $textbox->header  = $MODULES['argv']['T_HEADER'];
      $textbox->text    = $MODULES['argv']['T_TEXT'];
      $textbox->section = $MODULES['argv']['T_SECTION'];
      $textbox->type    = $MODULES['argv']['T_TYPE'];
      $textbox->priority = $MODULES['argv']['T_PRIORITY'];
      $textbox->writeDataDB();
      header("Location: index.php?s=".$textbox->section."\n");
    }
    elseif(!$textbox->ready)
      $textbox->readDataDB();
    $myMain .= $textbox->editForm();
    break;
  case 'xoftheday':
    if(!empty($MODULES['argv']['do_save']) and is_numeric($catSecNum))
    {
      $SQL->query("delete from ".DB_XOTD_SECTIONS_TABLE." where ".DB_XOTD_SECTIONS_SECTION_ID."=$catSecNum");
      $ls = '';
      $cnt = 0;
      while(list($k,$v) = each($MODULES['argv']['XS']))
        $ls .= ($cnt++?',':'')."($catSecNum,$v,'".$MODULES['argv']['XP'][$k]."')";

      $SQL->query("insert into ".DB_XOTD_SECTIONS_TABLE." values $ls");
      header("Location: index.php?s=$catSecNum");
    }
    if(is_numeric($catSecNum))
      $catSection = new Section($catSecNum);

    $catSection->readXOfTheDaysDB();
    $xs = readXOfTheDayIDs();
    $myMain .= $catSection->addXOfTheDayForm($xs);

    break;
  case 'linx':
    if(!is_numeric($catSecNum))
      $linx = new LinkList();
    else
      $linx = new LinkList($catSecNum);

    if($MODULES['argv']['do_delete'] and $miscId)
    {
      $SQL->query("delete from ".DB_LINX_TABLE." where ".DB_LINX_ID."=$miscId");
      header("Location: index.php?s=".$linx->section."\n");
      break;
    }

    if(!empty($MODULES['argv']['do_save']))
    {
      if($miscId)
      {
        $linx->linx[$miscId][DB_LINX_ID] = $miscId;
        $linx->linx[$miscId][DB_LINX_URL] = $MODULES['argv']['L_URL'];
        $linx->linx[$miscId][DB_LINX_NAME] = $MODULES['argv']['L_NAME'];
        $linx->linx[$miscId][DB_LINX_DESCRIPTION] = $MODULES['argv']['L_DESCRIPTION'];
        $linx->section = $MODULES['argv']['L_SECTION'];
        $linx->writeDataDB($miscId);
      }
      else
        $linx->addLinkDB($MODULES['argv']['L_URL'],$MODULES['argv']['L_NAME'],$MODULES['argv']['L_DESCRIPTION'],$MODULES['argv']['L_SECTION']);
    }
    elseif(!$linx->ready)
      $linx->readDataDB($miscId);
    $myMain .= $linx->editForm($miscId);
    break;
  case 'tuit':
    //error_reporting(2047);
    if(is_numeric($catSecNum))
      $catSection = new Section($catSecNum);

    if($MODULES['argv']['do_delete'] and is_numeric($catSecNum))
    {
      $catSection->readTuitContentsDB();
      if(is_numeric($MODULES['argv']['e']))
        $catSection->removeTuitElement("e: ".$MODULES['argv']['e']);
      elseif(is_array($MODULES['argv']['e']))
        $catSection->removeTuitElements($MODULES['argv']['e']);

      if(is_numeric($MODULES['argv']['u']))
        $catSection->removeTuitElement("u: ".$MODULES['argv']['u']);
      elseif(is_array($MODULES['argv']['u']))
        $catSection->removeTuitElements($MODULES['argv']['u']);

      $catSection->writeTuitContentsDB();
      header("Location: index.php?s=$catSecNum");
      break;
    }
    elseif(!empty($MODULES['argv']['do_add']) and is_numeric($catSecNum))
    {
      $catSection->readTuitContentsDB();
      if(is_array($MODULES['argv']['e']))
      {
        while(list($k,$v) = each($MODULES['argv']['e']))
        {
          $catSection->tuits[$catSection->tuitsCnt] = array('type'=>'e','id'=>$v);
          $catSection->tuitsCnt++;
        }
      }
      elseif(is_array($MODULES['argv']['u']))
      {
        while(list($k,$v) = each($MODULES['argv']['u']))
        {
          $catSection->tuits[$catSection->tuitsCnt] = array('type'=>'u','id'=>$v);
          $catSection->tuitsCnt++;
        }
      }
      $catSection->writeTuitContentsDB();
      header("Location: index.php?s=$catSecNum");
    }
    elseif(!empty($MODULES['argv']['do_save']) and is_numeric($catSecNum))
    {
      $catSection->tuitContents = $MODULES['argv']['T_CONTENTS'];
      $catSection->writeTuitContentsDB();
      header("Location: index.php?s=$catSecNum");
      break;
    }
    elseif(!empty($MODULES['argv']['do_find']) and isset($MODULES['argv']['find_name']) and is_numeric($catSecNum))
    {
      $myMain .= "<strong>Add ".$MODULES['argv']['add']." in section #$catSecNum</strong>
      <form action=\"$GLOBALS[PHP_SELF]\" method=\"post\">
      <input type=\"hidden\" name=\"m\" value=\"catalog\">
      <input type=\"hidden\" name=\"do_admin\" value=\"1\">
      <input type=\"hidden\" name=\"act\" value=\"tuit\">
      <input type=\"hidden\" name=\"s\" value=\"$catSecNum\">
      <input type=\"hidden\" name=\"add\" value=\"".$MODULES['argv']['add']."\">
      <br><br>";
      if($MODULES['argv']['add']=='exercise')
      {
        $SQL->query("select ex_id as id, ex_name as name, ex_description as description from exercises where ex_name like '%".addslashes($MODULES['argv']['find_name'])."%'");
        $t = 'e';
      }
      else
      {
        $SQL->query("select set_id as id, set_name as name from sets where set_name like '%".addslashes($MODULES['argv']['find_name'])."%'");
        $t = 'u';
      }
      if(!$SQL->numRows())
        $myMain .= "<h2>No ".$MODULES['argv']['add']." found!</h2>";
      else
      {
        while($row = $SQL->fetchArray())
        {
          if($t=='e')
            $hr = "exercise=$row[id]";
          else
            $hr = "unit=$row[id]";

          $myMain .= "<input type=\"checkbox\" name=\"".$t."[]\" value=\"$row[id]\"><strong>$row[name]</strong> (<a href=\"?m=tuit&$hr\" target=\"_new\">Preview</a>)<br>";
        }
      }
      $myMain .= "
      <input type=\"submit\" name=\"do_add\" value=\"   Add   \">
      </form>";
    }
    elseif(!empty($MODULES['argv']['do_move_up']) and is_numeric($catSecNum))
    {
      $catSection->readTuitContentsDB();
      if(is_numeric($MODULES['argv']['e']))
        $num = $catSection->tuitsOrder["e: ".$MODULES['argv']['e']];
      elseif(is_numeric($MODULES['argv']['u']))
        $num = $catSection->tuitsOrder["u: ".$MODULES['argv']['u']];

      if($num)
      {
        $buf = $catSection->tuits[$num];
        $catSection->tuits[$num] = $catSection->tuits[$num-1];
        $catSection->tuits[$num-1] = $buf;
      }
      $catSection->writeTuitContentsDB();
      header("Location: index.php?s=$catSecNum");
    }
    elseif(!empty($MODULES['argv']['do_move_down']) and is_numeric($catSecNum))
    {
      $catSection->readTuitContentsDB();
      if(is_numeric($MODULES['argv']['e']))
        $num = $catSection->tuitsOrder["e: ".$MODULES['argv']['e']];
      elseif(is_numeric($MODULES['argv']['u']))
        $num = $catSection->tuitsOrder["u: ".$MODULES['argv']['u']];

      if($num<$catSection->tuitsCnt-1)
      {
        $buf = $catSection->tuits[$num];
        $catSection->tuits[$num] = $catSection->tuits[$num+1];
        $catSection->tuits[$num+1] = $buf;
      }
      $catSection->writeTuitContentsDB();
      header("Location: index.php?s=$catSecNum");
    }
    else
    {
      $catSection->readTuitContentsDB();
      $myMain .= $catSection->addTuitContentsForm($MODULES['argv']['add']);
    }
    break;
}

?>
