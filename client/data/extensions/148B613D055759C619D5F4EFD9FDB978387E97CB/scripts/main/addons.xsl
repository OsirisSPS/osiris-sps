<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"                
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
	
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>
  
  <xsl:template match="/upgradable">
    <xsl:if test="@upgradable_counter > 0">
      <div class="os_button_counter" data-os-tooltip="{lang:text('main.pages.addons.upgrade_available')}">
        <xsl:value-of select="@upgradable_counter"/>
      </div>
    </xsl:if>
  </xsl:template>

  <xsl:template match="/home">
		
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('main.pages.addons.title')"/>
      <xsl:with-param name="content">
        <xsl:if test="@online != 'true'">
          <xsl:call-template name="help-box">
            <xsl:with-param name="text" select="lang:text('main.pages.addons.offline')"/>
          </xsl:call-template>
        </xsl:if>

        <div id="addons_tab" style="display:none" data-os-otype="tab" data-os-layout="left" data-os-storage="main.addons">

          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('main.pages.addons.installed')"/>
          </div>
          <div data-os-tabType="body">
            <xsl:apply-templates select="lists/installed"/>
          </div>

          <xsl:if test="@online != 'false'">
            <div data-os-tabType="header">
              <xsl:value-of select="lang:text('main.pages.addons.upgrade')"/>
            </div>
            <div data-os-tabType="body">
              <xsl:apply-templates select="lists/upgradable"/>
            </div>

            <div data-os-tabType="header">
              <xsl:value-of select="lang:text('main.pages.addons.recommended')"/>
            </div>
            <div data-os-tabType="body">
              <xsl:apply-templates select="lists/recommended"/>
            </div>
            <div data-os-tabType="header">
              <xsl:value-of select="lang:text('main.pages.addons.catalog')"/>
            </div>
            <div data-os-tabType="body">
              <xsl:apply-templates select="lists/catalog"/>
            </div>
          </xsl:if>
        </div>    
      </xsl:with-param>
    </xsl:call-template>
    

  </xsl:template>

  <xsl:template match="recommended">
    <xsl:call-template name="list">
      <xsl:with-param name="mode" select="'remote'"/>
      <xsl:with-param name="list" select="'recommended'"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="catalog">
    <xsl:call-template name="list">
      <xsl:with-param name="mode" select="'remote'"/>
      <xsl:with-param name="list" select="'catalog'"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="installed">
    <xsl:call-template name="list">
      <xsl:with-param name="mode" select="'local'"/>
      <xsl:with-param name="list" select="'installed'"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="upgradable">
      <xsl:call-template name="list">
          <xsl:with-param name="mode" select="'remote'"/>
          <xsl:with-param name="list" select="'upgradable'"/>
       </xsl:call-template>
    
  </xsl:template>

  <xsl:template name="list">
    <xsl:param name="mode"/>
    <xsl:param name="list"/>
    <div id="list_{$list}">
      <xsl:for-each select="addon">
        <div id="{@id}_{$list}">
          <xsl:apply-templates select="."/>
        </div>
      </xsl:for-each>
    </div>
  </xsl:template>
  
  <xsl:template match="addon">
    <div>
      <!--<xsl:param name="list"/>-->
      <xsl:choose>
        <xsl:when test="@active != 'false'">
          <xsl:attribute name="class">
            <xsl:text>os_extension</xsl:text>
          </xsl:attribute>
        </xsl:when>
        <xsl:otherwise>
          <xsl:attribute name="class">
            <xsl:text>os_extension_disabled</xsl:text>
          </xsl:attribute>
        </xsl:otherwise>
      </xsl:choose>

      <xsl:if test="@job_id != ''">
        <div class="os_extension_job">
          <xsl:attribute name="data-os-url">
            <xsl:value-of select="concat('/main/jobdetail?id=',@job_id)"/>
          </xsl:attribute>
          <xsl:attribute name="data-os-completed">
            <xsl:text>javascript:void(extensionActionCompleted('</xsl:text>
            <xsl:value-of select="@job_action"/>
            <xsl:text>','</xsl:text>
            <xsl:value-of select="@id"/>
            <xsl:text>'));</xsl:text>
          </xsl:attribute>
        </div>
      </xsl:if>
      <xsl:if test="actions">        
        <div class="os_extension_actions">
          <xsl:call-template name="actions-popup">
            <xsl:with-param name="prefix" select="'main.pages.addons.actions'"/>
            <xsl:with-param name="actions" select="actions"/>
          </xsl:call-template>
        </div>
        <xsl:if test="actions2">
          <div class="os_extension_actions">
            <xsl:call-template name="actions-popup">
              <xsl:with-param name="prefix" select="'main.pages.addons.actions'"/>
              <xsl:with-param name="actions" select="actions2"/>
            </xsl:call-template>
          </div>
        </xsl:if>
      </xsl:if>
      <div class="os_extension_box" data-os-otype="popup" data-os-popupMode="staticclick" data-os-popup="header">
        <xsl:if test="@icon_href">
          <img class="os_extension_icon" src="{system:resource-url('images/extension_icon.png')}" data-os-delayedsrc="{@icon_href}"/>
        </xsl:if>
        <span class="os_extension_title">
          <xsl:value-of select="@name"/>
        </span>
        <xsl:if test="@experimental = 'true'">
          <img style="width:16px;height:16px;margin:5px;vertical-align:bottom;" src="{system:resource-url('images/icons/16x16/under_construction.png')}" data-os-tooltip="{lang:text('main.pages.addons.extension_experimental')}"/>        	
        </xsl:if>
        <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
        <div data-os-popup="body" style="display:none;" >          
          <div class="os_extension_detail">
            <xsl:if test="@homepage != ''">
              <xsl:value-of select="system:parse(concat('[center][url=',@homepage,']',lang:text('main.pages.addons.homepage'),'[/url][/center]'))" disable-output-escaping="yes"/>
            </xsl:if>
            <table class="os_table_properties">
              <!--
              <xsl:if test="@homepage != ''">
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('main.pages.addons.homepage')"/>
                    <xsl:text>:</xsl:text>
                  </td>
                  <td class="os_value">
                    <a href="{@homepage}">
                      <xsl:value-of select="@homepage"/>                      
                    </a>
                  </td>
                </tr>
              </xsl:if>
              -->
              <xsl:if test="@author">
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('main.pages.addons.author')"/>
                    <xsl:text>:</xsl:text>
                  </td>
                  <td class="os_value">
                    <xsl:value-of select="@author"/>
                  </td>
                </tr>
              </xsl:if>

              <xsl:if test="@version_installed">
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('main.pages.addons.installed_version')"/>
                    <xsl:text>:</xsl:text>
                  </td>
                  <td class="os_value">
                    <xsl:value-of select="@version_installed"/>
                  </td>
                </tr>
              </xsl:if>

              <xsl:if test="@version_repository">
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('main.pages.addons.repository_version')"/>
                    <xsl:text>:</xsl:text>
                  </td>
                  <td class="os_value">
                    <xsl:value-of select="@version_repository"/>                    
                  </td>
                </tr>
              </xsl:if>
              <br />
              <xsl:if test="@compatibility">
                <xsl:if test="@compatibility != 0">
                  <tr>
                    <td class="os_label">
                      <xsl:value-of select="lang:text('main.pages.addons.compatibility')"/>
                      <xsl:text>:</xsl:text>
                    </td>
                    <td class="os_value">
                      <xsl:text>>= </xsl:text>
                      <xsl:value-of select="format-number(@compatibility,'##########.##')"/>
                    </td>

                  </tr>
                </xsl:if>
              </xsl:if>
              <xsl:if test="@category != ''">
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('main.pages.addons.category')"/>
                    <xsl:text>:</xsl:text>
                  </td>
                  <td class="os_value">
                    <xsl:value-of select="@category"/>
                  </td>
                </tr>
              </xsl:if>

              <xsl:if test="@tags != ''">
                <tr>
                  <td class="os_label">
                    <xsl:value-of select="lang:text('main.pages.addons.tags')"/>
                    <xsl:text>:</xsl:text>
                  </td>
                  <td class="os_value">
                    <xsl:value-of select="@tags"/>
                  </td>
                </tr>
              </xsl:if>
            </table>
          </div>
          <xsl:if test="@logo_href">
            <img class="os_extension_logo" src="{system:resource-url('images/extension_icon.png')}" data-os-delayedsrc="{@logo_href}"/>
          </xsl:if>
          <div class="os_extension_content">
            <xsl:value-of select="system:parse(@content)" disable-output-escaping="yes"/>
          </div>
        </div>

      </div>
      
      <xsl:if test="@verified != 'true'">
        <div class="os_extension_unverified">
          <xsl:value-of select="system:parse(lang:text('main.pages.addons.unverified.warning'))" disable-output-escaping="yes"/>
        </div>
      </xsl:if>
      <xsl:if test="@trust != ''">
        <div class="os_extension_trust">
          <h3>
            <xsl:value-of select="lang:text('main.pages.addons.trust.warning')"/>
          </h3>
          <xsl:value-of select="system:parse(@trust)" disable-output-escaping="yes"/>
        </div>
      </xsl:if>
    </div>
  </xsl:template>

  <xsl:template name="dialog-confirm">
    <xsl:param name="action"/>
    <xsl:param name="mode"/>
    <xsl:param name="showTrust"/>
    <div id="addon_confirm">
      <xsl:value-of select="lang:text(concat('main.pages.addons.',$action,'.description'))"/>      
      <div data-os-url="/main/addons?act=addon&amp;id={@id}&amp;mode={$mode}&amp;trust={$showTrust}" data-os-wait="center">
      </div>
    </div>
    <xsl:variable name="message" select="lang:text(concat('main.pages.addons.',$action,'.title'))"/>
    
    <script type="text/javascript">
      <xsl:text>extensionAction('</xsl:text>
      <xsl:value-of select="system:html-encode($message)" disable-output-escaping="yes"/>
      <xsl:text>','</xsl:text>
      <xsl:value-of select="system:html-encode('addon_confirm')" disable-output-escaping="yes"/>
      <xsl:text>','</xsl:text>
      <xsl:value-of select="system:html-encode(lang:text('common.actions.confirm'))" disable-output-escaping="yes"/>
      <xsl:text>','</xsl:text>
      <xsl:value-of select="system:html-encode(lang:text('common.actions.cancel'))" disable-output-escaping="yes"/>
      <xsl:text>','</xsl:text>
      <xsl:value-of select="system:js-encode($action)"/>
      <xsl:text>','</xsl:text>
      <xsl:value-of select="system:js-encode(@id)"/>
      <xsl:text>','</xsl:text>
      <xsl:value-of select="system:js-encode(@token)"/>
      <xsl:text>');</xsl:text>      
    </script>
  </xsl:template>

  <xsl:template match="upgrade">
    <xsl:call-template name="dialog-confirm">
      <xsl:with-param name="action" select="'upgrade'"/>
      <xsl:with-param name="mode" select="'remote'"/>
      <xsl:with-param name="showTrust" select="'yes'"/>
    </xsl:call-template>    
  </xsl:template>

  <xsl:template match="uninstall">
    <xsl:call-template name="dialog-confirm">
      <xsl:with-param name="action" select="'uninstall'"/>
      <xsl:with-param name="mode" select="'local'"/>
      <xsl:with-param name="showTrust" select="'no'"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="activate">
    <xsl:call-template name="dialog-confirm">
      <xsl:with-param name="action" select="'activate'"/>
      <xsl:with-param name="mode" select="'local'"/>
      <xsl:with-param name="showTrust" select="'yes'"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="deactivate">
    <xsl:call-template name="dialog-confirm">
      <xsl:with-param name="action" select="'deactivate'"/>
      <xsl:with-param name="mode" select="'local'"/>
      <xsl:with-param name="showTrust" select="'no'"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template match="install">
    <xsl:call-template name="dialog-confirm">
      <xsl:with-param name="action" select="'install'"/>
      <xsl:with-param name="mode" select="'remote'"/>
      <xsl:with-param name="showTrust" select="'yes'"/>
    </xsl:call-template>
  </xsl:template>
  
</xsl:stylesheet>