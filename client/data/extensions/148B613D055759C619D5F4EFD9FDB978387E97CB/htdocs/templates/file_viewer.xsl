<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang date system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/rating.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/post_viewer.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/tag_viewer.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="vote_editor"/>

  <xsl:template match="file">
    <xsl:call-template name="file_viewer"/>
  </xsl:template>

  <xsl:template name="file_content">
    <xsl:param name="show_authors" select="true()"/>
    <xsl:param name="show_rating" select="true()"/>
    <div>
      <!-- Image view -->
      <xsl:if test="starts-with(@mime_type,'image')">
        <div style="overflow:auto;text-align:center;">
          <img src="{@file_href}"/>
        </div>
        <div class="os_separator" colspan="3"/>
      </xsl:if>
      <div style="float:right;padding:20px; vertical-align:middle;text-align:center;">
        <a href="{@file_href}">
          <img src="{system:resource-url('images/download.png')}"/>
          <span style="margin:5px;font-size:2em">
            <xsl:value-of select="lang:text('common.download')"/>
          </span>
        </a>
      </div>
      <xsl:if test="$show_authors">
        <div style="float:left;padding:20px;width:200px;">
          <xsl:call-template name="authors"/>
        </div>
      </xsl:if>
      <div style="overflow:auto;padding:20px">
        <table class="os_table_properties os_content_box">
          <xsl:if test="@description != ''">
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'file.description'"/>
              <xsl:with-param name="value" select="@description"/>
            </xsl:call-template>
          </xsl:if>
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'object.entity_date'"/>
            <xsl:with-param name="value" select="date:long-datetime(@entity_date)"/>
          </xsl:call-template>
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'file.file_name'"/>
            <xsl:with-param name="value" select="@file_name"/>
          </xsl:call-template>
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'file.mime_type'"/>
            <xsl:with-param name="content">
              <xsl:call-template name="file-icon"/>
              <xsl:text> </xsl:text>
              <xsl:value-of select="@mime_type"/>
            </xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'file.file_size'"/>
            <xsl:with-param name="value" select="system:format-size(@file_size)"/>
          </xsl:call-template>          
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'file.viewer.bbcode'"/>
            <xsl:with-param name="content">
              <xsl:call-template name="copy-paste">
                <xsl:with-param name="body" select="concat('[img]',@file_osiris_href,'[/img]')"/>
              </xsl:call-template>
            </xsl:with-param>
          </xsl:call-template>
          <xsl:call-template name="row-information">
            <xsl:with-param name="name" select="'file.viewer.url'"/>
            <xsl:with-param name="content">
              <xsl:call-template name="copy-paste">
                <xsl:with-param name="body" select="@file_osiris_href"/>
              </xsl:call-template>
            </xsl:with-param>
          </xsl:call-template>
          <xsl:if test="$show_rating">
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'file.viewer.rating'"/>
              <xsl:with-param name="content">
                <xsl:call-template name="rating-edit">
                  <xsl:with-param name="votes" select="stats/@votes"/>
                  <xsl:with-param name="votes_average" select="stats/@votes_average"/>
                  <xsl:with-param name="editor" select="$vote_editor"/>
                </xsl:call-template>
              </xsl:with-param>
            </xsl:call-template>
          </xsl:if>
        </table>
      </div>
    </div>
    <xsl:apply-templates select="tags"/>
  </xsl:template>

   <xsl:template name="file_viewer">    

    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <xsl:call-template name="file_content"/>
      </xsl:with-param>
      <xsl:with-param name="startClose" select="pager/@current_page > 0"/>
    </xsl:call-template>
    <xsl:call-template name="objectActions"/>
    <xsl:if test="posts">
      <xsl:apply-templates select="posts"/>
      <xsl:call-template name="objectActions"/>
    </xsl:if>
  </xsl:template>

  <xsl:template match="file" mode="repository">
    <xsl:param name="show_separator" select="false()"/>
    <table class="os_table">
      <tr>
        <td width="20%">          
          <xsl:call-template name="object-state">
            <xsl:with-param name="new" select="(date:compare(@entity_date, system:reference-date()) > 0) or (date:compare(stats/@last_entry_date, system:reference-date()) > 0)"/>
            <xsl:with-param name="hot" select="stats/@posts >= system:get-option('ide.objects.hot_state_threshold')"/>
            <xsl:with-param name="style" select="'padding-right:4px;'"/>
          </xsl:call-template>
          <a href="{@view_href}">
            <b>
              <xsl:value-of select="@title"/>
            </b>
          </a>
        </td>
        <td width="80%" align="right">
          <a href="{@file_href}">
            <b>
              <xsl:value-of select="lang:text('file.viewer.download')"/>
            </b>
          </a>
        </td>
      </tr>
    </table>    
    <table class="os_table">
      <tr>
        <td style="padding:2px; height:104px;width:104px; text-align:center; vertical-align:top;">
          <xsl:if test="starts-with(@mime_type,'image')">
            <!--<img class="os_image_icon" src="{@file_href}" title="{@title}"/>-->
            <img onload="javascript:Osiris.imageResize(this, 100, 100);" src="{@file_href}" title="{@title}"/>
          </xsl:if>
        </td>
        <td>
          <table>
            <xsl:if test="@description">
              <xsl:call-template name="row-information">
                <xsl:with-param name="name" select="'file.description'"/>
                <xsl:with-param name="value" select="@description"/>
              </xsl:call-template>
            </xsl:if>
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'file.file_size'"/>
              <xsl:with-param name="value" select="system:format-size(@file_size)"/>
            </xsl:call-template>            
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'object.entity_date'"/>
              <xsl:with-param name="value" select="date:long-datetime(@entity_date)"/>
            </xsl:call-template>
            <xsl:if test="@revisioned">
              <xsl:call-template name="row-information">
                <xsl:with-param name="name" select="'object.submit_date'"/>
                <xsl:with-param name="value" select="date:long-datetime(@submit_date)"/>
              </xsl:call-template>
            </xsl:if>
            <xsl:if test="stats">
              <xsl:call-template name="row-information">
                <xsl:with-param name="name" select="'file.viewer.posts'"/>
                <xsl:with-param name="value" select="stats/@posts"/>
              </xsl:call-template>
            </xsl:if>
            <xsl:call-template name="row-information">
              <xsl:with-param name="name" select="'file.viewer.rating'"/>
              <xsl:with-param name="content">
                <xsl:call-template name="rating-view">
                  <xsl:with-param name="votes" select="stats/@votes"/>
                  <xsl:with-param name="votes_average" select="stats/@votes_average"/>
                </xsl:call-template>
              </xsl:with-param>
            </xsl:call-template>
          </table>          
        </td>
      </tr>
      <tr>
        <td colspan="2" align="right">
          <xsl:call-template name="authors-row"/>
        </td>
      </tr>     
      <xsl:if test="$show_separator = true()">
        <tr>
          <td class="os_separator" colspan="2"/>
        </tr>
      </xsl:if>      
    </table>
  </xsl:template>

  <xsl:template name="file-icon">
    <xsl:variable name="href" select="system:resource-url(concat('images/mime/', translate(@mime_type,'/','-'), '.gif'))"/>
    <xsl:choose>
      <xsl:when test="$href != ''">
        <img src="{$href}"/>
      </xsl:when>
      <xsl:otherwise>
        <img src="{system:resource-url('images/mime/unknown.gif')}"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

</xsl:stylesheet>
