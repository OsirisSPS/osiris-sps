<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="os lang system"
                version="1.0">

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="pathway">
    <xsl:if test="path">
      <div class="os_pathway">
        <!--
        <div class="os_pathway_toolbar">
          <a data-os-tooltip="{lang:text('help.assistant.pages.common.actions.view')}" href="javascript:osAssistant_objectPicker('onObjectView')">
            <img src="{system:resource-url('images/icons/16x16/view.png')}"/>
          </a>
          <xsl:text> </xsl:text>
          <a data-os-tooltip="{lang:text('help.assistant.pages.common.actions.edit')}" href="javascript:osAssistant_objectPicker('onObjectEdit')">
            <img src="{system:resource-url('images/icons/16x16/edit.png')}"/>
          </a>
          <xsl:text> </xsl:text>
          <a data-os-tooltip="{lang:text('help.assistant.pages.common.actions.edit')}" href="javascript:osAssistant_toggle()">
            <img src="{system:resource-url('images/icons/16x16/assistant.png')}"/>
          </a>
        </div>
        -->
        <a href="/main/home">
          <img style="margin-top:6px;" src="{system:resource-url('images/pathway_home.png')}" alt=""/>
        </a>
        <xsl:for-each select="path">
          <img src="{system:resource-url('images/pathway_sep.png')}" alt=""/>
          <a href="{@href}" data-os-entity="{@entity}">
            <xsl:value-of select="system:parse(@name, false(), false(), true())" disable-output-escaping="yes"/>
          </a>
          <!--
          <xsl:if test="position() != last()">
            <img src="{system:resource-url('images/pathway_sep.png')}" alt=""/>            
          </xsl:if>
          -->
        </xsl:for-each>
      </div>
    </xsl:if>
  </xsl:template>
 
</xsl:stylesheet>