<?
include('inc/pre.inc');
include('inc/classes.inc');
include('inc/mi_functions.inc');

$page = new PAGE('TuiT Demo');

include('inc/lookandfeel.inc');
$page->add_to_head($style_in_head);
$page->set_top('Specialist dictionaries');
#@mysql_select_db('dict');

$dicts = array();
@$res = mysql_query("select * from dicts");
while(@$row = mysql_fetch_array($res))
  $dicts[$row['d_id']]=$row['d_name'];
asort($dicts);

if(is_numeric($dict))
{
  $tab = new TABLE;
  if(!is_numeric($offset) or $offset<0) $offset = 0;
  if(!is_numeric($limit) or $limit<1) $limit = 20;
  @$res = mysql_query("select count(*) as cnt from vocab where v_d_id=$dict");
  @$row = mysql_fetch_array($res);
  $cnt = $row['cnt'];
  $tab->add_row(array(array('attr'=>'colspan=5','cont'=>pnav($cnt,$offset,$limit,"?dict=$dict&")."<br><br>")));
  @$res = mysql_query("select * from vocab where v_d_id=$dict order by v_english limit $offset,$limit");
  while(@$row = mysql_fetch_array($res))
  {
    $tab->add_row(array($row['v_english'],"<i>".$row['v_latin']."</i>",$row['v_czech'],$row['v_slovak'],$row['v_german']));
  }
  $page->add_to_middle($tab->prepare());
}
else
{
  $menu = new VMENU;
  while(list($k,$v) = each($dicts))
  {
    $menu->add_item($v,"?dict=$k");
  }
  $page->add_to_middle($menu->prepare());
}

$page->display();
?>
