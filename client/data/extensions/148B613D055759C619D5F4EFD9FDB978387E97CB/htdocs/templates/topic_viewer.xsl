<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="os date lang system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/post_viewer.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/tag_viewer.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="vote_editor"/>

  <xsl:template match="topic">
    <xsl:call-template name="block">
      <xsl:with-param name="title">
        <xsl:value-of select="@title"/>       
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:call-template name="message">
          <xsl:with-param name="title" select="@description"/>
          <xsl:with-param name="show_commands" select="'false'"/>
          <xsl:with-param name="vote_editor" select="$vote_editor"/>
        </xsl:call-template>
        <xsl:apply-templates select="tags"/>
      </xsl:with-param>
      <xsl:with-param name="startClose" select="pager/@current_page > 0"/>
    </xsl:call-template>
    
    <xsl:call-template name="topicActions"/>    
    <xsl:if test="posts">
      <xsl:apply-templates select="posts"/>
      <xsl:call-template name="topicActions"/>
    </xsl:if>
  </xsl:template>

  <xsl:template name="topicActions">
    <xsl:if test="pager">      
      <xsl:apply-templates select="pager"/>
    </xsl:if>
    <div style="float:right">
      <xsl:for-each select="actions/action">
        <!-- L'azione di quote è già implementata dal template "message" -->
        <xsl:if test="@name != 'quote'">
          <a class="os_button" href="{@href}">
            <xsl:value-of select="lang:text(concat('object.actions.', @name))"/>            
          </a>
        </xsl:if>
      </xsl:for-each>
    </div>
  </xsl:template>
</xsl:stylesheet>