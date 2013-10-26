<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl" />
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/htmlsnippet">
    <xsl:call-template name="help-box">
      <xsl:with-param name="text" select="lang:text(concat('extensions.',//@extension_id,'.page.help'))" />
    </xsl:call-template>

    <div style="text-align:center;padding:10px;">
      <span class="os_label">
        <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.action_global'))"/>        
        <xsl:text> : </xsl:text>
      </span>
      <span class="os_value">
        <select onchange="javascript:return osAutoPostBack(this);" name="action_global" id="action_global">
          <option value="">
            <xsl:if test="@action_global = ''">
              <xsl:attribute name="selected">selected</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.default'))"/>
          </option>
          <option value="no">
            <xsl:if test="@action_global = 'no'">
              <xsl:attribute name="selected">selected</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.no'))"/>
          </option>
          <option value="ask">
            <xsl:if test="@action_global = 'ask'">
              <xsl:attribute name="selected">selected</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.ask'))"/>
          </option>
          <option value="yes">
            <xsl:if test="@action_global = 'yes'">
              <xsl:attribute name="selected">selected</xsl:attribute>
            </xsl:if>
            <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.yes'))"/>
          </option>
        </select>        
      </span>
    </div>

    <div>
      <xsl:apply-templates select="snippet[@category != 'xxx']"/>
    </div>

    <div style="clear:both;"/>

    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title" select="'XXX - Adult only'" />
      <xsl:with-param name="startClose" select="true()" />
      <xsl:with-param name="content">
        <xsl:apply-templates select="snippet[@category = 'xxx']"/>
        <div style="clear:both;"/>
      </xsl:with-param>
    </xsl:call-template>
    
    <div style="clear:both;"/>
    
    <xsl:call-template name="help-box">
      <xsl:with-param name="text" select="lang:text(concat('extensions.',//@extension_id,'.page.help2'))" />
    </xsl:call-template>


  </xsl:template>

  <xsl:template match="snippet">
    <div class="os_htmlsnippet_box os_htmlsnippet_box_{@security}">
      <img class="os_htmlsnippet_box_logo" src="{concat('/',//@extension_id,'/images/snippets/',@code,'.png')}" alt="{@name} - Isis Code: {@code}" title="{@name} - Isis Code: {@code}"/>
      <div class="os_htmlsnippet_box_name">
        <xsl:value-of select="@name"/>
        <xsl:if test="@notes = 'true'">
          <xsl:call-template name="help-tooltip">
            <xsl:with-param name="text" select="lang:text(concat('extensions.',//@extension_id,'.snippets.',@code,'.help'))" />
          </xsl:call-template>
        </xsl:if>
      </div>
      <div class="os_htmlsnippet_box_url">
          <a target="_blank" href="{@href}">
            <xsl:value-of select="@url"/>
          </a>          
        </div>
      <div class="os_htmlsnippet_box_details">
        <table>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.security'))"/>
              <xsl:text> : </xsl:text>
            </td>
            <td class="os_value">
              <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.security.',@security))"/>              
              <xsl:call-template name="help-tooltip">
                <xsl:with-param name="text" select="lang:text(concat('extensions.',//@extension_id,'.security.',@security,'.help'))" />
              </xsl:call-template>
            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.action'))"/>
              <xsl:text> : </xsl:text>
            </td>
            <td class="os_value">

              <select onchange="javascript:return osAutoPostBack(this);" name="action_{@code}" id="action_{@code}">
                <option value="">
                  <xsl:if test="@action = ''">
                    <xsl:attribute name="selected">selected</xsl:attribute>
                  </xsl:if>
                  <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.default'))"/>
                </option>
                <option value="no">
                  <xsl:if test="@action = 'no'">
                    <xsl:attribute name="selected">selected</xsl:attribute>
                  </xsl:if>
                  <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.no'))"/>
                </option>
                <option value="ask">
                  <xsl:if test="@action = 'ask'">
                    <xsl:attribute name="selected">selected</xsl:attribute>
                  </xsl:if>
                  <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.ask'))"/>
                </option>
                <option value="yes">
                  <xsl:if test="@action = 'yes'">
                    <xsl:attribute name="selected">selected</xsl:attribute>
                  </xsl:if>
                  <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.action.yes'))"/>
                </option>
              </select>

            </td>
          </tr>
          <tr>
            <td class="os_label">
              <xsl:value-of select="lang:text(concat('extensions.',//@extension_id,'.label.current'))"/>
              <xsl:text> : </xsl:text>
            </td>
            <td class="os_value">
              <xsl:variable name="desc" select="lang:text(concat('extensions.',//@extension_id,'.action.',@status))" />
              <xsl:choose>
                <xsl:when test="@status = 'yes'">
                  <img src="{system:resource-url('images/icons/16x16/on.png')}" alt="{$desc}" data-os-tooltip="{$desc}" />
                </xsl:when>
                <xsl:when test="@status = 'session'">
                  <img src="{system:resource-url('images/icons/16x16/on.png')}" alt="{$desc}" data-os-tooltip="{$desc}" />
                  sessione
                </xsl:when>
                <xsl:when test="@status = 'ask'">
                  <img src="{system:resource-url('images/icons/16x16/help.png')}" alt="{$desc}" data-os-tooltip="{$desc}" />
                </xsl:when>
                <xsl:when test="@status = 'no'">
                  <img src="{system:resource-url('images/icons/16x16/off.png')}" alt="{$desc}" data-os-tooltip="{$desc}" />
                </xsl:when>
                <xsl:otherwise>                  
                </xsl:otherwise>
              </xsl:choose>

            </td>
          </tr>
        </table>        
      </div>      
      
    </div>
  </xsl:template>

  
</xsl:stylesheet>