<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/rating.xsl"/>  

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

	<xsl:template match="post">
    <xsl:call-template name="block">
      <xsl:with-param name="title">Messaggio</xsl:with-param>
      <xsl:with-param name="content">
        <xsl:call-template name="message">
          <xsl:with-param name="title" select="@title"/>
        </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>  
  </xsl:template>

  <xsl:template match="posts">    
    <xsl:call-template name="block">
      <xsl:with-param name="title">Messaggi</xsl:with-param>
      <xsl:with-param name="content">
        <xsl:for-each select="post">
          <a name="{@entity}"/>
          <xsl:call-template name="message">
            <xsl:with-param name="title" select="@title"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
 
  <xsl:template name="message">
    <xsl:param name="title" select="''"/>
    <xsl:param name="show_commands" select="'true'"/>
    <xsl:param name="vote_editor"/>

    <div class="os_content" data-os-entity="{@entity}">

      <div class="os_divcell" style="float:left;">
        <xsl:call-template name="authors"/>
      </div>
      <div class="os_divcell" style="overflow:auto;">
        <div class="os_divcell os_message_top">
          <a href="{@view_href}" style="text-decoration: none;">
            <img style="padding-right: 3px;" src = "{system:resource-url('images/post_details.gif')}"/>
            <xsl:if test="$title!=''">
              <xsl:value-of select="$title"/>
              <xsl:text>, </xsl:text>
            </xsl:if>
          </a>
          <span class="os_label">
            <xsl:value-of select="lang:text('object.entity_date')"/>
            <xsl:text>: </xsl:text>
          </span>
          <span class="os_value">
            <xsl:value-of select="date:long-datetime(@entity_date)"/>
          </span>
          <div style="float:right;">
            <xsl:if test="$vote_editor">
              <xsl:call-template name="rating-edit">
                <xsl:with-param name="votes" select="stats/@votes"/>
                <xsl:with-param name="votes_average" select="stats/@votes_average"/>
                <xsl:with-param name="editor" select="$vote_editor"/>
              </xsl:call-template>
            </xsl:if>
          </div>
        </div>
        <div style="clear:both;" class="os_message_body">
          <xsl:value-of select="system:parse(@content, true(), false(), false(), 0, @entity)" disable-output-escaping="yes"/>
        </div>
        <xsl:if test="author/@mark">
          <xsl:call-template name="mark">
            <xsl:with-param name="mark" select="author/@mark"/>
          </xsl:call-template>
        </xsl:if>
      </div>

      <div class="os_divcell" style="clear:both">
        <div class="os_divcell" style="float:right">
          <xsl:for-each select="actions/action">
            <xsl:if test="@name != 'reply'">
              <a class="os_button" href="{@href}">
                <xsl:value-of select="lang:text(concat('object.actions.', @name))"/>
              </a>
            </xsl:if>
          </xsl:for-each>
        </div>
        <div class="os_divcell" style="float:left;width:200px">
          <xsl:apply-templates select="author/actions" mode="user"/>
        </div>

        <div class="os_divcell" style="overflow:visible;">
          <a class="os_nolink" href="javascript:scroll(0,0);">
            <img src = "{system:resource-url('images/up.gif')}"/>
          </a>
        </div>

      </div>

      <div style="clear:both"/>

      
    </div>
    <xsl:if test="position() != last()">
      <div class="os_separator"/>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>