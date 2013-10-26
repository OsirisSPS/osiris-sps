<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">
  
  <xsl:output method="html"/>

  <xsl:param name="css"/>
  
  <xsl:template match="custom_css">    
    <table style="width:100%">
      <tr>
        <td class="os_label">
          <xsl:text>CSS</xsl:text>
         </td>
        <td class="os_value" style="width:100%">
          <xsl:value-of select="$css" disable-output-escaping="yes"/>
        </td>
      </tr>      
    </table>
  </xsl:template>

  
</xsl:stylesheet>
