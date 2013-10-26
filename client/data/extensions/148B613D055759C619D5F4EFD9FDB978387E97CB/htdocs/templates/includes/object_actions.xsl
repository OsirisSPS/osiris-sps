<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="os date lang system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="objectActions">
    <xsl:if test="pager">
      <div class="os_commands_left">
        <xsl:apply-templates select="pager"/>
      </div>
    </xsl:if>
    <div class="os_commands_right">
      <xsl:for-each select="actions/action">
        <a class="os_button" href="{@href}">
          <xsl:value-of select="lang:text(concat('object.actions.', @name))"/>
        </a>
      </xsl:for-each>
    </div>    
    <div class="os_clear"/>
  </xsl:template>

</xsl:stylesheet>