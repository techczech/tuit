<?
if(!is_super_admin($udata['admin']))
  return 1;
error_reporting (0);
if (!$db) $db="dict";

$tab = new TABLE;
$tab->set_parameters(array('border'=>1,'cellpadding'=>2));

echo $q;
if ($q)
{
#  mysql_connect ("localhost","jmeno_uzivatele","heslo");
  mysql_select_db ($db);
  $res=mysql_query ($q);
  echo mysql_error();
  if ($res)
  {
    $cnt = 0;
    while ($row=mysql_fetch_assoc($res))
    {
      if(!$cnt)
      {
        $bla = array();
        while(list($k,$v) = each($row))
          $bla[] = "<em>$k</em>";

        $tab->add_row($bla);
      }
      $tab->add_row($row);
      $cnt++;
    }
  }
}
$out="
<form action=$PHP_SELF method=post>

<input type=text name=db value=$db><br>
<input type=hidden name=section value=dbedit>
<textarea rows=10 cols=80 name=q wrap=virtual>$q</textarea><br>
<input type=submit>
</form><br><br>".$tab->prepare();
$page->add_to_middle($out);
return 123;
?>
