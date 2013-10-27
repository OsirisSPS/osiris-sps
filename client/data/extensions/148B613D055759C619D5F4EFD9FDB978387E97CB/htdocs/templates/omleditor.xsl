<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="os lang date system"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="text_editor"/>
  <xsl:param name="mode_lite"/>
  <xsl:param name="mode_secure"/>
  <xsl:param name="mode_mode"/>
  <xsl:param name="mode_report"/>

  <xsl:template match="omlEditor">
    <div class="os_omlEditor" data-os-otype="omleditor">
      <div class="os_omlEditor_toolbar" style="float:left;">
        <xsl:call-template name="select">
          <xsl:with-param name="name" select="'paragraph'"/>
          <xsl:with-param name="options">
            <option value="">
              <xsl:value-of select="lang:text('omleditor.commands.paragraph')" />
            </option>
            <option value="pre">
              <xsl:value-of select="lang:text('omleditor.commands.pre')" />
            </option>
            <option value="h1">
              <xsl:value-of select="lang:text('omleditor.commands.h1')" />
            </option>
            <option value="h2">
              <xsl:value-of select="lang:text('omleditor.commands.h2')" />
            </option>
            <option value="h3">
              <xsl:value-of select="lang:text('omleditor.commands.h3')" />
            </option>
            <option value="h4">
              <xsl:value-of select="lang:text('omleditor.commands.h4')" />
            </option>
            <option value="h5">
              <xsl:value-of select="lang:text('omleditor.commands.h5')" />
            </option>
            <option value="h6">
              <xsl:value-of select="lang:text('omleditor.commands.h6')" />
            </option>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:text> </xsl:text>
        <xsl:call-template name="select">
          <xsl:with-param name="name" select="'size'"/>
          <xsl:with-param name="tag" select="'size'"/>
          <xsl:with-param name="options">
            <option value="">
              <xsl:value-of select="lang:text('omleditor.commands.size')" />
            </option>
            <option value="50%">
              <xsl:value-of select="lang:text('omleditor.commands.size.tiny')" />
            </option>
            <option value="75%">
              <xsl:value-of select="lang:text('omleditor.commands.size.small')" />
            </option>
            <option value="100%">
              <xsl:value-of select="lang:text('omleditor.commands.size.normal')" />
            </option>
            <option value="150%">
              <xsl:value-of select="lang:text('omleditor.commands.size.large')" />
            </option>
            <option value="200%">
              <xsl:value-of select="lang:text('omleditor.commands.size.huge')" />
            </option>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:text> </xsl:text>
        <xsl:call-template name="select">
          <xsl:with-param name="name" select="'color'"/>
          <xsl:with-param name="tag" select="'color'"/>
          <xsl:with-param name="options">
            <option value="" style="color:black">
              <xsl:value-of select="lang:text('omleditor.commands.color')"/>
            </option>
            <option value="black" style="color:black">Black</option>
            <option value="silver" style="color:silver">Silver</option>
            <option value="gray" style="color:gray">Gray</option>
            <option value="maroon" style="color:maroon">Maroon</option>
            <option value="red" style="color:red">Red</option>
            <option value="purple" style="color:purple">Purple</option>
            <option value="fuchsia" style="color:fuchsia">Fuchsia</option>
            <option value="navy" style="color:navy">Navy</option>
            <option value="blue" style="color:blue">Blue</option>
            <option value="aqua" style="color:aqua">Aqua</option>
            <option value="teal" style="color:teal">Teal</option>
            <option value="lime" style="color:lime">Lime</option>
            <option value="green" style="color:green">Green</option>
            <option value="olive" style="color:olive">Olive</option>
            <option value="yellow" style="color:yellow">Yellow</option>
            <option value="white" style="color:white;background-color:gray">White</option>
          </xsl:with-param>
        </xsl:call-template>
      </div>

      <div class="os_omlEditor_toolbar" style="float:right;margin-top:5px;">
        <a href="{@preview_href}" class="os_button">
          <xsl:value-of select="lang:text('omleditor.actions.preview')"/>
        </a>
      </div>

      <div class="os_omlEditor_toolbar" style="float:left;">
        <div style="float:left" data-os-otype="popup" data-os-popupMode="popup">
          <div data-os-popup="header" data-os-tooltip="{lang:text('omleditor.commands.emoticons')}">
            <img class="os_omlEditor_tagButton" onmouseover="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_hover.gif')})';" onmouseout="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_normal.gif')})';" src="{system:resource-url(concat('images/oml/editor/','emoticons','.gif'))}" width="23" height="22" alt="emoticons" title=""/>
          </div>
          <div id="{/omlEditor/@areaId}Emoticons" class="os_omlEditor_emoticons" style="display:none;" data-os-popup="body">
            <xsl:apply-templates select="emoticons/emoticon"/>
          </div>
        </div>

        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'bold'"/>
          <xsl:with-param name="tag" select="'b'"/>
          <xsl:with-param name="shortkey" select="'b'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'italic'"/>
          <xsl:with-param name="tag" select="'i'"/>
          <xsl:with-param name="shortkey" select="'i'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'underline'"/>
          <xsl:with-param name="tag" select="'u'"/>
          <xsl:with-param name="shortkey" select="'u'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'strike'"/>
          <xsl:with-param name="tag" select="'s'"/>
          <xsl:with-param name="shortkey" select="'s'"/>
        </xsl:call-template>
        <xsl:call-template name="separator"/>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'quote'"/>
          <xsl:with-param name="tag" select="'quote'"/>
          <xsl:with-param name="shortkey" select="'q'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'code'"/>
          <xsl:with-param name="tag" select="'code'"/>
          <xsl:with-param name="shortkey" select="'c'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'url'"/>
          <xsl:with-param name="tag" select="'url'"/>
          <xsl:with-param name="shortkey" select="'u'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'img'"/>
          <xsl:with-param name="tag" select="'img'"/>
          <xsl:with-param name="shortkey" select="'i'"/>
        </xsl:call-template>
        <xsl:call-template name="separator"/>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'left'"/>
          <xsl:with-param name="tag" select="'left'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'center'"/>
          <xsl:with-param name="tag" select="'center'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'right'"/>
          <xsl:with-param name="tag" select="'right'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonTag">
          <xsl:with-param name="name" select="'justify'"/>
          <xsl:with-param name="tag" select="'justify'"/>
        </xsl:call-template>
        <xsl:call-template name="separator"/>
        <xsl:call-template name="buttonScript">
          <xsl:with-param name="name" select="'wrap'"/>
          <xsl:with-param name="script" select="'wrap'"/>
        </xsl:call-template>
        <!--
        <xsl:call-template name="buttonScript">
          <xsl:with-param name="name" select="'height_inc'"/>
          <xsl:with-param name="script" select="'height_inc'"/>
        </xsl:call-template>
        <xsl:call-template name="buttonScript">
          <xsl:with-param name="name" select="'height_dec'"/>
          <xsl:with-param name="script" select="'height_dec'"/>
        </xsl:call-template>
        -->
        <xsl:call-template name="separator"/>
        <div style="float:left;" data-os-otype="popup" data-os-popupMode="popup">
          <div data-os-popup="header" data-os-tooltip="{lang:text('omleditor.commands.advanced')}">
            <img class="os_omlEditor_tagButton" onmouseover="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_hover.gif')})';" onmouseout="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_normal.gif')})';" src="{system:resource-url(concat('images/oml/editor/','advanced','.png'))}" width="23" height="22" alt="advanced" title=""/>
          </div>
          <div id="{/omlEditor/@areaId}Advanced" class="os_omlEditor_advanced" style="display:none;" data-os-popup="body">
            <xsl:value-of select="$mode_lite" disable-output-escaping="yes"/>
            <br />
            <xsl:value-of select="$mode_secure" disable-output-escaping="yes"/>
            <br />
            <xsl:value-of select="$mode_mode" disable-output-escaping="yes"/>
            <br />
            <xsl:value-of select="$mode_report" disable-output-escaping="yes"/>
            <br />
            <br />
            <a style="display:block;" href="{@html2oml_href}" class="os_button">
              <xsl:value-of select="lang:text('omleditor.commands.html2oml')"/>
            </a>
            <a style="display:block;" href="{@htmlandbbcode2oml_href}" class="os_button">
              <xsl:value-of select="lang:text('omleditor.commands.htmlandbbcode2oml')"/>
            </a>
            
            <br />
            <!--
          <br />
          <a href="{@clean_href}" class="os_button">
            <xsl:value-of select="'clean'"/>
          </a>
          -->
          </div>
        </div>
        <xsl:call-template name="separator"/>
        <xsl:call-template name="buttonLink">
          <xsl:with-param name="name" select="'help'"/>
          <xsl:with-param name="js" select="'Osiris.loadUrl(&quot;/main/omlhelp?mode=dialog&quot;);'"/>          
        </xsl:call-template>

      </div>

      <div style="clear:both;">
        <xsl:value-of select="$text_editor" disable-output-escaping="yes"/>        
      </div>
      
      <xsl:if test="@preview">
        <div>
          <xsl:if test="@extended = 'true'">
            <xsl:attribute name="colspan">2</xsl:attribute>
          </xsl:if>
          <span class="os_label">
            <xsl:value-of select="lang:text('omleditor.preview')"/>
            <xsl:text>: </xsl:text>
          </span>

          <div class="os_omlEditor_preview">
            <xsl:value-of select="@preview" disable-output-escaping="yes"/>
          </div>

          <xsl:if test="@report!=''">
            <span class="os_label">
              <xsl:value-of select="lang:text('omleditor.warning')"/>
              <xsl:text>: </xsl:text>
            </span>
            <div class="os_omlEditor_preview">
              <xsl:value-of select="system:parse(@report)" disable-output-escaping="yes"/>
            </div>
          </xsl:if>
        </div>
      </xsl:if>
    </div>

  </xsl:template>

  <xsl:template match="emoticon">
    <a class="os_omlEditor_emoticon" href="javascript:omlEditorEmoticon('{/omlEditor/@areaId}', '{@code}');" data-os-tooltip="{concat('&lt;b&gt;',lang:text(concat('emoticons.',@name)),'&lt;/b&gt;&lt;br /&gt;',@code)}">
      <img src="{@href}" title="" alt="{@name}"/>
    </a>
  </xsl:template>

  <xsl:template name="buttonTag">
    <xsl:param name="name"></xsl:param>
    <xsl:param name="tag"></xsl:param>
    <xsl:param name="shortkey"></xsl:param>

    <div style="float:left">
      <a class="os_nolink" href="javascript:void(0);" accesskey="{$shortkey}" onclick="omlEditorTag('{@areaId}', '[{$tag}]', '[/{$tag}]'); return false;" data-os-tooltip="{lang:text(concat('omleditor.commands.',$name))}">
        <img class="os_omlEditor_tagButton" onmouseover="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_hover.gif')})';" onmouseout="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_normal.gif')})';" src="{system:resource-url(concat('images/oml/editor/',$name,'.gif'))}" width="23" height="22" alt="{$name}" title=""/>        
      </a>
    </div>
  </xsl:template>

  <xsl:template name="buttonLink">
    <xsl:param name="name"></xsl:param>
    <xsl:param name="js"></xsl:param>

    <div style="float:left">
      <a class="os_nolink" href="javascript:void(0);" onclick="{$js}" data-os-tooltip="{lang:text(concat('omleditor.commands.',$name))}">        
        <img class="os_omlEditor_tagButton" onmouseover="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_hover.gif')})';" onmouseout="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_normal.gif')})';" src="{system:resource-url(concat('images/oml/editor/',$name,'.gif'))}" width="23" height="22" alt="{$name}" title=""/>
      </a>
    </div>
  </xsl:template>

  <xsl:template name="buttonScript">
    <xsl:param name="name"></xsl:param>
    <xsl:param name="script"></xsl:param>

    <div style="float:left">
      <a class="os_nolink" href="javascript:void(0);" onclick="omlEditorScript_{$script}('{@areaId}'); return false;" data-os-tooltip="{lang:text(concat('omleditor.commands.',$script))}">
        <img class="os_omlEditor_tagButton" onmouseover="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_hover.gif')})';" onmouseout="this.style.backgroundImage = 'url({system:resource-url('images/oml/editor/back_normal.gif')})';" src="{system:resource-url(concat('images/oml/editor/',$name,'.gif'))}" width="23" height="22" alt="{$name}" title=""/>
      </a>
    </div>
  </xsl:template>

  <xsl:template name="select">
    <xsl:param name="name"></xsl:param>
    <xsl:param name="tag"></xsl:param>
    <xsl:param name="options"></xsl:param>
    <xsl:choose>
      <xsl:when test="$tag">
        <select class="os_omlEditor_tagSelect" name="fontsize" onchange="javascript:omlEditorTag('{@areaId}', '[{$tag}=' + this.options[this.selectedIndex].value + ']', '[/{$tag}]'); this.selectedIndex=0;">
          <xsl:copy-of select="$options"/>
        </select>
      </xsl:when>
      <xsl:otherwise>
        <select class="os_omlEditor_tagSelect" name="fontsize" onchange="javascript:omlEditorTag('{@areaId}', '[' + this.options[this.selectedIndex].value + ']', '[/' + this.options[this.selectedIndex].value + ']'); this.selectedIndex=0;">
          <xsl:copy-of select="$options"/>
        </select>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="separator">
    <div style="float:left">
      <img src="{system:resource-url('images/oml/editor/divider.gif')}" alt=""/>
    </div>
  </xsl:template>

</xsl:stylesheet>