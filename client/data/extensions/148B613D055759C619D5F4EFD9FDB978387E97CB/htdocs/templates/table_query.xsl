<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="table_query">
    <xsl:choose>
      <xsl:when test="header">
        <table class="os_table_data os_content_box" data-os-otype="table">
          <xsl:apply-templates />
        </table>
      </xsl:when>
      <xsl:otherwise>
        SQL Error.
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
 
  <xsl:template match="header"> 
    <thead>
      <tr>
        <xsl:apply-templates select="column"/>            
      </tr>
    </thead>
  </xsl:template>

  <xsl:template match="column">
    <th>
      <xsl:value-of select="@name"/>
    </th>
  </xsl:template>
  
  <xsl:template match="data">
    <tbody>
      <xsl:apply-templates select="row"/>
    </tbody>
  </xsl:template>

  <xsl:template match="row">
    <tr>
      <xsl:apply-templates select="item"/>
    </tr>
  </xsl:template>
  
  <xsl:template match="item">
    <td>
      <xsl:variable name="cur_pos" select="position()"/>
      
      <xsl:variable name="type" select = "/table_query/header/column[$cur_pos]/@type"/>
      
      <xsl:choose>
        <xsl:when test="$type='id'">
          <xsl:choose>
            <xsl:when test="@type = 'user'">
              <xsl:call-template name="user-row"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:call-template name="object-row-tech"/>
            </xsl:otherwise>
          </xsl:choose>          
        </xsl:when>
        <xsl:when test="$type='short-datetime'">
          <xsl:value-of select="date:short-datetime(@value)"/>
        </xsl:when>	
        <xsl:when test="$type='long-datetime'">
          <xsl:value-of select="date:long-datetime(@value)"/>          
        </xsl:when>
        <xsl:when test="$type='score'">
          <img class="os_reputation" src="{system:resource-url(concat('images/reputation_score/dot/',@value,'.png'))}" title="{@value}" alt="{@value}"/>
          <xsl:value-of select="@value"/>
        </xsl:when>        
        <xsl:otherwise>
          <xsl:value-of select="@value"/>
        </xsl:otherwise>
      </xsl:choose>
      
    </td>
  </xsl:template>

</xsl:stylesheet>


