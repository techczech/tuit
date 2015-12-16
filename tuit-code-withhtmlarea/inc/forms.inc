<?

/*
   function for forms that are included more than once.. most of them..
   (at the moment this only applies to login_form..)
*/

function change_udata_form()
{
  global $udata;

  if(is_acct_admin($udata['admin']))
  {
    @$res = mysql_query('select id,login,name,default_teacher from users where admin=0 or admin is null');
    if((@$cnt = mysql_num_rows($res)) > 0)
    {
      $sbox = new SELECTBOX;
      $sbox->set_style('selectbox');
      $sbox->set_multiple(1);
      $sbox->set_size($cnt>5?'5':$cnt);
      $sbox->set_name('default_teacher[]');

      while(@$row = mysql_fetch_array($res))
      {
        $sbox->add_item("$row[name] ($row[login])",$row['id']);
        if($row['default_teacher']==$udata['id'])
          $sbox->set_selected(array($sbox->how_many()-1));
      }
    }
  }

  $btable = new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable'));
  $btable->add_row(array('Current password','<input type="password" name="curr_passwd" size=12 maxlength=12 class="boxedfield">'));
  $btable->add_row(array('New password','<input type="password" name="new_passwd" size=12 maxlength=12 class="boxedfield"> <small>Verification</small>&nbsp;<input type="password" name="new_passwd2" size=12 maxlength=12 class="boxedfield">'));
  $btable->add_row(array('Name',"<input type=\"text\" name=\"change_name\" value=\"$udata[name]\" size=30 maxlength=30 class=\"boxedfield\">"));
  $btable->add_row(array('Email',"<input type=\"text\" name=\"change_email\" value=\"$udata[email]\" size=30 maxlength=30 class=\"boxedfield\">"));
  $btable->add_row(array('Send note?',"<input type=\"checkbox\" name=\"send_note\" value=1".($udata['send_note']?' checked':'')." class=\"checkbox\">"));
  if($sbox)
    $btable->add_row(array('Set yourself as default teacher for',$sbox->prepare()));

  $btable->add_row(array('&nbsp','<input type="submit" value="   Save changes   " name="change_udata" class="buttoninbox">'));
  return "<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"change_udata\">
  <center>
  <span class=\"middle\">
  You can change various settings on this page. If you want to change your
  password, you have to enter your current password too.
  </span>
  <br><br><br>\n".
  $btable->prepare()."
</center>
</form>\n";
}

function add_user_category_form($vals)
{
  $btable = new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable'));

  $btable->add_row(array('Category name',"<input type=\"text\" name=\"add_name\" size=30 maxlength=255 value=\"$vals[add_name]\" class=\"boxedfield\">"));
  $btable->add_row(array('Description',"<input type=\"text\" name=\"add_description\" size=30 value=\"$vals[add_description]\" class=\"boxedfield\">"));
  $btable->add_row(array('&nbsp;','<input type="submit" value="Add user category" name="add_category" class="buttoninbox">'));

  return "<center>
  <h3>Add new user category</h3><form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"user_categories\">".
  $btable->prepare().
  "</form></center>\n";
}

function edit_user_category_form($vals)
{
  $btable = new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable'));

  $btable->add_row(array('Category name',"<input type=\"text\" name=\"name\" size=30 maxlength=255 value=\"$vals[cou_name]\" class=\"boxedfield\">"));
  $btable->add_row(array('Description',"<input type=\"text\" name=\"description\" size=30 value=\"$vals[cou_description]\" class=\"boxedfield\">"));
  $btable->add_row(array('&nbsp;','<input type="submit" value="Save changes to user category" name="change_category" class="buttoninbox">'));

  return "<center>
  <h3>Edit user category</h3><form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"user_categories\">
  <input type=\"hidden\" name=\"id\" value=\"$vals[cou_id]\">
  ".
  $btable->prepare().
  "</form></center>\n";
}

function add_user_form($vals)
{
  $btable = new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable'));

  $btable->add_row(array('Userid',"<input type=\"text\" name=\"add_login\" size=12 maxlength=12 value=\"$vals[add_login]\" class=\"boxedfield\">"));
  $btable->add_row(array('Password','<input type="password" name="add_passwd" size=12 maxlength=12 class="boxedfield">'));
  $btable->add_row(array('Password verification','<input type="password" name="add_passwd2" size=12 maxlength=12 class="boxedfield">'));
  $btable->add_row(array('Real name',"<input type=\"text\" name=\"add_name\" size=30 maxlength=30 value=\"$vals[add_name]\" class=\"boxedfield\">"));
  $btable->add_row(array('Email',"<input type=\"text\" name=\"add_email\" size=30 maxlength=30 value=\"$vals[add_email]\" class=\"boxedfield\">"));
  $btable->add_row(array('Expiry (DD. MM. YYYY)<br><small>(Leave blank if permanent)</small>',"<input type=\"text\" name=\"add_expiry\" size=12 maxlength=12 value=\"$vals[add_expiry]\" class=\"boxedfield\">"));
  $btable->add_row(array('Teacher',"<input type=\"checkbox\" name=\"add_teacher\" value=".TEACHER.($vals['add_teacher']?' checked':'')." class=\"checkbox\">"));
  $btable->add_row(array('Contents administrator',"<input type=\"checkbox\" name=\"add_cont_admin\" value=".CONT_ADMIN.($vals['add_cont_admin']?' checked':'')." class=\"checkbox\">"));
  $btable->add_row(array('Accounts administrator',"<input type=\"checkbox\" name=\"add_acct_admin\" value=".ACCT_ADMIN.($vals['add_acct_admin']?' checked':'')." class=\"checkbox\">"));
  $btable->add_row(array('Super administrator',"<input type=\"checkbox\" name=\"add_super_admin\" value=".SUPER_ADMIN.($vals['add_super_admin']?' checked':'')." class=\"checkbox\">"));
  $btable->add_row(array('Visitor',"<input type=\"checkbox\" name=\"add_visitor\" value=".VISITOR.($vals['add_visitor']?' checked':'')." class=\"checkbox\">"));
  $btable->add_row(array('&nbsp;','<input type="submit" value="Add account" name="add_account" class="buttoninbox">'));

  return "<center>
  <h3>Add new user account</h3><form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"users\">".
  $btable->prepare().
  "</form></center>\n";
}

function acct_request_form($vals)
{
  $btable = new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable'));
  $btable->add_row(array("<input type=\"text\" name=\"req_login\" size=12 maxlength=12 value=\"$vals[req_login]\" class=\"boxedfield\">",'<strong> Desired userid</strong> <small>(Alphanumerical characters; 1-12 characters long; case insensitive)</small>'));
  $btable->add_row(array('<input type="password" name="req_passwd" size=12 maxlength=12 class="boxedfield">','<strong>Password</strong> <small>(Can be almost anything, except \'," and \\; up to 12 characters; case sensitive)</small>'));
  $btable->add_row(array('<input type="password" name="req_passwd2" size=12 maxlength=12 class="boxedfield">','<strong>Password verification</strong>'));
  $btable->add_row(array("<input type=\"text\" name=\"req_name\" size=30 maxlength=30 value=\"$vals[req_name]\" class=\"boxedfield\">",'<strong>Real name</strong>'));
  $btable->add_row(array("<input type=\"text\" name=\"req_email\" size=30 maxlength=30 value=\"$vals[req_email]\" class=\"boxedfield\">",'<strong>Email</strong>'));
  $btable->add_row(array(array('attr'=>'colspan=2','cont'=>'<input type="submit" value="   Send application   " name="acct_request" class="buttoninbox">')));
  return "<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"acct_request\">
  <center>
  <span class=\"middle\">
  Here you can apply for a trial account that will allow you a temporary
  access to the interactive exercises. An account is necessary for your
  identification, so that the teacher can correct your work and monitor
  your progress. All input boxes must be filled.
  </span>
  <br><br>".($btable->prepare()).
  "</center></form>\n";
}

function login_form()
{
  $btable=new BOXEDTABLE;
  $btable->set_parameters(array('style'=>'subtable','width'=>''));

  $btable->add_row(array('Userid:','<input type="text" size="12" maxlength="12" name="form_login" class="boxedfield">'));
  $btable->add_row(array('Password:','<input type="password" size="12" maxlength="12" name="form_passwd" class="boxedfield">'));
  $btable->add_row(array('&nbsp;','<input type="submit" value="  Submit  " class="buttoninbox">'));

  return "<form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"login\">\n".
    $btable->prepare()."\n</form>\n";
}

?>
