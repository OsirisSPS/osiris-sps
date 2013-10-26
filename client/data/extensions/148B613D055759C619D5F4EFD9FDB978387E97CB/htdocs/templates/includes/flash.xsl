<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:template name="flash">
    <xsl:param name="filename"/>
    <xsl:param name="width"/>
    <xsl:param name="height"/>

    <object style="width:{$width}px;height:{$height}px;" type="application/x-shockwave-flash" data="{$filename}">
      <param name="movie\" value="{$filename}" />
      <param name="WMode\" value="Transparent" />
    </object>
  </xsl:template>

</xsl:stylesheet>