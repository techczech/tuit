<?

$meMod = 'configuration';

$MODULES[$meMod]['title'] = 'Configuration';
$MODULES[$meMod]['topright'] = $MODULES[$meMod]['title'];

if(!empty($MODULES['argv']['do_save']) and is_array($MODULES['argv']['configuration']))
{
  reset($CONFIGURATION);
  while(list($k,$v) = each($CONFIGURATION))
    $SQL->query("update ".DB_TABLE_CONFIGURATION." set ".DB_CONFIGURATION_VALUE."='".addslashes($MODULES['argv']['configuration'][$k])."' where ".DB_CONFIGURATION_KEY."='$k'");

  $MODULES[$meMod]['mainright'] .= "Changes saved. <a href=\"?m=configuration\">Reload</a>.";
}
else
{
  $SQL->query("select ".DB_CONFIGURATION_KEY.", ".DB_CONFIGURATION_DESCRIPTION." from ".DB_TABLE_CONFIGURATION);
  while($row = $SQL->fetchArray())
    $confDesc[$row[DB_CONFIGURATION_KEY]] = $row[DB_CONFIGURATION_DESCRIPTION];
  $MODULES[$meMod]['mainright'] .= "
  <form action=\"$_SERVER[PHP_SELF]\" method=\"post\">
  <input type=\"hidden\" name=\"m\" value=\"configuration\">
  <table>
  ";
  reset($CONFIGURATION);
  while(list($k,$v) = each($CONFIGURATION))
  {
    $MODULES[$meMod]['mainright'] .= "
    <tr>
      <td><strong>".$confDesc[$k]."</strong><br>
      <textarea rows=\"2\" cols=\"80\" name=\"configuration[".$k."]\">$v</textarea></td>
    </tr>
    ";
  }

  $MODULES[$meMod]['mainright'] .= "
  </table>
  <input type=\"submit\" name=\"do_save\" value=\"   Save   \"/>
  </form>
  ";
}

$MODULES[$meMod]['mainright'] .= "<br/><br/>
<a href=\"?m=catalog\">Back to web builder</a>";

?>
