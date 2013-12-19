<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">
    
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/post_viewer.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/tag_viewer.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="vote_editor"/>

  <xsl:template match="text">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <div data-os-entity="{@entity}">

          <xsl:call-template name="text-details">
            <xsl:with-param name="show_stats" select="false()"/>
          </xsl:call-template>
          <div style="float:right;">
            <xsl:call-template name="rating-edit">
              <xsl:with-param name="votes" select="stats/@votes"/>
              <xsl:with-param name="votes_average" select="stats/@votes_average"/>
              <xsl:with-param name="editor" select="$vote_editor"/>
            </xsl:call-template>
          </div>
          <div style="clear:both;" class="os_text">
            <xsl:value-of select="system:parse(@content, true(), false(), false(), 0, @entity)" disable-output-escaping="yes"/>
          </div>
          <xsl:apply-templates select="tags"/>

        </div>
      </xsl:with-param>
    </xsl:call-template>
    
    <xsl:call-template name="objectActions"/>
    <xsl:if test="posts">
      <xsl:apply-templates select="posts"/>
      <xsl:call-template name="objectActions"/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="text" mode="blog">
    <xsl:param name="show_separator" select="false()"/>
    <div data-os-entity="{@entity}">
      <div class="os_blog_title">
        <!--
        <xsl:call-template name="object-state">
          <xsl:with-param name="new" select="(date:compare(@submit_date, system:reference-date()) > 0) or (date:compare(stats/@last_message_date, system:reference-date()) > 0)"/>
          <xsl:with-param name="hot" select="stats/@posts >= system:get-option('ide.objects.hot_state_threshold')"/>
          <xsl:with-param name="style" select="'padding-right:4px;'"/>
        </xsl:call-template>        
        -->
        <a href="{@view_href}">
          <xsl:value-of select="@title"/>
        </a>
      </div>
      <xsl:call-template name="text-details"/>
      <div class="os_text" style="min-height: 100px;">
        <xsl:value-of select="system:parse(@content, false(), false(), false(), 0, @entity)" disable-output-escaping="yes"/>
      </div>
    </div>
    <!--
    <xsl:if test="$show_separator = true()">
      <div class="os_separator"/>
    </xsl:if>
    -->
  </xsl:template>

  <xsl:template name="text-details">
    <div class="os_object_blog_details_left">
      <xsl:value-of select="lang:text('object.blog.author')"/>
      <xsl:text> </xsl:text>
      <xsl:apply-templates select="author" mode="row"/>
      <!--
        <xsl:text> </xsl:text>        
        <xsl:value-of select="lang:text('object.blog.where')"/>
        <xsl:text> </xsl:text>
        <xsl:text>Prova</xsl:text>
        -->
      <xsl:text> , </xsl:text>
      <xsl:value-of select="date:userfriendly-datetime(@entity_date)"/>
    </div>    
  </xsl:template>
  
</xsl:stylesheet>