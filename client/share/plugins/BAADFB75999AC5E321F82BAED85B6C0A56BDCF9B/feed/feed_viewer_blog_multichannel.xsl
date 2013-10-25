<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:dc="http://purl.org/dc/elements/1.1/" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                version="1.0">

  <xsl:output method="text"/>

  <xsl:template match="/">
    <xsl:choose>
      <xsl:when test="/rss/@collapseItems = 'false'">
        <xsl:apply-templates select="//channel"/>
      </xsl:when>
      <xsl:otherwise>        
        <xsl:apply-templates select="//channel/item">
          <xsl:sort select="pubDate" data-type="dc:date" order="descending"/>
        </xsl:apply-templates>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="viewDate">
    <xsl:text>[date mode="friendly"]</xsl:text>
    <xsl:if test="dc:date">      
      <xsl:value-of select="dc:date"/>      
    </xsl:if>
    <xsl:if test="pubDate">
      <xsl:value-of select="pubDate"/>      
    </xsl:if>
    <xsl:text>)[/date]</xsl:text>
  </xsl:template>

  <xsl:template name="link">    
    <xsl:text>[h4][url="</xsl:text>
    <xsl:apply-templates select="*[local-name()='link']" />
    <xsl:text>"]</xsl:text>
    <xsl:value-of select="system:parse-reverse(*[local-name()='title'])"/>    
    <xsl:text>[/url][/h4][div style="text-align:right;border-top:1px solid gray;margin:2px;"]</xsl:text>
    
    <xsl:text>[/div]</xsl:text>
    <!--
    <xsl:text>[noparse]</xsl:text>
    <xsl:value-of select="system:parse-reverse(description/text())"/>
    <xsl:text>[/noparse]</xsl:text>
    -->
    <xsl:value-of select="description/text()" disable-output-escaping="yes"/>
  </xsl:template>

  <xsl:template match="*[local-name()='channel']">
    <xsl:text>[center][h3]</xsl:text>
    <xsl:if test="image">
      <xsl:if test="/rss/@showImage = 'true'">
        <xsl:text>[div style="float:right"][img style="max-height:32px"]</xsl:text>
        <xsl:value-of select="image/url"/>
        <xsl:text>[/img][/div]</xsl:text>
      </xsl:if>
    </xsl:if>
    <xsl:call-template name="link"/>
    <xsl:text>[/h3][/center]</xsl:text>

    <xsl:apply-templates select="//*[local-name()='item']" />    
  </xsl:template>

  <xsl:template match="*[local-name()='item']">
    <xsl:text>[div class="os_content_box"]</xsl:text>

    <xsl:if test="/rss/@showImage = 'true'">
      <xsl:if test="../image/url">
        <xsl:text>[div style="float:right"][img style="max-height:32px"]</xsl:text>
        <xsl:value-of select="../image/url"/>
        <xsl:text>[/img][/div]</xsl:text>
      </xsl:if>
    </xsl:if>

    <xsl:text>[h4][url="</xsl:text>
    <xsl:apply-templates select="*[local-name()='link']" />
    <xsl:text>"]</xsl:text>
    <xsl:value-of select="system:parse-reverse(*[local-name()='title'])"/>
    <xsl:text>[/url][/h4]</xsl:text>

    <xsl:text>[div class="os_subtitle"]</xsl:text>
    <xsl:text>From: [b]</xsl:text>
    <xsl:value-of select="system:parse-reverse(../title)"/>
    <xsl:text>[/b]</xsl:text>
    <xsl:if test="../description != ../title">
      <xsl:text>[color="gray"], </xsl:text>
      <xsl:value-of select="system:parse-reverse(../description)"/>
      <xsl:text>[/color]</xsl:text>
    </xsl:if>
    <xsl:text> , [b]</xsl:text>
    <xsl:call-template name="viewDate"/>
    <xsl:text>[/b]</xsl:text>        
    <xsl:text>[/div]</xsl:text>

    <xsl:text>[subparse]</xsl:text>
    <xsl:value-of select="system:parse-reverse(description/text())"/>
    <xsl:text>[/subparse]</xsl:text>
    
    <xsl:text>[/div]</xsl:text>
  </xsl:template>
  
</xsl:stylesheet>
