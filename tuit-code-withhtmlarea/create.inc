<?

#error_reporting(2047);

include('ex_classes.inc');

if(!am_i_cont_admin())
  return 123;

$show_menu=0;

if(!is_numeric($limit) or $limit<0) $limit = 20;
if(!is_numeric($offset) or $offset<0) $offset = 0;

if($desc == 1 or $desc == 'desc') $desc = 'desc';
else $desc = '';

if($create == 'exercise' and (isset($type) or isset($exercise)))
{
  if(!is_numeric($difficulty)) $difficulty=1;
  $table = new BOXEDTABLE;
  $table->set_parameters(array('style'=>'subtable'));

  if(is_numeric($exercise) and $exercise>0)
  {
    @$res = mysql_query("select *,et_short from exercises,exercise_types where et_id=ex_type and ex_id=$exercise");
    @$ex_row = mysql_fetch_array($res);
    $type = $ex_row['et_short'];
  }
  else
    $exercise=0;

  if($type == 'complete' or $type == 'explanation')
  {
    /*
    include_once('ew/class.editworks.php');
    $myEW = new ew;
    $myEW->setName("story");
    $myEW->setDocumentType(EW_DOC_TYPE_SNIPPET);
    $myEW->setTextAreaDimensions(80,20);
    */
    $type_num = ($type=='complete')?COMPLETE:EXPLANATION;
    if($action == 'save')
    {
//      $story = $myEW->getValue();
      $story = ereg_replace('\\[(#|checkbox)\\]','[$1$0, checkbox]',$story);
      $story = ereg_replace('\\[(\\*|radio)([^]]*)\\]','[$0, radio\\2]',$story);
      $story = ereg_replace('\\[([[:alnum:]\\(\\)]*/[[:alnum:]\\(\\)/]+)\\]','[$1, select:\\1]',$story);
      $story = ereg_replace('\\[(\\$[[:digit:]]+)[[:space:]]*(:[[:alnum:]\\(\\)/]+)\\]','[\\1, select\\2]',$story);
      $story = ereg_replace('\\[(\\$[[:digit:]]+,)[[:space:]]*([[:digit:]]+,[[:digit:]]+[^]]*)\\]','[\\1textarea \\2]',$story);
      $story = ereg_replace('\\[([[:digit:]]+,[[:digit:]]+[^]]*)\\]','[$1, textarea \\1]',$story);
      $story = ereg_replace('\\[(\\$[[:digit:]]+,)[[:space:]]*([[:digit:]]+[^]]*)\\]','[\\1text \\2]',$story);
      $story = ereg_replace('\\[([[:digit:]]+[^]]*)\\]','[$1, text \\1]',$story);
      $story = ereg_replace('\\[\\]','[$1, text 20]',$story);
      $story = ereg_replace('\\[(\\$[[:digit:]]+)[[:space:]]*(:[^]]+)\\]','[\\1, text\\2]',$story);
      $story = ereg_replace('\\[([^]+0-9$/%^{"\\-]+)\\]','[$1, text:\\1]',$story);
      if($story[0]=='[')
        $story = " $story";
      @$res = mysql_query("replace into exercises values(".($exercise?$exercise:'NULL').",$type_num,'".addslashes($name)."','".addslashes($description)."',".$udata['id'].",'".addslashes($task)."','".addslashes($story)."',$difficulty,NULL)");
      $id = mysql_insert_id();
      if(!$res)
        $err = 'Cannot save new exercise!';
    }
    else
    {
      $def_task = $ex_row?$ex_row['ex_task']:(($type=='complete')?'Complete with':'Read');
 //     $myEW->setValue($ex_row['ex_story']);
      $form = "
      Enter text in the field below.
      <!--
      <ul>
      <li>You can write anything including HTML code. To create a pull-down
      menu with choices use this: <strong>[na/nad/(pod)]</strong>.
      In the exercise the student will be given a list with these options -
      'na', 'nad' and 'pod'. (<i>pod</i> was marked as the correct answer
      by ( ).)
      <li>To create an empty input field, write only <strong>[ ]</strong>.
      Indicate the length of field by a number. Enclose the correct answer
      in ( ). For example: [3(pod)] creates a field three characters long
      with the correct answer being <i>pod</i>.
      <li>To create a larger text area enclose two numbers in [ ]. E.g.
      [50,10] will create a text area 50 characters wide and 10 lines long.
      </ul>-->
      <br/>
      <textarea name=\"story\" cols=80 rows=20 wrap=\"virtual\" class=\"largetext\" id=\"htmlarea\">$ex_row[ex_story]</textarea>
      <script type=\"text/javascript\" defer=\"1\">
/*         HTMLArea.replace('htmlarea');*/
        initEditor();
      </script>
      ";
      $page->add_to_head('
      <script type="text/javascript">
        _editor_url = "../htmlarea/";
        _editor_lang = "en";
      </script>
      <script type="text/javascript" src="../htmlarea/htmlarea.js"></script>
      <script type="text/javascript">
        HTMLArea.loadPlugin("TableOperations");
        function initEditor()
        {
          // create an editor for the "ta" textbox
          editor = new HTMLArea("htmlarea");

          // register the TableOperations plugin with our editor
          editor.registerPlugin(TableOperations);

          editor.generate();
          return false;
        }
      </script>
      ');
//      $form .= "
//      <!--INSERTEWCONTROLHERE--> ";
//      $INSERTEWCONTROL = "myEW";
    }
  }
  elseif($type == 'qanda')
  {
    if($action == 'save')
    {
      @$res = mysql_query("replace into exercises values(".($exercise?$exercise:'NULL').",5,'".addslashes($name)."','".addslashes($description)."',".$udata['id'].",'".addslashes($task)."','".addslashes($story)."',$difficulty,NULL)");
      $id = mysql_insert_id();
      $sql = 'replace into ex_qanda values ';
      $cnt = 0;
      while(list($k,$v) = each($question))
        $sql .= ($cnt++?',':'')."(".($exercise?$k:'NULL').",$id,'".addslashes($v)."','".addslashes($answer[$k])."')";
      @$res = mysql_query($sql);
      if(!$res)
        $err = 'Cannot save new exercise!';
    }
    elseif(is_numeric($m) or $exercise)
    {
      if($exercise)
      {
        @$res = mysql_query("select * from ex_qanda where qa_ex_id=$exercise order by qa_id");
        @$m = mysql_num_rows($res);
        while(@$qa_rows[] = mysql_fetch_array($res))
          ;
      }

      $qtable = new TABLE;
      $qtable->set_parameters(array('cellspacing'=>2));
      $def_task = $ex_row?$ex_row['ex_task']:'Read the story and answer the questions';
      $qtable->add_row(array('No.','<strong>Question</strong>','<strong>Sample answer</strong>'));
      for($i = 0; $i < $m; $i++)
        $qtable->add_row(array(($i+1).")"," <input type=\"text\" size=40 maxlength=160 name=\"question[".$qa_rows[$i]['qa_id']."]\"".($exercise?" value=\"".$qa_rows[$i]['qa_question']."\"":'')." class=\"boxedfield\">","<input type=\"text\" size=40 maxlength=160 name=\"answer[".$qa_rows[$i]['qa_id']."]\"".($exercise?" value=\"".$qa_rows[$i]['qa_answer']."\"":'')." class=\"boxedfield\">"));
      $form = "
      Enter a text for students read (optional):<br>
      <textarea name=\"story\" cols=80 rows=5 wrap=\"virtual\" class=\"largetext\">$ex_row[ex_story]</textarea>
      <br>\n".
      $qtable->prepare();
    }
    else
      $want_more_parameters=1;
  }
  elseif($type == 'match')
  {
    if($action == 'save')
    {
      @$res = mysql_query("replace into exercises values(".($exercise?$exercise:'NULL').",4,'".addslashes($name)."','".addslashes($description)."',".$udata['id'].",'".addslashes($task)."','".addslashes($story)."',$difficulty,NULL)");
      $id = mysql_insert_id();
      $sql = 'replace into ex_match values ';
      $cnt = 0;
      while(list($k,$v) = each($first))
        $sql .= ($cnt++?',':'')."(".($exercise?$k:'NULL').",$id,'".addslashes($v)."','".addslashes($second[$k])."')";
      @$res = mysql_query($sql);
      if(!$res)
        $err = 'Cannot save new exercise!';
    }
    elseif(is_numeric($m) or $exercise)
    {
      if($exercise)
      {
        @$res = mysql_query("select * from ex_match where ma_ex_id=$exercise order by ma_id");
        @$m = mysql_num_rows($res);
        while(@$ma_rows[] = mysql_fetch_array($res))
          ;
      }

      $mtable = new TABLE;
      $mtable->set_parameters(array('cellspacing'=>2));
      $def_task = $ex_row?$ex_row['ex_task']:'Choose the right alternative';
      $mtable->add_row(array('No.','<strong>First part</strong>','<strong>Second part</strong>'));
      for($i = 0; $i < $m; $i++)
        $mtable->add_row(array(($i+1).")"," <input type=\"text\" size=40 maxlength=160 name=\"first[".$ma_rows[$i]['ma_id']."]\"".($exercise?" value=\"".$ma_rows[$i]['ma_first']."\"":'')." class=\"boxedfield\">","<input type=\"text\" size=40 maxlength=160 name=\"second[".$ma_rows[$i]['ma_id']."]\"".($exercise?" value=\"".$ma_rows[$i]['ma_second']."\"":'')." class=\"boxedfield\">"));
      $form = $mtable->prepare();
    }
    else
      $want_more_parameters=1;
  }
  elseif($type == 'choose')
  {
    if(!is_numeric($same_opt)) $same_opt=0;
    if($action == 'save')
    {
      mysql_query("replace into exercises values(".($exercise?$exercise:'NULL').",2,'".addslashes($name)."','".addslashes($description)."',".$udata['id'].",'".addslashes($task)."','".addslashes($story)."',$difficulty,'same_opt:$same_opt')");
      $id = mysql_insert_id();
      $sql = "replace into ex_choose values ";
      $cnt = 0;
      while(list($k,$v) = each($question))
      {
        $ch = '';
        if(!$same_opt or $i==0)
        {
          for($j=0;$j<$n;$j++)
            $ch .= $choice[$k][$j]."\n";
        }
        $sql .= ($cnt++?',':'')."(".($exercise?$k:'NULL').",$id,'".addslashes($question[$k])."','".addslashes($ch)."',".addslashes($correct[$k]).")";
      }
      @$res = mysql_query($sql);

      if(!$res)
        $err = 'Cannot save new exercise!';
    }
    elseif((is_numeric($m) and is_numeric($n)) or $exercise)
    {
      if($exercise)
      {
        @$res = mysql_query("select * from ex_choose where ch_ex_id=$exercise order by ch_id");
        @$m = mysql_num_rows($res);
        while(@$row = mysql_fetch_array($res))
          $ch_rows[] = $row;

        $options = get_options($ex_row['ex_options']);
        $same_opt = $options['same_opt'];
        while(list($k,$v) = each($ch_rows))
        {
          $ch_id[] = $v['ch_id'];
          $ch_questions[] = $v['ch_question'];
          $ch_choices[] = explode("\n",$v['ch_choices']);
          $ch_correct[] = $v['ch_correct'];
        }
        $n = count($ch_choices[0])-1;
      }

      $ctable = new TABLE;
      $ctable->set_parameters(array('cellspacing'=>2));
      $def_task = $ex_row?$ex_row['ex_task']:'Choose the right alternative';
      $pars[0] = array('attr'=>'colspan=2','cont'=>'<strong>Question</strong>');
      for($i = 1; $i <= $n; $i++)
        $pars[$i] = "<strong>Choice $i</strong>";

      $ctable->add_row($pars);
      for($i = 0; $i < $m; $i++)
      {
        $pars = array(($i+1).') ',"<input type=\"text\" size=40 maxlength=160 name=\"question[".$ch_id[$i]."]\"".($exercise?" value=\"".$ch_questions[$i]."\"":'')." class=\"boxedfield\">");
        if( (!$same_opt and $i>0) or $i==0 )
        {
          for($j = 0; $j < $n; $j++)
            $pars[$j+2] = "<input type=\"text\" size=".($n>4?'10':intval(40/$n))." maxlength=160 name=\"choice[".($ch_id?$ch_id[$i]:$i)."][$j]\"".($exercise?" value=\"".$ch_choices[$i][$j]."\"":'')." class=\"boxedfield\">";
          $ctable->add_row($pars);
          $pars = array('',"<strong>Correct</strong>");
        }
        for($j = 0; $j < $n; $j++)
          $pars[$j+2] = "<input type=\"radio\" name=\"correct[".($ch_id?$ch_id[$i]:$i)."]\" value=$j".($j==$ch_correct[$i]?' checked':'')." class=\"radio\">";
        $ctable->add_row($pars);
      }
      $form = "
      Enter questions in the first column. Enter options in the other columns
      and tick the correct one.<br><br>
      <input type=\"hidden\" name=\"same_opt\" value=\"$same_opt\">\n".
      $ctable->prepare();
    }
    else
      $want_more_parameters=1;
  }
  elseif($type == 'crossword')
  {
    if($action == 'save')
    {
      $inp = "$m*$n\n$solution\n".addslashes($translation)."\n";
      for($i=0; $i<$m; $i++)
      {
        $inp .= $hint[$i].':';
        for($j = 0; $j < $n; $j++)
          $inp .= $letter[$i][$j]?$letter[$i][$j]:'-';
        $inp .= "\n";
      }
      $sql = "replace into exercises values(".($exercise?$exercise:'NULL').",3,'".addslashes($name)."','".addslashes($description)."',".$udata['id'].",'".addslashes($task)."','".addslashes($inp)."',$difficulty,NULL)";
      @mysql_query($sql);
      $id = mysql_insert_id();
      if(!$res)
        $err = 'Cannot save new exercise!';
    }
    elseif((is_numeric($m) and is_numeric($n)) or $exercise)
    {
      if($exercise)
      {
        $mXn = strtok($ex_row['ex_story'],"\n");
        $sol = strtok("\n");
        $sol_trans = strtok("\n");
        while($c_row[] = strtok("\n"))
          ;
        list($m,$n) = explode('*',$mXn);
        while(list($k,$v) = each($c_row))
        {
          $parts = explode(':',$v);
          $hints[$k] = $parts[0];
          for($i = 0; $i < strlen($parts[1]); $i++)
            $letters[$k][$i] = $parts[1][$i];
        }
      }

      $ctable = new TABLE;
      $ctable->set_parameters(array('width'=>''));
      $def_task = $ex_row?$ex_row['ex_task']:'Solve and translate the puzzle';

      $form = "
      Enter letter as they would appear in a complete puzzle. The fields
      you don't fill in will not be displayed thus creating the crossword
      look. Enter clues in the first column. Check off the column containing
      the <i>tajenka</i>.<br><br>\n";
      
      $pars[0] = '';
      for($i = 0; $i < $n; $i++)
        $pars[$i+1] = "<input type=\"radio\" name=\"solution\" value=$i".($i==$sol?' checked':'')." class=\"radio\">\n";
      $ctable->add_row($pars);

      for($i = 0; $i < $m; $i++)
      {
        $pars[0] = "<input type=\"text\" name=\"hint[$i]\" maxlength=160 value=\"$hints[$i]\" class=\"boxedfield\">&nbsp;-&nbsp;\n";
        for($j = 0; $j < $n; $j++)
          $pars[$j+1] = "<input type=\"text\" name=\"letter[$i][$j]\" size=2 maxlength=2".($exercise?" value=\"".$letters[$i][$j]."\"":'')." class=\"boxedfield\">\n";

        $ctable->add_row($pars);
      }
      $form .= ($ctable->prepare())."<br>
      Translation of <i>tajenka</i>: <input type=\"text\" name=\"translation\" size=40 maxlength=160 value=\"$sol_trans\" class=\"boxedfield\">\n";
    }
    else
      $want_more_parameters=1;
  }

  if($want_more_parameters)
  {
    $subtable = new TABLE;
    $form = "Please enter parameters of the exercise<br><br>\n";
    switch ($type)
    {
      case 'qanda':
        $subtable->add_row(array('How many questions?','<input type="text" size=2 maxlength=2 name="m" class="boxedfield">'));
        break;
      case 'choose':
        $subtable->add_row(array('How many questions?','<input type="text" size=2 maxlength=2 name="m" class="boxedfield">'));
        $subtable->add_row(array('How many choices?','<input type="text" size=2 maxlength=2 name="n" class="boxedfield">'));
        $subtable->add_row(array('All questions have the same options?','<input type="checkbox" name="same_opt" value=1 class="checkbox">'));
        break;
      case 'match':
        $subtable->add_row(array('How many sentences?','<input type="text" size=2 maxlength=2 name="m" class="boxedfield">'));
        break;
      case 'crossword':
        $subtable->add_row(array('How many rows?','<input type="text" size=2 maxlength=2 name="m" class="boxedfield">'));
        $subtable->add_row(array('How many columns?','<input type="text" size=2 maxlength=2 name="n" class="boxedfield">'));
        break;
    }
    $form .= $subtable->prepare();
    $submit = 'Continue';
  }
  else
  {
    $sbox=new SELECTBOX;
    #$sbox->add_items(array($diff_strs[BASIC]=>BASIC,$diff_strs[INTERMEDIATE]=>INTERMEDIATE,$diff_strs[ADVANCED]=>ADVANCED));
    while(list($k,$v) = each($diff_strs))
      $sbox->add_item($v,$k);
    $sbox->set_size(1);
    $sbox->set_name('difficulty');
    $sbox->set_style('selectbox');
    if($ex_row)
      $sbox->set_selected(array($ex_row['ex_difficulty']-1));
    $submit = 'Save';
    $form = "
    <input type=\"hidden\" name=\"m\" value=\"$m\">
    <input type=\"hidden\" name=\"n\" value=\"$n\">
    <input type=\"hidden\" name=\"action\" value=\"save\">
    <input type=\"hidden\" name=\"exercise\" value=\"$exercise\">
    Enter the name of the exercise here:<br>
    <input type=\"text\" size=80 maxlength=80 name=\"name\" value=\"$ex_row[ex_name]\" class=\"boxedfield\"><br><br>
    Enter the description of the exercise:<br>
    <input type=\"text\" size=80 maxlength=255 name=\"description\" value=\"$ex_row[ex_description]\" class=\"boxedfield\"><br><br>
    Enter the task the students are expected to accomplish:<br>
    <input type=\"text\" size=80 maxlength=80 name=\"task\" value=\"$def_task\" class=\"boxedfield\"><br><br>
    Select difficulty: ".
    $sbox->prepare().
    "<br><br>\n".
    $form;
  }

  if($action == 'save' and !$err)
  {
    $submit = 'Finish';

    $ex_tab = new BOXEDTABLE;
    $ex_tab->set_parameters(array('style'=>'subtable_ex'));
    if(!$exercise and is_numeric($id))
      $exercise = $id;

    $ex = ex_inst($exercise);
    if($ex)
    {
      $ex_tab->add_row(array($ex->prepare_test()));
      $ex_tab->add_row(array("<br><br><a href=\"?section=create&create=exercise&exercise=$exercise\">Back to editing</a>"));
    }

    $ex_cat = array();
    @$res = mysql_query("select ec_et_id from exercise_categories where ec_ex_id=$exercise");
    while(@$row = mysql_fetch_array($res))
      $ex_cat[] = $row['ec_et_id'];

    @$res = mysql_query('select * from exercise_types where et_id>100 order by et_name');
    if(@$cnt = mysql_num_rows($res))
    {
      $sbox = new SELECTBOX;
      $sbox->set_multiple(1);
      $sbox->set_name('exercise_categories[]');
      $sbox->set_style('selectbox');
      $cnt = 0;
      while(@$row=mysql_fetch_array($res))
      {
        $sbox->add_item($row['et_name'],$row['et_id']);
        if(in_array($row['et_id'],$ex_cat))
          $sbox->set_selected(array($cnt));
        $cnt++;
      }

      $sbox->set_size($cnt>10?'10':$cnt);
      $form = "
      <input type=\"hidden\" name=\"action\" value=\"save_categories\">
      <input type=\"hidden\" name=\"id\" value=\"$id\">
      Choose categories that will be linked with this exercises or create new:<br><br>\n".
      $sbox->prepare()."<input type=\"text\" name=\"new_cat\" size=20 maxlength=50 class=\"boxedfield\" value=\"New category\">\n";
    }
    else
    {
      $page->insert_to_middle('<center>Exercise saved</center><br><br>'.($ex_tab->how_many()?$ex_tab->prepare():'').'<br>');
      if($exercise)
      {
        $difficulty = '';
        $section = 'edit';
        $edit = 'exercises';
        include('edit.inc');
        return 123;
      }
      $show_menu=1;
    }
  }

  if($action != 'save_categories')
  {
    $table->add_row(array($form));
    $table->add_row(array("<br><input type=\"submit\" value=\"   $submit   \" class=\"buttoninbox\">"));
  }
  elseif(is_array($exercise_categories) or isset($new_cat))
  {
    if(isset($new_cat) and $new_cat!='New category')
    {
      @mysql_query("insert into exercise_types set et_name='".addslashes($new_cat)."'");
      $new_et_id = mysql_insert_id();
    }

    $sql = 'replace into exercise_categories values ';
    $cnt = 0;
    while(list($k,$v) = each($exercise_categories))
      $sql .= ($cnt++?',':'')."($id,$v)";

    if($new_et_id)
      $sql .= ($cnt?',':'')."($id,$new_et_id)";

    if(@$res = mysql_query($sql))
      $page->insert_to_middle('<center>Categories saved.</center>');
    else
      $err = 'Could not save exercise categories!';

    $show_menu=1;
  }

  if($err)
    error_f($err);
  
  if(!$show_menu)
  {
    if($ex_tab and $ex_tab->how_many())
    {
      $metatab = new TABLE;
      $metatab->add_row(array(array('attr'=>'width="50%" align="center" valign="top"','cont'=>$ex_tab->prepare()),array('attr'=>'width="50%" align="center" valign="top"','cont'=>$table->prepare())));
    }
    else
      $metatab = &$table;

    $page->set_top('Create exercise');
    $page->add_to_middle("
    <center>
    <h3>Enter data to compose the exercise.</h3>
    If you need help, refer to
    <a href=\"http://www.bohemica.com/czechonline/coursemanual/index.htm\">
    online manual</a>.<br><br>
    <form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"create\">
    <input type=\"hidden\" name=\"create\" value=\"exercise\">
    <input type=\"hidden\" name=\"type\" value=\"$type\">".
    (($metatab and $metatab->how_many())?$metatab->prepare():'')."
    </form>
    </center>\n");
  }
}
elseif($create == 'set')
{
  $table = new BOXEDTABLE;
  $table->set_parameters(array('style'=>'subtable','cellpadding'=>5));

  if($action == 'save' and isset($name) and is_numeric($difficulty))
  {
    if(!is_numeric($type) or $type<0)
      $type = 0;

    @$res = mysql_query("replace into sets values(".(is_numeric($set)?$set:'NULL').",$type,'".addslashes($name)."',$udata[id],$difficulty)");
    if(@$set = mysql_insert_id())
      $page->insert_to_middle("<center>
    Unit base saved. <a href=\"?section=edit&edit=exercises&set=$set&ed_cont=1\"><strong>Edit contents</strong></a>.</center>");
  }

  if(is_numeric($set))
  {
    @$res = mysql_query("select * from sets where set_id=$set");
    @$set_row = mysql_fetch_array($res);
  }

  $dbox = new SELECTBOX;
  $dbox->set_style('selectbox');
  #$dbox->add_items(array($diff_strs[BASIC]=>BASIC,$diff_strs[INTERMEDIATE]=>INTERMEDIATE,$diff_strs[ADVANCED]=>ADVANCED));
  while(list($k,$v) = each($diff_strs))
    $dbox->add_item($v,$k);
  $dbox->set_size(1);
  $dbox->set_name('difficulty');
  $dbox->set_selected(array($set_row['set_difficulty']-1));

  $cbox = new SELECTBOX;
  $cbox->set_style('selectbox');
  $cbox->set_name('type');
  @$res = mysql_query("select * from set_types order by st_name");
  @$cnt = mysql_num_rows($res);
  $cbox->set_size($cnt>5?5:$cnt);
  $cnt = 0;
  while(@$row = mysql_fetch_array($res))
  {
    $cbox->add_item($row['st_name'],$row['st_id']);
    if($row['st_id'] == $set_row['set_type'])
      $cbox->set_selected(array($cnt));
    $cnt++;
  }

  $table->add_row(array('Name',"<input type=\"text\" name=\"name\" size=80 maxlength=80 value=\"$set_row[set_name]\" class=\"boxedfield\">"));
  $table->add_row(array('Category',$cbox->how_many()?$cbox->prepare():'(none)'));
  $table->add_row(array('Difficulty',$dbox->prepare()));
  $table->add_row(array('&nbsp;','<input type="submit" name="save" value="   Save   " class="buttoninbox">'));

  $page->set_top('Compose unit');
  $page->add_to_middle("
  <center>
  <h3>Enter data to compose the unit.</h3>
  <form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"create\">
  <input type=\"hidden\" name=\"create\" value=\"set\">
  <input type=\"hidden\" name=\"action\" value=\"save\">
  <input type=\"hidden\" name=\"set\" value=\"$set\">".
  $table->prepare()."
  </form>
  <center>\n");
}
elseif($create == 'set_types' or $create=='exercise_types')
{
  switch($create)
  {
    case 'set_types':
      $top = 'Add unit types';
      $table_top_r = 'Units of this type';
      $id_name = 'st_id';
      $sql_select = 'select st_id as id,st_name as name,count(set_type) as cnt from set_types left join sets on st_id=set_type group by st_id order by st_name';
      break;
    case 'exercise_types':
      $top = 'Add exercise categories';
      $table_top_r = 'Exercises of this category';
      $id_name = 'et_id';
      $sql_select = 'select et_id as id,et_name as name,count(ec_et_id) as cnt from exercise_types left join exercise_categories on et_id=ec_et_id where et_id>100 group by et_id order by et_name';
      $sql_insert = "'',";
      break;
  }

  $page->set_top($top);

  if($action == 'save' and isset($new_types))
  {
    if($types = split('[[:space:]]*;[[:space:]]*',$new_types))
    {
      $sql = "insert into $create values ";
      $cnt = 0;
      while(list($k,$v) = each($types))
        $sql .= ($cnt++?',':'')."(NULL,$sql_insert '".addslashes($v)."')";

      if(!@mysql_query($sql))
        error_f('Cannot save new types! Perhaps duplicate entry.');
    }
  }
  elseif($action == 'delete' and isset($delete_types))
  {
    $sql = "delete from $create where $id_name in";
    $cnt = 0;
    $ls = '';
    while(list($k,$v) = each($delete_types))
      $ls .= ($cnt++?',':'').$v;

    $sql .= "($ls)";
    $res = mysql_query($sql);
    if($res and $create == 'exercise_types')
      $res = @mysql_query("delete from exercise_categories where ec_et_id in($ls)");
    if(!$res)
      error_f('Cannot delete selected types!'."$sql");
  }

  if(@$res = mysql_query($sql_select) and @mysql_num_rows($res))
  {
    $rtable = new BOXEDTABLE;
    $rtable->set_parameters(array('style'=>'subtable','cellspacing'=>5));
    $rtable->add_row(array('<strong>Name</strong>',"<strong>$table_top_r</strong>"));
    $rtable->add_row(array(array('attr'=>'colspan=2 align="center"','cont'=>'<hr>')));
    while(@$row = mysql_fetch_array($res))
      $rtable->add_row(array("<input type=\"checkbox\" name=\"delete_types[$row[st_id]]\" value=\"$row[id]\" class=\"checkbox\">".$row['name'],array('attr'=>'align=right','cont'=>$row['cnt']?$row['cnt']:'0 ')));

    $rtable->add_row(array(array('attr'=>'colspan=2 align="center"','cont'=>'<input type="submit" value="   Delete selected   " class="buttoninbox">')));
  }

  $ltable = new BOXEDTABLE;
  $ltable->set_parameters(array('style'=>'subtable','cellspacing'=>0));
  $ltable->add_row(array("
  <form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"create\">
    <input type=\"hidden\" name=\"create\" value=\"$create\">
    <input type=\"hidden\" name=\"action\" value=\"save\">
    <center>Enter new types separated by semicolon (one entry can be up to 50 characters long).<br><br>
    <textarea name=\"new_types\" rows=10 cols=51 wrap=off class=\"largetext\"></textarea>
    <br><br>
    <input type=\"submit\" value=\"   Save   \" class=\"buttoninbox\">
  </form>
    </center>\n"));

  $table = new TABLE;
  $table->set_parameters(array('valign'=>'top'));
  $table->add_row(array(array('attr'=>'width="50%" align="center"','cont'=>"<h3>Add new types</h3>\n".$ltable->prepare()),array('attr'=>'width="50%" align="center"','cont'=>"<h3>Existing types</h3>\n<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"create\">
  <input type=\"hidden\" name=\"create\" value=\"$create\">
  <input type=\"hidden\" name=\"action\" value=\"delete\">"
  .($rtable?$rtable->prepare():'(none)')."</form>\n")));

  $page->add_to_middle("<center>
  ".(($create == 'set_types')?"
    Adding a new type of units does not add any specific functionality
    to the system, it only allows you to group the units to logical units
    (types). ":''). "When entering multiple new types, be careful not to include
    type with the name that already exists as it won't be added, but 
    neither will be any following entry. It is not recommended to
    delete type that an existing unit/exercise uses.<br><br>".
    $table->prepare()."</center>\n");
}
else
  $show_menu=1;

if($show_menu)
{
  $page->set_top('Create menu');
  $menu = new VMENU;
  $menu->set_style('middle');
  $menu->set_itemized('A');
  $menu->add_item('<li><strong>Exercise</strong>','');

/*
   as there are only 5 exercises types, we define them statically..
   the same reason applies as with permission types (see inc/pre.inc).
*/

  $h = '?section=create&create=exercise&type';
  $submenu = new VMENU;
  $submenu->set_itemized(2);
  $submenu->set_style('middle');
  $submenu->add_items(array(
    $ex_strs[COMPLETE]=>"$h=complete",
    $ex_strs[CHOOSE]=>"$h=choose",
    $ex_strs[CROSSWORD]=>"$h=crossword",
    $ex_strs[MATCH]=>"$h=match",
    $ex_strs[QANDA]=>"$h=qanda",
    '<br>'=>'',
    $ex_strs[EXPLANATION]=>"$h=explanation"
    ));

  $menu->add_item($submenu->prepare()."<br>",'');
  $menu->add_item('<strong>Unit of exercises</strong><br>','?section=create&create=set');
  $menu->add_item('<strong>Exercise categories</strong><br>','?section=create&create=exercise_types');
  $menu->add_item('<strong>Unit categories</strong><br>','?section=create&create=set_types');
  $page->add_to_middle($menu->prepare());
}

return 123;
?>
