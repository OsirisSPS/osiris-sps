<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

	<xsl:param name="save"/>
	<xsl:param name="text"/>
	<xsl:param name="boolean"/>

  <xsl:template match="/skinpreview">

    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('main.pages.skinpreview.title')}" data-os-otype="dialog" data-os-dialog-width="50%" >
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="page"/>        
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

	<xsl:template name="page">
		<div style="clear:both;display:none" data-os-otype="tab" data-os-layout="left" data-os-storage="portal.acp">
			<div data-os-place="below-tabs" style="padding-top:3em;width:350px;">

				<xsl:call-template name="action-row">
					<xsl:with-param name="prefix" select="'main.pages'"/>
					<xsl:with-param name="icon" select="'home'"/>
					<xsl:with-param name="name" select="'home'"/>
					<xsl:with-param name="href" select="/"/>
				</xsl:call-template>

				<xsl:call-template name="action-row">
					<xsl:with-param name="prefix" select="'main.pages'"/>
					<xsl:with-param name="icon" select="'about'"/>
					<xsl:with-param name="name" select="'about'"/>
					<xsl:with-param name="href" select="/main/about"/>
				</xsl:call-template>

			</div>
			<div data-os-tabType="header">
				<xsl:value-of select="'First tab'"/>
			</div>
			<div data-os-tabType="body">
				<xsl:call-template name="help-box">
					<xsl:with-param name="text" select="'Sample page for skin developing and screenshot'"/>
				</xsl:call-template>

				<div style="float:right;width:200px;">
					<xsl:call-template name="block_big">
						<xsl:with-param name="title" select="'Sample Big Block'"/>
						<xsl:with-param name="content">
							<div class="os_padding">
								Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
							</div>
						</xsl:with-param>
					</xsl:call-template>
				</div>

				<div style="float:right;width:200px;">
					<xsl:call-template name="block_small">
						<xsl:with-param name="title" select="'Sample Small Block'"/>
						<xsl:with-param name="content">
							<div class="os_padding">
								Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. 
							</div>
							<div>
								<xsl:call-template name="block_small">
									<xsl:with-param name="title" select="'Sample Small Block'"/>
									<xsl:with-param name="content">
										<div class="os_padding">
											Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. 
										</div>
									</xsl:with-param>
								</xsl:call-template>
							</div>
						</xsl:with-param>
					</xsl:call-template>
				</div>

				<div style="overflow:auto">

					<div class="os_padding">
						<h2>Properties</h2>
					</div>
					<table class="os_table_properties">
						<tr>
							<td>
								Text :
							</td>
							<td style="width:80%">
								<xsl:text>
								Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
							</xsl:text>
							</td>
						</tr>
						<tr>
							<td>
								Input :
							</td>
							<td>
								<xsl:value-of select="$text" disable-output-escaping="yes"/>
							</td>
						</tr>
						<tr>
							<td>
								Boolean :
							</td>
							<td>
								<xsl:value-of select="$boolean" disable-output-escaping="yes"/>
							</td>
						</tr>
					</table>

					<div class="os_commands">
						<xsl:value-of select="$save" disable-output-escaping="yes"/>
					</div>

				</div>

			</div>

			<div data-os-tabType="header">
				<xsl:value-of select="'Second tab'"/>
			</div>
			<div data-os-tabType="body">				
			</div>

			<div data-os-tabType="header">
				<xsl:value-of select="'Third tab'"/>
			</div>
			<div data-os-tabType="body">
			</div>
		</div>
	</xsl:template>

  
</xsl:stylesheet>