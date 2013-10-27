<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes=" date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>

  <xsl:output method="html" indent="no" omit-xml-declaration="yes"/>

  <xsl:template match="user">
    <xsl:choose>
      <xsl:when test="@mode = 'row'">        
        <xsl:for-each select="user">
          <xsl:call-template name="user-row" />
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="@mode = 'lite'">
        <xsl:for-each select="user">
          <!-- DA CHIAMARE IL TEMPLATE user-lite -->
          <xsl:call-template name="user-row" />
        </xsl:for-each>
      </xsl:when>
      <xsl:when test="@mode = 'full'">
        <xsl:for-each select="user">
          <!-- DA CHIAMARE IL TEMPLATE user-full -->
          <xsl:call-template name="user-row" />
        </xsl:for-each>
      </xsl:when>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>