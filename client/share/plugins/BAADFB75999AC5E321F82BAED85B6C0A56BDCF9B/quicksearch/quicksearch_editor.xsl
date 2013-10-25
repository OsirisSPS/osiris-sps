<!-- getRSS.xsl: retrieve RSS feed(s) and convert to HTML. -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:param name="style"/>

  <xsl:template match="quicksearch">
    <!--
    <xsl:call-template name="help-box">
      <xsl:with-param name="text">
        <xsl:value-of select="lang:text('plugins.components.modules.parameters_not_need')"/>
      </xsl:with-param>
    </xsl:call-template>
    -->
      <table>
        <tr>
          <td class="os_label">
            <xsl:value-of select="lang:text('plugins.components.modules.languageswitcher.style')"/>
            <xsl:text>:</xsl:text>
          </td>
          <td class="os_value" style="width:100%">            
            <xsl:value-of select="$style" disable-output-escaping="yes"/>            
          </td>
        </tr>        
      </table>
  </xsl:template>

</xsl:stylesheet>
