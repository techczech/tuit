<?
/*
 *   ClasSQL: public domain lightweight php database abstraction class
 */

if(!defined('LOCAL_INCLUDE_PATH'))
  define('LOCAL_INCLUDE_PATH','ClasSQL');

class ClasSQL
{
  var $DBName,$DBUser,$DBPassword,$DBHost;
  var $connection;
  var $lastQuery,$lastResult;

  /* ``Virtual'' methods: */

  function connect() { }

  function selectDB() { }

  function query() { }

  function limitedQuery() { }

  function fetchArray() { }

  function numRows() { }

  function affectedRows() { }

  function insertId() { }

  function commit() { }

  function rollback() { }

  function setSavePoint() { }

  function close() { }
}

function &newSQL($p)
{
  include_once(LOCAL_INCLUDE_PATH."/driver-$p.inc.php");
  $obj = new SQLSpecific();
  return $obj;
}

?>
