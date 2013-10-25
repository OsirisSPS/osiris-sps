<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:param name="style"/>
  <xsl:param name="cultures"/>
  
  <xsl:template match="data">
    <!--<img style="margin:5px;float:right;" src="extensions/mod_dev/res/languageswitcher.png"/>-->
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
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('plugins.components.modules.languageswitcher.cultures')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value" style="width:100%">
          <xsl:value-of select="$cultures" disable-output-escaping="yes"/>
        </td>
      </tr>
    </table>
  </xsl:template>
</xsl:stylesheet>
