<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

	<xsl:output method="html"/>

	<xsl:template match="post">		
		<table cellspacing="1">
			<tbody>
				<tr>
					<td class = "osForum_PostHeader">
						<a class = "osNormalName" href = "{author/@href}" ><xsl:value-of select="author/@name"/></a>
					</td>
								
					<td class = "osForum_PostHeader">
						<img class = "osForum_PostDetailsIcon" src = "{system:resource-url('images/post_details.gif')}" />						
						<span class = "osForum_PostDetails"> Posted: <xsl:value-of select="content/@entity_date"/></span>
					</td>
				</tr>
				<tr>
					<td class = "osForum_PostUser">						 
						<xsl:apply-templates select="author"/>
					</td>
					
					<td class = "osForum_PostContent">						
						<xsl:apply-templates select="content"/>
					</td>
				</tr>				
				<tr>
					<td class = "osForum_PostCommands">	
						<xsl:choose>
							<xsl:when test="author/@online='true'">
								<img src="{system:resource-url('images/user_online.gif')}" />
							</xsl:when>					
							<xsl:otherwise>	
								<img src="{system:resource-url('images/user_offline.gif')}" />
							</xsl:otherwise>					
						</xsl:choose>
					</td>
				
					<td class = "osForum_PostCommands">
						<table>
							<tr>
								<td>						
									<a class="no_link" href="javascript:scroll(0,0);"><img src = "{system:resource-url('images/up.gif')}" /></a>
								</td>
								
								<td align = "right">														
									<xsl:apply-templates select="actions"/>								
								</td>
							</tr>				
						</table>
					</td>
				</tr>
				<tr>
					<td class = "osForum_PostFooter" colspan = "2">						
					</td>
				</tr>
			</tbody>
		</table>		
	</xsl:template>

	<xsl:template match="author">
		<span class = "osForum_PostDetails">
			<xsl:if test="@avatar">
				<img class = "osForum_Avatar" src = "{@avatar}"/>
				<br/>
				<br/>
			</xsl:if>
			Description:
			<br/>
			Posts:
			<br/>
			Joined: <xsl:value-of select="@join_date"/>
		</span>
	</xsl:template>

	<xsl:template match="content">
		<div>
			<xsl:value-of select="@value"/>
			<xsl:if test="../author/@mark">				
				<br/>
				<br/>			
				--------------------
				<div>
					<xsl:value-of select="../author/@mark"/>
				</div>				
			</xsl:if>			
		</div>
	</xsl:template>

	<xsl:template match="actions">
		<xsl:for-each select="action">
			<a href="{@href}">
				<xsl:choose>
					<xsl:when test="@name='edit'">
						<img src = "{system:resource-url('images/edit.gif')}" />					
					</xsl:when>					
					<xsl:when test="@name='quote'">
						<img src = "{system:resource-url('images/quote.gif')}" />					
					</xsl:when>					
					<xsl:otherwise>	
						<xsl:value-of select="@name"/>
					</xsl:otherwise>					
				</xsl:choose>			
			</a>				
		</xsl:for-each>
	</xsl:template>
	
</xsl:stylesheet>