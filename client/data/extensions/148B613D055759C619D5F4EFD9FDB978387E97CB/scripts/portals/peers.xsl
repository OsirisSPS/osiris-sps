<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="addPeerIP"/>
  <xsl:param name="addPeerPort"/>
  <xsl:param name="addPeerCommand"/>

	<!--
  <xsl:template match="/check">
    <table>    
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('portal.pages.peers.ip')"/>
          <xsl:text> :</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="@ip"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('portal.pages.peers.port')"/>
          <xsl:text> :</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="@port"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('portal.pages.peers.super')"/>
          <xsl:call-template name="help-tooltip">
            <xsl:with-param name="text" select="lang:text('portal.pages.peers.test.help')"/>
          </xsl:call-template>
          <xsl:text> :</xsl:text>
        </td>
        <td class="os_value">
          <xsl:choose>
            <xsl:when test="@p2p_enabled = 'true'">
              <xsl:call-template name="boolean-icon">
                <xsl:with-param name="value" select="@result!='false'"/>
              </xsl:call-template>
              <xsl:text> </xsl:text>
              <xsl:value-of select="@message"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="lang:text('ui.application.port_check_without_p2p')"/>
            </xsl:otherwise>
          </xsl:choose>
          
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('portal.pages.peers.tor')"/>
          <xsl:call-template name="help-tooltip">
            <xsl:with-param name="text" select="lang:text('portal.pages.peers.tor.help')"/>
          </xsl:call-template>

          <xsl:text> :</xsl:text>
        </td>
        <td class="os_value">
          <xsl:call-template name="status-icon">
            <xsl:with-param name="value" select="@tor"/>
          </xsl:call-template>          
        </td>
      </tr>
    </table>
  </xsl:template>
	-->

  <xsl:template match="/home">    
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('portal.pages.peers.title')"/>
      <xsl:with-param name="content">

        <div style="float:right;margin:10px;width:300px;">
          <xsl:call-template name="block_small">
            <xsl:with-param name="title">
              <xsl:value-of select="lang:text('portal.pages.peers.add.title')"/>
            </xsl:with-param>
            <xsl:with-param name="content">
              <table>
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('portal.pages.peers.ip')"/>
                    <xsl:text> :</xsl:text>
                  </td>
                  <td class="os_value">
                    <xsl:value-of select="$addPeerIP" disable-output-escaping="yes"/>
                  </td>
                </tr>
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('portal.pages.peers.port')"/>
                    <xsl:text> :</xsl:text>
                  </td>
                  <td class="os_value">
                    <xsl:value-of select="$addPeerPort" disable-output-escaping="yes"/>
                  </td>
                </tr>
                <tr>
                  <td colspan="2" class="os_commands">
                    <xsl:value-of select="$addPeerCommand" disable-output-escaping="yes"/>
                  </td>
                </tr>
              </table>

            </xsl:with-param>
          </xsl:call-template>
        </div>
        
				<!--
        <div style="float:right;margin:10px;width:300px;">
          <xsl:call-template name="block_small">
            <xsl:with-param name="title">
              <xsl:value-of select="lang:text('portal.pages.peers.your.title')"/>
            </xsl:with-param>
            <xsl:with-param name="content">							
              <div data-os-url="{@page_url}&amp;act=your" data-os-wait="center"></div>              
            </xsl:with-param>
          </xsl:call-template>
        </div>
				-->

        

        <div style="overflow:auto">
          <xsl:call-template name="help-box">
            <xsl:with-param name="text" select="lang:text('portal.pages.peers.help')"/>
          </xsl:call-template>

          <table>
            <tr>
              <td class="os_label">
                <xsl:value-of select="lang:text('portal.pages.peers.stats.peers')"/>
                <xsl:text> :</xsl:text>
              </td>
              <td class="os_value">
                <xsl:value-of select="@peers"/>
              </td>
            </tr>
            <tr>
              <td class="os_label">
                <xsl:value-of select="lang:text('portal.pages.peers.stats.active')"/>
                <xsl:text> :</xsl:text>
              </td>
              <td class="os_value">
                <xsl:value-of select="@active"/>
              </td>
            </tr>
            <tr>
              <td class="os_label">
                <xsl:value-of select="lang:text('portal.pages.peers.stats.super')"/>
                <xsl:text> :</xsl:text>
              </td>
              <td class="os_value">
                <xsl:value-of select="@super"/>
              </td>
            </tr>
          </table>
          
        </div>

        <table class="os_table_data">
          <thead>
            <tr>
              <th>
                <xsl:value-of select="lang:text('portal.pages.peers.ip')"/>
              </th>
              <th>
                <xsl:value-of select="lang:text('portal.pages.peers.port')"/>
              </th>
              <th>
                <xsl:value-of select="lang:text('portal.pages.peers.origin')"/>
              </th>
              <th>
                <xsl:value-of select="lang:text('portal.pages.peers.last_contact')"/>
              </th>
              <th>
                <xsl:value-of select="lang:text('portal.pages.peers.active')"/>
              </th>
              <th>
                <xsl:value-of select="lang:text('portal.pages.peers.super')"/>
              </th>
              <th>
                <xsl:value-of select="lang:text('portal.pages.peers.retries')"/>
              </th>
            </tr>
          </thead>
          <tbody>
            <xsl:for-each select="peer">
              <xsl:sort select="@last_contact" order="descending"/>
              <tr>
                <td>
                  <xsl:value-of select="@ip"/>
                </td>
                <td>
                  <xsl:value-of select="@port"/>
                </td>
                <td>
                  <xsl:value-of select="lang:text(concat('ui.mainframe.connections.origin.',@origin))"/>
                </td>
                <td>
                  <xsl:value-of select="date:userfriendly-datetime(@last_contact)"/>
                </td>
                <td>
                  <xsl:call-template name="boolean-icon">
                    <xsl:with-param name="value" select="@active"/>
                  </xsl:call-template>
                </td>
                <td>
                  <xsl:call-template name="boolean-icon">
                    <xsl:with-param name="value" select="@super"/>
                  </xsl:call-template>
                </td>
                <td>
                  <xsl:value-of select="@retries"/>
                </td>
              </tr>
            </xsl:for-each>
          </tbody>
        </table>
        
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  
</xsl:stylesheet>