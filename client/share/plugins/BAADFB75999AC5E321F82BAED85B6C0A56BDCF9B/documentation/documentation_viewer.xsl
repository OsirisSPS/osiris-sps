<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="documentation">
    <xsl:apply-templates select="*"/>
  </xsl:template>
  
  <xsl:template match="contents">
    <xsl:if test="count(texts/text)>0">
      <xsl:call-template name="block">
        <xsl:with-param name="title" select="@title"/>
        <xsl:with-param name="content">          
          <xsl:call-template name="toc"/>          
          <xsl:call-template name="section_detail">
            <xsl:with-param name="level" select="'0'"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
  </xsl:template>

  <xsl:template match="subindex">
    <xsl:if test="sections/section">
      <xsl:call-template name="block">
        <xsl:with-param name="title">
          <xsl:choose>
            <xsl:when test="count(../contents/texts/text)>0">
              <xsl:value-of select="'Indice sotto-sezioni'"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
        <xsl:with-param name="content">
          <xsl:call-template name="section_index" />
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    
  </xsl:template>

  <xsl:template name="section_index">
    <ul>
      <xsl:if test="boolean(//@subindex_showtexts)">
        <xsl:for-each select="texts/text">
          <li data-os-entity="{@entity}">            
            <a href="{@view_href}">
              <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
            </a>
          </li>
        </xsl:for-each>
      </xsl:if>
      <xsl:for-each select="sections/section">
        <li data-os-entity="{@entity}">          
          <a href="{@view_href}">
            <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>            
          </a>
          <xsl:call-template name="section_index"/>
        </li>
      </xsl:for-each>
    </ul>
  </xsl:template>

  <xsl:template name="toc">
    <xsl:if test="//@toc_layout != 'none'">
      <xsl:if test="count(texts/text)>=//@toc_mintexts">
        <a href="#" id="doc_toc"></a>
        <div>
          <xsl:choose>
            <xsl:when test="//@toc_layout = 'right'">
              <xsl:attribute name="class">os_ext_documentation_toc_right</xsl:attribute>
            </xsl:when>
            <xsl:when test="//@toc_layout = 'top'">
              <xsl:attribute name="class">os_ext_documentation_toc_top</xsl:attribute>
            </xsl:when>
          </xsl:choose>
          <xsl:call-template name="toc_detail"/>
        </div>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <xsl:template name="toc_detail">
    <ul>
      <xsl:for-each select="texts/text">
        <li>
          <a href="#" onclick="Osiris.scrollTo('doc_{@entity}');return false;">
            <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
          </a>
        </li>
      </xsl:for-each>
    <xsl:for-each select="sections/section">
      <li>
        <a href="#" onclick="Osiris.scrollTo('doc_{@entity}');return false;">
          <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
        </a>
        <xsl:call-template name="toc_detail"/>
      </li>
    </xsl:for-each>
    </ul>
  </xsl:template>
  
  <xsl:template name="section_detail">
    <xsl:for-each select="texts/text">
      <div style="padding:10px" data-os-entity="{@entity}">        
        <a id="doc_{@entity}" href="{@view_href}">
          <h1>
            <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
          </h1>
        </a>
        <!--<div style="clear:both;" class="os_message_body">-->
        <!--<div class="os_message_body">
          <xsl:value-of select="system:parse(@content)" disable-output-escaping="yes"/>
        </div>-->
        <xsl:value-of select="system:parse(@content, true(), false(), false(), 0, @entity)" disable-output-escaping="yes"/>
        <div style="text-align:right;">
          <a href="#" onclick="Osiris.scrollTo('doc_toc');return false;">
            <img src="{system:resource-url('images/up.gif')}"/>
          </a>
        </div>
      </div>
    </xsl:for-each>
    <xsl:for-each select="sections/section">
      <div style="padding:10px" data-os-entity="{@entity}">
        <a id="doc_{@entity}" href="{@view_href}">
          <h1>
            <xsl:value-of select="system:parse(@title, false(), false(), true())" disable-output-escaping="yes"/>
          </h1>
        </a>
        <xsl:call-template name="section_detail"/>
      </div>
    </xsl:for-each>
  </xsl:template>
  
  <!--
  <xsl:template match="documentation">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@title"/>
      <xsl:with-param name="content">
        <div class="os_box" style="float:right;">
          <xsl:call-template name="section_index"/>
        </div>
        <br/>        
        <xsl:call-template name="section_detail">
          <xsl:with-param name="level" select="'0'"/>
        </xsl:call-template>
      </xsl:with-param>
    </xsl:call-template>    
  </xsl:template>

  
  
  <xsl:template name="section_detail">
    <xsl:param name="level"/>
    <ol>
      <xsl:for-each select="texts/text">
        <li data-os-entity="{@entity}" style="border-left:1px solid #EA8115;border-bottom:1px solid #EA8115;padding:5px;margin:5px;">
          <a href="{@view_href}">
            <h1>
              <xsl:value-of select="@title"/>
            </h1>
          </a>
          <xsl:choose>
            <xsl:when test="$level &lt; 1">
              <div style="clear:both;" class="os_message_body">
                <xsl:value-of select="system:parse(@content)" disable-output-escaping="yes"/>
              </div>
            </xsl:when>
            <xsl:otherwise>
              <xsl:text>[...]</xsl:text>
            </xsl:otherwise>
          </xsl:choose>
        </li>
      </xsl:for-each>
      <xsl:for-each select="sections/section">
        <li data-os-entity="{@entity}" style="border-left:1px solid #EA8115;border-bottom:1px solid #EA8115;padding:5px;margin:5px;">
          <a href="{@view_href}">            
            <h1>
              <xsl:value-of select="@title"/>
            </h1>
          </a>
          <xsl:if test="$level &lt; 1">
            <xsl:call-template name="section_detail">
              <xsl:with-param name="level" select="$level+1"/>
            </xsl:call-template>
          </xsl:if>
        </li>
      </xsl:for-each>
    </ol>   
  </xsl:template>
  -->

</xsl:stylesheet>