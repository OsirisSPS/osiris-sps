<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template name="bgcolor">
    <xsl:param name="color"/>

    <a style="display:block;float:right;margin:5px;padding:5px;background-color:{$color};border:1px solid gray;width:24px;height:16px;">
      <xsl:attribute name="href">
        <xsl:text>javascript:void(osGetById("images").style.backgroundColor='</xsl:text>
        <xsl:value-of select="$color"/>
        <xsl:text>');</xsl:text>
      </xsl:attribute>
      <xsl:text> </xsl:text>
    </a>
  </xsl:template>

  <xsl:template match="/">
    
    <style>
      <xsl:text>
      .filebox
      {
        float:left;
        /*
        width: 150px;
        height: 150px;
        */
        border:1px solid gray;
        margin:3px;
        padding:3px;
        text-align:center;
      }
      .fileboxtitle
      {
        display:block;
        width:100%;
        font-size:0.8em;
        text-align:center;
        background:blue;
        color:white;
        margin:2px;
        padding:2px;
      }
      </xsl:text>
    </style>
    <xsl:call-template name="bgcolor">
      <xsl:with-param name="color" select="'transparent'"/>
    </xsl:call-template>
    <xsl:call-template name="bgcolor">
      <xsl:with-param name="color" select="'black'"/>
    </xsl:call-template>
    <xsl:call-template name="bgcolor">
      <xsl:with-param name="color" select="'white'"/>
    </xsl:call-template>
    <xsl:call-template name="bgcolor">
      <xsl:with-param name="color" select="'#888888'"/>
    </xsl:call-template>
    <xsl:call-template name="bgcolor">
      <xsl:with-param name="color" select="'#CCCCCC'"/>
    </xsl:call-template>
    <xsl:call-template name="bgcolor">
      <xsl:with-param name="color" select="'#FF0000'"/>
    </xsl:call-template>
    <xsl:call-template name="bgcolor">
      <xsl:with-param name="color" select="'#0000FF'"/>
    </xsl:call-template>
    <div style="clear:both;" id="images">
      <xsl:apply-templates select="*"/>
    </div>
  </xsl:template>

  <xsl:template match="folder">
    <xsl:call-template name="block_minimal">
      <xsl:with-param name="title" select="@name" />
      <xsl:with-param name="content">
        <xsl:apply-templates select="folder"/>
        <xsl:apply-templates select="file"/>
        <div style="clear:both"/>
      </xsl:with-param>
    </xsl:call-template>    
  </xsl:template>

  <xsl:template match="file">
    <div class="filebox">
      <a class="fileboxtitle">
        <xsl:attribute name="href">
          <xsl:call-template name="copy-paste-link">
            <xsl:with-param name="text" select="@osiris"/>
          </xsl:call-template>
        </xsl:attribute>
        <xsl:value-of select="@name"/>                
      </a>

      <img src="{system:resource-url(@path)}"/>
      
    </div>
    <xsl:apply-templates select="*"/>
  </xsl:template>

  
</xsl:stylesheet>