<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:exsl="http://exslt.org/common" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="exsl os date lang system"
                version="1.0">
  
  <xsl:template name="help-tip">
    <xsl:param name="id"/>
    <xsl:param name="title"/>
    <div data-os-url="/main/helptip?title={$title}&amp;id={$id}" data-os-wait="none"/>
  </xsl:template>

  <xsl:template name="help-box">
    <xsl:param name="text"/>
    <xsl:param name="id"/>
    <div class="os_help_box">
      <img style="float:right" src="{system:resource-url('images/icons/32x32/help.png')}"/>
      <xsl:value-of select="system:parse($text)" disable-output-escaping="yes"/>
      <xsl:if test="$id != ''">
        <div class="os_help_more">

          <a target="_blank" href="{system:help-link($id)}">
            <xsl:value-of select="lang:text('help.box.docs_link')"/>
          </a>
        </div>
      </xsl:if>
    </div>    
  </xsl:template>

  <xsl:template name="help-tooltip">
    <xsl:param name="text"/>
    <xsl:param name="id"/>

    <xsl:choose>      
      <xsl:when test="$id != ''">
        <xsl:variable name="htmlText" select="concat(system:parse($text),'&lt;br /&gt;&lt;div class=&quot;os_help_more&quot;&gt;',lang:text('help.tooltip.docs_link'),'&lt;/div&gt;')"/>

        <a class="os_nolink" target="_blank" href="{system:help-link($id)}">
          <img class="os_help_icon" src="{system:resource-url('images/help_button.png')}" data-os-tooltip="{$htmlText}"/>          
        </a>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="htmlText" select="system:parse($text)"/>
        <img class="os_help_icon" src="{system:resource-url('images/help_button.png')}" data-os-tooltip="{$htmlText}"/>
      </xsl:otherwise>      
    </xsl:choose>
    
  </xsl:template>

</xsl:stylesheet>