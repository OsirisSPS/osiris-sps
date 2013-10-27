<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="jobs">
    <xsl:call-template name="block_big">
      <xsl:with-param name="title">
        <xsl:value-of select="lang:text('main.pages.jobs.title')"/>
      </xsl:with-param>
      <xsl:with-param name="content">
        <xsl:call-template name="jobsTable"/>             
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="jobsTable">
    <table class="os_table_data os_content_box">
      <th>
        <xsl:value-of select="lang:text('main.pages.jobs.name')"/>
      </th>
      <th>
        <xsl:value-of select="lang:text('main.pages.jobs.completed')"/>
      </th>
      <th>
        <xsl:value-of select="lang:text('main.pages.jobs.percentage')"/>
      </th>
      <xsl:for-each select="job">
        <tr>
          <td>
            <xsl:value-of select="@name"/>
          </td>
          <td>
            <xsl:value-of select="@completed"/>
          </td>
          <td>
            <xsl:choose>
              <xsl:when test="@percentage">
                <xsl:value-of select="concat(round(@percentage * 100), '%')"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="lang:text('main.pages.jobs.percentage.unknown')"/>
              </xsl:otherwise>
            </xsl:choose>
          </td>
        </tr>
      </xsl:for-each>
    </table>
  </xsl:template>
  
</xsl:stylesheet>
