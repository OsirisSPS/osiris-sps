<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="data">
    <div class="os_content" style="text-align:center;">
      <xsl:apply-templates select="cultures/culture"/>  
    </div>
  </xsl:template>

  <xsl:template match="culture">
    <span style="padding:2px">
      <xsl:choose>
        <xsl:when test="@selected = 'true'">
          <span>
            <xsl:call-template name="culture-obj"/>
          </span>
        </xsl:when>
        <xsl:otherwise>
          <a href="{@href}" class="os_alpha_highlight">
            <xsl:call-template name="culture-obj"/>
          </a>
        </xsl:otherwise>
      </xsl:choose>
    </span>
  </xsl:template>

  <xsl:template name="culture-obj">
    <xsl:choose>
      <xsl:when test="//@style='full'">
        <img style="vertical-align:middle;margin:3px;" src="{system:resource-url(concat('images/flags/small/',@id,'.png'))}" data-os-tooltip="{concat('&lt;b&gt;',@native,'&lt;/b&gt;&lt;br /&gt;',@english)}"/>
        <xsl:value-of select="@native"/>
      </xsl:when>
      <xsl:otherwise>
        <img src="{system:resource-url(concat('images/flags/small/',@id,'.png'))}" data-os-tooltip="{concat('&lt;b&gt;',@native,'&lt;/b&gt;&lt;br /&gt;',@english)}"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

</xsl:stylesheet>
