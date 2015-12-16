<?

/*
   account requests approving ground
   essentially finished..
*/

# by default show 10 oldest requests

if($desc==1 or $desc=='desc') $desc = 'desc';
else $desc = '';

if(!is_numeric($limit) or $limit<0) $limit = 10;
if(!is_numeric($offset) or $offset<0) $offset = 0;

if(!am_i_acct_admin())
  return 123;

if(isset($submit) and is_array($req) and @$res=mysql_query('select * from acct_requests'))
{
  $btable = new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable'));
  $btable->add_row(array(array('attr'=>'align="center"','cont'=>'&nbsp;<strong> Login <small>(name)</small> </strong>&nbsp;'),array('attr'=>'align="center"','cont'=>'&nbsp;<strong> Result </strong>&nbsp;')));

  while(@$row = mysql_fetch_array($res))
  {
    $result = '';
    if($req[$row['req_id']]=='defer')
      $result = 'Requests defered.';
    elseif($req[$row['req_id']]=='approve')
    {
      $sql = "insert into users set login='$row[req_login]', passwd='$row[req_passwd]', name='".addslashes($row['req_name'])."', email='".addslashes($row['req_email'])."'";
      if(ereg('^([0-9]{1,2}).[[:space:]]?([0-9]{1,2}).[[:space:]]?([0-9]{4})$',$exp[$row['req_id']],$regs))
        $sql .= ", expiry='$regs[3]-$regs[2]-$regs[1]'";

      if(@$res2 = mysql_query($sql))
      {
        if($row['req_email'])
          send_email($row['req_email'],"User account request on $server_name","
Your request for an account '$row[req_login]' on $server_addr
was accepted. The password is as you requested.\n\n".
($exp[$row['req_id']]?"Expiry date: ".$exp[$row['req_id']]."\n":''));

        @$res3 = mysql_query("delete from acct_requests where req_id=$row[req_id]");
        $result = 'Account added.';
      }
      else
        $result = 'Adding failed, perhaps account with the same userid exists.';
    }
    elseif($req[$row['req_id']]=='reject')
    {
      if($row['req_email'])
        send_email($row['req_email'],"User account request on $server_name","
Your request for an account '$row[req_login]' on $server_addr
was rejected. Either you haven't provided sufficient information or there
already exists an account with the same userid that you requested.");

      @$res2 = mysql_query("delete from acct_requests where req_id=$row[req_id]");
      $result = 'Request rejected.';
    }
    if($result)
      $btable->add_row(array(array('attr'=>'align="center"','cont'=>"$row[req_login] <small>($row[req_name])</small>"),array('attr'=>'align="center"','cont'=>$result)));
  }
  $page->add_to_middle('<center><strong>Results</strong><br><br>'.$btable->prepare().'</center><br><br>');
}

$page->set_top('Process account requests');
@$res = mysql_query("select count(*) as c from acct_requests");
@$row = mysql_fetch_array($res);
$num = $row['c'];
@$res = mysql_query("select req_id,req_login,req_name,req_email,date_format(req_time,'%e. %c. %Y') as date from acct_requests order by req_time $desc limit $offset,$limit");
if(!$res or !@mysql_num_rows($res))
  $page->add_to_middle('<center>No new account requests..</center>');
else
{
  $btable = new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable'));
  $btable->add_row(array('&nbsp;<strong> Required userid </strong>&nbsp;','&nbsp;<strong> Name and email </strong>&nbsp;','&nbsp;<strong> Date of application </strong>&nbsp;',array('attr'=>'align="center"','cont'=>'&nbsp;<strong> Action </strong>&nbsp;')));
  $btable->add_row(array(array('attr'=>'colspan=4','cont'=>'<hr width="100%">')));

  while(@$row = mysql_fetch_array($res))
  {
    $btable->add_row(array(array('attr'=>'align="center"','cont'=>"<h3>$row[req_login]</h3>"),
    array('attr'=>'align="center"','cont'=>"$row[req_name]<br>$row[req_email]"),
    array('attr'=>'align="center"','cont'=>"$row[date]"),
    array('attr'=>'align="left"','cont'=>"<input type=\"radio\" name=\"req[$row[req_id]]\" value=\"approve\" class=\"radio\">Approve<br>
    <input type=\"radio\" name=\"req[$row[req_id]]\" value=\"reject\" class=\"radio\">Reject<br>
    <input type=\"radio\" name=\"req[$row[req_id]]\" value=\"defer\" checked class=\"radio\">Defer<br>
    Expiry (DD. MM. YYYY) <input type=\"text\" size=12 maxlength=12 name=\"exp[$row[req_id]]\" class=\"boxedfield\">\n")));
    
    $btable->add_row(array(array('attr'=>'colspan=4','cont'=>'<hr width="100%">')));
  }
  
  $btable->add_row(array(array('attr'=>'colspan=4 align="center"','cont'=>'<input type="submit" value="   Submit   " name="submit" class="buttoninbox">')));

  $page->add_to_middle("<center>
  Here you can approve or reject new account requests for your students.
  You can also enter day of expiry in the required format
  (DD.&nbsp;MM.&nbsp;YYYY) in order to set a temporary account.<br><br>
  <form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"acct_approve\">
  <input type=\"submit\" name=\"show\" value=\" Show \" class=\"buttoninbox\">
  <input type=\"text\" size=5 maxlength=5 name=\"limit\" value=\"$limit\" class=\"boxedfield\">
  <input type=\"radio\" name=\"desc\" value=0".($desc?'':' checked')." class=\"radio\">Oldest/<input type=\"radio\" name=\"desc\" value=1".($desc?' checked':'')." class=\"radio\">Latest
  requests..............."
  .pnav($num,$offset,$limit,"?section=acct_approve&desc=$desc&")."<br><br>\n".
  $btable->prepare()."
</form>\n");
}

return 123;
?>
