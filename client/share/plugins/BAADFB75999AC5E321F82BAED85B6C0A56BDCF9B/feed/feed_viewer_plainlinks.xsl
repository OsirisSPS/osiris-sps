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
    <xsl:text>[url="</xsl:text>
    <xsl:apply-templates select="*[local-name()='link']" />
    <xsl:text>"]</xsl:text>
    <xsl:value-of select="system:parse-reverse(*[local-name()='title'])"/>
    <xsl:text>[/url]</xsl:text>
  </xsl:template>

  <xsl:template match="*[local-name()='channel']">    
    <xsl:apply-templates select="//*[local-name()='item']" />
  </xsl:template>

  <xsl:template match="*[local-name()='item']">
    <xsl:call-template name="link"/>
    <xsl:call-template name="viewDate"/>
    <xsl:text>[br /]</xsl:text>
  </xsl:template>


</xsl:stylesheet>
