<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:param name="text"/>
  <xsl:param name="style"/>
  
  <xsl:template match="text">
    <xsl:value-of select="$text" disable-output-escaping="yes"/>
    <table style="width:100%">
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('plugins.components.modules.text.style')"/>
          <xsl:text>: </xsl:text>          
        </td>
        <td class="os_value" style="width:100%">
          <xsl:value-of select="$style" disable-output-escaping="yes"/>
        </td>
      </tr>
    </table>
  </xsl:template>
</xsl:stylesheet>
