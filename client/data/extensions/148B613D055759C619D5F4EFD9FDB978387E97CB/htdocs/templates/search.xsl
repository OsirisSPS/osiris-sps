<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">
  
  <xsl:output method="html"/>

  <xsl:param name="search"/>
  
  <xsl:template match="page">
    <xsl:value-of select="$search" disable-output-escaping="yes"/>
</xsl:template>



</xsl:stylesheet>