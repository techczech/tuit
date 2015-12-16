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

    $this->connection = mssql_connect($this->DBHost,$this->DBUser,$this->DBPassword);
    return $this->connection;
  }

  function selectDB($db)
  {
    $this->DBName = $db;

    return mssql_select_db($this->DBName,$this->connection);
  }

  function query($q='')
  {
    if(!empty($q))
      $this->lastQuery = $q;

    $this->lastResult = mssql_query($this->lastQuery,$this->connection);
    return $this->lastResult;
  }

  function limitedQuery($q,$o=0,$l=20)
  {
    return $this->query($q);
  }

  function fetchArray($r=0)
  {
    if(!$r)
      $r = $this->lastResult;

    $row = mssql_fetch_assoc($r);
    return $row;
  }

  function numRows($r=0)
  {
    if(!$r)
      $r = $this->lastResult;

    $cnt = mssql_num_rows($r);
    return $cnt;
  }

  function affectedRows()
  {
    $cnt = mssql_rows_affected($this->connection);
    return $cnt;
  }

  function close()
  {
    return mssql_close($this->connection);
  }
}

?>
