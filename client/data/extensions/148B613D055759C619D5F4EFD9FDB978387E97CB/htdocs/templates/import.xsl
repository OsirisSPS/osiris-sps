<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="fileBrowser"/>
  <xsl:param name="portalPassword"/>
  <xsl:param name="cmdImport"/>
  <xsl:param name="cmdCancel"/>
  
  <xsl:template match="page">
		<div style="text-align:center">
			<div style="width:700px;margin:auto;">
				<xsl:call-template name="block_big">
					<xsl:with-param name="title">
						<xsl:value-of select="lang:text('main.pages.import.title')"/>
					</xsl:with-param>
					<xsl:with-param name="content">
						<xsl:call-template name="help-box">
							<xsl:with-param name="text" select="lang:text('main.pages.import.subtitle')"/>
						</xsl:call-template>
						<xsl:call-template name="importTable"/>
					</xsl:with-param>
				</xsl:call-template>
			</div>
		</div>
  </xsl:template>

  <xsl:template name="importTable">    
    <table class="os_table_properties">
      <tr>
        <td>
          <xsl:value-of select="lang:text('main.pages.import.filename')"/>
					<xsl:text>: </xsl:text>
        </td>
        <td>
          <xsl:value-of select="$fileBrowser" disable-output-escaping="yes"/>  
        </td>
      </tr>
      <tr>
        <td>
          <xsl:value-of select="lang:text('main.pages.import.password')"/>
					<xsl:text>: </xsl:text>
        </td>
        <td>
          <xsl:value-of select="$portalPassword" disable-output-escaping="yes"/>					
        </td>
      </tr>			
		</table>
		<div class="os_commands">
			<xsl:value-of select="$cmdImport" disable-output-escaping="yes"/>
			<xsl:value-of select="$cmdCancel" disable-output-escaping="yes"/>
		</div>
    
  </xsl:template>
  
</xsl:stylesheet>
