<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:lang="http://www.osiris-sps.org/xsl/lang" version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/editor.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="title"/>
  <xsl:param name="description"/>
  <xsl:param name="content"/>
  <xsl:param name="model"/>
  <xsl:param name="pinned"/>
	<xsl:param name="enable_position"/>
  <xsl:param name="position"/>
  <xsl:param name="enable_comment"/>
  <xsl:param name="comment"/>

  <xsl:template match="editor">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <xsl:call-template name="text_editor"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="text_editor">
    <table class="os_table">
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('text.editor.title')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('text.editor.description')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$description" disable-output-escaping="yes"/>
        </td>
      </tr>      
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('text.editor.content')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$content" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('text.editor.model')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$model" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('text.editor.options')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$pinned" disable-output-escaping="yes"/>
          <xsl:value-of select="lang:text('text.editor.pinned')"/>
        </td>
      </tr>
      
        <tr>
          <td class="os_label">
            <xsl:value-of select="lang:text('instance.editor.position')"/>
            <xsl:text>:</xsl:text>
          </td>
					<td class="os_value">
						<xsl:value-of select="$enable_position" disable-output-escaping="yes"/>
						<xsl:value-of select="$position" disable-output-escaping="yes"/>
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