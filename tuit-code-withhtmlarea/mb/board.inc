<?

#error_reporting(2047);

$email_from = 'czechonline@bohemica.com';
$admin_email = 'dlukes@bohemica.com';

function thread_contents(&$table,$thread=0,$indent=0,$tlthread=0)
{
  global $messages_list,$board,$action;
  global $posts,$age;

  if($indent and $thread)
    $ind = '&nbsp;&nbsp;&nbsp;&nbsp;';
  else
    $ind = '';

  $subtable = new TABLE;
  if(!$thread)
    krsort($posts[$thread]['following']);

  while(list($k,$v) = each($posts[$thread]['following']))
  {
    if($action=='threads' and !in_array($v,$messages_list) and $thread==$tlthread)
      continue;

    if(!$age or $posts[$v]['age']<$age)
    {
      $text = $posts[$v]['text'];
      //$text = strtr($text,'<>','[]');
      $text = str_replace(array('<','>'),array('&lt;','&gt;'),$text);
      //$text = eregi_replace('\\[(a[[:space:]]+href[^]]+)\\]','<\\1>',$text);
      $text = eregi_replace('&lt;(a[[:space:]]+href[^&]+)&gt;','<\\1>',$text);
      //$text = eregi_replace('\\[(\\/?[bia])\\]','<\\1>',$text);
      $text = eregi_replace('&lt;(\\/?[bia])&gt;','<\\1>',$text);
      //$text = str_replace('[br]','<br>',$text);
      $text = str_replace('&lt;br&gt;','<br>',$text);

      $subtable->add_row(array("$ind",'<strong>'.$posts[$v]['subject'].'</strong> <small>('.($posts[$v]['email']?'<a href="mailto:'.$posts[$v]['email']."\">":'').$posts[$v]['author'].($posts[$v]['email']?'</a>':'').', '.$posts[$v]['posted'].')'));
      $subtable->add_row(array("$ind","&nbsp;"));
      $subtable->add_row(array("$ind",nl2br($text)));
      $subtable->add_row(array("$ind","&nbsp;"));
      $subtable->add_row(array("$ind","<span class=\"modhref\"><a href=\"?section=mb&mb_section=board&board=$board&post=$v&action=send\" class=\"modhref\"><input type=\"button\" value=\"   Post reply   \" class=\"buttoninbox\" onclick=\"location.href='?section=mb&mb_section=board&board=$board&post=$v&action=send'\"></a></span>"));
      $subtable->add_row(array("$ind",'<hr>'));
    }
    if(is_array($posts[$v]['following']))
        thread_contents($subtable,$v,$indent,$tlthread);
  }

  $table->add_row(array("$ind",$subtable->prepare()));
}

function messages_contents()
{
  global $messages_list,$board,$boards_count,$posts,$action,$age;

  $sql = "select id,author,email,subject,following,text,date_format(sent,'%d.%m.%y, %H:%i') as posted, (to_days(now()) - to_days(sent)) as age from messages where board_id=$board";
  
  if(is_array($messages_list) and $action=='selected')
  {
    $sql .= ' and id in (';
    $cnt = 0;
    while(list($k,$v) = each($messages_list))
      $sql .= ($cnt++?',':'').$v;
    $sql .= ')';
  }

  $sql .= ' order by sent';
  @$res = mysql_query($sql);

  if(@$cnt = mysql_num_rows($res))
  {
    $posts = array();
    $m_table = new BOXEDTABLE;
    $m_table->set_parameters(array('style'=>'subtable','cellspacing'=>'5'));
    $threads[] = 0;

    while(@$row = mysql_fetch_array($res))
    {
      $id = $row['id'];
      $posts[$id]['author']  = $row['author'];
      $posts[$id]['email']   = $row['email'];
      $posts[$id]['subject'] = $row['subject'];
      $posts[$id]['posted']  = $row['posted'];
      $posts[$id]['text']    = $row['text'];
      $posts[$id]['after']   = $row['following'];
      $posts[$id]['age']     = $row['age'];
      $posts[$row['following']]['following'][] = $id;
      if(is_array($messages_list) and !in_array($row['following'],$messages_list) and !in_array($row['following'],$threads))
        $threads[] = $row['following'];
      if($action=='threads' and in_array($id,$messages_list))
        $tlthreads[$row['following']]=1;
    }

    if($action=='threads')
    {
      while(list($k,$v) = each($tlthreads))
        //thread_contents($m_table,$posts[$k]['after'],1);
        thread_contents($m_table,$k,1,$k);
    }
    else
    {
      sort($threads);
      while(list($k,$v) = each($threads))
        thread_contents($m_table,$v,is_array($messages_list)?0:1);
    }

    $m_table->add_row(array(array('attr'=>'colspan=2 align="center"','cont'=>"<strong><a href=\"?section=mb&mb_section=board&board=$board&action=send\">Post new topic</a> | <a href=\"?section=mb&mb_section=board&board=$board\">Back to board contents</a></strong>")));
    return $m_table;
  }
  else
    return 0;
}

function thread_list(&$table,$posts,$curr_level=1,$thread=0,$indent='')
{
  global $level,$age,$board;
  if(!$thread)
    krsort($posts[$thread]['following']);
  while(list($k,$v) = each($posts[$thread]['following']))
  {
    if(!$age or $posts[$v]['age']<$age)
      $table->add_row(array("$indent".($thread?'':'<strong>')."<a href=\"?section=mb&mb_section=board&board=$board&show_selected=1&messages_list[]=$v\" style=\"color: #003366;\">".$posts[$v]['subject']."</a>".($thread?'':'</strong>').($thread?'':" <small>&lt;<a href=\"?section=mb&mb_section=board&board=$board&messages_list[]=$v&show_threads=1\" style=\"font-size: 10px; color: #000022\">Show thread</a>&gt;</small>"), '', "<input type=\"checkbox\" name=\"messages_list[]\" value=$v class=\"checkbox\">",$posts[$v]['author'], '', $posts[$v]['posted']));
    if($curr_level<$level or $level=='all')
      thread_list($table,$posts,$curr_level+1,$v,"$indent&nbsp;&nbsp;&nbsp;&nbsp;");
  }
}

function messages_list()
{
  global $admin;
  global $desc,$offset,$limit,$boards_count,$board,$level,$age;

  @$res = mysql_query("select id,author,email,subject,following,date_format(sent,'%d.%m.%y, %H:%i') as posted, (to_days(now()) - to_days(sent)) as age from messages where board_id=$board order by sent $desc");
  if(@$cnt = mysql_num_rows($res))
  {
    $posts = array();
    $m_table = new BOXEDTABLE;
    $m_table->set_parameters(array('style'=>'subtable','cellspacing'=>5));
    $m_table->add_row(array('<strong><em>Subject</em></strong>','&nbsp','','<strong><em>Author</em></strong>','&nbsp;','<strong><em>Posted</em></strong>'));


    while(@$row = mysql_fetch_array($res))
    {
      $id = $row['id'];
      $posts[$id]['author']       = $row['author'];
      $posts[$id]['email']        = $row['email'];
      $posts[$id]['subject']      = $row['subject'];
      $posts[$id]['posted']       = $row['posted'];
      $posts[$id]['age']          = $row['age'];
      $posts[$row['following']]['following'][] = $id;
    }

    thread_list($m_table,$posts);

    $m_table->add_row(array(array('attr'=>'colspan=6 align=center','cont'=>"<input type=\"submit\" value=\"   Show selected   \" name=\"show_selected\" class=\"buttoninbox\"> <input type=\"submit\" value=\"   Show all   \" name=\"show_all\" class=\"buttoninbox\"> <input type=\"submit\" value=\"   Show threads   \" name=\"show_threads\" class=\"buttoninbox\"> <input type=\"submit\" value=\"   Post new   \" name=\"post_new\" class=\"buttoninbox\">".($admin?" <input type=\"submit\" value=\"   Delete selected   \" name=\"delete_selected\" class=\"buttoninbox\">":''))));
    $m_table->add_row(array(array('attr'=>'colspan=6 align=center','cont'=>"<input type=\"submit\" value=\"   Expand threads   \" name=\"expand\" class=\"buttoninbox\"> <input type=\"submit\" value=\"   Collapse threads   \" name=\"collapse\" class=\"buttoninbox\">&nbsp; From last <input type=\"text\" size=5 maxlength=5 name=\"age\" value=\"$age\" class=\"boxedfield\"> days")));
    
    $m_table->add_row(array(array('attr'=>'colspan=6 align=center','cont'=>'</form>')));
    #return $m_table;
    return "<form action=\"index.php\" method=\"post\">
    <input type=\"hidden\" name=\"section\" value=\"mb\">
    <input type=\"hidden\" name=\"mb_section\" value=\"board\">
    <input type=\"hidden\" name=\"board\" value=$board>
    ".$m_table->prepare()."
    </form>";

  }
  else
    return 0;
}

function post_form()
{
  global $subject,$autor,$a_email,$text,$board,$post;

  $out = '';
  $table = new BOXEDTABLE;
  $table->set_parameters(array('style'=>'subtable'));

  if($post>0)
  {
    @$res = mysql_query("select id,author,subject,text,email,send_note from messages where id=$post");
    @$row = mysql_fetch_array($res);

    $rtext = $row['text'];
    $rtext = strtr($rtext,'<>','[]');
    $rtext = eregi_replace('\\[(a[[:space:]]+href[^]]+)\\]','<\\1>',$rtext);
    $rtext = eregi_replace('\\[(\\/?[bia])\\]','<\\1>',$rtext);
    $rtext = eregi_replace('\\[br\\]','<br>',$rtext);

    $out .= "Refering to:<br>
    <strong><font size=4>$row[subject]</font></strong> (".($row['email']?"<a href=\"mailto:$row[email]\">":'').$row['author'].($row['email']?'</a>':'').")<br><br>
    <table><tr><td>
    ".
    nl2br($rtext)."
    </td></tr></table>
    <br>
    <h3>Post your comment</h3>";

    if(!eregi('^[[:space:]]*re:',$row['subject']))
      $subject = "Re: $row[subject]";
    else
      $subject = $row['subject'];
  }
  else
    $out .= '<h3>New post</h3>';

  $table->add_row(array('Your name',"<input type=\"text\" size=50 maxlength=50 name=\"autor\" value=\"$autor\" class=\"boxedfield\">"));
  $table->add_row(array('Your email',"<input type=\"text\" size=50 maxlength=50 name=\"a_email\" value=\"$a_email\" class=\"boxedfield\">"));
  $table->add_row(array('Subject',"<input type=\"text\" size=50 maxlength=160 name=\"subject\" value=\"$subject\" class=\"boxedfield\">"));
  $table->add_row(array(array('attr'=>'colspan=2','cont'=>"<br>Text:<br>
    <textarea rows=10 cols=80 wrap=virtual name=\"text\" class=\"largetext\">$text</textarea><br><br>
    <input type=\"submit\" value=\"   Send post   \" class=\"buttoninbox\">
    <input type=\"checkbox\" name=\"send_note\" value=1 class=\"checkbox\"><small>Send notification about followups")));

  $out .= "
  <form action=\"index.php\" method=\"post\">
  <input type=\"hidden\" name=\"section\" value=\"mb\">
  <input type=\"hidden\" name=\"mb_section\" value=\"board\">
  <input type=\"hidden\" name=\"board\" value=\"$board\">
  <input type=\"hidden\" name=\"action\" value=\"save_new\">
  <input type=\"hidden\" name=\"post\" value=\"$post\">
  <input type=\"hidden\" name=\"send_note_prev\" value=$row[send_note]>".
  $table->prepare();

  return $out;
}

if(!is_numeric($limit) or $limit<0) $limit = 20;
if(!is_numeric($offset) or $offset<0) $offset = 0;
if(!is_numeric($board)) return 123;
if(!is_numeric($post)) $post = 0;
if(!is_numeric($level)) $level = 'all';

if($desc == 1 or $desc == 'desc') $desc = 'desc';
else $desc = '';

@$res = mysql_query("select name,description,count,anonymous,admin_notify from parameters where id=$board");
@$row = mysql_fetch_array($res);

$messages_count = $row['count'];
$mb_page_title = $row['name'];
$mb_page_top = $row['description'];
$admin_notify = $row['admin_notify'];
$show_list=0;

if(!$row['anonymous'] and !$logged_in)
{
  $mb_page_body .= "<strong>Sorry, no anonymous access allowed on this board.</strong>";
  return 123;
}

$mb_table = new TABLE;
$mb_table->set_parameters(array('valign'=>'top'));
if(isset($show_selected))
  $action = 'selected';
elseif(isset($show_all))
  $action = 'all';
elseif(isset($show_threads))
  $action = 'threads';
elseif(isset($post_new))
  $action = 'new';
elseif(isset($delete_selected))
  $action = 'delete';

if(!is_numeric($age) and !isset($action)) $age = 60;
if(isset($expand))
{
  $level = 'all';
  $show_list=1;
}
elseif(isset($collapse))
{
  $level = 1;
  $show_list=1;
}
elseif(($action=='selected' and is_array($messages_list)) or $action=='all' or $action=='threads')
{
  if($action=='all')
    $messages_list = '';

  if($messages_table = messages_contents())
    $mb_page_body .= $messages_table->prepare();
  else
    $mb_page_body .= 'No messages';
}
elseif($action=='new' or $action == 'send')
{
  $mb_page_body .= post_form();
}
elseif($action == 'save_new')
{
  if($send_note != 1)
    $send_note = 0;
  if(!ereg('@',$a_email))
    $a_email = '';

  if(!$autor)
    post_form();
  else
  {
    @$res = mysql_query("insert into messages values(NULL,$board,'".addslashes($autor)."','".addslashes($a_email)."','".addslashes($subject)."','".addslashes($text)."',NULL,$post,$send_note)");
    @$res = mysql_query("update parameters set count=count+1 where id=$board");
    if($res)
      $mb_page_body .= "Your contribution was saved.";

    @$res = mysql_query("select email,send_note,name from messages,parameters where messages.board_id=parameters.id and messages.id=$post");
    @$row = mysql_fetch_array($res);
    if($row['send_note'] and $row['email'])
      mail($row['email'],"Re: $subject","This is a reply to your post on $row[name]:\n\n$autor wrote:\n\n$text","From: $email_from");

    if($admin_notify and $admin_email)
    {
      mail($admin_email,"Re: $subject","New post on $row[name]:\n\n$autor wrote:\n\n$text","From: $email_from");
    }
  }
  $show_list=1;
}
elseif($action=='delete' and $admin)
{
  $cnt = 0;
  while(list($k,$v) = each($messages_list))
    $list .= ($cnt++?',':'').$v;

  mysql_query("delete from messages where id in($list)");

  mysql_query("update messages set following=0,sent=sent where following in($list)");
  mysql_query("update parameters set count=count-$cnt where id=$board");

  $show_list=1;

}
else
  $show_list=1;

if($show_list)
{
  if($messages_table = messages_list())
    $mb_page_body .= $messages_table;
  else
    $mb_page_body .= "
    No messages.<br><br>
    <a href=\"?section=mb&mb_section=board&board=$board&action=new\">Post new topic</a> | <a href=\"?section=mb&mb_section=board&board=$board\">Back to board contents</a>";
}

return 123;
?>
