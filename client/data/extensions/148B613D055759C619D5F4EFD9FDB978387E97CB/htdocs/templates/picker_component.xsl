<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="root">

    <xsl:apply-templates select="component"/>
    
  </xsl:template>

  <xsl:template match="component">
    <a id="{@group}_{@key}" href="{@href}">
      <xsl:value-of select="system:parse(@name, false(), false(), true())"/>
    </a>
  </xsl:template>
  
</xsl:stylesheet>