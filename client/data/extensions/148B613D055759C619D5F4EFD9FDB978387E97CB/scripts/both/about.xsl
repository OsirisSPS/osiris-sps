<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html"/>
  
  <xsl:template match="page">

    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('main.pages.about.title')}" data-os-otype="dialog" data-os-dialog-width="window" data-os-dialog-height="window">
          <xsl:call-template name="page"/>
        </div>        
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="page"/>        
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="page">
    <xsl:choose>
      <xsl:when test="@filter = ''">
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.about.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="pages"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="@filter = 'general'">
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.about.title.general')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="about"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="@filter = 'changelog'">
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.about.title.changelogs')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="changelog"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="@filter = 'license'">
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.about.title.license')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="license"/>
          </xsl:with-param>
        </xsl:call-template>
        
      </xsl:when>
      <xsl:when test="@filter = 'libraries'">
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.about.title.third_parties')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="libraries"/>
          </xsl:with-param>
        </xsl:call-template>
        
      </xsl:when>
      <xsl:when test="@filter = 'sysinfo'">
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.about.title.system_info')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="sysinfo"/>
          </xsl:with-param>
        </xsl:call-template>
        
      </xsl:when>
    </xsl:choose>
    
  </xsl:template>

  <xsl:template name="code">
    <xsl:param name="text"/>
    <pre style="padding:10px;margin:10px;border:1px dotted gray;background-color:white">
      <xsl:value-of select="$text"/>
    </pre>
  </xsl:template>

  <xsl:template name="pages">
    <div style="display:none" data-os-otype="tab">
      <div data-os-tabType="header" data-os-tabPage="0">
        <xsl:value-of select="lang:text('main.pages.about.title.general')"/>
      </div>
      <div data-os-tabType="body" data-os-tabPage="0">
        <xsl:call-template name="about"/>
      </div>
      <div data-os-tabType="header" data-os-tabPage="1">
        <xsl:value-of select="lang:text('main.pages.about.title.changelogs')"/>
      </div>
      <div data-os-tabType="body" data-os-tabPage="1">
        <xsl:call-template name="changelog"/>
      </div>
      <div data-os-tabType="header" data-os-tabPage="2">
        <xsl:value-of select="lang:text('main.pages.about.title.license')"/>
      </div>
      <div data-os-tabType="body" data-os-tabPage="2">
        <xsl:call-template name="license"/>
      </div>
      <div data-os-tabType="header" data-os-tabPage="3">
        <xsl:value-of select="lang:text('main.pages.about.title.third_parties')"/>
      </div>
      <div data-os-tabType="body" data-os-tabPage="3">
        <xsl:call-template name="libraries"/>
      </div>
      <xsl:if test="//installer">
        <div data-os-tabType="header" data-os-tabPage="4">
          <xsl:value-of select="lang:text('main.pages.about.title.system_info')"/>
        </div>
        <div data-os-tabType="body" data-os-tabPage="4">
          <xsl:call-template name="sysinfo"/>
        </div>
      </xsl:if>
    </div>
  </xsl:template>

  <xsl:template name="about">

		<div style="text-align:center">
    	<div id="container" style="margin:30px;">

    	</div>

      <div>
        <a class="os_button" title="{lang:text('systembar.actions.help.homepage')}" href="{@href_home}" target="_blank">
          <xsl:value-of select="lang:text('systembar.actions.help.homepage')"/>
          <xsl:text> </xsl:text>
          <img src="{system:resource-url('images/link_external.png')}" />
        </a>
        <xsl:call-template name="separator" />
        <a class="os_button" title="{lang:text('systembar.actions.help.forum')}" href="{@href_forum}" target="_blank">
          <xsl:value-of select="lang:text('systembar.actions.help.forum')"/>
          <xsl:text> </xsl:text>
          <img src="{system:resource-url('images/link_external.png')}" />
        </a>
      </div>

      <div>        
        
        <div style="display:inline-block;width:200px;margin:20px;vertical-align:text-top;">
          <xsl:apply-templates select="credits/group[@group='main']"/>
        </div>

        <xsl:for-each select="credits/group[@group='supporters']">
          <div style="display:inline-block;width:200px;margin:20px;vertical-align:text-top;">
            <xsl:apply-templates select="."/>
          </div>
        </xsl:for-each>
      </div>
      
    </div>
    <script type="text/javascript">
      
      <xsl:text>$(function() { $.getScript(Osiris.adjustStaticUrl("/htdocs/js/libraries/ripple.js")); ripple('</xsl:text>
      <xsl:value-of select="system:resource-url('images/about/logobox.png')"/>
      <xsl:text>', document.getElementById('container')); });</xsl:text>
    </script>

    <!--
    <a class="os_button" title="{lang:text('systembar.actions.help.about')}" href="javascript:void(0);" onclick="Osiris.loadUrl('/main/about?mode=dialog&amp;filter=libraries')">
      <xsl:value-of select="lang:text('systembar.actions.help.about')"/>
    </a>


    <div style="float:right;text-align:center;margin-top:10px;margin-bottom:10px;">
      <a class="os_nolink" href="{@href_home}">
        <img style="margin:5px;" src="{system:resource-url('images/logo/logo1.png')}"/>
      </a>
      <br/>
      <span style="font-size:1.5em;font-style:italic;">
        <xsl:value-of select="@version"/>
      </span>
    </div>
    -->
    


    <div style="clear:both;text-align:right;font-size:1.5em;padding:20px;">
      <xsl:value-of select="lang:text('main.pages.about.footer_thanks')"/>      
    </div>
    
    
    
  </xsl:template>

  <xsl:template match="group">
    <table class="os_table_data">
      <xsl:value-of select="@label"/>
      <xsl:call-template name="group_info">
        <xsl:with-param name="label" select="@title"/>
      </xsl:call-template>

      <xsl:for-each select="person">
        <xsl:sort select="@nick" order="ascending"/>
        <tr>
          <td>
            <xsl:value-of select="@nick"/>
          </td>
        </tr>
      </xsl:for-each>
    </table>
  </xsl:template>

  <xsl:template name="changelog">
    <xsl:call-template name="code">
      <xsl:with-param name="text" select="@changelog"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="license">
    <xsl:call-template name="code">
      <xsl:with-param name="text" select="@license"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="libraries">
    <xsl:if test="libraries/library">
      <table class="os_table">        
        <xsl:for-each select="libraries/library">
          <xsl:sort select="@name" order="ascending"/>

          <xsl:call-template name="group_info">
            <xsl:with-param name="label" select="@name"/>
          </xsl:call-template>
          
          <xsl:call-template name="row_info">
            <xsl:with-param name="label" select="lang:text('main.pages.about.libraries.name')"/>
            <xsl:with-param name="value" select="@name"/>
          </xsl:call-template>

          <xsl:if test="@version">
            <xsl:call-template name="row_info">
              <xsl:with-param name="label" select="lang:text('main.pages.about.libraries.version')"/>
              <xsl:with-param name="value" select="@version"/>
            </xsl:call-template>
          </xsl:if>

          <xsl:if test="@description">
            <xsl:call-template name="row_info">
              <xsl:with-param name="label" select="lang:text('main.pages.about.libraries.description')"/>
              <xsl:with-param name="value" select="@description"/>
            </xsl:call-template>
          </xsl:if>

          <xsl:if test="@href_home">
            <xsl:call-template name="row_info">
              <xsl:with-param name="label" select="lang:text('main.pages.about.libraries.url')"/>
              <xsl:with-param name="value">                
                <xsl:value-of select="system:parse(concat('[url=',@href_home,']',@home,'[/url]'))" disable-output-escaping="yes"/>
              </xsl:with-param>
              <xsl:with-param name="copy" select="1"/>
            </xsl:call-template>
          </xsl:if>

          <xsl:if test="@license">
            <xsl:call-template name="row_info">
              <xsl:with-param name="label" select="lang:text('main.pages.about.libraries.license')"/>
              <xsl:with-param name="value">
                <xsl:choose>
                  <xsl:when test="@href_license">
                    <xsl:value-of select="system:parse(concat('[url=',@href_license,']',@license,'[/url]'))" disable-output-escaping="yes"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="@license"/>
                  </xsl:otherwise>
                </xsl:choose>                
              </xsl:with-param>
              <xsl:with-param name="copy">
                <xsl:choose>
                  <xsl:when test="@href_license">
                    <xsl:value-of select="1"/>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:value-of select="0"/>
                  </xsl:otherwise>
                </xsl:choose>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:if>

        </xsl:for-each>
      </table>
    </xsl:if>
  </xsl:template>

  <xsl:template name="sysinfo">
    <table class="os_table">
      <xsl:call-template name="group_info">
        <xsl:with-param name="label" select="lang:text('main.pages.about.sysinfo.system')"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Platform'"/>
        <xsl:with-param name="jsvar" select="'navigator.platform'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Screen color depth'"/>
        <xsl:with-param name="jsvar" select="'screen.colorDepth'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Screen width'"/>
        <xsl:with-param name="jsvar" select="'screen.width'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Screen height'"/>
        <xsl:with-param name="jsvar" select="'screen.height'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Screen available width'"/>
        <xsl:with-param name="jsvar" select="'screen.availWidth'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Screen available height'"/>
        <xsl:with-param name="jsvar" select="'screen.availHeight'"/>
      </xsl:call-template>
      <xsl:call-template name="group_info">
        <xsl:with-param name="label" select="lang:text('main.pages.about.sysinfo.browser')"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Name'"/>
        <xsl:with-param name="jsvar" select="'navigator.appName'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Version'"/>
        <xsl:with-param name="jsvar" select="'navigator.appVersion'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'User agent'"/>
        <xsl:with-param name="jsvar" select="'navigator.userAgent'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Language'"/>
        <xsl:with-param name="jsvar" select="'navigator.browserLanguage'"/>
      </xsl:call-template>
      <xsl:call-template name="infojs">
        <xsl:with-param name="label" select="'Cookie enabled'"/>
        <xsl:with-param name="jsvar" select="'navigator.cookieEnabled'"/>
      </xsl:call-template>
      <xsl:call-template name="group_info">
        <xsl:with-param name="label" select="lang:text('main.pages.about.sysinfo.installation')"/>
      </xsl:call-template>
      <xsl:call-template name="row_info">
        <xsl:with-param name="label" select="'Share path'"/>
        <xsl:with-param name="value" select="//installer/@folders.share"/>
      </xsl:call-template>
      <xsl:call-template name="row_info">
        <xsl:with-param name="label" select="'Data path'"/>
        <xsl:with-param name="value" select="//installer/@folders.data"/>
      </xsl:call-template>
      <xsl:call-template name="row_info">
        <xsl:with-param name="label" select="'Temp path'"/>
        <xsl:with-param name="value" select="//installer/@folders.temp"/>
      </xsl:call-template>
      <xsl:call-template name="row_info">
        <xsl:with-param name="label" select="'Log path'"/>
        <xsl:with-param name="value" select="//installer/@folders.log"/>
      </xsl:call-template>
    </table>
  </xsl:template>
    
  <xsl:template name="infojs">
    <xsl:param name="label"/>
    <xsl:param name="jsvar"/>
    <tr>
      <td class="os_label">
        <xsl:value-of select="$label"/>
        <xsl:text>: </xsl:text>
      </td>
      <td class="os_value">
        <div id="jsvar.{$jsvar}">
          <script type="text/javascript">            
            osGetById("jsvar.<xsl:value-of select="$jsvar"/>").innerHTML = <xsl:value-of select="$jsvar"/>;
          </script>
        </div>
      </td>
    </tr>
  </xsl:template>

  <xsl:template name="row_info">
    <xsl:param name="label"/>
    <xsl:param name="value"/>
    <xsl:param name="copy" select="0"/>
    <tr>
      <td class="os_label">
        <xsl:value-of select="$label"/>
        <xsl:text>: </xsl:text>
      </td>
      <td class="os_value">
        <xsl:choose>
          <xsl:when test="$copy = 0">
            <xsl:value-of select="$value"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:copy-of select="$value"/>
          </xsl:otherwise>                    
        </xsl:choose>        
      </td>
    </tr>
  </xsl:template>

  <xsl:template name="group_info">
    <xsl:param name="label"/>
    <tr>
      <th colspan="2">
        <xsl:value-of select="$label"/>
      </th>
    </tr>
  </xsl:template>
  
</xsl:stylesheet>