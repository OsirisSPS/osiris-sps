<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang system"
                version="1.0">
  
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/post_viewer.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/tag_viewer.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="calendar_event">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <div class="os_text">
          <xsl:value-of select="@description"/>
        </div>
        <xsl:apply-templates select="tags"/>
      </xsl:with-param>
    </xsl:call-template>
    
    <xsl:call-template name="objectActions"/>
    <xsl:if test="posts">
      <xsl:apply-templates select="posts"/>
      <xsl:call-template name="objectActions"/>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>