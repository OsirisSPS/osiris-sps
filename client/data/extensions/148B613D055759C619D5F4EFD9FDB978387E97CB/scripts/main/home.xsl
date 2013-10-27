<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/debug.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/common.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html"/>

  <xsl:template match="page">
    <xsl:call-template name="help-tip">
      <xsl:with-param name="id" select="'main.pages.home'"/>
    </xsl:call-template>

    <!--
    Test "Osiris Hall".
    <iframe style="float:right;width:300px;height:800px;" src="/portals/view?id=03000002D6FFEC28CC9FF49C4187E4A5F608590E41C915FE&amp;portal=010000014F87B96064EA304F761B8F905FF54E057DEEA49D&amp;page-mode=content"></iframe>
    -->

    <xsl:if test="@latest_osiris_notes">
      <div class="box-info">
        <xsl:value-of select="system:parse(@latest_osiris_notes)" disable-output-escaping="yes" />
      </div>
    </xsl:if>

    <div>

      <div class="os_commands_right">
        <a class="os_button" href="{actions/action[@name='options']/@href}" data-os-tooltip="{lang:text('main.pages.options.description')}">
        	<img src="{system:resource-url('images/icons/16x16/settings.png')}" />
          <xsl:value-of select="lang:text('main.pages.home.actions.settings')"/>
        </a>


        <a class="os_button" href="{actions/action[@name='addons']/@href}" data-os-tooltip="{lang:text('main.pages.addons.description')}">
        	

          <xsl:choose>
            <xsl:when test="@upgradable_counter">
              <xsl:if test="@upgradable_counter > 0">
                <div class="os_button_counter" data-os-tooltip="{lang:text('main.pages.addons.upgrade_available')}">
                  <xsl:value-of select="@upgradable_counter"/>
                </div>
              </xsl:if>
            </xsl:when>
            <xsl:otherwise>
              <div data-os-url="/main/addons?act=upgradable&amp;mode=counter"/>
            </xsl:otherwise>
          </xsl:choose>

					<img src="{system:resource-url('images/icons/16x16/addons.png')}" />
          <xsl:value-of select="lang:text('main.pages.addons.title')"/>
        </a>
       
        <a class="os_button" href="javascript:void(0);" onclick="Osiris.dialog('#more_dialog', true);return false;"  data-os-tooltip="{lang:text('main.pages.more.description')}">
        	<img src="{system:resource-url('images/icons/16x16/more.png')}" />
          <xsl:value-of select="lang:text('main.pages.home.actions.more')"/>
        </a>


      </div>

      <table style="width:100%">
        <xsl:if test="//@session_user != ''">
          <tr>
            <td>
              <xsl:call-template name="block">
                <xsl:with-param name="title" select="lang:text('main.pages.home.subscribed_your')" />
                <xsl:with-param name="content">
                  <xsl:choose>
                    <xsl:when test="subscribedPortals/portal[@id != //@session_user and @pov = //@session_user]">
                      <div>
                        <xsl:for-each select="subscribedPortals/portal[@id != //@session_user and @pov = //@session_user]">
                          <xsl:call-template name="tile-portal"/>
                        </xsl:for-each>
                      </div>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:call-template name="help-box">
                        <xsl:with-param name="text" select="lang:text('main.pages.home.no_portals')"/>
                      </xsl:call-template>
                    </xsl:otherwise>
                  </xsl:choose>
                  <div style="clear:both;"/>
                </xsl:with-param>
              </xsl:call-template>
            </td>
            <td>
              <xsl:call-template name="block">
                <xsl:with-param name="title" select="lang:text('~main.pages.home.subscribed_self')" />
                <xsl:with-param name="content">
                  <xsl:choose>
                    <xsl:when test="subscribedPortals/portal[@id = //@session_user and @pov = //@session_user]">
                      <div>
                        <xsl:for-each select="subscribedPortals/portal[@id = //@session_user and @pov = //@session_user]">
                          <xsl:call-template name="tile-portal"/>
                        </xsl:for-each>
                      </div>
                    </xsl:when>
                    <xsl:otherwise>
                      <div class="os_center">
                        <a href="{@subscribe_self_portal_href}">
                          <xsl:value-of select="lang:text('~main.pages.home.subscribe_self_portal')"/>
                        </a>
                      </div>

                    </xsl:otherwise>
                  </xsl:choose>
                  <div style="clear:both;"/>
                </xsl:with-param>
              </xsl:call-template>
            </td>
          </tr>
        </xsl:if>
        <tr>
          <td style="width:100%;">
            <xsl:call-template name="block">
              <xsl:with-param name="title" select="lang:text('main.pages.home.subscribed_portals')" />
              <xsl:with-param name="content">
                <xsl:choose>
                  <xsl:when test="subscribedPortals/portal[@id != @pov and @pov != //@session_user]">
                    <div>
                      <xsl:for-each select="subscribedPortals/portal[@id != @pov and @pov != //@session_user]">
                        <xsl:call-template name="tile-portal"/>
                      </xsl:for-each>
                    </div>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:call-template name="help-box">
                      <xsl:with-param name="text" select="lang:text('main.pages.home.no_portals')"/>
                    </xsl:call-template>
                  </xsl:otherwise>
                </xsl:choose>
                <div style="clear:both;"/>
              </xsl:with-param>
            </xsl:call-template>
          </td>
          <td>
            <xsl:call-template name="block">
              <xsl:with-param name="title" select="lang:text('~main.pages.home.subscribed_users')" />
              <xsl:with-param name="content">
                <xsl:choose>
                  <xsl:when test="subscribedPortals/portal[@id = @pov and @pov != //@session_user]">
                    <div>
                      <xsl:for-each select="subscribedPortals/portal[@id = @pov and @pov != //@session_user]">
                        <xsl:call-template name="tile-portal"/>
                      </xsl:for-each>
                    </div>
                  </xsl:when>
                  <xsl:otherwise>
                    <div class="os_content" style="padding:2em;text-align:center">
                      <xsl:value-of select="lang:text('~main.pages.home.no_users')"/>
                    </div>
                    <xsl:call-template name="help-box">
                      <xsl:with-param name="text" select="lang:text('~main.pages.home.no_users')"/>
                    </xsl:call-template>
                  </xsl:otherwise>
                </xsl:choose>
                <div style="clear:both;"/>
              </xsl:with-param>
            </xsl:call-template>
          </td>
        </tr>
      </table>
      <div style="clear:both;">       
        <div style="clear:both;margin-top:200px;">
        <xsl:call-template name="block">
          <xsl:with-param name="title" select="lang:text('main.pages.home.subscribed_portals')" />
          <xsl:with-param name="content">
            <xsl:choose>
              <xsl:when test="subscribedPortals">
                <table class="os_table_data">
                  <xsl:call-template name="row-header"/>
                  <xsl:for-each select="subscribedPortals/portal">
                    <xsl:sort select="@name" order="ascending"/>
                    <xsl:call-template name="row-portal"/>
                    
                  </xsl:for-each>
                </table>                
              </xsl:when>
              <xsl:otherwise>
                <xsl:call-template name="help-box">
                  <xsl:with-param name="text" select="lang:text('main.pages.home.no_portals')"/>
                </xsl:call-template>
              </xsl:otherwise>
            </xsl:choose>

          </xsl:with-param>
        </xsl:call-template>
        </div>
      </div>
      <div class="os_home_footer">        
        <span class="os_label">
          <xsl:value-of select="lang:text('main.pages.home.current_time')"/>
          <xsl:text> : </xsl:text>
        </span>
        <span class="os_value">
          <xsl:value-of select="date:long-datetime(date:current-datetime())"/>
        </span>
      </div>
      <div class="os_commands_right">
        <xsl:if test="actions/action[@name='create']">
          <a class="os_button" href="{actions/action[@name='create']/@href}" data-os-tooltip="{lang:text('main.pages.create.description')}">
          	<img src="{system:resource-url('images/icons/16x16/create.png')}" />
            <xsl:value-of select="lang:text('main.pages.home.actions.create')"/>
          </a>
        </xsl:if>
        <a class="os_button" href="{actions/action[@name='subscribe']/@href}" data-os-tooltip="{lang:text('main.pages.subscribe.description')}">
        	<img src="{system:resource-url('images/icons/16x16/subscribe.png')}" />
          <xsl:value-of select="lang:text('main.pages.home.actions.subscribe')"/>
        </a>
      </div>

      <div style="margin:100px;"/>

      <div id="more_dialog" title="{lang:text('main.pages.home.more.title')}" style="display:none;width:500px;">
        <xsl:call-template name="actions-row">
          <xsl:with-param name="prefix" select="'main.pages'"/>
        </xsl:call-template>
        <xsl:for-each select="actions">

        </xsl:for-each>
      </div>


    </div>
    
  </xsl:template>

  <xsl:template name="row-header">
    <tr>
      <th width="100%">
        <xsl:value-of select="lang:text('portal.name')"/>
      </th>
      <th class="os_center os_nowrap">
        <xsl:value-of select="lang:text('main.pages.home.last_object_date')"/>
      </th>
      <th class="os_center os_nowrap">
        <xsl:value-of select="lang:text('main.pages.home.actions')"/>
      </th>
    </tr>
  </xsl:template>

  <xsl:template name="row-portal">
    <tr>
      <td>
        <div class="os_home_portal_icons">                    
          <xsl:if test="@publicEnabled">
            <xsl:choose>              
              <xsl:when test="@default = 'true'">
                <xsl:variable name="desc" select="lang:text('main.pages.home.portals.default.on')"/>
                <img src="{system:resource-url('images/icons/16x16/default_on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:variable name="desc" select="lang:text('main.pages.home.portals.default.off')"/>
                <img src="{system:resource-url('images/icons/16x16/default_off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
              </xsl:otherwise>
            </xsl:choose>
            <xsl:choose>
              <xsl:when test="@publicEnabled = 'true'">
                <xsl:variable name="desc" select="lang:text('main.pages.home.portals.publicEnabled.on')"/>
                <img src="{system:resource-url('images/icons/16x16/public_on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:variable name="desc" select="lang:text('main.pages.home.portals.publicEnabled.off')"/>
                <img src="{system:resource-url('images/icons/16x16/public_off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:if>
          <xsl:choose>
            <xsl:when test="@exchangeEnabled = 'true'">              
              <xsl:variable name="desc" select="lang:text('main.pages.home.portals.exchangeEnabled.on')"/>
              <img src="{system:resource-url('images/icons/16x16/p2p_on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:variable name="desc" select="lang:text('main.pages.home.portals.exchangeEnabled.off')"/>
              <img src="{system:resource-url('images/icons/16x16/p2p_off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
            </xsl:otherwise>
          </xsl:choose>
          <xsl:if test="isis">
            <br/>
            <xsl:for-each select="isis/isis">
              <img>
                <xsl:choose>
                  <xsl:when test="@enabled='1'">
                    <xsl:attribute name="src">
                      <xsl:value-of select="system:resource-url('images/icons/16x16/isis_on.png')"/>
                    </xsl:attribute>
                  </xsl:when>
                  <xsl:otherwise>
                    <xsl:attribute name="src">
                      <xsl:value-of select="system:resource-url('images/icons/16x16/isis_off.png')"/>
                    </xsl:attribute>
                  </xsl:otherwise>
                </xsl:choose>
                <xsl:attribute name="data-os-tooltip">
                  <xsl:value-of select="concat(@name,'&lt;br&gt; ( ',@url,' )')"/>
                </xsl:attribute>
              </img>              
            </xsl:for-each>
          </xsl:if>          
        </div>
        <xsl:call-template name="portal-box"/>        
      </td>            
      <td class="os_center os_nowrap">
        <xsl:choose>
          <xsl:when test="@exchangeEnabled = 'true'">
            <xsl:value-of select="date:userfriendly-datetime(@lastObjectDate)"/>  
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="lang:text('main.pages.home.disabled_exchange')"/>
          </xsl:otherwise>
        </xsl:choose>
      </td>
      <td class="os_center os_nowrap">
        <!--
        <xsl:for-each select="actions/action">          
          <xsl:call-template name="action-link">
            <xsl:with-param name="suffix" select="'main.pages.home'"/>
          </xsl:call-template>
        </xsl:for-each>
        -->

        <xsl:call-template name="actions-popup">
          <xsl:with-param name="prefix" select="'main.pages.home.actions'"/>
          <xsl:with-param name="actions" select="actions"/>
          <xsl:with-param name="default" select="'enter'"/>
        </xsl:call-template>
      </td>
    </tr>
  </xsl:template>

  <xsl:template name="tile-portal">
    <div id="{@id}_{@pov}_dialog" style="display:none">
      <h1>
        <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes" />
      </h1>      
      <xsl:value-of select="system:parse(@description, false(), false(), true())" disable-output-escaping="yes" />
    </div>

    <div id="{@id}_{@pov}" class="os_home_tile_portal os_block_item" style="background-color:{concat('#',substring(@id,1,6))};">
      <div class="os_home_tile_portal_area1">
        <a class="os_button_link" style="float:right;" href="{actions/action[@name='info']/@href}">
          <img src="{system:resource-url('images/icons/24x24/more.png')}"/>
        </a>

        <a class="os_home_tile_portal_name" href="{actions/action[@name='enter']/@href}">
          <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes" />
        </a>
        <div class="os_home_tile_portal_description">
          <xsl:value-of select="system:parse(@description, false(), false(), true())" disable-output-escaping="yes" />
        </div>
      </div>

      <div class="os_home_tile_portal_area2">
        <div>
          <div class="os_home_portal_icons">
            <xsl:if test="@publicEnabled">
              <xsl:choose>
                <xsl:when test="@default = 'true'">
                  <xsl:variable name="desc" select="lang:text('main.pages.home.portals.default.on')"/>
                  <img src="{system:resource-url('images/icons/16x16/default_on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:variable name="desc" select="lang:text('main.pages.home.portals.default.off')"/>
                  <img src="{system:resource-url('images/icons/16x16/default_off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
                </xsl:otherwise>
              </xsl:choose>
              <xsl:choose>
                <xsl:when test="@publicEnabled = 'true'">
                  <xsl:variable name="desc" select="lang:text('main.pages.home.portals.publicEnabled.on')"/>
                  <img src="{system:resource-url('images/icons/16x16/public_on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
                </xsl:when>
                <xsl:otherwise>
                  <xsl:variable name="desc" select="lang:text('main.pages.home.portals.publicEnabled.off')"/>
                  <img src="{system:resource-url('images/icons/16x16/public_off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
                </xsl:otherwise>
              </xsl:choose>
            </xsl:if>
            <xsl:choose>
              <xsl:when test="@exchangeEnabled = 'true'">
                <xsl:variable name="desc" select="lang:text('main.pages.home.portals.exchangeEnabled.on')"/>
                <img src="{system:resource-url('images/icons/16x16/p2p_on.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:variable name="desc" select="lang:text('main.pages.home.portals.exchangeEnabled.off')"/>
                <img src="{system:resource-url('images/icons/16x16/p2p_off.png')}" alt="{$desc}" data-os-tooltip="{$desc}"/>
              </xsl:otherwise>
            </xsl:choose>
            <xsl:if test="isis">
              <br/>
              <xsl:for-each select="isis/isis">
                <img>
                  <xsl:choose>
                    <xsl:when test="@enabled='1'">
                      <xsl:attribute name="src">
                        <xsl:value-of select="system:resource-url('images/icons/16x16/isis_on.png')"/>
                      </xsl:attribute>
                    </xsl:when>
                    <xsl:otherwise>
                      <xsl:attribute name="src">
                        <xsl:value-of select="system:resource-url('images/icons/16x16/isis_off.png')"/>
                      </xsl:attribute>
                    </xsl:otherwise>
                  </xsl:choose>
                  <xsl:attribute name="data-os-tooltip">
                    <xsl:value-of select="concat(@name,'&lt;br&gt; ( ',@url,' )')"/>
                  </xsl:attribute>
                </img>
              </xsl:for-each>
            </xsl:if>
          </div>

          <xsl:choose>
            <xsl:when test="@exchangeEnabled = 'true'">
              <xsl:value-of select="date:userfriendly-datetime(@lastObjectDate)"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="lang:text('main.pages.home.disabled_exchange')"/>
            </xsl:otherwise>
          </xsl:choose>
        </div>
      </div>


    </div>
    
    <!--
        <xsl:for-each select="actions/action">          
          <xsl:call-template name="action-link">
            <xsl:with-param name="suffix" select="'main.pages.home'"/>
          </xsl:call-template>
        </xsl:for-each>
        -->

    <!--
        <xsl:call-template name="actions-popup">
          <xsl:with-param name="prefix" select="'main.pages.home.actions'"/>
          <xsl:with-param name="actions" select="actions"/>
          <xsl:with-param name="default" select="'enter'"/>
        </xsl:call-template>
        -->

    
  </xsl:template>
  
</xsl:stylesheet>