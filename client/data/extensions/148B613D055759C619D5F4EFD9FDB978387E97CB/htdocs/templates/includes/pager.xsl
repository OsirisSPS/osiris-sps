<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang date system"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="pager">    
    <xsl:call-template name="pager-forum"/>
  </xsl:template>

  <xsl:template match="pager" mode="center">
    <!--<div align="center" style="vertical-align:middle;padding-bottom:5px;">-->
      <xsl:call-template name="pager-forum"/>
    <!--</div>  -->
  </xsl:template>
  
  <xsl:template match="pager" mode="row">
    <!--<table>
      <tr>
        <td style="vertical-align:middle;padding-bottom:5px;">
        -->
          <xsl:call-template name="pager-forum"/>
    <!--
        </td>
      </tr>
    </table>
    -->
  </xsl:template>

  <xsl:template match="pager" mode="forum">
    <xsl:call-template name="pager-forum"/>
  </xsl:template>

  <xsl:template match="pager" mode="blog">
    <xsl:call-template name="pager-blog"/>
  </xsl:template>

  <xsl:template name="pager-blog">    
    <div class="os_pager">
      <xsl:if test="(@current_page+1) &gt; 1">
        <a class="os_pager_blog_left" href="{page[@index=(../@current_page)]/@href}" data-os-tooltip="{@current_page} / {@pages} {lang:text('pager.total_pages')}">
          <img src="{system:resource-url('images/pager_blog_prev.png')}"/>
          <xsl:value-of select="lang:text('pager.blog.left')"/>
        </a>
      </xsl:if>
      <xsl:if test="(@current_page+1) &lt; @pages">
        <a class="os_pager_blog_right" href="{page[@index=(../@current_page+2)]/@href}" data-os-tooltip="{@current_page+2} / {@pages} {lang:text('pager.total_pages')}">
          <xsl:value-of select="lang:text('pager.blog.right')"/>
          <img src="{system:resource-url('images/pager_blog_next.png')}"/>
        </a>
      </xsl:if>
    </div>
    <div style="clear:both;margin-bottom:30px;"/>
  </xsl:template>

  <xsl:template name="pager-forum">
    <div class="os_pager">
      <xsl:if test="@pages > 1">
        <a class="os_pager_info" onClick="javascript:osPagerGotoPage('{@goto_href}',1,{@pages},{@items_to_show}); return false;" title="{concat(@items, ' ', lang:text('pager.total_items'))}">
          <img src="{system:resource-url('images/pages_icon.gif')}"/>
          <xsl:text> </xsl:text>
          <xsl:value-of select="concat(@pages, ' ', lang:text('pager.total_pages'), ' ')"/>
          <img src="{system:resource-url('images/menu_action_down.gif')}"/>
        </a>        
      </xsl:if>
      <xsl:for-each select="page">
          
          <a href="{@href}" valign="middle">
            <xsl:attribute name="class">
              <xsl:text>os_pager_item </xsl:text>
              <xsl:choose>
                <xsl:when test="@type='first'">
                  os_pager_first
                </xsl:when>
                <xsl:when test="@type='current'">
                  os_pager_current
                </xsl:when>
                <xsl:when test="@type='last'">
                  os_pager_last
                </xsl:when>
              </xsl:choose>
            </xsl:attribute>
            
            <xsl:choose>
              <xsl:when test="@type='first'">
                <xsl:value-of select="lang:text('pager.page_first')"/>
              </xsl:when>
              <xsl:when test="@type='prev'">
                <xsl:value-of select="lang:text('pager.page_prev')"/>
              </xsl:when>
              <xsl:when test="@type='next'">
                <xsl:value-of select="lang:text('pager.page_next')"/>
              </xsl:when>
              <xsl:when test="@type='last'">
                <xsl:value-of select="lang:text('pager.page_last')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="@index"/>
              </xsl:otherwise>
            </xsl:choose>
          </a>        
      </xsl:for-each>
      <div class="os_pager_summary">
        <xsl:value-of select="@items_from"/>
        <xsl:text> - </xsl:text>
        <xsl:value-of select="@items_to"/>
        <xsl:text> / </xsl:text>
        <xsl:value-of select="@items"/>
        <xsl:text> </xsl:text>
        <xsl:value-of select="lang:text('pager.total_items')"/>
      </div>
      <div class="os_clear"/>
    </div>
  </xsl:template>
 
</xsl:stylesheet>