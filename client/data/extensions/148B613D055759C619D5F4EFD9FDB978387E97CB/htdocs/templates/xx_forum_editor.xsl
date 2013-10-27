<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/editor.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="title"/>
  <xsl:param name="description"/>
  <xsl:param name="enable_rules"/>
  <xsl:param name="rules"/>
  <xsl:param name="enable_comment"/>
  <xsl:param name="comment"/>

  <xsl:template match="editor">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <xsl:call-template name="forum_editor"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="forum_editor">
    <table class="os_table">
      <tr>
        <td width="20%" align="right">
          <b>
            <xsl:value-of select="lang:text('forum.editor.title')"/>
          </b>
        </td>
        <td width="80%" align="left">
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td width="20%" align="right">
          <b>
            <xsl:value-of select="lang:text('forum.editor.description')"/>
          </b>
        </td>
        <td width="80%" align="left">
          <xsl:value-of select="$description" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td width="20%" align="right" valign="top">
          <b>
            <xsl:value-of select="lang:text('forum.editor.rules')"/>
          </b>
        </td>
        <td width="80%" align="left">
          <xsl:if test="$enable_rules">
            <xsl:value-of select="$enable_rules" disable-output-escaping="yes"/>
            <xsl:value-of select="lang:text('forum.editor.enable_rules')"/>
          </xsl:if>
          <xsl:if test="$rules">
            <xsl:if test="$enable_rules">
              <br/>
            </xsl:if>
            <xsl:value-of select="$rules" disable-output-escaping="yes"/>
          </xsl:if>
        </td>
      </tr>
      <xsl:call-template name="row_object_comment">
        <xsl:with-param name="enable_comment" select="$enable_comment"/>
        <xsl:with-param name="comment" select="$comment"/>
      </xsl:call-template>
      <xsl:call-template name="editor_actions"/>
    </table>
  </xsl:template>
</xsl:stylesheet>