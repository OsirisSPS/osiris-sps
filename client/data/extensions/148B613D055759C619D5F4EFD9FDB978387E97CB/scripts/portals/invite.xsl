<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/invite">
    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('portal.pages.invite.title')}" data-os-otype="dialog" data-os-dialog-width="60%">
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('portal.pages.invite.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="page">
    <div class="os_content">
      <xsl:call-template name="help-box">
        <xsl:with-param name="text" select="lang:text('portal.pages.invite.help')"/>
      </xsl:call-template>
      <xsl:call-template name="action-row">
        <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
        <xsl:with-param name="icon" select="'export'"/>
        <xsl:with-param name="name" select="'export_link'"/>
        <xsl:with-param name="href" select="@export_href"/>
      </xsl:call-template>
      <xsl:call-template name="action-row">
        <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
        <xsl:with-param name="icon" select="'invite'"/>
        <xsl:with-param name="name" select="'osiris_invite_link'"/>
        <xsl:with-param name="href">
          <xsl:call-template name="copy-paste-link">
            <xsl:with-param name="text" select="@subscribe_href"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
      <xsl:call-template name="action-row">
        <xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
        <xsl:with-param name="icon" select="'invite'"/>
        <xsl:with-param name="name" select="'isis_invite_link'"/>
        <xsl:with-param name="href">
          <xsl:call-template name="copy-paste-link">
            <xsl:with-param name="text" select="@isis_subscribe_href"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
    </div>
  </xsl:template>


</xsl:stylesheet>