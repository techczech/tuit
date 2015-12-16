<?

# inc/ must be there!

include('inc/mi_classes.inc');

# class PAGE implements page layout

class PAGE
{
  var $title,$top,$middle,$bottom,$logged,$head,$top_text;
  var $disp_bohemica;
  var $topnav,$botnav,$mainnav;
  var $js_in_head;

  function PAGE($tt = '')
  {
    $this->logged = 0;
    $this->topnav      = new HMENU;
    $this->botnavleft  = new HMENU;
    $this->botnavright = new HMENU;
    $this->head='';
    $this->set_middle('');
    $this->set_title($tt);
    $this->topnav->add_items(array('<strong>Back</strong>'=>'javascript:history.go(-1);'));

    $this->botnavleft->set_style('botbot');

    $this->botnavright->set_style('botbot');
    $this->botnavright->set_separator('');
    $this->botnavright->add_items(array('<strong>'.$GLOBALS['server_name'].'</strong>'=>$GLOBALS['server_addr']));

    $this->set_top('');
    $this->disp_bohemica=1;
  }

  function set_logged($val)
  {
    $this->logged=$val;
  }

  function is_logged()
  {
    return $this->logged;
  }

  function set_title($tt)
  {
    $this->title = $tt;
  }

  function set_top($tt)
  {
    $this->top_text=$tt;
  }

  function set_top_table()
  {
    $table = new TABLE;
    $tt = $this->top_text;
    $table->add_rows(array(
      array(array('attr'=>'class="toptop"','cont'=>"<span class=\"toptop\">\n<strong>$this->title</strong>\n</span>\n")),
      array(array('attr'=>'class="topbot"','cont'=>"<span class=\"topbot\">\n&nbsp; $tt\n</span>\n")),
      array(array('attr'=>'class="topnav"','cont'=>'&nbsp;'.$this->topnav->prepare()))
      ));
      
    $this->top = $table->prepare();
  }

  function set_middle($mt)
  {
    $table = new TABLE;
    $subtable = new TABLE;

    $subtable->set_parameters(array('style'=>'middle'));
    $subtable->add_row(array(array('attr'=>'class="middle"',
    'cont'=>"<span class=\"middle\">
<!--middle_table_start-->
    $mt
<!--middle_table_end-->
  </span>\n")));
    
    $table->set_parameters(array('cellspacing'=>'5','cellpadding'=>'5','style'=>'middle'));
    $table->add_row(array(array('attr'=>'colspan=3','cont'=>'&nbsp;')));
    $table->add_row(array(array('attr'=>'width=5','cont'=>'&nbsp;'),
      $subtable->prepare(),array('attr'=>'width=5','cont'=>'&nbsp;')));

    $table->add_row(array(array('attr'=>'colspan=3','cont'=>'&nbsp;')));

    $this->middle = $table->prepare();
  }
  
  function insert_to_middle($mt)
  {
    $this->middle=ereg_replace('(<!--middle_table_start-->)',"\\1\n$mt",$this->middle);
  }

  function add_to_middle($mt)
  {
    $this->middle=ereg_replace('(<!--middle_table_end-->)',"$mt\n\\1",$this->middle);
  }

  function set_bottom($btl,$btr)
  {
    $table = new TABLE;
    
    $table->add_row(array(array('attr'=>'width="100%" align="right" class="bottop" colspan=2','cont'=>'<span class="bottop">'.date('j. n. Y')."</span>&nbsp;")));

    $table->add_row(array(array('attr'=>'class="botbot" align="left"','cont'=>$btl),array('attr'=>'class="botbot" align="right"','cont'=>$btr)));

    $this->bottom = $table->prepare();
  }

  function add_to_head($ht)
  {
    $this->head .= $ht;
  }

  function display($notepad=0)
  {
    global $udata,$MODULES;
    
    $this->add_to_head("<title>".$this->title."</title>\n");

    if(!$notepad and $this->logged)
    {
      if(is_array($MODULES))
        $href = 'tuit/';
      else
        $href = '';
      $this->js_in_head.="function openNotepad()
      {
        notepadWin=window.open('$href?section=notepad&wrapped=0&action=make_new&URL=".urlencode($_SERVER["REQUEST_URI"])."','Notepad','toolbar=no,location=no,directories=no,status=yes,menubar=no,resizable=yes,scrollbars=yes,width=420,height=420');
      }\n";
      $this->topnav->add_item('<a href="#" onclick="openNotepad();">Make note</a>','');
    }

    if(!$this->top and $this->disp_bohemica)
      $this->set_top_table();

    if($this->js_in_head)
      $this->add_to_head("<script language=\"javascript\">\n".$this->js_in_head."</script>");
    
    if($this->disp_bohemica)
      $this->botnavleft->add_items(array('<strong>Top</strong>'=>'#top'));
    if($this->logged)
    {
      $this->botnavleft->add_item('Main menu','?section=main');
      if(!$udata['admin'])
        $this->botnavleft->add_items(array('Exercises'=>'?section=exercises','Results'=>'?section=exercises&show_complete=1','User settings'=>'?section=change_udata'));
      if(is_teacher($udata['admin']))
        $this->botnavleft->add_items(array('Assign'=>'?section=assign','Correct/Review'=>'?section=correct'));
      if(is_cont_admin($udata['admin']))
        $this->botnavleft->add_item('Create','?section=create');
      $this->botnavleft->add_items(array('<strong>Logout</strong>'=>'?section=logout'));
    }
    else
      $this->botnavleft->add_items(array('Login form'=>'index.php?section=login','Account request'=>'index.php?section=acct_request','Public access'=>'index.php?section=public_access'));

    if($this->disp_bohemica)
      $this->set_bottom('&nbsp;'.$this->botnavleft->prepare(),$this->botnavright->prepare().'&nbsp;');

    return "<html>
<head>
<meta http-equiv=\"Content-language\" content=\"cs\">
<meta http-equiv=\"Content-type\" content=\"text/html; charset=windows-1250\">\n"
.$this->head
."
</head>
<body>
<a name=\"top\"></a>\n"
.$this->top
.$this->middle
.$this->bottom
."
</body>
</html>";
  }
}

?>
