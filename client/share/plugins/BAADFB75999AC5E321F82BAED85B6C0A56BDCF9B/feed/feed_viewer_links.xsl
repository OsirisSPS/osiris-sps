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
      <xsl:text>[div style="float:right;color:gray;"] [small]([date mode="short_date"]</xsl:text>
      <xsl:value-of select="dc:date"/>
      <xsl:text>)[/date])[/small][/div]</xsl:text>
    </xsl:if>
    <xsl:if test="pubDate">
      <xsl:text>[div style="float:right;color:gray;"] [small]([date mode="short_date"]</xsl:text>
      <xsl:value-of select="pubDate"/>
      <xsl:text>)[/date])[/small][/div]</xsl:text>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="link">
    <xsl:text>[url="</xsl:text>
    <xsl:apply-templates select="*[local-name()='link']" />
    <xsl:text>"]</xsl:text>
    <xsl:value-of select="system:parse-reverse(*[local-name()='title'])"/>
    <xsl:text>[/url]</xsl:text>
  </xsl:template>

  <xsl:template match="*[local-name()='channel']">
    <xsl:text>[table style="width:100%"]</xsl:text>
    <xsl:apply-templates select="//*[local-name()='item']" />
    <xsl:text>[/table]</xsl:text>
  </xsl:template>

  <xsl:template match="*[local-name()='item']">
    <xsl:text>[tr]</xsl:text>
    <xsl:choose>
      <xsl:when test="position() mod 2 = 0">
        <xsl:text>[td class="os_row_light" style="padding:2px"]</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:text>[td class="os_row_dark" style="padding:2px"]</xsl:text>
      </xsl:otherwise>
    </xsl:choose>    
    <xsl:call-template name="link"/>
    <xsl:call-template name="viewDate"/>    
    <xsl:text>[/td][/tr]</xsl:text>
  </xsl:template>


</xsl:stylesheet>
