<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <!--
  <xsl:template match="importerItem">
    <a href="run_href" style="float:right;" class="os_button">Run Now</a>
    <div>
      <xsl:value-of select="'Name:'"/>
      <xsl:value-of select="$name"/>
      <xsl:value-of select="'Url:'"/>
      <xsl:value-of select="$url"/>
      Scheduled: bool - Next date: date

      Last execution: date, x objects imported, x invalid, x skipped.
    </div>
  </xsl:template>
  -->

	<xsl:template match="/page">
		<xsl:call-template name="help-box">
			<xsl:with-param name="text" select="lang:text('portal.pages.importer.help')"/>
		</xsl:call-template>

		<div class="os_item_box">
			<a style="float:right;" class="os_button">Run Now</a>
			<div>Name: XX - Url: XX</div>
			Scheduled: bool - Next date: date

			Last execution: date, x objects imported, x invalid, x skipped.

			<!--
        Ogni tot, se QUELL'IMPORTER non è in esecuzione, lo lancia.
        Al termine dell'esecuzione, setta il next_date al "next-suggest" dell'xml; se non c'è, lo calcola in base al "scheduled" sopra-citato.
        -->
		</div>

		<xsl:call-template name="action-row">
			<xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
			<xsl:with-param name="icon" select="'export'"/>
			<xsl:with-param name="name" select="'export_link'"/>
			<xsl:with-param name="href" select="@export_href"/>
		</xsl:call-template>
		<xsl:call-template name="action-row">
			<xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
			<xsl:with-param name="icon" select="'invite'"/>
			<xsl:with-param name="name" select="'osiris_invite_link'"/>
			<xsl:with-param name="href">
				<xsl:call-template name="copy-paste-link">
					<xsl:with-param name="text" select="@subscribe_href"/>
				</xsl:call-template>
			</xsl:with-param>
		</xsl:call-template>
		<xsl:call-template name="action-row">
			<xsl:with-param name="prefix" select="'portal.pages.user.actions'"/>
			<xsl:with-param name="icon" select="'invite'"/>
			<xsl:with-param name="name" select="'isis_invite_link'"/>
			<xsl:with-param name="href">
				<xsl:call-template name="copy-paste-link">
					<xsl:with-param name="text" select="@isis_subscribe_href"/>
				</xsl:call-template>
			</xsl:with-param>
		</xsl:call-template>

	</xsl:template>


</xsl:stylesheet>