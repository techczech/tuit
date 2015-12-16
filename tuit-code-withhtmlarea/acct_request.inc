<?

/*
   account requests..
   finished, no known bugs..
*/

if($acct_request)
{
  if(!$req_login or !$req_passwd or !$req_passwd2 or !$req_name or !$req_email)
    $err = 'You must fill in all fields!';
  elseif($req_passwd != $req_passwd2)
    $err = 'Passwords do not match!';
  elseif(!eregi('^[[:alnum:]_]+$',$req_login) or ereg('[\'"\\]',$req_passwd))
    $err = 'Invalid characters in login or password!';
  elseif(is_user($req_login))
    $err = 'Sorry, but your desired userid is already taken. Please try another one.';
  else
  {
    @$res = mysql_query("insert into acct_requests set req_login='$req_login',req_passwd=password('$req_passwd'),req_name='".addslashes($req_name)."', req_email='".addslashes($req_email)."'");
    if(!$res)
      $err = 'Cannot send the request!';
    else
    {
      $page->add_to_middle("<center>
Your request for an account was sent. When your request is processed,
an email will be sent to your address: $req_email.</center>");
    }
  }
}

if(!$acct_request or $err)
{
  include('inc/forms.inc');
  if($err) error_f("$err<br><br>\n");
  $page->set_top('Account request');
  $page->add_to_middle(acct_request_form(array('req_login'=>$req_login,'req_name'=>$req_name,'req_email'=>$req_email)));
}

return 123;
?>
