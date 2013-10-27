<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:param name="url"/>
  <xsl:param name="showTitle" />
  <xsl:param name="showDescription" />
  <xsl:param name="showImage" />
  <xsl:param name="nItems" />
  <xsl:param name="showItemDescription" />
  <xsl:param name="templateModel" />
  <xsl:param name="templateCustom" />

  <xsl:template match="feed">
    <!--<img style="margin:5px;float:right;" src="extensions/mod_dev/res/feed.png"/>-->
    <table>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('plugins.components.modules.feed.url')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value" style="width:100%">
          <xsl:value-of select="$url" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('plugins.components.modules.feed.template')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$templateModel" disable-output-escaping="yes"/>
        </td>
      </tr>
    </table>
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title">
        <xsl:value-of select="lang:text('plugins.components.modules.feed.templateSpecific')"/>
      </xsl:with-param>
      <xsl:with-param name="startClose" select="'true'"/>
      <xsl:with-param name="content">
        <div class="os_subtitle">
          <xsl:value-of select="lang:text('plugins.components.modules.feed.templateSpecificSubtitle')"/>
        </div>
        <table>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.feed.showTitle')"/>
              <xsl:text>:</xsl:text>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$showTitle" disable-output-escaping="yes"/>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.feed.showDescription')"/>
              <xsl:text>:</xsl:text>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$showDescription" disable-output-escaping="yes"/>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.feed.showImage')"/>
              <xsl:text>:</xsl:text>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$showImage" disable-output-escaping="yes"/>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.feed.nItems')"/>
              <xsl:text>:</xsl:text>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$nItems" disable-output-escaping="yes"/>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text('plugins.components.modules.feed.showItemDescription')"/>
              <xsl:text>:</xsl:text>
            </td>
            <td class="os_value" style="width:100%">
              <xsl:value-of select="$showItemDescription" disable-output-escaping="yes"/>
            </td>
          </tr>
        </table>
        <div>
          <!--
          <xsl:value-of select="@templateModel"/>
          -->

          <xsl:if test="@templateModel != 'custom'">
            <xsl:attribute name="style">display:none;</xsl:attribute>
          </xsl:if>

          <xsl:value-of select="$templateCustom" disable-output-escaping="yes"/>
        </div>
      </xsl:with-param>
      
    </xsl:call-template>
  </xsl:template>
</xsl:stylesheet>
