<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                exclude-result-prefixes="os lang date system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>

  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="body"/>
  
  <xsl:template match="block">    
    <xsl:choose>            
			<!--
      <xsl:when test="@page">
        <xsl:call-template name="block_page">
          <xsl:with-param name="prefix" select="@title"/>
          <xsl:with-param name="body" select="$body"/>
        </xsl:call-template>
      </xsl:when>
			-->
      <xsl:when test="@type='none'">
        <xsl:call-template name="block_none">
          <xsl:with-param name="style" select="@style"/>
          <xsl:with-param name="body" select="$body"/>
        </xsl:call-template>
      </xsl:when>
			<!--
      <xsl:when test="@type='heading'">
        <xsl:call-template name="block_heading">
          <xsl:with-param name="title" select="@title"/>
          <xsl:with-param name="href" select="@href"/>
          <xsl:with-param name="innerStyle" select="@innerStyle"/>
          <xsl:with-param name="outerStyle" select="@outerStyle"/>
          <xsl:with-param name="body" select="$body"/>
        </xsl:call-template>
      </xsl:when>
			-->
      <xsl:when test="@type='minimal'">
        <xsl:call-template name="block_small">
          <xsl:with-param name="title" select="@title"/>
          <xsl:with-param name="href" select="@href"/>
          <xsl:with-param name="innerStyle" select="@innerStyle"/>
          <xsl:with-param name="outerStyle" select="@outerStyle"/>
          <xsl:with-param name="startClose" select="@startClose"/>
          <xsl:with-param name="body" select="$body"/>          
        </xsl:call-template>
      </xsl:when>			
      <xsl:when test="@type='small'">
        <xsl:call-template name="block_small">
          <xsl:with-param name="title" select="@title"/>
          <xsl:with-param name="href" select="@href"/>
          <xsl:with-param name="innerStyle" select="@innerStyle"/>
          <xsl:with-param name="outerStyle" select="@outerStyle"/>
          <xsl:with-param name="body" select="$body"/>
        </xsl:call-template>
      </xsl:when>
      <xsl:otherwise>
        <xsl:call-template name="block_big">
          <xsl:with-param name="title" select="@title"/>
          <xsl:with-param name="href" select="@href"/>
          <xsl:with-param name="innerStyle" select="@innerStyle"/>
          <xsl:with-param name="outerStyle" select="@outerStyle"/>
          <xsl:with-param name="startClose" select="@startClose"/>
          <xsl:with-param name="body" select="$body"/>
        </xsl:call-template>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
</xsl:stylesheet>