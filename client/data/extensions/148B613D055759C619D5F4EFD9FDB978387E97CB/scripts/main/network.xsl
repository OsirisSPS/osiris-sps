<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/help">

    <xsl:choose>
      <xsl:when test="@mode = 'dialog'">
        <div id="network" data-os-otype="dialog" data-os-dialog-width="50%" >
          <xsl:call-template name="page"/>
        </div>
      </xsl:when>
      <xsl:when test="@mode = 'content'">
        <xsl:call-template name="page"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block_big">
          <xsl:with-param name="title" select="lang:text('main.pages.network.title')"/>
          <xsl:with-param name="content">
            <xsl:call-template name="page"/>
          </xsl:with-param>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>

  </xsl:template>


  <xsl:template name="page">    
      <div style="float:right">
        <xsl:call-template name="block_small">
          <xsl:with-param name="title" select="'P2P Network'"/>
          <xsl:with-param name="content">
            <table class="os_table_properties">
              <tr>
                <td>
                  <xsl:value-of select="lang:text('portal.pages.peers.super')"/>
                  <xsl:call-template name="help-tooltip">
                    <xsl:with-param name="text" select="lang:text('portal.pages.peers.test.help')"/>
                  </xsl:call-template>
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:choose>
                    <xsl:when test="@p2p_enabled = 'true'">
                      <xsl:call-template name="boolean-icon">
                        <xsl:with-param name="value" select="@reachable!='false'"/>
                      </xsl:call-template>
                      <xsl:text> </xsl:text>
                      <xsl:value-of select="@message"/>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:value-of select="lang:text('ui.application.port_check_without_p2p')"/>
                    </xsl:otherwise>
                  </xsl:choose>

                </td>
              </tr>

              <tr>
                <td>
                  <xsl:value-of select="lang:text('portal.pages.peers.tor')"/>
                  <xsl:call-template name="help-tooltip">
                    <xsl:with-param name="text" select="lang:text('portal.pages.peers.tor.help')"/>
                  </xsl:call-template>

                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:choose>
                    <xsl:when test="@tor = 1">
                      <xsl:call-template name="status-icon">
                        <xsl:with-param name="value" select="'true'"/>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:when test="@tor = 0">
                      <xsl:call-template name="status-icon">
                        <xsl:with-param name="value" select="'false'"/>
                      </xsl:call-template>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:call-template name="status-icon">
                        <xsl:with-param name="value" select="'unknown'"/>
                      </xsl:call-template>
                    </xsl:otherwise>
                  </xsl:choose>
                </td>
              </tr>

              <tr>
                <td>
                  External IP / Port
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="@ip"/>
                  <xsl:text> : </xsl:text>
                  <xsl:value-of select="@port"/>
                </td>
              </tr>
              <tr>
                <td>
                  Geolocation
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <img src="{system:resource-url(concat('images/flags/countries/',translate(@country_code, 'abcdefghijklmnopqrstuvwxyz', 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'),'.png'))}" />
                  <xsl:text> </xsl:text>
                  <xsl:value-of select="@country_name"/>
                </td>
              </tr>
              <tr>
                <td>
                  Last update
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="date:userfriendly-datetime(@last_request)" />
                </td>
              </tr>
              <tr>
                <td>
                  Last checking
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="date:userfriendly-datetime(@last_check_date)" />
                </td>
              </tr>
              <tr>
                <td>
                  Last reachable
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:value-of select="date:userfriendly-datetime(@last_validation_date)" />
                </td>
              </tr>

            </table>
          </xsl:with-param>
        </xsl:call-template>
        <xsl:call-template name="block_small">
          <xsl:with-param name="title" select="'Internet Time'"/>
          <xsl:with-param name="content">
            <table class="os_table_properties">
              <tr>
                <td>
                  Internet Time available
                  <xsl:text> :</xsl:text>
                </td>
                <td>
                  <xsl:call-template name="boolean-icon">
                    <xsl:with-param name="value" select="@internet_datetime_available!='false'"/>
                  </xsl:call-template>
                  <xsl:if test="@internet_datetime_available!='false'">
                    <xsl:text> - </xsl:text>
                    Source :
                    <b>
                      <xsl:value-of select="@internet_datetime_sync_method" />
                    </b>
                  </xsl:if>

                </td>
              </tr>
              <xsl:if test="@internet_datetime_available!='false'">
                <tr>
                  <td>
                    Internet Time current
                    <xsl:text> :</xsl:text>
                  </td>
                  <td>
                    <xsl:value-of select="date:long-datetime(@internet_datetime)" />
                  </td>
                </tr>
                <tr>
                  <td>
                    Local Time current
                    <xsl:text> :</xsl:text>
                  </td>
                  <td>
                    <xsl:value-of select="date:long-datetime(@system_datetime)" />
                  </td>
                </tr>
                <tr>
                  <td>
                    Difference from local time
                    <xsl:text> :</xsl:text>
                  </td>
                  <td>
                    <xsl:value-of select="@internet_datetime_delta" />
                    <xsl:text> </xsl:text>
                    <xsl:value-of select="lang:text('datetime.timespan.seconds')"/>
                  </td>
                </tr>
              </xsl:if>
            </table>
          </xsl:with-param>
        </xsl:call-template>        
        
      </div>
      <div style="overflow:auto" class="os_padding">
        <xsl:choose>
          <xsl:when test="@last_error != ''">
            <div class="box-error">
              <xsl:text>Isis not reachable. Internet connection problems?</xsl:text>
              <xsl:text> - </xsl:text>
              <xsl:value-of select="@last_error"/>
            </div>
          </xsl:when>
          <xsl:otherwise>
            <div class="box-info">
              Isis reachable, Internet connection active.
            </div>

            <xsl:if test="@reachable != 'true'">
              <div class="box-warning">
                Not reachable from outside. Are you under NAT, or you don't configure your router/firewall?
              </div>
            </xsl:if>
          </xsl:otherwise>
        </xsl:choose>

        <xsl:if test="@p2p_enabled != 'true'">
          <div class="box-warning">
            P2P not active.
          </div>
        </xsl:if>

      </div>

      <div style="clear:both"/>

      <xsl:if test="@mode = 'dialog' or @mode = 'content'">
        <div class="os_center">
          <a class="os_button" href="javascript:Osiris.contentFromUrl('#network', '/main/network?mode=content&amp;force=true', 'none', 'notify');">Refresh</a>
        </div>
      </xsl:if>

      

    
  </xsl:template>

  
</xsl:stylesheet>