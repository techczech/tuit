<?

define('DB_TABLE_CONFIGURATION','configuration');
define('DB_CONFIGURATION_KEY','conf_key');
define('DB_CONFIGURATION_VALUE','conf_value');
define('DB_CONFIGURATION_DESCRIPTION','conf_description');

$SQL->query("select ".DB_CONFIGURATION_KEY.", ".DB_CONFIGURATION_VALUE." from ".DB_TABLE_CONFIGURATION);
while($row = $SQL->fetchArray())
  $CONFIGURATION[$row[DB_CONFIGURATION_KEY]] = $row[DB_CONFIGURATION_VALUE];

?>
