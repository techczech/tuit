<?

define('EXERCISES',1);
define('UNITS',2);
$task_str = array(EXERCISES=>'EXERCISES',UNITS=>'UNITS');

define('LOGIN',1);
define('COMPLETING',2);
$on_str = array(LOGIN=>'LOGIN',COMPLETING=>'COMPLETING');

define('SELECT',1);
define('SEQUENTIAL',2);
define('INTERVALS',3);
$mod_str = array(SELECT=>'SELECT',SEQUENTIAL=>'SEQUENTIAL',INTERVALS=>'INTERVALS');

define('STUDENTS',1);
define('CLASSES',2);
$who_str = array(STUDENTS=>'STUDENTS',CLASSES=>'CLASSES');

class ASSIGNMENT
{
  var $id;
  var $description;
  var $task_type,$task_list;
  var $on,$on_after;
  var $modifier,$intervals;
  var $who_type,$who_list;
  var $by,$when;
  var $limit;
  var $complete;
  var $active;
  var $is_in_db;
  var $comment;
  var $complete;
  var $corrected;

  function ASSIGNMENT($id = 0)
  {
    $this->id     = $id;
    $this->who_list  = array();
    $this->task_list = array();
    $this->on = LOGIN;
  }

  function set_description($ds = '')
  {
    $this->description=$ds;
  }

  function set_comment($co = '')
  {
    $this->comment=$co;
  }

  function set_tasks($type,$list)
  {
    if(is_numeric($type) and is_array($list))
    {
      $this->task_type = $type;
      $this->task_list = $list;
    }
    else
      return -1;
  }

  function set_action($o,$oa='')
  {
    $this->on = $o;
    $this->on_after = $oa;
  }

  function is_complete($t,$us)
  {
    if(!is_array($this->complete))
      $this->read_complete_list($us);

    if(in_array($t,$this->complete))
      return 1;
    else
      return 0;
  }

  function is_corrected($t,$us)
  {
    if(!is_array($this->corrected))
      $this->read_complete_list($us);

    if(in_array($t,$this->corrected))
      return 1;
    else
      return 0;
  }

  function read_complete_list($us=0)
  {
    if(!$us)
      return -1;
    $co = array();
    @$res = mysql_query("select an_ex_id,an_corr_by from answers where an_who_id=$us and an_as_id=$this->id order by an_id");
    while(@$row = mysql_fetch_array($res))
    {
      $co[] = $row['an_ex_id'];
      if($row['an_corr_by'])
        $cb[] = $row['an_ex_id'];
    }

    $this->complete=$co;
    $this->corrected=$cb;
  }

  function read_db_record($id='')
  {
    if(is_numeric($id) and ($id != $this->id))
      $this->id = $id;

    @$res = mysql_query("select as_description,as_task_type,as_on,as_on_after,as_modifier,as_limit,as_who_type,as_by,date_format(as_when,'%e. %c. %Y') as as_when,as_complete,as_active,as_comment,name from assignments,users where as_by=id and as_id=$this->id");
    if(@mysql_num_rows($res) == 1)
    {
      @$row = mysql_fetch_array($res);

      $this->description = $row['as_description'];
      $this->task_type   = $row['as_task_type'];
      $this->who_type    = $row['as_who_type'];
      $this->on          = $row['as_on'];
      $this->on_after    = $row['as_on_after'];
      $this->modifier    = $row['as_modifier'];
      $this->limit       = $row['as_limit'];
      $this->complete    = $row['as_complete'];
      $this->when        = $row['as_when'];
      $this->by['id']    = $row['as_by'];
      $this->by['name']  = $row['name'];
      $this->active      = $row['as_active'];
      $this->comment     = $row['as_comment'];
    }
    else
      return -1;

    @$res = mysql_query("select aw_who_id,aw_complete from as_who where aw_as_id=$this->id");
    $cnt = 0;
    while(@$row = mysql_fetch_array($res))
    {
      $this->who_list[$cnt]['id'] = $row['aw_who_id'];
      $this->who_list[$cnt]['complete'] = $row['aw_complete'];
      $cnt++;
    }

    @$res = mysql_query("select at_task_id from as_tasks where at_as_id=$this->id order by at_id");
    while(@$row = mysql_fetch_array($res))
      $this->task_list[] = $row['at_task_id'];

    if($this->task_type==UNITS)
    {
      $ls = '';
      $cnt = 0;
      reset($this->task_list);
      while(list($k,$v) = each($this->task_list))
        $ls .= ($cnt++?',':'').$v;

      $this->task_list = array();
      @$res = mysql_query("select se_ex_id from exercises_sets where se_set_id in ($ls) order by se_set_id,se_seq");
      while(@$row = mysql_fetch_array($res))
      {
        if(in_array($row['se_ex_id'],$this->task_list))
          continue;
        $this->task_list[] = $row['se_ex_id'];
      }
        
    }
    $this->is_in_db = 1;
  }

  function read_task_types()
  {
    reset($this->task_list);
    $c=0;
    $ls='';
    while(list($k,$v) = each($this->task_list))
      $ls .= ($c++?',':'').$v;

    #if($this->task_type==EXERCISES)
      $sql = "select ex_id as id,ex_type as type from exercises where ex_id in($ls)";

    @$res = mysql_query($sql);
    if(!mysql_num_rows($res))
      return -1;
    $tt = array();
    while(@$row = mysql_fetch_array($res))
      $tt[$row['id']] = $row['type'];

    $this->task_types=$tt;
  }

  function read_task_records($t=0)
  {
    if($t)
      $ls = $t;
    else
    {
      reset($this->task_list);
      $c = 0;
      $ls = '';
      while(list($k,$v) = each($this->task_list))
        $ls .= ($c++?',':'').$v;
    }

    #if($this->task_type==EXERCISES)
      $sql = "select ex_id as id, ex_type as type, ex_name as name, ex_task as task, ex_story as story, ex_difficulty as difficulty, ex_options as options from exercises where ex_id in($ls)";
    #elseif($this->task_type==UNITS)
    #  $sql = "select set_id as id, set_type as type, set_name as name, set_difficulty as difficulty from sets where set_id in($ls)";
    #else
    #  return -1;

    @$res = mysql_query($sql);
    if(!@mysql_num_rows($res) and count($this->task_list))
      return -1;

    $tp = array();
    while(@$row = mysql_fetch_array($res))
    {
      $tp[$row['id']]['name']       = $row['name'];
      $tp[$row['id']]['type']       = $row['type'];
      $tp[$row['id']]['task']       = $row['task'];
      $tp[$row['id']]['story']      = $row['story'];
      $tp[$row['id']]['difficulty'] = $row['difficulty'];
      $tp[$row['id']]['options']    = $row['options'];
    }
    return $tp;
  }

  function write_db_record()
  {
    global $udata;
    if(! (is_array($this->task_list) and is_numeric($this->task_type) and is_numeric($this->on) and is_numeric($this->who_type) ) )
      return -1;

    if($this->is_in_db)
      $com = 'replace';
    else
      $com = 'insert';

    $query = "$com into assignments set as_description='".addslashes($this->description)."',as_task_type=".$this->task_type.",as_on=".$this->on;
    if($this->on == COMPLETING)
    {
      if(!is_numeric($this->on_after))
        return -1;
      else
        $query .= ",as_on_after=".$this->on_after;
    }
    elseif(isset($this->on_after))
      $query .= ",as_on_after=to_days('".$this->on_after."')";

    if(is_numeric($this->modifier))
      $query .= ",as_modifier=".$this->modifier;

    if(isset($this->limit))
      $query .= ",as_limit='".addslashes($this->limit)."'";

    if($this->active)
      $query .=",as_active=1";

    if($this->comment)
      $query .= ",as_comment='".addslashes($this->comment)."'";

    $query .= ",as_who_type=".$this->who_type.",as_by=".$udata['id'];

    @$res = mysql_query($query);

    if(!$this->id = mysql_insert_id())
      return -1;

    if($this->is_in_db)
    {
      @mysql_query("delete from as_tasks where at_as_id=$this->id");
      @mysql_query("delete from as_who where aw_as_id=$this->id");
    }

    $ls = '';
    $cnt = 0;
    reset($this->task_list);
    while(list($k,$v) = each($this->task_list))
      $ls .= ($cnt++?',':'')."(NULL,$v,$this->id)";

    $query = "insert into as_tasks values $ls";
    @$res = mysql_query($query);

    $ls = '';
    $cnt = 0;
    reset($this->who_list);
    while(list($k,$v) = each($this->who_list))
      $ls .= ($cnt++?',':'')."(NULL,$v[id],$this->id,0,1)";

    $query = "insert into as_who values $ls";
    @$res = mysql_query($query);
  }

  function prepare_query()
  {
    global $task_str,$on_str,$who_str,$mod_str;
    if(! (is_array($this->task_list) and is_numeric($this->task_type) and is_numeric($this->on) ) )
      return -1;

    if($this->on == COMPLETING and !isset($this->on_after))
      return -1;

    $ls = '';
    $cnt = 0;
    reset($this->task_list);
    while(list($k,$v) = each($this->task_list))
      $ls .= ($cnt++?',':'').$v;

    $q = 'ASSIGN '.$task_str[$this->task_type]."($ls) ON ".$on_str[$this->on]." ".($this->on==COMPLETING?$this->on_after:($this->on_after?'AFTER '.$this->on_after:''));

    if($this->modifier)
    {
      $q .= ' '.$mod_str[$this->modifier];
      if($this->modifier == INTERVALS)
      {
        if(is_array($this->intervals))
        {
          $ls = '';
          $cnt = 0;
          reset($this->intervals);
          while(list($k,$v) = each($this->intervals))
            $ls .= ($cnt++?',':'').$v;

          $q .= "($ls)";
        }
        elseif(is_numeric($this->intervals))
          $q .= ' '.$this->intervals;
        else
          return -1;
      }
    }

    if($this->limit)
      $q .= ' LIMIT '.$this->limit;

    if(isset($this->who_type) and is_array($this->who_list))
    {
      $ls = '';
      $cnt = 0;
      reset($this->who_list);
      while(list($k,$v) = each($this->who_list))
        $ls .= ($cnt++?',':'').$v['id'];

      $q .= ' TO '.$who_str[$this->who_type]."($ls)";
    }

    if($this->active)
      $q .= ' ACTIVE';

    return $q;
  }

  function parse_query($q)
  {
    if(strncasecmp($q,'ASSIGN ',7))
      return $q;

    $q = ltrim(substr($q,7));

    if(!strncasecmp($q,'EXERCISES',9))
    {
      $this->task_type=EXERCISES;
      $q = substr($q,9);
    }
    elseif(!strncasecmp($q,'UNITS',5))
    {
      $this->task_type=UNITS;
      $q = substr($q,5);
    }
    else
      return $q;

    $q = ltrim($q);
    if(eregi('^\\(([^)]+)\\)(.*)$',$q,$regs))
    {
      $this->task_list = split('[[:space:]]*,[[:space:]]*',trim($regs[1]));
      $q = ltrim($regs[2]);
    }
    else
      return $q;

    if(strncasecmp($q,'ON ',3))
      return $q;

    $q = ltrim(substr($q,3));

    if(!strncasecmp($q,'LOGIN',5))
    {
      $this->on = LOGIN;
      $q = ltrim(substr($q,5));
      if(eregi('^AFTER[[:space:]]+([[:digit:]]{4}-[[:digit:]]{1,2}-[[:digit:]]{1,2})(.*)$',$q,$regs))
      {
        $this->on_after = $regs[1];
        $q = $regs[2];
      }
    }
    elseif(eregi('^COMPLETING[[:space:]]+([[:digit:]]+)(.*)$',$q,$regs))
    {
      $this->on = COMPLETING;
      $this->on_after = $regs[1];
      $q = $regs[2];
    }
    else
      return $q;

    $q = ltrim($q);

    while($q)
    {
      if(!strncasecmp('SELECT',$q,6))
      {
        $this->modifier = SELECT;
        $q = substr($q,6);
      }
      elseif(!strncasecmp('SEQUENTIAL',$q,10))
      {
        $this->modifier = SEQUENTIAL;
        $q = substr($q,10);
      }
      elseif(!strncasecmp('INTERVALS',$q,9))
      {
        $this->modifier = INTERVALS;
        $q = ltrim(substr($q,9));
        if(eregi('^\\(([^)]+)\\)(.*)$',$q,$regs))
        {
          $this->intervals = split('[[:space:]]*,[[:space:]]*',trim($regs[1]));
          $q = $regs[2];
        }
        elseif(eregi('^([[:digit:]]+)(.*)$',$q,$regs))
        {
          $this->intervals = $regs[1];
          $q = $regs[2];
        }
        else
          return $q;
      }
      elseif(!strncasecmp('LIMIT',$q,5))
      {
        $q = ltrim(substr($q,5));
        if(ereg('^([-[:digit:]]+)(.*)$',$q,$regs))
        {
          $this->limit = $regs[1];
          $q = ltrim($regs[2]);
        }
        else
          return $q;
      }
      elseif(!strncasecmp('TO',$q,2))
      {
        $q = ltrim(substr($q,2));
        if(!strncasecmp('STUDENTS',$q,8))
        {
          $this->who_type = STUDENTS;
          $q = substr($q,8);
        }
        elseif(!strncasecmp('CLASSES',$q,7))
        {
          $this->who_type = CLASSES;
          $q = substr($q,7);
        }
        else
          return $q;

        $q = ltrim($q);
        if(eregi('^\\(([^)]+)\\)(.*)$',$q,$regs))
        {
          $who_list = split('[[:space:]]*,[[:space:]]*',trim($regs[1]));
          $cnt = 0;
          while(list($k,$v) = each($who_list))
          {
            $this->who_list[$cnt]['id'] = $v;
            $cnt++;
          }
          $q = $regs[2];
        }
        else
          return $q;
      }
      elseif(!strncasecmp('ACTIVE',$q,6))
      {
        $this->active = 1;
        $q = ltrim(substr($q,6));
      }
      elseif(ereg('^[[:space:]]*$',$q))
        $q = ltrim($q);
      else
        return $q;

      $q = ltrim($q);
    }

    return 0;
  }
}

?>
