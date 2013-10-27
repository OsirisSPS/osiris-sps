<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/testsuite">
    <h1>Unicode from XSLT</h1>
    <xsl:value-of select="lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.testsuite.unicode')"/>

    <h1>Unicode from XSLT, template argument</h1>
    <xsl:call-template name="action-row">
      <xsl:with-param name="name" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.testsuite.unicode'"/>
      <xsl:with-param name="href" select="'/developer_tools/testsuite'"/>
    </xsl:call-template>

    <h1>Unicode from XSLT, parse</h1>
    <xsl:value-of select="system:parse(lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.testsuite.unicode'))" disable-output-escaping="yes"/>

    <h1>Unicode from XSLT, parse "row"</h1>
    <xsl:value-of select="system:parse(lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.testsuite.unicode'),false(),false(),true())" disable-output-escaping="yes"/>
  </xsl:template>

  
</xsl:stylesheet>