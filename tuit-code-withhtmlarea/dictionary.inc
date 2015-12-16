<?

$sound_img = 'speaker.gif';

function find_records(&$table,$find_in,$query)
{
  global $udata,$format;
  $table->add_row(array(array('attr'=>'colspan=9','cont'=>'<hr>')));
  $sql = "select * from dict1_2 where ";
  switch($find_in)
  {
    case 'w1':
      $sql .= 'd1_word1';
      break;
    case 'w2':
      $sql .= 'd1_word2';
      break;
    default:
      $sql .= 'd1_word1';
  }
  $sql .= " like '".addslashes($query)."' order by d1_word1,d1_word2";
  @$res = mysql_query($sql);
  if(mysql_num_rows($res))
  {
    $table->add_row(array("Czech","Unit","Note 1","Note 2","Note 3","Sound","English","Usage notes",$format=='short'?'':'<small><small>Select</small></small>'));
    while(@$row = mysql_fetch_array($res))
      $table->add_row(array((is_cont_admin($udata['admin'])?"<a href=\"?section=dictionary&record=$row[d1_id]\">":'').$row['d1_word1'].(is_cont_admin($udata['admin'])?"</a>":''),$row['d1_lesson'],$row['d1_note1'],$row['d1_note2'],$row['d1_note3'],((!empty($row['d1_sound']))?"<a href=\"sound/dictionary/$row[d1_sound]\"><img src=\"img/$sound_gi$sound_gif\" border=0></a>":''),$row['d1_word2'],$row['d1_usage'],$format=='short'?'':"<input type=\"checkbox\" name=\"records[$row[d1_id]]\" value=1 class=\"checkbox\">"));
    if(is_cont_admin($udata['admin']))
      $inp = '<input type="submit" name="delete" value="   Delete selected   " class="buttoninbox">';
    elseif($format != 'short')
      $inp = '<input type="submit" name="add_to_voc" value="   Add selected to the vocabulary   " class="buttoninbox">';
    $table->add_row(array(array('attr'=>'align="center" colspan=9','cont'=>$inp)));
  }
  else
    $table->add_row(array(array('attr'=>'align="center" colspan=9','cont'=>'<font color="#FF0000">No record found.</font>')));
}

function make_voc_select($v_id='')
{
  global $udata,$voc_name;
  $voc_name = '';
  $vbox = new SELECTBOX;
  $vbox->set_multiple(0);
  $vbox->set_size(1);
  $vbox->set_name('voc_id');
  @$res = mysql_query("select sd_id,sd_name from st_dict where sd_student_id=$udata[id] order by sd_name");
  $cnt = 0;
  while(@$row = mysql_fetch_array($res))
  {
    $vbox->add_item($row['sd_name'],$row['sd_id']);
    if($row['sd_id'] == $v_id)
    {
      $vbox->set_selected(array($cnt));
      $voc_name = $row['sd_name'];
    }
    $cnt++;
  }

  return $vbox;
}

$page->insert_to_middle("<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"dictionary\">");
if(is_cont_admin($udata['admin']))
{
  $page->topnav->add_items(array("Browse dictionary"=>"?section=dictionary&show_list=1","Create dictionary"=>"?section=dictionary"));
  $page->set_top('Dictionary');
}
elseif(!$udata['admin'])
{
  $page->topnav->add_items(array("Browse dictionary"=>"?section=dictionary&show_list=1","Create vocabulary list"=>"?section=dictionary","Use vocabulary list"=>"?section=dictionary&exam=1"));
  $page->set_top('Vocabulary');
}

if(!isset($find_in))
  $find_in = 'w1';

if(isset($show_list))
{
  #$page->set_top("Dictionary");
  if(!isset($char))
  {
    $table = new TABLE;
    $table->set_parameters(array('cellspacing'=>5));
    $table->add_row(array(array('attr'=>'colspan=2','cont'=>"A dictionary containing all Czech words used in exercises and conversation examples on this site.<br>&nbsp;")));
    $table->add_row(array(array('attr'=>'valign="top" align="left"','cont'=>"<h3>Czech&nbsp;-&nbsp;English</h3>
    | <a href=\"?section=dictionary&dir=1&show_list=1&char=a\">a</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=·\">·</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=b\">b</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=c\">c</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=Ë\">Ë</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=d\">d</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=e\">e</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=f\">f</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=g\">g</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=h\">h</a> |
    <br>
    | <a href=\"?section=dictionary&dir=1&show_list=1&char=ch\">ch</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=i\">i</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=j\">j</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=k\">k</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=l\">l</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=m\">m</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=n\">n</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=o\">o</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=p\">p</a> |
    <br>
    | <a href=\"?section=dictionary&dir=1&show_list=1&char=q\">q</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=r\">r</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=¯\">¯</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=s\">s</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=π\">ö</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=t\">t</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=ª\">ù</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=u\">u</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=˙\">˙</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=v\">v</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=w\">w</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=x\">x</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=y\">y</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=z\">z</a> |
    <a href=\"?section=dictionary&dir=1&show_list=1&char=æ\">û</a> |
    <br><br>&nbsp;"),
    array('attr'=>'valign="top" align="left"','cont'=>"<h3>English&nbsp;-&nbsp;Czech</h3>
    | <a href=\"?section=dictionary&dir=2&show_list=1&char=a\">a</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=b\">b</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=c\">c</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=d\">d</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=e\">e</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=f\">f</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=g\">g</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=h\">h</a> |
    <br>
    | <a href=\"?section=dictionary&dir=2&show_list=1&char=i\">i</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=j\">j</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=k\">k</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=l\">l</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=m\">m</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=n\">n</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=o\">o</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=p\">p</a> |
    <br>
    | <a href=\"?section=dictionary&dir=2&show_list=1&char=q\">q</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=r\">r</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=s\">s</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=t\">t</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=u\">u</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=v\">v</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=w\">w</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=x\">x</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=y\">y</a> |
    <a href=\"?section=dictionary&dir=2&show_list=1&char=z\">z</a> |
    ")));

    $page->add_to_middle("<center>".$table->prepare()."</center><br><br>");
  }
  elseif(is_numeric($dir))
  {
    if(strlen($char)>1 and $char != 'ch')
      $char = substr($char,0,1);

    if($dir==1)
    {
      $w = 'd1_word1';
      $list = "
      | <a href=\"?section=dictionary&dir=1&show_list=1&char=a\">a</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=·\">·</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=b\">b</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=c\">c</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=Ë\">Ë</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=d\">d</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=e\">e</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=f\">f</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=g\">g</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=h\">h</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=ch\">ch</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=i\">i</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=j\">j</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=k\">k</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=l\">l</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=m\">m</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=n\">n</a> |
        <br>
      | <a href=\"?section=dictionary&dir=1&show_list=1&char=o\">o</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=p\">p</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=q\">q</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=r\">r</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=¯\">¯</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=s\">s</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=ö\">ö</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=t\">t</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=ù\">ù</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=u\">u</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=˙\">˙</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=v\">v</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=w\">w</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=x\">x</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=y\">y</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=z\">z</a> |
        <a href=\"?section=dictionary&dir=1&show_list=1&char=û\">û</a> |
        ";
    }
    elseif($dir==2)
    {
      $w = 'd1_word2';
      $list= "
      | <a href=\"?section=dictionary&dir=2&show_list=1&char=a\">a</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=b\">b</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=c\">c</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=d\">d</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=e\">e</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=f\">f</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=g\">g</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=h\">h</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=i\">i</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=j\">j</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=k\">k</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=l\">l</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=m\">m</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=n\">n</a> |
        <br>
      | <a href=\"?section=dictionary&dir=2&show_list=1&char=o\">o</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=p\">p</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=q\">q</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=r\">r</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=s\">s</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=t\">t</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=u\">u</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=v\">v</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=w\">w</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=x\">x</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=y\">y</a> |
        <a href=\"?section=dictionary&dir=2&show_list=1&char=z\">z</a> |
        ";
    }
    else
      $w = 'd1_word1';

    @$res = mysql_query("select * from dict1_2 where $w like '$char%'");
    $table = new TABLE;
    $table->set_parameters(array('width'=>' ','cellpadding'=>2));
    $table->add_row(array(array('attr'=>'bgcolor="#FFCC00"','cont'=>"&nbsp;<font size=5>$char</font>"),array('attr'=>'colspan=4 bgcolor="#FFCC00"','cont'=>$list)));
    while(@$row = mysql_fetch_array($res))
    {
      if($dir==1)
        $table->add_row(array("<strong>$row[d1_word1]</strong>, <i>".$row['d1_lesson'].", ".$row['d1_note1'].", <strong>$row[d1_note2]</strong>, ".$row['d1_note3']."</i>".($row['d1_sound']?" <a href=\"sound/dictionary/$row[d1_sound]\"><img src=\"img/$sound_img\" border=0></a>; ":'; ').$row['d1_word2']."; ".$row['d1_usage']));
      elseif($dir==2)
        $table->add_row(array("<strong>$row[d1_word2]</strong>",$row['d1_lesson'],$row['d1_note1'],"<strong>$row[d1_note2]</strong>",$row['d1_note3'],$row['d1_sound']?"<a href=\"sound/dictionary/$row[d1_sound]\"><img src=\"img/$sound_img\" border=0></a>":'',$row['d1_word1'],$row['d1_usage']));
    }
    $page->add_to_middle($table->prepare());
  }
}
else
{
/* section for administrators - maintaining database */
  if(is_cont_admin($udata['admin']))
  {
    #$page->set_top('Dictionary');
    $sub = "Insert into database";
    if((isset($create_new) or isset($replace) and is_numeric($record)) and isset($word1) and isset($word1))
    {
      @$res = mysql_query("replace into dict1_2 values(".(is_numeric($record)?$record:'NULL').",'".addslashes($word1)."','".addslashes($word2)."','".addslashes($note1)."','".addslashes($note2)."','".addslashes($note3)."','".addslashes($lesson)."','".addslashes($sound)."','".addslashes($usage)."')");
      if($res)
      {
        $page->add_to_middle("<h4>New record saved.</h4>");
        $record = '';
      }
      else
        error_f("Cannot save new record!");
    }
  
    if(isset($delete) and is_array($records))
    {
      $ls = '';
      $cnt = 0;
      while(list($k,$v) = each($records))
        if($v == 1)
          $ls .= ($cnt++?',':'').$k;
      @$res = mysql_query("delete from dict1_2 where d1_id in ($ls)");
      if($res)
      {
        $page->add_to_middle("<h4>Record successfuly deleted.</h4>");
        $record = '';
      }
      else
        error_f("Cannot delete record!");
    }

    $table = new TABLE;
    $ttable = new BOXEDTABLE;
    $ttable->set_parameters(array('style'=>'subtable','cellpadding'=>2));

    $btable = new BOXEDTABLE;
    $btable->set_parameters(array('style'=>'subtable','cellpadding'=>2));
    $btable->add_row(array(array('attr'=>'align="center" colspan=9','cont'=>'<h4>Find a record</h4>')));
    $btable->add_row(array(array('attr'=>'colspan=9','cont'=>"Find <input type=\"text\" name=\"query\" size=40 maxlength=40 value=\"$query\" class=\"boxedfield\">")));
    $btable->add_row(array(array('attr'=>'colspan=9','cont'=>"In <input type=\"radio\" name=\"find_in\" value=\"w1\"".($find_in=='w1'?' checked':'')." class=\"radio\"> Czech word&nbsp;&nbsp;&nbsp;<input type=\"radio\" name=\"find_in\" value=\"w2\"".($find_in=='w2'?' checked':'')." class=\"radio\"> English word&nbsp;&nbsp;&nbsp;<input type=\"submit\" name=\"find\" value=\"   Start search   \" class=\"buttoninbox\">")));
    if(isset($find) and !empty($query) and isset($find_in))
      find_records($btable,$find_in,"%$query%");

    if(is_numeric($record))
    {
      $page->add_to_middle("<input type=\"hidden\" name=\"record\" value=\"$record\">");
      @$res = mysql_query("select * from dict1_2 where d1_id=$record");
      @$row = mysql_fetch_array($res);
      $w1 = $row['d1_word1'];
      $w2 = $row['d1_word2'];
      $n1 = $row['d1_note1'];
      $n2 = $row['d1_note2'];
      $n3 = $row['d1_note3'];
      $le = $row['d1_lesson'];
      $us = $row['d1_usage'];
      $so = $row['d1_sound'];
      $sub = "Replace in the database";
    }
    $ttable->add_row(array(array('attr'=>'align="center" colspan=4','cont'=>'<h4>Create a new record</h4>')));
    $ttable->add_row(array("Czech word",array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"word1\" size=64 maxlength=100 class=\"boxedfield\" value=\"$w1\">")));
    $ttable->add_row(array("English word",array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"word2\" size=64 maxlength=100 class=\"boxedfield\" value=\"$w2\">")));
    $ttable->add_row(array("Notes",array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"note1\" size=20 maxlength=20 class=\"boxedfield\" value=\"$n1\">&nbsp;&nbsp;&nbsp;<input type=\"text\" name=\"note2\" size=20 maxlength=20 class=\"boxedfield\" value=\"$n2\">&nbsp;&nbsp;&nbsp;<input type=\"text\" name=\"note3\" size=20 maxlength=20 class=\"boxedfield\" value=\"$n3\">")));
    $ttable->add_row(array("Unit","<input type=\"text\" name=\"lesson\" size=20 maxlength=20 class=\"boxedfield\" value=\"$le\">","Usage","<input type=\"text\" name=\"usage\" size=37 maxlength=200 class=\"boxedfield\" value=\"$us\">"));
    $ttable->add_row(array("Sound file name",array('attr'=>'colspan=3','cont'=>"<input type=\"text\" name=\"sound\" size=20 maxlength=20 class=\"boxedfield\" value=\"$so\">")));
    $ttable->add_row(array(array('attr'=>'align="center" colspan=4','cont'=>'<input type="submit" name="create_new" value="   '.$sub.'   " class="buttoninbox">&nbsp;<input type="reset" value="   Clear input fields   " class="buttoninbox">')));

    $table->add_row(array($ttable->prepare()));
    $table->add_row(array("&nbsp;"));
    if($btable)
      $table->add_row(array($btable->prepare()));

    $page->add_to_middle($table->prepare());
  }
/* next section for students */
  elseif(!$udata['admin'])
  {
    if($exam==1 or isset($use_voc))
    {
      if(!is_numeric($word_num))
        $word_num = 20;
      if(!isset($voc_id) or !isset($word_num))
      {
        $vbox = make_voc_select($voc_id);
        $table = new BOXEDTABLE;
        $table->set_parameters(array('style'=>'subtable'));
        $table->add_row(array(array('attr'=>'align="center"','cont'=>'<strong>Select the vocabulary from the list</strong><br><br>'.$vbox->prepare()."<br><br><strong>How many words?</strong><br><br><input type=\"text\" name=\"word_num\" size=5 maxlength=5  value=\"$word_num\" class=\"boxedfield\"><br><input type=\"submit\" name=\"use_voc\" value=\"   OK   \" class=\"buttoninbox\">")));
        $page->add_to_middle($table->prepare());
      }
      else
      {
        $table = new BOXEDTABLE;
        $table->set_parameters(array('style'=>'subtable'));
        @$res = mysql_query("select d1_word1,d1_word2 from sd_d1,dict1_2 where sdd_d1_id=d1_id and sdd_sd_id=$voc_id order by d1_word1");
        $cnt = 0;
        while(@$row = mysql_fetch_array($res))
        {
          $table->add_row(array("<strong>$row[d1_word1]</strong>","<input type=\"text\" size=20 maxlength=100 name=\"st_inp$cnt\" class=\"boxedfield\" onblur=\"if(form.st_inp$cnt.value=='$row[d1_word2]') form.chbox$cnt.checked=1; else form.chbox$cnt.checked=0;\"> <input type=\"button\" value=\" ? \" onclick=\"form.st_inp$cnt.value='$row[d1_word2]';\" class=\"buttoninbox\"> <input type=\"button\" value=\" X \" onclick=\"form.st_inp$cnt.value=''; form.chbox$cnt.checked=0;\" class=\"buttoninbox\"> <input type=\"checkbox\" name=\"chbox$cnt\" value=1 class=\"checkbox\">"));
          $cnt++;
        }

        $page->add_to_middle($table->prepare());
      }
    }
    else
    {
      if(isset($create_new_voc) and isset($voc_name))
      {
        @$res = mysql_query("insert into st_dict set sd_student_id=$udata[id],sd_name='".addslashes($voc_name)."'");
        if($res)
          $voc_id = mysql_insert_id();
      }
      elseif(isset($delete_voc) and is_numeric($voc_id))
      {
        @$res = mysql_query("select sd_student_id from st_dict where sd_id=$voc_id");
        @$row = mysql_fetch_array($res);
        $st_id = $row['sd_student_id'];
        if($st_id == $udata['id'])
        {
          @mysql_query("delete from st_dict where sd_student_id=$udata[id] and sd_id=$voc_id");
          @mysql_query("delete from sd_d1 where sdd_sd_id=$voc_id");
        }
      }
    #$page->set_top('Vocabulary');
      $rtable = new BOXEDTABLE;
      $rtable->set_parameters(array('style'=>'subtable'));

      $vbox = make_voc_select($voc_id);

      $rtable->add_row(array(array('attr'=>'align="left"','cont'=>'<input type="text" name="voc_name" value="Name of the vocabulary" size=30 maxlength=100 class="boxedfield"> <input type="submit" name="create_new_voc" value="   Create a new vocabulary   " class="buttoninbox">')));
      if($vbox->how_many())
        $rtable->add_row(array($vbox->prepare()."<input type=\"submit\" name=\"select_voc\" value=\"Select the vocabulary\" class=\"buttoninbox\"> <input type=\"submit\" name=\"delete_voc\" value=\"Delete the vocabulary\" class=\"buttoninbox\"> <input type=\"submit\" name=\"use_voc\" value=\"Use the vocabulary\" class=\"buttoninbox\">"));

      if(is_numeric($voc_id) and !isset($delete_voc) and !empty($voc_name))
      {
        if(isset($add_to_voc) and is_array($records))
        {
          $ls = '';
          $cnt = 0;
          while(list($k,$v) = each($records))
          {
            if($v==1)
              $ls .= ($cnt++?',':'')."($voc_id,$k)";
          }

          $sql = "replace into sd_d1 values $ls";
          @mysql_query($sql);
        }

        @$res = mysql_query("select d1_word1,d1_word2 from sd_d1,dict1_2 where sdd_d1_id=d1_id and sdd_sd_id=$voc_id order by d1_word1");
        if(@mysql_num_rows($res))
        {
          $ls = '';
          $cnt = 0;
          while(@$row = mysql_fetch_array($res))
            $ls .= ($cnt++?', ':'')."<small><strong>$row[d1_word1]</strong> <small>($row[d1_word2])</small></small>";
          $rtable->add_row(array($ls));
        }

        $rttop = "Vocabulary $voc_name";
      }
      else
        $rttop = "New vocabulary";

      $ltable = new BOXEDTABLE;
      $ltable->set_parameters(array('style'=>'subtable'));
      $ltable->add_row(array(array('attr'=>'align="center" colspan=6','cont'=>'<h4>Find a record</h4>')));
      $ltable->add_row(array(array('attr'=>'colspan=6','cont'=>"Find <input type=\"text\" name=\"query\" size=40 maxlength=40 value=\"$query\" class=\"boxedfield\">")));
      $ltable->add_row(array(array('attr'=>'colspan=6','cont'=>"In <input type=\"radio\" name=\"find_in\" value=\"w1\"".($find_in=='w1'?' checked':'')." class=\"radio\"> Czech word&nbsp;&nbsp;&nbsp;<input type=\"radio\" name=\"find_in\" value=\"w2\"".($find_in=='w2'?' checked':'')." class=\"radio\"> English word&nbsp;&nbsp;&nbsp;<input type=\"submit\" name=\"find\" value=\"   Start search   \" class=\"buttoninbox\">")));
      if(isset($find) and !empty($query) and isset($find_in))
        find_records($ltable,$find_in,"$query%");

      $table = new TABLE;
      if($format == 'short')
        $table->add_row(array("<h4>Results of search for <em>$query</em></h4><br>".$ltable->prepare()));
      else
        $table->add_row(array(array('attr'=>'width="50%" valign="top" align="center"','cont'=>"<h4>Search</h4>".$ltable->prepare()),array('attr'=>'width="50%" valign="top" align="center"','cont'=>"<h4>$rttop</h4>".$rtable->prepare())));
      $page->add_to_middle("<center>".$table->prepare()."</center>");
    }
  }
}

$page->add_to_middle("</form>");

return 123;
?>
