<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="os lang date system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
    
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="notes"/>

  <xsl:template match="assistant">
    <div id="assistant" class="os_assistant" style="display:none;" data-os-drag="drag">
      <!--     
      La versione con i div va in conflitto con le routine di drag in IE6.
      <div id="assistant_dragArea" class="os_header">
        <img class="os_assistant_closeDetail" onclick="javascript:Osiris.Assistant.toggle();" src="{system:resource-url('images/close_panel.gif')}"/>
        <span>Assistant</span>        
      </div>      
      -->
      <table style="width:100%" id="assistantDragArea" class="os_assistant_header">
        <tr>
          <td style="width:100%;text-align:center">
            <span>
              <xsl:value-of select="lang:text('assistant.title')"/>
            </span>
          </td>
          <td>
            <img class="os_assistant_closeDetail" onclick="javascript:Osiris.Assistant.toggle();" src="{system:resource-url('images/close_panel.gif')}"/>
          </td>
        </tr>
      </table>
      <div id="assistant_tab" class="os_assistant_tab" data-os-storage="assistant" data-os-tabSize="80">
        <div class="os_assistant_quickactions">
        	<!--
          <a id="os_assistant_quickactions_viewmode" data-os-tooltip="{lang:text('help.assistant.pages.common.actions.view')}" href="{@viewmode_href}">
            <img src="{system:resource-url('images/icons/16x16/viewmode.png')}"/>
          </a>
          -->
          <xsl:text>   </xsl:text>
          <a id="os_assistant_quickactions_view" data-os-tooltip="{lang:text('help.assistant.pages.common.actions.view')}" href="javascript:Osiris.Assistant.objectPicker('onObjectView')">
            <img src="{system:resource-url('images/icons/16x16/view.png')}"/>
          </a>
          <xsl:text> </xsl:text>
          <a id="os_assistant_quickactions_edit" data-os-tooltip="{lang:text('help.assistant.pages.common.actions.edit')}" href="javascript:Osiris.Assistant.objectPicker('onObjectEdit')">
            <img src="{system:resource-url('images/icons/16x16/edit.png')}"/>
          </a>
          <!--
          <a data-os-tooltip="{lang:text('help.assistant.pages.common.actions.details')}" href="javascript:Osiris.Assistant.objectPicker(Osiris.Assistant.showDetail)">
            <img src="{system:resource-url('images/icons/16x16/details.png')}"/>
          </a>
          -->
        </div>
        <div data-os-tabType="header">
          <xsl:value-of select="lang:text('assistant.pages.informations')"/>
        </div>
        <div data-os-tabType="header">
          <xsl:value-of select="lang:text('assistant.pages.search')"/>
        </div>
        <xsl:if test="@guest = 'false'">
          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('assistant.pages.objects')"/>
          </div>
          <div data-os-tabType="header">
            <xsl:value-of select="lang:text('assistant.pages.notes')"/>
          </div>
        </xsl:if>
        <div data-os-tabType="body">
            <xsl:choose>
              <xsl:when test="object">
                <xsl:apply-templates select="object" mode="assistant"/>
              </xsl:when>
              <xsl:otherwise>
                <fieldset>
                  <legend>
                    <b>
                      <xsl:value-of select="lang:text('assistant.informations.page')"/>
                    </b>
                  </legend>
                    <div style="float:right;">
                      <img src="{system:resource-url('images/objects/special_page.png')}"/>
                    </div>
                    <div style="overflow:auto;">
                    <table class="os_table_properties" style="">
                      <xsl:call-template name="row-information">
                        <xsl:with-param name="name" select="'object.title'"/>
                        <xsl:with-param name="content">
                          <xsl:value-of select="system:parse(page/@title, false(), false(), true())" disable-output-escaping="yes" />
                        </xsl:with-param>
                      </xsl:call-template>
                      <xsl:call-template name="row-information">
                        <xsl:with-param name="name" select="'object.description'"/>
                        <xsl:with-param name="value">
                          <xsl:value-of select="system:parse(page/@description, false(), false(), true())" disable-output-escaping="yes" />
                        </xsl:with-param>
                      </xsl:call-template>
                    </table>
                    </div>
                    <div class="os_commands_right">
                      <xsl:call-template name="copy-paste">
                        <xsl:with-param name="title" select="'object.actions.oml.link'"/>
                        <xsl:with-param name="body" select="page/@oml_code"/>
                      </xsl:call-template>                      
                    </div>
                    <div style="clear:both"/>

                  
                </fieldset> 
              </xsl:otherwise>
            </xsl:choose>      
            
            
            <!--
            <xsl:choose>              
              <xsl:otherwise>
                <xsl:apply-templates select="root" mode="assistant"/>
              </xsl:otherwise>              
            </xsl:choose>
            -->
            
            <xsl:if test="object">
              <xsl:call-template name="action-row">
                <xsl:with-param name="prefix" select="'portal.pages'"/>
                <xsl:with-param name="name" select="'view'"/>
                <xsl:with-param name="icon" select="'view'"/>
                <xsl:with-param name="href" select="object/@view_href"/>
              </xsl:call-template>
            </xsl:if>

            <xsl:choose>
              <xsl:when test="object">
                <xsl:call-template name="action-row">
                  <xsl:with-param name="prefix" select="'portal.pages'"/>
                  <xsl:with-param name="name" select="'edit'"/>
                  <xsl:with-param name="icon" select="'edit'"/>
                  <xsl:with-param name="href" select="object/@edit_href"/>
                </xsl:call-template>
              </xsl:when>
              <xsl:when test="@edit_href">
                <xsl:call-template name="action-row">
                  <xsl:with-param name="prefix" select="'portal.pages'"/>
                  <xsl:with-param name="name" select="'edit'"/>
                  <xsl:with-param name="icon" select="'edit'"/>
                  <xsl:with-param name="href" select="@edit_href"/>
                </xsl:call-template>
              </xsl:when>
            </xsl:choose>
            
            
            <xsl:call-template name="action-row">
              <xsl:with-param name="prefix" select="'portal.pages'"/>
              <xsl:with-param name="name" select="'more'"/>
              <xsl:with-param name="icon" select="'more'"/>
              <xsl:with-param name="call" select="@more_href"/>
            </xsl:call-template>
            
          
          
        </div>
        
        <div data-os-tabType="body">
            <table>
              <tr>
              	<xsl:variable name="start_text" select="lang:text('assistant.search')"/>
                <td style="width:100%;">                  
                  <input style="width:100%" id="assistant_search" value="{$start_text}" onfocus="javascript:if (this.value == '{$start_text}') this.value = '';" Onkeypress="javascript:if (osGetEvent(event).keyCode != 13)  return true; Osiris.Assistant.doSearch('assistant_search', '{@search_href}','{$start_text}',true); return false;"/>
                </td>
                <td style="vertical-align:middle;">
                  <img style="padding-left:10px;cursor:pointer;" src="{system:resource-url('images/quick_search.gif')}" onclick="javascript:Osiris.Assistant.doSearch('assistant_search', '{@search_href}','{$start_text}',true);"/>
                </td>
              </tr>
            </table>
            <div class="os_assistant_actions">
              <a class="os_button_link" href="{@search_href}">
                <img src="{system:resource-url('images/small/related.gif')}"/>
                <span data-os-tooltip="{lang:text('help.assistant.pages.search.actions.advanced')}">
                  <xsl:value-of select="lang:text('assistant.pages.search.actions.advanced')"/>
                </span>
              </a>
            </div>
            <div id="assistant_search_results" style="overflow:auto;max-height:500px;">
              
            </div>
          
        </div>
        <xsl:if test="@guest = 'false'">
          <div data-os-tabType="body">
              <fieldset>
                <legend>
                  <xsl:value-of select="lang:text('assistant.pages.objects.title')"/>
                </legend>
                <div class="os_assistant_objects">
                  <xsl:choose>
                    <xsl:when test="objects">
                      <xsl:apply-templates select="objects/object" mode="row-tech"/>
                    </xsl:when>
                    <xsl:otherwise>
											<div class="os_message_nodata">
												<xsl:value-of select="lang:text('assistant.pages.objects.none')"/>
											</div>
                    </xsl:otherwise>
                  </xsl:choose>
                </div>
              </fieldset>
              <div class="os_assistant_actions">
                <a class="os_button_link" href="javascript:Osiris.Assistant.objectPicker('onObjectAdd')">
                  <img src="{system:resource-url('images/small/add.gif')}"/>
                  <span data-os-tooltip="{lang:text('help.assistant.pages.objects.actions.add')}">
                    <xsl:value-of select="lang:text('assistant.pages.objects.actions.add')"/>
                  </span>
                </a>
                <a class="os_button_link" href="javascript:Osiris.Assistant.objectPicker('onObjectRemove')">
                  <img src="{system:resource-url('images/small/remove.gif')}"/>
                  <span data-os-tooltip="{lang:text('help.assistant.pages.objects.actions.remove')}">
                    <xsl:value-of select="lang:text('assistant.pages.objects.actions.remove')"/>
                  </span>
                </a>
                <a class="os_button_link" href="{@objectsClear_href}">
                  <img src="{system:resource-url('images/small/clear.gif')}"/>
                  <span data-os-tooltip="{lang:text('help.assistant.pages.objects.actions.clearAll')}">
                    <xsl:value-of select="lang:text('assistant.pages.objects.actions.clearAll')"/>
                  </span>
                </a>
              </div>
            
          </div>
          <div data-os-tabType="body">
            
              <xsl:value-of select="$notes" disable-output-escaping="yes"/>
              <textarea id="assistantNotes" class="os_assistant_notes" onchange="javascript:Osiris.Assistant.notesChange();"></textarea>
              <div class="os_assistant_notes_warning">                
                <xsl:value-of select="lang:text('assistant.notes.warning')"/>
              </div>
            
          </div>
        </xsl:if>
      </div>      
    </div>    
  </xsl:template>

  <xsl:template match="object" mode="assistant">
    <fieldset>
      <legend>
        <b>
          <xsl:value-of select="lang:text('assistant.informations.context')"/>
        </b>
      </legend>
      <xsl:call-template name="object-box-tech"/>
    </fieldset>
  </xsl:template>


  <xsl:template match="root" mode="assistant">
    <fieldset>
      <legend>
        <b>
          <xsl:value-of select="lang:text('assistant.informations.portal')"/>
        </b>
      </legend>

      <table>
        <tr>
          <td style="width:50px; text-align:center; padding-right:10px;">
            <xsl:call-template name="object-icon"/>
          </td>
          <td style="padding-left:10px;">

          </td>
        </tr>
      </table>



    </fieldset>
  </xsl:template>
</xsl:stylesheet>
