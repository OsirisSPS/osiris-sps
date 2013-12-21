<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="saveCommand"/>

  <xsl:param name="name"/>
  <xsl:param name="password"/>
	<xsl:param name="skin"/>
	<xsl:param name="language"/>

	
  <xsl:template match="/home">
    <xsl:call-template name="block_big">
      <xsl:with-param name="title" select="lang:text('main.pages.account.title')"/>        
      <xsl:with-param name="content">                
        <div class="os_commands_right">
          <xsl:value-of select="$saveCommand" disable-output-escaping="yes"/>
        </div>
        				

        <div style="clear:both;display:none" data-os-otype="tab" data-os-layout="left" data-os-storage="main.account">

          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('common.labels.overview')"/>
          </div>
          <div data-os-tabType="body">
            <table class="os_table_properties">              
              <tr>
                <td>
                  <xsl:value-of select="lang:text('main.pages.account.options.name.title')"/>
                  <xsl:text> :</xsl:text>
									<br/>
									<div class="os_description">
										<xsl:value-of select="lang:text('main.pages.account.options.name.description')"/>
									</div>
                </td>
                <td>
                  <xsl:value-of select="$name" disable-output-escaping="yes"/>
                </td>
              </tr>
							<tr>
								<td>
									<xsl:value-of select="lang:text('main.pages.account.options.password.title')"/>
									<xsl:text> :</xsl:text>
									<br/>
									<div class="os_description">
										<xsl:value-of select="lang:text('main.pages.account.options.password.description')"/>
									</div>
								</td>
								<td>
									<xsl:value-of select="$password" disable-output-escaping="yes"/>
								</td>
							</tr>
              

            </table>

            
          </div>

          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('main.pages.account.skin')"/>
          </div>
          <div data-os-tabType="body">
            <table class="os_table_properties">
							<tr>
								<td>
									<xsl:value-of select="lang:text('main.pages.account.options.skin')"/>
									<xsl:text> :</xsl:text>
								</td>
								<td>
									<xsl:value-of select="$skin" disable-output-escaping="yes"/>
								</td>
							</tr>
            </table>
          </div>

          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('main.pages.account.language')"/>
          </div>
          <div data-os-tabType="body">
            <table class="os_table_properties">
							<tr>
								<td>
									<xsl:value-of select="lang:text('main.pages.account.options.language')"/>
									<xsl:text> :</xsl:text>
								</td>
								<td>
									<xsl:value-of select="$language" disable-output-escaping="yes"/>
								</td>
							</tr>
            </table>
          </div>

          
          


        </div>


      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  
</xsl:stylesheet>