<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/debug.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="save"/>
	<xsl:param name="exchange"/>
	<xsl:param name="password"/>
	<xsl:param name="sync"/>

  <xsl:template match="/info">
		<xsl:call-template name="dump_xml"/>
    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('portal.pages.info.title')}" data-os-otype="dialog" data-os-dialog-width="60%">
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block_big">
          <xsl:with-param name="title" select="lang:text('portal.pages.info.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="page">

		<div style="clear:both;display:none" data-os-otype="tab" data-os-layout="left" data-os-storage="portal.acp">
			<div data-os-place="below-tabs" style="padding-top:3em;width:400px;">
					<xsl:call-template name="actions-row">
						<xsl:with-param name="prefix" select="'portal.pages'"/>
					</xsl:call-template>
				
			</div>
			<div data-os-tabType="header">
				<xsl:value-of select="lang:text('common.labels.overview')"/>
			</div>
			<div data-os-tabType="body">
				<div class="os_padding">
					<p>
						<span class="os_title" style="font-size:1.3em" href="{@href}">
							<xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes" />
						</span>
					</p>
					<p>
						<span class="os_description">
							<xsl:value-of select="system:parse(@description, false(), false(), true())" disable-output-escaping="yes" />
						</span>
					</p>
				</div>
				<table class="os_table_properties">
					<tr>
						<td>
							Align :
						</td>
						<td>
							<xsl:value-of select="@nodesSameAlign"/>
							<xsl:text> / </xsl:text>
							<xsl:value-of select="@nodesSamePortalPov"/>
						</td>
					</tr>					
					<tr>
						<td>
							Last object date :
						</td>
						<td>
							<xsl:value-of select="date:userfriendly-datetime(@lastObjectDate)" />
						</td>
					</tr>
					<tr>
						<td>
							Last exchange date :
						</td>
						<td>
							<xsl:value-of select="date:userfriendly-datetime(@lastExchangeDate)" />
						</td>
					</tr>
					<tr>
						<td>
							Last downloaded object date :
						</td>
						<td>
							<xsl:value-of select="date:userfriendly-datetime(@lastDownloadedObjectDate)" />
						</td>
					</tr>
					<tr>
						<td>
							Last uploaded object date :
						</td>
						<td>
							<xsl:value-of select="date:userfriendly-datetime(@lastUploadedObjectDate)" />
						</td>
					</tr>
					<tr>
						<td>
							Last optimization date :
						</td>
						<td>
							<xsl:value-of select="date:userfriendly-datetime(@lastOptimizationDate)" />
						</td>
					</tr>					
					
				</table>

				<div class="os_padding">
					<h2>Isis Subscriptions</h2>
				</div>

				<xsl:for-each select="isis/isis">					
					<table class="os_table_properties">						
						<tr>
							<td>
								<xsl:value-of select="'Name'"/>
								<xsl:text>: </xsl:text>
							</td>
							<td>
								<xsl:value-of select="@name"/>
							</td>
						</tr>
						<tr>
							<td>
								<xsl:value-of select="'Url'"/>
								<xsl:text>: </xsl:text>
							</td>
							<td>
								<xsl:value-of select="@url"/>
							</td>
						</tr>
						<tr>
							<td>
								<xsl:value-of select="'Enabled'"/>
								<xsl:text>: </xsl:text>
							</td>
							<td>
								<xsl:call-template name="boolean-icon">
									<xsl:with-param name="value" select="@enabled = '1'"/>
								</xsl:call-template>
							</td>
						</tr>
						<tr>
							<td colspan="2" class="os_separator"/>
						</tr>
					</table>
				</xsl:for-each>
			</div>

			<div data-os-tabType="header">
				<xsl:value-of select="lang:text('portal.pages.info.invite')"/>
			</div>
			<div data-os-tabType="body">
				<xsl:call-template name="help-box">
					<xsl:with-param name="text" select="lang:text('portal.pages.info.invite.help')"/>
				</xsl:call-template>
				<xsl:call-template name="action-row">
					<xsl:with-param name="prefix" select="'portal.pages.info.invite'"/>
					<xsl:with-param name="icon" select="'export'"/>
					<xsl:with-param name="name" select="'export_link'"/>
					<xsl:with-param name="href" select="@export_href"/>
				</xsl:call-template>
				<xsl:call-template name="action-row">
					<xsl:with-param name="prefix" select="'portal.pages.info.invite'"/>
					<xsl:with-param name="icon" select="'invite'"/>
					<xsl:with-param name="name" select="'osiris_invite_link'"/>
					<xsl:with-param name="href">
						<xsl:call-template name="copy-paste-link">
							<xsl:with-param name="text" select="@subscribe_href"/>
						</xsl:call-template>
					</xsl:with-param>
				</xsl:call-template>
				<xsl:call-template name="action-row">
					<xsl:with-param name="prefix" select="'portal.pages.info.invite'"/>
					<xsl:with-param name="icon" select="'invite'"/>
					<xsl:with-param name="name" select="'isis_invite_link'"/>
					<xsl:with-param name="href">
						<xsl:call-template name="copy-paste-link">
							<xsl:with-param name="text" select="@isis_subscribe_href"/>
						</xsl:call-template>
					</xsl:with-param>
				</xsl:call-template>
			</div>

			<div data-os-tabType="header">
				<xsl:value-of select="lang:text('portal.pages.info.settings')"/>
			</div>
			<div data-os-tabType="body">
				<div class="os_commands_right">
					<xsl:value-of select="$save" disable-output-escaping="yes"/>
				</div>
				<table class="os_table_properties">
					<tr>
						<td>
							<div>
								<xsl:value-of select="lang:text('portal.pages.info.settings.exchange.title')"/>
								<xsl:text>: </xsl:text>
							</div>
							<div class="os_description">
								<xsl:value-of select="lang:text('portal.pages.info.settings.exchange.description')"/>
							</div>
						</td>
						<td>
							<xsl:value-of select="$exchange" disable-output-escaping="yes"/>
						</td>
					</tr>
					<tr>
						<td>
							<div>
								<xsl:value-of select="lang:text('portal.pages.info.settings.password.title')"/>
								<xsl:text>: </xsl:text>
							</div>
							<div class="os_description">
								<xsl:value-of select="lang:text('portal.pages.info.settings.password.description')"/>
							</div>
						</td>
						<td>
							<xsl:value-of select="$password" disable-output-escaping="yes"/>
						</td>
					</tr>
					<tr>
						<td>
							<div>
								<xsl:value-of select="lang:text('portal.pages.info.settings.sync.title')"/>
								<xsl:text>: </xsl:text>
							</div>
							<div class="os_description">
								<xsl:value-of select="lang:text('portal.pages.info.settings.sync.description')"/>
							</div>							
						</td>
						<td>
							<xsl:value-of select="$sync" disable-output-escaping="yes"/>
							<br/>
							<xsl:value-of select="'Last object - Insert Date'"/>
							<xsl:text>: </xsl:text>
							<xsl:value-of select="date:userfriendly-datetime(@lastSyncDate)" />
						</td>
					</tr>
				</table>
			</div>
		</div>
	 
  </xsl:template>


</xsl:stylesheet>