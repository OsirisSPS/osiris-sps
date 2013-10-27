<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/editor.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="title"/>
  <xsl:param name="description"/>
  <xsl:param name="schema"/>
  <xsl:param name="stylesheet"/>
  <xsl:param name="sample"/>
  <xsl:param name="enable_comment"/>
  <xsl:param name="comment"/>

  <xsl:template match="editor">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <xsl:call-template name="model_editor"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="model_editor">
    <table class="os_table">
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('model.editor.title')"/>
          <xsl:text>:</xsl:text>          
        </td>
        <td class="os_value">
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('model.editor.description')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$description" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('model.editor.schema')"/>
          <xsl:text>:</xsl:text>          
        </td>
        <td class="os_value">
          <xsl:value-of select="$schema" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('model.editor.stylesheet')"/>
          <xsl:text>:</xsl:text>          
        </td>
        <td class="os_value">
          <xsl:value-of select="$stylesheet" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('model.editor.sample')"/>
          <xsl:text>:</xsl:text>          
        </td>
        <td class="os_value">
          <xsl:value-of select="$sample" disable-output-escaping="yes"/>
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