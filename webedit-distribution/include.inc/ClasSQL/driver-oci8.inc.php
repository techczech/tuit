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

    $this->connection = ocilogon($this->DBUser,$this->DBPassword,$this->DBHost);
    return $this->connection;
  }

  function query($q='')
  {
    if(!empty($q))
      $this->lastQuery = $q;

    $this->lastResult = ociparse($this->connection,$this->lastQuery);
    ociexecute($this->lastResult);
    return $this->lastResult;
  }

  function limitedQuery($q,$o=0,$l=20)
  {
    $q = "select * from (select rownum as row_num, a.* from ($q) a) b where row_num between ".($o+1)." and ".($o+$l);
    
    return $this->query($q);
  }

  function fetchArray($r=0,$t=0)
  {
    if(!$r)
      $r = $this->lastResult;

    if(!$t)
      $t = OCI_ASSOC+OCI_RETURN_NULLS+OCI_RETURN_LOBS;

    $res = ocifetchinto($r,$temp,$t);
    if(is_array($temp))
    {
      while(list($k,$v) = each($temp))
      {
        $x = strtolower($k);
        $row[$x] = $v;
      }
    }
    else
      $row = 0;
    return $row;
  }

  function numRows()
  {
    if($tmpdb = ocinlogon($this->DBUser,$this->DBPassword,$this->DBHost))
    {
      $res = ociparse($tmpdb,$this->lastQuery);
      ociexecute($res);
      $cnt = ocifetchstatement($res,$temp);
      ocilogoff($tmpdb);
    }
    else
      $cnt = FALSE;
    return $cnt;
  }

  function affectedRows($r=0)
  {
    if(!$r)
      $r = $this->lastResult;

    return ocirowcount($this->lastResult);
  }

  function commit()
  {
    return ocicommit($this->connection);
  }

  function rollback($sp)
  {
    if($sp)
      return ociexecute(ociparse($this->connection,"ROLLBACK TO SAVEPOINT $sp"));
    else
      return ocirollback($this->connection);
  }

  function setSavePoint($sp)
  {
    ociexecute(ociparse($this->connection,"SAVEPOINT $sp"));
  }

  function close()
  {
    return ocilogoff($this->connection);
  }
}

?>
