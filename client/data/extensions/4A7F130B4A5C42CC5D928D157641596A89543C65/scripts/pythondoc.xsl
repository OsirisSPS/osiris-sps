<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.pythondoc.title')"/>
      <xsl:with-param name="content">
        <iframe id="frame_doc" style="width:100%" src="/developer_tools_pythondoc/index.html"></iframe>
        <script type="text/javascript">
          <xsl:text>
            osGetById("frame_doc").height = document.documentElement.clientHeight-180;
          </xsl:text>
        </script>
      </xsl:with-param>
    </xsl:call-template>
    
  </xsl:template>

  
</xsl:stylesheet>