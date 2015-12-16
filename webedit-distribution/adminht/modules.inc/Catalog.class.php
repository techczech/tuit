<?

include('catalog-constants.inc.php');

/* Classes definitions */

function findLinkedArticle(&$art)
{
  if(ereg('^%([[:digit:]]+)$',$art->name,$regs))
  {
    $art2id = $regs[1];
    $art2 = new Article($art2id);
    $art->name    = $art2->name;
    $art->perex   = $art2->perex;
    $art->author  = $art2->author;
    $art->text    = $art2->text;
  }
}

function findLinkedSectionInfo(&$sec)
{
  if(ereg('^%([[:digit:]]+)$',$sec->name,$regs))
  {
    $sec2id = $regs[1];
    $sec2 = new Section($sec2id);
    $sec->name = $sec2->name;
    $sec->title = $sec2->title;
    $sec->short_desc = $sec2->short_desc;
    $sec->desc = $sec2->desc;
    $sec->logo = $sec2->logo;
    $sec->url  = $sec2->url;
  }
}

function findLinkedSection(&$sec)
{
  if(ereg('^%([[:digit:]]+)$',$sec->name,$regs))
  {
    $sec2id = $regs[1];
    $sec = new Section($sec2id);
  }
}

class Section
{
  var $id,$name,$title,$short_desc,$desc,$previous,$time,$priority,$subpriority,$logo,$url,$options;
  var $lastUpdate;
  var $subs,$arts;
  var $ready;
  var $textBoxes;
  var $xOfTheDays;
  var $tuits,$tuitsCnt,$tuitsOrder;
  var $elements;
  var $SQL;

  function Section($id='')
  {
    $this->SQL = &$GLOBALS['SQL'];
    $this->ready = 0;
    $this->subs  = $this->arts = array();
    if(is_numeric($id))
    {
      $this->id = $id;
      $this->readDataDB();
    }
  }

  function isInvisible()
  {
    return $this->options&S_OPTION_INVISIBLE;
  }

  function findByName($n)
  {
    $res = $this->SQL->query("select ".DB_SECTION_ID." from ".DB_SECTION_TABLE." where ".DB_SECTION_NAME."='$n'");
    if($this->SQL->numRows($res)!=1)
      return FALSE;

    $row = $this->SQL->fetchArray($res);
    $this->id = $row[DB_SECTION_ID];
    $this->readDataDB();
  }

  function readDataDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $res = $this->SQL->query("select *,to_days(now())-to_days(".DB_SECTION_TIME.") as ".DB_LAST_UPDATE." from ".DB_SECTION_TABLE." where ".DB_SECTION_ID."=".$this->id);
    if($this->SQL->numRows($res)!=1)
      return;
    $row = $this->SQL->fetchArray($res);
    $this->name  = $row[DB_SECTION_NAME];
    $this->title = $row[DB_SECTION_TITLE];
    $this->desc  = $row[DB_SECTION_DESCRIPTION];
    $this->short_desc = $row[DB_SECTION_SHORT_DESCRIPTION];
    $this->previous = $row[DB_SECTION_PREVIOUS];
    $this->time  = $row[DB_SECTION_TIME];
    $this->priority = $row[DB_SECTION_PRIORITY];
    $this->subpriority = $row[DB_SECTION_SUBPRIORITY];
    $this->logo = $row[DB_SECTION_LOGO];
    $this->url = $row[DB_SECTION_URL];
    $this->options = $row[DB_SECTION_OPTIONS];
    $this->lastUpdate = $row[DB_LAST_UPDATE];
    $this->ready=1;
  }

  function readSubsDB()
  {
    $res = $this->SQL->query("select ".DB_SECTION_ID." from ".DB_SECTION_TABLE." where ".DB_SECTION_PREVIOUS."=".$this->id." order by ".DB_SECTION_PRIORITY.",".DB_SECTION_SUBPRIORITY.",".DB_SECTION_TIME." desc");
    while($row = $this->SQL->fetchArray($res))
      $this->subs[$row[DB_SECTION_ID]] = 1;

    return count($this->subs);
  }

  function readSubsDataDB()
  {
    if(!count($this->subs))
      if(!$this->readSubsDB())
        return 0;

    reset($this->subs);
    $subs = array();
    for($i=0;list($k,$v) = each($this->subs);$i++)
    {
      $subs[$i] = new Section($k);
      findLinkedSectionInfo($subs[$i]);
      $this->elements[TEXTBOX_SMALL][$subs[$i]->priority][] = &$subs[$i];
    }
    $this->subs = &$subs;

    return $subs;
  }

  function sectionInfoHTML($num='')
  {
    if(!$this->ready)
      return;

    if($this->isInvisible())
      $num = '<font color=white><strong>Inv</strong></font>';
    elseif(!$num)
      $num = '<img src="images/bluebulletsquare.gif" border=0 width=13 height=13>';

    if($this->url)
      $url = $this->url;
    else
      $url = "?m=catalog&s=".$this->id;

    return "
    <table class=\"sectioninfo\" cellspacing=0 cellpadding=0>
      <tr>
        <td class=\"sectinfobullet\">
        $num
        </td>
        <td class=\"sectinfohead\">
          <span class=\"sectinfohead\">
          <a href=\"$url\">".$this->name."</a>
          </span>
        </td>
      </tr>
      <tr>
        <td colspan=2 class=\"sectinfomain\">
        <p class=\"sectinfomain\">
        ".$this->short_desc.
        ( ($this->lastUpdate<=MAX_DAYS_FOR_NEW)?' '.NEW_INFO_TEXT:'' )."
        </p>
        <center>
        <a href=\"?m=catalog&do_admin=1&act=section&s=".$this->id."\">Edit</a> |
        <a href=\"#\" onclick=\"this.href=confirmDelete('?m=catalog&do_admin=1&act=section&do_delete=1&s=".$this->id."');\">Delete</a>
        </center>
        </td>
      </tr>
    </table>\n";
  }

  function sectionDescriptionHTML()
  {
    if(!$this->ready)
      return;

    return "
    <table class=\"xoftheday\" width=\"100%\">
    <tr class=\"xofthedayhead\">
      <th class=\"xofthedayhead\" width=\"100%\">
      About ".$this->name."
      </th>
    </tr>
    <tr class=\"xofthedaybody\">
      <td class=\"xofthedaybody\">
      ".$this->desc."
      </td>
    </tr>
    </table>\n";
  }

  function leftMenuHTML()
  {
    if(!$this->ready)
      return;

    reset($this->subs);
    $out = "<table cellspacing=0 cellpadding=0 class=\"sectionsmenu\">\n";

    if($this->previous)
    {
      $upSection = new Section($this->previous);
      $leftSubs = &$upSection->readSubsDataDB();
      reset($leftSubs);
      while(list($k,$v) = each($leftSubs))
      {
        if(!empty($v->url))
          $url = $v->url;
        else
          $url = "?m=catalog&s=".$v->id;
        if($v->isInvisible())
          continue;
          
        if($v->priority>$pri)
        {
          $pri = $v->priority;
          $out .= "
      <tr><td><img src=\"images/nic.gif\" border=0 width=1 height=5></td></tr>\n";
        }
        $out .= "
      <tr>
        <td valign=\"top\"><img src=\"images/bluebulletsquare.gif\" border=0 width=13 height=13></td>
        <td class=\"sectinfohead\"><span class=\"textsmall\"><a href=\"$url\">".(ereg_replace('^Czech (.*)$','\\1',$v->name))."</a></span></td>
      </tr>
      <tr><td></td></tr>\n";
      }
      return $out;
    }
    else
    {
      $pri = 1;
      while(list($k,$v) = each($this->subs))
      {
        if(!empty($v->url))
          $url = $v->url;
        else
          $url = "?m=catalog&s=".$v->id;
            
        if($v->isInvisible())
          continue;

        if($v->priority>$pri)
        {
          $pri = $v->priority;
          $out .= "<tr><td><img src=\"images/nic.gif\" border=0 width=1 height=5></td></tr>\n";
        }
        $out .= "
        <tr>
          <td valign=\"top\"><img src=\"images/bluebulletsquare.gif\" border=0 width=13 height=13></td>
          <td class=\"sectinfohead\"><span class=\"textsmall\"><a href=\"$url\">".(ereg_replace('^Czech (.*)$','\\1',$v->name))."</a></span></td>
        </tr>
        <tr><td></td></tr>\n";
      }
    }
    return $out;
  }

  function allElementsHTML($doCnt=0)
  {
    $out = '<tr valign="top">';
    $col = 0;
    $sec_cnt = 0;
    ksort($this->elements[TEXTBOX_SMALL]);
    reset($this->elements[TEXTBOX_SMALL]);
    while(list($k,$v) = each($this->elements[TEXTBOX_SMALL]))
    {
      while(list($k2,$v2) = each($v))
      {
        $pp = 1;
        if(get_class($v2) == 'section')
        {
          if($v2->isInvisible())
            continue;

          $out .= "<td width=\"50%\">\n".$v2->sectionInfoHTML($doCnt?($sec_cnt+1):'')."</td>\n";
          $sec_cnt++;
        }
        elseif(get_class($v2) == 'article')
        {
          $pp = 0;
          if($col%2)
            $out .= "</tr>\n<tr valign=\"top\">\n";
          $out .= "<td width=\"100%\" colspan=2>\n".$v2->articleInfoHTML($doCnt?($sec_cnt+1):'')."</td>
          </tr><tr valign=\"top\">\n";
          $sec_cnt++;
        }
        elseif(get_class($v2) == 'textbox')
        {
          if($v2->type==TEXTBOX)
          {
            $w = '100%';
            $s = 'colspan=2';
            $pp = 0;
            if($col%2)
              $out .= "</tr>\n<tr valign=\"top\">\n";
          }
          else
          {
            $w = '50%';
            $s = '';
          }
          $out .= "<td width=\"$w\" $s>\n".$v2->textBoxHTML()."</td>\n";
          if($v2->type==TEXTBOX)
            $out .= "</tr>\n<tr valign=\"top\">\n";
        }

        if($col%2)
          $out .= "</tr>\n<tr valign=\"top\">\n";
        if($pp)
          $col++;
      }
    }

    for($i=0;$i<$this->tuitsCnt;$i++)
    {
       $out .= "
      <td width=\"50%\">\n"; 
      if($this->tuits[$i]['type']=='e')
        $out .= $this->exerciseInfoHTML($i);
      elseif($this->tuits[$i]['type']=='u')
        $out .= $this->unitInfoHTML($i);

      $out .= "
      </td>\n";

      if($col%2)
        $out .= "</tr>\n<tr valign=\"top\">\n";
      $col++;

    }
    if($col%2)
      $out .= "</tr>\n";

    return $out;
  }

  function readArtsDB()
  {
    $res = $this->SQL->query("select ".DB_ARTICLE_ID.",".DB_ARTICLE_PRIORITY." from ".DB_ARTICLE_TABLE." where ".DB_ARTICLE_SECTION."=".$this->id." order by ".DB_ARTICLE_PRIORITY.",".DB_ARTICLE_TIME);
    while($row = $this->SQL->fetchArray($res))
      $this->arts[$row[DB_ARTICLE_ID]] = 1;

    return(count($this->arts));
  }

  function readArtsDataDB()
  {
    if(!count($this->arts))
      if(!$this->readArtsDB())
        return 0;

    reset($this->arts);
    $arts = array();
    for($i=0;list($k,$v) = each($this->arts);$i++)
    {
      $arts[$i] = new Article($k);
      findLinkedArticle($arts[$i]);
      $this->elements[TEXTBOX_SMALL][$arts[$i]->priority][] = &$arts[$i];
    }

    return $arts;
  }

  function readTextBoxesDB()
  {
    $res = $this->SQL->query("select * from ".DB_TEXT_TABLE." where ".DB_TEXT_SECTION."=".$this->id." order by ".DB_TEXT_PRIORITY);
    while($row = $this->SQL->fetchArray($res))
    {
      $tb = new TextBox($row[DB_TEXT_ID],$row[DB_TEXT_HEADER],$row[DB_TEXT_TEXT],$row[DB_TEXT_SECTION],$row[DB_TEXT_TYPE],$row[DB_TEXT_PRIORITY]);
      $this->textBoxes[$row[DB_TEXT_TYPE]][] = &$tb;
      $this->elements[TEXTBOX_SMALL][$row[DB_TEXT_PRIORITY]][] = &$tb;

      unset($tb);
    }

    return $this->SQL->numRows($res);
  }

  function textBoxesHTML()
  {
    if(!$this->ready)
      return;
    $out = '';
    while(list($k,$v) = each($this->textBoxes[TEXTBOX]))
      $out .= $v->textBoxHTML();

    return $out;
  }

  function readXOfTheDaysDB()
  {
    $res = $this->SQL->query("select ".DB_XOTD_SECTIONS_XOTD_ID.",".DB_XOTD_SECTIONS_PRIORITY." from ".DB_XOTD_SECTIONS_TABLE." where ".DB_XOTD_SECTIONS_SECTION_ID."=".$this->id." order by ".DB_XOTD_SECTIONS_PRIORITY);
    $cnt = 0;
    while($row = $this->SQL->fetchArray($res))
    {
      $this->xOfTheDays[$cnt]['id'] = $row[DB_XOTD_SECTIONS_XOTD_ID];
      $this->xOfTheDays[$cnt++]['pri'] = $row[DB_XOTD_SECTIONS_PRIORITY];
    }

    return $cnt;
  }

  function readXOfTheDaysDataDB()
  {
    if(!count($this->xOfTheDays))
      if(!$this->readXOfTheDaysDB())
        return;

    for($i = 0; $i < count($this->xOfTheDays); $i++)
      $this->xOfTheDays[$i]['obj'] = new XOfTheDay($this->xOfTheDays[$i]['id']);
  }

  function xOfTheDaysHTML()
  {
    if(!count($this->xOfTheDays))
      return;

    for($i = 0; $i < count($this->xOfTheDays); $i++)
      $out .= $this->xOfTheDays[$i]['obj']->xOfTheDayHTML();

    return $out;
  }

  function editForm()
  {
    $opt = "
    <select name=\"S_OPTIONS[]\" multiple size=1>
      <option value=".S_OPTION_INVISIBLE.($this->isInvisible()?' selected':'').">Invisible
    ";
    $opt .= "
    </select>\n";
    $out = "
    <strong>".(is_numeric($this->id)?("Editing section #".$this->id):'New section')."</strong><br><br>
    <form action=\"$GLOBALS[PHP_SELF]\" method=\"post\">
    <input type=\"hidden\" name=\"m\" value=\"catalog\">
    <input type=\"hidden\" name=\"do_admin\" value=1>
    <input type=\"hidden\" name=\"act\" value=\"section\">
    <input type=\"hidden\" name=\"S_PREVIOUS\" value=\"".$this->previous."\">
    <input type=\"hidden\" name=\"s\" value=\"".$this->id."\">
    <strong>Name</strong><br>
    <input type=\"text\" name=\"S_NAME\" value=\"".$this->name."\" size=80 maxlength=100><br>
    <strong>Title</strong><br>
    <input type=\"text\" name=\"S_TITLE\" value=\"".$this->title."\" size=80 maxlength=100><br>
    <strong>Short description</strong><br>
    <textarea name=\"S_SHORT_DESC\" rows=2 cols=78>".htmlspecialchars($this->short_desc)."</textarea> <br>
    <strong>Description</strong><br>
    <textarea name=\"S_DESCRIPTION\" id=\"I_S_DESCRIPTION\" rows=20 cols=100>".htmlspecialchars($this->desc)."</textarea>
    <br>
    <strong>Priority</strong><br>
    <input type=\"text\" name=\"S_PRIORITY\" value=\"".$this->priority."\" size=4 maxlength=4><br>
    <strong>Subpriority</strong><br>
    <input type=\"text\" name=\"S_SUBPRIORITY\" value=\"".$this->subpriority."\" size=4 maxlength=4><br>
    <strong>Options</strong><br>
    $opt<br>
    <strong>Logo</strong> (optional)<br>
    <input type=\"text\" name=\"S_LOGO\" value=\"".$this->logo."\" size=80 maxlength=100><br>
    <strong>URL</strong> (optional)<br>
    <input type=\"text\" name=\"S_URL\" value=\"".$this->url."\" size=80 maxlength=255><br>
    <input type=\"submit\" name=\"do_save\" value=\"   Save   \">
    </form>
    <script type=\"text/javascript\">
      editor = new HTMLArea('I_S_DESCRIPTION');
      editor.registerPlugin(TableOperations);
      editor.generate();
    </script>
    \n";

    return $out;
  }

  function addXOfTheDayForm(&$xs)
  {
    if(!$this->id)
      return -1;

    $out = "
    <strong>Adding X Of The Day in section #".$this->id.", ".$this->name."</strong><br><br>
    <form action=\"$GLOBALS[PHP_SELF]\" method=\"post\">
    <input type=\"hidden\" name=\"m\" value=\"catalog\">
    <input type=\"hidden\" name=\"do_admin\" value=1>
    <input type=\"hidden\" name=\"act\" value=\"xoftheday\">
    <input type=\"hidden\" name=\"s\" value=".$this->id.">
    <table border=1 style=\"border-collapse: collapse;\">
    <tr>
      <th><strong>X Of The Day</strong></th>
      <th><strong>Priority</strong></th>\n";

    $sel = array();
    for($i=0;$i<count($this->xOfTheDays);$i++)
    {
      if(in_array($this->xOfTheDays[$i]['id'],$xs))
      {
        $sel[] = $this->xOfTheDays[$i]['id'];
        $pri[$this->xOfTheDays[$i]['id']] = $this->xOfTheDays[$i]['pri'];
      }
    }

    $cnt = 0;
    while(list($k,$v) = each($xs))
    {
      $out .= "
      <tr>
      <td><input type=\"checkbox\" name=\"XS[$cnt]\" value=$v".(in_array($v,$sel)?' checked':'')."> $k</td>
      <td align=\"center\"><input type=\"text\" size=4 maxlength=4 name=\"XP[$cnt]\" value=\"".$pri[$v]."\"></td>
      </tr>\n";
      $cnt++;
    }
    $out .= "
    </table>
    <br><br>
    <input type=\"submit\" name=\"do_save\" value=\"   Save   \">
    </form>\n";

    return $out;
  }

  function writeDataDB()
  {
    $sql = "replace into ".DB_SECTION_TABLE." values(".
      (is_numeric($this->id)?$this->id:'null').
      ",'".
      addslashes($this->name)."','".
      addslashes($this->title)."','".
      addslashes($this->short_desc)."','".
      addslashes($this->desc)."',".
      addslashes($this->previous).
      ",null,".
      addslashes($this->priority).",".
      addslashes($this->subpriority).",'".
      addslashes($this->logo)."','".
      addslashes($this->url).
      "',".
      addslashes($this->options).")";

    if(!$this->SQL->query($sql))
      return -1;

    $this->SQL->query("update ".DB_SECTION_TABLE." set ".DB_SECTION_TIME."=null where ".DB_SECTION_ID."=".$this->previous);
    return 0;
  }

  function deleteFromDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $sql = "delete from ".DB_SECTION_TABLE." where ".DB_SECTION_ID."=".$this->id;
    if(!$this->SQL->query($sql))
      return -1;

    return 0;
  }

  function exerciseInfoHTML($n)
  {
    return "
    <table class=\"sectioninfo\" cellspacing=0 cellpadding=0>
      <tr>
        <td class=\"sectinfobullet\">
        <img src=\"images/bluebulletsquare.gif\" border=0 width=13 height=13>
        </td>
        <td class=\"sectinfohead\">
          <span class=\"sectinfohead\">
          <a href=\"?m=tuit&exercise=".$this->tuits[$n]['id']."\">".$this->tuits[$n]['instance']->parameters['ex_name']."</a>
          </span>
        </td>
      </tr>
      <tr>
        <td colspan=2 class=\"sectinfomain\">
        <p class=\"sectinfomain\">
        ".$this->tuits[$n]['instance']->parameters['ex_description']."<br>
        <small>
        ".$this->tuits[$n]['instance']->parameters['ex_task']."
        </small>
        </p>
        <center>
        ".($n?("
        <a href=\"?m=catalog&do_admin=1&act=tuit&do_move_up=1&s=".$this->id."&e=".$this->tuits[$n]['id']."\">Up</a> |"):'').
        ($n==$this->tuitsCnt-1?'':("
        <a href=\"?m=catalog&do_admin=1&act=tuit&do_move_down=1&s=".$this->id."&e=".$this->tuits[$n]['id']."\">Down</a> |"))."
        <a href=\"#\" onclick=\"this.href=confirmDelete('?m=catalog&do_admin=1&act=tuit&do_delete=1&s=".$this->id."&e=".$this->tuits[$n]['id']."');\">Delete</a>
        </center>
        </td>
      </tr>
    </table>\n";
  }

  function unitInfoHTML($n)
  {
    return "
    <table class=\"sectioninfo\" cellspacing=0 cellpadding=0>
      <tr>
        <td class=\"sectinfobullet\">
        <img src=\"images/bluebulletsquare.gif\" border=0 width=13 height=13>
        </td>
        <td class=\"sectinfohead\">
          <span class=\"sectinfohead\">
          <a href=\"?m=tuit&unit=".$this->tuits[$n]['id']."\">".$this->tuits[$n]['instance']->name."</a>
          </span>
        </td>
      </tr>
      <tr>
        <td colspan=2 class=\"sectinfomain\">
        <p class=\"sectinfomain\">
        ".$this->tuits[$n]['instance']->creator."<br>
        <small>
    <!--    ".$this->tuits[$n]['instance']->difficulty."-->
        </small>
        </p>
        <center>
        ".($n?("
        <a href=\"?m=catalog&do_admin=1&act=tuit&do_move_up=1&s=".$this->id."&u=".$this->tuits[$n]['id']."\">Up</a> |"):'').
        ($n==$this->tuitsCnt-1?'':("
        <a href=\"?m=catalog&do_admin=1&act=tuit&do_move_down=1&s=".$this->id."&u=".$this->tuits[$n]['id']."\">Down</a> |"))."
        <a href=\"#\" onclick=\"this.href=confirmDelete('?m=catalog&do_admin=1&act=tuit&do_delete=1&s=".$this->id."&u=".$this->tuits[$n]['id']."');\">Delete</a>
        </center>
        </td>
      </tr>
    </table>\n";
  }

  function parseTuitContents()
  {
    $cont = split('[[:space:]]*;[[:space:]]*',$this->tuitContents);
    $this->tuitsCnt=0;
    while(list($k,$v) = each($cont))
    {
      $v = str_replace(' ','',$v);
      list($t,$i) = explode(':',$v);
      $this->tuits[$this->tuitsCnt] = array('type'=>$t,'id'=>$i);
      if($t=='e')
        $this->tuits[$this->tuitsCnt]['instance'] = ex_inst($i);
      elseif($t=='u')
        $this->tuits[$this->tuitsCnt]['instance'] = new UNIT($i);

      $this->tuitsOrder["$t: $i"] = $this->tuitsCnt;
      $this->tuitsCnt++;
    }
  }

  function readTuitContentsDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $sql = "select * from ".DB_XTUIT_TABLE." where ".DB_XTUIT_SECTION."=".$this->id;
    if(!$res = $this->SQL->query($sql))
      return -1;

    if(!$row = $this->SQL->fetchArray($res))
      return -1;

    include('inc/constants.inc');
    include('un_classes.inc');
    $this->tuitContents = $row[DB_XTUIT_CONTENTS];
    $this->parseTuitContents();
  }

  function removeTuitElements($a)
  {
    reset($a);
    while(list($k,$v) = each($a))
    ;
  }

  function removeTuitElement($k)
  {
    $num = $this->tuitsOrder[$k];
    for($i=$num+1;$i<$this->tuitsCnt;$i++)
      $this->tuits[$i-1] = $this->tuits[$i];

    unset($this->tuits[$this->tuitsCnt]);
    $this->tuitsCnt--;
  }

  function tuitContentsList()
  {
    for($i=0;$i<$this->tuitsCnt;$i++)
    {
      if(get_class($this->tuits[$i]['instance']) == 'unit')
      {
        $t = 'u';
        $id = $this->tuits[$i]['instance']->id;
        $name = $this->tuits[$i]['instance']->name;
      }
      else
      {
        $t = 'e';
        $id = $this->tuits[$i]['instance']->ex_id;
        $name = $this->tuits[$i]['instance']->parameters['ex_name'];
      }

      $out .= "
        <input type=\"checkbox\" name=\"".$t."[]\" value=\"$id\">$name<br>";
    }
    return $out;
  }

  function addTuitContentsForm($add)
  {
    if(!is_numeric($this->id))
      return -1;
    $out = "
    <strong>Editing Tuit contents in section #$this->id</strong><br><br>
    <form action=\"$GLOBALS[PHP_SELF]\" method=\"post\">
    <input type=\"hidden\" name=\"m\" value=\"catalog\">
    <input type=\"hidden\" name=\"do_admin\" value=1>
    <input type=\"hidden\" name=\"act\" value=\"tuit\">
    <input type=\"hidden\" name=\"s\" value=\"".$this->id."\">
    <strong>Contents</strong><br>
    <!--
    ".$this->tuitContentsList()."
    <input type=\"submit\" name=\"do_delete\" value=\"   Remove   \">
    <br>
    <br>
    <textarea name=\"T_CONTENTS\" rows=2 cols=78>".$this->tuitContents."</textarea> <br>
    <input type=\"submit\" name=\"do_save\" value=\"   Save   \">
    <br>
    -->
    <br>
    <input type=\"text\" name=\"find_name\"> Find $add by name<br><br>
    <input type=\"submit\" name=\"do_find\" value=\"   Find   \">
    <input type=\"hidden\" name=\"add\" value=\"$add\">
    </form>\n";

    return $out;
  }

  function prepareTuitContents()
  {
    for($i=0;$i<$this->tuitsCnt;$i++)
    {
      $t = $this->tuits[$i]['type'];
      $id = $this->tuits[$i]['id'];
      $out .= ($out?'; ':'')."$t: $id";
    }
    return $out;
  }

  function writeTuitContentsDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $this->tuitContents = $this->prepareTuitContents();
    $this->SQL->query("replace into ".DB_XTUIT_TABLE." values($this->id,'".addslashes($this->tuitContents)."')");
  }
}

class Article
{
  var $id,$name,$author,$perex,$text,$time,$readers,$section,$priority;
  var $utime;
  var $ready;
  var $SQL;

  function Article($id='')
  {
    $this->SQL = &$GLOBALS['SQL'];
    if(is_numeric($id))
    {
      $this->id = $id;
      $this->readDataDB();
    }
    else
      $this->ready = 0;
  }

  function readDataDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $res = $this->SQL->query("select *,unix_timestamp(".DB_ARTICLE_TIME.") as ".DB_ARTICLE_UTIME.",to_days(now())-to_days(".DB_ARTICLE_TIME.") as ".DB_LAST_UPDATE." from ".DB_ARTICLE_TABLE." where ".DB_ARTICLE_ID."=".$this->id);
    if($this->SQL->numRows($res)!=1)
      return;
    $row = $this->SQL->fetchArray($res);
    $this->name     = $row[DB_ARTICLE_NAME];
    $this->author   = $row[DB_ARTICLE_AUTHOR];
    $this->perex    = $row[DB_ARTICLE_PEREX];
    $this->text     = $row[DB_ARTICLE_TEXT];
    $this->time     = $row[DB_ARTICLE_TIME];
    $this->utime    = $row[DB_ARTICLE_UTIME];
    $this->readers  = $row[DB_ARTICLE_READERS];
    $this->section  = $row[DB_ARTICLE_SECTION];
    $this->priority = $row[DB_ARTICLE_PRIORITY];
    $this->lastUpdate = $row[DB_LAST_UPDATE];
    $this->ready = 1;
  }

  function articleInfoHTML($num='')
  {
    if(!$this->ready)
      return;

    return "
    <div class=\"articleinfo\">
      <span class=\"artinfohead\">
        <a href=\"?m=catalog&s=".$this->section."&a=".$this->id."\">".$this->name."</a>
      </span>
      <br>
      ".$this->perex." -- ".$this->author." <small>(".$this->readers.")</small> <small>[<a href=\"?m=catalog&s=".$this->section."&a=".$this->id."\">more</a>]</small>".
      ( ($this->lastUpdate<8)?' '.NEW_INFO_TEXT:'' )."
      <center>
      <a href=\"?m=catalog&do_admin=1&act=article&a=".$this->id."&s=".$this->section."\">Edit</a> |
      <a href=\"#\" onclick=\"this.href=confirmDelete('?m=catalog&do_admin=1&act=article&do_delete=1&a=".$this->id."');\">Delete</a>
      </center>
    </div>\n";
  }

  function articleHTML()
  {
    if(!$this->ready)
      return;

    return "
    <div class=\"article\">
    <h3>".$this->name."</h3>
    <em>".$this->perex."</em><br>
    <br>
    ".$this->text."
    </div>
    <div style=\"text-align: right;\"><em>".$this->author." -- ".date('j. n. Y',$this->utime)."</em></div>
    <center>
      <a href=\"?m=catalog&do_admin=1&act=article&a=".$this->id."&s=".$this->section."\">Edit</a> |
      <a href=\"#\" onclick=\"this.href=confirmDelete('?m=catalog&do_admin=1&act=article&do_delete=1&a=".$this->id."');\">Delete</a>
    </center>\n";
  }

  function editForm()
  {
    $out = "
    <strong>
    ".(is_numeric($this->id)?("Editing article #".$this->id):"New article in section #".$this->section)."
    </strong>
    <br><br>
    <form action=\"$GLOBALS[PHP_SELF]\" method=\"post\">
    <input type=\"hidden\" name=\"m\" value=\"catalog\">
    <input type=\"hidden\" name=\"do_admin\" value=1>
    <input type=\"hidden\" name=\"act\" value=\"article\">
    <input type=\"hidden\" name=\"a\" value=\"".$this->id."\">
    <input type=\"hidden\" name=\"A_SECTION\" value=\"".$this->section."\">
    <strong>Name</strong><br>
    <input type=\"text\" name=\"A_NAME\" size=80 maxlength=100 value=\"".$this->name."\"><br>
    <strong>Author</strong><br>
    <input type=\"text\" name=\"A_AUTHOR\" size=80 maxlength=100 value=\"".$this->author."\"><br>
    <strong>Perex</strong><br>
    <textarea rows=10 cols=100 name=\"A_PEREX\" id=\"I_A_PEREX\">".htmlspecialchars($this->perex)."</textarea>
    <br>
    <strong>Text</strong><br>
    <textarea rows=25 cols=100 name=\"A_TEXT\" id=\"I_A_TEXT\">".htmlspecialchars($this->text)."</textarea>
    <br>
    <strong>Priority</strong><br>
    <input type=\"text\" name=\"A_PRIORITY\" size=4 maxlength=4 value=\"".$this->priority."\"><br><br>
    <input type=\"submit\" name=\"do_save\" value=\"   Save   \">
    </form>
    <script type=\"text/javascript\">
      editor = new HTMLArea('I_A_PEREX');
      editor.registerPlugin(TableOperations);
      editor.generate();
      editor2 = new HTMLArea('I_A_TEXT');
      editor2.registerPlugin(TableOperations);
      editor2.generate();
    </script>
    \n";
    return $out;
  }

  function writeDataDB()
  {
    $sql = "replace into ".DB_ARTICLE_TABLE." values(".
      (is_numeric($this->id)?$this->id:'null').
      ",'".
      addslashes($this->name)."','".
      addslashes($this->author)."','".
      addslashes($this->perex)."','".
      addslashes($this->text)."',".
      (isset($this->time)?("'".$this->time."'"):"null").",".
      (is_numeric($this->readers)?$this->readers:'0').",".
      addslashes($this->section).",".
      addslashes($this->priority).
      ")";

    if(!$this->SQL->query($sql))
      return -1;

    $this->SQL->query("update ".DB_SECTION_TABLE." set ".DB_SECTION_TIME."=null where ".DB_SECTION_ID."=".$this->id);
    return 0;
  }

  function increaseReaders()
  {
    $this->readers++;
    $this->SQL->query("update ".DB_ARTICLE_TABLE." set ".DB_ARTICLE_READERS."=".$this->readers.",".DB_ARTICLE_TIME."=".DB_ARTICLE_TIME." where ".DB_ARTICLE_ID."=".$this->id);
  }

  function deleteFromDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $sql = "delete from ".DB_ARTICLE_TABLE." where ".DB_ARTICLE_ID."=".$this->id;
    if(!$this->SQL->query($sql))
      return -1;

    return 0;
  }
}

class TextBox
{
  var $id,$header,$text,$section,$type,$priority;
  var $ready;
  var $SQL;

  function TextBox($id='',$h='',$t='',$s='',$ty=0,$pr=1)
  {
    $this->SQL = &$GLOBALS['SQL'];
    if(is_numeric($id))
    {
      $this->id = $id;
      if(!empty($h) and !empty($t) and is_numeric($s))
      {
        $this->header  = $h;
        $this->text    = $t;
        $this->section = $s;
        $this->type    = $ty;
        $this->priority = $pr;
        $this->ready   = 1;
      }
      else
        $this->readDataDB();
    }
    else
      $this->ready=0;
  }

  function readDataDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $res = $this->SQL->query("select ".DB_TEXT_HEADER.",".DB_TEXT_TEXT.",".DB_TEXT_SECTION.",".DB_TEXT_TYPE.",".DB_TEXT_PRIORITY." from ".DB_TEXT_TABLE." where ".DB_TEXT_ID."=".$this->id);
    if($this->SQL->numRows($res)!=1)
      return;

    $row = $this->SQL->fetchArray($res);
    $this->header   = $row[DB_TEXT_HEADER];
    $this->text     = $row[DB_TEXT_TEXT];
    $this->section  = $row[DB_TEXT_SECTION];
    $this->type     = $row[DB_TEXT_TYPE];
    $this->priority = $row[DB_TEXT_PRIORITY];
    $this->ready    = 1;
  }

  function textBoxHTML()
  {
    if(!$this->ready)
      return;

    return "
    <div class=\"textbox".$this->type."\">".
    (!empty($this->header)?("<div class=\"textbox".$this->type."head\">"
    .$this->header."</div>"):"").
    $this->text."
    <br><br>
    <center>
    <a href=\"?m=catalog&do_admin=1&act=textbox&i=".$this->id."&s=".$this->section."\">Edit</a> |
    <a href=\"#\" onclick=\"this.href=confirmDelete('?m=catalog&do_admin=1&act=textbox&do_delete=1&i=".$this->id."');\">Delete</a>
    </center>
    </div>\n";
  }

  function editForm()
  {
    $out = "
    <strong>
    ".(is_numeric($this->id)?("Editing textbox #".$this->id):"New textbox in section #".$this->section)."
    </strong>
    <br><br>
    <form action=\"$GLOBALS[PHP_SELF]\" method=\"post\">
    <input type=\"hidden\" name=\"m\" value=\"catalog\">
    <input type=\"hidden\" name=\"do_admin\" value=1>
    <input type=\"hidden\" name=\"act\" value=\"textbox\">
    <input type=\"hidden\" name=\"i\" value=\"".$this->id."\">
    <input type=\"hidden\" name=\"T_SECTION\" value=\"".$this->section."\">
    <strong>Header</strong><br>
    <input type=\"text\" name=\"T_HEADER\" size=80 maxlength=100 value=\"".$this->header."\"><br>
    <strong>Text</strong><br>
    <textarea rows=25 cols=100 name=\"T_TEXT\" id=\"I_T_TEXT\">".htmlspecialchars($this->text)."</textarea>
    <br>
    <strong>Type</strong><br>
    <select name=\"T_TYPE\" size=1>
      <option value=".TEXTBOX.($this->type==TEXTBOX?' selected':'').">Classic
      <option value=".TEXTBOX_SMALL.($this->type==TEXTBOX_SMALL?' selected':'').">Small
    </select>
    <br>
    <strong>Priority</strong><br>
    <input type=\"text\" name=\"T_PRIORITY\" value=\"".$this->priority."\" size=4 maxlength=4><br>
    <br>
    <input type=\"submit\" name=\"do_save\" value=\"   Save   \">
    </form>
    <script type=\"text/javascript\">
      editor = new HTMLArea('I_T_TEXT');
      editor.registerPlugin(TableOperations);
      editor.generate();
    </script>
    \n";
    return $out;
  }

  function writeDataDB()
  {
    $sql = "replace into ".DB_TEXT_TABLE." values(".
      (is_numeric($this->id)?$this->id:'null').
      ",'".
      addslashes($this->header)."','".
      addslashes($this->text)."',".
      addslashes($this->section).",".
      addslashes($this->type).",".
      addslashes($this->priority).
      ")";

    if(!$this->SQL->query($sql))
      return -1;

    $this->SQL->query("update ".DB_SECTION_TABLE." set ".DB_SECTION_TIME."=null where ".DB_SECTION_ID."=".$this->id);
    return 0;
  }

  function deleteFromDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $sql = "delete from ".DB_TEXT_TABLE." where ".DB_TEXT_ID."=".$this->id;
    if(!$this->SQL->query($sql))
      return -1;

    return 0;
  }
}

class LinkList
{
  var $section;
  var $linx;
  var $ready;
  var $count;
  var $SQL;

  function LinkList($s='')
  {
    $this->SQL = &$GLOBALS['SQL'];
    if(is_numeric($s))
    {
      $this->section = $s;
      $this->readDataDB();
    }
    else
      $this->ready = 0;
  }

  function readDataDB($id=0)
  {
    if(!is_numeric($this->section))
      return -1;

    $sql = "select * from ".DB_LINX_TABLE." where ".DB_LINX_SECTION."=".$this->section;
    if($id)
      $sql .= " and ".DB_LINX_ID."=$id";
    $sql .= " order by ".DB_LINX_NAME;

    $res = $this->SQL->query($sql);
    $this->count = 0;
    while($row = $this->SQL->fetchArray($res))
      $this->linx[$row[DB_LINX_ID]] = $row;

    $this->ready = 1;
    return $this->SQL->numRows($res);
  }

  function linxHTML($id=0)
  {
    if(!$this->ready)
      return;

    $out = '';
    while(list($k,$v) = each($this->linx))
    {
      $out .= "<br><a href=\"".$v[DB_LINX_URL]."\">".$v[DB_LINX_NAME]."</a>
      -- <a href=\"?m=catalog&do_admin=1&act=linx&i=".$v[DB_LINX_ID]."&s=".$this->section."\">Edit</a> | <a href=\"#\" onclick=\"this.href=confirmDelete('?m=catalog&do_admin=1&act=linx&i=".$v[DB_LINX_ID]."&s=".$this->section."&do_delete=1');\">Delete</a>
      <br>\n".
        "<small>".$v[DB_LINX_DESCRIPTION]."</small><br>\n";
    }

    return $out;
  }

  function editForm($id=0)
  {
    $out = "
    <strong>
    ".($id>0?("Editing link #".$id):"New link in section #".$this->section)."
    </strong>
    <br><br>
    <form action=\"$GLOBALS[PHP_SELF]\" method=\"post\">
    <input type=\"hidden\" name=\"m\" value=\"catalog\">
    <input type=\"hidden\" name=\"do_admin\" value=1>
    <input type=\"hidden\" name=\"act\" value=\"linx\">
    <input type=\"hidden\" name=\"i\" value=\"".$id."\">
    <input type=\"hidden\" name=\"L_SECTION\" value=\"".$this->section."\">
    <strong>URL</strong><br>
    <input type=\"text\" name=\"L_URL\" size=80 maxlength=255 value=\"".$this->linx[$id][DB_LINX_URL]."\"><br>
    <strong>Name</strong><br>
    <input type=\"text\" name=\"L_NAME\" size=80 maxlength=100 value=\"".$this->linx[$id][DB_LINX_NAME]."\"><br>
    <strong>Description</strong><br>
    <textarea name=\"L_DESCRIPTION\" rows=2 cols=78>".$this->linx[$id][DB_LINX_DESCRIPTION]."</textarea><br>
    <br>
    <input type=\"submit\" name=\"do_save\" value=\"   Save   \">
    </form>\n";
    return $out;
  }

  function writeDataDB($id=0)
  {
    if($id)
    {
      $sql = "replace into ".DB_LINX_TABLE." values($id,'".addslashes($this->linx[$id][DB_LINX_URL])."','".addslashes($this->linx[$id][DB_LINX_NAME])."','".addslashes($this->linx[$id][DB_LINX_DESCRIPTION])."',".addslashes($this->section).")";
      if(!$res = $this->SQL->query($sql))
        return -1;
    }
    else
    {
    }

    return $this->SQL->affectedRows($res);
  }

  function addLinkDB($url,$name,$desc,$sec)
  {
    $sql = "insert into ".DB_LINX_TABLE." values(null,'".addslashes($url)."','".addslashes($name)."','".addslashes($desc)."',".addslashes($sec).")";
    if($res = $this->SQL->query($sql))
      return -1;
    else
      return 0;
  }
}

class XOfTheDay
{
  var $id,$name;
  var $content;
  var $ready;
  var $SQL;

  function XOfTheDay($id=0)
  {
    $this->SQL = &$GLOBALS['SQL'];
    if(is_numeric($id) and $id>0)
    {
      $this->id=$id;
      $this->readDataDB();
    }
    else
      $this->ready = 0;

  }

  function readDataDB()
  {
    if(!is_numeric($this->id))
      return -1;

    $sql = "select ".DB_XOTD_NAME." from ".DB_XOTD_TABLE." where ".DB_XOTD_ID."=".$this->id;
    $res = $this->SQL->query($sql);
    if($this->SQL->numRows($res)!=1)
      return;

    $row = $this->SQL->fetchArray($res);
    $this->name = $row[DB_XOTD_NAME];
    $this->readContentsDB();
  }

  function readContentsDB()
  {
    $sql = "select ".DB_XOTDD_CONTENT." from ".DB_XOTDD_TABLE." where ".DB_XOTDD_XOTD_ID."=".$this->id." order by rand() limit 1";
    $res = $this->SQL->query($sql);
    if($this->SQL->numRows($res)!=1)
      return;

    $row = $this->SQL->fetchArray($res);
    $this->content = $row[DB_XOTDD_CONTENT];
    $this->ready = 1;
  }

  function xOfTheDayHTML()
  {
    if(!$this->ready)
      return;

    return "
    <table class=\"xoftheday\" width=\"100%\">
    <tr class=\"xofthedayhead\">
      <th class=\"xofthedayhead\" width=\"100%\">
      ".$this->name."
      </th>
    </tr>
    <tr class=\"xofthedaybody\">
      <td class=\"xofthedaybody\">
      ".$this->content."
      </td>
    </tr>
    </table>\n";
  }
}

?>
