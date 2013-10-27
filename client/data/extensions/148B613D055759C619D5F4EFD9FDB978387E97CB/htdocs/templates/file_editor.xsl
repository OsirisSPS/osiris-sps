<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:lang="http://www.osiris-sps.org/xsl/lang" version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/editor.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="title"/>
  <xsl:param name="description"/>
  <xsl:param name="enable_browser"/>
  <xsl:param name="browser"/>
  <xsl:param name="enable_comment"/>
  <xsl:param name="comment"/>

  <xsl:template match="editor">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <xsl:call-template name="file_editor"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="file_editor">
    <table class="os_table">
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('file.editor.title')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('file.editor.description')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$description" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('file.editor.file')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:if test="$enable_browser">
            <xsl:value-of select="$enable_browser" disable-output-escaping="yes"/>
            <xsl:value-of select="lang:text('file.editor.enable_browser')"/>
          </xsl:if>
          <xsl:if test="$browser">
            <xsl:if test="$enable_browser">
              <br/>
            </xsl:if>
            <xsl:value-of select="$browser" disable-output-escaping="yes"/>
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