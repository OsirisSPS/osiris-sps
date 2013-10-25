<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xsl/os" 
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:param name="xml"/>
  <xsl:param name="xsl"/>
  <xsl:param name="xsd"/>

  <xsl:template match="/">
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title">
        <xsl:value-of select="'XML - Extensible Markup Language'"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:value-of select="$xml" disable-output-escaping="yes"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title">
        <xsl:value-of select="'XSLT - Extensible Stylesheet Language Transformations'"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:value-of select="$xsl" disable-output-escaping="yes"/>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title">
        <xsl:value-of select="'XSD - XML Schema Definition'"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:value-of select="$xsd" disable-output-escaping="yes"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

</xsl:stylesheet>
