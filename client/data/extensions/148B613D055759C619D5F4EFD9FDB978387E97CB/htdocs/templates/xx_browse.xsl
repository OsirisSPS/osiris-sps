<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="show_system"/>
  <xsl:param name="show_invisible"/>
  
  <xsl:template match="browse">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('portal.pages.browse.current')"/>
      <xsl:with-param name="content">
        <xsl:for-each select="object">
          <xsl:call-template name="object-lite">
            <xsl:with-param name="childs" select="false()"/>
          </xsl:call-template>
        </xsl:for-each>
      </xsl:with-param>
    </xsl:call-template>
    
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('portal.pages.browse.childs')"/>
      <xsl:with-param name="content">
        <xsl:apply-templates select="pager" mode="row"/>

        <xsl:apply-templates select="." mode="browse_childs"/>

        <xsl:apply-templates select="pager" mode="row"/>

        <div class="os_commands_right" style="padding:10px;">
          <xsl:if test="$show_system">
            <xsl:value-of select="$show_system" disable-output-escaping="yes"/>
          </xsl:if>
          <xsl:value-of select="$show_invisible" disable-output-escaping="yes"/>
        </div>
        <div style="clear:both"></div>

      </xsl:with-param>
    </xsl:call-template>    
  </xsl:template>



  <xsl:template match="browse" mode="browse_childs">  
    <table class="os_table2">
      <tr>
        <th colspan="2" width="20%">
          <xsl:value-of select="lang:text('object.title')"/>
        </th>                
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('object.type')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('object.author')"/>
        </th>
        <th width="20%" align="center">
          <xsl:value-of select="lang:text('object.submit_date')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('object.editor')"/>
        </th>
        <th width="20%" align="center">
          <xsl:value-of select="lang:text('object.revision_date')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('object.actions')"/>
        </th>
      </tr>
      <xsl:if test="parent">
        <tr class="os_parent" >
          <td valign="middle" align="center" width="1%">
            <a class="os_nolink" href="{parent/@browse_href}">
              <img src="{system:resource-url('images/objects/utils_parent.png')}"/>
            </a>            
          </td>
          <td colspan="7" >
            <b>
              <a href="{parent/@browse_href}">
                <xsl:text>..</xsl:text>
              </a>
            </b>
          </td>
        </tr>
      </xsl:if>
      <xsl:for-each select="childs/object">
        <xsl:variable name="type" select="lang:text(concat('objects.types.', @type))"/>
        <tr data-os-objectId="{@id}">
          <td valign="middle" align="center" width="1%">
            <a class="os_nolink" href="{@browse_href}">
              <xsl:call-template name="object-icon"/>
            </a>
          </td>
          <td>
            <b>
              <a href="{@browse_href}">
                <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
              </a>
            </b>
            <xsl:if test="@description != ''">
              <br />
              <span class="os_object_description">
                <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
              </span>
            </xsl:if>
          </td>                    
          <td align="center">
            <xsl:call-template name="object-type"/>
          </td>
          <td align="center">
            <xsl:apply-templates select="author" mode="row"/>
          </td>
          <td align="center">
            <xsl:value-of select="date:long-datetime(@submit_date)"/>
          </td>
          <td align="center">
            <xsl:choose>
              <xsl:when test="editor">
                <xsl:apply-templates select="editor" mode="row"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>-</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </td>
          <td align="center">
            <xsl:choose>
              <xsl:when test="@revision_date">
                <xsl:value-of select="date:long-datetime(@revision_date)"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>-</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </td>
          <td align="center">
            <xsl:call-template name="object-actions"/>
          </td>
        </tr>
      </xsl:for-each>
    </table>
  </xsl:template>

</xsl:stylesheet>
