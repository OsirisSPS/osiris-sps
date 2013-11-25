<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="saveCommand"/>

  <xsl:param name="portalName"/>
  <xsl:param name="portalDescription"/>

	<xsl:param name="registerTerms"/>
	
	<xsl:param name="layoutComponent"/>
	<xsl:param name="layoutTileImage"/>
	<xsl:param name="layoutTileColorBackground"/>
	<xsl:param name="layoutTileColorForeground"/>
	<xsl:param name="layoutSkinParams"/>
  <xsl:param name="layoutCss"/>
  <xsl:param name="layoutHeader"/>

  <xsl:param name="objectsMaxSize"/>
  <xsl:param name="badWords"/>

  <xsl:param name="allowObjectInFuture"/>
  <xsl:param name="allowObjectUnsigned"/>
  <xsl:param name="objectsPhysicalRemove"/>
  <xsl:param name="objectsPhysicalRemoveDays"/>
  

  
  <xsl:template match="/home">
    <xsl:call-template name="block_page">
      <xsl:with-param name="prefix" select="'portal.pages.acp'"/>        
      <xsl:with-param name="content">
        <xsl:value-of select="//@guest"/>
        <xsl:value-of select="//@userOfPov"/>
        <a href="{@onCreatePov_href}">
          <xsl:value-of select="//@onCreatePov_href"/>
        </a>
        <xsl:choose>
          <xsl:when test="//@isGuest = 'true'">
            <xsl:call-template name="help-box">
              <xsl:with-param name="text" select="lang:text('~portal.pages.acp.guest.help')"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:when test="//@isUserOfPov = 'true'">
            <xsl:call-template name="help-box">
              <xsl:with-param name="text" select="lang:text('~portal.pages.acp.admin.help')"/>
            </xsl:call-template>
          </xsl:when>
          <xsl:otherwise>
            <xsl:call-template name="help-box">
              <xsl:with-param name="text" select="lang:text('~portal.pages.acp.user.help')"/>
            </xsl:call-template>
          </xsl:otherwise>          
        </xsl:choose>

        <xsl:if test="//@isUserOfPov = 'true'">
          <div class="os_commands_right">
            <xsl:value-of select="$saveCommand" disable-output-escaping="yes"/>
          </div>
        </xsl:if>

        <div id="addons_tab" style="display:none" data-os-otype="tab" data-os-layout="left" data-os-storage="portal.acp">

          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('portal.pages.acp.main')"/>
          </div>
          <div data-os-tabType="body">
            <table class="os_table_properties">              
              <tr>
                <td>
                  <xsl:value-of select="'portalName'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$portalName" disable-output-escaping="yes"/>
                </td>
              </tr>
              <tr>
                <td>
                  <xsl:value-of select="'portalDescription'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$portalDescription" disable-output-escaping="yes"/>
                </td>
              </tr>
              

            </table>

            <xsl:if test="//@isGuest = 'false' and //@isUserOfPov = 'false'">
              <h1>Fork this portal!</h1>
              <xsl:if test="//@povOfUserExists = 'true'">
                <a href="{//@povOfUserHref}">Visit</a>
              </xsl:if>
              <xsl:if test="//@povOfUserExists = 'false'">
                <a href="{//@povOfUserHref}">Create now!</a>
              </xsl:if>
            </xsl:if>
          </div>

          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('portal.pages.acp.layout')"/>
          </div>
          <div data-os-tabType="body">
            <table class="os_table_properties">
              <tr>
                <td>
                  <xsl:value-of select="'layoutComponent'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$layoutComponent" disable-output-escaping="yes"/>
                </td>
              </tr>
							<!--
              <tr>
                <td>
                  <xsl:value-of select="'registerTerms'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$registerTerms" disable-output-escaping="yes"/>
                </td>
              </tr>
							-->
              <tr>
                <td>
                  <xsl:value-of select="'layoutSkinParams'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$layoutSkinParams" disable-output-escaping="yes"/>
                </td>
              </tr>
							<tr>
								<td>
									<xsl:value-of select="'tile'"/>
									<xsl:text> :</xsl:text>
								</td>
								<td>
									<xsl:value-of select="$layoutTileImage" disable-output-escaping="yes"/>
									<br/>
									BG: <xsl:value-of select="$layoutTileColorBackground" disable-output-escaping="yes"/>
									FG: <xsl:value-of select="$layoutTileColorForeground" disable-output-escaping="yes"/>									
								</td>
							</tr>
              <tr>
                <td>
                  <xsl:value-of select="'layoutCss'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$layoutCss" disable-output-escaping="yes"/>
                </td>
              </tr>
              <tr>
                <td>
                  <xsl:value-of select="'layoutHeader'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$layoutHeader" disable-output-escaping="yes"/>
                </td>
              </tr>
            </table>
          </div>

          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('portal.pages.acp.rules')"/>
          </div>
          <div data-os-tabType="body">
            <table class="os_table_properties">              
              <tr>
                <td>
                  <xsl:value-of select="'objectsMaxSize'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$objectsMaxSize" disable-output-escaping="yes"/>
                </td>
              </tr>              
              <tr>
                <td>
                  <xsl:value-of select="'badWords'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$badWords" disable-output-escaping="yes"/>
                </td>
              </tr>
            </table>
          </div>

          <!-- rules_ex : Rules dedicated to beta-tester or experience users. Defaults for most common use are ok. -->
          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('portal.pages.acp.rules_ex')"/>
          </div>
          <div data-os-tabType="body">
            <table class="os_table_properties">
              <tr>
                <td>
                  <xsl:value-of select="'allowObjectInFuture'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$allowObjectInFuture" disable-output-escaping="yes"/>
                </td>
              </tr>
              <tr>
                <td>
                  <xsl:value-of select="'allowObjectUnsigned'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$allowObjectUnsigned" disable-output-escaping="yes"/>
                </td>
              </tr>
              <tr>
                <td>
                  <xsl:value-of select="'objectsPhysicalRemove'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$objectsPhysicalRemove" disable-output-escaping="yes"/>
                </td>
              </tr>
              <tr>
                <td>
                  <xsl:value-of select="'objectsPhysicalRemoveDays'"/>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="$objectsPhysicalRemoveDays" disable-output-escaping="yes"/>
                </td>
              </tr>
            </table>
          </div>
          
          
          


        </div>


      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  
</xsl:stylesheet>