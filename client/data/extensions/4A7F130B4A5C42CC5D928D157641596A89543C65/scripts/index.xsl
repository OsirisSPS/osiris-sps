<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os"
                exclude-result-prefixes="lang date system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/actions.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/utils.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/help.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:template match="/index">
    <div class="os_content">
      <xsl:call-template name="help-box">
        <xsl:with-param name="text">
          <xsl:value-of select="lang:text('main.pages.developer_tools.help')"/>
        </xsl:with-param>
      </xsl:call-template>
      
      <xsl:call-template name="block_big">
        <xsl:with-param name="title">
          <xsl:value-of select="lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.group.python')"/>          
        </xsl:with-param>
        <xsl:with-param name="content">
          <xsl:call-template name="action-row">
            <xsl:with-param name="prefix" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65'"/>
            <xsl:with-param name="icon_path" select="'/4A7F130B4A5C42CC5D928D157641596A89543C65/images/icons/32x32/pythondoc.png'"/>
            <xsl:with-param name="name" select="'pythoninfo'"/>
            <xsl:with-param name="href" select="'/developer_tools/pythoninfo'"/>
          </xsl:call-template>
        <xsl:call-template name="action-row">
            <xsl:with-param name="prefix" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65'"/>
            <xsl:with-param name="icon_path" select="'/4A7F130B4A5C42CC5D928D157641596A89543C65/images/icons/32x32/pythondoc.png'"/>
            <xsl:with-param name="name" select="'pythondoc'"/>
            <xsl:with-param name="href" select="'/developer_tools/pythondoc'"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>

      <xsl:call-template name="block_big">
        <xsl:with-param name="title">
          <xsl:value-of select="lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.group.resources')"/>
        </xsl:with-param>
        <xsl:with-param name="content">
          <xsl:call-template name="action-row">
            <xsl:with-param name="prefix" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65'"/>
            <xsl:with-param name="icon_path" select="'/4A7F130B4A5C42CC5D928D157641596A89543C65/images/icons/32x32/images.png'"/>
            <xsl:with-param name="name" select="'images'"/>
            <xsl:with-param name="href" select="'/developer_tools/images'"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>

      <xsl:call-template name="block_big">
        <xsl:with-param name="title">
          <xsl:value-of select="lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.group.testing_pages')"/>
        </xsl:with-param>
        <xsl:with-param name="content">
          <xsl:call-template name="action-row">
            <xsl:with-param name="prefix" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65'"/>
            <xsl:with-param name="icon_path" select="'/4A7F130B4A5C42CC5D928D157641596A89543C65/images/icons/32x32/oml.png'"/>
            <xsl:with-param name="name" select="'omltest'"/>
            <xsl:with-param name="href" select="'/developer_tools/omltest'"/>
          </xsl:call-template>
          <xsl:call-template name="action-row">
            <xsl:with-param name="prefix" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65'"/>
            <xsl:with-param name="icon_path" select="'/4A7F130B4A5C42CC5D928D157641596A89543C65/images/icons/32x32/html.png'"/>
            <xsl:with-param name="name" select="'htmltest'"/>
            <xsl:with-param name="href" select="'/developer_tools/htmltest'"/>
          </xsl:call-template>
          <xsl:call-template name="action-row">
            <xsl:with-param name="prefix" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65'"/>
            <xsl:with-param name="icon_path" select="'/4A7F130B4A5C42CC5D928D157641596A89543C65/images/icons/32x32/regex.png'"/>
            <xsl:with-param name="name" select="'regextest'"/>
            <xsl:with-param name="href" select="'/developer_tools/regextest'"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>

      <xsl:call-template name="block_big">
        <xsl:with-param name="title">
          <xsl:value-of select="lang:text('extensions.4A7F130B4A5C42CC5D928D157641596A89543C65.group.testsuite_pages')"/>
        </xsl:with-param>
        <xsl:with-param name="content">
          <xsl:call-template name="action-row">
            <xsl:with-param name="prefix" select="'extensions.4A7F130B4A5C42CC5D928D157641596A89543C65'"/>
            <xsl:with-param name="icon_path" select="'/4A7F130B4A5C42CC5D928D157641596A89543C65/images/icons/32x32/testsuite.png'"/>
            <xsl:with-param name="name" select="'testsuite'"/>
            <xsl:with-param name="href" select="'/developer_tools/testsuite'"/>
          </xsl:call-template>
        </xsl:with-param>
      </xsl:call-template>
      
    </div>
    
  </xsl:template>

  
</xsl:stylesheet>