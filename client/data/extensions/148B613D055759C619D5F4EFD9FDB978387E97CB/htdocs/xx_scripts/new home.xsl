<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/dump_xml.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:template match="page">
    <xsl:call-template name="dump_xml"/>
    <!--
    <div class="os_commands_left">
      <a class="os_button_big200" href="{actions/action[@name='create_portal']/@href}">
        <xsl:value-of select="lang:text('main.pages.home.actions.create_portal')"/>
      </a>
      <a class="os_button_big200" href="{actions/action[@name='subscribe_portal']/@href}">
        <xsl:value-of select="lang:text('main.pages.home.actions.subscribe_portal')"/>
      </a>
    </div>
    <div class="os_commands_right">
      <a class="os_button_big200" href="{actions/action[@name='settings']/@href}">
        <xsl:value-of select="lang:text('main.pages.home.actions.settings')"/>
      </a>      
    </div>
    -->

    <div style="float:left;width:49%;border:1px solid red;">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('main.pages.home.welcome')" />
        <xsl:with-param name="content">
          <a title="{lang:text('systembar.actions.help.quick')}" class="os_systembar_link" href="/main/help">
            <xsl:value-of select="lang:text('systembar.actions.help.quick')"/>
          </a>
          <xsl:call-template name="separator" />
          <a title="{lang:text('systembar.actions.help.about')}" class="os_systembar_link" href="/main/about">
            <xsl:value-of select="lang:text('systembar.actions.help.about')"/>
          </a>
          <xsl:call-template name="separator" />
          <a title="{lang:text('systembar.actions.help.homepage')}" class="os_systembar_link" href="{@href_home}" target="_blank">
            <xsl:value-of select="lang:text('systembar.actions.help.homepage')"/>
            <xsl:text> </xsl:text>
            <img src="{system:resource-url('images/link_external.png')}" />
          </a>
          <xsl:call-template name="separator" />
          <a title="{lang:text('systembar.actions.help.docs')}" class="os_systembar_link" href="{@href_docs}" target="_blank">
            <xsl:value-of select="lang:text('systembar.actions.help.docs')"/>
            <xsl:text> </xsl:text>
            <img src="{system:resource-url('images/link_external.png')}" />
          </a>
          <xsl:call-template name="separator" />
          <a title="{lang:text('systembar.actions.help.forum')}" class="os_systembar_link" href="{@href_forum}" target="_blank">
            <xsl:value-of select="lang:text('systembar.actions.help.forum')"/>
            <xsl:text> </xsl:text>
            <img src="{system:resource-url('images/link_external.png')}" />
          </a>
        </xsl:with-param>
      </xsl:call-template>
    </div>    
    <div style="float:right;width:49%;border:1px solid red;">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('main.pages.home.welcome')" />
        <xsl:with-param name="content">
          <iframe style="width:100%;height:100px;" src="http://www.osiris-sps.org"></iframe>
        </xsl:with-param>
      </xsl:call-template>
    </div>

    <div>
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('main.pages.home.subscribed_portals')" />
        <xsl:with-param name="content">
          <table class="os_table2">
            <xsl:choose>
              <xsl:when test="subscribedPortals">
                <xsl:call-template name="row-header"/>
                <xsl:for-each select="subscribedPortals/portal">
                  <xsl:sort select="@name" order="ascending"/>
                  <xsl:call-template name="row-portal"/>
                </xsl:for-each>
              </xsl:when>
              <xsl:otherwise>
                <tr>
                  <td colspan="4" style="text-align:center;">
                    <b>
                      <xsl:value-of select="lang:text('main.pages.home.no_portals')"/>
                    </b>
                  </td>
                </tr>
              </xsl:otherwise>
            </xsl:choose>
          </table>
        </xsl:with-param>
      </xsl:call-template>
    </div>

    <div>
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('main.pages.home.actions')" />
        <xsl:with-param name="content">
          <xsl:call-template name="action-row">
            <xsl:with-param name="suffix" select="'portal.pages.home'"/>
            <xsl:with-param name="icon" select="'settings'"/>
            <xsl:with-param name="name" select="'settings'"/>
            <xsl:with-param name="href" select="actions/action[@name='settings']/@href"/>
          </xsl:call-template>
          <xsl:call-template name="action-row">
            <xsl:with-param name="suffix" select="'portal.pages.home'"/>
            <xsl:with-param name="icon" select="'subscribe'"/>
            <xsl:with-param name="name" select="'subscribe_portal'"/>
            <xsl:with-param name="href" select="actions/action[@name='subscribe_portal']/@href"/>
          </xsl:call-template>
          <xsl:call-template name="action-row">
            <xsl:with-param name="suffix" select="'portal.pages.home'"/>
            <xsl:with-param name="icon" select="'create'"/>
            <xsl:with-param name="name" select="'create_portal'"/>
            <xsl:with-param name="href" select="actions/action[@name='create_portal']/@href"/>
          </xsl:call-template>
          <div data-os-otype="popup" os:popupMode="staticclick">
            <div class="os_more" data-os-popup="header">More..</div>
            <div style="display:none" data-os-popup="body">
              <xsl:call-template name="action-row">
                <xsl:with-param name="suffix" select="'portal.pages.home'"/>
                <xsl:with-param name="icon" select="'isis'"/>
                <xsl:with-param name="name" select="'isis'"/>
                <xsl:with-param name="href" select="actions/action[@name='isis']/@href"/>
              </xsl:call-template>
              <xsl:call-template name="action-row">
                <xsl:with-param name="suffix" select="'portal.pages.home'"/>
                <xsl:with-param name="icon" select="'jobs'"/>
                <xsl:with-param name="name" select="'jobs'"/>
                <xsl:with-param name="href" select="actions/action[@name='jobs']/@href"/>
              </xsl:call-template>
              <xsl:call-template name="action-row">
                <xsl:with-param name="suffix" select="'portal.pages.home'"/>
                <xsl:with-param name="icon" select="'maintenance'"/>
                <xsl:with-param name="name" select="'maintenance'"/>
                <xsl:with-param name="href" select="actions/action[@name='maintenance']/@href"/>
              </xsl:call-template>
            </div>
          </div>
        </xsl:with-param>
      </xsl:call-template>
    </div>
        
  </xsl:template>

  <xsl:template name="row-header">
    <tr>
      <th width="100%">
        <xsl:value-of select="lang:text('portal.name')"/>
      </th>
      <th class="os_center os_nowrap">
        <xsl:value-of select="lang:text('main.pages.home.typology')"/>
      </th>
      <th class="os_center os_nowrap">
        <xsl:value-of select="lang:text('main.pages.home.last_object_date')"/>
      </th>
      <th class="os_center os_nowrap">
        <xsl:value-of select="lang:text('main.pages.home.actions')"/>
      </th>
    </tr>
  </xsl:template>

  <xsl:template name="row-portal">
    <tr>
      <td>
        <xsl:call-template name="portal-box"/>        
      </td>      
      <td class="os_center os_nowrap">
        <xsl:call-template name="portal-type"/>        
      </td>
      <td class="os_center os_nowrap">
        <xsl:choose>
          <xsl:when test="@enableExchange = 'true'">
            <xsl:value-of select="date:userfriendly-datetime(@lastObjectDate)"/>  
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="lang:text('main.pages.home.disabled_exchange')"/>
          </xsl:otherwise>
        </xsl:choose>
      </td>
      <td class="os_center os_nowrap">
        <!--
        <xsl:for-each select="actions/action">          
          <xsl:call-template name="action-link">
            <xsl:with-param name="suffix" select="'main.pages.home'"/>
          </xsl:call-template>
        </xsl:for-each>
        -->

        <xsl:call-template name="actions-popup">
          <xsl:with-param name="suffix" select="'main.pages.home'"/>
          <xsl:with-param name="actions" select="actions"/>
          <xsl:with-param name="default" select="'enter'"/>
        </xsl:call-template>
      </td>
    </tr>
  </xsl:template>
  
</xsl:stylesheet>