<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="os lang system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>  
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/results.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/debug.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/">
    <xsl:if test="results/@showFilterAlphabetic = 'true'">
      <xsl:for-each select="results">        
        <xsl:call-template name="filter-alphabetic"/>
      </xsl:for-each>
    </xsl:if>
    
    <xsl:apply-templates select="results">
      <xsl:with-param name="template" select="results/@templateView"/>
    </xsl:apply-templates>

    <xsl:if test="results/@showInfo = 'true' or results/@rssUrl">
      <div class="os_results_info">
        <xsl:if test="results/@showInfo = 'true'">
          <xsl:if test="results/@query">
            <span class="os_label">
              <xsl:value-of select="lang:text('search.query')"/>
              <xsl:text>: </xsl:text>
            </span>
            <span class="os_value">
              <xsl:value-of select="results/@query"/>
            </span>
          </xsl:if>
          <xsl:call-template name="separator"/>
          <span class="os_label">
            <xsl:value-of select="lang:text('search.elapsed')"/>
            <xsl:text>: </xsl:text>
          </span>
          <span class="os_value">
            <xsl:value-of select="results/@elapsed"/>
            <xsl:text> </xsl:text>
            <xsl:value-of select="lang:text('datetime.timespan.seconds')"/>
          </span>
          <xsl:if test="results/@partial = 'true'">
            <xsl:call-template name="separator"/>
            <span class="os_label">
              <xsl:value-of select="lang:text('search.partial')"/>
            </span>
            <xsl:call-template name="help-tooltip">
              <xsl:with-param name="text" select="lang:text('search.partial.help')"/>
            </xsl:call-template>
          </xsl:if>
        </xsl:if>
        
        <xsl:if test="results/@rssUrl">
          <xsl:call-template name="separator"/>
          <a class="os_nolink" href="{results/@rssUrl}">
            <img src="{system:resource-url('images/feed.png')}"/>
          </a>          
        </xsl:if>
      </div>
    </xsl:if>

    <xsl:choose>
      <xsl:when test="results/@templatePager = 'forum'">
        <xsl:apply-templates select="results/pager" mode="forum"/>
      </xsl:when>
      <xsl:when test="results/@templatePager = 'blog'">
        <xsl:apply-templates select="results/pager" mode="blog"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="results/pager"/>
      </xsl:otherwise>
    </xsl:choose>


  </xsl:template>
</xsl:stylesheet>


