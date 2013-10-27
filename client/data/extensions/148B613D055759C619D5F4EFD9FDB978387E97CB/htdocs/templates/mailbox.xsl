<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:lang="http://www.osiris-sps.org/xsl/lang" 
                xmlns:date="http://www.osiris-sps.org/xsl/date" 
                xmlns:system="http://www.osiris-sps.org/xsl/system" 
                xmlns:os="http://www.osiris-sps.org/xhtml/os" 
                exclude-result-prefixes="date lang system os"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/user.xsl"/>
  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/pager.xsl"/>
  
  <xsl:output method="html" indent="yes" omit-xml-declaration="yes"/>

  <xsl:param name="secure"/>

  <xsl:template name="actions">
    <div class="os_commands_right" align="right">
      <xsl:for-each select="actions/action">
        <xsl:if test="position() != 1">
          <xsl:text> </xsl:text>
        </xsl:if>
        <a class = "os_button" href="{@href}">
          <xsl:value-of select="lang:text(concat('mailbox.', ../../@name, '.actions.', @name))"/>
        </a>
      </xsl:for-each>
    </div>
    <xsl:if test="pager">
      <div class="os_commands_left">
        <xsl:apply-templates select="pager"/>
      </div>
    </xsl:if>
    <div class="os_clear"/>
  </xsl:template>
  
  <xsl:template match="folder">

    <xsl:call-template name="actions"/>
    
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text(concat('mailbox.', @name, '.name'))"/>      
      <xsl:with-param name="content">
        <xsl:call-template name="folder"/>
      </xsl:with-param>
    </xsl:call-template>

    <xsl:call-template name="actions"/>
        
  </xsl:template>

  <xsl:template match="message">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="@subject"/>
      <xsl:with-param name="content">
        <xsl:call-template name="message"/>
      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>
  
  <xsl:template name="folder">    
    <table class="os_table" cellspacing="0">
      <tr>
        <th width="0%"/>
        <th width="40%">
          <xsl:value-of select="lang:text('mailbox.inbox.subject')"/>
        </th>
        <th width="30%" align="center">
          <xsl:value-of select="lang:text('mailbox.inbox.sender')"/>
        </th>
        <th width="30%" align="center">
          <xsl:value-of select="lang:text('mailbox.inbox.date')"/>
        </th>
      </tr>
      <xsl:for-each select="messages/message">
        <tr>
          <td valign="middle" align="center">
            <img src="{system:resource-url(concat('images/pm_', @status, '.gif'))}"/>
          </td>
          <td>
            <a href="{@href}">
              <xsl:value-of select="@subject"/>
            </a>
          </td>
          <td align="center">
            <xsl:apply-templates select="author" mode="row"/>
          </td>
          <td align="center">
            <xsl:value-of select="date:long-datetime(@submit_date)"/>
          </td>
        </tr>
      </xsl:for-each>
    </table>  
  </xsl:template>
  
  <xsl:template name="message">
    <table class="os_table" cellspacing="0">
      <tr>
        <td>
          <xsl:call-template name="authors"/>
        </td>
        <td style="width:100%;" colspan="2">
          <div>            
            <xsl:value-of select="lang:text('object.submit_date')"/>
            <xsl:text> </xsl:text>
            <xsl:value-of select="date:long-datetime(@submit_date)"/>
          </div>
          <div style="padding-top:10px;">
            <xsl:value-of select="@body" disable-output-escaping="yes"/>
            <xsl:if test="author/@mark">
              <xsl:call-template name="mark">
                <xsl:with-param name="mark" select="author/@mark"/>
                <xsl:with-param name="secure" select="$secure"/>
              </xsl:call-template>
            </xsl:if>
          </div>
        </td>
      </tr>
      <tr>
        <td>
          <xsl:apply-templates select="author/actions" mode="user"/>
        </td>
        <td>
          <a href="javascript:scroll(0,0);">
            <img src = "{system:resource-url('images/up.gif')}"/>
          </a>
        </td>
        <td class="os_commands_right">          
          <xsl:for-each select="actions/action">            
            <a class = "os_button" href="{@href}">
              <xsl:value-of select="lang:text(concat('mailbox.message.actions.', @name))"/>
            </a>
          </xsl:for-each>
        </td>
      </tr>
    </table>
  </xsl:template>
  
</xsl:stylesheet>

