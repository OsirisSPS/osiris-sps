<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>

<xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="isis">
    <xsl:call-template name="block_page">
      <xsl:with-param name="prefix" select="'main.pages.isis'"/>
      <xsl:with-param name="content">
        
        <xsl:choose>
          <xsl:when test="portals/portal/isisEndpoints/isisEndpoint">
            <table class="os_table_data">
              <tr>
                <th>
                  <xsl:value-of select="lang:text('portal.name')"/>
                </th>
                <th>
                </th>
                <th>
                  <xsl:value-of select="lang:text('main.pages.settings.isis.name')"/>
                </th>
                <th>
                  <xsl:value-of select="lang:text('main.pages.settings.isis.url')"/>
                </th>
                <th>
                  <xsl:value-of select="lang:text('main.pages.settings.isis.last_event')"/>
                </th>
                <th>
                  <xsl:value-of select="lang:text('main.pages.settings.isis.actions')"/>
                </th>
              </tr>
              <xsl:for-each select="portals/portal/isisEndpoints/isisEndpoint">
                <xsl:sort select="../../@name" order="ascending"/>
                <xsl:sort select="@name" order="ascending"/>
                <xsl:call-template name="isisEndpoint"/>
              </xsl:for-each>
            </table>
          </xsl:when>
          <xsl:otherwise>
            <div class="os_message_nodata">
              <xsl:value-of select="lang:text('main.pages.isis.no_isis')"/>
            </div>
          </xsl:otherwise>
        </xsl:choose>
        
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="portal">
    <xsl:call-template name="portal-box"/>
  </xsl:template>

  <xsl:template name="isisEndpoint">
    <tr>
      <td>
        <xsl:for-each select="../..">
          <xsl:call-template name="portal-box"/>
        </xsl:for-each>
      </td>
      <td>
        <xsl:call-template name="active">
          <xsl:with-param name="active" select="@enabled"/>
        </xsl:call-template>
      </td>
      <td>        
        <xsl:value-of select="@name"/>
      </td>
      <td>
        <xsl:value-of select="@url"/>
      </td>
      <td>
        <xsl:value-of select="@lastEvent"/>
      </td>      
      <td>        
        <xsl:call-template name="actions-popup">
          <xsl:with-param name="prefix" select="'common.actions'"/>
          <xsl:with-param name="actions" select="actions"/>
          <xsl:with-param name="default" select="'edit'"/>
        </xsl:call-template>
      </td>
    </tr>
  </xsl:template>
  
</xsl:stylesheet>