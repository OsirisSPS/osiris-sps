<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang date system"
                version="1.0">

<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
<xsl:output method="html"/>  

<xsl:template match="page">
	<div style="width:400px;margin:auto;">
		<xsl:call-template name="block">
			<xsl:with-param name="title" select="lang:text('main.pages.redirect.title')"/>
			<xsl:with-param name="content">
				<div class="os_padding os_center">
					<img src="{system:resource-url('images/redirect.gif')}"/>
					<br/>
					<a href="{@url}">
						<xsl:value-of select="lang:text('main.pages.redirect.skip_link')"/>
					</a>
				</div>
			</xsl:with-param>
		</xsl:call-template>
	</div>
</xsl:template>
	
</xsl:stylesheet>