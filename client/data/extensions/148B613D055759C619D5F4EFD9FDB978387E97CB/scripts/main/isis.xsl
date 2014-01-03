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
	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

	<xsl:param name="portal"/>
	<xsl:param name="name"/>
	<xsl:param name="url"/>	
	<xsl:param name="password"/>
	<xsl:param name="enabled"/>
	<xsl:param name="save"/>

  <xsl:template match="/isis">

		<xsl:call-template name="dump_xml"/>
    <xsl:call-template name="page"/>
    
  </xsl:template>


	<xsl:template name="page">

		<xsl:choose>
			<xsl:when test="@action = 'home'">
				<xsl:call-template name="block_big">
					<xsl:with-param name="title" select="lang:text('main.pages.isis.title')"/>
					<xsl:with-param name="content">
						<xsl:call-template name="help-box">
							<xsl:with-param name="text" select="lang:text('main.pages.isis.help')"/>
						</xsl:call-template>

						<table class="os_table_data">
							<tr>
								<th>Portal</th>
								<th>Name</th>
								<th>Url</th>
								<th>Enabled</th>
								<th>Last Event</th>
								<th>Actions</th>
							</tr>
							<xsl:for-each select="portals/portal">
								<xsl:for-each select="isis">
									<tr>
										<td>
											<xsl:value-of select="@portal_name"/>
											<div class="os_description">
												<xsl:value-of select="@portal_description"/>
											</div>
										</td>
										<td>
											<xsl:value-of select="@name"/>
										</td>
										<td>
											<xsl:value-of select="@url"/>
										</td>
										<td>
											<xsl:call-template name="boolean-icon">
												<xsl:with-param name="value" select="@enabled = '1'"/>
											</xsl:call-template>
										</td>
										<td>
											<xsl:value-of select="@last_event"/>
										</td>
										<td>
											<a class="os_button" href="{@edit_href}">
												<img src="/htdocs/images/icons/16x16/edit.png"/>
												Edit
											</a>
											<a class="os_button" href="{@remove_href}">
												<img src="/htdocs/images/icons/16x16/remove.png"/>
												Remove
											</a>
										</td>
									</tr>
								</xsl:for-each>
							</xsl:for-each>
						</table>
						<div class="os_commands">
							<a class="os_button" href="{@add_href}">
								<img src="/htdocs/images/icons/16x16/create.png"/>
								Add
							</a>
						</div>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="(@action = 'add') or (@action = 'edit')">

				<xsl:call-template name="block_big">
					<xsl:with-param name="title" select="lang:text(concat('main.pages.isis.',@action,'.title'))"/>
					<xsl:with-param name="content">
						<table class="os_table_properties">
							<tr>
								<td>
									<xsl:value-of select="'Portal'"/>
									<xsl:text>: </xsl:text>
								</td>
								<td>
									<xsl:value-of select="$portal" disable-output-escaping="yes"/>
								</td>
							</tr>
							<tr>
								<td>
									<xsl:value-of select="'Name'"/>
									<xsl:text>: </xsl:text>
								</td>
								<td>
									<xsl:value-of select="$name" disable-output-escaping="yes"/>
								</td>
							</tr>
							<tr>
								<td>
									<xsl:value-of select="'Url'"/>
									<xsl:text>: </xsl:text>
								</td>
								<td>
									<xsl:value-of select="$url" disable-output-escaping="yes"/>
								</td>
							</tr>
							<tr>
								<td>
									<xsl:value-of select="'Password'"/>
									<xsl:text>: </xsl:text>
								</td>
								<td>
									<xsl:value-of select="$password" disable-output-escaping="yes"/>
								</td>
							</tr>
							<tr>
								<td>
									<xsl:value-of select="'Enabled'"/>
									<xsl:text>: </xsl:text>
								</td>
								<td>
									<xsl:value-of select="$enabled" disable-output-escaping="yes"/>
								</td>
							</tr>
						</table>
						<div class="os_commands">
							<xsl:value-of select="$save" disable-output-escaping="yes"/>
						</div>
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
		</xsl:choose>
		

	</xsl:template>

  
</xsl:stylesheet>