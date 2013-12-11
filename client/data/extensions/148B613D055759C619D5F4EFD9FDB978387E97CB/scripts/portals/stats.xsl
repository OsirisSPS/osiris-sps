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
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="table_trash_reason"/>

  <xsl:template match="/stats">
    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('portal.pages.stats.title')}" data-os-otype="dialog" data-os-dialog-width="60%">
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('portal.pages.stats.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="page">
		<xsl:call-template name="block_small">
			<xsl:with-param name="title" select="lang:text('portal.pages.stats.general')"/>
			<xsl:with-param name="content">
				<table class="os_table_properties">
					<tr>
						<td>
							<xsl:value-of select="lang:text('portal.pages.stats.stabilization')"/>
							<xsl:text>: </xsl:text>
						</td>
						<td>
							<xsl:value-of select="@stabilization"/>
						</td>
					</tr>
					<tr>
						<td>
							<xsl:value-of select="lang:text('portal.pages.stats.total_objects')"/>
							<xsl:text>: </xsl:text>
						</td>
						<td>
							<xsl:value-of select="@total_objects"/>
						</td>
					</tr>
					<tr>
						<td>
							<xsl:value-of select="lang:text('portal.pages.stats.unchecked_objects')"/>
							<xsl:text>: </xsl:text>
						</td>
						<td>
							<xsl:value-of select="@unchecked_objects"/>
						</td>
					</tr>
					<tr>
						<td>
							<xsl:value-of select="lang:text('portal.pages.stats.trash_objects')"/>
							<xsl:text>: </xsl:text>
						</td>
						<td>
							<xsl:value-of select="@trash_objects"/>
						</td>
					</tr>
					<tr>
						<td>
							Alignment hash :
						</td>
						<td class="hash">
							<xsl:call-template name="hash">
								<xsl:with-param name="hash" select="@align_hash"/>
							</xsl:call-template>
						</td>
					</tr>
					<tr>
						<td>
							Acceptable hash :
						</td>
						<td class="hash">
							<xsl:call-template name="hash">
								<xsl:with-param name="hash" select="@acceptable_hash"/>
							</xsl:call-template>
						</td>
					</tr>
					<tr>
						<td class="os_label">
							Machine ID :
						</td>
						<td class="os_value">
							<xsl:call-template name="hash">
								<xsl:with-param name="hash" select="//@machine_id"/>
							</xsl:call-template>
						</td>
					</tr>
				</table>
			</xsl:with-param>
		</xsl:call-template>

		<xsl:call-template name="block_small">
			<xsl:with-param name="title" select="lang:text('portal.pages.stats.trash_reason')"/>
			<xsl:with-param name="content">
				<xsl:value-of select="$table_trash_reason" disable-output-escaping="yes"/>
			</xsl:with-param>
		</xsl:call-template>

		<xsl:call-template name="block_small">
			<xsl:with-param name="title" select="lang:text('portal.pages.stats.objects_types')"/>
			<xsl:with-param name="content">
				<table class="os_table_data">
					<tr>
						<th>Type</th>
						<th>N. Total</th>
						<th>N. Trash</th>
					</tr>					
				</table>
				
			</xsl:with-param>
		</xsl:call-template>
    
  </xsl:template>


</xsl:stylesheet>