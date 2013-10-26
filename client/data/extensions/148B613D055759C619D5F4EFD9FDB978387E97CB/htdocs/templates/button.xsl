<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                exclude-result-prefixes="lang"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="controls"/>

  <xsl:template match="button">

    <input id="{@id}" onclick="{@href}; return false;" value="{@caption}" class="os_button">
      <xsl:choose>
        <xsl:when test="@isDefault=true()">
          <xsl:attribute name="type">submit</xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="type">button</xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>      
    </input>
    
  </xsl:template>
  
</xsl:stylesheet>