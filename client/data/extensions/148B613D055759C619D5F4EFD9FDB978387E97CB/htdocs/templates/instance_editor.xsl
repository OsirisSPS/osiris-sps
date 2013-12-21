<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                exclude-result-prefixes="os lang"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/editor.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="title"/>
  <xsl:param name="area"/>
  <xsl:param name="inherit"/>
  <xsl:param name="module"/>
  <xsl:param name="editor"/>
  <xsl:param name="block"/>
  <xsl:param name="style"/>
  <xsl:param name="position"/>
  <xsl:param name="enable_comment"/>
  <xsl:param name="comment"/>

  <xsl:template match="editor">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <xsl:call-template name="instance_editor"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="instance_editor">

    <table class="os_table_data">
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('instance.editor.title')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$title" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('instance.editor.area')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$area" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('instance.editor.block')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$block" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('instance.editor.style')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$style" disable-output-escaping="yes"/>
        </td>
      </tr>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('instance.editor.inherit')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$inherit" disable-output-escaping="yes"/>
        </td>
      </tr>
      <xsl:if test="$position">
        <tr>
          <td class="os_label">
            <xsl:value-of select="lang:text('instance.editor.position')"/>
            <xsl:text>:</xsl:text>
          </td>
          <td class="os_value">
            <xsl:value-of select="$position" disable-output-escaping="yes"/>
          </td>
        </tr>
      </xsl:if>
      <tr>
        <td class="os_label">
          <xsl:value-of select="lang:text('instance.editor.module')"/>
          <xsl:text>:</xsl:text>
        </td>
        <td class="os_value">
          <xsl:value-of select="$module" disable-output-escaping="yes"/>
        </td>
      </tr>
      <xsl:call-template name="row_object_comment">
        <xsl:with-param name="enable_comment" select="$enable_comment"/>
        <xsl:with-param name="comment" select="$comment"/>
      </xsl:call-template>

      <xsl:if test="$editor">
        <tr>
          <td colspan="2">
            <xsl:call-template name="block_minimal">
              <xsl:with-param name="title" select="lang:text('instance.editor.editor')"/>
							<xsl:with-param name="content">
								<xsl:value-of select="$editor" disable-output-escaping="yes"/>
							</xsl:with-param>
            </xsl:call-template>
          </td>
        </tr>

      </xsl:if>
      <xsl:call-template name="editor_actions"/>
    </table>
    
  </xsl:template>
</xsl:stylesheet>