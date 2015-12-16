<?

class SQLSpecific extends ClasSQL
{
  function SQLSpecific()
  {
  }

  function connect($h,$u,$p)
  {
    $this->DBHost = $h;
    $this->DBUser = $u;
    $this->DBPassword = $p;

    $this->connection = mysql_connect($this->DBHost,$this->DBUser,$this->DBPassword);
    return $this->connection;
  }

  function selectDB($db)
  {
    $this->DBName = $db;

    return mysql_select_db($this->DBName,$this->connection);
  }

  function query($q='')
  {
    if(!empty($q))
      $this->lastQuery = $q;

    $this->lastResult = mysql_query($this->lastQuery,$this->connection);
    return $this->lastResult;
  }

  function limitedQuery($q,$o=0,$l=20)
  {
    $q = "$q limit $o,$l";
    
    return $this->query($q);
  }

  function fetchArray($r=0,$t=0)
  {
    if(!$r)
      $r = $this->lastResult;

    if(!$t)
      $t = MYSQL_ASSOC;

    $row = mysql_fetch_array($r,$t);
    return $row;
  }

  function numRows($r=0)
  {
    if(!$r)
      $r = $this->lastResult;

    $cnt = mysql_num_rows($r);
    return $cnt;
  }

  function affectedRows()
  {
    $cnt = mysql_affected_rows($this->connection);
    return $cnt;
  }

  function insertId()
  {
    return mysql_insert_id($this->connection);
  }

  function close()
  {
    return mysql_close($this->connection);
  }
}

?>
