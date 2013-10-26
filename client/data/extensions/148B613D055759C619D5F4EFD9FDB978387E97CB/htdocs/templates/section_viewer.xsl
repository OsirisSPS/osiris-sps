<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"              
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="os lang system"
                version="1.0">

  <!--
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/file_viewer.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/xx_forum_viewer.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/text_viewer.xsl"/>
  -->

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <!--
  <xsl:param name="mode"/>
  -->


  <xsl:template match="/">
    <!--
    <xsl:choose>
      <xsl:when test="$mode = 'blog'">
        <xsl:apply-templates select="section" mode="blog"/>
      </xsl:when>
      <xsl:when test="$mode = 'forum'">
        <xsl:apply-templates select="section" mode="forum"/>
        <xsl:call-template name="forum-legend">
          <xsl:with-param name="topics_legend" select="false()"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:when test="$mode = 'repository'">
        <xsl:apply-templates select="section" mode="repository"/>
      </xsl:when>
    </xsl:choose>    
    -->
  </xsl:template>  

  <!--
  <xsl:template match="section" mode="blog">    
    <xsl:call-template name="objectActions"/>    
    <xsl:if test="texts">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="@title"/>
        <xsl:with-param name="content">
          <xsl:for-each select="texts/text">
            <xsl:apply-templates select="." mode="blog">
              <xsl:with-param name="show_separator" select="position() != last()"/>
            </xsl:apply-templates>
          </xsl:for-each>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="objectActions"/>
    </xsl:if>
  </xsl:template>
  -->

  <!--
  <xsl:template match="section" mode="forum">    
    <xsl:if test="forums">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="@title"/>
        <xsl:with-param name="href" select="@view_href"/>
        <xsl:with-param name="content">
          <xsl:apply-templates select="forums"/>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    <xsl:if test="descendant::sections/*/forums">
      <xsl:apply-templates select="sections" mode="forum"/>
    </xsl:if>
  </xsl:template>
  -->

  <!--
  <xsl:template match="section" mode="repository">
    <xsl:call-template name="objectActions"/>
    <xsl:if test="files">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="@title"/>
        <xsl:with-param name="content">
          <xsl:for-each select="files/file">
            <xsl:apply-templates select="." mode="repository">
              <xsl:with-param name="show_separator" select="position() != last()"/>
            </xsl:apply-templates>
          </xsl:for-each>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="objectActions"/>
    </xsl:if>
  </xsl:template>
  -->

  <!--
  <xsl:template match="sections" mode="forum">
    <xsl:for-each select="section">
      <xsl:apply-templates select="." mode="forum"/>
    </xsl:for-each>
  </xsl:template>
  -->
</xsl:stylesheet>