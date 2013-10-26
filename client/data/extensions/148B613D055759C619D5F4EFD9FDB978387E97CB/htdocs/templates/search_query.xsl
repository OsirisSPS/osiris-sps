<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                exclude-result-prefixes="lang"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>  
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/results.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="globalParams"/>
  <xsl:param name="results"/>
    
  <xsl:template match="query">
    <xsl:if test="$globalParams">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('search.options')"/>
        <xsl:with-param name="startClose">
          <xsl:choose>
            <xsl:when test="$results">
              <xsl:value-of select="'true'"/>  
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="'false'"/>
            </xsl:otherwise>
          </xsl:choose>          
        </xsl:with-param>        
        <xsl:with-param name="content">
          <xsl:value-of select="$globalParams" disable-output-escaping="yes"/>
          <div class="os_commands">
            <a href="{@submit_href}" class="os_button">
              <xsl:value-of select="lang:text('search.submit')"/>
            </a>            
          </div>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>

    <xsl:if test="$results">
      <xsl:choose>
        <xsl:when test="$globalParams">
          <xsl:call-template name="block">
            <xsl:with-param name="title" select="lang:text('search.results')"/>
            <xsl:with-param name="content">
              <xsl:value-of select="$results" disable-output-escaping="yes"/>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:when>
        <xsl:otherwise>
          <xsl:value-of select="$results" disable-output-escaping="yes"/>
        </xsl:otherwise>
      </xsl:choose>
    </xsl:if>    

  </xsl:template>
</xsl:stylesheet>


