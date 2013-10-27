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

  <xsl:template match="/info">
    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('portal.pages.info.title')}" data-os-otype="dialog" data-os-dialog-width="60%">
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block_page">
          <xsl:with-param name="prefix" select="'portal.pages.info'"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="page">    
    <div style="float:right" class="os_content">
      <p>
        <span class="os_title" style="font-size:1.3em" href="{@href}">
          <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes" />
        </span>
      </p>
      <p>
        <span class="os_description">
          <xsl:value-of select="system:parse(@description, false(), false(), true())" disable-output-escaping="yes" />
        </span>
      </p>
      <xsl:value-of select="$stats_table" disable-output-escaping="yes"/>

      <xsl:call-template name="actions-row">
        <xsl:with-param name="prefix" select="'~portal.pages'"/>
      </xsl:call-template>
    </div>
    <div style="clear:left;">
      <table class="os_table_properties">
        <tr>
          <td>
            Align :
          </td>
          <td>
            <xsl:value-of select="@nodesSameAlign"/>
            <xsl:text> / </xsl:text>
            <xsl:value-of select="@nodesSamePortalPov"/>            
          </td>
        </tr>
        <tr>
          <td>
            Machine ID :
          </td>
          <td class="hash">
            <xsl:call-template name="hash">
              <xsl:with-param name="hash" select="//@machine_id"/>
            </xsl:call-template>
          </td>
        </tr>
        <tr>
          <td>
            Alignment hash :
          </td>
          <td class="hash">
            <xsl:call-template name="hash">
              <xsl:with-param name="hash" select="//@align_hash"/>
            </xsl:call-template>
          </td>
        </tr>
        <tr>
          <td>
            Acceptable hash :
          </td>
          <td class="hash">
            <xsl:call-template name="hash">
              <xsl:with-param name="hash" select="//@acceptable_hash"/>
            </xsl:call-template>
          </td>
        </tr>
        <tr>
          <td>
            Last optimization date :
          </td>
          <td>
            <xsl:value-of select="date:userfriendly-datetime(@lastOptimizationDate)" />            
          </td>
        </tr>
        <tr>
          <td>
            Last object date :
          </td>
          <td>
            <xsl:value-of select="date:userfriendly-datetime(@lastObjectDate)" />
          </td>
        </tr>
        <tr>
          <td>
            Last exchange date :
          </td>
          <td>
            <xsl:value-of select="date:userfriendly-datetime(@lastExchangeDate)" />            
          </td>
        </tr>
        <tr>
          <td>
            Last downloaded object date :
          </td>
          <td>
            <xsl:value-of select="date:userfriendly-datetime(@lastDownloadedObjectDate)" />            
          </td>
        </tr>
        <tr>
          <td>
            Last uploaded object date :
          </td>
          <td>
            <xsl:value-of select="date:userfriendly-datetime(@lastUploadedObjectDate)" />
          </td>
        </tr>

      </table>
    </div>
    <div style="overflow:auto;">
      
    </div>    
  </xsl:template>


</xsl:stylesheet>