<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="lang date system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/file_viewer.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="activity"/>

  <xsl:param name="show_system"/>
  <xsl:param name="show_invisible"/>

  <xsl:template match="edit">
    <table style="width:100%">
      <tr>
        <td style="min-width:320px">
          <xsl:call-template name="object-box-tech"/>
          <xsl:if test="author">
            <div>
              <xsl:call-template name="block_minimal">
                <xsl:with-param name="startClose" select="true()"/>
                <xsl:with-param name="storage" select="'edit_author'"/>
                <xsl:with-param name="title">
                  <xsl:value-of select="lang:text('object.author')"/>
                </xsl:with-param>
                <xsl:with-param name="content">
                  <xsl:apply-templates select="author"/>
                </xsl:with-param>
              </xsl:call-template>
            </div>
          </xsl:if>
          <xsl:if test="editor">
            <div>
              <xsl:call-template name="block_minimal">
                <xsl:with-param name="startClose" select="true()"/>
                <xsl:with-param name="storage" select="'edit_editor'"/>
                <xsl:with-param name="title">
                  <xsl:value-of select="lang:text('object.editor')"/>
                </xsl:with-param>
                <xsl:with-param name="content">
                  <xsl:apply-templates select="editor"/>
                </xsl:with-param>
              </xsl:call-template>
            </div>
          </xsl:if>
          
          <xsl:for-each select="actions/action">
            <xsl:call-template name="action-page">
              <xsl:with-param name="prefix" select="'object.actions'"/>
            </xsl:call-template>
          </xsl:for-each>

          <br/>

          <xsl:for-each select="actions2/action">
            <xsl:call-template name="action-row">
              <xsl:with-param name="prefix" select="'object.actions'"/>
              <xsl:with-param name="name" select="@name"/>
              <xsl:with-param name="icon" select="@name"/>
              <xsl:with-param name="href" select="@href"/>
            </xsl:call-template>  
          </xsl:for-each>

          <xsl:if test="@showable = 'false'">
            <xsl:call-template name="help-box">
              <xsl:with-param name="text" select="'portal.pages.edit.object_deleted.help'">                
              </xsl:with-param>
            </xsl:call-template>            
          </xsl:if>
          
        </td>
        <td style="width:100%">
          <div class="os_tab_left_bodies">            
            <xsl:choose>              
              <xsl:when test="@action = 'moveable'">
                <xsl:call-template name="block">
                  <xsl:with-param name="title">
                    <xsl:value-of select="lang:text('object.actions.move')"/>
                  </xsl:with-param>
                  <xsl:with-param name="content">
                    <xsl:for-each select="moveable/object">
                      <xsl:apply-templates select="." mode="lite"></xsl:apply-templates>
                      <div class="os_commands_right">
                        <a class="os_button" href="{@move_href}">
                          <xsl:value-of select="lang:text('portal.pages.edit.move_item')"/>
                        </a>
                      </div>
                      <div style="clear:both;" class="os_separator"></div>
                    </xsl:for-each>
                  </xsl:with-param>
                </xsl:call-template>
                
              </xsl:when>
              <xsl:when test="@action = 'overview'">
                <!-- Summary page -->
                <xsl:call-template name="block">
                  <xsl:with-param name="title" select="lang:text('portal.pages.edit.current')"/>
                  <xsl:with-param name="content">
                    <xsl:for-each select=".">
                      <xsl:call-template name="object-lite">
                        <xsl:with-param name="childs" select="false()"/>
                      </xsl:call-template>
                    </xsl:for-each>
                    <xsl:if test="@type = 'file'">
                      <xsl:call-template name="file_content">
                        <xsl:with-param name="show_authors" select="false()"/>
                      </xsl:call-template>
                    </xsl:if>
                    <div style="clear:both;"/>
                  </xsl:with-param>
                </xsl:call-template>

                <xsl:if test="childs">
                  <xsl:call-template name="block">
                    <xsl:with-param name="title" select="lang:text('portal.pages.edit.childs')"/>
                    <xsl:with-param name="content">
                      <xsl:apply-templates select="pager" mode="row"/>

                      <xsl:call-template name="browse_childs"/>

                      <xsl:apply-templates select="pager" mode="row"/>

                      <div class="os_commands_right" style="padding:10px;">
                        <xsl:if test="$show_system">
                          <xsl:value-of select="$show_system" disable-output-escaping="yes"/>
                        </xsl:if>
                        <xsl:value-of select="$show_invisible" disable-output-escaping="yes"/>
                      </div>
                      <div style="clear:both"></div>

                    </xsl:with-param>
                  </xsl:call-template>
                </xsl:if>


              </xsl:when>
              <xsl:when test="@action = 'history'">
                <xsl:call-template name="block">
                  <xsl:with-param name="title">
                    <xsl:value-of select="lang:text('object.actions.history')"/>
                  </xsl:with-param>
                  <xsl:with-param name="content">
                    <xsl:apply-templates select="history" mode="revision"/>
                  </xsl:with-param>
                </xsl:call-template>
              </xsl:when>
              <xsl:otherwise>
                <xsl:value-of select="$activity" disable-output-escaping="yes"/>
              </xsl:otherwise>
            </xsl:choose>




          </div>
        </td>
      </tr>
    </table>    
  </xsl:template>

  <xsl:template match="editor">
    <xsl:call-template name="user-full"/>
  </xsl:template>

  <xsl:template match="author">
    <xsl:call-template name="user-full"/>
  </xsl:template>
  
  <xsl:template match="history">
    <xsl:apply-templates select="object" mode="revision"/>
  </xsl:template>

  <xsl:template match="*/content/object">
    <xsl:apply-templates select="." mode="lite"/>
  </xsl:template>

  <xsl:template name="browse_childs">
    <table class="os_table_data">
      <tr>
        <th colspan="2" width="20%">
          <xsl:value-of select="lang:text('object.title')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('object.type')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('object.author')"/>
        </th>
        <th width="20%" align="center">
          <xsl:value-of select="lang:text('object.entity_date')"/>
        </th>
        <th width="10%" align="center">
          <xsl:value-of select="lang:text('object.editor')"/>
        </th>
        <th width="20%" align="center">
          <xsl:value-of select="lang:text('object.submit_date')"/>
        </th>
        <th width="20%" align="center">
          <xsl:value-of select="lang:text('object.actions')"/>
        </th>
      </tr>
      <xsl:if test="parent">
        <tr class="os_parent" >
          <td valign="middle" align="center" width="1%">
            <a class="os_nolink" href="{parent/@edit_href}">
              <img src="{system:resource-url('images/objects/utils_parent.png')}"/>
            </a>
          </td>
          <td colspan="7" >
            <b>
              <a href="{parent/@edit_href}">
                <xsl:text>..</xsl:text>
              </a>
            </b>
          </td>
        </tr>
      </xsl:if>
      <xsl:for-each select="childs/object">
        <xsl:variable name="type" select="lang:text(concat('objects.types.', @type))"/>
        <tr data-os-entity="{@entity}">
          <xsl:if test="@showable = 'false'">
            <xsl:attribute name="class">
              <xsl:text>os_object_hidden</xsl:text>
            </xsl:attribute>
          </xsl:if>
          <td valign="middle" align="center" width="1%">
            <a class="os_nolink" href="{@edit_href}">
              <xsl:call-template name="object-icon"/>
            </a>
          </td>
          <td>
            <!-- Start Image preview-->
            <xsl:if test="@type = 'file' and starts-with(@file_type,'image')">
              <div style="float:right;">
                <img src="{@file_href}" style="max-width:48px;max-height:48px;">                  
                </img>                
              </div>
            </xsl:if>
            <!-- End Image preview-->
            <b>
              <a href="{@edit_href}">
                <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes" />
              </a>
            </b>
            <xsl:if test="@description != ''">
              <br />
              <span class="os_object_description">
                <xsl:value-of select="system:parse(@description)" disable-output-escaping="yes"/>
              </span>
            </xsl:if>
          </td>
          <td align="center">
            <xsl:call-template name="object-type"/>
            <xsl:if test="@type = 'file'">              
              <div class="os_object_description">
                <xsl:value-of select="@file_type"/>
                <br/>
                <xsl:value-of select="@file_size"/>
                <xsl:text> </xsl:text>
                <xsl:value-of select="lang:text('common.bytes')"/>                
              </div>
            </xsl:if>
          </td>
          <td align="center">
            <xsl:apply-templates select="author" mode="row"/>
          </td>
          <td align="center">
            <xsl:value-of select="date:long-datetime(@entity_date)"/>
          </td>
          <td align="center">
            <xsl:choose>
              <xsl:when test="editor">
                <xsl:apply-templates select="editor" mode="row"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>-</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </td>
          <td align="center">
            <xsl:choose>
              <xsl:when test="@revisioned">
                <xsl:value-of select="date:long-datetime(@submit_date)"/>
              </xsl:when>
              <xsl:otherwise>
                <xsl:text>-</xsl:text>
              </xsl:otherwise>
            </xsl:choose>
          </td>
          <td>
            <div style="padding:3px;">
              <a class="os_button_link" href="{@view_href}">
                <img src="{system:resource-url('images/icons/16x16/view.png')}"/>
              </a>
              <a class="os_button_link" href="{@edit_href}">
                <img src="{system:resource-url('images/icons/16x16/edit.png')}"/>
              </a>
            </div>
          </td>
        </tr>
      </xsl:for-each>


    </table>
    <xsl:if test="not(childs/object)">
      <div class="os_message_nodata">
        <xsl:value-of select="lang:text('portal.pages.edit.no_childs')"/>
      </div>
    </xsl:if>
  </xsl:template>
  
</xsl:stylesheet>
