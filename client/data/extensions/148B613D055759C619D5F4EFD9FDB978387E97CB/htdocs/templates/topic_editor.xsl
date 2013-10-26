<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:lang="http://www.osiris-sps.org/xsl/lang" version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/editor.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="title"/>
  <xsl:param name="description"/>
  <xsl:param name="content"/>
  <xsl:param name="pinned"/>
  <xsl:param name="enable_comment"/>
  <xsl:param name="comment"/>
  
  <xsl:template match="editor">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <xsl:call-template name="topic_editor"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="topic_editor">
    <table class="os_table">
      <tr>
        <td width="20%" align="right">
          <b>
            <xsl:value-of select="lang:text('topic.editor.title')"/>
          </b>
        </td>
        <td width="80%" align="left">
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td width="20%" align="right">
          <b>
            <xsl:value-of select="lang:text('topic.editor.description')"/>
          </b>
        </td>
        <td width="80%" align="left">
          <xsl:value-of select="$description" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td width="20%" align="right" valign="top">
          <b>
            <xsl:value-of select="lang:text('topic.editor.content')"/>
          </b>
        </td>
        <td width="80%" align="left">
          <xsl:value-of select="$content" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td width="20%" align="right" valign="top">
          <b>
            <xsl:value-of select="lang:text('topic.editor.options')"/>
          </b>
        </td>
        <td width="80%" align="left">
          <xsl:value-of select="$pinned" disable-output-escaping="yes"/>
          <xsl:value-of select="lang:text('topic.editor.pinned')"/>
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