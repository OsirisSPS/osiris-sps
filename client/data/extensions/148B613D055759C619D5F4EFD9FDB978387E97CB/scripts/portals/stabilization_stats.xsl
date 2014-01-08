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

  <xsl:param name="table_trash_reason"/>

  <xsl:template match="/stats">		
    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div title="{lang:text('portal.pages.stabilization_stats.title')}" data-os-otype="dialog" data-os-dialog-width="60%">
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('portal.pages.stabilization_stats.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>

  <xsl:template name="page">

		<table style="width:100%">
			<tr>
				<td>
					<xsl:call-template name="block_small">
						<xsl:with-param name="title" select="lang:text('common.labels.overview')"/>
						<xsl:with-param name="content">
							<table class="os_table_properties">
								<tr>
									<td>
										<xsl:value-of select="lang:text('portal.pages.stabilization_stats.stabilization')"/>
										<xsl:text>: </xsl:text>
									</td>
									<td>
										<xsl:value-of select="@stabilization"/>
									</td>
								</tr>
								<tr>
									<td>
										<xsl:value-of select="lang:text('portal.pages.stabilization_stats.total_objects')"/>
										<xsl:text>: </xsl:text>
									</td>
									<td>
										<xsl:value-of select="@total_objects"/>
									</td>
								</tr>
								<tr>
									<td>
										<xsl:value-of select="lang:text('portal.pages.stabilization_stats.unchecked_objects')"/>
										<xsl:text>: </xsl:text>
									</td>
									<td>
										<xsl:value-of select="@unchecked_objects"/>
									</td>
								</tr>
								<tr>
									<td>
										<xsl:value-of select="lang:text('portal.pages.stabilization_stats.trash_objects')"/>
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
									<td>
										Machine ID :
									</td>
									<td>
										<xsl:call-template name="hash">
											<xsl:with-param name="hash" select="//@machine_id"/>
										</xsl:call-template>
									</td>
								</tr>
							</table>
						</xsl:with-param>
					</xsl:call-template>

					<xsl:call-template name="block_small">
						<xsl:with-param name="title" select="lang:text('portal.pages.stabilization_stats.trash_reason')"/>
						<xsl:with-param name="content">

							<table class="os_table_data">
								<tr>
									<th>Reason</th>
									<th>N. Objects</th>
								</tr>
								<xsl:for-each select="trash_reasons/reason">
									<tr>
										<td>
											<xsl:if test="@id = ''">
												<xsl:value-of select="'Valid, not in trash'"/>
											</xsl:if>
											<xsl:value-of select="@id"/>
										</td>
										<td>
											<xsl:value-of select="@n"/>
										</td>
									</tr>
								</xsl:for-each>
							</table>

						</xsl:with-param>
					</xsl:call-template>


				</td>
				<td>
					<xsl:call-template name="block_small">
						<xsl:with-param name="title" select="lang:text('portal.pages.stabilization_stats.objects_types')"/>
						<xsl:with-param name="content">
							<table class="os_table_data">
								<tr>
									<th>Type</th>
									<th>N. Total</th>
									<th>N. Unchecked</th>
									<th>N. Trash</th>
									<th>Actions</th>
								</tr>
								<xsl:for-each select="objects_types/object_type">
									<tr>
										<td>
											<xsl:value-of select="lang:text(concat('objects.types.',@name))"/>
										</td>
										<td>
											<xsl:value-of select="@total"/>
										</td>
										<td>
											<xsl:value-of select="@unchecked"/>
										</td>
										<td>
											<xsl:value-of select="@trash"/>
										</td>
										<td>
											<xsl:if test="@trash > 0">
												<a href="{//@trash_href}&amp;type={@type_code}" class="os_button">
													<xsl:value-of select="'View trash'"/>
												</a>
											</xsl:if>
										</td>
									</tr>
								</xsl:for-each>
							</table>

						</xsl:with-param>
					</xsl:call-template>
				</td>
			</tr>
		</table>
		
		


		<xsl:call-template name="dump_xml"/>
    
  </xsl:template>


</xsl:stylesheet>