<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="date lang system"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="editor_actions">
    <tr>
      <td class="os_commands" colspan="2">
        <xsl:for-each select="actions/action">
          <a class="os_button" href="{@href}">
            <xsl:value-of select="lang:text(concat('editor.actions.', @name))"/>
          </a>
        </xsl:for-each>
      </td>
    </tr>
  </xsl:template>  
  
  <xsl:template name="row_object_comment">
    <xsl:param name="enable_comment"/>
    <xsl:param name="comment"/>

    <tr>
      <td class="os_label">
        <xsl:value-of select="lang:text('object.editor.comment')"/>
        <xsl:text>:</xsl:text>
      </td>
      <td class="os_value">
        <xsl:if test="$enable_comment">
          <xsl:value-of select="$enable_comment" disable-output-escaping="yes"/>
          <xsl:value-of select="lang:text('object.editor.enable_comment')"/>
        </xsl:if>
        <xsl:if test="$comment">
          <xsl:if test="$enable_comment">
            <br/>
          </xsl:if>
          <xsl:value-of select="$comment" disable-output-escaping="yes"/>
        </xsl:if>
      </td>
    </tr>
  </xsl:template>
    
</xsl:stylesheet>
