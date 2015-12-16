<?

function pnav($num,$offset,$limit,$href='')
{
 return ($offset>0?"<a href=\"${href}offset=0&limit=$limit\">":'').'|&lt;'.($offset>0?'</a>':'').'&nbsp;'.
  ($offset>0?"<a href=\"${href}offset=".($offset-$limit>0?$offset-$limit:'0')."&limit=$limit\">":'').'&lt;&lt;'.($offset>0?'</a>':'').'&nbsp;'.
  "&nbsp;Previous&nbsp;-&nbsp;<strong>Total $num</strong>&nbsp;-&nbsp;Next&nbsp;".
  ($offset+$limit<$num?"<a href=\"${href}offset=".($offset+$limit)."&limit=$limit\">":'').'&gt;&gt;'.($offset+$limit<$num?'</a>':'').'&nbsp;'.
  ($offset+$limit<$num?"<a href=\"${href}offset=".($num-$limit>0?$num-$limit:'0')."&limit=$limit\">":'').'&gt;|'.($offset+$limit<$num?'</a>':'');
}

function readable($s)
{
  echo "<br><pre>";
  print_R($s);
  echo "</pre><br>";
}

function stripCzechChars($s)
{
  return strtr($s,"éìø»ýúùíóá¹ï¾èòÉÌØ«ÝÚÙÍÓÁ©Ï®ÈÒ","eertyuuioasdzcnEERTYUUIOASDZCN");
}

function windows2iso($s)
{
  return strtr($s,"éìøýúùíóášïžèòÉÌØÝÚÙÍÓÁŠÏŽÈÒ","éìø»ýúùíóá¹ï¾èòÉÌØ«ÝÚÙÍÓÁ©Ï®ÈÒ");
}

?>
