<?xml version='1.0' encoding='utf-8'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:lang="http://www.osiris-sps.org/xsl/lang"
                xmlns:date="http://www.osiris-sps.org/xsl/date"
                xmlns:system="http://www.osiris-sps.org/xsl/system"
                exclude-result-prefixes="lang date system"
                version="1.0">

  <xsl:import href="http://www.osiris-sps.org/htdocs/templates/includes/block.xsl"/>

  <xsl:output method="html"/>

  <xsl:template match="page">
    <xsl:call-template name="block">
      <xsl:with-param name="title" select="lang:text('main.pages.bbobject.title')"/>
      <xsl:with-param name="content">
        <div id="redirect" style="display:none;text-align:center;padding:30px;">
          <img src="{system:resource-url('images/bbobject_redirect.gif')}"/>
          <br/>
          <a id="skip_link" href="{@url}">
            <xsl:value-of select="lang:text('main.pages.bbobject.skip_link')"/>
          </a>
        </div>
        <script>
          <![CDATA[
        function doStartRedirect()
        {
          var divWarning = osGetById("warning");
          if(divWarning)
          divWarning.style.display = "none";
          osGetById("redirect").style.display = "block";        
          timerID = setTimeout("doRedirect()", 3000);
        }
        function doRedirect()
        {
          window.location = osGetById("skip_link").href;       
        }
        ]]>
        </script>
        <xsl:choose>
          <xsl:when test="@confirm='true'">
            <div id="warning" style="text-align:center;padding:30px;">
              <img src="{system:resource-url('images/bbobject_confirm.gif')}"/>
              <br/>
              <div style="padding:5px;margin:5px;border:1px solid gray;">
                <xsl:value-of select="lang:text('main.pages.bbobject.warning')"/>
                <br/>
                <br/>
                <xsl:choose>
                  <xsl:when test="@name = 'type'">
                    <span class="os_label">
                      <xsl:value-of select="lang:text('oml.object.messages.warning.type')"/>
                      <xsl:text>: </xsl:text>                      
                    </span>
                    <span class="os_value">
                      <xsl:value-of select="@value"/>
                    </span>
                  </xsl:when>
                  <xsl:when test="@name = 'domain'">
                    <span class="os_label">
                      <xsl:value-of select="lang:text('oml.object.messages.warning.url')"/>
                      <xsl:text>: </xsl:text>
                    </span>
                    <span class="os_value">
                      <xsl:value-of select="@value"/>
                    </span>
                  </xsl:when>
                </xsl:choose>

              </div>
              <a href="javascript:doStartRedirect()">
                <xsl:value-of select="lang:text('main.pages.bbobject.confirm_link')"/>
              </a>
              <xsl:text> - </xsl:text>
              <a href="javascript:history.back()">
                <xsl:value-of select="lang:text('main.pages.bbobject.back_link')"/>
              </a>
            </div>
          </xsl:when>
          <xsl:otherwise>
            <script>
              <![CDATA[
            doStartRedirect();
            ]]>
            </script>
          </xsl:otherwise>
        </xsl:choose>

      </xsl:with-param>
    </xsl:call-template>
  </xsl:template>

</xsl:stylesheet>