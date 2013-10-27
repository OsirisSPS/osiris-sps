<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:dc="http://purl.org/dc/elements/1.1/" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                version="1.0">

  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:apply-templates select="//channel"/>
  </xsl:template>

  <xsl:template name="viewDate">
    <xsl:if test="dc:date">
      <xsl:text>[span style="color:gray;"] [small]([date mode="friendly"]</xsl:text>
      <xsl:value-of select="dc:date"/>
      <xsl:text>)[/date])[/small][/span]</xsl:text>
    </xsl:if>
    <xsl:if test="pubDate">
      <xsl:text>[span style="color:gray;"] [small]([date mode="friendly"]</xsl:text>
      <xsl:value-of select="pubDate"/>
      <xsl:text>)[/date])[/small][/span]</xsl:text>
    </xsl:if>
  </xsl:template>

  <xsl:template name="link">
    <xsl:text>[popup mode="tooltip"][?=default]</xsl:text>
    <!--<xsl:apply-templates select="*[local-name()='description']"/>-->
    <xsl:text>[div class="os_content_box" style="max-width:300px;"]</xsl:text>
    <xsl:value-of select="system:parse-reverse(description/text())"/>
    <xsl:text>[/div]</xsl:text>
    <!--<xsl:value-of select="system:parse(*[local-name()='description'])"/>    -->
    <xsl:text>[/?]</xsl:text>
    <xsl:text>[url="</xsl:text>
    <xsl:apply-templates select="*[local-name()='link']" />
    <xsl:text>"]</xsl:text>
    <xsl:value-of select="system:parse-reverse(*[local-name()='title'])"/>
    <xsl:text>[/url]</xsl:text>

    <xsl:call-template name="viewDate"/>

    <xsl:text>[/popup]</xsl:text>
  </xsl:template>
  
  <xsl:template match="*[local-name()='channel']">    
    <xsl:text>[table class="os_table"][tr][th]</xsl:text>
    <xsl:if test="image">
      <xsl:if test="/rss/@showImage = 'true'">
        <xsl:text>[div style="float:right"][img]</xsl:text>
        <xsl:value-of select="image/url"/>
        <xsl:text>[/img][/div]</xsl:text>
      </xsl:if>
    </xsl:if>
    <xsl:text>[h3]</xsl:text>
    <xsl:call-template name="link"/>
    <xsl:text>[/h3]</xsl:text>
    <xsl:text>[/th][/tr]</xsl:text>
    <xsl:apply-templates select="//*[local-name()='item']" />
    <xsl:text>[/table]</xsl:text>
  </xsl:template>

  <xsl:template match="*[local-name()='item']">
    <xsl:text>[tr]</xsl:text>
    <xsl:choose>
      <xsl:when test="position() mod 2 = 0">
        <xsl:text>[td class="os_row_light"]</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>[td class="os_row_dark"]</xsl:text>
      </xsl:otherwise>
    </xsl:choose>
    <xsl:call-template name="link"/>
    <xsl:text>[/td][/tr]</xsl:text>
  </xsl:template>
  
</xsl:stylesheet>
