<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/help">

    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div data-os-otype="dialog" data-os-dialog-width="50%" >
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.network.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
  

  <xsl:template name="page">
    <div class="os_content">
      <div style="float:right">
        <table class="os_table_properties">
          <tr>
            <td>Status</td>
            <td></td>
          </tr>
          <tr>
            <td>External IP / Port</td>
            <td>
              <xsl:value-of select="@ip"/>
              <xsl:text> : </xsl:text>
              <xsl:value-of select="@port"/>
            </td>
          </tr>
          <tr>
            <td>Geolocation</td>
            <td>1.2.3.4 : 1233</td>
          </tr>
          <tr>
            <td>Last update</td>
            <td>
              <xsl:value-of select="date:userfriendly-datetime(@last_request)" />
            </td>
          </tr>          
        </table>
      </div>      
      <div style="overflow:auto">

        <xsl:if test="@last_error != ''">
          <div class="box-error">
            <xsl:value-of select="@last_error"/>
          </div>
        </xsl:if>
        
        <div class="box-info">
          Ok.
        </div>
        <div class="box-warning">
          P2P not active.
        </div>
        <div class="box-warning">
          Isis not active.
        </div>
        <div class="box-warning">
          Not reachable from outside. Are you under NAT, or you don't configure your router/firewall?
        </div>
        <div class="box-error">
          Isis not reachable. Internet connection problems?
        </div>
      </div>      
      
    </div>    
  </xsl:template>

  
</xsl:stylesheet>