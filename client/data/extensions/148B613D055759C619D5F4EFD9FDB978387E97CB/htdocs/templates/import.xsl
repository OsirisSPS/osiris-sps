<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="fileBrowser"/>
  <xsl:param name="portalPassword"/>
  <xsl:param name="cmdImport"/>
  <xsl:param name="cmdCancel"/>
  
  <xsl:template match="page">
    <xsl:call-template name="block_big">
      <xsl:with-param name="title">
        <xsl:value-of select="lang:text('main.pages.import.title')"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:call-template name="importTable"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="importTable">    
    <table class="os_table_data os_content_box">
      <tr>
        <th class="os_subtitle" colspan="2">
          <xsl:value-of select="lang:text('main.pages.import.subtitle')"/>          
        </th>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('main.pages.import.filename')"/>          
        </td>
        <td class="os_value">
          <xsl:value-of select="$fileBrowser" disable-output-escaping="yes"/>  
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('main.pages.import.password')"/>          
        </td>
        <td class="os_value">
          <xsl:value-of select="$portalPassword" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_commands" colspan="2">
          <xsl:value-of select="$cmdImport" disable-output-escaping="yes"/>
          <xsl:value-of select="$cmdCancel" disable-output-escaping="yes"/>
        </td>        
      </tr>
    </table>
    
  </xsl:template>
  
</xsl:stylesheet>
