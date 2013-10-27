<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                exclude-result-prefixes="lang"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>  
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/results.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="text"/>  
  <xsl:param name="textMode"/>
  <xsl:param name="textWhere"/>
  <xsl:param name="orderMode"/>
  <xsl:param name="orderDirection"/>
  <xsl:param name="fromSubmitDate"/>
  <xsl:param name="toSubmitDate"/>
  <xsl:param name="fromInsertDate"/>
  <xsl:param name="toInsertDate"/>
  <xsl:param name="user"/>
  <xsl:param name="parentsList"/>
  <xsl:param name="parentsRecursive"/>
  <xsl:param name="treeMode"/>
  <xsl:param name="groupMode"/>
  <xsl:param name="showDeleted"/>
  <xsl:param name="showInfo"/>
  <xsl:param name="showFilterAlphabetic"/>
  <xsl:param name="templateView"/>
  <xsl:param name="templatePager"/>
  <xsl:param name="maxResults"/>
  <xsl:param name="pagerSize"/>
  
  <xsl:template match="/">
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title" select="lang:text('search.what_options')"/>
      <xsl:with-param name="content">
        <div class="os_content">
          <xsl:value-of select="$text" disable-output-escaping="yes"/>
          <xsl:call-template name="help-tooltip">
            <xsl:with-param name="text" select="lang:text('search.text.help')"/>
            <xsl:with-param name="id" select="'8A41E8F946494467CE07B6FB8B7F9D85964448CB'"/>
          </xsl:call-template>          
          <xsl:value-of select="$textMode" disable-output-escaping="yes"/>
          <span class="os_label">
            <xsl:value-of select="lang:text('search.where')"/>
          </span>
          <xsl:value-of select="$textWhere" disable-output-escaping="yes"/>          
        </div>
      </xsl:with-param>
    </xsl:call-template>
    
    <div>
      <div style="width:50%;float:left;">
        <xsl:call-template name="block_minimal">
          <xsl:with-param name="title" select="lang:text('search.order_options')"/>
          <xsl:with-param name="content">
            <div class="os_content">
              <xsl:value-of select="$orderMode" disable-output-escaping="yes"/>
              <xsl:value-of select="$orderDirection" disable-output-escaping="yes"/>
            </div>
          </xsl:with-param>
        </xsl:call-template>
      </div>
      <div style="width:50%;float:right;">
        <xsl:call-template name="block_minimal">
          <xsl:with-param name="title" select="lang:text('search.view_options')"/>
          <xsl:with-param name="content">
            <div class="os_content">              
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.tree_mode')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$treeMode" disable-output-escaping="yes"/>
              <br />              
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.group_mode')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$groupMode" disable-output-escaping="yes"/>
              <br />              
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.template.view')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$templateView" disable-output-escaping="yes"/>              
            </div>
          </xsl:with-param>
        </xsl:call-template>
      </div>
      <div style="width:50%;float:left;">
        <xsl:call-template name="block_minimal">
          <xsl:with-param name="title" select="lang:text('search.user_options')"/>
          <xsl:with-param name="startClose" select="true()"/>
          <xsl:with-param name="content">
            <div class="os_content">
              <xsl:value-of select="$user" disable-output-escaping="yes"/>
            </div>
          </xsl:with-param>
        </xsl:call-template>
      </div>

      <div style="width:50%;float:right;">
        <xsl:call-template name="block_minimal">
          <xsl:with-param name="title" select="lang:text('search.parents')"/>
          <xsl:with-param name="startClose" select="true()"/>
          <xsl:with-param name="content">
            <div class="os_content">
              <xsl:value-of select="$parentsList" disable-output-escaping="yes"/>
              <br />              
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.parentsRecursive')"/>
              </span>
              <xsl:value-of select="$parentsRecursive" disable-output-escaping="yes"/>
            </div>
          </xsl:with-param>
        </xsl:call-template>
      </div>

      <div style="width:50%;float:left;">
        <xsl:call-template name="block_minimal">
          <xsl:with-param name="title" select="lang:text('search.dates')"/>
          <xsl:with-param name="startClose" select="true()"/>
          <xsl:with-param name="content">
            <div class="os_content">
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.fromSubmitDate')"/>
                <xsl:text>: </xsl:text>
              </span>              
              <xsl:value-of select="$fromSubmitDate" disable-output-escaping="yes"/>

              <span class="os_label_right_next">
                <xsl:value-of select="lang:text('search.toSubmitDate')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$toSubmitDate" disable-output-escaping="yes"/>

              <br />
              
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.fromInsertDate')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$fromInsertDate" disable-output-escaping="yes"/>

              <span class="os_label_right_next">
                <xsl:value-of select="lang:text('search.toInsertDate')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$toInsertDate" disable-output-escaping="yes"/>

            </div>
          </xsl:with-param>
        </xsl:call-template>
      </div>

      <div class="os_clear">
        <xsl:call-template name="block_minimal">
          <xsl:with-param name="title" select="lang:text('search.advanced')"/>
          <xsl:with-param name="startClose" select="true()"/>
          <xsl:with-param name="content">
            <div class="os_content">                            
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.show_deleted')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$showDeleted" disable-output-escaping="yes"/>
              <br />              
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.show_info')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$showInfo" disable-output-escaping="yes"/>
              <br />              
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.show_filter_alphabetic')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$showFilterAlphabetic" disable-output-escaping="yes"/>
              <br />
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.template.pager')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$templatePager" disable-output-escaping="yes"/>
              <br />
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.max_results')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$maxResults" disable-output-escaping="yes"/>
              <br />
              <span class="os_label_right">
                <xsl:value-of select="lang:text('search.pager_size')"/>
                <xsl:text>: </xsl:text>
              </span>
              <xsl:value-of select="$pagerSize" disable-output-escaping="yes"/>
            </div>
          </xsl:with-param>
        </xsl:call-template>
      </div>
    </div>

    <div class="os_clear" />

  </xsl:template>
</xsl:stylesheet>


