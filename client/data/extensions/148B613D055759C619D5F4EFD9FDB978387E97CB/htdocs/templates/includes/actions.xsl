<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="system lang os"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="generate-url">
    <xsl:param name="prefix"/>
    
    <xsl:choose>
      <xsl:when test="@confirm">        
        <xsl:variable name="text" select="lang:text(concat($prefix,'.', @name, '.confirm.title'))"/>
        <xsl:variable name="textConfirm" select="lang:text(concat($prefix,'.', @name, '.confirm.body'))"/>
        <xsl:variable name="textButton" select="lang:text(concat($prefix,'.', @name, '.confirm.button'))"/>
        <xsl:variable name="textCancel" select="lang:text('common.labels.cancel')"/>
        <xsl:text>javascript:void(osConfirm('</xsl:text>
        <xsl:value-of select="system:html-encode($text)" disable-output-escaping="yes"/>
        <xsl:text>','</xsl:text>
        <xsl:value-of select="system:html-encode($textConfirm)" disable-output-escaping="yes"/>
        <xsl:text>','</xsl:text>
        <xsl:value-of select="system:html-encode($textButton)" disable-output-escaping="yes"/>
        <xsl:text>','</xsl:text>
        <xsl:value-of select="system:html-encode($textCancel)" disable-output-escaping="yes"/>
        <xsl:text>',"</xsl:text>
        <xsl:value-of select="system:js-encode(@href)"/>
        <xsl:text>"));</xsl:text>        
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="@href"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="action-href">
    <xsl:param name="prefix"/>
    <xsl:param name="class"/>
    <xsl:param name="accesskey"/>
    <xsl:param name="href"/>
    <xsl:param name="call"/>
    <xsl:param name="content"/>        
    
    <a>
      <xsl:if test="$accesskey">
        <xsl:attribute name="accesskey">
          <xsl:value-of select="$accesskey"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$class">
        <xsl:attribute name="class">
          <xsl:value-of select="$class"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$href">
        <xsl:attribute name="href">
          
          <xsl:choose>
            <xsl:when test="@confirm">
              <xsl:variable name="text" select="lang:text(concat($prefix,'.', @name, '.confirm.title'))"/>
              <xsl:variable name="textConfirm" select="lang:text(concat($prefix,'.', @name, '.confirm.body'))"/>
              <xsl:variable name="textButton" select="lang:text(concat($prefix,'.', @name, '.confirm.button'))"/>
              <xsl:variable name="textCancel" select="lang:text('common.labels.cancel')"/>
              <xsl:text>javascript:void(osConfirm('</xsl:text>
              <xsl:value-of select="system:html-encode($text)" disable-output-escaping="yes"/>
              <xsl:text>','</xsl:text>
              <xsl:value-of select="system:html-encode($textConfirm)" disable-output-escaping="yes"/>
              <xsl:text>','</xsl:text>
              <xsl:value-of select="system:html-encode($textButton)" disable-output-escaping="yes"/>
              <xsl:text>','</xsl:text>
              <xsl:value-of select="system:html-encode($textCancel)" disable-output-escaping="yes"/>
              <xsl:text>','</xsl:text>
              <xsl:value-of select="system:js-encode($href)"/>
              <xsl:text>'));</xsl:text>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$href"/>
            </xsl:otherwise>
          </xsl:choose>
          
        </xsl:attribute>
      </xsl:if>
      <xsl:if test="$call">
        <xsl:attribute name="href">
          <xsl:value-of select="'javascript:void(0);'"/>
        </xsl:attribute>
        <xsl:attribute name="onclick">
          <xsl:text>Osiris.loadUrl('</xsl:text>
          <xsl:value-of select="$call"/>
          <xsl:text>');</xsl:text>
        </xsl:attribute>
      </xsl:if>
      <xsl:copy-of select="$content"/>
    </a>
  </xsl:template>

  <xsl:template name="action-link">
    <!-- Esempio di prefix: main.pages.home -->
    <xsl:param name="prefix"/>
    <xsl:param name="class"/>
    
    <xsl:variable name="text">
      
      <xsl:choose>
        <xsl:when test="@title">
          <xsl:value-of select="@title"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="lang:text(concat($prefix,'.', @name))"/>
        </xsl:otherwise>
      </xsl:choose>      
    </xsl:variable>

    <xsl:call-template name="action-href">
      <xsl:with-param name="prefix" select="$prefix"/>
      <xsl:with-param name="class" select="$class"/>
      <xsl:with-param name="accesskey" select="@accesskey"/>
      <xsl:with-param name="href" select="@href"/>
      <xsl:with-param name="call" select="@call"/>      
      <xsl:with-param name="content">
        <img src="{system:resource-url(concat('images/icons/16x16/',@name,'.png'))}" alt="{$text}"/>
        <xsl:value-of select="system:parse($text, false(), false(), true())" disable-output-escaping="yes"/>
      </xsl:with-param>
    </xsl:call-template>


  </xsl:template>

  <xsl:template name="action-page2">
    <xsl:param name="prefix"/>

    <xsl:variable name="text" select="lang:text(concat($prefix,'.', @name))"/>
    
    <xsl:choose>
      <xsl:when test="content">
        <div id="{@name}" data-os-otype="popup" data-os-popupMode="tooltip">
          <div data-os-popup="header">
            <xsl:call-template name="action-href">
              <xsl:with-param name="prefix" select="$prefix"/>
              <xsl:with-param name="href" select="@href"/>
              <xsl:with-param name="call" select="@call"/>
              <xsl:with-param name="content">
                <img src="{system:resource-url('images/list_arrow.gif')}"></img>
                <xsl:value-of select="$text"/>
              </xsl:with-param>
            </xsl:call-template>
          </div>
          <div data-os-popup="body">
            <div style="background-color: #f2f3f3;">
              <xsl:apply-templates select="content"/>
            </div>
          </div>
        </div>
      </xsl:when>
      <xsl:when test="@name">
        <xsl:call-template name="action-href">
          <xsl:with-param name="prefix" select="$prefix"/>
          <xsl:with-param name="href" select="@href"/>
          <xsl:with-param name="content">
            <img src="{system:resource-url('images/list_arrow.gif')}"></img>
            <xsl:value-of select="$text"/>
            <br/>
          </xsl:with-param>
        </xsl:call-template>


      </xsl:when>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="action-page">
    <xsl:param name="prefix"/>

    <xsl:variable name="text" select="lang:text(concat($prefix,'.', @name))"/>
    
    <xsl:call-template name="action-href">
      <xsl:with-param name="prefix" select="$prefix"/>
      <xsl:with-param name="href" select="@href"/>
      <xsl:with-param name="class">
        <xsl:choose>
          <xsl:when test="@selected = 'true'">os_tab_left_header_selected</xsl:when>
          <xsl:otherwise>os_tab_left_header</xsl:otherwise>
        </xsl:choose>
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:value-of select="$text"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="actions-row">
    <xsl:param name="prefix"/>

    <xsl:for-each select="actions/action">
      <xsl:call-template name="action-row">
        <xsl:with-param name="prefix" select="$prefix"/>
        <xsl:with-param name="icon" select="@name"/>        
        <xsl:with-param name="name" select="@name"/>
        <xsl:with-param name="href" select="@href"/>
        <xsl:with-param name="call" select="@call"/>
        <xsl:with-param name="content" select="@content"/>
      </xsl:call-template>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="action-row">
    <xsl:param name="prefix"/>
    <xsl:param name="name"/>
    <xsl:param name="icon"/>
    <xsl:param name="icon_path"/>
    <xsl:param name="href"/>
    <xsl:param name="call"/>
    <xsl:param name="content"/>

    <xsl:variable name="title">
      <xsl:choose>
        <xsl:when test="$prefix">
          <xsl:value-of select="system:parse(lang:text(concat($prefix,'.', $name, '.title')),false(),false(),true())"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="system:parse(lang:text($name),false(),false(),true())"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>

    <xsl:variable name="description">
      <xsl:choose>
        <xsl:when test="$prefix">
          <xsl:value-of select="system:parse(lang:text(concat($prefix,'.', $name, '.description')),false(),false(),true())"/>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="''"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:variable>
    
    <!-- This commented version support action without link. -->
    <!--
    <div class="os_action_row">      
      <xsl:if test="$icon or $icon_path">
        <div style="margin:3px;margin-right:15px;float:left;">
          <xsl:choose>
            <xsl:when test="$icon_path">
              <img src="{$icon_path}"/>
            </xsl:when>
            <xsl:otherwise>
              <img src="{system:resource-url(concat('images/icons/32x32/',$icon,'.png'))}"/>
            </xsl:otherwise>
          </xsl:choose>

        </div>
      </xsl:if>
      <xsl:choose>
        <xsl:when test="$href or $call">
          <xsl:call-template name="action-href">
            <xsl:with-param name="prefix" select="$prefix"/>
            <xsl:with-param name="href" select="$href"/>
            <xsl:with-param name="call" select="$call"/>
            <xsl:with-param name="content">
              <span style="font-size:1.2em;">
                <xsl:value-of select="$title" disable-output-escaping="yes"/>
              </span>
            </xsl:with-param>
          </xsl:call-template>          
        </xsl:when>
        <xsl:otherwise>
          <span style="font-size:1.2em;">
            <xsl:value-of select="$title" disable-output-escaping="yes"/>
          </span>
        </xsl:otherwise>
      </xsl:choose>
      <br />
      <span style="color:gray;font-size:0.9em">
        <xsl:value-of select="$description" disable-output-escaping="yes"/>
      </span>
      <xsl:if test="$content">
        <br />
        <xsl:copy-of select="$content"/>
      </xsl:if>

      <div class="os_clear"/>
    </div>
    -->

    <xsl:call-template name="action-href">
      <xsl:with-param name="prefix" select="$prefix"/>
      <xsl:with-param name="href" select="$href"/>
      <xsl:with-param name="call" select="$call"/>
      <xsl:with-param name="class" select="'os_action_row'"/>
      <xsl:with-param name="content">
        <xsl:if test="$icon or $icon_path">
          <div style="margin:3px;margin-right:15px;float:left;">
            <xsl:choose>
              <xsl:when test="$icon_path">
                <img src="{$icon_path}"/>
              </xsl:when>
              <xsl:otherwise>
                <img src="{system:resource-url(concat('images/icons/32x32/',$icon,'.png'))}"/>
              </xsl:otherwise>
            </xsl:choose>

          </div>
        </xsl:if>

        <span style="font-size:1.2em;">
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </span>
        <br />
        <span style="color:gray;font-size:0.9em">
          <xsl:value-of select="$description" disable-output-escaping="yes"/>
        </span>
        <xsl:if test="$content">
          <br />
          <xsl:copy-of select="$content"/>
        </xsl:if>

        <div class="os_clear"/>
      </xsl:with-param>
    </xsl:call-template>
    
    

  </xsl:template>

  <xsl:template name="action-icon">
    <xsl:param name="prefix"/>
    <xsl:param name="name"/>
    <xsl:param name="icon"/>
    <xsl:param name="href"/>
    <xsl:param name="call"/>
    <xsl:param name="content"/>

    <xsl:variable name="title" select="system:parse(lang:text(concat($prefix,'.', $name, '.title')),false(),false(),true())"/>
    <xsl:variable name="description" select="system:parse(lang:text(concat($prefix,'.', $name, '.description')),false(),false(),true())"/>

    <span class="os_actions_icon">
      <xsl:call-template name="action-href">
        <xsl:with-param name="href" select="$href"/>
        <xsl:with-param name="call" select="$call"/>
        <xsl:with-param name="content">
          <xsl:attribute name="data-os-tooltip">
            <xsl:value-of select="$description" disable-output-escaping="yes"/>
          </xsl:attribute>
          <xsl:if test="$icon">
            <img class="os_nolink" src="{system:resource-url(concat('images/icons/32x32/',$icon,'.png'))}"/>
          </xsl:if>
          <br />
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </xsl:with-param>
      </xsl:call-template>
      
    </span>

  </xsl:template>

  <!--
  <xsl:template name="actions-object">
    <xsl:param name="prefix"/>

    <xsl:variable name="text" select="lang:text(concat($prefix,'.actions.', @name))"/>
    <xsl:variable name="href">
      <xsl:call-template name="action-href">
        <xsl:with-param name="prefix" select="$prefix"/>
        <xsl:with-param name="href" select="@href"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:if test="pager">
      <div class="os_commands_left">
        <xsl:apply-templates select="pager"/>
      </div>
    </xsl:if>
    <div class="os_commands_right">
      <xsl:for-each select="actions/action">
        <a class="os_button" href="{$href}">
          <xsl:value-of select="$text"/>
        </a>
      </xsl:for-each>
    </div>
    <div class="os_clear"/>
  </xsl:template>
  -->



  <xsl:template name="actions-popup">
    <xsl:param name="prefix"/>
    <xsl:param name="actions"/>
    <xsl:param name="default" select="$actions/*[1]/@name"/>
    <xsl:param name="actions_style" select="''"/>

    <!--
    <div class="os_actions_popup" data-os-otype="popup" data-os-popupMode="staticclick">
      <div data-os-popup="header" class="os_actions_popup_default">
        <xsl:for-each select="$actions/action[@name=$default]">
          <xsl:call-template name="action-link">
            <xsl:with-param name="prefix" select="$prefix"/>            
          </xsl:call-template>
        </xsl:for-each>
      </div>

      <xsl:if test="count($actions/*)>1">
        <div data-os-popup="body" style="display:none;" class="os_actions_popup_actions">
          <xsl:for-each select="$actions/action[@name!=$default]">
            <div class="os_actions_popup_action">              
              <xsl:call-template name="action-link">
                <xsl:with-param name="prefix" select="$prefix"/>
              </xsl:call-template>
            </div>
          </xsl:for-each>
        </div>
      </xsl:if>
    </div>
    -->

    <div class="os_actions_popup" data-os-otype="popup" data-os-popupMode="staticclick">
      
      <xsl:if test="count($actions/*)>1">
        <div class="os_actions_popup_open" data-os-popup="header">
          <img src="{system:resource-url('images/action_padded.gif')}" />
        </div>
      </xsl:if>

      <xsl:for-each select="$actions/action[@name=$default]">
        <xsl:call-template name="action-link">
          <xsl:with-param name="prefix" select="$prefix"/>
          <xsl:with-param name="class">
            <xsl:choose>
              <xsl:when test="count($actions/*)>1">
                <xsl:value-of select="'os_actions_popup_default'"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="'os_actions_popup_default_alone'"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:for-each>
    
      <xsl:if test="count($actions/*)>1">
        <div data-os-popup="body" style="display:none;{$actions_style}" class="os_actions_popup_actions">
          <xsl:for-each select="$actions/action[@name!=$default]">
            <xsl:call-template name="action-link">
              <xsl:with-param name="prefix" select="$prefix"/>
              <xsl:with-param name="class" select="'os_actions_popup_action'"/>              
            </xsl:call-template>
          </xsl:for-each>
        </div>
      </xsl:if>
    </div>
    
  </xsl:template>

</xsl:stylesheet>