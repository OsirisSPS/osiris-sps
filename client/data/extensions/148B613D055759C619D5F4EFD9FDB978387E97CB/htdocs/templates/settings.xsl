<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="name"/>
  <xsl:param name="description"/>
  <xsl:param name="exchangeEnabled"/>
  <xsl:param name="publicDefault"/>
  <xsl:param name="publicEnabled"/>
  <xsl:param name="loginEnabled"/>
  <xsl:param name="registerEnabled"/>
  <xsl:param name="save"/>
  <xsl:param name="isisName"/>
  <xsl:param name="isisUrl"/>
  <xsl:param name="isisEnabled"/>
  <xsl:param name="isisPassword"/>
  <xsl:param name="sync"/>

  <xsl:template match="settings">

    <xsl:call-template name="block_big">
      <xsl:with-param name="title" select="lang:text('main.pages.settings.title')"/>        
      <xsl:with-param name="content">
        <div style="display:none" data-os-otype="tab" data-os-storage="portal.settings">
          <xsl:attribute name="data-os-startPage">
            <xsl:choose>
              <xsl:when test="@showIsis">
                <xsl:text>1</xsl:text>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>0</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </xsl:attribute>
          <xsl:call-template name="optionsPage"/>
          <xsl:call-template name="isisPage"/>
          <xsl:call-template name="advancedPage"/>          
        </div>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="optionsPage">
    <div data-os-tabType="header" data-os-tabPage="0">
      <xsl:value-of select="lang:text('main.pages.settings.configuration')"/>
    </div>
    <div data-os-tabType="body" data-os-tabPage="0">
      <table class="os_table_properties">
        <tr>
          <td>
            <xsl:value-of select="lang:text('main.pages.settings.name')"/>
          </td>
          <td>
            <xsl:value-of select="$name" disable-output-escaping="yes"/>
          </td>
        </tr>        
        <tr>
          <td>
            <xsl:value-of select="lang:text('main.pages.settings.enableExchange')"/>
          </td>
          <td>
            <xsl:value-of select="$exchangeEnabled" disable-output-escaping="yes"/>
          </td>
        </tr>
        <xsl:if test="$publicDefault">
          <tr>
            <td>
              <xsl:value-of select="lang:text('main.pages.settings.publicDefault')"/>
            </td>
            <td>
              <xsl:value-of select="$publicDefault" disable-output-escaping="yes"/>
            </td>
          </tr>
        </xsl:if>
        <xsl:if test="$publicEnabled">
          <tr>
            <td>
              <xsl:value-of select="lang:text('main.pages.settings.publicEnabled')"/>
            </td>
            <td>
              <xsl:value-of select="$publicEnabled" disable-output-escaping="yes"/>
            </td>
          </tr>
        </xsl:if>
        <xsl:if test="$loginEnabled">
          <tr>
            <td>
              <xsl:value-of select="lang:text('main.pages.settings.loginEnabled')"/>
            </td>
            <td>
              <xsl:value-of select="$loginEnabled" disable-output-escaping="yes"/>
            </td>
          </tr>
        </xsl:if>
        <xsl:if test="$registerEnabled">
          <tr>
            <td>
              <xsl:value-of select="lang:text('main.pages.settings.registerEnabled')"/>
            </td>
            <td>
              <xsl:value-of select="$registerEnabled" disable-output-escaping="yes"/>
            </td>
          </tr>
        </xsl:if>
      </table>
      <div class="os_commands">
        <a class="os_button" href="{actions/action[@name='saveOptions']/@href}">
          <xsl:value-of select="lang:text('main.pages.settings.actions.save')" disable-output-escaping="yes"/>
        </a>
        <a class="os_button" href="home">
          <xsl:value-of select="lang:text('main.pages.settings.actions.cancel')" disable-output-escaping="yes"/>
        </a>
      </div>
    </div>
  </xsl:template>

  <xsl:template name="isisPage">
    <div data-os-tabType="header" data-os-tabPage="1">
      <xsl:value-of select="lang:text('main.pages.settings.isis')"/>
    </div>
    <div data-os-tabType="body" data-os-tabPage="1">

      <xsl:if test="isisEndpoints">
        <div class="os_content_box">
          <table class="os_table_data">

            <th style="width:100%;">
              <xsl:value-of select="lang:text('main.pages.settings.isis.name')"/>
            </th>
            <th style="width:100%;">
              <xsl:value-of select="lang:text('main.pages.settings.isis.url')"/>
            </th>
            <th style="width:100%;">
              <xsl:value-of select="lang:text('main.pages.settings.isis.enabled')"/>
            </th>
            <th style="width:100%;">
              <xsl:value-of select="lang:text('main.pages.settings.isis.password')"/>
            </th>
            <th style="width:100%;">
              <xsl:value-of select="lang:text('main.pages.settings.isis.last_event')"/>
            </th>
            <th style="width:100%;">
              <xsl:value-of select="lang:text('main.pages.settings.isis.actions')"/>
            </th>

            <xsl:for-each select="isisEndpoints/isisEndpoint">
              <tr>
                <td style="width:1%;">
                  <xsl:value-of select="@name"/>
                </td>
                <td class="os_center os_nowrap">
                  <xsl:value-of select="@url"/>
                </td>
                <td class="os_center os_nowrap">
                  <xsl:value-of select="@enabled"/>
                </td>
                <td class="os_center os_nowrap">
                  <xsl:value-of select="@password"/>
                </td>
                <td class="os_center os_nowrap">
                  <xsl:value-of select="@lastEvent"/>
                </td>
                <td class="os_center os_nowrap">
                  <xsl:call-template name="actions-popup">
                    <xsl:with-param name="prefix" select="'common.actions'"/>
                    <xsl:with-param name="actions" select="actions"/>
                    <xsl:with-param name="default" select="'edit'"/>
                  </xsl:call-template>
                </td>
              </tr>
            </xsl:for-each>
          </table>
        </div>
      </xsl:if>


      <table class="os_table_properties">
        <th colspan="2">
          <xsl:text>Aggiungi Isis</xsl:text>
        </th>
        <tr>
          <td>
            <xsl:value-of select="lang:text('common.labels.name')"/>
          </td>
          <td>
            <xsl:value-of select="$isisName" disable-output-escaping="yes"/>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="lang:text('common.labels.url')"/>
          </td>
          <td>
            <xsl:value-of select="$isisUrl" disable-output-escaping="yes"/>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="lang:text('common.labels.enabled')"/>
          </td>
          <td>
            <xsl:value-of select="$isisEnabled" disable-output-escaping="yes"/>
          </td>
        </tr>
        <tr>
          <td>
            <xsl:value-of select="lang:text('common.labels.password')"/>
          </td>
          <td>
            <xsl:value-of select="$isisPassword" disable-output-escaping="yes"/>
          </td>
        </tr>
      </table>
      <div class="os_commands">
        <xsl:for-each select="actions/action[@group='isis']">
          <a class="os_button" href="{@href}">
            <xsl:value-of select="lang:text(concat('common.actions.', @name))" disable-output-escaping="yes"/>
          </a>
        </xsl:for-each>
      </div>
    </div>
  </xsl:template>

  <xsl:template name="advancedPage">
    <div data-os-tabType="header" data-os-tabPage="0">
      <xsl:value-of select="lang:text('main.pages.settings.advanced')"/>
    </div>
    <div data-os-tabType="body" data-os-tabPage="0">
      <table class="os_table_properties">
        <tr>
          <td class="os_label">
            <xsl:value-of select="lang:text('main.pages.settings.sync')"/>
          </td>
          <td class="os_value">
            <xsl:value-of select="$sync" disable-output-escaping="yes"/>
          </td>
        </tr>
      </table>
      <div class="os_commands">
        <a class="os_button" href="{actions/action[@name='saveOptions']/@href}">
          <xsl:value-of select="lang:text('main.pages.settings.actions.save')" disable-output-escaping="yes"/>
        </a>
        <a class="os_button" href="home">
          <xsl:value-of select="lang:text('main.pages.settings.actions.cancel')" disable-output-escaping="yes"/>
        </a>
      </div>
    </div>
  </xsl:template>

</xsl:stylesheet>
