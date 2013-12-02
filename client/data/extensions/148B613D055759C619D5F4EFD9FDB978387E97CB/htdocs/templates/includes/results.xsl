<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  

  <!-- Temp for compatibility-->
  
  <xsl:template match="results">
    <xsl:param name="template" select="preview"/>
  
    <xsl:choose>
      <xsl:when test="$template='table'">
        <table class="os_table_data">
          <tr>            
            <th width="100%">
              <xsl:value-of select="lang:text('object.table.title')"/>
            </th>
            <th>
              <xsl:value-of select="lang:text('object.table.date')"/>
            </th>
            <th>
              <xsl:value-of select="lang:text('object.table.author')"/>
            </th>
            <th>
              <xsl:value-of select="lang:text('object.table.rating')"/>
            </th>
            <th>
              <xsl:value-of select="lang:text('object.table.comments')"/>
            </th>
          </tr>
          <xsl:apply-templates select="object" mode="table"/>
        </table>
      </xsl:when>
      <xsl:when test="$template='preview'">        
        <xsl:apply-templates select="object" mode="preview"/>
      </xsl:when>
      <xsl:when test="$template='row'">
        <xsl:apply-templates select="object" mode="row"/>
      </xsl:when>
      <xsl:when test="$template='row-detail'">
        <xsl:apply-templates select="object" mode="row-detail"/>
      </xsl:when>
      <xsl:when test="$template='row-tech'">
        <xsl:apply-templates select="object" mode="row-tech"/>
      </xsl:when>
      <xsl:when test="$template='blog'">
        <xsl:apply-templates select="object" mode="blog"/>
      </xsl:when>
      <xsl:when test="$template='blog-twocols'">
        <xsl:for-each select="object">
          <div style="float:left;width:50%">
            <xsl:apply-templates select="." mode="blog"/>
          </div>
        </xsl:for-each>        
      </xsl:when>
      <xsl:otherwise>
        <xsl:apply-templates select="object" mode="preview"/>
      </xsl:otherwise>
    </xsl:choose>

    <xsl:if test="count(object) = 0">
      <div class="os_message_nodata">
        <xsl:value-of select="lang:text('search.no_results')"/>        
      </div>
    </xsl:if>

  </xsl:template>

  <!-- Preview mode - list & tree -->  

  <!--
  <xsl:template match="object" mode="preview">
    <div>
      <xsl:if test="@isResult='false'">
        <xsl:attribute name="class">os_results_lowlight</xsl:attribute>
      </xsl:if>

      <xsl:apply-templates select="." mode="lite"/>
      <xsl:if test="objects/*">
        <div class="os_results">
          <xsl:apply-templates select="objects/object" mode="preview"/>
        </div>
      </xsl:if>
    </div>
  </xsl:template>   
  -->

</xsl:stylesheet>