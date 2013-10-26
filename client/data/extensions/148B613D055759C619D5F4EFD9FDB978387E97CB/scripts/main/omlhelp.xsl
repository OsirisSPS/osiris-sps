<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>
  
  <xsl:template match="/omlhelp">
    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('portal.pages.omlhelp.title')}" data-os-otype="dialog" data-os-dialog-width="window" data-os-dialog-height="window">
          <xsl:call-template name="main"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="main"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="main">
    <xsl:call-template name="help-box">
      <xsl:with-param name="text" select="lang:text('portal.pages.omlhelp.help')"/>
    </xsl:call-template>
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('portal.pages.omlhelp.title')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'b'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'i'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'u'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'s'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'o'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'color'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'size'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'font'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'highlight'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'sup'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'sub'"/>
          </xsl:call-template>

          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'h1'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'h2'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'h3'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'h4'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'h5'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'h6'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'pre'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'tt'"/>
          </xsl:call-template>
        </div>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('bbcodes.help.section.layout')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'left'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'justify'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'indent'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'table'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'div'"/>
          </xsl:call-template>
        </div>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('bbcodes.help.section.misc')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'list'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'hr'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'br'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'marquee'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'abbr'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'acronym'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'object'"/>
          </xsl:call-template>
        </div>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('bbcodes.help.section.blocks')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'code'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'codebox'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'quote'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'box'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'spoiler'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'whisper'"/>
          </xsl:call-template>

        </div>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('bbcodes.help.section.links')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'img'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'url'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'email'"/>
          </xsl:call-template>
        </div>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('bbcodes.help.section.language')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'lang'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'date'"/>
          </xsl:call-template>
        </div>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('bbcodes.help.section.special')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'noparse'"/>
          </xsl:call-template>
          <xsl:call-template name="bbCode">
            <xsl:with-param name="code" select="'html'"/>
          </xsl:call-template>
        </div>
      </xsl:with-param>
    </xsl:call-template>

  </xsl:template>


  <xsl:template name="bbCode">
    <xsl:param name="code"/>

    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title" select="lang:text(concat('bbcodes.help.',$code,'.title'))"/>
      <xsl:with-param name="content">
        <table class="os_table">
          <tr>
            <td class="os_subtitle" colspan="2">
              <xsl:value-of select="lang:text(concat('bbcodes.help.',$code,'.description'))"/>
            </td>
          </tr>
          <tr>
            <th>
              <xsl:value-of select="lang:text('bbcodes.help.sample')"/>
            </th>
            <th>
              <xsl:value-of select="lang:text('bbcodes.help.result')"/>
            </th>
          </tr>
          <tr>
            <td width="50%">
              <div class="os_description">                
                <xsl:value-of select="system:html-encode(lang:text(concat('bbcodes.help.',$code,'.sample')))" disable-output-escaping="yes"/>                
              </div>
            </td>
            <td width="50%">
              <div class="os_description">
	              <xsl:value-of select="system:parse(lang:text(concat('bbcodes.help.',$code,'.sample')))" disable-output-escaping="yes"/>	                      
              </div>
            </td>
          </tr>
        </table>
      </xsl:with-param>
    </xsl:call-template>
    
  </xsl:template>

  
</xsl:stylesheet>