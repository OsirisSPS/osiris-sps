<!-- getRSS.xsl: retrieve RSS feed(s) and convert to HTML. -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:dc="http://purl.org/dc/elements/1.1/" 
                exclude-result-prefixes="dc"
                version="1.0">

  <xsl:output method="html"/>
  
  <xsl:template match="custom_css">
    <style type="text/css" media="all">
      <xsl:value-of select="@dump" disable-output-escaping="yes"/>
    </style>
  </xsl:template>

</xsl:stylesheet>
