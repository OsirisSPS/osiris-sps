<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/debug.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="name"/>
  <xsl:param name="password"/>
  <xsl:param name="passwordChecker"/>
  <xsl:param name="savePassword"/>
  <xsl:param name="register"/>
	<xsl:param name="login"/>

	<xsl:param name="file"/>
	<xsl:param name="import"/>
  

  <xsl:template match="/accounts">

		<xsl:choose>
			<xsl:when test="@action = 'home'">
				<xsl:call-template name="block_big">
					<xsl:with-param name="title" select="lang:text('main.pages.accounts.title')" />
					<xsl:with-param name="content">
						<xsl:call-template name="help-box">
							<xsl:with-param name="text" select="lang:text('main.pages.accounts.help')" />
						</xsl:call-template>

						<div style="float:right;width:500px">
							<xsl:call-template name="actions-row">
								<xsl:with-param name="prefix" select="'main.pages.accounts'"/>
							</xsl:call-template>
						</div>

						<xsl:choose>
							<xsl:when test="accounts/account"></xsl:when>
							<xsl:otherwise>
								<div class="os_message_nodata">
									<xsl:value-of select="lang:text('main.pages.accounts.empty')"/>
								</div>
							</xsl:otherwise>
						</xsl:choose>

						<xsl:for-each select="accounts/account">
							<xsl:call-template name="block_item">
								<xsl:with-param name="style" select="'display:inline-block;width:300px;'"/>
								<xsl:with-param name="title">
									<xsl:value-of select="@name"/>
								</xsl:with-param>
								<xsl:with-param name="content">
									<table class="os_table_properties">
										<tr>
											<td style="width:40%">
												<xsl:value-of select="lang:text('main.pages.accounts.last_access')"/>
												<xsl:text>: </xsl:text>
											</td>
											<td>
												<xsl:value-of select="date:userfriendly-datetime(@last_access)"/>
											</td>
										</tr>
										<tr>
											<td>
												<xsl:value-of select="lang:text('main.pages.accounts.saved_password')"/>
												<xsl:text>: </xsl:text>
											</td>
											<td>
												<xsl:call-template name="boolean-icon">
													<xsl:with-param name="value" select="@saved_password"/>
												</xsl:call-template>
											</td>
										</tr>
										<tr>
											<td>
												<xsl:value-of select="lang:text('main.pages.accounts.connected')"/>
												<xsl:text>: </xsl:text>
											</td>
											<td>
												<xsl:call-template name="boolean-icon">
													<xsl:with-param name="value" select="@logged"/>
												</xsl:call-template>
											</td>
										</tr>
									</table>
									<div class="os_commands">
										<a class="os_button" style="display:block;" href="{actions/action[@name='enter']/@href}">
											<img src="{system:resource-url('images/icons/16x16/enter.png')}"/>
											<xsl:value-of select="lang:text('main.pages.accounts.enter')"/>											
										</a>
										<a class="os_button" style="display:block;" href="{actions/action[@name='export']/@href}">
											<img src="{system:resource-url('images/icons/16x16/export.png')}"/>
											<xsl:value-of select="lang:text('main.pages.accounts.export')"/>
										</a>
										<a class="os_button" style="display:block;" href="{actions/action[@name='remove']/@href}">
											<img src="{system:resource-url('images/icons/16x16/remove.png')}"/>
											<xsl:value-of select="lang:text('main.pages.accounts.remove')"/>
										</a>
									</div>
								</xsl:with-param>
							</xsl:call-template>
						</xsl:for-each>

						
					</xsl:with-param>
				</xsl:call-template>
			</xsl:when>
			<xsl:when test="@action = 'login'">
				<div style="width:750px;margin:auto;">
					<xsl:call-template name="block">
						<xsl:with-param name="title" select="lang:text('main.pages.accounts.login.title')" />
						<xsl:with-param name="content">							
							<table class="os_table_properties">
								<tr>
									<td>
										<xsl:value-of select="lang:text('common.labels.password')"/>
										<xsl:text> :</xsl:text>
									</td>
									<td>
										<xsl:value-of select="$password" disable-output-escaping="yes"/>
									</td>
								</tr>
								<tr>
									<td>
										<xsl:value-of select="lang:text('common.labels.password_remember')"/>
										<xsl:text> :</xsl:text>
									</td>
									<td>
										<xsl:value-of select="$savePassword" disable-output-escaping="yes"/>
									</td>
								</tr>
							</table>


							<div class="os_commands">
								<xsl:value-of select="$login" disable-output-escaping="yes"/>
							</div>
						</xsl:with-param>
					</xsl:call-template>
				</div>
			</xsl:when>
			<xsl:when test="@action = 'register'">
				<div style="width:750px;margin:auto;">
					<xsl:call-template name="block">
						<xsl:with-param name="title" select="lang:text('main.pages.accounts.register.title')" />
						<xsl:with-param name="content">
							<xsl:call-template name="help-box">
								<xsl:with-param name="text" select="lang:text('main.pages.accounts.register.help')"/>
							</xsl:call-template>
							<table>
								<tr>
									<td class="os_padding">
										<img src="{system:resource-url('images/icons/48x48/register.png')}"/>
									</td>
									<td>
										<table class="os_table_properties">
											<tr>
												<td>
													<xsl:value-of select="lang:text('main.pages.accounts.name')"/>
													<xsl:text> :</xsl:text>
												</td>
												<td>
													<xsl:value-of select="$name" disable-output-escaping="yes"/>
												</td>
											</tr>
											<tr>
												<td>
													<xsl:value-of select="lang:text('common.labels.password')"/>
													<xsl:text> :</xsl:text>
												</td>
												<td>
													<xsl:value-of select="$password" disable-output-escaping="yes"/>
												</td>
											</tr>
											<tr>
												<td>
													<xsl:value-of select="lang:text('common.labels.password_confirm')"/>
													<xsl:text> :</xsl:text>
												</td>
												<td>
													<xsl:value-of select="$passwordChecker" disable-output-escaping="yes"/>
												</td>
											</tr>
											<tr>
												<td>
													<xsl:value-of select="lang:text('common.labels.password_remember')"/>
													<xsl:text> :</xsl:text>
												</td>
												<td>
													<xsl:value-of select="$savePassword" disable-output-escaping="yes"/>
												</td>
											</tr>
										</table>
									</td>
								</tr>
							</table>

							<div class="os_commands">
								<xsl:value-of select="$register" disable-output-escaping="yes"/>								
							</div>
						</xsl:with-param>
					</xsl:call-template>
				</div>
			</xsl:when>

			<xsl:when test="@action = 'import'">
				<div style="width:750px;margin:auto;">
					<xsl:call-template name="block">
						<xsl:with-param name="title" select="lang:text('main.pages.accounts.import.title')" />
						<xsl:with-param name="content">							
							<table class="os_table_properties">
								<tr>
									<td>
										<xsl:value-of select="lang:text('main.pages.accounts.import.file')"/>
										<xsl:text>: </xsl:text>
									</td>
									<td>
										<xsl:value-of select="$file" disable-output-escaping="yes"/>
									</td>
								</tr>
							</table>
							<div class="os_commands">
								<xsl:value-of select="$import" disable-output-escaping="yes"/>
							</div>
						</xsl:with-param>
					</xsl:call-template>
				</div>
			</xsl:when>
			
		</xsl:choose>
        
  </xsl:template>
  
</xsl:stylesheet>