<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
	<xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>

<xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="action">
    <xsl:param name="href"/>
    <xsl:param name="name"/>
    
		<xsl:call-template name="action-row">
			<xsl:with-param name="prefix" select="'main.pages.maintenance.actions'"/>
			<xsl:with-param name="name" select="$name"/>
			<xsl:with-param name="icon" select="'maintenance'"/>
			<xsl:with-param name="href" select="$href"/>
		</xsl:call-template>
  </xsl:template>

  <xsl:template match="maintenance">
		<div style="text-align:center">
			<div style="width:700px;margin:auto;">
				<xsl:call-template name="block_big">
					<xsl:with-param name="title">
						<xsl:value-of select="lang:text('main.pages.maintenance.title')"/>
					</xsl:with-param>
					<xsl:with-param name="content">
						<xsl:call-template name="help-box">
							<xsl:with-param name="text" select="lang:text('main.pages.maintenance.description')"/>
						</xsl:call-template>
						<xsl:call-template name="action">
							<xsl:with-param name="href" select="@stability_restart_href"/>
							<xsl:with-param name="name" select="'stability_restart'"/>
						</xsl:call-template>
						<xsl:call-template name="action">
							<xsl:with-param name="href" select="@stability_rebuild_href"/>
							<xsl:with-param name="name" select="'stability_rebuild'"/>
						</xsl:call-template>
						<xsl:call-template name="action">
							<xsl:with-param name="href" select="@lucene_rebuild_href"/>
							<xsl:with-param name="name" select="'lucene_rebuild'"/>
						</xsl:call-template>
						<xsl:call-template name="action">
							<xsl:with-param name="href" select="@lucene_optimize_href"/>
							<xsl:with-param name="name" select="'lucene_optimize'"/>
						</xsl:call-template>
						<xsl:call-template name="action">
							<xsl:with-param name="href" select="@database_vacuum_href"/>
							<xsl:with-param name="name" select="'database_vacuum'"/>
						</xsl:call-template>
						<xsl:call-template name="action">
							<xsl:with-param name="href" select="@database_rebuild_indexes_href"/>
							<xsl:with-param name="name" select="'database_rebuild_indexes'"/>
						</xsl:call-template>
						<xsl:call-template name="action">
							<xsl:with-param name="href" select="@database_analyze_href"/>
							<xsl:with-param name="name" select="'database_analyze'"/>
						</xsl:call-template>
					</xsl:with-param>
				</xsl:call-template>
			</div>
		</div>
  </xsl:template>
  
</xsl:stylesheet>