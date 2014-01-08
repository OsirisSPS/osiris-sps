<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:exsl="http://exslt.org/common" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="exsl os date lang system"
                version="1.0">

  <xsl:template name="row-information">
    <xsl:param name="name"/>
    <xsl:param name="value"/>
    <xsl:param name="content"/>    
    <tr>
      <td style="text-align:right;vertical-align:middle;">
        <xsl:value-of select="lang:text($name)"/>:        
      </td>
      <td style="text-align:left;vertical-align:middle;width:100%;">
        <xsl:choose>          
          <xsl:when test="$value != ''">
            <xsl:value-of select="$value"/>            
          </xsl:when>
          <xsl:when test="$content and exsl:node-set($content)/node()">
            <xsl:copy-of select="$content"/>
          </xsl:when>
          <xsl:otherwise>
            <span class="os_unspecified">
              <xsl:value-of select="lang:text('users.unspecified')"/>
            </span>
          </xsl:otherwise>
        </xsl:choose>
      </td>
    </tr>
  </xsl:template>

  <xsl:template name="separator">
    <img src="{system:resource-url('images/dot.png')}"/>
  </xsl:template>
  
  <xsl:template name="active">
    <xsl:param name="active"/>
    <xsl:choose>
      <xsl:when test="$active = 'true'">
        <img src="{system:resource-url('images/icons/16x16/on.png')}"/>
      </xsl:when>
      <xsl:otherwise>
        <img src="{system:resource-url('images/icons/16x16/off.png')}"/>
      </xsl:otherwise>
    </xsl:choose>    
  </xsl:template>
  
  <xsl:template name="link-list">
    <xsl:param name="name"/>
    <xsl:param name="href"/>
    <a href="{$href}">
      <img src="{system:resource-url('images/list_arrow.gif')}"/>
      <xsl:value-of select="$name"/>
    </a>
  </xsl:template>

  <xsl:template name="copy-paste-link">
    <xsl:param name="text"/>

    <xsl:text>javascript:osCopyClipboard('</xsl:text>
    <xsl:value-of select="system:js-encode(lang:text('ide.copy_to_clipboard'))"/>
    <xsl:text>','</xsl:text>
    <xsl:value-of select="system:url-encode(system:js-encode($text))"/>
    <xsl:text>','</xsl:text>
    <xsl:value-of select="system:js-encode(lang:text('common.labels.close'))"/>
    <xsl:text>')</xsl:text>    
  </xsl:template>

  <xsl:template name="copy-paste">
    <xsl:param name="title"/>
    <xsl:param name="body"/>
    <a class="os_button_link">
      <xsl:attribute name="href">
        <xsl:call-template name="copy-paste-link">
          <xsl:with-param name="text" select="$body"/>
        </xsl:call-template>
      </xsl:attribute>
      <img src="{system:resource-url('images/small/edit.gif')}"/>
      <xsl:if test="$title != ''">
        <xsl:value-of select="lang:text($title)"/>
      </xsl:if>
      
    </a>

  </xsl:template>
  <!--
  <xsl:template name="copy-paste">
    <xsl:param name="body"/>

    <div data-os-otype="popup" data-os-popupMode="staticclick">
      <img src="{system:resource-url('images/clipboard_copy.png')}" alt="" onClick="javascript:prompt('{system:js-encode(lang:text('ide.copy_to_clipboard'))}','{$body}'); return false;" style="text-decoration:none;"/>
      <img data-os-popup="header" src="{system:resource-url('images/clipboard_view.png')}" alt=""/>
      <div data-os-popup="body" style="display:none;width:99%;overflow:auto;font-weight:normal;background-color:#FFFFFF;border:1px solid gray; padding:2px;">
        <xsl:value-of select="$body"/>
      </div>
    </div>
    
  </xsl:template>
  -->

  <xsl:template name="limit-title">
    <xsl:param name="title" />
    <xsl:choose>
      <xsl:when test="string-length($title) > 30">
        <xsl:value-of select="substring($title, 0, 30)"/>
        <xsl:value-of select="'...'"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$title"/>
      </xsl:otherwise>
    </xsl:choose>
    
  </xsl:template>

  <xsl:template name="progressbar">
    <xsl:param name="perc"/>
    <xsl:param name="title"/>

    <xsl:choose>
      <xsl:when test="$perc=100">
        <div class="os_progressbar_done">
          <div class="os_progressbar_text">
            <xsl:if test="$title != ''">
              <xsl:value-of select="$title"/>
              <xsl:text> - </xsl:text>
            </xsl:if>
            <xsl:value-of select="lang:text('common.progressbar.completed')"/>            
          </div>
        </div>
      </xsl:when>
      <xsl:when test="$perc=-1">
        <div class="os_progressbar_wait">
          <div class="os_progressbar_text">
            <xsl:if test="$title != ''">
              <xsl:value-of select="$title"/>
              <xsl:text> - </xsl:text>
            </xsl:if>
            <xsl:value-of select="lang:text('common.progressbar.waiting')"/>
          </div>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <div class="os_progressbar_act_back">
          <div class="os_progressbar_act" style="width:{$perc}%">
            <div class="os_progressbar_text">
              <xsl:if test="$title != ''">
                <xsl:value-of select="$title"/>
                <xsl:text> - </xsl:text>
              </xsl:if>
              <xsl:value-of select="concat($perc,'%')"/>
            </div>
          </div>
        </div>
      </xsl:otherwise>
    </xsl:choose>    
  </xsl:template>


</xsl:stylesheet>