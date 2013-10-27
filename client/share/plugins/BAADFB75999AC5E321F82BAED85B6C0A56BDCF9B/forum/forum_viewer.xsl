<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/plugins/BAADFB75999AC5E321F82BAED85B6C0A56BDCF9B/forum/common.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="forum">
    <xsl:call-template name="forum"/>
    <xsl:call-template name="forum-legend">
      <xsl:with-param name="forums_legend" select="boolean(forums)"/>
      <xsl:with-param name="discussions_legend" select="boolean(discussions)"/>
    </xsl:call-template>    
  </xsl:template>
  
</xsl:stylesheet>
