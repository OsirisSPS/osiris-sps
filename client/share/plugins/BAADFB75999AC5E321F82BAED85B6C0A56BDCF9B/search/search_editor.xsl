<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:param name="globalParams"/>
  <xsl:param name="showParams"/>
  <xsl:param name="directRun"/>
  <xsl:param name="allowRss"/>
  <xsl:param name="rssDescription"/>
  <xsl:param name="rssBody"/>
  
  <xsl:template match="search">
    <!--<img style="margin:5px;float:right;" src="extensions/mod_dev/res/search.png"/>-->
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title">
        <xsl:value-of select="lang:text('plugins.components.modules.search.global_params')"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:value-of select="$globalParams" disable-output-escaping="yes"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title">
        <xsl:value-of select="lang:text('plugins.components.modules.search.other_params')"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <table>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.search.showParams')"/>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$showParams" disable-output-escaping="yes"/>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.search.directRun')"/>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$directRun" disable-output-escaping="yes"/>
            </td>
          </tr>          
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.search.allowRss')"/>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$allowRss" disable-output-escaping="yes"/>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.search.rssDescription')"/>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$rssDescription" disable-output-escaping="yes"/>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.search.rssBody')"/>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$rssBody" disable-output-escaping="yes"/>
            </td>
          </tr>
        </table>
      </xsl:with-param>
    </xsl:call-template>
    
  </xsl:template>
</xsl:stylesheet>
