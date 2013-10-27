<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system"                 
                exclude-result-prefixes="lang system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>  
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/results.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>
  
  <xsl:template match="objectsPicker">

    
    <xsl:apply-templates select="object"/>
  </xsl:template>

  <xsl:template match="object">
    <div style="border:1px solid gray;margin:0px;padding:0px;background-color:white">
      <xsl:attribute name="class">
        <xsl:choose>
          <xsl:when test="picker/@selected = 'true'">
            os_objectspicker_item_select
          </xsl:when>
          <xsl:otherwise>
            os_objectspicker_item_unselect
          </xsl:otherwise>
        </xsl:choose>
      </xsl:attribute>
      <xsl:choose>
        <xsl:when test="(picker/@open = 'false') or childs/object">
          <a href="{picker/@open_href}">
            <xsl:choose>
              <xsl:when test="picker/@open = 'true'">
                <img class="os_objectspicker_icon_open" src="{system:resource-url('images/small/collapse.gif')}"/>
              </xsl:when>
              <xsl:otherwise>
                <img class="os_objectspicker_icon_open" src="{system:resource-url('images/small/expand.gif')}"/>
              </xsl:otherwise>
            </xsl:choose>
          </a>
        </xsl:when>
        <xsl:otherwise>
          <img class="os_objectspicker_icon_open" src="{system:resource-url('images/small/null.png')}"/>          
        </xsl:otherwise>
      </xsl:choose>
      
      <a href="{picker/@select_href}">
        <xsl:choose>
          <xsl:when test="picker/@selected = 'true'">
            <img class="os_objectspicker_icon_select" src="{system:resource-url('images/small/check_on.png')}"/>
          </xsl:when>
          <xsl:otherwise>
            <img class="os_objectspicker_icon_select" src="{system:resource-url('images/small/check_off.png')}"/>
          </xsl:otherwise>
        </xsl:choose>
      </a>
      <xsl:call-template name="object-icon">
        <xsl:with-param name="object" select="."/>
        <xsl:with-param name="size" select="'16px'"/>
      </xsl:call-template>
      <xsl:text> </xsl:text>
      <xsl:value-of select="@title"/>
      <xsl:if test="@entity=''">
        root
      </xsl:if>
    </div>
    <div class="os_objectspicker_childs">
      <xsl:apply-templates select="childs/object"/>
    </div>

  </xsl:template>
</xsl:stylesheet>


