<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes=" date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:template name="user-full">
    <xsl:call-template name="user-lite"/>
  </xsl:template>
  
  <xsl:template name="user-lite">
    <div class="os_user_full">

      <a class="os_user_name" href="{@view_href}">
        <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes"/>
      </a>
      <xsl:if test="@avatar">
      	<br/>
        <img class = "os_avatar_box" src = "{@avatar}" alt ="avatar"/>
      </xsl:if>      
      
      <xsl:if test="@description != ''">
        <div class="os_user_description">
          <xsl:value-of select="system:parse(@description, false(), false(), true())" disable-output-escaping="yes"/>
        </div>        
      </xsl:if>
      <xsl:call-template name="user-stats"/>
    </div>
  </xsl:template>

  <xsl:template name="user-box">
    <div class="os_user_box">      
    	<a class="os_user_name" href="{@view_href}">
          <xsl:call-template name="avatar-small">
            <xsl:with-param name="width" select="64"/>
            <xsl:with-param name="style" select="'float:right;margin:10px;'"/>
          </xsl:call-template>        
          <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes"/>                
      </a>
      
      <div class="os_user_description">
        <xsl:value-of select="system:parse(@description, false(), false(), true())" disable-output-escaping="yes"/>
      </div>
      <xsl:call-template name="user-stats"/>
    </div>
  </xsl:template>

  <xsl:template name="user-stats">
    <div class="os_center os_clear">
      <table class="os_user_stats os_table_properties">
        <xsl:call-template name="row-information">
          <xsl:with-param name="name" select="'user.reputation'"/>
          <xsl:with-param name="content">
            <xsl:call-template name="reputation-line"/>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:if test="stats">
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'user.stats.objects'"/>
            <xsl:with-param name="value" select="stats/@objects"/>
          </xsl:call-template>
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'user.stats.revisions'"/>
            <xsl:with-param name="value" select="stats/@revisions"/>
          </xsl:call-template>
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'user.join_date'"/>
            <xsl:with-param name="value" select="date:short-date(@join_date)"/>
          </xsl:call-template>
          <xsl:if test="stats/@progressive_number">
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'user.stats.progressive_number'"/>
              <xsl:with-param name="value" select="stats/@progressive_number"/>
            </xsl:call-template>
          </xsl:if>
        </xsl:if>
      </table>
    </div>
  </xsl:template>

  <xsl:template name="user-row">
    <xsl:param name="with_detail"/>




    <xsl:choose>
      <xsl:when test="@name">

        <a class="os_button_link os_user_row" href="{@view_href}">

          <xsl:call-template name="avatar-small">
            <xsl:with-param name="width" select="16"/>
          </xsl:call-template>

          <span style="margin-left:10px">
            <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes"/>
          </span>

          <!--
          <img class="os_reputation" src="{system:resource-url(concat('images/reputation_score/dot/',@score_value,'.png'))}" title="Score: {@score_value} - Comment: {@score_comment}" alt="Score: {@score_value} - Comment: {@score_comment}"/>
          
          -->
        </a>

      </xsl:when>
      <xsl:otherwise>
        <xsl:text>~unknown~</xsl:text>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template match="actions" mode="user">
    <xsl:for-each select="action">
      <a href="{@href}">
        <xsl:value-of select="lang:text(concat('user.actions.', @name))"/>                
      </a>
    </xsl:for-each>
  </xsl:template>

  <xsl:template name="reputation-line">
    <img src="{system:resource-url(concat('images/reputation_score/line/',@score_value,'.png'))}" title="Score: {@score_value} - Comment: {@score_comment}" alt="Score: {@score_value} - Comment: {@score_comment}"/>
  </xsl:template>
    
  <xsl:template name="avatar-small">
    <xsl:param name="width" select="64"/>
    <xsl:param name="style" select="''"/>
    <img style="max-width:{$width}px;max-height:{$width}px;{$style}" class = "os_avatar_box" alt = "avatar" src="{system:resource-url('images/avatar.png')}">
    	<xsl:if test="@avatar">
    		<xsl:attribute name="data-os-delayedsrc">
    			<xsl:value-of select="@avatar"/>
    		</xsl:attribute>
    	</xsl:if>
    	<!--
      <xsl:attribute name="src">
        <xsl:choose>
          <xsl:when test="@avatar">
            <xsl:value-of select="@avatar"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="system:resource-url('images/avatar_undef.png')"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      -->
    </img>
  </xsl:template>

  <xsl:template name="mark">    
    <xsl:param name="mark"/>
    <xsl:param name="secure" select="false()"/>    
    <div class="os_signature">
      <xsl:value-of select="system:parse($mark, true(), $secure)" disable-output-escaping="yes"/>
    </div>    
  </xsl:template>
  
  <xsl:template name="authors">
    <xsl:apply-templates select="author" mode="lite"/>    
    <xsl:if test="editor">
      <br/>
      <div class="os_label">
        <xsl:value-of select="lang:text('user.edited_by')"/>
        <xsl:text>: </xsl:text>
      </div>
      <div class="os_center">
        <xsl:apply-templates select="editor" mode="row"/>
      </div>
    </xsl:if>
  </xsl:template>

  <xsl:template name="authors-row">
    <span class="os_nowrap">
      <span class="os_label">
        <xsl:value-of select="lang:text('object.author')"/>
        <xsl:text>: </xsl:text>
      </span>
      <xsl:apply-templates select="author" mode="row"/>
      <xsl:if test="editor">
        <xsl:text> </xsl:text>
        <span class="os_label">
          <xsl:value-of select="lang:text('object.editor')"/>
          <xsl:text>: </xsl:text>
        </span>
        <xsl:apply-templates select="editor" mode="row"/>
      </xsl:if>
    </span>
  </xsl:template>

  <xsl:template name="editor-row">
    <span class="os_nowrap">      
      <xsl:choose>
        <xsl:when test="editor">
          <span class="os_label">
            <xsl:value-of select="lang:text('object.editor')"/>
            <xsl:text>: </xsl:text>
          </span>
          <xsl:apply-templates select="editor" mode="row"/>
        </xsl:when>
        <xsl:otherwise>
          <span class="os_label">
            <xsl:value-of select="lang:text('object.author')"/>
            <xsl:text>: </xsl:text>
          </span>
          <xsl:apply-templates select="author" mode="row"/>
        </xsl:otherwise>
      </xsl:choose>
    </span>
  </xsl:template>

  <xsl:template match="author" mode="lite">
    <xsl:call-template name="user-lite"/>
  </xsl:template>

  <xsl:template match="editor" mode="lite">
    <xsl:call-template name="user-lite"/>
  </xsl:template>
  
  <xsl:template match="author" mode="row">
    <xsl:call-template name="user-row"/>
  </xsl:template>

  <xsl:template match="editor" mode="row">
    <xsl:call-template name="user-row"/>
  </xsl:template>

</xsl:stylesheet>