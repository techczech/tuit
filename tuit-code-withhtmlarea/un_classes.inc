<?

require('ex_classes_modified.inc');

class UNIT
{
  var $id,$type,$name,$creator,$difficulty,$numtype,$numcreator;
  var $sections,$sequence;
  var $ready;

  function UNIT($id=0)
  {
    $this->ready = 0;
    if($id)
    {
      $this->id = $id;
      $this->get_parameters();
    }
  }

  function get_parameters()
  {
    if(!$this->id)
      return -1;

    @$res = mysql_query("select set_type,set_name,set_creator,set_difficulty,st_name,name from sets left join set_types on set_type=st_id, users where set_creator=id and set_id=".$this->id);
    if(@mysql_num_rows($res)!=1)
      return -1;
    @$row = mysql_fetch_array($res);
    $this->numtype = $row['set_type'];
    $this->type    = $row['st_name'];
    $this->name    = $row['set_name'];
    $this->creator = $row['name'];
    $this->numcreator = $row['set_creator'];
    $this->difficulty = $row['set_difficulty'];
    $this->ready = 1;
  }

  function get_sections($s = array())
  {
    if(!is_numeric($this->id))
      return -1;

    if(is_numeric($s))
      $s = array($s);

    $ls = '';
    for($i=0;$i<count($s);$i++)
      $ls .= ($i?',':'').$s[$i];

    if(!empty($ls))
      $w = "and ss_id in ($ls)";
    else
      $w = '';

    $sql = "select ss_id, ss_name, ss_seq from set_sections where ss_set_id=".$this->id." $w order by ss_seq";
    @$res = mysql_query($sql);
    if(!$res)
      return -1;

    while(@$row = mysql_fetch_array($res))
    {
      $this->sections[$row['ss_seq']] = array('id'=>$row['ss_id'],'name'=>$row['ss_name']);
      $this->sequence[$row['ss_id']] = $row['ss_seq'];
    }
  }

  function read_data()
  {
    if(!is_numeric($this->id))
      return -1;

    $ls = '';
    $c = 0;
    ksort($this->sections);
    reset($this->sections);
    while(list($k,$v) = each($this->sections))
      $ls .= ($c++?',':'').$v['id'];

    if(!empty($ls))
      $w = "and se_ss_id in ($ls)";
    else
      $w = '';

    $sql = "select se_ex_id,se_ss_id,se_seq from exercises_sets where se_set_id=".$this->id." $w order by se_seq";
    @$res = mysql_query($sql);
    if(!$res)
      return -1;

    $c = 0;
    while(@$row = mysql_fetch_array($res))
    {
      $ex[$c] = ex_inst($row['se_ex_id']);
      $this->sections[$this->sequence[$row['se_ss_id']]]['exercises'][$row['se_seq']] = &$ex[$c];
      $c++;
    }
  }

  function show_section($seq)
  {
    $out = '';
    reset($this->sections[$seq]['exercises']);
    $c = 0;
    $io = 0;
    while(list($k,$v) = each($this->sections[$seq]['exercises']))
    {
      $v->input_offset = $io;
      $out .= ($c++?'<br><br>':'').$v->prepare_test(0);
      $out .= "<input type=\"hidden\" name=\"input_offset[".$v->ex_id."]\" value=\"$io\">";
      $io = $v->input_offset+count($v->riddles);
    }

    return $out;
  }
}

return 123;
?>
