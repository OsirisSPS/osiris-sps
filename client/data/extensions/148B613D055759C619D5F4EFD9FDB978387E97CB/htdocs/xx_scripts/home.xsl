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
    <table style="clear:both;">
      <tr>
        <td>
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
        </td>
      </tr>
    </table>

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