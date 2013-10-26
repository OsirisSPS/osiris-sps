<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="tag">
    <xsl:apply-templates select="pager" mode="row"/>    
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@name"/>
      <xsl:with-param name="href" select="@edit_href"/>
      <xsl:with-param name="content">
        <xsl:for-each select="objects/object">
          <xsl:apply-templates select="." mode="lite"/>
        </xsl:for-each>
      </xsl:with-param>
    </xsl:call-template>
    <xsl:apply-templates select="pager" mode="row"/>
  </xsl:template>

  <xsl:template match="tags">
    <xsl:param name="show_separator" select="true()"/>
    <xsl:if test="$show_separator">
      <div class="os_separator"/>
    </xsl:if>
    <div class="os_tags">
      <span class="os_label">
        <xsl:value-of select="lang:text('object.tags.title')"/>:
      </span>
      <span class="os_value">
        <xsl:for-each select="tag">
          <xsl:if test="position() != 1">
            <xsl:text> </xsl:text>
          </xsl:if>
          <a href="{@view_href}">
            <xsl:value-of select="@name"/>
          </a>
        </xsl:for-each>
      </span>
    </div>
  </xsl:template>

</xsl:stylesheet>