<?

define('ANSWERS',1);
define('RADIOS',2);
define('MAX_EXPL_IN_LIST',3);

$indicators['correct']   = "<img src=\"img/check_small.gif\">";
$indicators['incorrect'] = "<img src=\"img/cross_small.gif\">";

class EXERCISE
{
  var $ex_parameters;
  var $ex_id, /*$ex_type, $ex_name, $ex_creator, $ex_task, */ $ex_story /*, $ex_options, $ex_difficulty*/;
  var $ready;
  var $options;
  var $answers;
  var $correct;

  var $categories;
  var $explanations;
  var $input_offset,$radios_offset;

  function EXERCISE($id)
  {
    $this->input_offset = 0;
    if(is_numeric($id))
    {
      $this->ex_id=$id;
      $this->ready=0;
      $this->get_parameters();
    }
    else
      $this->ready=-1;
  }

  function get_options($str)
  {
    $opts = array();
    $tok = strtok($str,"\n");
    while($tok)
    {
      list($l,$r) = explode(':',$tok);
      $opts[$l] = $r;
      $tok = strtok("\n");
    }
    return $opts;
  }

  function get_parameters()
  {
    @$res = mysql_query("select ex_type,ex_name,ex_creator,ex_task,ex_difficulty,ex_options,ex_description from exercises where ex_id=$this->ex_id");
    if(@mysql_num_rows($res) != 1)
    {
      $this->ready=-1;
      return;
    }
    @$row = mysql_fetch_array($res);
    $this->parameters=$row;
    $this->options=$this->get_options($this->parameters['ex_options']);
  }

  function get_categories()
  {
    @$res = mysql_query("select et_name, ec_et_id from exercise_categories, exercise_types where et_id=ec_et_id and ec_ex_id=".$this->ex_id);
    while(@$row = mysql_fetch_array($res))
      $this->categories[]=$row;
  }

  function read_explanations()
  {
    if(!is_array($this->categories))
      $this->get_categories();

    $ls = '';
    $cnt = 0;
    while(list($k,$v) = each ($this->categories))
      $ls .= ($cnt++?',':'').$v['ec_et_id'];

    if($this->parameters['ex_type']!=EXPLANATION)
      @$res = mysql_query("select ex_name, ex_id,et_name from exercises,exercise_categories,exercise_types where ex_id=ec_ex_id and et_id=ec_et_id and ex_type=".EXPLANATION." and ec_et_id in($ls) order by et_name,ex_name");
    else
      @$res = mysql_query("select ex_name, ex_id,et_name from exercises, exercise_categories,exercise_types where ex_id=ec_ex_id and et_id=ec_et_id and ex_type<".EXPLANATION." and ec_et_id in($ls) order by et_name,ex_name");

    while(@$row = mysql_fetch_array($res))
      $this->explanations[] = $row;
  }

  function get_explanations()
  {
    if(!is_array($this->explanations))
      $this->read_explanations();
    
    return $this->explanations;
  }

  function get_data()
  {
    @$res = mysql_query("select ex_story from exercises where ex_id=$this->ex_id");
    if(@mysql_num_rows($res) != 1)
    {
      $this->ready=-1;
      exit;
    }
    @$row = mysql_fetch_array($res);
    $this->ex_story=$row['ex_story'];
    if($this->parameters['ex_type']==COMPLETE or $this->parameters['ex_type']==CROSSWORD)
      $this->ready=1;
  }

  function set_answers($an=array())
  {
    $this->answers=$an;
  }

  function read_answers($st=0,$as_id=0)
  {
    if(!$st)
      return -1;

    switch($this->parameters['ex_type'])
    {
      case MATCH:
        $pre = 'am';
        $tab = 'an_match';
        break;
      case CHOOSE:
        $pre = 'ac';
        $tab = 'an_choose';
        break;
      case QANDA:
        $pre = 'aq';
        $tab = 'an_qanda';
    }

    if($this->parameters['ex_type'] == COMPLETE)
    {
      @$res = mysql_query("select aco_answer,aco_type,aco_correct,aco_points from an_complete,answers where aco_an_id=an_id and an_ex_id=".$this->ex_id." and an_who_id=$st and an_as_id=$as_id order by aco_id");
      if(!mysql_num_rows($res) or mysql_num_rows($res)>2)
        return -1;
      while(@$row = mysql_fetch_array($res))
      {
        if($row['aco_type']==ANSWERS)
        {
          $this->answers = unserialize($row['aco_answer']);
          $this->correct = unserialize($row['aco_correct']);
          $this->pt      = unserialize($row['aco_points']);
        }
        elseif($row['aco_type']==RADIOS)
        {
          $this->radios_ans = unserialize($row['aco_answer']);
          $this->radios_cor = unserialize($row['aco_correct']);
        }
      }
    }
    elseif($this->parameters['ex_type'] == CROSSWORD)
    {
      @$res = mysql_query("select acr_answer,acr_correct from an_crossword,answers where acr_an_id=an_id and an_ex_id=".$this->ex_id." and an_who_id=$st and an_as_id=$as_id order by acr_id");
      if(mysql_num_rows($res)!=1)
        return -1;
      while(@$row = mysql_fetch_array($res))
      {
        $this->answers = unserialize($row['acr_answer']);
        $this->correct = unserialize($row['acr_correct']);
      }
    }
    else
    {
      $sql = "select ".$pre."_answer,".$pre."_correct $n from $tab,answers where ".$pre."_an_id=an_id and an_ex_id=".$this->ex_id." and an_who_id=$st and an_as_id=$as_id order by ".$pre."_id";
      @$res = mysql_query($sql);
      if(!mysql_num_rows($res))
        return -1;
      while(@$row = mysql_fetch_array($res))
      {
        $this->answers[] = $row[$pre."_answer"];
        $this->correct[] = $row[$pre."_correct"];
      }
    }
    @$res = mysql_query("select date_format(an_corr_when,'%e. %c. %Y') as cwhen, an_points,an_comment,name from answers,users where an_corr_by=id and an_ex_id=".$this->ex_id." and an_who_id=$st and an_as_id=$as_id");
    if(mysql_num_rows($res)==1)
      $this->is_corrected=1;
    @$row = mysql_fetch_array($res);
    $this->corrected['when']=$row['cwhen'];
    $this->corrected['points']=$row['an_points'];
    $this->corrected['comment']=$row['an_comment'];
    $this->corrected['name']=$row['name'];

    return 0;
  }
}

class EX_COMPLETE extends EXERCISE
{
  var $contents,$riddles;
  var $cont_cnt,$rid_cnt;
  var $radios;
  var $radios_ans;

  function EX_COMPLETE($ob)
  {
    $this->ex_id = $ob->ex_id;
    $this->parameters = $ob->parameters;
    $this->contents = array();
  }

  function get_data()
  {
    parent::get_data();
  }

  function set_radios_ans($ra=array())
  {
    $this->radios_ans=$ra;
  }

  function explode_contents()
  {
    $len  = strlen($this->ex_story);
    $tok  = strtok($this->ex_story,'[');
    $len2 = strlen($tok);
    $i    = 1;
    while($len2<=$len)
    {
      $this->contents[] = $tok;
      $tok = strtok(($i%2)?']':'[');
      $len2 += strlen($tok)+1;
      $i++;
    }
    $this->cont_cnt=$i-1;
  }

  function expand_tag($tag,$cnt,$show_an=0,$correct=0,$st_watch=0)
  {
    global $indicators,$javatags,$page;
    if($tag[0] == '%')
    {
      $r = substr($tag,1);
      return "<input type=\"text\" name=\"param[$cnt]\" value=\"$r\" size=\"".(strlen($r)+10)."\" class=\"boxedfield_ex\">";
    }
    elseif($tag[0] == '^')
    {
      list($l,$r) = explode(':',substr($tag,1));
      return "<a href=\"?section=dictionary&find=1&format=short&query=".urlencode($r?$r:$l)."\" target=\"_new\">".($l?$l:$r)."</a>";
    }
    elseif($tag[0] == '"')
    {
      list($l,$r) = explode(':',substr($tag,1));
      return "<a href=\"#\" onclick=\"alert('$r');\">$l</a>";
    }
    elseif($tag[0] == '{')
    {
      $javatags[]=$cnt;
      list($l,$r) = explode(':',substr($tag,1));
      if(empty($r) and strncasecmp($l,'name=',5))
      // textovy policko, buttonky [?] [X], napoveda se zobrazi primo v policku..
        return "<input type=\"text\" name=\"j_inp_$cnt\" onblur=\"if(form.j_inp_$cnt.value=='$l') form.chbox$cnt.checked=1; else form.chbox$cnt.checked=0;\" class=\"boxedfield_ex\"> <input type=\"button\" value=\" ? \" onclick=\"form.j_inp_$cnt.value='$l';\" class=\"buttoninbox\"> <input type=\"button\" value=\" X \" onclick=\"form.j_inp_$cnt.value=''; form.chbox$cnt.checked=0;\" class=\"buttoninbox\"> <input type=\"checkbox\" name=\"chbox$cnt\" value=1 class=\"checkbox\">";
      elseif(!strncasecmp($l,'name=',5))
      // pojmenovana textarea (target pro napovedu)
      // [{name=t1]
        return "<textarea name=\"j_inp_".substr($l,5)."\" class=\"largetext_ex\"></textarea>";
      elseif(!strncasecmp($l,'target=',7))
      {
      // textovy policko, pri kliknuti na [?] se napoveda zobrazi v textarea jmenem "to_co_je_za_target="
      // [{target=t1:toto je napoveda]
        $t = substr($l,7);
        return "<input type=\"text\" name=\"j_inp_$cnt\" class=\"boxedfield_ex\"> <input type=\"button\" value=\" ? \" onclick=\"form.j_inp_$t.value='$r';\" class=\"buttoninbox\"> <input type=\"button\" value=\" X \" onclick=\"form.j_inp_$cnt.value='';\" class=\"buttoninbox\">";
      }
    }
    elseif($tag[0]=='+')
    {
      return "<input type=\"button\" value=\"Show all\" class=\"buttoninbox\" onclick=\"show_all();\">";
    }
    elseif($tag[0]=='-')
    {
      $buf = "function clear_all()\n{\n";
      while(list($k,$v) = each($javatags))
        $buf .= "  document.ex_form.j_inp_$v.value=''; document.ex_form.chbox$v.checked=0;\n";
      $buf .= "}\n";
      $page->js_in_head.=$buf;
      return "<input type=\"button\" value=\"Clear all\" class=\"buttoninbox\" onclick=\"clear_all();\">";
    }

    $out = '';
    list($l,$r) = split(':',$tag,2);
    list($pts,$pars) = split('[[:space:]]*,[[:space:]]*',$l,2);
    if($pts[0] == '$')
      $this->riddles[$cnt]['points'] = substr($pts,1);
    else
      $this->riddles[$cnt]['points'] = 1;
    list($n,$mods) = split('[[:space:]]+',$pars,2);
    str_replace(' ','',$mods);
    $perbox = new SELECTBOX;
    $perbox->set_multiple(0);
    $perbox->set_style('selectbox');
    $perbox->set_name("percents[$cnt]");
    $perbox->set_size(1);
    for($i=0;$i<=100;$i+=10)
      $perbox->add_item($i.'%',$i);

/* for compatibility with old version - selectbox: */
    if(!$r and !$pars)
    {
      $n = 'select';
      $r = $l;
    }

    switch($n)
    {
      case 'text':
      	if($r[0] == '(')
          $r = substr($r,1);
        if($show_an)
        {
          if($this->correct[$cnt] or ($correct and $this->answers[$cnt]==$r))
            $corrClass = ' class="correctanswer"';
          elseif($correct or $st_watch)
            $corrClass = ' class="wronganswer"';
          else
            $corrClass = ' class="submitted"';
          $out .= "<strong><span$corrClass>".$this->answers[$cnt]."</span></strong>";
          if($st_watch && count($this->pt))
            $out .= " (".sprintf("%1.1f pts",$this->pt[$cnt]).")".($this->correct[$cnt]?'':($r?" - <span class=\"correctanswer\">$r</span>":""));
          elseif($correct)
          {
            if($r and $this->answers[$cnt]==$r)
              $perbox->set_selected(array(10));
            elseif(!empty($r))
              $more = "(<span class=\"correctanswer\">$r</span>)";
            if(!$st_watch)
              $out .= "<input type=\"checkbox\" name=\"complete_cor[$cnt]\" value=1 class=\"checkbox\"".(($r and $this->answers[$cnt]==$r)?' checked':'').">"."<input type=\"hidden\" name=\"points[$cnt]\" value=".$this->riddles[$cnt]['points'].">".$perbox->prepare();
            $out .= $more;
          }
        }
        else
        {
          if(!$mods and strlen($r))
             $mods = strlen($r);
          $out .= "<input type=\"text\" name=\"answers[".($cnt+$this->input_offset)."]\"".(is_numeric($mods)?" size=$mods maxlength=$mods":'')." class=\"boxedfield_ex\" value=\"".$this->answers[$cnt]."\">";
        }
        break;
      case 'textarea':
        list($cols,$rows) = split(',',$mods);
        if($show_an)
        {
          $out .= "<textarea cols=$cols rows=$rows class=\"largetext_ex\" readonly>".$this->answers[$cnt]."</textarea>";
          if($st_watch && count($this->pt))
            $out .= " (".sprintf("%1.1f pts",$this->pt[$cnt]).")";
          elseif($correct and !$st_watch)
            $out .= "<input type=\"checkbox\" name=\"complete_cor[$cnt]\" value=1 class=\"checkbox\"><input type=\"hidden\" name=\"points[$cnt]\" value=".$this->riddles[$cnt]['points'].">".$perbox->prepare();
        }
        else
          $out .= "<textarea name=\"answers[".($cnt+$this->input_offset)."]\" cols=$cols rows=$rows wrap=\"off\" class=\"largetext_ex\">".($show_an?$this->answers[$cnt]:$r)."</textarea>";
        break;
      case 'select':
        $choices = split('/',$r);
        for($i=0;$i<count($choices);$i++)
          if(ereg('^\\((.*)\\)*$',$choices[$i],$reg))
            $cor_choice = "(<span class=\"correctanswer\">$reg[1]</span>)";

        if($show_an)
        {
          if(ereg('^\\(.*\\)*$',$choices[$this->answers[$cnt]]))
            $cor = 1;

          if($this->correct[$cnt] or ($correct and $cor))
            $corrClass = ' class="correctanswer"';
          elseif($correct or $st_watch)
            $corrClass = ' class="wronganswer"';
          else
            $corrClass = ' class="submitted"';
          $out .= "<strong><span $corrClass>".str_replace(')','',str_replace('(','',$choices[$this->answers[$cnt]]))."</span></strong>";
          if($st_watch and count($this->correct))
          {
            $out .= " (".($cor?$this->riddles[$cnt]['points']:"0")." pts) ".($cor?'':" $cor_choice");
          }
          elseif($correct)
          {
            $out .= "<input type=\"checkbox\" name=\"complete_cor[$cnt]\" value=1 class=\"checkbox\"".($cor?" checked":'').($st_watch?" disabled":'').">"."<input type=\"hidden\" name=\"points[$cnt]\" value=".($cor?$this->riddles[$cnt]['points']:0).">".($cor?'':" $cor_choice");
          }
        }
        else
        {
          $sbox = new SELECTBOX;
          $sbox->set_multiple(0);
          $sbox->set_name("answers[".($cnt+$this->input_offset)."]");
          $sbox->set_style('selectbox_ex');
          $size = $mods?$mods:1;
          $sbox->set_size($mods);
          while(list($k,$v) = each($choices))
            $sbox->add_item(str_replace(')','',str_replace('(','',$v)),$k);
          $out .= $sbox->prepare();
        }
        break;
      case 'checkbox':
      case '#':
        list($pntch,$pntnch) = explode('/',$this->riddles[$cnt]['points']);
        $out .= "<input type=\"checkbox\" name=\"answers[".($cnt+$this->input_offset)."]\" value=1 class=\"checkbox\"".(($show_an and $this->answers[$cnt])?' checked':'').($show_an?' disabled':'').">";
        if(($show_an and $correct))
        {
          if($this->answers[$cnt])
            $out .= "(<input type=\"hidden\" name=\"complete_cor[$cnt]\" value=1><input type=\"hidden\" name=\"points[$cnt]\" value=".(is_numeric($pntch)?$pntch:0).">".(is_numeric($pntch)?$pntch:0)." pts)";
          else
            $out .= "(<input type=\"hidden\" name=\"complete_cor[$cnt]\" value=0><input type=\"hidden\" name=\"points[$cnt]\" value=".(is_numeric($pntnch)?$pntnch:0).">".(is_numeric($pntnch)?$pntnch:0)." pts)";
        }
        break;
      case 'radio':
      case '*':
        if(!$this->radios[$mods])
          $this->radios[$mods]=0;
        $out .= "<input type=\"radio\" name=\"radios[".($mods+$this->radios_offset)."]\" value=".($this->radios[$mods]).(($show_an and $this->radios[$mods]==$this->radios_ans[$mods])?' checked':'').($show_an?' disabled':'')." class=\"radio\">";
        if($correct)
        {
          if($this->radios[$mods]==$this->radios_ans[$mods])
            $out .= "(".$this->riddles[$cnt]['points']."pts)<input type=\"hidden\" name=\"points[$cnt]\" value=".$this->riddles[$cnt]['points'].">";
        }
        $this->radios[$mods]++;
        break;
    }
    
    return $out;
  }

  function prepare_test($show_an=0,$correct=0,$student=0,$as_id=0,$st_watch=0)
  {
    if($this->ready != 1)
      $this->get_data(); 

    if($correct and !is_array($this->answers) and is_numeric($student))
      $this->read_answers($student,$as_id);

    $out = "<center><h3>".$this->parameters['ex_task']."</h3><br></center>\n";
    $this->explode_contents();
    $c = 0;
    while(list($k,$v) = each($this->contents))
    {
      if($c%2 == 0)
        $out .= $v;//str_replace("\n","<br>",$v);
      else
        $out .= $this->expand_tag($v,floor($c/2),$show_an,$correct,$st_watch);

      $c++;
    }

    $this->radios_offset+=count($this->radios);
    return $out;
  }
}

class EX_QANDA extends EXERCISE
{
  var $questions,$sample_answers;
  var $items_cnt;

  function EX_QANDA($ob)
  {
    $this->ex_id = $ob->ex_id;
    $this->parameters = $ob->parameters;
    @$res = mysql_query("select count(*) as cnt from ex_qanda where qa_ex_id=$this->ex_id");
    if(@mysql_num_rows($res) == 1)
    {
      @$row = mysql_fetch_array($res);
      $this->items_cnt = $row['cnt'];
    }
  }

  function get_data()
  {
    parent::get_data();
    @$res = mysql_query("select qa_question,qa_answer from ex_qanda where qa_ex_id=$this->ex_id order by qa_id");
    if(@mysql_num_rows($res) != $this->items_cnt)
    {
      $this->ready = -1;
      exit;
    }
    while(@$row = mysql_fetch_array($res))
    {
      $this->questions[] = $row['qa_question'];
      $this->sample_answers[]   = $row['qa_answer']; 
    }
    $this->ready = 1;
  }

  function prepare_test($show_an=0,$correct=0,$student=0,$as_id=0,$st_watch=0)
  {
    global $indicators;
    if($this->ready != 1)
      $this->get_data();
      
    if(($correct or $st_watch) and !is_array($this->answers))
      $this->read_answers($student,$as_id);

    $table = new TABLE;
    $table->set_parameters(array('cellspacing'=>2));
    $table->add_row(array(array('attr'=>'colspan=3 align="center"','cont'=>"<h3>".$this->parameters['ex_task']."</h3><br>\n")));

    $cor_cnt = 0;
    for($i = 0; $i < $this->items_cnt; $i++)
    {
      if($correct and $this->sample_answers[$i] and $this->answers[$i]==$this->sample_answers[$i])
      {
        $cor = 1;
        $cor_cnt++;
      }
      elseif($st_watch and $this->correct[$i])
      {
        $cor = 1;
        $cor_cnt++;
      }
      else
        $cor = 0;

      if($correct and !$st_watch)
        $last = "<input type=\"checkbox\" name=\"qanda_cor[$i]\" value=1 class=\"checkbox\"".($cor?' checked':'').">";
      elseif($st_watch)
        $last = $cor?$indicators['correct']:$indicators['incorrect'];
      $table->add_row(array($this->questions[$i],$show_an?("<strong>".$this->answers[$i]."<strong>"):"<input type=\"text\" name=\"answers[".($i+$this->input_offset)."]\" maxlength=160 size=80 class=\"boxedfield_ex\" value=\"".$this->answers[$i]."\">",/*$correct?"<input type=\"checkbox\" name=\"qanda_cor[$i]\" value=1 class=\"checkbox\"".($cor?' checked':'').">":''*/$last));
    }
    if($st_watch)
      $table->add_row(array(array('attr'=>'colspan=3 align=center','cont'=>"<strong>Points: $cor_cnt of $i</strong>, ratio: ".sprintf("%3.0f%%",$cor_cnt/$i*100))));

    return $table->prepare();
  }
}

class EX_MATCH extends EXERCISE
{
  var $firsts,$seconds;
  var $items_cnt;

  function EX_MATCH($ob)
  {
    $this->ex_id = $ob->ex_id;
    $this->parameters = $ob->parameters;
    @$res = mysql_query("select count(*) as cnt from ex_match where ma_ex_id=$this->ex_id");
    if(@mysql_num_rows($res) == 1)
    {
      @$row = mysql_fetch_array($res);
      $this->items_cnt = $row['cnt'];
    }
  }

  function get_data()
  {
    parent::get_data();
    @$res = mysql_query("select ma_first,ma_second from ex_match where ma_ex_id=$this->ex_id order by ma_id");
    if(@mysql_num_rows($res) != $this->items_cnt)
    {
      $this->ready = -1;
      exit;
    }
    while(@$row = mysql_fetch_array($res))
    {
      $this->firsts[]  = $row['ma_first'];
      $this->seconds[] = $row['ma_second'];
    }
    $this->ready = 1;
  }

  function prepare_test($show_ans=0,$correct=0,$student=0,$as_id=0,$st_watch=0)
  {
    global $indicators;
    if($this->ready!=1)
      $this->get_data();

    if($correct and !is_array($this->answers))
      $this->read_answers($student,$as_id);

    if(!$show_ans)
    {
      $sbox = new SELECTBOX;
      $sbox->set_size(1);
      $sbox->set_multiple(0);
      #$sbox->set_name('answers[]');
      #srand ((double)microtime()*1000000);
      #shuffle ($this->seconds);
      asort($this->seconds);

      $c = 0;
      while(list($k,$v) = each($this->seconds))
      {
        $sbox->add_item($v,$k);
        $position[$k] = $c;
        $c++;
      }
    }

    $table = new TABLE;
    $table->set_parameters(array('cellspacing'=>2));
    if($show_ans)
    {
      if($correct and is_numeric($student))
        $h = $this->parameters['ex_name'].": Answers of $student";
      else
        $h = "Your answers";
    }
    else
      $h = $this->parameters['ex_task'];
    $table->add_row(array(array('attr'=>'colspan=3 align="center"','cont'=>"<h3>$h</h3><br>\n")));
    $cnt = 0;
    $cor_cnt = 0;
    while(list($k,$v) = each($this->firsts))
    {
      $sbox->set_name('answers['.($cnt+$this->input_offset).']');
      $cor = '';
      if($correct or $st_watch)
      {
        if($this->correct[$k] or ($this->answers[$k] == $cnt))
        {
          $cor = $indicators['correct'];
          $cor_cnt++;
        }
        else
          $cor = $indicators['incorrect'];
      }
      if(!$show_ans)
      {
        $sbox->reset_selected();
        $sbox->set_selected(array($position[$this->answers[$cnt]]));
      }
      $table->add_row(array($v,$show_ans?("<strong>".$this->seconds[$this->answers[$k]]."</strong>"):$sbox->prepare(),$cor));
      $cnt++;
    }
    if($correct or $st_watch)
      $table->add_row(array(array('attr'=>'colspan=3 align="center"','cont'=>"<br><input type=\"hidden\" name=\"cor_cnt\" value=$cor_cnt><strong>Points: $cor_cnt</strong>")));

    return $table->prepare();
  }
}

class EX_CHOOSE extends EXERCISE
{
  var $questions,$choices,$correct_choice;
  var $q_cnt,$ch_cnt;

  function EX_CHOOSE($ob)
  {
    $this->ex_id = $ob->ex_id;
    $this->parameters = $ob->parameters;
    $this->options = $ob->options;
    @$res = mysql_query("select count(*) as cnt from ex_choose where ch_ex_id=$this->ex_id");
    if(@mysql_num_rows($res) == 1)
    {
      @$row = mysql_fetch_array($res);
      $this->q_cnt = $row['cnt'];
    }
  }

  function get_data()
  {
    parent::get_data();
    @$res = mysql_query("select ch_question,ch_choices,ch_correct from ex_choose where ch_ex_id=$this->ex_id order by ch_id");
    if(@mysql_num_rows($res) != $this->q_cnt)
    {
      $this->ready = -1;
      exit;
    }
    $cnt = 0;
    while(@$row = mysql_fetch_array($res))
    {
      $this->questions[] = $row['ch_question'];
      $this->correct_choice[] = $row['ch_correct'];
      if(!$cnt or !$this->options['same_opt'])
        $this->choices[] = explode("\n",$row['ch_choices']);
      $cnt++;
    }
    $this->ch_cnt = count($this->choices[0])-1;
    $this->ready = 1;
  }

  function prepare_test($show_an=0,$correct=0,$student=0,$as_id=0,$st_watch=0)
  {
    global $indicators;
    if($this->ready != 1)
      $this->get_data();

    if($correct and !is_array($this->answers))
      $this->read_answers($student,$as_id);

    $table = new TABLE;
    $table->set_parameters(array('cellspacing'=>2,'width'=>'100%'));
    $table->add_row(array(array('attr'=>'align="center" colspan='.(count($this->choices[0])+$correct),'cont'=>"<h3>".$this->parameters['ex_task']."</h3><br>\n")));

    $cor_cnt=0;
    for($i = 0; $i < count($this->questions); $i++)
    {
      $ar = array();
      $ar[] = $this->questions[$i];
      for($j = 0; $j < count($this->choices[$this->options['same_opt']?0:$i])-1; $j++)
      {
        $rad = '';
        $ins = $this->choices[$this->options['same_opt']?0:$i][$j];
        if(!$show_an)
          $out = "<input type=\"radio\" name=\"answers[".($i+$this->input_offset)."]\" value=$j class=\"radio\">".$ins;
        elseif($j == $this->answers[$i])
          $out = "<strong>$ins</strong>";
        else
          $out = $ins;

        #$ar[] = ($show_an?'':"<input type=\"radio\" name=\"answers[$i]\" value=$j class=\"radio\">").$this->choices[$this->options['same_opt']?0:$i][$j];
        $ar[] = $out;
      }
  //    if($correct and $this->answers[$i]==$this->correct_choice[$i])
      if($correct or $st_watch)
      {
        if($this->correct[$i] or $this->answers[$i]==$this->correct_choice[$i])
        {
          $cor_cnt++;
          $ar[] = $indicators['correct'];
        }
        else
          $ar[] = $indicators['incorrect'];
      }
      $table->add_row($ar);
    }
    if($correct or $st_watch)
      $table->add_row(array(array('attr'=>'align="center" colspan='.(count($this->choices[0])+$correct),'cont'=>"<br><input type=\"hidden\" name=\"cor_cnt\" value=$cor_cnt><strong>Points: $cor_cnt of $i</strong>, ratio:".sprintf("%3.0f%%",$cor_cnt/$i*100))));

    return $table->prepare();
  }
}

class EX_CROSSWORD extends EXERCISE
{
  var $hints,$letters,$solution,$translation;
  var $m,$n;

  function parse_story()
  {
    parent::get_data();
    $tok = strtok($this->ex_story,"\n");
    list($this->m,$this->n) = explode('*',$tok);
    $this->solution = strtok("\n");
    $this->translation = strtok("\n");
    $c = 0;
    while($tok = strtok("\n"))
    {
      if($tok)
      {
        list($this->hints[$c],$let) = explode(':',$tok);
        for($i = 0; $i < strlen($let); $i++)
          $this->letters[$c][$i] = $let[$i];

        $c++;
      }
    }
    $this->ready=1;
  }

  function EX_CROSSWORD($ob)
  {
    $this->ex_id = $ob->ex_id;
    $this->parameters = $ob->parameters;
    $this->parse_story();
  }

  function prepare_test($show_an=0,$correct=0,$student=0,$as_id=0,$st_watch=0)
  {
    if($this->ready!=1)
      $this->parse_story();

    if($correct and !is_array($this->answers))
      $this->read_answers($student,$as_id);

    $table = new TABLE;
    $table->set_parameters(array('cellspacing'=>2));
    $table->add_row(array(array('attr'=>'align="center"','cont'=>"<h3>".$this->parameters['ex_task']."</h3><br>\n")));
    $ctable = new BOXEDTABLE;
    $ctable->set_parameters(array('border'=>0,'cellpadding'=>2,'style'=>'subtable'));
    $cor_cnt = $c = 0;
    for($i = 0; $i < $this->m; $i++)
    {
      $ar = array("<strong>".$this->hints[$i]."</strong>");
      for($j = 0; $j < $this->n; $j++)
      {
        if($this->letters[$i][$j]=='-')
          $inp = '&nbsp;';
        elseif($show_an)
        {
          if(($correct or $st_watch) and $this->answers[$c] == $this->letters[$i][$j])
          {
            $cor_cnt++;
            $color="#0000FF";
          }
          else
            $color="#FF0000";
          if($this->answers[$c])
            $inp = "<font color=\"$color\"><strong>".$this->answers[$c]."</strong></font>";
    //      else
     //      $inp = "<font color=\"#ffffff\">".$this->letters[$i][$j]."</font>";
          $c++;
        }
        else
        {
          $inp = "<input type=\"text\" name=\"answers[".($c+$this->input_offset)."]\" size=1 maxlength=1 class=\"boxedfield_ex\">";
          $c++;
        }

        if($j == $this->solution)
          $ar[] = array('attr'=>'bgcolor="#505050"','cont'=>"$inp");
        else
          $ar[] = $inp;
      }

      $ctable->add_row($ar);
    }
    $table->add_row(array(array('attr'=>'align="center"','cont'=>$ctable->prepare())));
    if($correct or $st_watch)
      $table->add_row(array(array('attr'=>'align="center"','cont'=>'<br><strong>Solution <small>(translated)</small></strong>: '.$this->answers['solution'].(($this->translation==$this->answers['solution'])?" - <font color=\"#0000FF\">Correct</font>":" - <font color=\"#FF0000\">Incorrect</font>, should be <strong>".$this->translation."</strong>.".($st_watch?'':"<br>Mark as correct <input type=\"checkbox\" name=\"mark_trans_cor\" value=1 class=\"checkbox\"><input type=\"hidden\" name=\"cor_cnt\" value=$cor_cnt>")))));
    else
      $table->add_row(array(array('attr'=>'align="center"','cont'=>"<br><strong>Solution <small>(translated)</small></strong>: <input type=\"text\" size=40 maxlength=160 name=\"translation\" class=\"boxedfield_ex\" value=\"".($show_an?($this->answers['solution']."\" readonly"):'"').">")));
    return $table->prepare();
  }
}

function ex_inst($id)
{
  $ex = new EXERCISE($id);
  switch($ex->parameters['ex_type'])
  {
    case COMPLETE:
      $exx = new EX_COMPLETE($ex);
      break;
    case CHOOSE:
      $exx = new EX_CHOOSE($ex);
      break;
    case CROSSWORD:
      $exx = new EX_CROSSWORD($ex);
      break;
    case MATCH:
      $exx = new EX_MATCH($ex);
      break;
    case QANDA:
      $exx = new EX_QANDA($ex);
      break;
    case EXPLANATION:
      $exx = new EX_COMPLETE($ex);
      break;
  }

  unset($ex);
  return $exx;
}

?>
