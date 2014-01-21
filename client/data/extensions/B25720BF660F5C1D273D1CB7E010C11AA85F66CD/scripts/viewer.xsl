<!-- getRSS.xsl: retrieve RSS feed(s) and convert to HTML. -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:dc="http://purl.org/dc/elements/1.1/" version="1.0">

  <xsl:output method="html"/>
  
  <xsl:template match="flash_clocks_com">
    <embed src="http://flash-clocks.com/free-flash-clocks-blog-topics/free-flash-clock-{@number}.swf" width="{@width}" height="{@height}" wmode="transparent" type="application/x-shockwave-flash">
    </embed>
  </xsl:template>

</xsl:stylesheet>
