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
	<xsl:param name="enableExchange"/>
  <xsl:param name="id"/>
  <xsl:param name="pov"/>
  <xsl:param name="subscribe"/>
  
  <xsl:template match="/subscribe">
		<div style="width:900px;margin:auto;">
			<xsl:call-template name="block_big">
				<xsl:with-param name="title" select="lang:text(concat('main.pages.subscribe.',@mode,'.title'))" />
				<xsl:with-param name="content">


					<xsl:call-template name="help-box">
						<xsl:with-param name="text" select="lang:text(concat('main.pages.subscribe.',@mode,'.help'))" />
					</xsl:call-template>
					
					<xsl:if test="@mode = 'subscribe'">

						<div style="text-align:center;padding:20px;">
							<input id="osiris_url" style="width:90%;font-size:1.5em;" value="osiris:?"/>

							<script type="text/javascript">
								$('#osiris_url').blur(parseOsirisUrl);
								$('#osiris_url').keyup(parseOsirisUrl);

								function parseOsirisUrl()
								{
									var l = $('#osiris_url').val();
									
									if(l.substr(0,8) != "osiris:?")
									{
										$('#osiris_url_feedback').val("Not a valid url");
										return;
									}
									else
									{								
										var pairs = l.substr(8).split('&amp;');

								$.each(pairs, function(index, chunk)
								{
								var v2 = chunk.split('=');
								if(v2.length == 2)
								{
								var k = v2[0];
								var v = decodeURIComponent(v2[1]);

								console.log("key:" + k + ",val:" + v);

								if(k == "portal")
								$("#page-id").val(v).trigger("change");
								else if(k == "name")
								$("#page-name").val(v).trigger("change");
								else if(k == "pov")
								$("#page-pov").val(v).trigger("change");
								}
								});
								}
								}




							</script>
						</div>
					</xsl:if>

					<table class="os_table_properties">
						<tr>
							<td>
								<xsl:value-of select="lang:text('main.pages.subscribe.name')"/>
								<xsl:text>: </xsl:text>
							</td>
							<td>
								<xsl:value-of select="$name" disable-output-escaping="yes"/>
							</td>
						</tr>
						<tr>
							<td>
								<xsl:value-of select="lang:text('main.pages.subscribe.password')"/>
								<xsl:text>: </xsl:text>
							</td>
							<td>
								<xsl:value-of select="$password" disable-output-escaping="yes"/>
							</td>
						</tr>
						<tr>
							<td>
								<xsl:value-of select="lang:text('main.pages.subscribe.enableExchange')"/>
								<xsl:text>: </xsl:text>
							</td>
							<td>
								<xsl:value-of select="$enableExchange" disable-output-escaping="yes"/>
							</td>
						</tr>
						<xsl:if test="@mode = 'subscribe'">
							<tr>
								<td>
									<xsl:value-of select="lang:text('main.pages.subscribe.id')"/>
									<xsl:text>: </xsl:text>
								</td>
								<td>
									<xsl:value-of select="$id" disable-output-escaping="yes"/>
								</td>
							</tr>
							<tr>
								<td>
									<xsl:value-of select="lang:text('main.pages.subscribe.pov')"/>
									<xsl:text>: </xsl:text>
								</td>
								<td>
									<xsl:value-of select="$pov" disable-output-escaping="yes"/>
								</td>
							</tr>
						</xsl:if>

					</table>
					<div class="os_commands">
						<xsl:value-of select="$subscribe" disable-output-escaping="yes"/>
					</div>

				</xsl:with-param>
			</xsl:call-template>
		</div>
  </xsl:template>
  
  
</xsl:stylesheet>