<a name="top"></a>
<table cellspacing=0 cellpadding=0 width="100%">
<tr valign="top">
  <td width=146 height=92 class="topleft">
  <?
    if(!$MODULES[$m]['logo'])
      $MODULES[$m]['logo'] = $CONFIGURATION['default_logo'];
  ?>
  <a href="<?echo $CONFIGURATION['server_addr']?>"><img src="<? echo $MODULES[$m]['logo']; ?>" border=0 alt="Logo"></a>
  <? echo $MODULES[$m]['mainleft']; ?>
  <div class="mainleft">
  <table class="sectionsmenu" cellspacing=0 cellpadding=0>
  <tr>
    <td><img src="images/bluebulletsquare.gif" border=0 width=13 height=13></td>
    <td class="sectinfohead"><span class="textsmall"><a href="javascript:history.go(-1);">Back</a></span></td>
  </tr>
  <tr>
    <td><img src="images/bluebulletsquare.gif" border=0 width=13 height=13></td>
    <td class="sectinfohead"><span class="textsmall"><a href="index.php">Home</a></span></td>
  </tr>
  <tr><td><img src="images/bluebulletsquare.gif" border=0 width=13 height=13></td>
      <td class="sectinfohead"><span class="textsmall"><a href="?m=about">About</a></td>
  </tr>
  </table>
  </div>
  </td>
  <td class="topright" rowspan=2>
    <table width="100%">
      <tr><td></td></tr>
      <tr><td class="toprighttop" width="100%"><span class="toprighttop"><?echo $CONFIGURATION['server_name']?></span></td></tr>
      <tr>
        <td class="toprightbottom" width="100%"><span class="toprightbottom"><? echo $MODULES[$m]['topright']; ?></span></td>
      </tr>
      <tr>
        <td class="topnavbar" width="100%">
        &nbsp;&nbsp;<?
          if(!empty($MODULES[$m]['topmenu']))
            echo $MODULES[$m]['topmenu'];
          else
            echo $CONFIGURATION['topmenu'];
        ?>
        </td>
      </tr>
      <tr>
        <td class="mainright">
        <? 
        
        if(preg_match('/--INSERTEWCONTROLHERE--/',$MODULES[$m]['mainright']))
        {
          list($pre,$post) = explode('<!--INSERTEWCONTROLHERE-->',$MODULES[$m]['mainright']);
          echo $pre;
          eval("$".$INSERTEWCONTROL."->ShowControl('800','400');");
          echo $post;
        }
        else
          echo $MODULES[$m]['mainright']; 
        
        ?>
  
        <br><br>
        <table width="100%" cellspacing=0 cellpadding=0>
          <tr>
            <td width="100%" class="datebar" colspan=2>
            <span class="datebar"><? echo date('l, j. M Y');?></span>
            </td>
          </tr>
          <tr>
            <td class="leftbottombar">
            <span class="leftbottombar">
            | <a href="#top" class="leftbottombar" style="color: #ffffff;">Top</a>
            <?
              if(!empty($MODULES[$m]['bottommenu']))
                echo $MODULES[$m]['bottommenu'];
              else
                echo "|";
            ?>
            </span>
            </td>
            <td class="rightbottombar">
            <span class="rightbottombar">
              <a href="<?echo $CONFIGURATION['server_addr']?>" class="rightbottombar"><?echo $CONFIGURATION['server_name']?></a>
            </span>
            </td>
          </tr>
        </table>
        </td>
      </tr>
    </table>
  </td>
</tr>
</table>