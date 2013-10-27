<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"                 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"                 
                exclude-result-prefixes="os lang"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="viewer"/>

  <xsl:template match="instance">
    <!-- The "position:relative;top:0px;left:0px;" style, allow users to define a "position:absolute" style for the istance, with relative coordinates and not window-absolute coordinated.
    As example, are used for istances "Quick Search" & "Language Switcher" in Osiris portal.
    -->
    <div style="position:relative;top:0px;left:0px;">
      <div data-os-entity="{@entity}" style="{@style}">
        <xsl:value-of select="$viewer" disable-output-escaping="yes"/>
      </div>
    </div>
  </xsl:template>
</xsl:stylesheet>