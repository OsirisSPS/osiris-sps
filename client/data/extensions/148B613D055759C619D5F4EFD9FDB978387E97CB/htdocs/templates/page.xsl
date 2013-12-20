<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                exclude-result-prefixes="os lang date system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/debug.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pathway.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="area_left"/>
  <xsl:param name="area_top"/>
  <xsl:param name="area_right"/>
  <xsl:param name="area_bottom"/>
  <xsl:param name="area_content_top"/>
  <xsl:param name="area_content_bottom"/>
  <xsl:param name="area_content"/>
  <xsl:param name="area_unknown"/>
  <xsl:param name="assistant"/>
  <!--
  <xsl:param name="isis_area_systembar"/>
  <xsl:param name="isis_area_footer"/>
  -->

  <xsl:template match="page">

    <!--<xsl:call-template name="dump_xml"/>
    -->
       
    <xsl:if test="@page_mode = 'full'">
      <xsl:call-template name="systembar"/>
    </xsl:if>

    <xsl:choose>
      <xsl:when test="@page_mode = 'content'">        
        <xsl:value-of select="$area_content" disable-output-escaping="yes"/>
      </xsl:when>
      <xsl:otherwise>
        <div class="os_page_container3">
          <div class="os_page_container2">
            <div class="os_page_container">
              <div class="os_page">

                <a id="top" href="#top" accesskey="t" title="{lang:text('accesskey.top')}"></a>

                <a href="javascript:Osiris.Assistant.toggle()" accesskey="a"/>

                <!-- > 0.14 RC1 -->
                <div class="os_header">

                  <xsl:call-template name="userbox"/>

                  <!-- Default Links -->
                  <xsl:if test="//@skip_default_header = 'false'">
                    <xsl:value-of select="system:parse(lang:text('portal.defaults.links'))" disable-output-escaping="yes"/>
                  </xsl:if>

                  <xsl:if test="$area_top">
                    <div class="os_area_top" style="padding-top:5px;">
                      <xsl:value-of select="$area_top" disable-output-escaping="yes"/>
                    </div>
                    <!--<div class="os_clear"/>-->
                  </xsl:if>

                  <div style="clear:both"/>
                </div>

                <div class="os_page_content">

                  <div class="os_layout">


                    <!-- Pathway -->
                    <xsl:apply-templates select="pathway"/>

                    <!-- Messages -->
                    <xsl:for-each select="messages/message">
                      <div class="box-{@type}">
                        <xsl:value-of select="@description" />
                      </div>                      
                    </xsl:for-each>

                    <xsl:if test="$area_left">
                      <div style="float:left" class="os_area_left">
                        <xsl:value-of select="$area_left" disable-output-escaping="yes"/>
                      </div>
                    </xsl:if>

                    <xsl:if test="$area_right">
                      <div style="float:right" class="os_area_right">
                        <xsl:value-of select="$area_right" disable-output-escaping="yes"/>
                      </div>
                    </xsl:if>

                    <xsl:if test="$area_content_top or $area_content or $area_content_bottom">
                      <div style="overflow:auto;">
                        <xsl:if test="$area_content_top">
                          <div class="os_area_content_top">
                            <xsl:value-of select="$area_content_top" disable-output-escaping="yes"/>
                          </div>
                        </xsl:if>
                        <xsl:if test="$area_content">
                          <div class="os_area_content">
                            <xsl:value-of select="$area_content" disable-output-escaping="yes"/>
                          </div>
                        </xsl:if>
                        <xsl:if test="$area_content_bottom">
                          <div class="os_area_content_bottom">
                            <xsl:value-of select="$area_content_bottom" disable-output-escaping="yes"/>
                          </div>
                        </xsl:if>
                      </div>
                    </xsl:if>


                    <div class="os_clear"/>

                    <xsl:if test="$area_bottom">
                      <div class="os_area_bottom">
                        <xsl:value-of select="$area_bottom" disable-output-escaping="yes"/>
                      </div>
                      <div class="os_clear"/>
                    </xsl:if>

                    <xsl:if test="$area_unknown">
                      <div class="os_area_unknown">
                        <xsl:call-template name="block_small">
                          <xsl:with-param name="title" select="unknown"/>
                          <xsl:with-param name="body" select="$area_unknown"/>
                        </xsl:call-template>
                      </div>
                      <div class="os_clear"/>
                    </xsl:if>
                  </div>

                </div>

                <a id="bottom" href="#bottom" accesskey="z" title="{lang:text('accesskey.bottom')}"></a>

                <xsl:value-of select="$assistant" disable-output-escaping="yes"/>

                <xsl:if test="@isis">
                  <!--<xsl:value-of select="$isis_area_systembar" disable-output-escaping="yes"/>-->
                  <xsl:text disable-output-escaping="yes"><![CDATA[
                  <!--{__Isis_Area_Footer__}-->
                  ]]></xsl:text>
                </xsl:if>


              </div>
            </div>
          </div>
        </div>
      </xsl:otherwise>
    </xsl:choose>



  </xsl:template>

  <xsl:template name="systembar">

    <a id="system" href="#system" accesskey="s" title="{lang:text('accesskey.system')}"></a>

    <xsl:choose>
      <xsl:when test="@isis_mode">
        <!--<xsl:value-of select="$isis_area_systembar" disable-output-escaping="yes"/>-->
        <xsl:text disable-output-escaping="yes"><![CDATA[
          <!--{__Isis_Area_System__}-->
          ]]></xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <div class="os_systembar">
          <div id="osiris_advertising" class="os_advertising"></div>

          <div class="os_systembar_left">
            <a class="os_systembar_logo" target="_blank" title="Osiris Home Page" href="{@href_home}">
              <img src="{system:resource-url('images/systembar/logo.png')}" alt="Osiris"/>
            </a>
            <br />
            <a title="{lang:text('systembar.actions.home')}" class="os_systembar_icon" href="/main/home">
              <img src="{system:resource-url('images/systembar/icon_home.png')}" data-os-tooltip="{lang:text('systembar.actions.home')}"/>
            </a>
            <a title="{lang:text('systembar.actions.help')}" class="os_systembar_icon" href="javascript:void(0);" onclick="Osiris.loadUrl('/main/help?mode=dialog')" data-os-tooltip="{lang:text('systembar.actions.help')}">
              <img src="{system:resource-url('images/systembar/icon_help.png')}"/>
            </a>
            <!-- TODO: pulsante Network: rosso (isis non raggiungibile), giallo (isis ok, ma nat), verde (isis ok, no NAT).
              Valutare se farlo automatico/temporizzato/ajax. -->
            <a title="{lang:text('systembar.actions.network')}" class="os_systembar_icon" href="javascript:void(0);" onclick="Osiris.loadUrl('/main/network?mode=dialog')" data-os-tooltip="{lang:text('systembar.actions.network')}">
              <xsl:value-of select="@isis_status"/>
              <xsl:choose>
                <xsl:when test="@isis_status = 2">                  
                  <img src="{system:resource-url('images/systembar/icon_network_info.png')}"/>
                </xsl:when>
                <xsl:when test="@isis_status = 1">
                  <img src="{system:resource-url('images/systembar/icon_network_warning.png')}"/>
                </xsl:when>
                <xsl:otherwise>
                  <img src="{system:resource-url('images/systembar/icon_network_error.png')}"/>
                </xsl:otherwise>
              </xsl:choose>              
            </a>

            <xsl:if test="@portal_name">
              <a title="{lang:text('systembar.actions.assistant')}" class="os_systembar_icon" href="javascript:void(0);" onclick="Osiris.loadUrl('{@assistant_href}')" data-os-tooltip="{lang:text('systembar.actions.assistant')}">
                <img src="{system:resource-url('images/systembar/icon_assistant.png')}"/>
              </a>
            </xsl:if>

            <xsl:call-template name="separator" />


            <xsl:if test="@logged = 'true'">
              <xsl:value-of select="'Logged as '"/>
              <strong>
                <xsl:value-of select="@account_name"/>
              </strong>
              <xsl:text> ( </xsl:text>
              <a href="/main/account">Settings</a>
              <xsl:text>, </xsl:text>
              <a href="{actions/action[@name='logout']/@href}">Logout</a>
              <xsl:text> ) </xsl:text>
            </xsl:if>
            <xsl:if test="@logged = 'false'">
              <i>Not logged.</i>
              <xsl:text> ( </xsl:text>
              <a href="/main/accounts">Login</a>
              <xsl:text> ) </xsl:text>
            </xsl:if>

            

            <!-- Portal & User -->
            <xsl:if test="@portal_name and @logged = 'true'">

              <xsl:call-template name="separator" />

              <xsl:choose>
                <xsl:when test="user/@name">
                  <xsl:text>User: </xsl:text>

                  <a href="{user/@view_href}">
                    <xsl:if test="user/@avatar">
                      <img style="width:16px;height:16px;" src="{user/@avatar}"/>
                    </xsl:if>

                    <b>
                      <xsl:text> </xsl:text>
                      <xsl:value-of select="user/@name"/>
                    </b>
                  </a>
                  <xsl:text> ( </xsl:text>
                  <a href="{user/actions/action[@name='account']/@href}">
                    <xsl:value-of select="lang:text('portal.userbox.actions.account')"/>
                  </a>
                  <xsl:text>, </xsl:text>
                  <a href="{user/actions/action[@name='logout']/@href}">
                    <xsl:value-of select="lang:text('portal.userbox.actions.logout')"/>
                  </a>
                  <xsl:text> ) </xsl:text>
                </xsl:when>
                <xsl:otherwise>
                  <a href="{user/actions/action[@name='login']/@href}">
                    <xsl:value-of select="lang:text('portal.userbox.actions.login')"/>
                  </a>
                  <xsl:if test="user/actions/action[@name='register']">
                    <xsl:text> </xsl:text>
                    <xsl:value-of select="lang:text('common.or')"/>
                    <xsl:text> </xsl:text>
                    <a href="{user/actions/action[@name='register']/@href}">
                      <xsl:value-of select="lang:text('portal.userbox.actions.register')"/>
                    </a>
                  </xsl:if>
                </xsl:otherwise>
              </xsl:choose>


              <xsl:text> on </xsl:text>
              <b>
                <xsl:value-of select="@portal_name"/>
              </b>

              <xsl:call-template name="separator" />

              <xsl:value-of select="lang:text('systembar.stability_status')"/>
              <xsl:text>: </xsl:text>
              <b>
                <xsl:value-of select="lang:text(concat('stability.',@stability_status))"/>
              </b>
            </xsl:if>

          </div>
          

        </div>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
                
  
  <xsl:template name="userbox">
    <xsl:if test="user/actions/action">
      <div class="os_userbox">

        <a id="userbox_assistant" class="os_userbox_assistant_up" href="javascript:Osiris.Assistant.toggle()" data-os-tooltip="{lang:text('portal.userbox.assistant')}"/>

        
        <xsl:choose>
          <xsl:when test="user/@name">
            <xsl:if test="user/@avatar">
              <img style="width:16px;height:16px;" src="{user/@avatar}"/>
            </xsl:if>
            <xsl:text>User: </xsl:text>
            <xsl:value-of select="user/@name"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="lang:text('portal.userbox.guest')"/>
            Not registered.
          </xsl:otherwise>
        </xsl:choose>
        
          
        <div style="float:left;margin-left:10px;">
          <div style="float:left;">
            <xsl:call-template name="actions-popup">
              <xsl:with-param name="prefix" select="'portal.userbox.actions'"/>
              <xsl:with-param name="actions" select="user/actions"/>
              <xsl:with-param name="actions_style" select="'position:absolute'"/>
            </xsl:call-template>
          </div>
          <xsl:if test="user/@name">
            <a class="os_userbox_notifications" href="{user/actions/action[@name='messenger']/@href}" data-os-tooltip="{user/@new_messages_tooltip}">
              <xsl:value-of select="user/@new_messages"/>
            </a>
          </xsl:if>
          <div class="os_userbox_links">
            <xsl:choose>
              <xsl:when test="user/@name">
                <a href="{user/actions/action[@name='account']/@href}">
                  <xsl:value-of select="lang:text('portal.userbox.actions.account')"/>
                </a>
                <xsl:text> - </xsl:text>
                <a href="{user/actions/action[@name='logout']/@href}">
                  <xsl:value-of select="lang:text('portal.userbox.actions.logout')"/>
                </a>
              </xsl:when>
              <xsl:when test="//@mcp = 'true'">
                <xsl:value-of select="lang:text('portal.userbox.guest')"/>
                <xsl:text> </xsl:text>
                <a href="{user/actions/action[@name='login']/@href}">
                  <xsl:value-of select="lang:text('portal.userbox.actions.login')"/>
                </a>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="lang:text('portal.userbox.guest')"/>
                <xsl:text> </xsl:text>
                <a href="{user/actions/action[@name='login']/@href}">
                  <xsl:value-of select="lang:text('portal.userbox.actions.login')"/>
                </a>
                <xsl:if test="user/actions/action[@name='register']">
                  <xsl:text> </xsl:text>
                  <xsl:value-of select="lang:text('common.or')"/>
                  <xsl:text> </xsl:text>
                  <a href="{user/actions/action[@name='register']/@href}">
                    <xsl:value-of select="lang:text('portal.userbox.actions.register')"/>
                  </a>
                </xsl:if>
              </xsl:otherwise>
            </xsl:choose>
          </div>

        </div>

      </div>

    </xsl:if>
  </xsl:template>

</xsl:stylesheet>

