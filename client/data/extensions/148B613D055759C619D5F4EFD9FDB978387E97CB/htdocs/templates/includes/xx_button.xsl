<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="os"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="button-big">
    <!--Titolo del blocco-->
    <xsl:param name="title" />
    <!--Link opzionale-->
    <xsl:param name="href" />
    
    <a class="os_button_big100" href="{$href}">
      <xsl:value-of select="$title"/>
    </a>

  </xsl:template>  
  
</xsl:stylesheet>