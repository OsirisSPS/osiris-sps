<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/job_detail.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="showAll"/>

  <xsl:template match="jobs">    
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('main.pages.jobs.title')"/>      
      <xsl:with-param name="content">
        <xsl:call-template name="jobsTable"/>             
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="jobsTable">
    <div class="os_content_box">
      <table class="os_table_data">
        <tr>
          <th>
            <xsl:value-of select="lang:text('main.pages.jobs.name')"/>
          </th>
          <th width="50%">
            <xsl:value-of select="lang:text('main.pages.jobs.status')"/>
          </th>
        </tr>
        <xsl:for-each select="job">
          <xsl:sort select="@lastProgressTime" order="descending"/>
          <tr id="job_{@id}">
            <xsl:if test="@id = //@highlight">
              <xsl:attribute name="class">os_current</xsl:attribute>
            </xsl:if>
            <td>
              <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes"/>
            </td>
            <td class="os_nowrap">
              <div style="padding-right:16px">
                <xsl:if test="@completed != 'true'">
                  <xsl:attribute name="data-os-url">
                    <xsl:value-of select="concat('/main/jobdetail?id=',@id)"/>
                  </xsl:attribute>
                  <xsl:attribute name="data-os-wait">
                    <xsl:text>right</xsl:text>
                  </xsl:attribute>
                </xsl:if>

                <xsl:apply-templates select="."/>
              </div>
            </td>
          </tr>
        </xsl:for-each>
      </table>
    </div>

    <div class="os_commands_right">
      <xsl:value-of select="$showAll" disable-output-escaping="yes"/>
    </div>

    <xsl:for-each select="actions/action">
      <xsl:call-template name="action-row">
        <xsl:with-param name="prefix" select="'main.pages.jobs.actions'"/>
        <xsl:with-param name="icon" select="'clear'"/>
        <xsl:with-param name="name" select="@name"/>
        <xsl:with-param name="href" select="@href"/>
      </xsl:call-template>
    </xsl:for-each>



    <xsl:if test="//@highlight">
      <script type="text/javascript">        
        <xsl:text>Effect.ScrollTo('job_</xsl:text>
        <xsl:value-of select="//@highlight"/>
        <xsl:text>');</xsl:text>        
      </script>
    </xsl:if>    
  </xsl:template>
  
</xsl:stylesheet>
