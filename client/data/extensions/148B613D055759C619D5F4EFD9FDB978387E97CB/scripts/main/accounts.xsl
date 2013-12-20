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

  <xsl:param name="username"/>
  <xsl:param name="password"/>
  <xsl:param name="passwordChecker"/>
  <xsl:param name="savePassword"/>
  <xsl:param name="register"/>
  <xsl:param name="cancel"/>

  <xsl:template match="/accounts">
    <xsl:call-template name="dump_xml"/>
    <xsl:call-template name="block_page">
      <xsl:with-param name="prefix" select="'main.pages.accounts'" />
      <xsl:with-param name="content">
        <xsl:call-template name="help-box">
          <xsl:with-param name="text" select="lang:text('main.pages.accounts.help')" />
        </xsl:call-template>


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
            <xsl:with-param name="style" select="'float:left;width:200px;height:200px;'"/>
            <xsl:with-param name="title">
              <xsl:value-of select="@name"/>
            </xsl:with-param>
            <xsl:with-param name="content">
              <table class="os_table_properties">
                <tr>
                  <td>
                    <xsl:value-of select="lang:text('main.pages.accounts.lastAccess')"/>
                  </td>
                  <td>
                    <xsl:value-of select="date:userfriendly-datetime(@last_access)"/>
                  </td>
                </tr>
                <tr>
                  <td>
                    <xsl:value-of select="lang:text('main.pages.accounts.connected')"/>
                  </td>
                  <td>
                    <xsl:call-template name="boolean-icon">
                      <xsl:with-param name="value" select="@logged"/>
                    </xsl:call-template>
                  </td>
                </tr>
              </table>
              <div>
                <xsl:call-template name="actions-popup">
                  <xsl:with-param name="prefix" select="'main.pages.accounts.actions'" />
                  <xsl:with-param name="actions" select="actions" />
                  <xsl:with-param name="default" select="'enter'" />
                </xsl:call-template>
              </div>
            </xsl:with-param>
          </xsl:call-template>
        </xsl:for-each>

        <xsl:call-template name="block_item">
          <xsl:with-param name="style" select="'float:left;width:200px;height:200px;'"/>
          <xsl:with-param name="title">
            <xsl:value-of select="@name"/>
          </xsl:with-param>
          <xsl:with-param name="content">
          </xsl:with-param>
        </xsl:call-template>
            
        
        <table class="os_table_data">

          <tr>
            <th>
              <xsl:value-of select="lang:text('main.pages.accounts.name')"/>
            </th>
            <th class="center">
              <xsl:value-of select="lang:text('main.pages.accounts.lastAccess')"/>
            </th>
            <th class="center">
              <xsl:value-of select="lang:text('main.pages.accounts.connected')"/>
            </th>
            <th class="center">
              <xsl:value-of select="lang:text('main.pages.accounts.actions')"/>
            </th>
          </tr>
          <xsl:for-each select="accounts/account">
            <tr>
              <td>
                <xsl:value-of select="@name"/>
              </td>
              <td class="center">
                <xsl:value-of select="date:userfriendly-datetime(@last_access)"/>
              </td>
              <td class="center">
                <xsl:call-template name="boolean-icon">
                  <xsl:with-param name="value" select="@logged"/>
                </xsl:call-template>                
              </td>
              <td class="center">
                actions
              </td>
            </tr>
          </xsl:for-each>
        </table>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template match="/register">
    <div style="width:500px;margin:auto;">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('portal.pages.register.title')" />
        <xsl:with-param name="content">
          <xsl:call-template name="help-box">
            <xsl:with-param name="text" select="lang:text('portal.pages.register.help')"/>
          </xsl:call-template>
          <table style="width:100%"> 
            <tr>
              <td class="os_cell_icon">
                <img src="{system:resource-url('images/icons/48x48/register.png')}"/>
              </td>
              <td>
                <table class="os_table_data os_content_box">
                  <tr>
                    <td class="os_label">
                      <xsl:value-of select="lang:text('portal.pages.register.username')"/>
                      <xsl:text> :</xsl:text>
                    </td>
                    <td class="os_value">
                      <xsl:value-of select="$username" disable-output-escaping="yes"/>
                    </td>
                  </tr>
                  <tr>
                    <td class="os_label">
                      <xsl:value-of select="lang:text('portal.pages.register.password')"/>
                      <xsl:text> :</xsl:text>
                    </td>
                    <td class="os_value">
                      <xsl:value-of select="$password" disable-output-escaping="yes"/>
                    </td>
                  </tr>
                  <tr>
                    <td class="os_label">
                      <xsl:value-of select="lang:text('portal.pages.register.passwordConfirm')"/>
                      <xsl:text> :</xsl:text>
                    </td>
                    <td class="os_value">
                      <xsl:value-of select="$passwordChecker" disable-output-escaping="yes"/>
                    </td>
                  </tr>
                  <tr>
                    <td class="os_label">
                      <xsl:value-of select="lang:text('portal.pages.register.rememberPassword')"/>
                      <xsl:text> :</xsl:text>
                    </td>
                    <td class="os_value">
                      <xsl:value-of select="$savePassword" disable-output-escaping="yes"/>
                    </td>
                  </tr>                  
                </table>
              </td>
            </tr>
          </table>
          
          <div class="os_commands">
            <xsl:value-of select="$register" disable-output-escaping="yes"/>
            <xsl:value-of select="$cancel" disable-output-escaping="yes"/>            
          </div>
        </xsl:with-param>
      </xsl:call-template>
    </div>
    
  </xsl:template>

  
</xsl:stylesheet>