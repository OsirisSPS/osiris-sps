<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/help">

    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('main.pages.help.title')}" data-os-otype="dialog" data-os-dialog-width="50%" >
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block_big">
          <xsl:with-param name="title" select="lang:text('main.pages.help.title')"/>
          <xsl:with-param name="content">						
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>
  

  <xsl:template name="page">
		<div class="os_padding">
			<div style="text-align:center;">
				<img src="{system:resource-url('images/logo/logo1.png')}" alt="Osiris"/>
				<br/>
				<xsl:value-of select="@version"/>
				<p>
					<a class="os_button" title="{lang:text('systembar.actions.help.about')}" href="{@href_about}">
						<xsl:value-of select="lang:text('systembar.actions.help.about')"/>
					</a>
					<xsl:call-template name="separator" />
					<a class="os_button" title="{lang:text('systembar.actions.help.homepage')}" href="{@href_home}" target="_blank">
						<xsl:value-of select="lang:text('systembar.actions.help.homepage')"/>
						<xsl:text> </xsl:text>
						<img src="{system:resource-url('images/link_external.png')}" />
					</a>
					<xsl:call-template name="separator" />
					<a class="os_button" title="{lang:text('systembar.actions.help.forum')}" href="{@href_forum}" target="_blank">
						<xsl:value-of select="lang:text('systembar.actions.help.forum')"/>
						<xsl:text> </xsl:text>
						<img src="{system:resource-url('images/link_external.png')}" />
					</a>
				</p>
			</div>			
		</div>
  </xsl:template>

  
</xsl:stylesheet>