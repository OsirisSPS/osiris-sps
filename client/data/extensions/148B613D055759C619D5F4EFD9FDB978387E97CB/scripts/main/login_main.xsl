<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="username"/>
  <xsl:param name="password"/>
  <xsl:param name="savePassword"/>
  <xsl:param name="login"/>
  <xsl:param name="cancel"/>

  <xsl:template match="/login">
    <div style="width:500px;margin:auto;">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="lang:text('portal.pages.login.title')" />
        <xsl:with-param name="content">
          <xsl:call-template name="help-box">
            <xsl:with-param name="text" select="lang:text('portal.pages.login.help')"/>
          </xsl:call-template>
          <table style="width:100%"> 
            <tr>
              <td class="os_cell_icon">
                <img src="{system:resource-url('images/icons/48x48/permission.png')}"/>
              </td>
              <td>
                <table class="os_table_data os_content_box">
                  <xsl:if test="$username">
                    <tr>
                      <td class="os_label">
                        <xsl:value-of select="lang:text('portal.pages.login.username')"/>
                        <xsl:text> :</xsl:text>
                      </td>
                      <td class="os_value">
                        <xsl:value-of select="$username" disable-output-escaping="yes"/>
                      </td>
                    </tr>
                  </xsl:if>
                  <tr>
                    <td class="os_label">
                      <xsl:value-of select="lang:text('portal.pages.login.password')"/>
                      <xsl:text> :</xsl:text>
                    </td>
                    <td class="os_value">
                      <xsl:value-of select="$password" disable-output-escaping="yes"/>
                    </td>
                  </tr>
                  <tr>
                    <td class="os_label">
                      <xsl:value-of select="lang:text('portal.pages.login.save_password')"/>
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
            <xsl:value-of select="$login" disable-output-escaping="yes"/>
            <xsl:value-of select="$cancel" disable-output-escaping="yes"/>
          </div>
        </xsl:with-param>
      </xsl:call-template>
    </div>
    
  </xsl:template>

  
</xsl:stylesheet>