<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/object_actions.xsl"/>

  <xsl:template match="object">
    
    <div id="{@entity}_detail" class="os_assistant_objectDetail" data-os-drag="drag">
      <table width="100%" id="{@entity}_detailDragArea" class="os_assistant_header">
        <tr>
          <td style="width:100%;text-align:center">
            <xsl:value-of select="@title"/>
          </td>
          <td>
            <img class="os_assistant_closeDetail" onclick="javascript:osAssistant_toggleDetail(event, '{@entity}');" src="{system:resource-url('images/close_panel.gif')}"/>
          </td>
        </tr>
      </table>

      <div id="assistantTab" class="os_assistant_tab" data-os-otype="tab" data-os-tabSize="80">        
        <div class="os_assistant_tabHeader" data-os-tabType="header">
          <xsl:value-of select="lang:text('assistant.pages.informations')"/>
        </div>
        <div class="os_assistant_tabHeader" data-os-tabType="header">
          <xsl:value-of select="'Autori'"/>
        </div>
        <div class="os_assistant_tabHeader" data-os-tabType="header">
          <xsl:value-of select="'Stats'"/>
        </div>
        <div class="os_assistant_tabHeader" data-os-tabType="header">
          <xsl:value-of select="'Actions'"/>
        </div>
        <div style="clear:both;" />
        <div class="os_assistant_tabBody" data-os-tabType="body">
          <xsl:apply-templates select="/object" mode="lite"/>
        </div>
        <div class="os_assistant_tabBody" data-os-tabType="body">
          <table>
            <tr>
              <td>
                <fieldset>
                  <legend>Author</legend>
                  <xsl:apply-templates select="author" mode="lite"/>
                </fieldset>
              </td>
              <xsl:if test="editor">
                <td>
                  <fieldset>
                    <legend>Editor</legend>
                    <xsl:apply-templates select="editor" mode="lite"/>
                  </fieldset>
                </td>
              </xsl:if>
            </tr>
          </table>
          
        </div>
        <div class="os_assistant_tabBody" data-os-tabType="body">
          
        </div>
        <div class="os_assistant_tabBody" data-os-tabType="body">
          <xsl:call-template name="objectActions"/>
          <fieldset>
            <legend>OML</legend>            
          </fieldset>
        </div>
      </div>
    </div>
  </xsl:template>
</xsl:stylesheet>