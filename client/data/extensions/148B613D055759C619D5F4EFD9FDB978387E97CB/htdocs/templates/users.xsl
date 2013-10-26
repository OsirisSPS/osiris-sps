<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="nickMode"/>
  <xsl:param name="nickName"/>
  <xsl:param name="orderMode"/>
  <xsl:param name="orderDirection"/>
  <xsl:param name="template"/>
  <xsl:param name="pagerSize"/>
  <xsl:param name="submit"/>
  
  <xsl:template match="users">

    <xsl:call-template name="block_page">
      <xsl:with-param name="prefix" select="'portal.pages.users'"/>
      <xsl:with-param name="content">
        



        <xsl:apply-templates select="pager" mode="row"/>

        <div style="clear:both;"/>

        <xsl:call-template name="filter-alphabetic"/>

            <xsl:if test="count(user) = 0">
              <div class="os_message_nodata">
                <xsl:value-of select="lang:text('portal.pages.users.no_results')" />
              </div>
            </xsl:if>

            <xsl:choose>
              <xsl:when test="@template='table'">
                <xsl:call-template name="users-table"/>
              </xsl:when>
              <xsl:when test="@template='box'">
                <xsl:call-template name="users-boxes"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="users-table"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:with-param>
        </xsl:call-template>

        <xsl:apply-templates select="pager" mode="row"/>


    <xsl:call-template name="block_small">
      <xsl:with-param name="title">
        <xsl:value-of select="lang:text('portal.pages.users.options')"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <div class="os_content">
          <div style="width:35%;float:left;">
            <xsl:call-template name="block_minimal">
              <xsl:with-param name="title" select="lang:text('search.what_options')"/>
              <xsl:with-param name="content">
                <div class="os_content">
                  <xsl:value-of select="$nickMode" disable-output-escaping="yes"/>
                  <xsl:value-of select="$nickName" disable-output-escaping="yes"/>
                </div>
              </xsl:with-param>
            </xsl:call-template>
          </div>

          <div style="width:25%;float:left;">
            <xsl:call-template name="block_minimal">
              <xsl:with-param name="title" select="lang:text('search.view_options')"/>
              <xsl:with-param name="content">
                <div class="os_content">
                  <span class="os_label">
                    <xsl:value-of select="lang:text('portal.pages.users.search.pager_size')"/>
                  </span>
                  <xsl:value-of select="$pagerSize" disable-output-escaping="yes"/>
                  <br/>
                  <span class="os_label">
                    <xsl:value-of select="lang:text('portal.pages.users.search.template')"/>
                  </span>
                  <xsl:value-of select="$template" disable-output-escaping="yes"/>
                </div>
              </xsl:with-param>
            </xsl:call-template>
          </div>

          <div style="width:40%;float:left;">
            <xsl:call-template name="block_minimal">
              <xsl:with-param name="title" select="lang:text('search.order_mode')"/>
              <xsl:with-param name="content">
                <div class="os_content">
                  <xsl:value-of select="$orderMode" disable-output-escaping="yes"/>
                  <xsl:value-of select="$orderDirection" disable-output-escaping="yes"/>
                </div>
              </xsl:with-param>
            </xsl:call-template>
          </div>

          <div class="os_clear" />

          <div class="os_commands">
            <xsl:value-of select="$submit" disable-output-escaping="yes"/>
          </div>
        </div>
      </xsl:with-param>
    </xsl:call-template>

  </xsl:template>
  
  <!-- Table view -->

  <xsl:template name="users-table">
    <table class="os_table_data">
      <tr>
        <th>
          
        </th>
        <th>
          <xsl:value-of select="lang:text('user.name')"/>
        </th>
        <th class="os_center os_nowrap">
          <xsl:value-of select="lang:text('user.reputation')"/>
        </th>
        <th class="os_center os_nowrap">
          <xsl:value-of select="lang:text('user.join_date')"/>
        </th>
        <th class="os_center os_nowrap">
          <xsl:value-of select="lang:text('user.stats.objects')"/>
        </th>
        <th class="os_center os_nowrap">
          <xsl:value-of select="lang:text('user.stats.revisions')"/>
        </th>
      </tr>
      <xsl:for-each select="user">
        <xsl:call-template name="user-table"/>
      </xsl:for-each>      
    </table>
  </xsl:template>

  <xsl:template name="user-table">
    <tr>
      <td width="32px">
        <xsl:call-template name="avatar-small">
          <xsl:with-param name="width" select="32"/>
        </xsl:call-template>
      </td>
      <td width="100%">
        <b>
          <a href="{@view_href}" style="white-space: nowrap;">
            <xsl:value-of select="@name"/>
          </a>
        </b>
      </td>
      <td class="os_center">
        <xsl:call-template name="reputation-line"/>
        <xsl:if test="@score_comment != ''">
          <br />
          <span style="font-size:0.8em;color:gray;">
            <xsl:value-of select="@score_comment"/>
          </span>
        </xsl:if>
      </td>
      <td class="os_nowrap os_center">
        <xsl:value-of select="date:userfriendly-datetime(@join_date)"/>
      </td>
      <td class="os_center">
        <xsl:value-of select="stats/@objects"/>
      </td>
      <td class="os_center">
        <xsl:value-of select="stats/@revisions"/>
      </td>
    </tr>
  </xsl:template>
  
  <!-- Boxes view - Floating -->
  
  <xsl:template name="users-boxes">
    <xsl:for-each select="user">
      <xsl:call-template name="user-boxes"/>
    </xsl:for-each>

    <div class="os_clear"/>
  </xsl:template>

  <xsl:template name="user-boxes">
    <div style="float:left;">
      <xsl:call-template name="user-box"/>
    </div>    
  </xsl:template>

  <!-- Boxes view - Table -->

  <xsl:template name="users-boxes-table">
    <table style="width:100%">
      <tr>
        <xsl:for-each select="user">
          <xsl:call-template name="user-boxes-table"/>
          <xsl:if test="position() mod 4 = 0">
            <xsl:value-of select="'&lt;/tr&gt;&lt;tr&gt;'" disable-output-escaping="yes"/>
          </xsl:if>
        </xsl:for-each>
      </tr>
    </table>
  </xsl:template>

  <xsl:template name="user-boxes-table">
    <td>
      <xsl:call-template name="user-box"/>
    </td>    
  </xsl:template>

  

</xsl:stylesheet>