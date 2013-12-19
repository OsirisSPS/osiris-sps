<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>

  <xsl:template match="job">
    <div>
      <xsl:choose>
        <xsl:when test="@completed != 'true'">
          <xsl:attribute name="data-os-refresh">
            <xsl:value-of select="3"/>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="data-os-completed"/>            
        </xsl:otherwise>
      </xsl:choose>
      
      <xsl:variable name="perc">
        <xsl:choose>
          <xsl:when test="@completed = 'true'">
            <xsl:text>100</xsl:text>
          </xsl:when>
          <xsl:when test="@running != 'true'">
            <xsl:text>-1</xsl:text>
          </xsl:when>
          <xsl:when test="@percentage">
            <xsl:value-of select="round(@percentage * 100)"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>0</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>

      <div style="margin:10px;">
        <xsl:call-template name="progressbar">
          <xsl:with-param name="perc" select="$perc"/>
          <xsl:with-param name="title" select="@status"/>
        </xsl:call-template>
      </div>

      <xsl:if test="@result">
        <xsl:value-of select="system:parse(@result)" disable-output-escaping="yes"/>        
      </xsl:if>


    </div>    
  </xsl:template>
</xsl:stylesheet>