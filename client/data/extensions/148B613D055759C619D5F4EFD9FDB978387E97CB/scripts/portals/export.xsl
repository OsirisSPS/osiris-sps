<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="stats_table"/>

  <xsl:template match="/stats">
    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('portal.pages.stats.title')}" data-os-otype="dialog" data-os-dialog-width="60%">
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('portal.pages.stats.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="page">
    <div class="os_content">
      <table>
        <tr>
          <td class="os_label">
            Machine ID :
          </td>
          <td class="os_value">
            <xsl:call-template name="hash">
              <xsl:with-param name="hash" select="//@machine_id"/>
            </xsl:call-template>            
          </td>
        </tr>
        <tr>
          <td class="os_label">
            Alignment hash :
          </td>
          <td class="os_value hash">
            <xsl:call-template name="hash">
              <xsl:with-param name="hash" select="//@align_hash"/>
            </xsl:call-template>
          </td>
        </tr>
        <tr>
          <td class="os_label">
            Acceptable hash :
          </td>
          <td class="os_value hash">
            <xsl:call-template name="hash">
              <xsl:with-param name="hash" select="//@acceptable_hash"/>
            </xsl:call-template>
          </td>
        </tr>
        
      </table>

      <xsl:value-of select="$stats_table" disable-output-escaping="yes"/>
    </div>
  </xsl:template>


</xsl:stylesheet>