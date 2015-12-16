<?

/******************************* Mi Classes ******************************/
/* public domain html elements classes ..                                */
/*************************************************************************/

# class MENU only implements a hash of menu items
# layout of the menu itself will be defined in inherited classes..

class MENU
{
  var $items,$count;

  function MENU($it = array())
  {
    $this->count = 0;
    while(list($k,$v) = each($it))
    {
      $this->items[$this->count]['name'] = $k;
      $this->items[$this->count]['href'] = $v;
      $this->count++;
    }
  }

  function add_item($name,$href,$opt='')
  {
    $this->items[$this->count]['name'] = $name;
    $this->items[$this->count]['href'] = $href;
    $this->items[$this->count]['opt']  = $opt;
    $this->count++;
  }

  function add_items($it = array())
  {
    while(list($k,$v) = each($it))
      $this->add_item($k,$v);
  }

  function recycle()
  {
    $this->items=array();
    $this->count=0;
  }

  function how_many()
  {
    return $this->count;
  }
}

# HMENU stands for horizontal menu of hyperlinks..
# if 'href' is not present, then the item will not be a link

class HMENU extends MENU
{
  var $style,$separator;

  function HMENU($it = array(),$st='',$sp=' | ')
  {
    $this->MENU($it);
    $this->style = $st;
    $this->separator = $sp;
  }

  function set_style($st)
  {
    $this->style = $st;
  }

  function set_separator($sp)
  {
    $this->separator = $sp;
  }

  function prepare()
  {
    $out = "<span class=\"".$this->style."\">$this->separator";
    for($i = 0; $i < $this->count; $i++)
    {
      if($this->items[$i]['name'])
        $out .= "<a href=\"".$this->items[$i]['href']."\">".$this->items[$i]['name']."</a>$this->separator";
    }

    $out .= "</span>\n";

    return $out;
  }
}

# VMENU for vertical menu of hyperlinks..

class VMENU extends MENU
{
  var $style,$itemized;

  function VMENU($it = array(),$st = '',$itm = 1)
  {
    $this->MENU($it);
    $this->style = $st;
    $this->itemized = $itm;
  }

  function set_style($st)
  {
    $this->style = $st;
  }

  function set_itemized($it)
  {
    $this->itemized = $it;
  }

  function prepare()
  {
    $ul = $sul = $li = '';
    if($this->itemized==1)
    {
      $ul  = '<ul>';
      $sul = '</ul>';
      $li  = '<li>';
    }
    elseif($this->itemized==2)
    {
      $ul = '<ol>';
      $sul = '</ol>';
      $li = '<li>';
    }
    elseif(preg_match('/^[AaiI]$/',$this->itemized))
    {
      $ul = '<ul type="'.$this->itemized.'">';
      $sul = '</ul>';
      $li  = '<li>';
    }
    $out = "<span class=\"".$this->style."\">\n$ul\n";
    for($i = 0; $i < $this->count; $i++)
    {
      if($this->items[$i]['href'])
        $out .= "$li<a href=\"".$this->items[$i]['href']."\">".$this->items[$i]['name']."</a>".$this->items[$i]['opt']."<br>\n";
      else
        $out .= $this->items[$i]['name'].$this->items[$i]['opt']."\n";
    }
    $out .= "$sul\n</span>\n";

    return $out;
  }
}

# SELECTBOX for <select> in forms

class SELECTBOX extends MENU
{
  var $size,$multiple,$name;
  var $style;

  function SELECTBOX($items = array(),$nm='',$sz=10,$mt=0)
  {
    $this->MENU($items);
    
    $this->name     = $nm;
    $this->size     = $sz;
    $this->multiple = $mt;
    $this->style    = 'selectbox';
  }

  function set_selected($sel = array())
  {
    while(list($k,$v) = each($sel))
      $this->items[$v]['selected'] = 1;
  }

  function reset_selected()
  {
    for($i = 0; $i < $this->count; $i++)
      $this->items[$i]['selected'] = 0;
  }

  function set_size($sz)
  {
    $this->size = $sz;
  }

  function set_multiple($mt)
  {
    $this->multiple = $mt;
  }

  function set_name($nm)
  {
    $this->name = $nm;
  }

  function set_style($st)
  {
    $this->style = $st;
  }

  function prepare()
  {
    $out = "<select name=\"".$this->name."\" class=\"".$this->style."\" size=".$this->size 
      .($this->multiple?' multiple':'').">\n";

    for($i = 0; $i < $this->count; $i++)
      $out .= "<option value=\"".$this->items[$i]['href']."\"".($this->items[$i]['selected']?' selected':'').">".$this->items[$i]['name']."\n";

    $out .= "</select>\n";
    return $out;
  }
}

# simple checkbox class, no javascript for autochecking of subcathegories

class CHBOX extends MENU
{
  var $name;
  var $style;

  function CHBOX($items = array(),$nm='')
  {
    $this->MENU($items);
    
    $this->name     = $nm;
    $this->style    = 'checkbox';
  }

  function set_checked($sel = array())
  {
    while(list($k,$v) = each($sel))
      $this->items[$v]['checked'] = 1;
  }

  function set_name($nm)
  {
    $this->name = $nm;
  }

  function set_style($st)
  {
    $this->style = $st;
  }

  function prepare()
  {
    for($i = 0; $i < $this->count; $i++)
      $out .= "<input type=\"checkbox\" name=\"$this->name\" class=\"".$this->style."\" value=\"".$this->items[$i]['href']."\"".($this->items[$i]['checked']?' checked':'').">".$this->items[$i]['name']."<br>\n";

    return $out;
  }
}

# class TABLE implements various tables

class TABLE
{
  var $width,$style,$cellspacing,$cellpadding,$border,$bgcolor,$valign;
  var $rows_cnt,$cols_cnt;
  var $rows;

  function TABLE($wi='100%',$st='',$cs=0,$cp=0,$bo=0,$bg='')
  {
    $this->width       = $wi;
    $this->style       = $st;
    $this->cellspacing = $cs;
    $this->cellpadding = $cp;
    $this->border      = $bo;
    $this->bgcolor     = $bg;
    $this->rows        = array();
    $this->rows_cnt    = count($this->rows);
    $this->cols_cnt    = 0;
  }

  function how_many()
  {
    return $this->rows_cnt;
  }

  function set_parameters($par = array())
  {
    while(list($k,$v) = each($par))
      $this->$k = $v;
  }

  function add_row($row = array())
  {
    $i = 0;
    while(list($k,$v) = each($row))
    {
      if(is_array($v))
      {
        $this->rows[$this->rows_cnt][$i]['cont'] = $v['cont'];
        $this->rows[$this->rows_cnt][$i]['attr'] = $v['attr'];
      }
      else
        $this->rows[$this->rows_cnt][$i]['cont'] = $v;

      $i++;
    }
    if($i > $this->cols_cnt) $this->cols_cnt = $i;
    $this->rows_cnt++;
  }
  
  function add_rows($rows = array())
  {
    while(list($k,$v) = each($rows))
      $this->add_row($v);
  }

  function prepare()
  {
    $rows = '';
    if($this->style)
      $css = " class=\"$this->style\"";
    else
      $css = '';

    if($this->valign)
      $valign = " valign=\"$this->valign\"";

    for($i = 0; $i < $this->rows_cnt; $i++)
    {
      $rows .= "<tr$css$valign>\n";

      for($j = 0; $j < $this->cols_cnt; $j++)
      {
        if(isset($this->rows[$i][$j]['cont']))
          $rows .= "<td ".($this->rows[$i][$j]['attr']).">\n".($this->rows[$i][$j]['cont'])."</td>\n";
      }
      
      $rows .= "</tr>\n";
    }

    return "<table width=\"$this->width\" cellspacing=\"$this->cellspacing\" cellpadding=\"$this->cellpadding\" valign=\"$this->valign\" border=\"$this->border\"$css".($this->bgcolor?" bgcolor=\"$this->bgcolor\"":'').">
    $rows
    </table>\n";
  }
}

class BOXEDTABLE extends TABLE
{
  function BOXEDTABLE($wi='',$st='',$cs=2,$cp=0,$bo=0,$bg='')
  {
    $this->TABLE($wi,$st,$cs,$cp,$bo,$bg);
  }

  function prepare()
  {
    return parent::prepare();
  }
}

class JS_CHECKBOX
{
  var $items,$count,$itemized;

  function JS_CHECKBOX()
  {
    $this->items=array();
    $this->count=0;
  }

  function set_itemized($it = 0)
  {
    $this->itemized=$it;
  }

  function how_many()
  {
    return $this->count;
  }

  function add_item($name='',$items=array())
  {
    $this->items[$this->count]['name']=$name;
    reset($items);
    for($i=0;$i<count($items);)
    {
      list($k,$v) = each($items);
      $this->items[$this->count][$i++] = $v;
    }
    $this->count++;
  }

  function prepare($i)
  {
    $plus = 0;
    for($j=0;$j<$i;$j++)
      $plus+=count($this->items[$j]);

    $offset = $i+$i?$plus+1:1;
    $limit = count($this->items[$i]);
    $limit and $limit--;

    if($this->itemized)
    {
      $dl  = '';
      $ul  = '<ul>';
      $sul = '</ul>';
      $li  = '<li>';
      $sdl = '';
      $dt  = '';
      $dd  = '';
    }
    else
    {
      $dl  = '<dl>';
      $ul  = '';
      $sul = '';
      $li  = '';
      $sdl = '</dl>';
      $dt  = '<dt>';
      $dd  = '<dd>';
    }

    $out = "$dl\n";
    $out .= "$dt<input type=\"checkbox\" onclick=\"mark_sub($i,$offset,$limit);\"><strong>".$this->items[$i]['name']."</strong>".
    "$dd$ul\n";
    for($j=0;$j<count($this->items[$i])-1;$j++)
      $out .= "$li<input type=\"checkbox\" name=\"".$this->items[$i]['name']."[$j]\" value=1 onclick=\"mark_cat($i,$offset,$limit);\"> ".$this->items[$i][$j]."<br>\n";

    $out .= "$sdl$sul\n";
    return $out;
  }

  function prepare_js($form_num=0)
  {
    $out = "<script language=\"JavaScript\">
    marked = new Array($this->count);
    marked_cnt = new Array($this->count);
    function mark_sub(cat,offset,limit)
    {
      for(i=offset;i<offset+limit;i++)
        document.forms[$form_num].elements[i].checked=marked[cat]?0:1;
      marked[cat]=marked[cat]?0:1;
    }

    function mark_cat(cat,offset,limit)
    {
      count = 0;
      for(i=offset;i<offset+limit;i++)
        count += document.forms[$form_num].elements[i].checked?1:0;

      if(count/limit>0.8)
      {
        document.forms[$form_num].elements[offset-1].checked=1;
        marked[cat]=1;
      }
      else
      {
        document.forms[$form_num].elements[offset-1].checked=0;
        marked[cat]=0;
      }
    }
    </script>\n";
    return $out;
  }
}

?>
